#include "MainScene_PageHideout.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void HideOutBG::Init_Sub(int*, int*, float*) noexcept {
		isDrew.clear();
		for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
			isDrew.resize(isDrew.size() + 1);
			isDrew.back().first = L.GetID();
			for (int loop = 0; loop < L.GetLvData().size(); loop++) {
				DrewData tmp;
				isDrew.back().second.emplace_back(tmp);
			}
		}
		m_Mode = EnumHideoutDrawMode::Normal;
		m_ReturnButtonPress = [&]() {
			auto* PageMngr = PageManager::Instance();
			if (m_Mode != EnumHideoutDrawMode::Normal) {
				m_Mode = EnumHideoutDrawMode::Normal;
			}
			else {
				PageMngr->TurnOnGoNextPage();
			}
		};
	}
	void HideOutBG::LateExecute_Sub(int* xpos, int*ypos, float*) noexcept {
		switch (m_Mode) {
		case EnumHideoutDrawMode::Normal:
			break;
		case EnumHideoutDrawMode::Item:
			*xpos = std::min(*xpos, DXDraw::Instance()->GetUIY(50));
			*ypos = LineHeight + DXDraw::Instance()->GetUIY(50);
			break;
		default:
			break;
		}
	}

	bool HideOutBG::DrawHideoutList(HideoutID MyID, int MyLv, int xpos, int* ypos, int xsize, int ysize) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		auto* Pad = PadControl::Instance();

		int index = MyLv - 1;
		auto IdDrew = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == MyID; });
		if (IdDrew == isDrew.end()) { return false; }
		if (IdDrew->second.at(index).IsDrew) { return false; }
		if (MyLv >= 2) {//2以上の時は同施設の前レベが描画されるまで待つ
			if (!IdDrew->second.at(static_cast<size_t>(index - 1)).IsDrew) { return false; }
		}
		IdDrew->second.at(index).IsDrew = true;
		IdDrew->second.at(index).xpos = xpos;
		IdDrew->second.at(index).ypos = *ypos;
		auto& L = *DataBase::Instance()->GetHideoutData()->FindPtr(MyID);
		//親が同施設の前レベ以外の時そいつの子にもなれるようにXをずらす
		for (auto& P : L.GetLvData().at(index).m_Parent) {
			auto IdDrewParent = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == P.GetID(); });
			if (IdDrewParent != isDrew.end()) {
				auto& Parent = IdDrewParent->second.at(static_cast<size_t>(P.GetValue() - 1));
				if (Parent.IsDrew) {
					xpos = std::max(xpos, Parent.xpos + xsize + DXDraw::Instance()->GetUIY(100));
				}
			}
		}
		for (auto& P : L.GetLvData().at(index).m_Parent) {
			auto IdDrewParent = std::find_if(isDrew.begin(), isDrew.end(), [&](const auto& tgt) {return tgt.first == P.GetID(); });
			if (IdDrewParent != isDrew.end()) {
				auto& Parent = IdDrewParent->second.at(static_cast<size_t>(P.GetValue() - 1));
				if (Parent.IsDrew) {
					int start_x = Parent.xpos + xsize;
					int start_y = Parent.ypos + ysize / 2;

					int end_x = xpos;
					int end_y = *ypos + ysize / 2;

					float Scale = 1.f;//仮
					auto XAddLine = (int)((float)DXDraw::Instance()->GetUIY(25) * Scale);
					if (Scale > 0.6f) {
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back, start_x, start_y, start_x + XAddLine, start_y, Black, (int)(15.f * Scale));
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back, start_x + XAddLine, start_y, end_x - XAddLine, end_y, Black, (int)(15.f * Scale));
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back, end_x - XAddLine, end_y, end_x, end_y, Black, (int)(15.f * Scale));
					}
					int Col = GetColor(255, DrawLineCount % 256, DrawLineCount % 256);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back2, start_x, start_y, start_x + XAddLine, start_y, Col, (int)(5.f * Scale));
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back2, start_x + XAddLine, start_y, end_x - XAddLine, end_y, Col, (int)(5.f * Scale));
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Back2, end_x - XAddLine, end_y, end_x, end_y, Col, (int)(5.f * Scale));
				}
			}
		}
		DrawLineCount += 3;

		auto color = Gray15;
		if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), MyLv)) {
			color = Gray50;
		}
		if (WindowSystem::SetMsgClickBox(xpos, *ypos, xpos + xsize, *ypos + ysize, ysize, color, false, !WindowMngr->PosHitCheck(nullptr),  "")) {
			if (Pad->GetKey(PADS::JUMP).press()) {
				PlayerData::Instance()->OnOffHideoutClear(L.GetName().c_str(), MyLv);
			}
		}
		L.Draw(xpos, *ypos, xsize, ysize, MyLv, color, !Pad->GetKey(PADS::JUMP).press() && !WindowMngr->PosHitCheck(nullptr), 0);
		if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), MyLv)) {
			if (DrawGraphs::Instance()->GetFirGraph().GetGraph()) {
				WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetFirGraph().GetGraph(), xpos + xsize / 2, *ypos + ysize / 2, 1.f, 0.f, true);
			}
		}

		for (auto& C : L.GetLvData().at(index).m_Child) {
			int TmpYPos = *ypos;
			if (DrawHideoutList(C.GetID(), C.GetValue(), xpos + xsize + DXDraw::Instance()->GetUIY(100), &TmpYPos, xsize, ysize)) {
				*ypos += ysize + DXDraw::Instance()->GetUIY(10);
			}
		}
		return true;
	}

	void HideOutBG::Draw_Back_Sub(int xpos, int ypos, float) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		auto* PageMngr = PageManager::Instance();

		PageMngr->SetScaleActive(false);

		switch (m_Mode) {
		case EnumHideoutDrawMode::Normal:
		{
			for (auto& d : isDrew) { for (auto& d2 : d.second) { d2.IsDrew = false; } }
			DrawLineCount = 0;

			int xp = xpos, yp = ypos;
			int xs = DXDraw::Instance()->GetUIY(300), ys = LineHeight;
			for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
				for (const auto& C : L.GetLvData()) {
					int Lv = (int)(&C - &L.GetLvData().front()) + 1;
					if (C.m_Parent.size() == 0) {
						DrawHideoutList(L.GetID(), Lv, xp, &yp, xs, ys);
						yp += DXDraw::Instance()->GetUIY(130);
					}
				}
			}
		}
		break;
		case EnumHideoutDrawMode::Item:
		{
			std::vector<std::vector<std::pair<ItemID, int>>> Counter;
			Counter.resize(DataBase::Instance()->GetItemTypeData()->GetList().size());
			for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
				for (const auto& Ld : L.GetLvData()) {
					bool IsChecktask = true;
					if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
						IsChecktask = false;
					}
					if (!IsChecktask) { continue; }
					for (const auto& w : Ld.m_ItemReq) {
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
			}
			//描画
			{
				int xpBase = xpos;
				int ypBase = ypos;
				int xp = xpBase;
				int yp = ypBase;
				int xsize = (DXDraw::Instance()->GetUIY(360));
				int ysize = LineHeight;
				int xsizeAdd = xsize + DXDraw::Instance()->GetUIY(5);
				int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);

				for (auto& Cat : DataBase::Instance()->GetItemCategoryData()->GetList()) {
					bool IsHit = false;
					for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
						if (Type.GetCategoryID() == Cat.GetID()) {
							auto& Types = Counter.at(Type.GetID());
							for (auto& c : Types) {
								auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.first);
								if (ptr) {
									if (yp >= (DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(50))) {
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
	void HideOutBG::DrawFront_Sub(int, int, float) noexcept {
		auto* Pad = PadControl::Instance();
		//
		{
			int xp = DXDraw::Instance()->GetUIY(10) + DXDraw::Instance()->GetUIY(200) + DXDraw::Instance()->GetUIY(10);
			int yp = LineHeight + DXDraw::Instance()->GetUIY(10);
			if (WindowSystem::SetMsgClickBox(xp, yp, xp + DXDraw::Instance()->GetUIY(400), yp + LineHeight, LineHeight, (m_Mode != EnumHideoutDrawMode::Item) ? Gray25 : Green, false, this->m_Mode != EnumHideoutDrawMode::Item, "開放アイテム")) {
				m_Mode = EnumHideoutDrawMode::Item;
			}
		}
		//
		if (m_Mode == EnumHideoutDrawMode::Normal) {
			int yofs = 0;
			if (!Pad->GetKey(PADS::JUMP).press()) {
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y() + yofs, RedPop, Black,
					"スペースを押しながらクリックでタスククリア"
				);
				yofs -= LineHeight;
			}
			if (!Pad->GetKey(PADS::WALK).press()) {
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y() + yofs, RedPop, Black,
					"LCtrl中に施設クリックでアンロック条件を表示"
				);
				yofs -= LineHeight;
			}
		}
	}
};
