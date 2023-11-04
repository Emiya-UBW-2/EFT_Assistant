#include"MainScene_PageBase.hpp"
#include "../../../Parts/WindowParts.hpp"
#include "../../../Parts/StrControl.hpp"
#include "../../../Parts/DrawSystem.hpp"
#include "../../../Parts/InputParts.hpp"
#include "../../../Parts/PlayerDataParts.hpp"
#include "../../../Parts/InterruptParts.hpp"

#include "MainScene_PageCustom.hpp"
#include "MainScene_PageItem.hpp"

const FPS_n2::BGBase* SingletonBase<FPS_n2::BGBase>::m_Singleton = nullptr;

namespace FPS_n2 {
	BGBase::BGBase(void)noexcept {
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
	BGBase::~BGBase(void)noexcept {
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
	void BGBase::SetNext(BGSelect value)noexcept {
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
};
