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
		bool														m_IsPullDown{ false };
		float														m_PullDown{ 1.f };
		//
		bool														m_Loading{ false };
		//
		bool														m_WindowMove{ false };
	public:
		void Load_Sub(void) noexcept override {
			//パーツ
			PlayerData::Create();
			InputControl::Create();
			DataErrorLog::Create();
			DrawControl::Create();
			InterruptParts::Create();
			WindowSystem::WindowManager::Create();
			//データベース
			DataBase::Create();
		}
		void Set_Sub(void) noexcept override {
			bool DrawLog = false;
#ifdef DEBUG
			DrawLog = true;
#endif
			//
			DataBase::Instance()->SetDataList();
			DataBase::Instance()->WaitDataList();
			DataBase::Instance()->LoadList(DrawLog);
			m_Loading = true;
			//
			BGBase::Create();

			BGBase::Instance()->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
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
				//SetWindowPosition(-960, 0);//0,0
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
					m_WindowMove = ((m_PullDown >= 1.f) && in2_(Input->GetMouseX(), Input->GetMouseY(), 0, 0, y_r(1920), LineHeight));
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
					m_IsPullDown = true;
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

				Easing(&m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
				if (m_PullDown >= 0.95f) { this->m_PullDown = 1.f; }
				if (m_PullDown <= 0.05f) { this->m_PullDown = 0.f; }
				int Xwin = (int)(Lerp((float)Xmin, (float)DrawParts->m_DispXSize, this->m_PullDown));
				int Ywin = (int)(Lerp((float)Ymin, (float)DrawParts->m_DispYSize, this->m_PullDown));
				int DieCol = std::clamp((int)(Lerp(1.f, 128.f, this->m_NoneActiveTimes / 5.f)), 0, 255);

				DrawControl::Instance()->SetDrawBox(DrawLayer::BackGround, 0, 0, Xwin, Ywin, Gray75, TRUE);
				if (m_PullDown >= 1.f) {
					//Back
					BGs->Draw_Back(this->m_posx, this->m_posy, this->m_Scale);
					WindowMngr->Draw();
				}
				WindowSystem::SetBox(0, 0, Xwin, LineHeight, GetColor(DieCol, DieCol, DieCol));				//タイトルバック
				//展開
				if (WindowSystem::ClickCheckBox(0, 0, Xmin, Ymin, false, true, Gray25, !m_IsPullDown ? "折りたたむ" : "展開")) { this->m_IsPullDown ^= 1; }
				if (m_PullDown >= 1.f) {
					//タイトル
					if (WindowSystem::ClickCheckBox(Xmin + y_r(10), 0, Xmin + y_r(230), LineHeight, false, true, Gray25, "全窓を閉じる")) { WindowMngr->DeleteAll(); }
					WindowSystem::SetMsg(0, 0, DrawParts->m_DispXSize, Ymin, LineHeight, STRX_MID, White, Black, "EFT Assistant");
					WindowSystem::SetMsg(y_r(1280), LineHeight * 1 / 10, y_r(1280), LineHeight, LineHeight * 8 / 10, STRX_LEFT, White, Black, "ver %d.%d.%d", 0, 3, 2);
					WindowSystem::SetMsg(0, Ymin + LineHeight * 1 / 10, DrawParts->m_DispXSize, Ymin + LineHeight, LineHeight * 8 / 10, STRX_MID, White, Black, "最終更新:%s", PlayerData::Instance()->GetLastDataReceive().c_str());
					if (WindowSystem::CloseButton(DrawParts->m_DispXSize - Ymin, 0)) { SetisEnd(true); }
					//Front
					BGs->DrawFront(this->m_posx, this->m_posy, this->m_Scale);
					DrawControl::Instance()->SetDrawCircle(DrawLayer::Front, DrawParts->m_DispXSize, DrawParts->m_DispYSize, y_r(100), TransColor, TRUE);					//中央位置回避のための小円
				}
				DataErrorLog::Instance()->Draw();
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
