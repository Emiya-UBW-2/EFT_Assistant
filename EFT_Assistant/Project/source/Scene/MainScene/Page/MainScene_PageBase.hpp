#pragma once
#include"../../../Header.hpp"
#include "MainScene_PageParents.hpp"
#include "MainScene_PageTitle.hpp"
#include "MainScene_PageTask.hpp"
#include "MainScene_PageHideout.hpp"
#include "MainScene_PageItem.hpp"
#include "MainScene_PageMap.hpp"
#include "MainScene_PageCustom.hpp"
#include "MainScene_PageTrader.hpp"
#include "MainScene_PageItemList.hpp"
#include "MainScene_PagePlayerInfo.hpp"

namespace FPS_n2 {
	class BGBase : public SingletonBase<BGBase> {
	private:
		friend class SingletonBase<BGBase>;
	private:
		std::shared_ptr<BGParent>									m_BGPtr;
		std::shared_ptr<TitleBG>									m_TitleBG;
		std::shared_ptr<TaskBG>										m_TaskBG;
		std::shared_ptr<HideOutBG>									m_HideOutBG;
		std::shared_ptr<ItemBG>										m_ItemBG;
		std::shared_ptr<MapBG>										m_MapBG;
		std::shared_ptr<CustomBG>									m_CustomBG;
		std::shared_ptr<TraderBG>									m_TraderBG;
		std::shared_ptr<ItemListBG>									m_ItemListBG;
		std::shared_ptr<PlayerInfoBG>								m_PlayerInfoBG;
	private:
		BGBase(void)noexcept {
			m_TitleBG = std::make_shared<TitleBG>();
			m_TaskBG = std::make_shared<TaskBG>();
			m_HideOutBG = std::make_shared<HideOutBG>();
			m_ItemBG = std::make_shared<ItemBG>();
			m_MapBG = std::make_shared<MapBG>();
			m_CustomBG = std::make_shared<CustomBG>();
			m_TraderBG = std::make_shared<TraderBG>();
			m_ItemListBG = std::make_shared<ItemListBG>();
			m_PlayerInfoBG = std::make_shared<PlayerInfoBG>();
			m_BGPtr = this->m_TitleBG;
		}
		~BGBase(void)noexcept {
			m_BGPtr.reset();
			m_TaskBG.reset();
			m_HideOutBG.reset();
			m_ItemBG.reset();
			m_MapBG.reset();
			m_CustomBG.reset();
			m_TraderBG.reset();
			m_ItemListBG.reset();
			m_PlayerInfoBG.reset();
		}
	public:
		void SetNext(BGSelect value)noexcept {
			switch (value) {
			case BGSelect::Title:
				m_BGPtr = this->m_TitleBG;
				break;
			case BGSelect::Task:
				m_BGPtr = this->m_TaskBG;
				break;
			case BGSelect::HideOut:
				m_BGPtr = this->m_HideOutBG;
				break;
			case BGSelect::Item:
				m_BGPtr = this->m_ItemBG;
				break;
			case BGSelect::Map:
				m_BGPtr = this->m_MapBG;
				break;
			case BGSelect::Custom:
				m_BGPtr = this->m_CustomBG;
				break;
			case BGSelect::Trader:
				m_BGPtr = this->m_TraderBG;
				break;
			case BGSelect::ItemList:
				m_BGPtr = this->m_ItemListBG;
				break;
			case BGSelect::PlayerInfo:
				m_BGPtr = this->m_PlayerInfoBG;
				break;
			default:
				m_BGPtr = this->m_TitleBG;
				break;
			}
		}
	public:
		void Init(int *posx, int *posy, float* Scale) noexcept { this->m_BGPtr->Init(posx, posy, Scale); }
		void LateExecute(int *posx, int *posy, float* Scale) noexcept { this->m_BGPtr->LateExecute(posx, posy, Scale); }
		void Draw_Back(int posx, int posy, float Scale) noexcept { this->m_BGPtr->Draw_Back(posx, posy, Scale); }
		void DrawFront(int posx, int posy, float Scale) noexcept { this->m_BGPtr->DrawFront(posx, posy, Scale); }
		void Dispose(void) noexcept { this->m_BGPtr->Dispose(); }
	public:
		auto& GetNowBG(void) noexcept { return this->m_BGPtr; }
		auto& GetTitleBG(void) noexcept { return this->m_TitleBG; }
		auto& GetCustomBG(void) noexcept { return this->m_CustomBG; }
	};
};
