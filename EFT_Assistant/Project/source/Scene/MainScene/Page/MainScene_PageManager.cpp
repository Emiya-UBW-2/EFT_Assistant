#include "MainScene_PageManager.hpp"

#include "../../../Header.hpp"
#include "../../../PartsHeader.hpp"

#include "MainScene_PageTitle.hpp"
#include "MainScene_PageTask.hpp"
#include "MainScene_PageHideout.hpp"
#include "MainScene_PageItem.hpp"
#include "MainScene_PageMap.hpp"
#include "MainScene_PageCustom.hpp"
#include "MainScene_PageTrader.hpp"
#include "MainScene_PageItemList.hpp"
#include "MainScene_PagePlayerInfo.hpp"

const FPS_n2::PageManager* SingletonBase<FPS_n2::PageManager>::m_Singleton = nullptr;

namespace FPS_n2 {
	PageManager::PageManager(void)noexcept {
		this->m_PagePtr.at((int)BGSelect::Title) = std::make_shared<TitleBG>();
		this->m_PagePtr.at((int)BGSelect::Task) = std::make_shared<TaskBG>();
		this->m_PagePtr.at((int)BGSelect::HideOut) = std::make_shared<HideOutBG>();
		this->m_PagePtr.at((int)BGSelect::Item) = std::make_shared<ItemBG>();
		this->m_PagePtr.at((int)BGSelect::Map) = std::make_shared<MapBG>();
		this->m_PagePtr.at((int)BGSelect::Custom) = std::make_shared<CustomBG>();
		this->m_PagePtr.at((int)BGSelect::Trader) = std::make_shared<TraderBG>();
		this->m_PagePtr.at((int)BGSelect::ItemList) = std::make_shared<ItemListBG>();
		this->m_PagePtr.at((int)BGSelect::PlayerInfo) = std::make_shared<PlayerInfoBG>();
		m_NowOpenPagePtr = m_PagePtr.at((int)BGSelect::Title);
	}
	PageManager::~PageManager(void)noexcept {
		m_NowOpenPagePtr.reset();
		for (auto& B : m_PagePtr) {
			B.reset();
		}
	}
	//
	void PageManager::ChangePage(BGSelect Select) noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		WindowMngr->DeleteAll();
		Dispose();
		m_NowOpenPagePtr = m_PagePtr.at((int)Select);
		Init();
	}
	//
	void PageManager::Init() noexcept {
		this->m_NowOpenPagePtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
		this->m_IsGoNextPage = false;
	}
	void PageManager::FirstExecute() noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* Input = InputControl::Instance();
		auto iSWindowHit = !WindowMngr->PosHitCheck(nullptr);
		if ((Input->GetWheelAdd() != 0) && !iSWindowHit && Input->GetScaleActive()) {
			auto PrevScale = this->m_Scale;
			this->m_Scale = std::clamp(this->m_Scale + (float)Input->GetWheelAdd() / 10.f, 0.1f, 2.f);
			auto ScaleChange = (this->m_Scale - PrevScale);
			if (ScaleChange != 0.f) {
				this->m_posx -= (int)((float)(Input->GetMouseX() - this->m_posx) * ScaleChange / this->m_Scale);
				this->m_posy -= (int)((float)(Input->GetMouseY() - this->m_posy) * ScaleChange / this->m_Scale);
			}
		}
		Input->SetScaleActive(true);
		if (Input->GetRightClick().press() && !iSWindowHit) {
			this->m_posx += Input->GetMouseMoveX();
			this->m_posy += Input->GetMouseMoveY();
			HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
			SetCursor(hCursor);
		}
	}
	void PageManager::LateExecute() noexcept {
		this->m_NowOpenPagePtr->LateExecute(&this->m_posx, &this->m_posy, &this->m_Scale);
		if (this->m_IsGoNextPage) {
			ChangePage((this->m_NowOpenPagePtr == GetPage(BGSelect::Title)) ? ((std::shared_ptr<TitleBG>&)GetPage(BGSelect::Title))->GetNextSelect() : BGSelect::Title);
		}
	}
	void PageManager::Draw_Back() noexcept {
		this->m_NowOpenPagePtr->Draw_Back(this->m_posx, this->m_posy, this->m_Scale);
	}
	void PageManager::DrawFront() noexcept {
		this->m_NowOpenPagePtr->DrawFront(this->m_posx, this->m_posy, this->m_Scale);
	}
	void PageManager::Dispose(void) noexcept {
		this->m_NowOpenPagePtr->Dispose();
	}
};
