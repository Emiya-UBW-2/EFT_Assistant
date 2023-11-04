#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	class MAINLOOP : public TEMPSCENE {
	private:
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		float														m_Scale{ 0.6f };
		//
		float														m_NoneActiveTimes{ 0.f };

		bool														m_IsPull{ false };

		bool														m_IsPullRight{ false };
		bool														m_IsPullDown{ false };
		float														m_PullRight{ 1.f };
		float														m_PullDown{ 1.f };
		//
		bool														m_Loading{ false };
		//
		bool														m_WindowMove{ false };
		bool														m_DrawLog{ false };
	private:
		bool IsOpen() const noexcept { return(this->m_PullDown >= 1.f); }
	public:
		void Load_Sub(void) noexcept override {
		}
		void Set_Sub(void) noexcept override;
		bool Update_Sub(void) noexcept override;
		void Dispose_Sub(void) noexcept override;
	public:
		void BG_Draw_Sub(void) noexcept override {}
		void DrawUI_In_Sub(void) noexcept override;
	};
};
