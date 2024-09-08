#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	class TitleBG :public PageParent {
	private:
		BGSelect	m_Select{ (BGSelect)0 };
		std::string	m_strResult;
		std::string	m_InfoStr;
		int			m_CoolTimeCount{ -1 };
	private:
		const auto CanUpdateData() const noexcept { return m_CoolTimeCount == -1; }
		void DrawOnce(int xpos1, int ypos1, int xpos2, int ypos2, const char* Name, const char* Info, BGSelect SelWindow) noexcept {
			auto* PageMngr = PageManager::Instance();
			if (IntoMouse(xpos1, ypos1, xpos2, ypos2)) {
				m_InfoStr = Info;
			}
			if (WindowSystem::SetMsgClickBox(xpos1, ypos1, xpos2, ypos2, ypos2 - ypos1, Gray25, false, true, Name)) {
				m_Select = SelWindow;
				PageMngr->TurnOnGoNextPage();
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
