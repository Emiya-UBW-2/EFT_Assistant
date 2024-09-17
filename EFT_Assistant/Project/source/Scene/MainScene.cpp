#include "MainScene.hpp"
#include "MainScene/Data/MainScene_Base.hpp"
#include "MainScene/Page/MainScene_PageManager.hpp"

#include "MainScene/Page/MainScene_PageCustom.hpp"

namespace FPS_n2 {
	void MAINLOOP::Set_Sub(void) noexcept {
		PageManager::Create();
		PageManager::Instance()->Init();
		m_Loading = true;
	}
	bool MAINLOOP::Update_Sub(void) noexcept {
		auto* DataBases = DataBase::Instance();
		auto* PageMngr = PageManager::Instance();
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* Pad = PadControl::Instance();
		auto* InterParts = InterruptParts::Instance();

		if (m_Loading) {
			DataBases->WhenAfterLoadListCommon();
			if (GetASyncLoadNum() == 0) {
				DataBases->WhenAfterLoadList();
				m_Loading = false;
			}
		}
		//FirstDoing
		if (GetIsFirstLoop()) {
			SetWindowPosition(0, 0);//0,0
		}
		//ドラッグ開始時の処理
		if (Pad->GetKey(PADS::ULT).press()) {
			if (Pad->GetKey(PADS::ULT).trigger()) {
				m_WindowMove = (IsOpen() && IntoMouse(0, 0, DXDraw::Instance()->GetUIY(1920), LineHeight));
			}
			if (m_WindowMove) {
				int start_windowX = 0, start_windowY = 0;
				GetWindowPosition(&start_windowX, &start_windowY);			//ウィンドウの位置を格納
				start_windowX += static_cast<int>(Pad->GetLS_X());
				start_windowY += static_cast<int>(Pad->GetLS_Y());
				SetWindowPosition(start_windowX, start_windowY);			//マウス位置の差を算出し、ウィンドウを動かす
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
		}
		//
		PageMngr->FirstExecute();
		WindowMngr->Execute();
		PageMngr->LateExecute();
		//割り込み出のページ切替
		if (InterParts->IsActive()) {
			PageMngr->ChangePage(InterParts->GetNextScene());
			if (InterParts->GetNextScene() == BGSelect::Custom) {
				((std::shared_ptr<CustomBG>&)PageMngr->GetPage(BGSelect::Custom))->SetSubparam(
					InterParts->GetInitParam(0),//アイテム名
					InterParts->GetInitParam(1)//プリセット名
				);
			}
			InterParts->Complete();
		}
		if (!GetWindowActiveFlag()) {
			if (m_NoneActiveTimes > 0.f) {
				m_NoneActiveTimes -= 1.f / DrawParts->GetFps();
			}
			else {
				m_NoneActiveTimes = 0.f;
				m_IsPull = true;
			}
		}
		else {
			m_NoneActiveTimes = 5.f;
		}
		//m_NoneActiveTimes = 5.f;
		//SetDraw
		{
			int Xmin = DXDraw::Instance()->GetUIY(320);
			int Ymin = LineHeight;

			if (!m_IsPull) {
				m_IsPullRight = false;
			}
			else {
				m_IsPullDown = true;
			}
			Easing(&this->m_PullRight, !m_IsPullRight ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
			if (this->m_PullRight >= 0.99f) { this->m_PullRight = 1.f; }
			Easing(&this->m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
			if (this->m_PullDown >= 0.95f) { this->m_PullDown = 1.f; }

			if (this->m_PullRight >= 0.99f) { if (!m_IsPull) { m_IsPullDown = false; } }
			if (this->m_PullDown <= 0.01f) {
				if (!m_IsPullRight) {
					int start_windowX = 0, start_windowY = 0;
					GetWindowPosition(&start_windowX, &start_windowY);			//ウィンドウの位置を格納
					start_windowX = std::clamp(start_windowX, 0, 1920);
					start_windowY = std::clamp(start_windowY, 0, 1080 - 100);
					SetWindowPosition(start_windowX, start_windowY);			//ウィンドウを動かす
				}

				if (m_IsPull) { m_IsPullRight = true; }
			}

			int Xwin = (int)(Lerp((float)Xmin, (float)DrawParts->GetUIY(1920), this->m_PullRight));
			int Ywin = (int)(Lerp((float)Ymin, (float)DrawParts->GetScreenY(1080), this->m_PullDown));
			int DieCol = std::clamp((int)(Lerp(1.f, 128.f, this->m_NoneActiveTimes / 5.f)), 0, 255);

			WindowSystem::DrawControl::Instance()->SetDrawBox(WindowSystem::DrawLayer::BackGround, 0, 0, Xwin, Ywin, Gray75, TRUE);
			if (IsOpen()) {
				//Back
				PageMngr->Draw_Back();
				WindowMngr->Draw();
			}
			WindowSystem::SetBox(0, 0, Xwin, LineHeight, GetColor(DieCol, DieCol, DieCol));				//タイトルバック
			//展開
			if (WindowSystem::SetMsgClickBox(0, 0, Xmin, Ymin, Ymin, Gray25, false, true, !m_IsPull ? "折りたたむ" : "展開")) { this->m_IsPull ^= 1; }
			if (IsOpen()) {
				//タイトル
				if (WindowSystem::SetMsgClickBox(Xmin + DXDraw::Instance()->GetUIY(10), 0, Xmin + DXDraw::Instance()->GetUIY(230), LineHeight, LineHeight, Gray25, false, true, "全窓を閉じる")) { WindowMngr->DeleteAll(); }
				if (WindowSystem::SetMsgClickBox(Xmin + DXDraw::Instance()->GetUIY(240), 0, Xmin + DXDraw::Instance()->GetUIY(460), LineHeight, LineHeight, Gray25, false, true, "ログ表示")) { m_DrawLog ^= 1; }
				WindowSystem::SetMsg(DrawParts->GetUIY(1920) / 2, Ymin / 2, LineHeight, STRX_MID, White, Black, "EFT Assistant");
				WindowSystem::SetMsg(DXDraw::Instance()->GetUIY(1280), LineHeight * 11 / 20, LineHeight * 8 / 10, STRX_LEFT, White, Black, "ver %d.%d.%d", 0, 3, 7);
				WindowSystem::SetMsg(DrawParts->GetUIY(1920) / 2, Ymin + LineHeight * 11 / 20, LineHeight * 8 / 10, STRX_MID, White, Black, "最終更新:%s", PlayerData::Instance()->GetLastDataReceive().c_str());
				if (WindowMySystem::CloseButton(DrawParts->GetUIY(1920) - Ymin, 0)) {
					PlayerData::Instance()->Save();
					DataErrorLog::Instance()->Save();
				}
				//Front
				PageMngr->DrawFront();
				WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, DrawParts->GetUIY(1920), DrawParts->GetScreenY(1080), DXDraw::Instance()->GetUIY(100), TransColor, TRUE);					//中央位置回避のための小円
			}
			if (m_DrawLog) {
				DataErrorLog::Instance()->Draw();
			}
			if (GetASyncLoadNum() > 0) {
				WindowSystem::SetMsg(DXDraw::Instance()->GetUIY(0), DrawParts->GetScreenY(1080) - LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "Loading...");
			}
		}
		return true;
	}
	void MAINLOOP::Dispose_Sub(void) noexcept {
		PlayerData::Instance()->Save();
		DataErrorLog::Instance()->Save();
	}
};
