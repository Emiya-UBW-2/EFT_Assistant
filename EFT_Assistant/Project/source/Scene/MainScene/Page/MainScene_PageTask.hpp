#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	class TaskBG :public BGParent {
	private:
		int															m_MyLevel{ 1 };
		int															m_posxMaxBuffer{ 0 };
		int															m_posyMaxBuffer{ 0 };
		std::vector<Rect2D>											m_TaskRect;
		std::vector<GraphHandle>									m_TaskGraph;
		std::vector<std::shared_ptr<WindowSystem::WindowControl>>	m_TaskPtr;
	private:
		void DrawChildTaskClickBox(std::unique_ptr<WindowSystem::WindowManager>& Windowup, float Scale, TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) noexcept {
			if (ParentID == InvalidID) {
				m_posxMaxBuffer = 0;
				m_posyMaxBuffer = 0;
			}
			for (const auto& tasks : TaskData::Instance()->GetTaskList()) {
				if (ParentID == InvalidID) {
					TraderData::Instance()->ResetRep();
				}
				if (tasks.GetTaskNeedData().GettaskID() == ParentID) {
					auto* trader = TraderData::Instance()->FindPtr(tasks.GetTrader());
					auto parentCanDo_t = parentCanDo;
					auto color = trader->GetColors(0);
					//�M���x�A�b�v�_�E����Ή�
					std::vector<float> PrevRep;
					PrevRep.resize(tasks.GetTaskRewardData().GetLLAdd().size());
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()] = *traderRep;
						*traderRep += LL.GetAdd();
					}
					//�M���x�`�F�b�N
					if (
						(
						(this->m_MyLevel < tasks.GetTaskNeedData().GetLevel())
							) || !parentCanDo) {
						color = trader->GetColors(-100);
						parentCanDo_t = false;
					}
					if (ParentID != InvalidID) {
						DrawLine(start_x, start_y, xp, yp + ys / 2, Red, (int)(5.f * Scale));
					}
					if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ys, false, !Windowup->PosHitCheck(), color, (Scale > 0.5f) ? tasks.GetName() : "")) {
						auto sizeXBuf = y_r(640);
						auto sizeYBuf = y_r(0);
						tasks.DrawWindow(0, 0, &sizeXBuf, &sizeYBuf);//�����ɃT�C�Y�v��
						m_TaskGraph.emplace_back(GraphHandle::Make(sizeXBuf, sizeYBuf, true));
						m_TaskGraph.back().SetDraw_Screen();
						{
							tasks.DrawWindow(0, 0, &sizeXBuf, &sizeYBuf);
						}
						SetDrawScreen(DX_SCREEN_BACK);
						//
						m_TaskPtr.emplace_back(Windowup->Add());
						m_TaskPtr.back()->Set(xp + xs / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, [&](WindowSystem::WindowControl* win) {
							for (auto& t : m_TaskPtr) {
								if (t.get() == win) {
									m_TaskGraph[&t - &m_TaskPtr.front()].DrawGraph(win->GetPosX(), win->GetPosY(), true);
									break;
								}
							}
						});
					}
					int suby = ys;
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						auto* trader2 = TraderData::Instance()->FindPtr(LL.GetTraderID());
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						if (Scale > 0.5f) {
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
					P_Next.Set(xp + (xs + (int)((float)y_r(100) * Scale)), yp, xs, suby);
					//xs, ys
					//����Ă��牺�ɉ�����
					while (true) {
						bool isHit = false;
						for (auto&r : m_TaskRect) {
							if (r.IsHit(P_Next)) {
								isHit = true;
								P_Next.Set(xp + (xs + (int)((float)y_r(50) * Scale)), P_Next.GetPosY() + ys, xs, suby);
								break;
							}
						}
						if (!isHit) { break; }
					}
					DrawChildTaskClickBox(Windowup, Scale, tasks.GetID(), xp + xs, yp + ys / 2, P_Next.GetPosX(), P_Next.GetPosY(), xs, ys, parentCanDo_t);
					//�e�Ȃ̂ł�������M���x��߂�
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						*traderRep = PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()];
					}
					//
					if (ParentID == InvalidID) {
						yp += (ys + (int)((float)y_r(400) * Scale));
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
		void LateExecute_Sub(void) noexcept override {
			//�E�B���h�E�Ƃ̓���
			for (int i = 0; i < m_TaskPtr.size(); i++) {
				if (m_TaskPtr[i].use_count() <= 1) {
					std::swap(m_TaskGraph[i], m_TaskGraph.back());
					std::swap(m_TaskPtr[i], m_TaskPtr.back());
					m_TaskGraph.back().Dispose();
					m_TaskPtr.back().reset();
					m_TaskGraph.pop_back();
					m_TaskPtr.pop_back();
					i--;
				}
			}
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float Scale) noexcept override {
			int xs = (int)((float)y_r(640) * Scale);
			int ys = (int)((float)LineHeight * Scale);
			DrawChildTaskClickBox(Windowup, Scale, InvalidID, posx + xs, posy + ys / 2, posx, posy, xs, ys);
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			//���x������
			WindowSystem::SetMsg(y_r(0), y_r(1080) - y_r(36), y_r(0), y_r(1080), y_r(36), STR_LEFT, White, Black, "Level");
			WindowSystem::SetMsg(y_r(200), y_r(1080) - y_r(48), y_r(200), y_r(1080), y_r(48), STR_RIGHT, White, Black, "%d", this->m_MyLevel);
			if (WindowSystem::ClickCheckBox(y_r(0), y_r(1080) - y_r(48) - LineHeight, y_r(100), y_r(1080) - y_r(48), true, !Windowup->PosHitCheck(), Red, "DOWN")) {
				this->m_MyLevel--;
			}
			if (WindowSystem::ClickCheckBox(y_r(100), y_r(1080) - y_r(48) - LineHeight, y_r(200), y_r(1080) - y_r(48), true, !Windowup->PosHitCheck(), Green, "UP")) {
				this->m_MyLevel++;
			}
			this->m_MyLevel = std::clamp(this->m_MyLevel, 1, 71);
			//�ꏊ�K�C�h
			{
				int xp = y_r(1440);
				int yp = y_r(820);

				int xs = y_r(320);
				int ys = y_r(180);

				int x_p1 = std::max(posx * xs / DrawParts->m_DispXSize, -xs / 2);
				int y_p1 = std::max(posy * ys / DrawParts->m_DispYSize, -ys / 2);
				int x_p2 = std::min(this->m_posxMaxBuffer * xs / DrawParts->m_DispXSize, xs + xs / 2);
				int y_p2 = std::min(this->m_posyMaxBuffer * ys / DrawParts->m_DispYSize, ys + ys / 2);

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
				DrawBox(xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Black, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawBox(xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Green, FALSE);
				DrawBox(xp, yp, xp + xs, yp + ys, Red, FALSE);
			}
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "�߂�")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
			for (int i = 0; i < m_TaskPtr.size(); i++) {
				m_TaskGraph.back().Dispose();
				m_TaskPtr.back().reset();
				m_TaskGraph.pop_back();
				m_TaskPtr.pop_back();
			}
		}
	};
};