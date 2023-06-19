#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	class MAINLOOP : public TEMPSCENE {
	private:
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		float														m_Scale{ 0.6f };
		//
		float														m_NoneActiveTimes{ 0.f };

		bool														m_IsPull{ false };

		bool														m_IsPullRight{ false };
		bool														m_IsPullDown{ false };
		float														m_PullRight{ 1.f };
		float														m_PullDown{ 1.f };
		//
		bool														m_Loading{ false };
		//
		bool														m_WindowMove{ false };
		bool														m_DrawLog{ false };
	private:
		bool IsOpen() const noexcept { return(this->m_PullDown >= 1.f); }
	public:
		void Load_Sub(void) noexcept override {
		}
		void Set_Sub(void) noexcept override {
			BGBase::Create();
			BGBase::Instance()->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
			m_Loading = true;
		}
		bool Update_Sub(void) noexcept override {
			auto* DataBases = DataBase::Instance();
			auto* BGs = BGBase::Instance();
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* Input = InputControl::Instance();
			auto* InterParts = InterruptParts::Instance();

			DrawControl::Instance()->ClearList();

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
			auto mouse_moveX = Input->GetMouseX();							//ドラッグ前のマウス座標格納
			auto mouse_moveY = Input->GetMouseY();
			Input->Execute();
			mouse_moveX = Input->GetMouseX() - mouse_moveX;
			mouse_moveY = Input->GetMouseY() - mouse_moveY;
			//ドラッグ開始時の処理
			if (Input->GetMiddleClick().press()) {
				if (Input->GetMiddleClick().trigger()) {
					m_WindowMove = (IsOpen() && in2_(Input->GetMouseX(), Input->GetMouseY(), 0, 0, y_r(1920), LineHeight));
				}
				if (m_WindowMove) {
					int start_windowX = 0, start_windowY = 0;
					GetWindowPosition(&start_windowX, &start_windowY);			//ウィンドウの位置を格納
					start_windowX += mouse_moveX;
					start_windowY += mouse_moveY;
					SetWindowPosition(start_windowX, start_windowY);			//マウス位置の差を算出し、ウィンドウを動かす
					Input->SetMouse();
					HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
					SetCursor(hCursor);
				}
			}
			//
			if ((Input->GetWheelAdd() != 0) && !WindowMngr->PosHitCheck(nullptr) && Input->GetScaleActive()) {
				auto PrevScale = this->m_Scale;
				this->m_Scale = std::clamp(this->m_Scale + (float)Input->GetWheelAdd() / 10.f, 0.1f, 2.f);
				auto ScaleChange = (this->m_Scale - PrevScale);
				if (ScaleChange != 0.f) {
					this->m_posx -= (int)((float)(Input->GetMouseX() - this->m_posx) * ScaleChange / this->m_Scale);
					this->m_posy -= (int)((float)(Input->GetMouseY() - this->m_posy) * ScaleChange / this->m_Scale);
				}
			}
			Input->SetScaleActive(true);
			if ((Input->GetRightClick().press()) && !WindowMngr->PosHitCheck(nullptr)) {
				this->m_posx += mouse_moveX;
				this->m_posy += mouse_moveY;
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			//
			WindowMngr->Execute();
			BGs->LateExecute(&this->m_posx, &this->m_posy, &this->m_Scale);
			//
			if (BGs->GetNowBG()->IsGoNextBG()) {
				WindowMngr->DeleteAll();
				BGs->Dispose();
				BGs->SetNext((BGs->GetNowBG() == BGs->GetTitleBG()) ? BGs->GetTitleBG()->GetNextSelect() : BGSelect::Title);
				BGs->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
			}
			//
			if (InterParts->IsActive()) {
				WindowMngr->DeleteAll();
				BGs->Dispose();
				BGs->SetNext(InterParts->GetNextScene());
				BGs->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
				if (InterParts->GetNextScene() == BGSelect::Custom) {
					BGs->GetCustomBG()->SetSubparam(
						InterParts->GetInitParam(0),//アイテム名
						InterParts->GetInitParam(1)//プリセット名
					);
				}
				InterParts->Complete();
			}
			if (!GetWindowActiveFlag()) {
				if (m_NoneActiveTimes > 0.f) {
					m_NoneActiveTimes -= 1.f / FPS;
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
				int Xmin = y_r(320);
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

				int Xwin = (int)(Lerp((float)Xmin, (float)DrawParts->m_DispXSize, this->m_PullRight));
				int Ywin = (int)(Lerp((float)Ymin, (float)DrawParts->m_DispYSize, this->m_PullDown));
				int DieCol = std::clamp((int)(Lerp(1.f, 128.f, this->m_NoneActiveTimes / 5.f)), 0, 255);

				DrawControl::Instance()->SetDrawBox(DrawLayer::BackGround, 0, 0, Xwin, Ywin, Gray75, TRUE);
				if (IsOpen()) {
					//Back
					BGs->Draw_Back(this->m_posx, this->m_posy, this->m_Scale);
					WindowMngr->Draw();
				}
				WindowSystem::SetBox(0, 0, Xwin, LineHeight, GetColor(DieCol, DieCol, DieCol));				//タイトルバック
				//展開
				if (WindowSystem::ClickCheckBox(0, 0, Xmin, Ymin, false, true, Gray25, !m_IsPull ? "折りたたむ" : "展開")) { this->m_IsPull ^= 1; }
				if (IsOpen()) {
					//タイトル
					if (WindowSystem::ClickCheckBox(Xmin + y_r(10), 0, Xmin + y_r(230), LineHeight, false, true, Gray25, "全窓を閉じる")) { WindowMngr->DeleteAll(); }
					if (WindowSystem::ClickCheckBox(Xmin + y_r(240), 0, Xmin + y_r(460), LineHeight, false, true, Gray25, "ログ表示")) { m_DrawLog ^= 1; }
					WindowSystem::SetMsg(0, 0, DrawParts->m_DispXSize, Ymin, LineHeight, STRX_MID, White, Black, "EFT Assistant");
					WindowSystem::SetMsg(y_r(1280), LineHeight * 1 / 10, y_r(1280), LineHeight, LineHeight * 8 / 10, STRX_LEFT, White, Black, "ver %d.%d.%d", 0, 3, 6);
					WindowSystem::SetMsg(0, Ymin + LineHeight * 1 / 10, DrawParts->m_DispXSize, Ymin + LineHeight, LineHeight * 8 / 10, STRX_MID, White, Black, "最終更新:%s", PlayerData::Instance()->GetLastDataReceive().c_str());
					if (WindowSystem::CloseButton(DrawParts->m_DispXSize - Ymin, 0)) { SetisEnd(true); }
					//Front
					BGs->DrawFront(this->m_posx, this->m_posy, this->m_Scale);
					DrawControl::Instance()->SetDrawCircle(DrawLayer::Front, DrawParts->m_DispXSize, DrawParts->m_DispYSize, y_r(100), TransColor, TRUE);					//中央位置回避のための小円
				}
				if (m_DrawLog) {
					DataErrorLog::Instance()->Draw();
				}
				if (GetASyncLoadNum() > 0) {
					WindowSystem::SetMsg(0, DrawParts->m_DispYSize - LineHeight, y_r(0), DrawParts->m_DispYSize, LineHeight, STRX_LEFT, White, Black, "Loading...");
				}
			}
			return true;
		}
		void Dispose_Sub(void) noexcept override {
			PlayerData::Instance()->Save();
			DataErrorLog::Instance()->Save();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}
		void DrawUI_In_Sub(void) noexcept  override {
			DrawControl::Instance()->Draw();
		}
	};
};
