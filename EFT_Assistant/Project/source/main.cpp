#include "Header.hpp"
#include "PartsHeader.hpp"
//
#include "Scene/MainSceneLoader.hpp"
#include "Scene/MainScene.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	//SetWindowStyleMode(2);
	//SetUseBackBufferTransColorFlag(TRUE);
	SetDoubleStartValidFlag(TRUE);
	SetEnableXAudioFlag(TRUE);//Xaudio(ロードが長いとロストするので必要に応じて)
	DXLib_ref::Create();
	//使用するボタンの指定
	auto* Pad = PadControl::Instance();
	Pad->SetIsUseButton(PADS::MOVE_W, true);
	Pad->SetIsUseButton(PADS::MOVE_A, true);
	Pad->SetIsUseButton(PADS::MOVE_S, true);
	Pad->SetIsUseButton(PADS::MOVE_D, true);
	Pad->SetIsUseButton(PADS::MOVE_STICK, true);
	Pad->SetIsUseButton(PADS::DIR_UP, true);
	Pad->SetIsUseButton(PADS::DIR_DOWN, true);
	Pad->SetIsUseButton(PADS::DIR_LEFT, true);
	Pad->SetIsUseButton(PADS::DIR_RIGHT, true);
	Pad->SetIsUseButton(PADS::DIR_STICK, true);
	Pad->SetIsUseButton(PADS::LEAN_L, true);//UIのみ
	Pad->SetIsUseButton(PADS::LEAN_R, true);//UIのみ
	Pad->SetIsUseButton(PADS::RELOAD, true);//UIのみ
	Pad->SetIsUseButton(PADS::INTERACT, true);//UIのみ
	Pad->SetIsUseButton(PADS::THROW, true);
	Pad->SetIsUseButton(PADS::MELEE, true);
	Pad->SetIsUseButton(PADS::JUMP, true);
	Pad->SetIsUseButton(PADS::INVENTORY, true);
	Pad->SetIsUseButton(PADS::RUN, true);
	Pad->SetIsUseButton(PADS::WALK, true);
	Pad->SetIsUseButton(PADS::SHOT, true);
	Pad->SetIsUseButton(PADS::AIM, true);
	Pad->SetIsUseButton(PADS::ULT, true);
	Pad->SetIsUseButton(PADS::SQUAT, true);
	Pad->SetIsUseButton(PADS::PRONE, true);
	Pad->SetIsUseButton(PADS::CHECK, true);
	//
	auto* DXLib_refParts = DXLib_ref::Instance();
	if (!DXLib_refParts->StartLogic()) { return 0; }
	//追加設定
	SetMainWindowText("EFT Assistant");
	//SetUseHalfLambertLighting(TRUE);
	//MV1SetLoadModelReMakeNormal(TRUE);

	//パーツ
	FPS_n2::PlayerData::Create();
	FPS_n2::DataErrorLog::Create();
	FPS_n2::DrawGraphs::Create();
	FPS_n2::InterruptParts::Create();
	FPS_n2::WindowMySystem::WindowManager::Create();

#if false
	{
#ifdef DEBUG
		auto* DebugParts = DebugClass::Instance();			//デバッグ
#endif // DEBUG
		auto* DrawParts = DXDraw::Instance();
		//

		std::vector<std::string> lines_buf;
		std::string lines_bufs;
		std::vector<std::string> lines;

		while (true) {
			lines_buf.clear();
			lines_bufs = "";
			std::string Url = "https://wikiwiki.jp/eft/?cmd=source&page=Factory emergency exit key";
			HINTERNET hSession;
			hSession = InternetOpen("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
			if (hSession)
			{
				HINTERNET hService;
				hService = InternetOpenUrl(hSession, Url.c_str(), NULL, 0, 0, 0);
				if (hService) {
					while (true) {
						char lpBuffer[1024 + 1]; //一度に読み込むバイト数
						DWORD dwBytesRead;     //読み込んだbyte数
						InternetReadFile(hService, lpBuffer, 1024, &dwBytesRead);
						if (dwBytesRead == 0) break; //読み込んだbyte数が0になったらループを抜ける
						lpBuffer[dwBytesRead] = 0; //読み込んだデータの最後に0を書き込み文字列化

						lines_bufs += lpBuffer;

						while (true) {
							auto CR_LF = lines_bufs.find("\n");
							if (CR_LF != std::string::npos) {
								lines_buf.emplace_back(lines_bufs.substr(0, CR_LF));
								lines_bufs = lines_bufs.substr(CR_LF + strlenDx("\n"));
							}
							else {
								break;
							}
						}
					}
					if (lines_bufs != "") {
						while (true) {
							auto CR_LF = lines_bufs.find("\n");
							if (CR_LF != std::string::npos) {
								lines_buf.emplace_back(lines_bufs.substr(0, CR_LF));
								lines_bufs = lines_bufs.substr(CR_LF + strlenDx("\n"));
							}
							else {
								break;
							}
						}
					}
				}
				InternetCloseHandle(hService);
			}
			InternetCloseHandle(hSession);

			lines.clear();
			bool isStart = false;
			for (auto& line : lines_buf) {
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
		}
		SetLogFontSize(10);
		setPrintColorDx(GetColor(0, 255, 0), GetColor(0, 64, 0));
		int yofs = 0;
		int ysize = 20;
		while (ProcessMessage() == 0) {
			clsDx();
			GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
			{
				DrawBox(0, 0, DXDraw::Instance()->GetUIY(1920) + 1, DXDraw::Instance()->GetUIY(1080) + 1, GetColor(1, 1, 1), TRUE);//背景透明

				yofs = std::clamp(yofs - GetMouseWheelRotVolWithCheck()*ysize, 0, std::max((int)lines.size() - ysize, 0));
				int i = -yofs;
				for (auto& L : lines) {
					if (i >= 0) {
						printfDx("%s\n", L.c_str());
					}
					i++;
					if (i >= ysize) {
						break;
					}
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

	{
#ifdef DEBUG
		auto* DebugParts = DebugClass::Instance();			//デバッグ
#endif // DEBUG
		auto* DrawParts = DXDraw::Instance();
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
			clsDx();
			GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
			{
				DrawBox(0, 0, DXDraw::Instance()->GetUIY(1920) + 1, DXDraw::Instance()->GetUIY(1080) + 1, GetColor(1, 1, 1), TRUE);//背景透明

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
#else
	//シーン作成
	auto MAINLOOPloader = std::make_shared<FPS_n2::MAINLOOPLOADER>();
	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//遷移先指定
	MAINLOOPloader->SetNextSceneList(0, MAINLOOPscene);
	MAINLOOPscene->SetNextSceneList(0, MAINLOOPloader);

	auto* SceneParts = SceneControl::Instance();
	SceneParts->AddList(MAINLOOPloader);
	SceneParts->AddList(MAINLOOPscene);
	//最初の読み込み
	if (!DXLib_refParts->MainLogic()) { return 0; }
#endif
	return 0;
}
