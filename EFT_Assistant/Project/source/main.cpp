#include"Header.hpp"
#include <WinUser.h>


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetWindowStyleMode(2);
	SetUseBackBufferTransColorFlag(TRUE);
	OPTION::Create();
	DXDraw::Create("EFT_Assistant",960,540);		//汎用
	PostPassEffect::Create();						//シェーダー
	//EffectResource::Create();						//エフェクト
	//SoundPool::Create();							//サウンド
	FontPool::Create();								//フォント
#ifdef DEBUG
	DebugClass::Create();
	auto* DebugParts = DebugClass::Instance();		//デバッグ
#endif // DEBUG
	auto* DrawParts = DXDraw::Instance();
	//
	//auto* EffectUseControl = EffectResource::Instance();
	//シーン
	auto MAINLOOPloader = std::make_shared<FPS_n2::MAINLOOPLOADER>();
	
	auto MAINLOOPscene = std::make_shared<FPS_n2::MAINLOOP>();
	//シーンコントロール
	auto scene = std::make_unique<SceneControl>(MAINLOOPloader);
	//遷移先指定
	MAINLOOPloader->Set_Next(MAINLOOPscene);
	MAINLOOPscene->Set_Next(MAINLOOPloader);

	//繰り返し
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
			if (scene->Execute()) { break; }		//更新
			if (!scene->isPause()) {
				//EffectUseControl->Calc();			//エフェクシアのアプデを60FPS相当に変更
			}

			scene->Draw();							//描画
			//デバッグ
#ifdef DEBUG
			DebugParts->DebugWindow(50, 50);
#endif // DEBUG
			DrawParts->Screen_Flip();				//画面の反映
		}
		if (scene->isEnd()) {
			break;
		}
		scene->NextScene();							//次のシーンへ移行
	}
	return 0;
}
