#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	class MAINLOOPLOADER : public TEMPSCENE {
	private:
	public:
		MAINLOOPLOADER(void) noexcept {}
	public:
		void Set_Sub(void) noexcept  override;
		bool Update_Sub(void) noexcept  override;
		void Dispose_Sub(void) noexcept  override;
	public:
		//UI•\Ž¦
		void DrawUI_In_Sub(void) noexcept  override;
	};
};
