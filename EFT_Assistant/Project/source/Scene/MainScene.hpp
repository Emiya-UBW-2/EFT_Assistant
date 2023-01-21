#pragma once
#include"Header.hpp"

#define LineHeight	y_r(48)

//Datas
#include "Scene/MainScene/Data/MainScene_Parents.hpp"
#include "Scene/MainScene/Data/MainScene_Map.hpp"
#include "Scene/MainScene/Data/MainScene_ItemCategory.hpp"
#include "Scene/MainScene/Data/MainScene_ItemType.hpp"
#include "Scene/MainScene/Data/MainScene_Item.hpp"
#include "Scene/MainScene/Data/MainScene_Enemy.hpp"
#include "Scene/MainScene/Data/MainScene_Trader.hpp"
#include "Scene/MainScene/Data/MainScene_Task.hpp"
//Pages
#include "Scene/MainScene/Page/MainScene_PageParents.hpp"
#include "Scene/MainScene/Page/MainScene_PageTitle.hpp"
#include "Scene/MainScene/Page/MainScene_PageTask.hpp"
#include "Scene/MainScene/Page/MainScene_PageHideout.hpp"
#include "Scene/MainScene/Page/MainScene_PageItem.hpp"
#include "Scene/MainScene/Page/MainScene_PageMap.hpp"

namespace FPS_n2 {
	class MAINLOOP : public TEMPSCENE {
	private:
		std::unique_ptr<WindowSystem::WindowManager>				m_Window;
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
	public:
		void Load_Sub(void) noexcept override {}

		void Set_Sub(void) noexcept override {
			//
			InputControl::Create();
			DataErrorLog::Create();
			m_Window = std::make_unique<WindowSystem::WindowManager>();
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
			auto* Input = InputControl::Instance();
			auto mouse_moveX = Input->GetMouseX();							//�h���b�O�O�̃}�E�X���W�i�[
			auto mouse_moveY = Input->GetMouseY();
			Input->Execute();
			mouse_moveX = Input->GetMouseX() - mouse_moveX;
			mouse_moveY = Input->GetMouseY() - mouse_moveY;
			//�h���b�O�J�n���̏���
			if (Input->GetMiddleClick().press()) {
				int start_windowX = 0, start_windowY = 0;
				GetWindowPosition(&start_windowX, &start_windowY);			//�E�B���h�E�̈ʒu���i�[
				start_windowX += mouse_moveX;
				start_windowY += mouse_moveY;
				SetWindowPosition(start_windowX, start_windowY);			//�}�E�X�ʒu�̍����Z�o���A�E�B���h�E�𓮂���
				Input->SetMouse();
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
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
			m_Window->Execute();
			m_BGPtr->LateExecute();
			if (m_BGPtr->IsGoNextBG()) {
				m_Window->DeleteAll();
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
			return true;
		}
		void Dispose_Sub(void) noexcept override {
			m_Window.reset();
			m_BGPtr.reset();
			m_TaskBG.reset();
			m_HideOutBG.reset();
			m_ItemBG.reset();
			m_MapBG.reset();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}

		//UI�\��
		void DrawUI_Base_Sub(void) noexcept  override {
			auto* DrawParts = DXDraw::Instance();

			int Xsize = DrawParts->m_DispXSize;
			int Ysize = DrawParts->m_DispYSize;

			int Xmin = y_r(320);
			int Ymin = LineHeight;

			DrawBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), (int)(Lerp((float)Ymin, (float)Ysize, m_PullDown)), Gray75, TRUE);
			if (m_PullDown >= 1.f) {
				//Back
				m_BGPtr->Draw_Back(this->m_Window, this->m_posx, this->m_posy, this->m_Scale);
				//�E�B���h�E
				m_Window->Draw();
			}
			//�^�C�g���o�b�N
			WindowSystem::SetBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), LineHeight, Gray50);
			//�^�C�g��
			if (m_PullDown >= 1.f) {
				WindowSystem::SetMsg(0, 0, y_r(1920), LineHeight, LineHeight, STR_MID, White, Black, "EFT Assistant");
				WindowSystem::SetMsg(y_r(1280), LineHeight * 3 / 10, y_r(1280), LineHeight, LineHeight * 7 / 10, STR_LEFT, White, Black, "ver %d.%d.%d", 0, 0, 10);
				if (WindowSystem::CloseButton(y_r(1920) - LineHeight, 0)) { SetisEnd(true); }
			}
			//�W�J
			if (WindowSystem::ClickCheckBox(0, 0, Xmin, Ymin, false, true, Gray25, !m_IsPullDown ? "�܂肽����" : "�W�J")) { m_IsPullDown ^= 1; }
			Easing(&m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
			if (m_PullDown >= 0.95f) { m_PullDown = 1.f; }
			if (m_PullDown <= 0.05f) { m_PullDown = 0.f; }
			//
			if (m_PullDown >= 1.f) {
				//Front
				m_BGPtr->DrawFront(this->m_Window, this->m_posx, this->m_posy, this->m_Scale);
				//�����ʒu����̂��߂̏��~
				DrawCircle(Xsize, Ysize, y_r(100), TransColor, TRUE);
			}
			//

			DataErrorLog::Instance()->Draw();
			if (GetASyncLoadNum() > 0) {
				WindowSystem::SetMsg(0, y_r(1080) - LineHeight, y_r(0), y_r(1080), LineHeight, STR_LEFT, White, Black, "Loading...");
			}
		}
	};
};
