#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageBase.hpp"

namespace FPS_n2 {
	enum class EnumHideoutDrawMode {
		Normal,
		Item,
	};

	class HideOutBG :public BGParent {
	private:
		struct DrewData {
			bool IsDrew{ false };
			int xpos{ 0 };
			int ypos{ 0 };
		};

		std::vector<std::pair<HideoutID, std::vector<DrewData>>>	isDrew;
		int															DrawLineCount{ 0 };
		EnumHideoutDrawMode											m_Mode{ EnumHideoutDrawMode::Normal };
	private:
		void Init_Sub(int*, int*, float*) noexcept override;
		void LateExecute_Sub(int* xpos, int*ypos, float*) noexcept override;

		bool DrawHideoutList(HideoutID MyID, int MyLv, int xpos, int* ypos, int xsize, int ysize) noexcept;

		void Draw_Back_Sub(int xpos, int ypos, float) noexcept override;
		void DrawFront_Sub(int, int, float) noexcept override;
		void Dispose_Sub(void) noexcept override {}
	};
};
