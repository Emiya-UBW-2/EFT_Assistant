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
				if (MapPtr->GetMapGraph((int)m_WatchMapSelect)) {
					DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, MapPtr->GetMapGraph((int)m_WatchMapSelect), posx, posy, Scale / 2.f, m_Rad, false);
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