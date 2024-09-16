#include "MainScene_PageTask.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void TaskBG::DrawChildTaskClickBox(float Scale, TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		if (ParentID == InvalidID) {
			m_posxMaxBuffer = 0;
			m_posyMaxBuffer = 0;
		}
		for (auto& tasks : DataBase::Instance()->GetTaskData()->SetList()) {
			bool IsTrue = false;
			if (std::find_if(m_Drawed.begin(), this->m_Drawed.end(), [&](const TaskID& obj) {return obj == tasks.GetID(); }) == this->m_Drawed.end()) {
				if (!IsTrue && tasks.GetTaskNeedData().GetParenttaskID().size() == 0 && ParentID == InvalidID) {
					IsTrue = true;
				}
				if (!IsTrue && tasks.GetTaskNeedData().GetParenttaskID().size() > 0) {
					for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
						if (p.GetID() == ParentID) {
							IsTrue = true;
							break;
						}
					}
				}
			}

			if (IsTrue) {
				//信頼度チェック
				auto parentCanDo_t = parentCanDo && tasks.GetIsHittoPlayerInfo();
				if (!parentCanDo_t) {
					continue;
				}

				if (ParentID != InvalidID) {
					auto XAddLine = (int)((float)DXDraw::Instance()->GetUIY(25) * Scale);
					if (Scale > 0.6f) {
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back, start_x, start_y, start_x + XAddLine, start_y, Black, (int)(15.f * Scale));
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back, start_x + XAddLine, start_y, xp - XAddLine, yp + ys / 2, Black, (int)(15.f * Scale));
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Black, (int)(15.f * Scale));
					}
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back2, start_x, start_y, start_x + XAddLine, start_y, Red, (int)(5.f * Scale));
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back2, start_x + XAddLine, start_y, xp - XAddLine, yp + ys / 2, Red, (int)(5.f * Scale));
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back2, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Red, (int)(5.f * Scale));
					//
					if (tasks.GetTaskNeedData().GetParenttaskID().size() > 1) {//2つ以上親がある場合
						for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
							if (p.GetID() != ParentID) {
								for (auto& ppos : this->m_ParentLinePos) {
									if (p.GetID() == ppos.m_ID) {
										//
										if (Scale > 0.6f) {
											WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back3, ppos.m_XPos, ppos.m_YPos, ppos.m_XPos + XAddLine, ppos.m_YPos, Black, (int)(15.f * Scale));
											WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back3, ppos.m_XPos + XAddLine, ppos.m_YPos, xp - XAddLine, yp + ys / 2, Black, (int)(15.f * Scale));
											WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back3, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Black, (int)(15.f * Scale));
										}
										WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back4, ppos.m_XPos, ppos.m_YPos, ppos.m_XPos + XAddLine, ppos.m_YPos, Red, (int)(5.f * Scale));
										WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back4, ppos.m_XPos + XAddLine, ppos.m_YPos, xp - XAddLine, yp + ys / 2, Red, (int)(5.f * Scale));
										WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back4, xp - XAddLine, yp + ys / 2, xp, yp + ys / 2, Red, (int)(5.f * Scale));
										//
									}
								}
							}
						}
					}
				}
				tasks.Draw(xp, yp, xs, ys, 0, !WindowMngr->PosHitCheck(nullptr));
				int suby = ys;
				m_posxMaxBuffer = std::max(m_posxMaxBuffer, xp + xs);
				m_posyMaxBuffer = std::max(m_posyMaxBuffer, yp + ys + suby);
				{
					WindowSystem::Rect2D tmp; tmp.Set(xp, yp, xs, suby);
					m_TaskRect.emplace_back(tmp);
				}
				WindowSystem::Rect2D P_Next;
				P_Next.Set(xp + (xs + (int)((float)DXDraw::Instance()->GetUIY(400) * Scale)), yp, xs, suby);
				//xs, ys
				//被ってたら下に下げる
				while (true) {
					bool isHit = false;
					for (auto&r : this->m_TaskRect) {
						if (r.IsHit(P_Next)) {
							isHit = true;
							P_Next.Set(xp + (xs + (int)((float)DXDraw::Instance()->GetUIY(400) * Scale)), P_Next.GetPosY() + ys * 3 / 2, xs, suby);
							break;
						}
					}
					if (!isHit) { break; }
				}
				m_ParentLinePos.emplace_back(tasks.GetID(), xp + xs, yp + ys / 2);
				m_Drawed.emplace_back(tasks.GetID());
				DrawChildTaskClickBox(Scale, tasks.GetID(), xp + xs, yp + ys / 2, P_Next.GetPosX(), P_Next.GetPosY(), xs, ys, parentCanDo_t);
				//
				if (ParentID == InvalidID) {
					yp += (ys + (int)((float)DXDraw::Instance()->GetUIY(800) * Scale));
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

	void TaskBG::DrawTab(int xp, int yp, std::string_view Info) noexcept {
		xp -= DXDraw::Instance()->GetUIY(64 + 4);
		auto size = WindowSystem::GetMsgLen(LineHeight, Info);
		WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_RIGHT, White, Black, Info);
		WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal, xp - size, yp + LineHeight, xp + DXDraw::Instance()->GetUIY(64), yp + LineHeight, Gray25, DXDraw::Instance()->GetUIY(4));
	}
	void TaskBG::DrawCheckBox(int xp, int yp, std::string_view Info, bool* Check) noexcept {
		xp -= DXDraw::Instance()->GetUIY(64 + 4);
		auto OLD = *Check;
		*Check = WindowSystem::CheckBox(xp, yp, *Check);
		WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_RIGHT, White, Black, Info);
		if (OLD != *Check) {
			//m_IsListChange = true;
		}
	}

	void TaskBG::Init_Sub(int *posx, int *posy, float* Scale) noexcept {
		*Scale = 0.3f;

		*posx = DXDraw::Instance()->GetUIY(20);
		*posy = LineHeight + DXDraw::Instance()->GetUIY(120);

		m_Mode = EnumTaskDrawMode::Normal;

		m_DrawClearTask = true;
		m_DrawNotClearTask = true;
		m_DrawCanClearTask = false;
	}

	void TaskBG::LateExecute_Sub(int*posx, int*posy, float*Scale) noexcept {
		if (m_Mode == EnumTaskDrawMode::List) {
			*Scale = 0.3f;

			*posx = DXDraw::Instance()->GetUIY(20);
			*posy = LineHeight + DXDraw::Instance()->GetUIY(120);
		}
	}
	void TaskBG::Draw_Back_Sub(int posx, int posy, float Scale) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		switch (m_Mode) {
		case EnumTaskDrawMode::Tree:
		{
			int xs = (int)((float)DXDraw::Instance()->GetUIY(800) * Scale);
			int ys = (int)((float)LineHeight * 2 * Scale);
			m_ParentLinePos.clear();
			m_Drawed.clear();
			DrawChildTaskClickBox(Scale, InvalidID, posx + xs, posy + ys / 2, posx, posy, xs, ys);
		}
		break;
		case EnumTaskDrawMode::Fir:
		{
			std::vector<std::vector<std::pair<ItemID, int>>> Counter;
			Counter.resize(DataBase::Instance()->GetItemTypeData()->GetList().size());
			for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
				bool IsChecktask = tasks.GetIsHittoPlayerInfo();
				if (PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str())) {
					IsChecktask = false;
				}
				if (!IsChecktask) { continue; }
				for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
					auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
					if (ptr) {
						auto& Types = Counter.at(ptr->GetTypeID());
						auto Find = std::find_if(Types.begin(), Types.end(), [&](const std::pair<ItemID, int>& obj) {return obj.first == w.GetID(); });
						if (Find != Types.end()) {
							Find->second += w.GetValue();
						}
						else {
							Types.emplace_back(std::make_pair(w.GetID(), w.GetValue()));
						}
					}
				}
			}
			//描画
			{
				int xpBase = posx;
				int ypBase = posy;
				int xp = xpBase;
				int yp = ypBase;
				int xsize = (int)((float)DXDraw::Instance()->GetUIY(640)*Scale);
				int ysize = (int)((float)DXDraw::Instance()->GetUIY(96)*Scale);
				int xsizeAdd = (int)((float)DXDraw::Instance()->GetUIY(640 + 30)*Scale);
				int ysizeAdd = (int)((float)DXDraw::Instance()->GetUIY(96 + 15)*Scale);

				for (auto& Cat : DataBase::Instance()->GetItemCategoryData()->GetList()) {
					bool IsHit = false;
					for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
						if (Type.GetCategoryID() == Cat.GetID()) {
							auto& Types = Counter.at(Type.GetID());
							for (auto& c : Types) {
								auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.first);
								if (ptr) {
									ptr->Draw(xp, yp, xsize, ysize, (c.second >= 2) ? c.second : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, Scale < 0.6f);
									yp += ysizeAdd;
									if ((yp - ypBase) >= (int)((float)DXDraw::Instance()->GetUIY(1920)*Scale)) {
										xp += xsizeAdd;
										yp = ypBase;
									}
									IsHit = true;
								}
							}
						}
					}
					if (IsHit) {
						xp += xsizeAdd;
						yp = ypBase;
					}
				}
			}
		}
		break;
		case EnumTaskDrawMode::NotFir:
		{
			std::vector<std::vector<std::pair<ItemID, int>>> Counter;
			Counter.resize(DataBase::Instance()->GetItemTypeData()->GetList().size());
			for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
				bool IsChecktask = tasks.GetIsHittoPlayerInfo();
				if (PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str())) {
					IsChecktask = false;
				}
				if (!IsChecktask) { continue; }
				for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
					auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
					if (ptr) {
						auto& Types = Counter.at(ptr->GetTypeID());
						auto Find = std::find_if(Types.begin(), Types.end(), [&](const std::pair<ItemID, int>& obj) {return obj.first == w.GetID(); });
						if (Find != Types.end()) {
							Find->second += w.GetValue();
						}
						else {
							Types.emplace_back(std::make_pair(w.GetID(), w.GetValue()));
						}
					}
				}
			}
			//描画
			{
				int xpBase = posx;
				int ypBase = posy;
				int xp = xpBase;
				int yp = ypBase;
				int xsize = (int)((float)DXDraw::Instance()->GetUIY(640)*Scale);
				int ysize = (int)((float)DXDraw::Instance()->GetUIY(96)*Scale);
				int xsizeAdd = (int)((float)DXDraw::Instance()->GetUIY(640 + 30)*Scale);
				int ysizeAdd = (int)((float)DXDraw::Instance()->GetUIY(96 + 15)*Scale);

				for (auto& Cat : DataBase::Instance()->GetItemCategoryData()->GetList()) {
					bool IsHit = false;
					for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
						if (Type.GetCategoryID() == Cat.GetID()) {
							auto& Types = Counter.at(Type.GetID());
							for (auto& c : Types) {
								auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.first);
								if (ptr) {
									ptr->Draw(xp, yp, xsize, ysize, (c.second >= 2) ? c.second : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, Scale < 0.6f);
									yp += ysizeAdd;
									if ((yp - ypBase) >= (int)((float)DXDraw::Instance()->GetUIY(1920)*Scale)) {
										xp += xsizeAdd;
										yp = ypBase;
									}
									IsHit = true;
								}
							}
						}
					}
					if (IsHit) {
						xp += xsizeAdd;
						yp = ypBase;
					}
				}
			}
		}
		break;
		case EnumTaskDrawMode::List:
		{
			int xpos = DXDraw::Instance()->GetUIY(50);
			int ypos = LineHeight + DXDraw::Instance()->GetUIY(10) + LineHeight + DXDraw::Instance()->GetUIY(10);

			//描画
			{
				int xpBase = xpos;
				int ypBase = ypos - (int)m_YNow;
				int xp = xpBase;
				int yp = ypBase;
				int ypMax = (DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(40));
				int xsize = DXDraw::Instance()->GetUIY(1200);
				int ysize = LineHeight;
				int xsizeAdd = xsize + DXDraw::Instance()->GetUIY(30);
				int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);

				for (auto& tasks : DataBase::Instance()->GetTaskData()->SetList()) {
					{
						bool isHit = false;
						//クリアチェック
						bool IsClearTask = PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str());
						if (m_DrawClearTask) {
							if (IsClearTask) { isHit = true; }
						}
						if (m_DrawNotClearTask) {
							if (!IsClearTask) { isHit = true; }
						}
						//クリアできるタスクだけ表示
						if (isHit && this->m_DrawCanClearTask) {
							if (tasks.GetTaskNeedData().GetParenttaskID().size() > 0) {
								for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
									if (!PlayerData::Instance()->GetTaskClear(DataBase::Instance()->GetTaskData()->FindPtr(p.GetID())->GetName().c_str())) {
										isHit = false;
										break;
									}
								}
							}
						}
						//
						if (isHit) {
							isHit = tasks.GetIsHittoPlayerInfo();
						}
						//
						if (!isHit) { continue; }
					}
					if (ypos - ysizeAdd < yp && yp < ypMax) {
						if (ypos - 1 < yp && yp < ypMax - ysizeAdd + 1) {
							WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
						}
						else {
							if (yp <= ypos) {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypos - yp) / ysizeAdd, 0, 255));
							}
							else {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
							}
						}
						tasks.Draw(xp, yp, xsize, ysize, 0, !WindowMngr->PosHitCheck(nullptr));
					}
					yp += ysizeAdd;
				}
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);

				int ScrPosX = xpos + xsizeAdd;
				int ScrSizY = ypMax - ypos;
				m_Scroll.SetScrollBoxParam(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp - ypBase, ScrSizY) / (float)ScrSizY, !WindowMngr->PosHitCheck(nullptr));
				m_Scroll.ScrollBox();
				m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer()*(float)((yp - ypBase) - ScrSizY));
			}
		}
		break;
		default:
			break;
		}
	}
	void TaskBG::DrawFront_Sub(int posx, int posy, float) noexcept {
		auto* Pad = PadControl::Instance();
		//モードセレクト
		{
			int xp = DXDraw::Instance()->GetUIY(10) + DXDraw::Instance()->GetUIY(200) + DXDraw::Instance()->GetUIY(10);
			int yp = LineHeight + DXDraw::Instance()->GetUIY(10);
			if (WindowSystem::SetMsgClickBox(xp, yp, xp + DXDraw::Instance()->GetUIY(400), yp + LineHeight, LineHeight, Gray25, false, true, TaskDrawModeStr[(int)m_Mode])) {
				m_Mode = (EnumTaskDrawMode)(((int)m_Mode + 1) % (int)EnumTaskDrawMode::Max);
			}
			int Max = (int)EnumTaskDrawMode::Max;
			for (int i = 0; i < Max; i++) {
				WindowSystem::SetBox(
					xp + DXDraw::Instance()->GetUIY(400)*i / Max + DXDraw::Instance()->GetUIY(5), yp + LineHeight + DXDraw::Instance()->GetUIY(4),
					xp + DXDraw::Instance()->GetUIY(400)*(i + 1) / Max - DXDraw::Instance()->GetUIY(5), yp + LineHeight + DXDraw::Instance()->GetUIY(4) + DXDraw::Instance()->GetUIY(6),
					(m_Mode == (EnumTaskDrawMode)i) ? Green : Gray25);
			}
		}
		//
		if (m_Mode == EnumTaskDrawMode::Tree) {
			//場所ガイド
			SetPositionGuide(posx, posy, this->m_posxMaxBuffer, this->m_posyMaxBuffer);
		}
		//
		if (m_Mode == EnumTaskDrawMode::Tree || this->m_Mode == EnumTaskDrawMode::List) {
			//
			{
				if (!Pad->GetKey(PADS::JUMP).press()) {
					WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
						FontPool::FontType::MS_Gothic, LineHeight,
						STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
						"スペースを押しながらクリックでタスククリア"
					);
				}
			}
		}
		//
		if (m_Mode == EnumTaskDrawMode::List) {
			int xp = DXDraw::Instance()->GetUIY(1920) - DXDraw::Instance()->GetUIY(16);
			int yp = DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(110) - DXDraw::Instance()->GetUIY(40) * 9;
			//背景
			{
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 32);
				WindowSystem::SetBox(xp - DXDraw::Instance()->GetUIY(340), yp, xp + DXDraw::Instance()->GetUIY(5), yp + DXDraw::Instance()->GetUIY(40) * 9 + DXDraw::Instance()->GetUIY(5), Black);
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
			}
			//
			{
				DrawTab(xp, yp, "タスクを表示:"); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "クリア済", &m_DrawClearTask); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "未クリア", &m_DrawNotClearTask); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "クリア可能", &m_DrawCanClearTask); yp += DXDraw::Instance()->GetUIY(40);
			}
			//
			{
				DrawTab(xp, yp, "凡例:"); yp += DXDraw::Instance()->GetUIY(40);
				int ysize = DXDraw::Instance()->GetUIY(40);
				int ysizeI = ysize - 4;
				{
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 255);
					WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "敵をキル");
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 128, 128, 128);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_Kill").GetGraph(), xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_Kill").GetGraph(), xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_Kill").GetGraph(), xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_Kill").GetGraph(), xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 0, 0);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_Kill").GetGraph(), xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					yp += DXDraw::Instance()->GetUIY(40);
				}
				{
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 255);
					WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "FiR品を納品");
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 32, 32, 32);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 0);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					yp += DXDraw::Instance()->GetUIY(40);
				}
				{
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 255);
					WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "NotFiR品を納品");
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 192, 192, 192);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 0, 0, 255);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Task_FiR").GetGraph(), xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					yp += DXDraw::Instance()->GetUIY(40);
				}
				{
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 255);
					WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "マップ指定");
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 255);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Map").GetGraph(), xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Map").GetGraph(), xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Map").GetGraph(), xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Map").GetGraph(), xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 0, 128, 0);
					WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetGuide("Map").GetGraph(), xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
					yp += DXDraw::Instance()->GetUIY(40);
				}
				WindowSystem::DrawControl::Instance()->SetBright(WindowSystem::DrawLayer::Normal, 255, 255, 255);
			}
		}
	}
};
