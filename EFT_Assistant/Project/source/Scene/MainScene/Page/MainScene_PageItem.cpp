#include "MainScene_PageItem.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void ItemBG::Init_Sub(int *, int *, float*) noexcept {
		Items.clear();
		for (auto& L : DataBase::Instance()->GetItemData()->GetList()) {//todo
			Items.emplace_back(&L);
		}
		//
		m_RaidMode = false;
		m_TraderSort = true;
		m_ValueSort = true;
		m_ValuePerSort = true;
		//
		InitLists(2, y_r(1920 - 10) - y_r(400), LineHeight + y_r(5), y_r(400));

		m_SearchBox.Init();
		m_ReturnButtonPress = [&]() {
			auto* PageMngr = PageManager::Instance();
			if (!(!m_RaidMode && BackLists())) {
				PageMngr->TurnOnGoNextPage();
			}
		};
	}
	void ItemBG::Draw_Back_Sub(int, int, float) noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* DrawParts = DXDraw::Instance();
		auto* Input = InputControl::Instance();

		int xpos = y_r(40);
		int ypos = LineHeight + y_r(10) + LineHeight;
		int ysize = (y_r(80));

		int xs = 400;
		int ScrPosX = y_r(1920 - xs * 3 / 2 - 10) - y_r(80);

		int ScrPxItem = this->m_RaidMode ? (xpos + y_r(400)) : ScrPosX;

		int yp0 = ypos - (int)m_YNow;
		if (m_RaidMode) {
			int PrevSize = y_r(400);
			int PerSize = PrevSize + y_r(250);
			if (WindowSystem::ClickCheckBox(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->m_DispYSize, false, true, Gray75, "")) {
				std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
					TraderID IDA = InvalidID;
					int ValueA = -1;
					if (a->GetSellValue(&IDA, &ValueA)) {}
					TraderID IDB = InvalidID;
					int ValueB = -1;
					if (b->GetSellValue(&IDB, &ValueB)) {}
					return this->m_TraderSort ? (IDA > IDB) : (IDA < IDB);
				});
				m_TraderSort ^= 1;

			}
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xpos + PrevSize, yp0, xpos + PerSize, DrawParts->m_DispYSize)) {
				DrawControl::Instance()->SetString(DrawLayer::Front,
					FontPool::FontType::Nomal_Edge, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
					"トレーダー"
				);
			}

			PrevSize = PerSize + y_r(25);
			PerSize += y_r(250);
			if (WindowSystem::ClickCheckBox(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->m_DispYSize, false, true, Gray75, "")) {
				std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
					TraderID IDA = InvalidID;
					int ValueA = -1;
					if (a->GetSellValue(&IDA, &ValueA)) {}
					TraderID IDB = InvalidID;
					int ValueB = -1;
					if (b->GetSellValue(&IDB, &ValueB)) {}
					return this->m_ValueSort ? (ValueA > ValueB) : (ValueA < ValueB);
				});
				m_ValueSort ^= 1;
			}
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xpos + PrevSize, yp0, xpos + PerSize, DrawParts->m_DispYSize)) {
				DrawControl::Instance()->SetString(DrawLayer::Front,
					FontPool::FontType::Nomal_Edge, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
					"売値"
				);
			}

			PrevSize = PerSize + y_r(25);
			PerSize += y_r(250);
			if (WindowSystem::ClickCheckBox(xpos + PrevSize, yp0, xpos + PerSize, DrawParts->m_DispYSize, false, true, Gray75, "")) {
				std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
					TraderID IDA = InvalidID;
					int ValueA = -1;
					if (a->GetSellValue(&IDA, &ValueA)) {}
					int ValueAP = ((a->Getwidth()*a->Getheight()) > 0) ? (ValueA / (a->Getwidth()*a->Getheight())) : 0;

					TraderID IDB = InvalidID;
					int ValueB = -1;
					if (b->GetSellValue(&IDB, &ValueB)) {}
					int ValueBP = ((b->Getwidth()*b->Getheight()) > 0) ? (ValueB / (b->Getwidth()*b->Getheight())) : 0;
					return this->m_ValuePerSort ? (ValueAP > ValueBP) : (ValueAP < ValueBP);
				});
				m_ValuePerSort ^= 1;
			}
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xpos + PrevSize, yp0, xpos + PerSize, DrawParts->m_DispYSize)) {
				DrawControl::Instance()->SetString(DrawLayer::Front,
					FontPool::FontType::Nomal_Edge, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
					"マス単価"
				);
			}
		}
		for (auto& L : Items) {//todo
			if (ListsSel(1) == InvalidID) {
				bool isHit = false;
				for (auto& TL : DataBase::Instance()->GetItemTypeData()->GetList()) {
					if (TL.GetCategoryID() == ListsSel(0) || ListsSel(0) == InvalidID) {
						isHit = (L->GetTypeID() == TL.GetID());
						if (isHit) { break; }
					}
				}
				if (!isHit) { continue; }
			}
			if (L->GetIsPreset()) { continue; }

			if (L->GetTypeID() == ListsSel(1) || ListsSel(1) == InvalidID) {
				bool ishit = true;
				/*
				bool ishit = (ListsSel(2) == InvalidID);
				for (auto& m : L->GetMapID()) {
					if (m.GetID() == ListsSel(2)) {
						ishit = true;
						break;
					}
				}
				if (ListsSel(2) == ElseSelectID) {
					ishit = (L->GetMapID().size() == 0);
				}
				//*/
				if (ishit && m_SearchBox.GetIsHit(L->GetName())) {
					if (((0 - ysize) < yp0) && (yp0 < DrawParts->m_DispYSize)) {
						const_cast<ItemList*>(L)->Draw(xpos, yp0, ScrPxItem - xpos - y_r(36), ysize, 0, Gray75, !WindowMngr->PosHitCheck(nullptr), false, !m_RaidMode, false);
						if (m_RaidMode) {
							TraderID ID = InvalidID;
							int Value = -1;
							if (L->GetSellValue(&ID, &Value)) {
								auto Color = Green;
								std::string TraderName = "Flea Market";
								if (ID != InvalidID) {
									auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(ID);
									if (ptr) {
										TraderName = ptr->GetName();
										Color = ptr->GetColors(50);
									}
								}
								int PerSize = y_r(400) + y_r(250);
								WindowSystem::SetMsg(xpos + PerSize, yp0, xpos + PerSize, yp0 + ysize, LineHeight * 9 / 10, STRX_RIGHT, Color, Black, "%s", TraderName.c_str());
								PerSize += y_r(250);
								WindowSystem::SetMsg(xpos + PerSize, yp0, xpos + PerSize, yp0 + ysize, LineHeight * 9 / 10, STRX_RIGHT, Color, Black, "%6d", Value);
								PerSize += y_r(250);
								WindowSystem::SetMsg(xpos + PerSize, yp0, xpos + PerSize, yp0 + ysize, LineHeight * 9 / 10, STRX_RIGHT, Color, Black, "%6d", Value / (L->Getwidth()*L->Getheight()));
							}
						}
					}
					yp0 += ysize;
				}
			}
		}
		yp0 -= ypos - (int)m_YNow;


		int ScrSizY = (DrawParts->m_DispYSize - (y_r(10) + LineHeight)) - ypos;
		m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp0, ScrSizY) / (float)ScrSizY, !WindowMngr->PosHitCheck(nullptr));

		m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer()*(float)(yp0 - ScrSizY));
		//List
		{
			int xgoal = 0;
			int xs_add = m_ListXSize + y_r(50);
			bool isChild = false;
			//
			isChild |= MakeLists(0, true, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemCategoryData()->DrawList(XP + xgoal, YP, XS, "ItemCategory", &IDs->first, !IDs->second, false, true);
			});
			isChild |= MakeLists(1, ((ListsSel(1) != InvalidID)/* && (DataBase::Instance()->GetItemTypeData()->FindPtr(ListsSel(1))->GetName() == "mechanical-key")*/),
				[&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemTypeData()->DrawList(XP + xgoal, YP, XS, "ItemType", &IDs->first, !IDs->second, false, true , [&](const auto *ptr) { return (ptr->GetCategoryID() == ListsSel(1 - 1)); });
			});
			/*
			isChild |= MakeLists(2, false, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetMapData()->DrawList(XP + xgoal, YP, XS, "Map", &IDs->first, !IDs->second, true, true);
			});
			//*/
			ExecuteLists(isChild, xgoal);
		}
	}
	void ItemBG::DrawFront_Sub(int, int, float) noexcept {
		//
		{
			int xp = y_r(1910) - LineHeight * 3;
			int yp = y_r(900);
			WindowSystem::CheckBox(xp, yp, &m_RaidMode);
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "売却価格モード");
			yp += LineHeight + y_r(6);
		}
		//検索
		m_SearchBox.Draw(y_r(1910) - y_r(500), y_r(850));
	}
};
