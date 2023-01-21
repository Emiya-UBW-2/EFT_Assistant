#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	enum class BGSelect {
		Task,
		HideOut,
		Item,
		Map,
	};
	class TitleBG :public BGParent {
	private:
		BGSelect m_Select{ (BGSelect)0 };
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Select = (BGSelect)0;
		}
		void LateExecute_Sub(void) noexcept override {
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			int xsize = y_r(420);
			int ysize = y_r(52);
			int ypos = y_r(540);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray25, "タスク")) {
				m_Select = BGSelect::Task;
				TurnOnGoNextBG();
			}
			ypos += y_r(100);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray50, "ハイドアウト")) {
				//m_Select = BGSelect::HideOut;
				//TurnOnGoNextBG();
			}
			ypos += y_r(100);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray25, "アイテム")) {
				m_Select = BGSelect::Item;
				TurnOnGoNextBG();
			}
			ypos += y_r(100);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray25, "マップ")) {
				m_Select = BGSelect::Map;
				TurnOnGoNextBG();
			}
			ypos += y_r(100);
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		const auto& GetNextSelect() const noexcept { return m_Select; }
	};
};