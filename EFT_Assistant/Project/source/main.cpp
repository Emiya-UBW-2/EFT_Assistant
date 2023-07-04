#include"Header.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetWindowStyleMode(2);
	SetUseBackBufferTransColorFlag(TRUE);
	OPTION::Create();
	DXDraw::Create("EFT Assistant", 950, 950 * 9 / 16);	//汎用
	//DXDraw::Create("EFT Assistant", 1920, 720);	//汎用
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
		auto GetTagStr = [&](std::string* line, const char* Left, const char* Right) {
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
		auto SubTag = [&](std::string* line, const char* Left, const char* Right) {
			while (true) {
				std::string line_t = *line;
				if (GetTagStr(&line_t, Left, Right)) {					//メタ
					if (line_t != "") {
						FPS_n2::SubStrs(line, line_t.c_str());
					}
				}
				else {
					break;
				}
				std::string Total = Left;
				Total += Right;
				FPS_n2::SubStrs(line, Total.c_str());
			}
		};
		auto ReplaceTag = [&](std::string* line, const char* Left, const char* Right, const char* After) {
			while (true) {
				std::string line_t = *line;
				if (GetTagStr(&line_t, Left, Right)) {					//メタ
					if (line_t != "") {
						FPS_n2::SubStrs(line, line_t.c_str());
					}
				}
				else {
					break;
				}
				std::string Total = Left;
				Total += Right;
				FPS_n2::ReplaceStrs(line, Total.c_str(), After);
			}
		};
		auto ReplaceTags = [&](std::string* line, const char* Tag, const char* Type1, const char* After) {
			std::string Left = "<";	Left += Tag;
			std::string Total = Left; Total += ">";


			std::string line_p = *line;
			while (true) {
				std::string line_t = line_p;
				if (GetTagStr(&line_t, Left.c_str(), ">")) {					//メタ
					if (line_t != "") {
						FPS_n2::SubStrs(&line_p, line_t.c_str());
						if (Type1 != "") {
							if (line_t.find(Type1) != std::string::npos) {
								*line = line_p;
							}
							else {

							}
						}
						else {
							*line = line_p;
						}
					}
				}
				else {
					break;
				}
				FPS_n2::ReplaceStrs(&line_p, Total.c_str(), After);
				FPS_n2::ReplaceStrs(line, Total.c_str(), After);
			}
		};
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

		auto SubStrs = [&](std::string* line) {
			std::vector<std::string> Ret;
			while (true) {
				auto Start = line->find("\"");
				if (Start != std::string::npos) {
					auto End = line->find("\"",Start + strlenDx("\""));
					if (End != std::string::npos) {
						Ret.emplace_back(line->substr(Start + strlenDx("\""), End));
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
			return;
		};

		std::vector<std::string> lines;
		std::ifstream File("WikiData/input/item/key_html/Factory emergency exit key - Escape from Tarkov Wiki_.html");
		std::string line;
		bool isStart = false;
		bool isScript = false;
		while (std::getline(File, line)) {
			//これらのキーワードがハイライトされています～コメント
			{
				if (!isStart) {
					auto Start = line.find("これらのキーワードがハイライトされています");
					if (Start != std::string::npos) {
						isStart = true;
					}
					else {
						continue;
					}
				}
				else {
					auto Start = line.find("コメント");
					if (Start != std::string::npos) {
						isStart = false;
						continue;
					}
				}
			}
			//スクリプトをスルー
			{
				if (!isScript) {
					auto Start = line.find("<script>");
					if (Start != std::string::npos) {
						isScript = true;
						continue;
					}
				}
				else {
					auto Start = line.find("</script>");
					if (Start != std::string::npos) {
						isScript = false;
					}
					continue;
				}
			}
			{
				SubTag(&line, "<!--", "-->");		//コメント消す
				//SubTag(&line, "\"", "\"");
				//FPS_n2::SubStrs(&line, "\"");
				//FPS_n2::SubStrs(&line, "\/");
			}
			if (SpaceOnlyCheck(&line)) { continue; }
			{
				SubStrs(&line);

				//ReplaceTags(&line, "a", "class", "");
				//ReplaceTags(&line, "a", "href", "");
				ReplaceTags(&line, "a", "", "");			FPS_n2::SubStrs(&line, "</a>");

				ReplaceTags(&line, "br", "", "");
				ReplaceTags(&line, "b", "", "");			FPS_n2::SubStrs(&line, "</b>");

				ReplaceTags(&line, "div", "", "");			FPS_n2::SubStrs(&line, "</div>");
				ReplaceTags(&line, "hr", "", "");
				ReplaceTags(&line, "h1", "", "");			FPS_n2::SubStrs(&line, "</h1>");
				ReplaceTags(&line, "h2", "", "");			FPS_n2::SubStrs(&line, "</h2>");
				ReplaceTags(&line, "h3", "", "");			FPS_n2::SubStrs(&line, "</h3>");
				ReplaceTags(&line, "h4", "", "");			FPS_n2::SubStrs(&line, "</h4>");
				ReplaceTags(&line, "h5", "", "");			FPS_n2::SubStrs(&line, "</h5>");
				ReplaceTags(&line, "h6", "", "");			FPS_n2::SubStrs(&line, "</h6>");
				//ReplaceTags(&line, "img", "", "");
				//ReplaceTags(&line, "i", "", "");			FPS_n2::SubStrs(&line, "</i>");
				ReplaceTags(&line, "meta", "", "");
				ReplaceTags(&line, "path", "", "");			FPS_n2::SubStrs(&line, "</path>");
				ReplaceTags(&line, "p", "", "");			FPS_n2::SubStrs(&line, "</p>");
				ReplaceTags(&line, "span", "", "");			FPS_n2::SubStrs(&line, "</span>");
				ReplaceTags(&line, "strong", "", "");		FPS_n2::SubStrs(&line, "</strong>");
				ReplaceTags(&line, "svg", "", "");			FPS_n2::SubStrs(&line, "</svg>");
				ReplaceTags(&line, "s", "", "");			FPS_n2::SubStrs(&line, "</s>");
				ReplaceTags(&line, "ul", "", "");			FPS_n2::SubStrs(&line, "</ul>");

				ReplaceTags(&line, "table", "","[");				FPS_n2::ReplaceStrs(&line, "</table>", "]");
				ReplaceTags(&line, "thead", "", "[");				FPS_n2::ReplaceStrs(&line, "</thead>", "]");
				ReplaceTags(&line, "tbody", "", "[");				FPS_n2::ReplaceStrs(&line, "</tbody>", "]");
				ReplaceTags(&line, "tr", "", "/[");				FPS_n2::ReplaceStrs(&line, "</tr>", "]/");
				ReplaceTags(&line, "td", "", "[");				FPS_n2::ReplaceStrs(&line, "</td>", "]");
				ReplaceTags(&line, "li", "", "[");				FPS_n2::ReplaceStrs(&line, "</li>", "]");
			}
			if (SpaceOnlyCheck(&line)) { continue; }

			lines.emplace_back(line);
		}
		File.close();

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
