#include"Header.hpp"
#include <wininet.h>
#include "Parts/WindowParts.hpp"
#include "Parts/StrControl.hpp"
#include "Parts/DrawSystem.hpp"
#include "Parts/InputParts.hpp"
#include "Parts/PlayerDataParts.hpp"
#include "Parts/InterruptParts.hpp"

class MainClass {
private:
public:
	void Init() {
		SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
		SetWindowStyleMode(2);
		SetUseBackBufferTransColorFlag(TRUE);

		DXDraw::Create();	//�ėp//950, 950 * 9 / 16
		SetMainWindowText("EFT Assistant");
		PostPassEffect::Create();							//�V�F�[�_�[
		DXDraw::Instance()->PauseIn();
		//�p�[�c
		FPS_n2::PlayerData::Create();
		FPS_n2::InputControl::Create();
		FPS_n2::DataErrorLog::Create();
		FPS_n2::DrawControl::Create();
		FPS_n2::InterruptParts::Create();
		FPS_n2::WindowSystem::WindowManager::Create();
	}

	bool SceneExecute(std::unique_ptr<SceneControl>& NowScene) {
#ifdef DEBUG
		auto* DebugParts = DebugClass::Instance();			//�f�o�b�O
#endif // DEBUG
		auto* DrawParts = DXDraw::Instance();
#ifdef DEBUG
		clsDx();
		DebugParts->SetStartPoint();
#endif // DEBUG
		if (NowScene->Execute()) { return false; }			//�X�V
		NowScene->Draw();							//�`��
#ifdef DEBUG
			//�f�o�b�O���
		DebugParts->DebugWindow(50, 50);
#endif // DEBUG
		DrawParts->Screen_Flip();					//��ʂ̔��f
		return true;
	}
};




















int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	MainClass mainClass;

	mainClass.Init();
#if false
	{
#ifdef DEBUG
		auto* DebugParts = DebugClass::Instance();			//�f�o�b�O
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
						char lpBuffer[1024 + 1]; //��x�ɓǂݍ��ރo�C�g��
						DWORD dwBytesRead;     //�ǂݍ���byte��
						InternetReadFile(hService, lpBuffer, 1024, &dwBytesRead);
						if (dwBytesRead == 0) break; //�ǂݍ���byte����0�ɂȂ����烋�[�v�𔲂���
						lpBuffer[dwBytesRead] = 0; //�ǂݍ��񂾃f�[�^�̍Ō��0���������ݕ�����

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
			FPS = GetFPS();
			clsDx();
			GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
			{
				DrawBox(0, 0, y_r(1920) + 1, y_r(1080) + 1, GetColor(1, 1, 1), TRUE);//�w�i����

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
			//�f�o�b�O���
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();					//��ʂ̔��f
		}
		return 0;
	}

	{
#ifdef DEBUG
		auto* DebugParts = DebugClass::Instance();			//�f�o�b�O
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
			FPS = GetFPS();
			clsDx();
			GraphHandle::SetDraw_Screen((int32_t)(DX_SCREEN_BACK), true);
			{
				DrawBox(0, 0, y_r(1920) + 1, y_r(1080) + 1, GetColor(1, 1, 1), TRUE);//�w�i����

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
			//�f�o�b�O���
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();					//��ʂ̔��f
		}
		return 0;
	}
#else
	//�V�[���쐬
	auto MAINLOOPloader = std::make_shared<FPS_n2::MAINLOOPLOADER>();
	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//�J�ڐ�w��
	MAINLOOPloader->Set_Next(MAINLOOPscene);
	MAINLOOPscene->Set_Next(MAINLOOPloader);

	//�V�[���R���g���[��
	auto NowScene = std::make_unique<SceneControl>(MAINLOOPloader);
	//�J��Ԃ�
	while (true) {
		NowScene->StartScene();
		while (true) {
			if (ProcessMessage() != 0) { return 0; }
			FPS = GetFPS();
			if (!mainClass.SceneExecute(NowScene)) { break; }
		}
		if (NowScene->isEnd()) {
			break;
		}
		NowScene->NextScene();							//���̃V�[���ֈڍs
	}
#endif
	return 0;
}
