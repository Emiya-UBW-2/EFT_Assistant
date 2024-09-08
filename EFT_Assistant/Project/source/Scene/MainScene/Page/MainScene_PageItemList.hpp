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
	"�S��",
	"�^�X�N�̂�",
	"�n�C�h�A�E�g�̂�",
	"���g�p",
	};

	class ItemListBG :public PageParent {
	private:
		bool							m_IsCheckCraft{ true };//�^�X�N�ɕK�v�ȃA�C�e�����Ȃ����ۂ�
		bool							m_IsCheckBarter{ true };//�^�X�N�ɕK�v�ȃA�C�e�����Ȃ����ۂ�
		bool							m_IsNeedItem{ true };//�^�X�N�ɕK�v�ȃA�C�e�����Ȃ����ۂ�

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
