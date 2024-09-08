#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"

namespace FPS_n2 {
	enum class EnumListDrawMode {
		Normal,
		All = Normal,
		Task,
		Hideout,
		None,
		Max,
	};
	static const char* ListDrawModeStr[(int)EnumListDrawMode::Max] = {
	"全て",
	"タスクのみ",
	"ハイドアウトのみ",
	"未使用",
	};

	class ItemListBG :public PageParent {
	private:
		bool							m_IsCheckCraft{ true };//タスクに必要なアイテムを省くか否か
		bool							m_IsCheckBarter{ true };//タスクに必要なアイテムを省くか否か
		bool							m_IsNeedItem{ true };//タスクに必要なアイテムを省くか否か

		bool							m_DrawCanClearTask{ false };
		bool							m_DrawCanClearHideout{ false };

		WindowMySystem::SearchBox			m_SearchBox;
		WindowMySystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };

		EnumListDrawMode				m_Mode{ EnumListDrawMode::Normal };
		bool							m_IsListChange{ true };

		struct counts
		{
			ItemID	m_ID{ InvalidID };
			int		count{ 0 };
			bool	isFir{ true };
			bool	isNeed{ false };
		};
		std::vector<std::vector<counts>> Counter;
	private:
		void AddItemList(std::vector<counts>* Types, ItemID ID, int value, bool isFiR, bool isNeed) noexcept;
		void SetItemList() noexcept;

		void DrawTab(int xp, int yp, std::string_view Info) noexcept;
		void DrawCheckBox(int xp, int yp, std::string_view Info, bool* Check) noexcept;
	private:
		void Init_Sub(int*, int*, float*) noexcept override;
		void LateExecute_Sub(int*, int*, float*) noexcept override {}

		void Draw_Back_Sub(int, int, float) noexcept override;
		void DrawFront_Sub(int, int, float) noexcept override;
		void Dispose_Sub(void) noexcept override {}
	};
};
