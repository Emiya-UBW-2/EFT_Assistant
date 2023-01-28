#pragma once
#include"../Header.hpp"

//Datas
#include "MainScene/Data/MainScene_Parents.hpp"
#include "MainScene/Data/MainScene_Map.hpp"
#include "MainScene/Data/MainScene_ItemCategory.hpp"
#include "MainScene/Data/MainScene_ItemType.hpp"
#include "MainScene/Data/MainScene_Item.hpp"
#include "MainScene/Data/MainScene_Enemy.hpp"
#include "MainScene/Data/MainScene_Trader.hpp"
#include "MainScene/Data/MainScene_Task.hpp"
//Pages
#include "MainScene/Page/MainScene_PageParents.hpp"
#include "MainScene/Page/MainScene_PageTitle.hpp"
#include "MainScene/Page/MainScene_PageTask.hpp"
#include "MainScene/Page/MainScene_PageHideout.hpp"
#include "MainScene/Page/MainScene_PageItem.hpp"
#include "MainScene/Page/MainScene_PageMap.hpp"

namespace FPS_n2 {
	class MAINLOOP : public TEMPSCENE {
	private:
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		float														m_Scale{ 0.6f };
		bool														m_IsPullDown{ false };
		float														m_PullDown{ 1.f };

		std::shared_ptr<BGParent>									m_BGPtr;
		std::shared_ptr<TitleBG>									m_TitleBG;
		std::shared_ptr<TaskBG>										m_TaskBG;
		std::shared_ptr<HideOutBG>									m_HideOutBG;
		std::shared_ptr<ItemBG>										m_ItemBG;
		std::shared_ptr<MapBG>										m_MapBG;

		bool														m_Loading{ false };

		bool														m_WindowMove{ false };

		float														m_NoneActiveTimes{ 0.f };
	public:
		void Load_Sub(void) noexcept override {}

		void Set_Sub(void) noexcept override {
			//
			InputControl::Create();
			DataErrorLog::Create();
			DrawControl::Create();
			WindowSystem::WindowManager::Create();
			//
			ItemData::Create();
			EnemyData::Create();
			TraderData::Create();
			MapData::Create();
			TaskData::Create();

			SetUseASyncLoadFlag(TRUE);
			ItemData::Instance()->LoadList();
			EnemyData::Instance()->LoadList();
			TraderData::Instance()->LoadList();
			MapData::Instance()->LoadList();
			TaskData::Instance()->LoadList();
			SetUseASyncLoadFlag(FALSE);
			m_Loading = true;
			//
			m_TitleBG = std::make_shared<TitleBG>();
			m_TaskBG = std::make_shared<TaskBG>();
			m_HideOutBG = std::make_shared<HideOutBG>();
			m_ItemBG = std::make_shared<ItemBG>();
			m_MapBG = std::make_shared<MapBG>();
			//
			m_BGPtr = m_TitleBG;
			//
			m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
		}

		bool Update_Sub(void) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* Input = InputControl::Instance();

			DrawControl::Instance()->ClearList();

