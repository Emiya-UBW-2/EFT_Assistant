#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	class MAINLOOPLOADER : public TEMPSCENE {
	private:
	public:
		MAINLOOPLOADER(void) noexcept {}
	public:
		void Set_Sub(void) noexcept override {
			DataBase::Create();
		}
		bool Update_Sub(void) noexcept override {
			return !(!GetIsFirstLoop() && DataBase::Instance()->SetDataList());
		}
		void Dispose_Sub(void) noexcept override {
			bool DrawLog = false;
#ifdef DEBUG
			DrawLog = true;
#endif
			DataBase::Instance()->LoadList(DrawLog);
		}
	public:
		//UI•\Ž¦
		void DrawUI_In_Sub(void) noexcept  override {
			int YSize = y_r(108);
			int YSize1 = y_r(64);

			int XMin = y_r(960) - y_r(960) * 3 / 5;
			int XMax = y_r(960) + y_r(960) * 3 / 5;
			int YPosMin = y_r(540) - YSize / 2;
			int YPosMax = y_r(540) + YSize / 2;

			DrawBox(0, 0, y_r(1920) + 1, y_r(1080) + 1, TransColor, TRUE);//”wŒi“§–¾
			DrawBox(XMin, YPosMin, XMax, YPosMax, Gray75, TRUE);
			FontPool::Instance()->Get(FontPool::FontType::Nomal_Edge, y_r(48)).DrawString(-1, STRX_MID, STRY_MIDDLE, y_r(960), YPosMin + YSize1 / 2, White, Black, "EFT Assistant ‹N“®’†c");
			//
			int commonEdge = y_r(5);
			int XMinGauge = XMin + commonEdge + commonEdge;
			int XMaxGauge = XMax - commonEdge - commonEdge;
			float Per = (float)(DataBase::Instance()->GetSetCount()) / (float)(DataBase::Instance()->GetMaxCount());
			DrawBox(XMin + commonEdge, YPosMin + YSize1 + commonEdge, XMax - commonEdge, YPosMax - commonEdge, Gray25, TRUE);
			DrawBox(XMinGauge, YPosMin + YSize1 + commonEdge + commonEdge, XMaxGauge, YPosMax - commonEdge - commonEdge, Gray50, TRUE);
			DrawBox(XMinGauge, YPosMin + YSize1 + commonEdge + commonEdge, XMinGauge + (int)((float)(XMaxGauge - XMinGauge)*Per), YPosMax - commonEdge - commonEdge, Green, TRUE);
		}
	};
};
