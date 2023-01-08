#include"Header.hpp"
#include <WinUser.h>


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetWindowStyleMode(2);
	SetUseBackBufferTransColorFlag(TRUE);
	OPTION::Create();
	DXDraw::Create("EFT_Assistant",960,540);		//�ėp
	PostPassEffect::Create();						//�V�F�[�_�[
	//EffectResource::Create();						//�G�t�F�N�g
	//SoundPool::Create();							//�T�E���h
	FontPool::Create();								//�t�H���g
#ifdef DEBUG
	DebugClass::Create();
	auto* DebugParts = DebugClass::Instance();		//�f�o�b�O
#endif // DEBUG
	auto* DrawParts = DXDraw::Instance();
	//
	//auto* EffectUseControl = EffectResource::Instance();
	//�V�[��
	auto MAINLOOPloader = std::make_shared<FPS_n2::MAINLOOPLOADER>();
	
	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//�V�[���R���g���[��
	auto scene = std::make_unique<SceneControl>(MAINLOOPloader);
	//�J�ڐ�w��
	MAINLOOPloader->Set_Next(MAINLOOPscene);
	MAINLOOPscene->Set_Next(MAINLOOPloader);

	//�J��Ԃ�
	while (true) {
		scene->StartScene();
		while (true) {
			if ((ProcessMessage() != 0)
				//|| (CheckHitKeyWithCheck(KEY_INPUT_ESCAPE) != 0)
				) {
				return 0;
			}
			FPS = GetFPS();
			clsDx();
#ifdef DEBUG
			DebugParts->SetStartPoint();
#endif // DEBUG
			if (scene->Execute()) { break; }		//�X�V
			if (!scene->isPause()) {
				//EffectUseControl->Calc();			//�G�t�F�N�V�A�̃A�v�f��60FPS�����ɕύX
			}

			scene->Draw();							//�`��
			//�f�o�b�O
#ifdef DEBUG
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();				//��ʂ̔��f
		}
		if (scene->isEnd()) {
			break;
		}
		scene->NextScene();							//���̃V�[���ֈڍs
	}
	return 0;
}
