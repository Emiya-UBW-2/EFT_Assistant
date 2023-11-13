#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"


namespace FPS_n2 {
	class TraderBG :public PageParent {
	private:
		TraderID	m_EditTraderID{ InvalidID };
		int			m_LLSelect{ 0 };
		WindowSystem::ScrollBoxClass		m_Scroll;
	private:
		void Init_Sub(int *, int *, float*) noexcept override;
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override;
		void DrawFront_Sub(int, int, float) noexcept override;
		void Dispose_Sub(void) noexcept override {}
	};
};
