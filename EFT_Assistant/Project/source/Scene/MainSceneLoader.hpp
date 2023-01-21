#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	class MAINLOOPLOADER : public TEMPSCENE {
	private:
	public:
		MAINLOOPLOADER(void) noexcept {}
	public:
		void Set_Sub(void) noexcept override {
			SetUseASyncLoadFlag(TRUE);
			Get_Next()->Load();//次シーンのロード物
			SetUseASyncLoadFlag(FALSE);
		}
		bool Update_Sub(void) noexcept override {
			return (GetASyncLoadNum() > 0);
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		//UI表示
		void DrawUI_In_Sub(void) noexcept  override {
			FontPool::Instance()->Get(FontPool::FontType::Nomal_Edge).DrawString(y_r(24), FontHandle::FontXCenter::MIDDLE, FontHandle::FontYCenter::MIDDLE,
				y_r(960), y_r(540), GetColor(255, 255, 255), GetColor(0, 0, 0), "Loading...");
		}
	};
};