#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"
#include "../Data/MainScene_Item.hpp"

namespace FPS_n2 {
	class ItemBG :public PageParent {
	private:
		ItemTypeID						m_ItemTypeSel{ InvalidID };
		MapID							m_MapTypeSel{ InvalidID };
		WindowMySystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
		bool							m_RaidMode{ false };

		std::vector<const ItemList*>	Items;
		bool							m_TraderSort{ false };
		bool							m_ValueSort{ false };
		bool							m_ValuePerSort{ false };

		WindowMySystem::SearchBox			m_SearchBox;
	private:
		void Init_Sub(int *, int *, float*) noexcept override;
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override;
		void DrawFront_Sub(int, int, float) noexcept override;
		void Dispose_Sub(void) noexcept override {}
	};
};
