#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	enum class EnumHideoutDrawMode {
		Normal,
		Item,
	};

	class HideOutBG :public BGParent {
	private:
		struct DrewData {
			bool IsDrew{ false };
			int xpos{ 0 };
			int ypos{ 0 };
		};

		std::vector<std::pair<HideoutID, std::vector<DrewData>>>	isDrew;
		int															DrawLineCount{ 0 };
		EnumHideoutDrawMode											m_Mode{ EnumHideoutDrawMode::Normal };
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			isDrew.clear();
			for (const auto&L : HideoutData::Instance()->GetList()) {
				isDrew.resize(isDrew.size() + 1);
				isDrew.back().first = L.GetID();
				for (int loop = 0; loop < L.GetLvData().size(); loop++) {
					DrewData tmp;
					isDrew.back().second.emplace_back(tmp);
				}
			}
			m_Mode = EnumHideoutDrawMode::Normal;
		}
		void LateExecute_Sub(int* xpos, int*ypos, float*) noexcept override {
			switch (m_Mode) {
			case EnumHideoutDrawMode::Normal:
				break;
			case EnumHideoutDrawMode::Item:
				*xpos = std::min(*xpos, y_r(50));
				*ypos = LineHeight + y_r(50);
				break;
			default:
				break;
			}
		}

		bool DrawHideoutList(HideoutID MyID, int MyLv, int xpos, int* ypos, int xsize, int ysize) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* Input = InputControl::Instance();

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
				auto IdDrewParent = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == P.GetID(); });
				if (IdDrewParent != isDrew.end()) {
					if (IdDrewParent->second.at(P.GetValue() - 1).IsDrew == true) {
						xpos = std::max(xpos, IdDrewParent->second.at(P.GetValue() - 1).xpos + xsize + y_r(100));
					}
				}
			}
			for (auto& P : L.GetLvData().at(MyLv - 1).m_Parent) {
				auto IdDrewParent = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == P.GetID(); });
				if (IdDrewParent != isDrew.end()) {
					if (IdDrewParent->second.at(P.GetValue() - 1).IsDrew == true) {
						int start_x = IdDrewParent->second.at(P.GetValue() - 1).xpos + xsize;
						int start_y = IdDrewParent->second.at(P.GetValue() - 1).ypos + ysize / 2;

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
			DrawLineCount += 3;

			auto color = Gray15;
			if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), MyLv)) {
				color = Gray50;
			}
			if (WindowSystem::ClickCheckBox(xpos, *ypos, xpos + xsize, *ypos + ysize, false, !WindowMngr->PosHitCheck(nullptr), color, "")) {
				if (Input->GetSpaceKey().press()) {
					PlayerData::Instance()->OnOffHideoutClear(L.GetName().c_str(), MyLv);
				}
			}
			L.Draw(xpos, *ypos, xsize, ysize, MyLv, color, !Input->GetSpaceKey().press() && !WindowMngr->PosHitCheck(nullptr));
			if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), MyLv)) {
				DrawControl::Instance()->SetDrawRotaFiR(DrawLayer::Normal, xpos + xsize / 2, *ypos + ysize / 2, 1.f, 0.f, true);
			}

			for (auto& C : L.GetLvData().at(MyLv - 1).m_Child) {
				int TmpYPos = *ypos;
				if (DrawHideoutList(C.GetID(), C.GetValue(), xpos + xsize + y_r(100), &TmpYPos, xsize, ysize)) {
					*ypos += ysize + y_r(10);
				}
			}
			return true;
		}

		void Draw_Back_Sub(int xpos, int ypos, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* Input = InputControl::Instance();

			Input->SetScaleActive(false);

			switch (m_Mode) {
			case EnumHideoutDrawMode::Normal:
			{
				for (auto& d : isDrew) { for (auto& d2 : d.second) { d2.IsDrew = false; } }
				DrawLineCount = 0;

				int xp = xpos, yp = ypos;
				int xs = y_r(300), ys = LineHeight;
				for (const auto&L : HideoutData::Instance()->GetList()) {
					for (const auto& C : L.GetLvData()) {
						int Lv = (int)(&C - &L.GetLvData().front()) + 1;
						if (C.m_Parent.size() == 0) {
							DrawHideoutList(L.GetID(), Lv, xp, &yp, xs, ys);
							yp += y_r(130);
						}
					}
				}
			}
			break;
			case EnumHideoutDrawMode::Item:
			{
				std::vector<std::vector<std::pair<ItemID, int>>> Counter;
				Counter.resize(ItemTypeData::Instance()->GetList().size());
				for (const auto&L : HideoutData::Instance()->GetList()) {
					for (const auto& Ld : L.GetLvData()) {
						bool IsChecktask = true;
						if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
							IsChecktask = false;
						}
						if (!IsChecktask) { continue; }
						for (const auto& w : Ld.m_ItemReq) {
							auto* ptr = ItemData::Instance()->FindPtr(w.GetID());
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
				}
				//描画
				{
					int xpBase = xpos;
					int ypBase = ypos;
					int xp = xpBase;
					int yp = ypBase;
					int xsize = (int)((float)y_r(360));
					int ysize = (int)((float)y_r(32));
					int xsizeAdd = xsize + y_r(5);
					int ysizeAdd = ysize + y_r(5);

					for (auto& Cat : ItemCategoryData::Instance()->GetList()) {
						bool IsHit = false;
						for (auto& Type : ItemTypeData::Instance()->GetList()) {
							if (Type.GetCategoryID() == Cat.GetID()) {
								auto& Types = Counter.at(Type.GetID());
								for (auto& c : Types) {
									auto* ptr = ItemData::Instance()->FindPtr(c.first);
									if (ptr) {
										if (yp >= (y_r(1080) - y_r(50))) {
											xp += xsizeAdd;
											yp = ypBase;
										}
										ptr->Draw(xp, yp, xsize, ysize, (c.second >= 2) ? c.second : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, false);
										yp += ysizeAdd;
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
			default:
				break;
			}


		}
		void DrawFront_Sub(int, int, float) noexcept override {
			auto* Input = InputControl::Instance();
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					if (m_Mode != EnumHideoutDrawMode::Normal) {
						m_Mode = EnumHideoutDrawMode::Normal;
					}
					else {
						TurnOnGoNextBG();
					}
				}
			}
			//
			{
				int xp = y_r(10) + y_r(200) + y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(400), yp + LineHeight, false, m_Mode != EnumHideoutDrawMode::Item, (m_Mode != EnumHideoutDrawMode::Item) ? Gray25 : Green, "開放アイテム")) {
					m_Mode = EnumHideoutDrawMode::Item;
				}
			}
			//
			if (m_Mode == EnumHideoutDrawMode::Normal) {
				int yofs = 0;
				if (!Input->GetSpaceKey().press()) {
					DrawControl::Instance()->SetString(DrawLayer::Front,
						FontPool::FontType::Nomal_Edge, LineHeight,
						STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY() + yofs, RedPop, Black,
						"スペースを押しながらクリックでタスククリア"
					);
					yofs -= LineHeight;
				}
				if (!Input->GetCtrlKey().press()) {
					DrawControl::Instance()->SetString(DrawLayer::Front,
						FontPool::FontType::Nomal_Edge, LineHeight,
						STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY() + yofs, RedPop, Black,
						"LCtrl中に施設クリックでアンロック条件を表示"
					);
					yofs -= LineHeight;
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};
