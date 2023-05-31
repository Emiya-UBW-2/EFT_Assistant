#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemBG :public BGParent {
	private:
		std::vector<std::pair<int, bool>>	m_ItemIDs;
		ItemTypeID						m_ItemTypeSel{ InvalidID };
		MapID							m_MapTypeSel{ InvalidID };
		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
		float							m_XChild{ 0.f };
		bool							m_RaidMode{ false };
		bool							m_TaskMode{ false };

		std::vector<const ItemList*>	Items;
		bool							m_TraderSort{ false };
		bool							m_ValueSort{ false };
		bool							m_ValuePerSort{ false };
	private:
		void MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*)>& ListSet) noexcept {
			auto& NowSel = this->m_ItemIDs.at(Layer);
			NowSel.second = ((NowSel.first != InvalidID) && AndNext);
			if (Layer == 0 || (Layer >= 1 && this->m_ItemIDs.at(Layer - 1).second)) {
				ListSet(&NowSel);
			}
			else {
				NowSel.first = InvalidID;
			}
		}
	private:
		void Init_Sub(int *, int *, float*) noexcept override {
			m_XChild = 0.f;
			m_ItemIDs.clear();
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));

			m_RaidMode = false;
			m_TaskMode = false;

			Items.clear();
			for (auto& L : DataBase::Instance()->GetItemData()->GetList()) {//todo
				Items.emplace_back(&L);
			}
			m_TraderSort = true;
			m_ValueSort = true;
			m_ValuePerSort = true;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}
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
				if (m_ItemIDs[1].first == InvalidID) {
					bool isHit = false;
					for (auto& TL : DataBase::Instance()->GetItemTypeData()->GetList()) {
						if (TL.GetCategoryID() == this->m_ItemIDs[0].first || this->m_ItemIDs[0].first == InvalidID) {
							isHit = (L->GetTypeID() == TL.GetID());
							if (isHit) { break; }
						}
					}
					if (!isHit) { continue; }
				}
				if (L->GetIsPreset()) { continue; }

				if (L->GetTypeID() == this->m_ItemIDs[1].first || this->m_ItemIDs[1].first == InvalidID) {
					bool ishit = false;
					for (auto& m : L->GetMapID()) {
						if (m.GetID() == this->m_ItemIDs[2].first) {
							ishit = true;
							break;
						}
					}
					if (m_ItemIDs[2].first == ElseSelectID) {
						ishit = (L->GetMapID().size() == 0);
					}
					if (ishit || this->m_ItemIDs[2].first == InvalidID) {
						if (((0 - ysize) < yp0) && (yp0 < DrawParts->m_DispYSize)) {
							L->Draw(xpos, yp0, ScrPxItem - xpos - y_r(36), ysize, 0, Gray75, !WindowMngr->PosHitCheck(nullptr), false, !m_RaidMode && !m_TaskMode, false);
							if (m_TaskMode) {
								if (in2_(Input->GetMouseX(), Input->GetMouseY(), xpos, yp0, xpos + (ScrPxItem - xpos - y_r(36)), yp0 + ysize)) {
									auto Color = Green;
									int YPosAdd = 0;
									for (auto& t : L->GetUseTaskID()) {
										auto* ptr = DataBase::Instance()->GetTaskData()->FindPtr(t);
										DrawControl::Instance()->SetString(DrawLayer::Front,
											FontPool::FontType::Nomal_Edge, LineHeight,
											STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY() + YPosAdd, Color, Black,
											"%s", ptr->GetName().c_str()
										); YPosAdd += LineHeight;
									}
								}
							}
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
							if (m_TaskMode) {
								if (L->GetUseTaskID().size() > 0) {
									int PerSize = y_r(400) + y_r(250);
									WindowSystem::SetMsg(xpos + PerSize, yp0, xpos + PerSize, yp0 + ysize, LineHeight * 9 / 10, STRX_RIGHT, White, Black, "〇");
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
			//
			{
				int xgoal = 0;
				int xsize = y_r(400);
				int xs_add = -(xsize + y_r(50));
				int xp = y_r(1920 - 10) - xsize - (int)m_XChild;
				int yp = LineHeight + y_r(5);
				bool isChild = false;
				int Layer = 0;
				//
				{
					Layer = 0;
					MakeLists(Layer, true, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemCategoryList>(xp + xgoal, yp, DataBase::Instance()->GetItemCategoryData()->GetList(), "ItemCategory", &IDs->first, !IDs->second, false, true, [&](const auto *) { return true; });
					});
				}
				//
				{
					Layer = 1;
					bool CanGoNext = ((m_ItemIDs.at(Layer).first != InvalidID) && (DataBase::Instance()->GetItemTypeData()->FindPtr(m_ItemIDs.at(Layer).first)->GetName() == "Mechanical Key"));
					MakeLists(Layer, CanGoNext, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemTypeList>(xp + xgoal, yp, DataBase::Instance()->GetItemTypeData()->GetList(), "ItemType", &IDs->first, !IDs->second, false, true, [&](const auto *ptr) { return (ptr->GetCategoryID() == this->m_ItemIDs.at(Layer - 1).first); });
					});
				}
				//
				{
					Layer = 2;
					MakeLists(Layer, false, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<MapList>(xp + xgoal, yp, DataBase::Instance()->GetMapData()->GetList(), "Map", &IDs->first, !IDs->second, true, true, [&](const auto *) { return true; });
					});
				}
				//
				if (isChild) {
					xgoal -= xs_add / 2;
				}
				Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					bool isHit = false;
					if (!m_RaidMode) {
						for (auto it = this->m_ItemIDs.rbegin(); it != this->m_ItemIDs.rend(); ++it) {
							if (it->first != InvalidID) {
								it->first = InvalidID;
								isHit = true;
								break;
							}
						}
					}
					if (!isHit) {
						TurnOnGoNextBG();
					}
				}
			}
			//
			{
				auto PrevTask = this->m_TaskMode;
				int xp = y_r(1910) - LineHeight * 3;
				int yp = y_r(900);
				WindowSystem::CheckBox(xp, yp, &m_RaidMode);
				if (m_RaidMode) { this->m_TaskMode = false; }
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "レイドモード(ルート品のみ)");
				yp += LineHeight + y_r(6);
				WindowSystem::CheckBox(xp, yp, &m_TaskMode);
				if (m_TaskMode) { this->m_RaidMode = false; }
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "タスクモード");

				if (m_TaskMode && PrevTask != this->m_TaskMode) {
					std::sort(Items.begin(), Items.end(), [&](const ItemList* a, const ItemList* b) {
						return (a->GetUseTaskID().size() > b->GetUseTaskID().size());
					});
				}
			}
			//検索
			{

			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};
