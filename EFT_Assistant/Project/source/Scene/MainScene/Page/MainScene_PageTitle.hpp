#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageBase.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	class TitleBG :public BGParent {
	private:
		BGSelect	m_Select{ (BGSelect)0 };
		std::string	strResult;
		std::string	InfoStr;
		int			ttt{ -1 };
	private:
		void DrawOnce(int xpos1, int ypos1, int xpos2, int ypos2, const char* Name, const char* Info, BGSelect SelWindow) noexcept {
			auto* Input = InputControl::Instance();
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xpos1, ypos1, xpos2, ypos2)) {
				InfoStr = Info;
			}
			if (WindowSystem::ClickCheckBox(xpos1, ypos1, xpos2, ypos2, false, true, Gray25, Name)) {
				m_Select = SelWindow;
				TurnOnGoNextBG();
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override { m_Select = (BGSelect)0; }
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override;
		void DrawFront_Sub(int, int, float) noexcept override;
		void Dispose_Sub(void) noexcept override {}
	public:
		const auto& GetNextSelect() const noexcept { return this->m_Select; }
	};
};
