#include"Header.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetWindowStyleMode(2);
	SetUseBackBufferTransColorFlag(TRUE);
	OPTION::Create();
	//DXDraw::Create("EFT Assistant", 950, 950 * 9 / 16);	//汎用
	DXDraw::Create("EFT Assistant", 1920, 720);	//汎用
	EffectResource::Create();
	PostPassEffect::Create();							//シェーダー
	//SoundPool::Create();								//サウンド
	FontPool::Create();									//フォント
#ifdef DEBUG
	DebugClass::Create();
	auto* DebugParts = DebugClass::Instance();			//デバッグ
#endif // DEBUG
	auto* DrawParts = DXDraw::Instance();

	while(false)
	{
		//
		auto SubStrs = [&](std::string* line) {
			std::vector<std::string> Ret;
			while (true) {
				auto Start = line->find("\"");
				if (Start != std::string::npos) {
					auto Startp = Start + strlenDx("\"");
					auto End = line->find("\"", Startp);
					if (End != std::string::npos) {
						Ret.emplace_back(line->substr(Startp, End - Startp));
						*line = line->substr(0, Start) + line->substr(End + strlenDx("\""));
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			return Ret;
		};
		auto GetStrInTag = [&](std::string* line, const char* Left, const char* Right) {
			auto Start = line->find(Left);
			if (Start != std::string::npos) {
				auto Ofs = Start + strlenDx(Left);
				auto End = line->find(Right, Ofs);
				if (End != std::string::npos) {
					*line = line->substr(Ofs, End - Ofs);
					return true;
				}
			}
			return false;
		};
		auto SubComment = [&](std::string* line) {
			while (true) {
				std::string line_t = *line;
				if (GetStrInTag(&line_t, "<!--", "-->")) {					//メタ
					if (line_t != "") {
						FPS_n2::SubStrs(line, line_t.c_str());
					}
				}
				else {
					break;
				}
				FPS_n2::SubStrs(line, "<!---->");
			}
		};
		//
		auto ReplaceTags = [&](std::string* line, const char* Tag, const char* Type1, const char* AfterStart, const char* AfterEnd) {
			bool isHit = false;
			bool isEnd = true;

			std::string Left_Start = "<";	Left_Start += Tag;
			std::string Total_Start = Left_Start; Total_Start += ">";

			std::string Left_End = "</";	Left_End += Tag;
			std::string Total_End = Left_End; Total_End += ">";

			std::string line_ret = *line;

			std::string line_p = line_ret;
			while (true) {
				std::string line_t = line_p;
				if (GetStrInTag(&line_t, Left_Start.c_str(), ">")) {
					isHit = true;
					isEnd = false;
					if (line_t != "") {
						FPS_n2::SubStrs(&line_p, line_t.c_str());
					}
					FPS_n2::ReplaceRIGHTStrs(&line_p, Total_Start.c_str(), AfterStart);
					if (FPS_n2::ReplaceRIGHTStrs(&line_p, Total_End.c_str(), AfterEnd)) {
						isEnd = true;
					}
					if (Type1 != nullptr) {
						if (Type1 == "") {
							line_ret = line_p;
						}
						else {
							if ((line_t != "") && (line_t.find(Type1) != std::string::npos)) {
								line_ret = line_p;
							}
						}
					}
					else {
						//何も書いてない場合だけ
						if (line_t == "") {
							line_ret = line_p;
						}
						else {

						}
					}
				}
				else {
					break;
				}
			}

			*line = line_ret;
			return isEnd;
		};
		auto ReplaceTags_P = [&](std::vector<std::string>& lines_before, int now, const char* Tag, const char* Type1, const char* AfterStart, const char* AfterEnd) {
			std::string Left_End = "</";	Left_End += Tag;
			std::string Total_End = Left_End; Total_End += ">";
			auto& line = lines_before[now];
			if (!ReplaceTags(&line, Tag, Type1, AfterStart, AfterEnd)) {//todo
				for (int k = now + 1; k < lines_before.size(); k++) {
					auto& linek = lines_before[k];
					if (FPS_n2::ReplaceRIGHTStrs(&linek, Total_End.c_str(), AfterEnd)) {
						break;
					}
				}
			}
		};
		auto ReplaceImageTags_P = [&](std::vector<std::string>& lines_before, int now) {
			while (true) {
				std::string line_t = lines_before[now];
				if (GetStrInTag(&line_t, "<img", ">")) {
					bool isHit = false;
					if (line_t != "") {
						FPS_n2::SubStrs(&lines_before[now], line_t.c_str());
						std::string FileName = "";
						std::string FileWidth = "";
						std::string FileHeight = "";
						{
							std::string line_t2 = line_t;
							if (GetStrInTag(&line_t2, " alt=\"", "\" ")) {					//メタ
								if (line_t2 != "") {
									FileName = line_t2;
									isHit = true;
								}
							}
						}
						{
							std::string line_t2 = line_t;
							if (GetStrInTag(&line_t2, " width=\"", "\" ")) {					//メタ
								if (line_t2 != "") {
									FileWidth = line_t2;
									isHit = true;
								}
							}
						}
						{
							std::string line_t2 = line_t;
							if (GetStrInTag(&line_t2, " height=\"", "\" ")) {					//メタ
								if (line_t2 != "") {
									FileHeight = line_t2;
									isHit = true;
								}
							}
						}
						if (isHit) {
							line_t = "&ref(img/" + FileName + "," + FileWidth + "x" + FileHeight + ");";
							FPS_n2::ReplaceRIGHTStrs(&lines_before[now], "<img>", line_t.c_str());
						}
					}
					if (!isHit) {
						FPS_n2::ReplaceRIGHTStrs(&lines_before[now], "<img>", "&ref();");
					}
				}
				else {
					break;
				}
			}
		};
		//
		auto SpaceOnlyCheck = [&](std::string* line) {
			if (*line == "") { return true; }					//もう空白しかねえ
			FPS_n2::SubStrs(line, "	");							//タグけし
			if (*line == "") { return true; }					//もう空白しかねえ
			{													//もう空白しかねえ
				std::string line_t = *line;
				FPS_n2::SubStrs(&line_t, " ");
				if (line_t == "") { return true; }
			}
			return false;
		};
		//
		auto SubLeftSpace = [&](std::string* line) {
			while (true) {
				if ((*line)[0] == ' ') {
					*line = line->substr(1);
				}
				else {
					break;
				}
			}
		};
		//
		std::vector<std::string> lines_before;
		std::vector<std::string> lines;
		bool isStart = false;
		bool isScript = false;
		std::array<int, 32> count;
		for (auto& c : count) { c = 0; }
		{
			std::string filename = "WikiData/input/item/key_html/Factory emergency exit keyのソース - Escape from Tarkov Wiki_.html";
			std::ifstream File(FPS_n2::UTF8toSjis(filename));
			std::string line;
			while (std::getline(File, line)) {
				//これらのキーワードがハイライトされています～コメント
				{
					if (!isStart) {
						auto Start = line.find("<code>");
						if (Start != std::string::npos) {
							isStart = true;
							line = line.substr(Start+strlenDx("<code>"));
						}
						else {
							continue;
						}
					}
					else {
						auto Start = line.find("</code>");
						if (Start != std::string::npos) {
							isStart = false;
							continue;
						}
					}
				}
				lines.emplace_back(line);
			}
			File.close();
		}
		{
			std::ofstream outputfile("WikiData/input/item/key_html/tmp.txt");
			for (auto& L : lines) {
				outputfile << L + "\n";
			}
			outputfile.close();
		}
		SetLogFontSize(10);
		setPrintColorDx(GetColor(0, 255, 0), GetColor(0, 64, 0));
		int yofs = 0;
		int ysize = 70;
		while (ProcessMessage() == 0) {
			FPS = GetFPS();
			clsDx();
			GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
			{
				DrawBox(0, 0, y_r(1920) + 1, y_r(1080) + 1, GetColor(1, 1, 1), TRUE);//背景透明

				yofs = std::clamp(yofs - GetMouseWheelRotVolWithCheck()*ysize, 0, std::max((int)lines.size() - ysize, 0));
				int i = 0;
				for (auto& L : lines) {
					if (i >= yofs) {
						printfDx("%s\n", L.c_str());
					}
					i++; if (i >= yofs + ysize) { break; }
				}
			}
#ifdef DEBUG
			//デバッグ画面
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();					//画面の反映
		}
		return 0;
	}

	//パーツ
	FPS_n2::PlayerData::Create();
	FPS_n2::InputControl::Create();
	FPS_n2::DataErrorLog::Create();
	FPS_n2::DrawControl::Create();
	FPS_n2::InterruptParts::Create();
	FPS_n2::WindowSystem::WindowManager::Create();
	//シーン
	auto MAINLOOPloader = std::make_shared<FPS_n2::MAINLOOPLOADER>();

	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//シーンコントロール
	auto NowScene = std::make_unique<SceneControl>(MAINLOOPloader);
	//遷移先指定
	MAINLOOPloader->Set_Next(MAINLOOPscene);
	MAINLOOPscene->Set_Next(MAINLOOPloader);

	//繰り返し
	while (true) {
		NowScene->StartScene();
		while (true) {
			if (ProcessMessage() != 0) { return 0; }
			FPS = GetFPS();
			clsDx();
#ifdef DEBUG
			DebugParts->SetStartPoint();
#endif // DEBUG
			if (NowScene->Execute()) { break; }			//更新
			NowScene->Draw();							//描画
#ifdef DEBUG
			//デバッグ画面
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();					//画面の反映
		}
		if (NowScene->isEnd()) {
			break;
		}
		NowScene->NextScene();							//次のシーンへ移行
	}
	return 0;
}
