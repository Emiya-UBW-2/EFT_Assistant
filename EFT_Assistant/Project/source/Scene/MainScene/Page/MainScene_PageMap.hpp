#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	class MapBG :public BGParent {
	private:
		MapID	m_MapSelect{ InvalidID };
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override {
			*posx = y_r(960);
			*posy = y_r(540);
			*Scale = 1.f;
		}
		void LateExecute_Sub(void) noexcept override {
			if (m_MapSelect == InvalidID) {
				m_MapSelect = MapData::Instance()->FindID("Customs");
			}
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int posx, int posy, float Scale) noexcept override {
			if (m_MapSelect != InvalidID) {
				auto* MapPtr = MapData::Instance()->FindPtr(m_MapSelect);
				if (MapPtr->GetInGameMapGraph()) {
					MapPtr->GetInGameMapGraph()->DrawRotaGraph(posx, posy, Scale / 2.f, 0.f, false);
				}
			}
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "–ß‚é")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};