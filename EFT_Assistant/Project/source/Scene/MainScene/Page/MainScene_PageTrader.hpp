#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TraderBG :public BGParent {
	private:
		TraderID	m_EditTraderID{ InvalidID };
		int			m_LLSelect{ 0 };
		WindowSystem::ScrollBoxClass		m_Scroll;
	private:
		void Init_Sub(int *, int *, float*) noexcept override {
			m_EditTraderID = InvalidID;
		}

		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}
		void Draw_Back_Sub(int, int, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(50);
				MakeList<TraderList>(xp, yp, TraderData::Instance()->GetList(), "Trader", (int*)&m_EditTraderID, true, false, false, [&](const auto*) { return true; });
			}
			//
			if (m_EditTraderID != InvalidID) {
				auto* ptr = TraderData::Instance()->FindPtr(m_EditTraderID);
				for (auto& LL : ptr->GetLvData()) {
					int index = (int)(&LL - &ptr->GetLvData().front());
					int xp = y_r(500) + (y_r(100) + y_r(10)) * index;
					int yp = LineHeight + y_r(50);
					if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(100), yp + LineHeight, false, true, (m_LLSelect == index) ? Gray15 : Gray50, "Lv%d", index + 1)) {
						m_LLSelect = index;
					}
				}
				{
					int xp = y_r(460);
					int yp = LineHeight + y_r(50) + y_r(100);

					int ysize = (int)((float)y_r(48));
					int ysizeAdd = ysize + y_r(5);
					int yofs = LineHeight;

					int Max = (int)(ptr->GetLvData().at(m_LLSelect).m_ItemBarters.size());

					int ofset = (int)(this->m_Scroll.GetNowScrollYPer()*(std::max(0, Max - 10 + 1)*ysizeAdd));

					int ypMin = yp;
					int ypMax = (y_r(1080) - y_r(100));
					int yp1 = yp + yofs - ofset;
					for (auto& Bar : ptr->GetLvData().at(m_LLSelect).m_ItemBarters) {
						int xofs_buf = y_r(10);
						if (ypMin - ysizeAdd < yp1 && yp1 < ypMax) {
							if (ypMin < yp1 && yp1 < ypMax - ysizeAdd) {
								DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
							}
							else {
								if (yp1 <= ypMin) {
									DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (ypMin - yp1) / ysizeAdd, 0, 255));
								}
								else {
									DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (yp1 - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
								}
							}
							{
								for (const auto& I : Bar.m_ItemReq) {
									auto* ptr2 = ItemData::Instance()->FindPtr(I.GetID());
									if (ptr2) {
										int xstart = xp + xofs_buf;
										xofs_buf += ptr2->Draw(xp + xofs_buf, yp1, y_r(200), ysize, I.GetValue(), Gray25, !WindowMngr->PosHitCheck(nullptr), false, false, true);
										DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp1, xp + xofs_buf, yp1 + ysize, Gray75, false);
									}
								}
							}
							{
								xofs_buf = std::max(xofs_buf, y_r(10) + ysize * 16);
								xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp1, xp + 0, yp1 + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
									"->");
								xofs_buf += y_r(30);
							}
							{
								for (const auto& I : Bar.m_ItemReward) {
									auto* ptr2 = ItemData::Instance()->FindPtr(I.GetID());
									if (ptr2) {
										int xstart = xp + xofs_buf;
										xofs_buf += ptr2->Draw(xp + xofs_buf, yp1, y_r(200), ysize, I.GetValue(), Gray25, !WindowMngr->PosHitCheck(nullptr), false, false, true);
										DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp1, xp + xofs_buf, yp1 + ysize, Gray75, false);
									}
								}
							}
						}
						yofs += ysizeAdd;
						yp1 += ysizeAdd;
					}
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
					//スクロールバー
					{
						float Total = (float)yofs / (ypMax - ypMin);
						if (Total > 1.f) {
							m_Scroll.ScrollBox(xp, ypMin, y_r(1920) - y_r(100), ypMax, Total, true);
						}
					}
				}
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					if (m_EditTraderID != InvalidID) {
						m_EditTraderID = InvalidID;
					}
					else {
						TurnOnGoNextBG();
					}
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};