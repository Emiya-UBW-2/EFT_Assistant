#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	enum class EnumTaskDrawMode {
		Normal,
		List = Normal,
		Tree,
		Fir,
		NotFir,
		Max,
	};
	static const char* TaskDrawModeStr[(int)EnumTaskDrawMode::Max] = {
	"タスク一覧",
	"ツリー",
	"タスクFirアイテム",
	"タスクNotFirアイテム",
	};


	class TaskBG :public BGParent {
	private:
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
		EnumTaskDrawMode											m_Mode{ EnumTaskDrawMode::Normal };

		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };

		bool							m_DrawClearTask{ true };
		bool							m_DrawNotClearTask{ true };
		bool							m_DrawCanClearTask{ false };
	private:
		void DrawChildTaskClickBox(float Scale, TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			if (ParentID == InvalidID) {
				m_posxMaxBuffer = 0;
				m_posyMaxBuffer = 0;
			}
			for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
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
						auto XAddLine = (int)((float)y_r(25) * Scale);
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
								if (p.GetID() != ParentID) {
									for (auto& ppos : this->m_ParentLinePos) {
										if (p.GetID() == ppos.m_ID) {
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
					tasks.Draw(xp, yp, xs, ys, 0, !WindowMngr->PosHitCheck(nullptr));
					int suby = ys;
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
						for (auto&r : this->m_TaskRect) {
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

		void DrawTab(int xp, int yp, std::string_view Info) noexcept {
			xp -= y_r(64 + 4);
			auto size = WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, Info);
			DrawControl::Instance()->SetDrawLine(DrawLayer::Normal, xp - size, yp + LineHeight, xp + y_r(64), yp + LineHeight, Gray25, y_r(4));
		}
		void DrawCheckBox(int xp, int yp, std::string_view Info, bool* Check) noexcept {
			xp -= y_r(64 + 4);
			auto OLD = *Check;
			WindowSystem::CheckBox(xp, yp, Check);
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, Info);
			if (OLD != *Check) {
				//m_IsListChange = true;
			}
		}
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override {
			*Scale = 0.3f;

			*posx = y_r(20);
			*posy = LineHeight + y_r(120);

			m_Mode = EnumTaskDrawMode::Normal;

			m_DrawClearTask = true;
			m_DrawNotClearTask = true;
			m_DrawCanClearTask = false;
		}

		void LateExecute_Sub(int*posx, int*posy, float*Scale) noexcept override {
			if (m_Mode == EnumTaskDrawMode::List) {
				*Scale = 0.3f;

				*posx = y_r(20);
				*posy = LineHeight + y_r(120);
			}
		}
		void Draw_Back_Sub(int posx, int posy, float Scale) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			switch (m_Mode) {
			case EnumTaskDrawMode::Tree:
			{
				int xs = (int)((float)y_r(800) * Scale);
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
					int xsize = (int)((float)y_r(640)*Scale);
					int ysize = (int)((float)y_r(96)*Scale);
					int xsizeAdd = (int)((float)y_r(640 + 30)*Scale);
					int ysizeAdd = (int)((float)y_r(96 + 15)*Scale);

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
										if ((yp - ypBase) >= (int)((float)y_r(1920)*Scale)) {
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
					int xsize = (int)((float)y_r(640)*Scale);
					int ysize = (int)((float)y_r(96)*Scale);
					int xsizeAdd = (int)((float)y_r(640 + 30)*Scale);
					int ysizeAdd = (int)((float)y_r(96 + 15)*Scale);

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
										if ((yp - ypBase) >= (int)((float)y_r(1920)*Scale)) {
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
				int xpos = y_r(50);
				int ypos = LineHeight + y_r(10) + LineHeight + y_r(10);

				//描画
				{
					int xpBase = xpos;
					int ypBase = ypos - (int)m_YNow;
					int xp = xpBase;
					int yp = ypBase;
					int ypMax = (y_r(1080) - y_r(40));
					int xsize = y_r(1200);
					int ysize = LineHeight;
					int xsizeAdd = xsize + y_r(30);
					int ysizeAdd = ysize + y_r(5);

					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
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
								DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
							}
							else {
								if (yp <= ypos) {
									DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (ypos - yp) / ysizeAdd, 0, 255));
								}
								else {
									DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (yp - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
								}
							}
							tasks.Draw(xp, yp, xsize, ysize, 0, !WindowMngr->PosHitCheck(nullptr));
						}
						yp += ysizeAdd;
					}
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);

					int ScrPosX = xpos + xsizeAdd;
					int ScrSizY = ypMax - ypos;
					m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp - ypBase, ScrSizY) / (float)ScrSizY, !WindowMngr->PosHitCheck(nullptr));

					m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer()*(float)((yp - ypBase) - ScrSizY));
				}
			}
			break;
			default:
				break;
			}
		}
		void DrawFront_Sub(int posx, int posy, float) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			auto* Input = InputControl::Instance();
			//戻る
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					TurnOnGoNextBG();
				}
			}
			//モードセレクト
			{
				int xp = y_r(10) + y_r(200) + y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(400), yp + LineHeight, false, true, Gray25, TaskDrawModeStr[(int)m_Mode])) {
					m_Mode = (EnumTaskDrawMode)(((int)m_Mode + 1) % (int)EnumTaskDrawMode::Max);
				}
				int Max = (int)EnumTaskDrawMode::Max;
				for (int i = 0; i < Max; i++) {
					WindowSystem::SetBox(
						xp + y_r(400)*i / Max + y_r(5), yp + LineHeight + y_r(4),
						xp + y_r(400)*(i + 1) / Max - y_r(5), yp + LineHeight + y_r(4) + y_r(6),
						(m_Mode == (EnumTaskDrawMode)i) ? Green : Gray25);
				}
			}
			//
			if (m_Mode == EnumTaskDrawMode::Tree) {
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
			}
			//
			if (m_Mode == EnumTaskDrawMode::Tree || this->m_Mode == EnumTaskDrawMode::List) {
				//
				{
					if (!Input->GetSpaceKey().press()) {
						DrawControl::Instance()->SetString(DrawLayer::Front,
							FontPool::FontType::Nomal_Edge, LineHeight,
							STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
							"スペースを押しながらクリックでタスククリア"
						);
					}
				}
			}
			//
			if (m_Mode == EnumTaskDrawMode::List) {
				int xp = y_r(1920) - y_r(16);
				int yp = y_r(1080) - y_r(110) - y_r(40) * 9;
				//背景
				{
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 32);
					DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp - y_r(340), yp, xp + y_r(5), yp + y_r(40) * 9 + y_r(5), Black, true);
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
				}
				//
				{
					DrawTab(xp, yp, "タスクを表示:"); yp += y_r(40);
					DrawCheckBox(xp, yp, "クリア済", &m_DrawClearTask); yp += y_r(40);
					DrawCheckBox(xp, yp, "未クリア", &m_DrawNotClearTask); yp += y_r(40);
					DrawCheckBox(xp, yp, "クリア可能", &m_DrawCanClearTask); yp += y_r(40);
				}
				//
				{
					DrawTab(xp, yp, "凡例:"); yp += y_r(40);
					int ysize = y_r(40);
					int ysizeI = ysize - 4;
					{
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 255, 255);
						WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "敵をキル");
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 128, 128, 128);
						DrawControl::Instance()->SetDrawRotaGuide("Task_Kill", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_Kill", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_Kill", DrawLayer::Normal, xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_Kill", DrawLayer::Normal, xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 0, 0);
						DrawControl::Instance()->SetDrawRotaGuide("Task_Kill", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						yp += y_r(40);
					}
					{
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 255, 255);
						WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "FiR品を納品");
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 32, 32, 32);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 255, 0);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						yp += y_r(40);
					}
					{
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 255, 255);
						WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "NotFiR品を納品");
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 192, 192, 192);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 0, 0, 255);
						DrawControl::Instance()->SetDrawRotaGuide("Task_FiR", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						yp += y_r(40);
					}
					{
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 255, 255);
						WindowSystem::SetMsg(xp - ysize, yp + ysize / 2, xp - ysize, yp + ysize / 2, LineHeight, STRX_RIGHT, White, Black, "マップ指定");
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 255, 255, 255);
						DrawControl::Instance()->SetDrawRotaGuide("Map", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 + 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Map", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2 - 1, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Map", DrawLayer::Normal, xp - ysize / 2 - 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetDrawRotaGuide("Map", DrawLayer::Normal, xp - ysize / 2 + 1, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						DrawControl::Instance()->SetBright(DrawLayer::Normal, 0, 128, 0);
						DrawControl::Instance()->SetDrawRotaGuide("Map", DrawLayer::Normal, xp - ysize / 2, yp + ysize / 2, (float)(ysizeI) / (float)(64.f), 0.f, true);
						yp += y_r(40);
					}
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};
