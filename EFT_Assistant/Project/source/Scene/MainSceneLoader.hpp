#pragma once
#include"../Header.hpp"

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
			return !(!GetIsFirstLoop() && (GetASyncLoadNum() == 0));
		}
		void Dispose_Sub(void) noexcept override {}
	public:
		//UI表示
		void DrawUI_In_Sub(void) noexcept  override {
			DrawBox(0, 0, y_r(1920), y_r(1080), GetColor(1, 1, 1), TRUE);
			FontPool::Instance()->Get(FontPool::FontType::Nomal_Edge, y_r(64)).DrawString(-1, FontHandle::FontXCenter::MIDDLE, FontHandle::FontYCenter::MIDDLE,
				y_r(960), y_r(540), GetColor(255, 255, 255), GetColor(1, 1, 1), "起動中...");
		}
	};
};
