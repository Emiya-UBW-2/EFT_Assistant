#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class HideOutBG :public BGParent {
	private:
		struct DrewData {
			bool IsDrew{ false };
			int xpos{ 0 };
			int ypos{ 0 };
		};

		std::vector<std::pair<HideoutID, std::vector<DrewData>>> isDrew;
		int DrawLineCount{ 0 };
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			isDrew.clear();
			for (const auto&L : HideoutData::Instance()->GetList()) {
				isDrew.resize(isDrew.size() + 1);
				isDrew.back().first = L.GetID();
				for (const auto& C : L.GetLvData()) {
					DrewData tmp;
					isDrew.back().second.emplace_back(tmp);
				}
			}
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}

		bool DrawHideoutList(HideoutID MyID, int MyLv, int xpos, int* ypos, int xsize, int ysize) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();

			auto IdDrew = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == MyID; });
			if (IdDrew == isDrew.end()) { return false; }
			if (IdDrew->second.at(MyLv - 1).IsDrew == true) { return false; }
			if (MyLv >= 2) {//2以上の時は同施設の前レベが描画されるまで待つ
				if (IdDrew->second.at(MyLv - 1 - 1).IsDrew == false) { return false; }
			}
			IdDrew->second.at(MyLv - 1).IsDrew = true;
			IdDrew->second.at(MyLv - 1).xpos = xpos;
			IdDrew->second.at(MyLv - 1).ypos = *ypos;
			auto& L = *HideoutData::Instance()->FindPtr(MyID);
			//親が同施設の前レベ以外の時そいつの子にもなれるようにXをずらす
			for (auto& P : L.GetLvData().at(MyLv - 1).m_Parent) {
				auto IdDrewParent = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == HideoutData::Instance()->FindID(P.GetID().c_str()); });
				if (IdDrewParent != isDrew.end()) {
					if (IdDrewParent->second.at(P.GetLv() - 1).IsDrew == true) {
						xpos = std::max(xpos, IdDrewParent->second.at(P.GetLv() - 1).xpos + xsize + y_r(100));
					}
				}
			}
			for (auto& P : L.GetLvData().at(MyLv - 1).m_Parent) {
				auto IdDrewParent = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == HideoutData::Instance()->FindID(P.GetID().c_str()); });
				if (IdDrewParent != isDrew.end()) {
					if (IdDrewParent->second.at(P.GetLv() - 1).IsDrew == true) {
						int start_x = IdDrewParent->second.at(P.GetLv() - 1).xpos + xsize;
						int start_y = IdDrewParent->second.at(P.GetLv() - 1).ypos + ysize / 2;

						int end_x = xpos;
						int end_y = *ypos + ysize / 2;

						float Scale = 1.f;//仮
						auto XAddLine = (int)((float)y_r(25) * Scale);
						if (Scale > 0.6f) {
							DrawControl::Instance()->SetDrawLine(DrawLayer::Back, start_x, start_y, start_x + XAddLine, start_y, Black, (int)(15.f * Scale));
							DrawControl::Instance()->SetDrawLine(DrawLayer::Back, start_x + XAddLine, start_y, end_x - XAddLine, end_y, Black, (int)(15.f * Scale));
							DrawControl::Instance()->SetDrawLine(DrawLayer::Back, end_x - XAddLine, end_y, end_x, end_y, Black, (int)(15.f * Scale));
						}
						int Col = GetColor(255, DrawLineCount % 256, DrawLineCount % 256);
						DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, start_x, start_y, start_x + XAddLine, start_y, Col, (int)(5.f * Scale));
						DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, start_x + XAddLine, start_y, end_x - XAddLine, end_y, Col, (int)(5.f * Scale));
						DrawControl::Instance()->SetDrawLine(DrawLayer::Back2, end_x - XAddLine, end_y, end_x, end_y, Col, (int)(5.f * Scale));
					}
				}
			}
			DrawLineCount +=3;

			L.Draw(xpos, *ypos, xsize, ysize, MyLv, Gray15, !WindowMngr->PosHitCheck(nullptr));
			for (auto& C : L.GetLvData().at(MyLv-1).m_Child) {
				int TmpYPos = *ypos;
				if (DrawHideoutList(HideoutData::Instance()->FindID(C.GetID().c_str()), C.GetLv(), xpos + xsize + y_r(100), &TmpYPos, xsize, ysize)) {
					*ypos += ysize + y_r(10);
				}
			}
			return true;
		}

		void Draw_Back_Sub(int xpos, int ypos, float) noexcept override {
			for (auto& d : isDrew) { for (auto& d2 : d.second) { d2.IsDrew = false; } }
			DrawLineCount = 0;

			int xp = xpos, yp = ypos;
			int xs = y_r(300), ys = LineHeight;
			for (const auto&L : HideoutData::Instance()->GetList()) {
				for (const auto& C : L.GetLvData()) {
					int Lv = (int)(&C - &L.GetLvData().front()) + 1;
					if(C.m_Parent.size()==0){
						DrawHideoutList(L.GetID(), Lv, xp, &yp, xs, ys);
						yp += y_r(130);
					}
				}
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			auto* Input = InputControl::Instance();
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					TurnOnGoNextBG();
				}
			}
			//
			if (!Input->GetCtrlKey().press()) {
				DrawControl::Instance()->SetString(DrawLayer::Front,
					FontPool::FontType::Nomal_Edge, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
					"LCtrl中に施設クリックでアンロック条件を表示"
				);
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};