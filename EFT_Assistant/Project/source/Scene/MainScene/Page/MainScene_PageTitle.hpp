#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	enum class BGSelect {
		Task,
		HideOut,
		Item,
		Map,
		Custom,
	};
	class TitleBG :public BGParent {
	private:
		BGSelect m_Select{ (BGSelect)0 };
	private:
		void DrawOnce(int xpos1, int ypos1, int xpos2, int ypos2,const char* Name, BGSelect SelWindow) noexcept {
			if (WindowSystem::ClickCheckBox(xpos1, ypos1, xpos2, ypos2, false, true, Gray25, Name)) {
				m_Select = SelWindow;
				TurnOnGoNextBG();
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Select = (BGSelect)0;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override {
			int xsize = y_r(420);
			int ysize = y_r(52);
			int ypos = y_r(540);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "タスク", BGSelect::Task);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "ハイドアウト", BGSelect::HideOut);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "アイテム", BGSelect::Item);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "マップ", BGSelect::Map);
			ypos += y_r(100);
			DrawOnce(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, "カスタマイズ", BGSelect::Custom);
			ypos += y_r(100);
		}
		void DrawFront_Sub(int, int, float) noexcept override {
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		const auto& GetNextSelect() const noexcept { return m_Select; }
	};
};