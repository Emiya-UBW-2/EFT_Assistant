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
		//
		std::vector<std::string> lines;
		{
			std::string filename = "WikiData/input/item/key_html/Factory emergency exit key.html";
			std::ifstream File(FPS_n2::UTF8toSjis(filename));
			std::string line;
			bool isStart = false;
			while (std::getline(File, line)) {
				if (!isStart) {
					auto Start = line.find("<code>");
					if (Start != std::string::npos) {
						isStart = true;
						line = line.substr(Start + strlenDx("<code>"));
					}
				}
				else {
					auto Start = line.find("</code>");
					if (Start != std::string::npos) {
						isStart = false;
					}
				}
				if (isStart) {
					lines.emplace_back(line);
				}
			}
			File.close();
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
