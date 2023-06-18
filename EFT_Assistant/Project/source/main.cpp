#include"Header.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetWindowStyleMode(2);
	SetUseBackBufferTransColorFlag(TRUE);
	OPTION::Create();
	DXDraw::Create("EFT Assistant", 950, 950 * 9 / 16);	//�ėp
	EffectResource::Create();
	PostPassEffect::Create();							//�V�F�[�_�[
	//SoundPool::Create();								//�T�E���h
	FontPool::Create();									//�t�H���g
#ifdef DEBUG
	DebugClass::Create();
	auto* DebugParts = DebugClass::Instance();			//�f�o�b�O
#endif // DEBUG
	auto* DrawParts = DXDraw::Instance();
	//�p�[�c
	FPS_n2::PlayerData::Create();
	FPS_n2::InputControl::Create();
	FPS_n2::DataErrorLog::Create();
	FPS_n2::DrawControl::Create();
	FPS_n2::InterruptParts::Create();
	FPS_n2::WindowSystem::WindowManager::Create();
	//�V�[��
	auto MAINLOOPloader = std::make_shared<FPS_n2::MAINLOOPLOADER>();

	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//�V�[���R���g���[��
	auto NowScene = std::make_unique<SceneControl>(MAINLOOPloader);
	//�J�ڐ�w��
	MAINLOOPloader->Set_Next(MAINLOOPscene);
	MAINLOOPscene->Set_Next(MAINLOOPloader);

	//�J��Ԃ�
	while (true) {
		NowScene->StartScene();
		while (true) {
			if (ProcessMessage() != 0) { return 0; }
			FPS = GetFPS();
			clsDx();
#ifdef DEBUG
			DebugParts->SetStartPoint();
#endif // DEBUG
			if (NowScene->Execute()) { break; }			//�X�V
			NowScene->Draw();							//�`��
#ifdef DEBUG
			//�f�o�b�O���
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();					//��ʂ̔��f
		}
		if (NowScene->isEnd()) {
			break;
		}
		NowScene->NextScene();							//���̃V�[���ֈڍs
	}
	return 0;
}
