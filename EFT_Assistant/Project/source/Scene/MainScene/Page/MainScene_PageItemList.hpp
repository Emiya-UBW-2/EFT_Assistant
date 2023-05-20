#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemListBG :public BGParent {
	private:
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}

		void Draw_Back_Sub(int xpos, int ypos, float) noexcept override {
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//auto* Input = InputControl::Instance();
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "–ß‚é")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};