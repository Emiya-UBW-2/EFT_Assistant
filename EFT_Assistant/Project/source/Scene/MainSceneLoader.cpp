#include "MainSceneLoader.hpp"
#include "MainScene/Data/MainScene_Base.hpp"
#include "MainScene/Page/MainScene_PageManager.hpp"
#include "../PartsHeader.hpp"

namespace FPS_n2 {
	void MAINLOOPLOADER::Set_Sub(void) noexcept {
		DataBase::Create();
	}
	bool MAINLOOPLOADER::Update_Sub(void) noexcept {
		return !(!GetIsFirstLoop() && DataBase::Instance()->SetDataList());
	}
	void MAINLOOPLOADER::Dispose_Sub(void) noexcept {
		bool DrawLog = false;
#ifdef DEBUG
		DrawLog = true;
#endif
		DataBase::Instance()->LoadList(DrawLog);
	}
	//UI•\Ž¦
	void MAINLOOPLOADER::DrawUI_In_Sub(void) noexcept {
		int YSize = DXDraw::Instance()->GetUIY(108);
		int YSize1 = DXDraw::Instance()->GetUIY(64);

		int XMin = DXDraw::Instance()->GetUIY(960) - DXDraw::Instance()->GetUIY(960) * 3 / 5;
		int XMax = DXDraw::Instance()->GetUIY(960) + DXDraw::Instance()->GetUIY(960) * 3 / 5;
		int YPosMin = DXDraw::Instance()->GetUIY(540) - YSize / 2;
		int YPosMax = DXDraw::Instance()->GetUIY(540) + YSize / 2;

		DrawBox(0, 0, DXDraw::Instance()->GetUIY(1920) + 1, DXDraw::Instance()->GetUIY(1080) + 1, TransColor, TRUE);//”wŒi“§–¾
		DrawBox(XMin, YPosMin, XMax, YPosMax, Gray75, TRUE);
		FontPool::Instance()->Get(FontPool::FontType::MS_Gothic, DXDraw::Instance()->GetUIY(48), 3)->DrawString(-1, STRX_MID, STRY_MIDDLE, DXDraw::Instance()->GetUIY(960), YPosMin + YSize1 / 2, White, Black, "EFT Assistant ‹N“®’†c");
		//
		int commonEdge = DXDraw::Instance()->GetUIY(5);
		int XMinGauge = XMin + commonEdge + commonEdge;
		int XMaxGauge = XMax - commonEdge - commonEdge;
		DrawBox(XMin + commonEdge, YPosMin + YSize1 + commonEdge, XMax - commonEdge, YPosMax - commonEdge, Gray25, TRUE);
		DrawBox(XMinGauge, YPosMin + YSize1 + commonEdge + commonEdge, XMaxGauge, YPosMax - commonEdge - commonEdge, Gray50, TRUE);
		DrawBox(XMinGauge, YPosMin + YSize1 + commonEdge + commonEdge, XMinGauge + (int)((float)(XMaxGauge - XMinGauge)*DataBase::Instance()->GetSetPer()), YPosMax - commonEdge - commonEdge, Green, TRUE);
	}
};
