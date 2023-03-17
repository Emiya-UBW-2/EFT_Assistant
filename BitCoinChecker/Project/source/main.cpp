#include"Header.hpp"
#include <WinUser.h>


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	OPTION::Create();
	DXDraw::Create("BitCoinChecker", 30*16, 30 * 9);	//�ėp
	PostPassEffect::Create();							//�V�F�[�_�[
	//SoundPool::Create();								//�T�E���h
	FontPool::Create();									//�t�H���g
#ifdef DEBUG
	DebugClass::Create();
	auto* DebugParts = DebugClass::Instance();			//�f�o�b�O
#endif // DEBUG
	auto* DrawParts = DXDraw::Instance();
	//�V�[��
	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//�V�[���R���g���[��
	auto NowScene = std::make_unique<SceneControl>(MAINLOOPscene);
	//�J�ڐ�w��
	MAINLOOPscene->Set_Next(MAINLOOPscene);

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
