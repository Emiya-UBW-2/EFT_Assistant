#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemBG :public BGParent {
	private:
		ItemTypeID						m_ItemTypeSel{ InvalidID };
		MapID							m_MapTypeSel{ InvalidID };
		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
		bool							m_RaidMode{ false };

		std::vector<const ItemList*>	Items;
		bool							m_TraderSort{ false };
		bool							m_ValueSort{ false };
		bool							m_ValuePerSort{ false };

		bool							m_Search{ false };
		std::string						m_SearchWord;
	private:
		void Init_Sub(int *, int *, float*) noexcept override {
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
			InitLists(3, y_r(1920 - 10) - y_r(400), LineHeight + y_r(5), y_r(400));

			m_SearchWord = "";
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override {
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
					bool ishit = false;
					for (auto& m : L->GetMapID()) {
						if (m.GetID() == ListsSel(2)) {
							ishit = true;
							break;
						}
					}
					if (ListsSel(2) == ElseSelectID) {
						ishit = (L->GetMapID().size() == 0);
					}
					ishit |= (ListsSel(2) == InvalidID);
					if (ishit) {
						if (m_SearchWord != "") {
							std::string Name = L->GetName();
							std::transform(Name.begin(), Name.end(), Name.begin(), [](unsigned char c) { return (char)(std::tolower(c)); });
							ishit = (Name.find(m_SearchWord) != std::string::npos);
						}
					}
					if (ishit) {
						if (((0 - ysize) < yp0) && (yp0 < DrawParts->m_DispYSize)) {
							L->Draw(xpos, yp0, ScrPxItem - xpos - y_r(36), ysize, 0, Gray75, !WindowMngr->PosHitCheck(nullptr), false, !m_RaidMode, false);
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
			m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp0, ScrSizY) / (float)ScrSizY, true);

			m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer()*(float)(yp0 - ScrSizY));
			//List
			{
				int xgoal = 0;
				int xs_add = m_ListXSize + y_r(50);
				bool isChild = false;
				//
				isChild |= MakeLists(0, true, [&](std::pair<int, bool>* IDs, bool IsChild) {
					if (IsChild) { xgoal -= xs_add; }
					MakeList<ItemCategoryList>(m_ListXPos + xgoal, m_ListYPos, DataBase::Instance()->GetItemCategoryData()->GetList(), "ItemCategory", &IDs->first, !IDs->second, false, true, [&](const auto *) { return true; });
				});
				isChild |= MakeLists(1, ((ListsSel(1) != InvalidID) && (DataBase::Instance()->GetItemTypeData()->FindPtr(ListsSel(1))->GetName() == "Mechanical Key")), [&](std::pair<int, bool>* IDs, bool IsChild) {
					if (IsChild) { xgoal -= xs_add; }
					MakeList<ItemTypeList>(m_ListXPos + xgoal, m_ListYPos, DataBase::Instance()->GetItemTypeData()->GetList(), "ItemType", &IDs->first, !IDs->second, false, true, [&](const auto *ptr) { return (ptr->GetCategoryID() == ListsSel(1 - 1)); });
				});
				isChild |= MakeLists(2, false, [&](std::pair<int, bool>* IDs, bool IsChild) {
					if (IsChild) { xgoal -= xs_add; }
					MakeList<MapList>(m_ListXPos + xgoal, m_ListYPos, DataBase::Instance()->GetMapData()->GetList(), "Map", &IDs->first, !IDs->second, true, true, [&](const auto *) { return true; });
				});
				ExecuteLists(isChild, xgoal);
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					if (!(!m_RaidMode && BackLists())) {
						TurnOnGoNextBG();
					}
				}
			}
			//
			{
				int xp = y_r(1910) - LineHeight * 3;
				int yp = y_r(900);
				WindowSystem::CheckBox(xp, yp, &m_RaidMode);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "売却価格モード");
				yp += LineHeight + y_r(6);
			}
			//検索
			{
				auto* Input = InputControl::Instance();
				int xp1 = y_r(1910) - y_r(500);
				int yp1 = y_r(850);
				int xp2 = xp1 + y_r(500);
				int yp2 = yp1 + LineHeight+y_r(6);
				m_Search = in2_(Input->GetMouseX(), Input->GetMouseY(), xp1, yp1, xp2, yp2);

				if (m_Search) {
					for (char az = 'a'; az <= 'z'; az++) {
						if (Input->GetKey(az).trigger()) {
							m_SearchWord += az;
						}
					}
					if ((Input->GetBackSpaceKey().trigger() || Input->GetBackSpaceKey().trigger()) && m_SearchWord != "") {
						m_SearchWord.pop_back();
					}
				}

				WindowSystem::SetBox(xp1, yp1, xp2, yp2, m_Search ? Gray15 : Gray25);
				WindowSystem::SetMsg(xp1, yp1, xp2, yp2, LineHeight, STRX_LEFT, White, Black, (m_SearchWord != "") ? m_SearchWord.c_str() : "キーワードを入力…");
				//m_Search
			}
		}
		void Dispose_Sub(void) noexcept override {}
	};
};
