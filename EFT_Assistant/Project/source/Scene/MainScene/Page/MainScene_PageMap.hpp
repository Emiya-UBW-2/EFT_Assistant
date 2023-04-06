#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	static const char* WatchMapTypeStr[3] = {
		"InGameMap",
		"2DMap",
		"Else",
	};

	class MapBG :public BGParent {
	private:
		MapID	m_SelectBuffer{ InvalidID };
		MapID	m_MapSelect{ InvalidID };
		int	m_WatchMapSelect{ 0 };
		float m_Rad{ 0.f };
		float m_Rad_Goal{ 0.f };
		float m_BaseRad{ 0.f };
		bool isLoaded{ false };

		int mouse_moveX{ 0 };
		int mouse_moveY{ 0 };

		Graphs		ComPass;
		float		m_ComPassRad{ 0.f };

		TraderID	m_EditTraderID{ InvalidID };
		TaskID		m_EditTaskID{ InvalidID };
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override {
			auto* Input = InputControl::Instance();
			*posx = y_r(960);
			*posy = y_r(540);
			*Scale = 1.f;

			m_Rad = 0.f;
			m_Rad_Goal = 0.f;
			m_BaseRad = 0.f;
			m_ComPassRad = 0.f;

			ComPass.SetPath("data/Compass.png");
			ComPass.SetIsTrans(true);
			ComPass.LoadByPath(false);
			ComPass.WhenAfterLoad();

			mouse_moveX = Input->GetMouseX();							//ドラッグ前のマウス座標格納
			mouse_moveY = Input->GetMouseY();

			m_EditTraderID = InvalidID;
			//m_EditTaskID = InvalidID;
		}
		void LateExecute_Sub(int *, int *, float*) noexcept override {
			auto* Input = InputControl::Instance();
			if (m_MapSelect != m_SelectBuffer) {
				if (m_MapSelect != InvalidID) {
					auto* MapPtr = MapData::Instance()->FindPtr(m_MapSelect);
					MapPtr->DisposeMapPics();
				}
				m_MapSelect = m_SelectBuffer;
				{
					auto* MapPtr = MapData::Instance()->FindPtr(m_MapSelect);
					if (m_WatchMapSelect <= MapPtr->GetMapPicNum()) { m_WatchMapSelect = 0; }
					m_BaseRad = MapPtr->GetMapNorthRad(m_WatchMapSelect);
					m_Rad = 0.f;
					m_Rad_Goal = 0.f;
					SetUseASyncLoadFlag(TRUE);
					MapPtr->LoadMapPics();
					SetUseASyncLoadFlag(FALSE);
				}
				isLoaded = true;
			}
			if (GetASyncLoadNum() == 0) {
				if (isLoaded) {
					isLoaded = false;
					auto* MapPtr = MapData::Instance()->FindPtr(m_MapSelect);
					MapPtr->WhenAfterLoadMapPics();
				}
			}
			//ドラッグ開始時の処理
			bool notpress = true;
			if (!Input->GetRightClick().press() && !Input->GetMiddleClick().press()) {
				if (Input->GetLeftClick().press()) {
					m_Rad_Goal += deg2rad(Input->GetMouseX() - mouse_moveX);
					//Input->GetMouseY() - mouse_moveY;
					HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEWE);
					SetCursor(hCursor);
					notpress = false;
				}
				mouse_moveX = Input->GetMouseX();							//ドラッグ前のマウス座標格納
				mouse_moveY = Input->GetMouseY();
			}
			Easing(&m_Rad, m_Rad_Goal, 0.8f, EasingType::OutExpo);
			Easing(&m_ComPassRad, (m_BaseRad + m_Rad), 0.8f, EasingType::OutExpo);
		}
		void Draw_Back_Sub(int posx, int posy, float Scale) noexcept override {
			if (m_MapSelect != InvalidID) {
				auto* MapPtr = MapData::Instance()->FindPtr(m_MapSelect);
				auto* Input = InputControl::Instance();
				auto* GraphPtr = MapPtr->GetMapGraph((int)m_WatchMapSelect);
				TaskList* TaskPtr = (m_EditTaskID != InvalidID) ? TaskData::Instance()->FindPtr(m_EditTaskID) : nullptr;
				if (GraphPtr) {
					DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GraphPtr, posx, posy, Scale / 2.f, m_Rad, false);
					float XSize = (float)MapPtr->GetMapXSize((int)m_WatchMapSelect)*Scale / 2.f;
					float YSize = (float)MapPtr->GetMapYSize((int)m_WatchMapSelect)*Scale / 2.f;
					{
						float Xper = ((float)Input->GetMouseX() - (float)posx);
						float Yper = ((float)Input->GetMouseY() - (float)posy);
						float Len = std::hypotf(Xper, Yper);
						float Rad = std::atan2f(Xper, Yper);

						Xper = Len * std::sin(Rad + m_Rad);
						Yper = Len * std::cos(Rad + m_Rad);

						Xper = Xper / XSize;
						Yper = Yper / YSize;
						if (Input->GetSpaceKey().trigger()) {
							m_EditTaskID++;
						}

						//
						{
							int xp = y_r(10);
							int yp = LineHeight + y_r(20);
							MakeList<TraderList>(xp, yp, TraderData::Instance()->GetList(), "", (int*)&m_EditTraderID, true, false, false, [&](const auto*) { return true; });
						}
						//
						{
							int xp = y_r(10 + 400 + 10);
							int yp = LineHeight + y_r(20);
							MakeList<TaskList>(xp, yp, TaskData::Instance()->GetList(), "", (int*)&m_EditTaskID, true, false, false, [&](const auto* tgt) {
								if (tgt->GetTrader() != m_EditTraderID && m_EditTraderID != InvalidID) {
									return false;
								}

								for (const auto& m : tgt->GetTaskWorkData().GetMap()) {
									if (m == m_MapSelect) { return true; }
								}
								return false;
							});
						}

						if (TaskPtr) {
							auto& Pins = TaskPtr->SetTaskWorkData().SetPin();
							if (Input->GetLeftClick().trigger()) {
								if (Input->GetShiftKey().press()) {
									if ((-0.5f <= Xper && Xper <= 0.5f) && (-0.5f <= Yper && Yper <= 0.5f)) {
										Pins.resize(Pins.size() + 1);
										Pins.back().m_Point.Set(Xper, Yper, 0.f);
										Pins.back().m_MapID = MapPtr->GetID();
										Pins.back().m_MapSel = m_WatchMapSelect;
									}
								}
							}
						}
					}
					if (TaskPtr) {
						auto& Pins = TaskPtr->SetTaskWorkData().SetPin();
						for (int loop = 0; loop < Pins.size(); loop++) {
							const auto& p = Pins[loop];
							if (p.m_MapID == MapPtr->GetID() && p.m_MapSel == m_WatchMapSelect) {
								float Xs = XSize * p.m_Point.x();
								float Ys = YSize * p.m_Point.y();
								int xp = posx + (int)(std::cos(m_Rad)*Xs - std::sin(m_Rad)*Ys);
								int yp = posy + (int)(std::cos(m_Rad)*Ys + std::sin(m_Rad)*Xs);
								DrawControl::Instance()->SetDrawCircle(DrawLayer::Front, xp, yp, 6, Black);
								DrawControl::Instance()->SetDrawCircle(DrawLayer::Front, xp, yp, 5, Green);
								if (Input->GetRightClick().trigger()) {
									if (Input->GetShiftKey().press()) {
										if (std::hypotf((float)(Input->GetMouseX() - xp), (float)(Input->GetMouseY() - yp)) < 5.f) {
											Pins.erase(Pins.begin() + loop);
											loop--;
										}
									}
								}
							}
						}
					}
				}
				if (TaskPtr) {
					int xp = y_r(960);
					int yp = y_r(1080);
					int ys = LineHeight;
					WindowSystem::SetMsg(xp, yp - ys * 3, xp, yp - ys * 2, ys, STRX_MID, Green, Black, "Shift+左クリックClick　ピン打ち");
					WindowSystem::SetMsg(xp, yp - ys * 2, xp, yp - ys * 1, ys, STRX_MID, Green, Black, "Shift+右クリックClick　ピン消し");
					WindowSystem::SetMsg(xp, yp - ys * 1, xp, yp - ys * 0, ys, STRX_MID, Green, Black, "タスク[%s]を編集中", TaskPtr->GetName().c_str());
				}
			}
			if (ComPass.GetGraph()) {
				DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, ComPass.GetGraph(), y_r(100), y_r(1080 - 100), 1.f, m_ComPassRad, true);
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//
			{
				int xp = y_r(1920 - 400 - 10);
				int yp = LineHeight + y_r(10);
				MakeList<MapList>(xp, yp, MapData::Instance()->GetList(), "Map", (int*)&m_SelectBuffer, true, false, false, [&](const auto *) { return true; });
			}
			//
			if (m_MapSelect != InvalidID) {
				int xp = y_r(1920 - 400 - 10 - 500);
				int yp = LineHeight + y_r(10);

				auto* MapPtr = MapData::Instance()->FindPtr(m_MapSelect);
				int Max = (int)MapPtr->GetMapPicNum();
				int NextID = (m_WatchMapSelect + 1) % Max;
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(400), yp + LineHeight, false, true, Gray25, "Select %s", WatchMapTypeStr[std::clamp(NextID, 0, 2)])) {
					m_WatchMapSelect = NextID;
					m_BaseRad = MapPtr->GetMapNorthRad(m_WatchMapSelect);
					m_Rad = 0.f;
					m_Rad_Goal = 0.f;
				}
				yp += LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(400), yp + LineHeight, false, true, Gray25, "Reset Degree")) {
					m_Rad = 0.f;
					m_Rad_Goal = 0.f;
				}
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
			ComPass.DisposeGraph();
		}
	};
};