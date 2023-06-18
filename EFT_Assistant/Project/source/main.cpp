#include"Header.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);
	SetWindowStyleMode(2);
	SetUseBackBufferTransColorFlag(TRUE);
	OPTION::Create();
	DXDraw::Create("EFT Assistant", 950, 950 * 9 / 16);	//汎用
	EffectResource::Create();
	PostPassEffect::Create();							//シェーダー
	//SoundPool::Create();								//サウンド
	FontPool::Create();									//フォント
#ifdef DEBUG
	DebugClass::Create();
	auto* DebugParts = DebugClass::Instance();			//デバッグ
#endif // DEBUG
	auto* DrawParts = DXDraw::Instance();
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