			if (m_Loading) {
				if (GetASyncLoadNum() == 0) {
					ItemData::Instance()->WhenAfterLoadList();
					EnemyData::Instance()->WhenAfterLoadList();
					TraderData::Instance()->WhenAfterLoadList();
					MapData::Instance()->WhenAfterLoadList();
					TaskData::Instance()->WhenAfterLoadList();
					m_Loading = false;
				}
			}
			//FirstDoing
			if (GetIsFirstLoop()) {
				SetWindowPosition(0, 0);
			}
			auto mouse_moveX = Input->GetMouseX();							//ドラッグ前のマウス座標格納
			auto mouse_moveY = Input->GetMouseY();
			Input->Execute();
			mouse_moveX = Input->GetMouseX() - mouse_moveX;
			mouse_moveY = Input->GetMouseY() - mouse_moveY;
			//ドラッグ開始時の処理
			bool inMouse = in2_(Input->GetMouseX(), Input->GetMouseY(), 0, 0, y_r(1920), LineHeight);
			if (Input->GetMiddleClick().press()) {
				if (Input->GetMiddleClick().trigger()) {
					m_WindowMove = false;
					if (m_PullDown >= 1.f) {
						if (inMouse) {
							m_WindowMove = true;
						}
					}
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
			if (Input->GetWheelAdd() != 0) {
				auto PrevScale = this->m_Scale;
				this->m_Scale = std::clamp(this->m_Scale + (float)Input->GetWheelAdd() / 10.f, 0.1f, 2.f);
				auto ScaleChange = (this->m_Scale - PrevScale);
				if (ScaleChange != 0.f) {
					this->m_posx -= (int)((float)(Input->GetMouseX() - this->m_posx) * ScaleChange / this->m_Scale);
					this->m_posy -= (int)((float)(Input->GetMouseY() - this->m_posy) * ScaleChange / this->m_Scale);
				}
			}
			if (Input->GetRightClick().press()) {
				this->m_posx += mouse_moveX;
				this->m_posy += mouse_moveY;
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			//
			WindowMngr->Execute();
			m_BGPtr->LateExecute(&this->m_posx, &this->m_posy, &this->m_Scale);
			if (m_BGPtr->IsGoNextBG()) {
				WindowMngr->DeleteAll();
				m_BGPtr->Dispose();
				if (m_BGPtr == m_TitleBG) {
					switch (m_TitleBG->GetNextSelect()) {
					case BGSelect::Task:
						m_BGPtr = m_TaskBG;
						break;
					case BGSelect::HideOut:
						m_BGPtr = m_HideOutBG;
						break;
					case BGSelect::Item:
						m_BGPtr = m_ItemBG;
						break;
					case BGSelect::Map:
						m_BGPtr = m_MapBG;
						break;
					default:
						m_BGPtr = m_TaskBG;
						break;
					}
				}
				else {
					m_BGPtr = m_TitleBG;
				}
				m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
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
			//SetrDraw
			{
				int Xsize = DrawParts->m_DispXSize;
				int Ysize = DrawParts->m_DispYSize;

				int Xmin = y_r(320);
				int Ymin = LineHeight;

				DrawControl::Instance()->SetDrawBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), (int)(Lerp((float)Ymin, (float)Ysize, m_PullDown)), Gray75, TRUE);
				if (m_PullDown >= 1.f) {
					//Back
					m_BGPtr->Draw_Back(this->m_posx, this->m_posy, this->m_Scale);
					//ウィンドウ
					WindowMngr->Draw();
				}
				//タイトルバック
				int DieCol = std::clamp((int)(Lerp(1.f, 128.f, m_NoneActiveTimes / 5.f)), 0, 255);
				WindowSystem::SetBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), LineHeight, GetColor(DieCol, DieCol, DieCol));
				//タイトル
				if (m_PullDown >= 1.f) {
					if (WindowSystem::ClickCheckBox(Xmin + y_r(10), 0, Xmin + y_r(10 + 460), Ymin, false, true, Gray25, "全てのウィンドウを閉じる")) {
						WindowMngr->DeleteAll();
					}

					WindowSystem::SetMsg(0, 0, y_r(1920), LineHeight, LineHeight, STR_MID, White, Black, "EFT Assistant");
					WindowSystem::SetMsg(y_r(1280), LineHeight * 1 / 10, y_r(1280), LineHeight, LineHeight * 8 / 10, STR_LEFT, White, Black, "ver %d.%d.%d", 0, 1, 2);
					if (WindowSystem::CloseButton(y_r(1920) - LineHeight, 0)) { SetisEnd(true); }
				}
				//展開
				if (WindowSystem::ClickCheckBox(0, 0, Xmin, Ymin, false, true, Gray25, !m_IsPullDown ? "折りたたむ" : "展開")) { m_IsPullDown ^= 1; }
				Easing(&m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
				if (m_PullDown >= 0.95f) { m_PullDown = 1.f; }
				if (m_PullDown <= 0.05f) { m_PullDown = 0.f; }
				//
				if (m_PullDown >= 1.f) {
					//Front
					m_BGPtr->DrawFront(this->m_posx, this->m_posy, this->m_Scale);
					//中央位置回避のための小円
					DrawControl::Instance()->SetDrawCircle(Xsize, Ysize, y_r(100), TransColor, TRUE);
				}

				DataErrorLog::Instance()->Draw();
				if (GetASyncLoadNum() > 0) {
					WindowSystem::SetMsg(0, y_r(1080) - LineHeight, y_r(0), y_r(1080), LineHeight, STR_LEFT, White, Black, "Loading...");
				}
			}
			//SetIsUpdateDraw(false);
			return true;
		}
		void Dispose_Sub(void) noexcept override {
			m_BGPtr.reset();
			m_TaskBG.reset();
			m_HideOutBG.reset();
			m_ItemBG.reset();
			m_MapBG.reset();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}

		//UI表示
		void DrawUI_In_Sub(void) noexcept  override {
			DrawControl::Instance()->Draw();
		}
	};
};
