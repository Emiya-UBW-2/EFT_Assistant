#include "MainScene_PageMap.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void MapBG::Init_Sub(int *posx, int *posy, float* Scale) noexcept {
		auto* Pad = PadControl::Instance();
		*posx = DXDraw::Instance()->GetUIY(960);
		*posy = DXDraw::Instance()->GetUIY(540);
		*Scale = 1.f;

		m_Rad = 0.f;
		m_Rad_Goal = 0.f;
		m_BaseRad = 0.f;
		m_ComPassRad = 0.f;

		ComPass.SetPath(u8"data/UI/Compass.png");
		ComPass.SetIsTrans(true);
		ComPass.LoadByPath(false);
		ComPass.WhenAfterLoad();

		mouse_moveX = Pad->GetMS_X();							//ドラッグ前のマウス座標格納
		mouse_moveY = Pad->GetMS_Y();

		m_EditTraderID = InvalidID;
		//m_EditTaskID = InvalidID;
	}
	void MapBG::LateExecute_Sub(int *, int *, float*) noexcept {
		auto* Pad = PadControl::Instance();
		if (m_MapSelect != this->m_SelectBuffer) {
			if (m_MapSelect != InvalidID) {
				auto* MapPtr = DataBase::Instance()->GetMapData()->FindPtr(m_MapSelect);
				MapPtr->DisposeMapPics();
			}
			m_MapSelect = this->m_SelectBuffer;
			{
				auto* MapPtr = DataBase::Instance()->GetMapData()->FindPtr(m_MapSelect);
				if (m_WatchMapSelect <= MapPtr->GetMapPicNum()) { this->m_WatchMapSelect = 0; }
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
				auto* MapPtr = DataBase::Instance()->GetMapData()->FindPtr(m_MapSelect);
				MapPtr->WhenAfterLoadMapPics();
			}
		}
		//ドラッグ開始時の処理
		bool notpress = true;
		if (!Pad->GetKey(PADS::AIM).press() && !Pad->GetKey(PADS::ULT).press()) {
			if (Pad->GetMouseClick().press()) {
				m_Rad_Goal += deg2rad(Pad->GetMS_X() - mouse_moveX);
				//Pad->GetMS_Y() - mouse_moveY;
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEWE);
				SetCursor(hCursor);
				notpress = false;
			}
			mouse_moveX = Pad->GetMS_X();							//ドラッグ前のマウス座標格納
			mouse_moveY = Pad->GetMS_Y();
		}
		Easing(&m_Rad, this->m_Rad_Goal, 0.8f, EasingType::OutExpo);
		Easing(&m_ComPassRad, (m_BaseRad + this->m_Rad), 0.8f, EasingType::OutExpo);
	}
	void MapBG::Draw_Back_Sub(int posx, int posy, float Scale) noexcept {
		if (m_MapSelect != InvalidID) {
			auto* MapPtr = DataBase::Instance()->GetMapData()->FindPtr(m_MapSelect);
			auto* Pad = PadControl::Instance();
			auto* GraphPtr = MapPtr->GetMapGraph((int)m_WatchMapSelect);
			TaskList* TaskPtr = (m_EditTaskID != InvalidID) ? DataBase::Instance()->GetTaskData()->FindPtr(m_EditTaskID) : nullptr;
			if (GraphPtr) {
				WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, GraphPtr, posx, posy, Scale / 2.f, this->m_Rad, false);
				float XSize = (float)MapPtr->GetMapXSize((int)m_WatchMapSelect)*Scale / 2.f;
				float YSize = (float)MapPtr->GetMapYSize((int)m_WatchMapSelect)*Scale / 2.f;
				{
					float Xper = ((float)Pad->GetMS_X() - (float)posx);
					float Yper = ((float)Pad->GetMS_Y() - (float)posy);
					float Len = std::hypotf(Xper, Yper);
					float Rad = std::atan2f(Xper, Yper);

					Xper = Len * std::sin(Rad + this->m_Rad);
					Yper = Len * std::cos(Rad + this->m_Rad);

					Xper = Xper / XSize;
					Yper = Yper / YSize;
					if (Pad->GetKey(PADS::JUMP).trigger()) {
						m_EditTaskID++;
					}

					//
					DataBase::Instance()->GetTaskData()->DrawList(DXDraw::Instance()->GetUIY(10), LineHeight + DXDraw::Instance()->GetUIY(20) + LineHeight, DXDraw::Instance()->GetUIY(400), "Task", (int*)&m_EditTaskID, true, false, false, [&](const auto* tgt) {
						if (tgt->GetTrader() != this->m_EditTraderID && this->m_EditTraderID != InvalidID) {
							return false;
						}
						if (!tgt->GetIsUSECorBEAR()) {
							return false;
						}

						for (const auto& m : tgt->GetTaskWorkData().GetMap()) {
							if (m == this->m_MapSelect) { return true; }
						}
						return false;
					});

					if (TaskPtr) {
						auto& Pins = TaskPtr->SetTaskWorkData().SetPin();
						if (Pad->GetMouseClick().trigger()) {
							if (Pad->GetKey(PADS::RUN).press()) {
								if ((-0.5f <= Xper && Xper <= 0.5f) && (-0.5f <= Yper && Yper <= 0.5f)) {
									Pins.resize(Pins.size() + 1);
									Pins.back().m_Point.Set(Xper, Yper, 0.f);
									Pins.back().m_MapID = MapPtr->GetID();
									Pins.back().m_MapSel = this->m_WatchMapSelect;
								}
							}
						}
					}
				}
				if (TaskPtr) {
					auto& Pins = TaskPtr->SetTaskWorkData().SetPin();
					for (int loop = 0; loop < Pins.size(); loop++) {
						const auto& p = Pins[loop];
						if (p.m_MapID == MapPtr->GetID() && p.m_MapSel == this->m_WatchMapSelect) {
							float Xs = XSize * p.m_Point.x;
							float Ys = YSize * p.m_Point.y;
							int xp = posx + (int)(std::cos(m_Rad)*Xs - std::sin(m_Rad)*Ys);
							int yp = posy + (int)(std::cos(m_Rad)*Ys + std::sin(m_Rad)*Xs);
							WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, xp, yp, 6, Black);
							WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, xp, yp, 5, Red);
							if (Pad->GetKey(PADS::AIM).trigger()) {
								if (Pad->GetKey(PADS::RUN).press()) {
									if (std::hypotf((float)(Pad->GetMS_X() - xp), (float)(Pad->GetMS_Y() - yp)) < 5.f) {
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
				int xp = DXDraw::Instance()->GetUIY(960);
				int yp = DXDraw::Instance()->GetUIY(1080);
				int ys = LineHeight;
				WindowSystem::SetMsg(xp, yp - ys * 2 - ys / 2, ys, STRX_MID, Green, Black, "Shift+左クリックClick　ピン打ち");
				WindowSystem::SetMsg(xp, yp - ys * 1 - ys / 2, ys, STRX_MID, Green, Black, "Shift+右クリックClick　ピン消し");
				WindowSystem::SetMsg(xp, yp - ys * 0 - ys / 2, ys, STRX_MID, Green, Black, "タスク[%s]を編集中", TaskPtr->GetName().c_str());
			}
		}
		if (ComPass.GetGraph()) {
			WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, ComPass.GetGraph(), DXDraw::Instance()->GetUIY(100), DXDraw::Instance()->GetUIY(1080 - 100), 1.f, this->m_ComPassRad, true);
		}
	}
	void MapBG::DrawFront_Sub(int posx, int posy, float Scale) noexcept {
		//
		DataBase::Instance()->GetMapData()->DrawList(DXDraw::Instance()->GetUIY(1920 - 400 - 10), LineHeight + DXDraw::Instance()->GetUIY(10), DXDraw::Instance()->GetUIY(400), "Map", (int*)&m_SelectBuffer, true, false, false);
		//
		DataBase::Instance()->GetTraderData()->DrawList(DXDraw::Instance()->GetUIY(1920 - 400 - 10), LineHeight + DXDraw::Instance()->GetUIY(540), DXDraw::Instance()->GetUIY(400), "Task Trader Filter", (int*)&m_EditTraderID, true, false, false);
		//
		if (m_MapSelect != InvalidID) {
			int xp = DXDraw::Instance()->GetUIY(1920 - 400 - 10 - 500);
			int yp = LineHeight + DXDraw::Instance()->GetUIY(10);

			auto* MapPtr = DataBase::Instance()->GetMapData()->FindPtr(m_MapSelect);
			int Max = (int)MapPtr->GetMapPicNum();
			int NextID = (m_WatchMapSelect + 1) % Max;
			if (WindowSystem::SetMsgClickBox(xp, yp, xp + DXDraw::Instance()->GetUIY(400), yp + LineHeight, LineHeight, Gray25, false, true, "Select %s", WatchMapTypeStr[std::clamp(NextID, 0, (int)EnumWatchMapType::Max)])) {
				m_WatchMapSelect = NextID;
				m_BaseRad = MapPtr->GetMapNorthRad(m_WatchMapSelect);
				m_Rad = 0.f;
				m_Rad_Goal = 0.f;
			}
			yp += LineHeight + DXDraw::Instance()->GetUIY(10);
			if (WindowSystem::SetMsgClickBox(xp, yp, xp + DXDraw::Instance()->GetUIY(400), yp + LineHeight, LineHeight, Gray25, false, true, "Reset Degree")) {
				m_Rad = 0.f;
				m_Rad_Goal = 0.f;
			}
		}
		auto* Pad = PadControl::Instance();
		float Xper = ((float)Pad->GetMS_X() - (float)posx);
		float Yper = ((float)Pad->GetMS_Y() - (float)posy);
		float Len = std::hypotf(Xper, Yper);
		float Rad = std::atan2f(Xper, Yper);
		if (m_MapSelect != InvalidID) {
			auto* MapPtr = DataBase::Instance()->GetMapData()->FindPtr(m_MapSelect);
			float XSize = (float)MapPtr->GetMapXSize((int)m_WatchMapSelect)*Scale / 2.f;
			float YSize = (float)MapPtr->GetMapYSize((int)m_WatchMapSelect)*Scale / 2.f;

			Xper = Len * std::sin(Rad + this->m_Rad);
			Yper = Len * std::cos(Rad + this->m_Rad);

			Xper = Xper / XSize;
			Yper = Yper / YSize;

			if (Pad->GetKey(PADS::MELEE).press()) {
				if (Pad->GetKey(PADS::MELEE).trigger()) {
					m_BaseLength.Set(Xper, Yper, 0.f);
				}
				{
					float Xs1 = XSize * this->m_BaseLength.x;
					float Ys1 = YSize * this->m_BaseLength.y;
					int xp1 = posx + (int)(std::cos(m_Rad)*Xs1 - std::sin(m_Rad)*Ys1);
					int yp1 = posy + (int)(std::cos(m_Rad)*Ys1 + std::sin(m_Rad)*Xs1);
					float Xs2 = XSize * Xper;
					float Ys2 = YSize * Yper;
					int xp2 = posx + (int)(std::cos(m_Rad)*Xs2 - std::sin(m_Rad)*Ys2);
					int yp2 = posy + (int)(std::cos(m_Rad)*Ys2 + std::sin(m_Rad)*Xs2);

					float LenF = std::hypotf((float)(xp1 - xp2), (float)(yp1 - yp2));
					WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Normal, xp1, yp1, (int)LenF, Black, false, 2);


					WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, xp1, yp1, 6, Black);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Front, xp1, yp1, xp2, yp2, Black, 3);
					WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, xp2, yp2, 6, Black);

					WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, xp1, yp1, 5, Blue);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Front, xp1, yp1, xp2, yp2, Blue, 2);
					WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Front, xp2, yp2, 5, Blue);

					int Len2 = (int)(LenF / (Scale / 2.f) * MapPtr->GeSizetoMeter(m_WatchMapSelect));
					int LenPer = 5;

					WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
						FontPool::FontType::MS_Gothic, LineHeight,
						STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
						"%d～%dm", LenPer * (Len2 / LenPer), LenPer * (Len2 / LenPer + 1)
					);
				}
			}
		}
	}
	void MapBG::Dispose_Sub(void) noexcept {
		ComPass.DisposeGraph();
	}
};
