#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TaskBG :public BGParent {
	private:
		int															m_MyLevel{ 1 };
		int															m_posxMaxBuffer{ 0 };
		int															m_posyMaxBuffer{ 0 };
		std::vector<Rect2D>											m_TaskRect;
		struct LinePos {
			TaskID	m_ID{ InvalidID };
			int		m_XPos{ 0 };
			int		m_YPos{ 0 };
		public:
			LinePos(TaskID ID, int xp, int yp) noexcept {
				this->m_ID = ID;
				this->m_XPos = xp;
				this->m_YPos = yp;
			}
		};
		std::vector<LinePos>										m_ParentLinePos;
		std::vector<TaskID>											m_Drawed;
	private:
		void DrawChildTaskClickBox(float Scale, TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			if (ParentID == InvalidID) {
				m_posxMaxBuffer = 0;
				m_posyMaxBuffer = 0;
			}
			for (const auto& tasks : TaskData::Instance()->GetTaskList()) {
				if (ParentID == InvalidID) {
					TraderData::Instance()->ResetRep();
				}
				bool IsTrue = false;
				if (std::find_if(m_Drawed.begin(), m_Drawed.end(), [&](const TaskID& obj) {return obj == tasks.GetID(); }) == m_Drawed.end()) {
					if (!IsTrue && tasks.GetTaskNeedData().GetParenttaskID().size() == 0 && ParentID == InvalidID) {
						IsTrue = true;
					}
					if (!IsTrue && tasks.GetTaskNeedData().GetParenttaskID().size() > 0) {
						for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
							if (p.GetParenttaskID() == ParentID) {
								IsTrue = true;
								break;
							}
						}
					}
				}

				if (IsTrue) {
					auto* ptr = TraderData::Instance()->FindPtr(tasks.GetTrader());
					auto parentCanDo_t = parentCanDo;
					auto color = ptr->GetColors(0);
					//信頼度アップダウンを対応
					std::vector<float> PrevRep;
					PrevRep.resize(tasks.GetTaskRewardData().GetLLAdd().size());
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()] = *traderRep;
						*traderRep += LL.GetAdd();
					}
					//信頼度チェック
					if (
						(
						(this->m_MyLevel < tasks.GetTaskNeedData().GetLevel())
							) || !parentCanDo) {
						color = ptr->GetColors(-100);
						parentCanDo_t = false;
					}
					if (ParentID != InvalidID) {
						auto XAddLine = (int)((float)y_r(25) * Scale);
						//
						//DrawControl::Instance()->SetDrawLine(DrawLayer::Back, start_x, start_y, xp, yp + ys / 2, Black, (int)(15.f * Scale));
						//DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, start_x, start_y, xp, yp + ys / 2, Red, (int)(5.f * Scale));

						if (Scale > 0.6f) {
							DrawControl::Instance()->SetDrawLine(DrawLayer::Back, start_x, start_y, start_x + XAddLine, start_y, Black, (int)(15.f * Scale));
							DrawControl::Instance()->SetDrawLine(DrawLayer::Back, start_x + XAddLine, start_y, xp - XAddLine, yp + ys / 2, Black, (int)(15.f * Scale));
							DrawControl::Instance()->SetDrawLine(DrawLayer::Back, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Black, (int)(15.f * Scale));
						}
						DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, start_x, start_y, start_x + XAddLine, start_y, Red, (int)(5.f * Scale));
						DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, start_x + XAddLine, start_y, xp - XAddLine, yp + ys / 2, Red, (int)(5.f * Scale));
						DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Red, (int)(5.f * Scale));
						//
						if (tasks.GetTaskNeedData().GetParenttaskID().size() > 1) {//2つ以上親がある場合
							for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
								if (p.GetParenttaskID() != ParentID) {
									for (auto& ppos : m_ParentLinePos) {
										if (p.GetParenttaskID() == ppos.m_ID) {
											//
											if (Scale > 0.6f) {
												DrawControl::Instance()->SetDrawLine(DrawLayer::Back3, ppos.m_XPos, ppos.m_YPos, ppos.m_XPos + XAddLine, ppos.m_YPos, Black, (int)(15.f * Scale));
												DrawControl::Instance()->SetDrawLine(DrawLayer::Back3, ppos.m_XPos + XAddLine, ppos.m_YPos, xp - XAddLine, yp + ys / 2, Black, (int)(15.f * Scale));
												DrawControl::Instance()->SetDrawLine(DrawLayer::Back3, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Black, (int)(15.f * Scale));
											}
											DrawControl::Instance()->SetDrawLine(DrawLayer::Back4, ppos.m_XPos, ppos.m_YPos, ppos.m_XPos + XAddLine, ppos.m_YPos, Red, (int)(5.f * Scale));
											DrawControl::Instance()->SetDrawLine(DrawLayer::Back4, ppos.m_XPos + XAddLine, ppos.m_YPos, xp - XAddLine, yp + ys / 2, Red, (int)(5.f * Scale));
											DrawControl::Instance()->SetDrawLine(DrawLayer::Back4, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Red, (int)(5.f * Scale));
											//
										}
									}
								}
							}
						}
					}
					if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ys, false, !WindowMngr->PosHitCheck(nullptr), color, (Scale > 0.2f) ? tasks.GetName() : "")) {
						auto sizeXBuf = y_r(800);
						auto sizeYBuf = y_r(0);
						tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
						//
						signed long long FreeID = tasks.GetID();
						WindowMngr->Add()->Set(xp + xs / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
							TaskData::Instance()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
						});
					}
					int suby = ys;
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						auto* trader2 = TraderData::Instance()->FindPtr(LL.GetTraderID());
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						if (Scale > 0.2f) {
							WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + ys + suby, ys, STR_LEFT, trader2->GetColors(0), Black, "[%4.2f->%4.2f]", PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()], *traderRep);
						}
						suby += ys;
					}
					m_posxMaxBuffer = std::max(m_posxMaxBuffer, xp + xs);
					m_posyMaxBuffer = std::max(m_posyMaxBuffer, yp + ys + suby);
					{
						Rect2D tmp; tmp.Set(xp, yp, xs, suby);
						m_TaskRect.emplace_back(tmp);
					}
					Rect2D P_Next;
					P_Next.Set(xp + (xs + (int)((float)y_r(400) * Scale)), yp, xs, suby);
					//xs, ys
					//被ってたら下に下げる
					while (true) {
						bool isHit = false;
						for (auto&r : m_TaskRect) {
							if (r.IsHit(P_Next)) {
								isHit = true;
								P_Next.Set(xp + (xs + (int)((float)y_r(400) * Scale)), P_Next.GetPosY() + ys * 3 / 2, xs, suby);
								break;
							}
						}
						if (!isHit) { break; }
					}
					m_ParentLinePos.emplace_back(tasks.GetID(), xp + xs, yp + ys / 2);
					m_Drawed.emplace_back(tasks.GetID());
					DrawChildTaskClickBox(Scale, tasks.GetID(), xp + xs, yp + ys / 2, P_Next.GetPosX(), P_Next.GetPosY(), xs, ys, parentCanDo_t);
					//親なのでいったん信頼度を戻す
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						*traderRep = PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()];
					}
					//
					if (ParentID == InvalidID) {
						yp += (ys + (int)((float)y_r(800) * Scale));
					}
					else {
						yp += (ys + suby);
					}
				}
			}
			if (ParentID == InvalidID) {
				m_TaskRect.clear();
			}
		}
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override {
			*Scale = 0.3f;
			int xs = (int)((float)y_r(800) * *Scale);
			int ys = (int)((float)LineHeight * 2 * *Scale);

			*posx = y_r(1920 / 2) - xs / 2;
			*posy = LineHeight + y_r(1080 / 2) - ys / 2;
		}

		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}
		void Draw_Back_Sub(int posx, int posy, float Scale) noexcept override {
			int xs = (int)((float)y_r(800) * Scale);
			int ys = (int)((float)LineHeight * 2 * Scale);
			m_ParentLinePos.clear();
			m_Drawed.clear();
			DrawChildTaskClickBox(Scale, InvalidID, posx + xs, posy + ys / 2, posx, posy, xs, ys);
		}
		void DrawFront_Sub(int posx, int posy, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* DrawParts = DXDraw::Instance();
			//レベル操作
			WindowSystem::SetMsg(y_r(0), y_r(1080) - y_r(36), y_r(0), y_r(1080), y_r(36), STR_LEFT, White, Black, "Level");
			WindowSystem::SetMsg(y_r(200), y_r(1080) - y_r(48), y_r(200), y_r(1080), y_r(48), STR_RIGHT, White, Black, "%d", this->m_MyLevel);
			if (WindowSystem::ClickCheckBox(y_r(0), y_r(1080) - y_r(48) - LineHeight, y_r(100), y_r(1080) - y_r(48), true, !WindowMngr->PosHitCheck(nullptr), Red, "DOWN")) {
				this->m_MyLevel--;
			}
			if (WindowSystem::ClickCheckBox(y_r(100), y_r(1080) - y_r(48) - LineHeight, y_r(200), y_r(1080) - y_r(48), true, !WindowMngr->PosHitCheck(nullptr), Green, "UP")) {
				this->m_MyLevel++;
			}
			this->m_MyLevel = std::clamp(this->m_MyLevel, 1, 71);
			//場所ガイド
			{
				int xp = y_r(1440);
				int yp = y_r(820);

				int xs = y_r(320);
				int ys = y_r(180);

				int x_p1 = std::max(posx * xs / DrawParts->m_DispXSize, -xs / 2);
				int y_p1 = std::max(posy * ys / DrawParts->m_DispYSize, -ys / 2);
				int x_p2 = std::min(this->m_posxMaxBuffer * xs / DrawParts->m_DispXSize, xs + xs / 2);
				int y_p2 = std::min(this->m_posyMaxBuffer * ys / DrawParts->m_DispYSize, ys + ys / 2);

				DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 64);
				DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Black, TRUE);
				DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
				DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Green, FALSE);
				DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp, yp, xp + xs, yp + ys, Red, FALSE);
			}
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};