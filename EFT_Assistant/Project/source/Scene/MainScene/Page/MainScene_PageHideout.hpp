#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	class HideOutBG :public BGParent {
	private:
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
		}
		void LateExecute_Sub(void) noexcept override {
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "�߂�")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};