#include "MainScene_PageItemList.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void ItemListBG::AddItemList(std::vector<counts>* Types, ItemID ID, int value, bool isFiR, bool isNeed) noexcept {
		auto Find = std::find_if(Types->begin(), Types->end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == isFiR) && (obj.isNeed == isNeed); });
		if (Find != Types->end()) {
			Find->count += value;
		}
		else {
			counts tmp;
			tmp.m_ID = ID;
			tmp.count = value;
			tmp.isFir = isFiR;
			tmp.isNeed = isNeed;
			Types->emplace_back(tmp);
		}
	}
	void ItemListBG::SetItemList() noexcept {
		Counter.resize(DataBase::Instance()->GetItemTypeData()->GetList().size());
		for (auto& Types : Counter) {
			Types.clear();
		}
		//タスク
		for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
			if (m_Mode == EnumListDrawMode::All || m_Mode == EnumListDrawMode::Task) {
				bool IsChecktask = tasks.GetIsHittoPlayerInfo();
				if (IsChecktask && PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str())) {
					IsChecktask = false;
				}
				if (IsChecktask && this->m_DrawCanClearTask) {//クリアできるタスクだけ表示
					if (tasks.GetTaskNeedData().GetParenttaskID().size() > 0) {
						for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
							if (!PlayerData::Instance()->GetTaskClear(DataBase::Instance()->GetTaskData()->FindPtr(p.GetID())->GetName().c_str())) {
								IsChecktask = false;
								break;
							}
						}
					}
				}
				if (!IsChecktask) { continue; }
			}
			if (m_Mode != EnumListDrawMode::Hideout) {
				for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
					auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
					if (ptr) {
						AddItemList(&Counter.at(ptr->GetTypeID()), w.GetID(), w.GetValue(), true, false);
					}
				}
				for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
					auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
					if (ptr) {
						AddItemList(&Counter.at(ptr->GetTypeID()), w.GetID(), w.GetValue(), false, false);
					}
				}
				if (m_IsNeedItem) {
					for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
						bool isHit = false;
						if (!isHit) {
							for (const auto& w2 : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w2.GetID() == w.GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (!isHit) {
							for (const auto& w2 : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w2.GetID() == w.GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (isHit) {
							continue;
						}
						auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
						if (ptr) {
							AddItemList(&Counter.at(ptr->GetTypeID()), w.GetID(), w.GetValue(), false, false);
						}
					}
				}
			}
		}
		//ハイドアウト開放
		for (const auto& L : DataBase::Instance()->GetHideoutData()->GetList()) {
			for (const auto& Ld : L.GetLvData()) {
				if (m_Mode == EnumListDrawMode::All || m_Mode == EnumListDrawMode::Hideout) {
					bool IsChecktask = true;
					if (IsChecktask && PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
						IsChecktask = false;
					}
					if (IsChecktask && this->m_DrawCanClearHideout) {
						for (const auto& w : Ld.m_Parent) {
							if (!PlayerData::Instance()->GetHideoutClear(DataBase::Instance()->GetHideoutData()->FindPtr(w.GetID())->GetName().c_str(), w.GetValue())) {
								IsChecktask = false;
								break;
							}
						}
					}
					if (!IsChecktask) { continue; }
				}
				if (m_Mode != EnumListDrawMode::Task) {
					for (const auto& w : Ld.m_ItemReq) {
						auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
						if (ptr) {
							AddItemList(&Counter.at(ptr->GetTypeID()), w.GetID(), w.GetValue(), false, false);
						}
					}
				}
			}
		}
		//何もないリスト
		if (m_Mode == EnumListDrawMode::None) {
			//除外リスト作成
			std::vector<ItemID> HitIDs;
			for (const auto& I : DataBase::Instance()->GetItemData()->GetList()) {
				HitIDs.emplace_back(I.GetID());
			}
			for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
				auto& Types = Counter.at(Type.GetID());
				for (auto& c : Types) {
					auto Find = std::find_if(HitIDs.begin(), HitIDs.end(), [&](const ItemID& obj) {return (obj == c.m_ID); });
					if (Find != HitIDs.end()) {
						HitIDs.erase(Find);
					}
				}
			}
			//追加設定:クラフト品等で該当するものの除外
			for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
				auto& Types = Counter.at(Type.GetID());
				for (auto& c : Types) {
					//ハイドアウトクラフト
					if (m_IsCheckCraft) {
						for (const auto& L : DataBase::Instance()->GetHideoutData()->GetList()) {
							for (const auto& Ld : L.GetLvData()) {
								for (const auto& cf : Ld.m_ItemCraft) {
									for (const auto& I : cf.m_ItemReward) {
										if (I.GetID() == c.m_ID) {
											for (const auto& r : cf.m_ItemReq) {
												auto Find = std::find_if(HitIDs.begin(), HitIDs.end(), [&](const ItemID& obj) {return (obj == r.GetID()); });
												if (Find != HitIDs.end()) {
													HitIDs.erase(Find);
												}
											}
											break;
										}
									}
								}
							}
						}
					}
					//交換
					if (m_IsCheckBarter) {
						for (auto& L : DataBase::Instance()->GetTraderData()->SetList()) {
							for (const auto& Ld : L.GetLvData()) {
								for (const auto& cf : Ld.m_ItemBarters) {
									for (const auto& I : cf.m_ItemReward) {
										if (I.GetID() == c.m_ID) {
											for (const auto& r : cf.m_ItemReq) {
												auto Find = std::find_if(HitIDs.begin(), HitIDs.end(), [&](const ItemID& obj) {return (obj == r.GetID()); });
												if (Find != HitIDs.end()) {
													HitIDs.erase(Find);
												}
											}
											break;
										}
									}
								}
							}
						}
					}
					//
				}
			}
			//反映
			for (auto& Types : Counter) {
				Types.clear();
			}
			for (auto& ID : HitIDs) {
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(1);
				AddItemList(&Counter.at(ptr->GetTypeID()), ID, 1, false, false);
			}
		}
	}

	void ItemListBG::DrawTab(int xp, int yp, std::string_view Info) noexcept {
		xp -= DXDraw::Instance()->GetUIY(64 + 4);
		auto size = WindowSystem::GetMsgLen(LineHeight, Info);
		WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_RIGHT, White, Black, Info);
		WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal, xp - size, yp + LineHeight, xp + DXDraw::Instance()->GetUIY(64), yp + LineHeight, Gray25, DXDraw::Instance()->GetUIY(4));
	}
	void ItemListBG::DrawCheckBox(int xp, int yp, std::string_view Info, bool* Check) noexcept {
		xp -= DXDraw::Instance()->GetUIY(64 + 4);
		auto OLD = *Check;
		*Check = WindowSystem::CheckBox(xp, yp, *Check);
		WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_RIGHT, White, Black, Info);
		if (OLD != *Check) {
			m_IsListChange = true;
		}
	}
	void ItemListBG::Init_Sub(int*, int*, float*) noexcept {
		m_Mode = EnumListDrawMode::Normal;
		m_IsListChange = true;

		m_SearchBox.Init();
	}
	void ItemListBG::Draw_Back_Sub(int, int, float) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		//auto* DrawParts = DXDraw::Instance();
		//auto* Pad = PadControl::Instance();

		int xpos = DXDraw::Instance()->GetUIY(50);
		int ypos = LineHeight + DXDraw::Instance()->GetUIY(10) + LineHeight + DXDraw::Instance()->GetUIY(10);
		{
			if (m_IsListChange) {
				SetItemList();
				m_IsListChange = false;
			}
			//描画
			{
				int xpBase = xpos;
				int ypBase = ypos - (int)m_YNow;
				int xp = xpBase;
				int yp = ypBase;
				int ypMax = (DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(20));
				int xsize = (DXDraw::Instance()->GetUIY(600));
				int ysize = (DXDraw::Instance()->GetUIY(64));
				int xsizeAdd = xsize + DXDraw::Instance()->GetUIY(30);
				int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);
				for (auto& Cat : DataBase::Instance()->GetItemCategoryData()->GetList()) {
					bool IsHit = false;
					for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
						if (Type.GetCategoryID() == Cat.GetID()) {
							auto& Types = Counter.at(Type.GetID());
							for (auto& c : Types) {
								auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.m_ID);
								if (ptr) {
									//検索結果
									if (!m_SearchBox.GetIsHit(ptr->GetName())) {
										continue;
									}
									//
									if (ypos - ysizeAdd < yp && yp < ypMax) {
										if (ypos - 1 < yp && yp < ypMax - ysizeAdd + 1) {
											WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
											ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), c.isFir, false, false);
											//サイド描画
											int xp2 = xp + xsizeAdd + DXDraw::Instance()->GetUIY(10);
											int yp2 = yp;
											int xsize2 = ysize / 2;
											int ysize2 = ysize / 2 - DXDraw::Instance()->GetUIY(3);
											//ハイドアウトクラフト
											if (m_IsCheckCraft) {
												for (const auto& L : DataBase::Instance()->GetHideoutData()->GetList()) {
													for (const auto& Ld : L.GetLvData()) {
														int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
														/*
														bool IsChecktask = true;
														if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), Lv)) {
															IsChecktask = false;
														}
														if (!IsChecktask) { continue; }
														//*/
														for (const auto& cf : Ld.m_ItemCraft) {
															for (const auto& I : cf.m_ItemReward) {
																int craftcount = std::max(1, c.count / I.GetValue());
																if (I.GetID() == c.m_ID) {
																	xp2 += L.DrawCraft(nullptr, Gray15, xp2, yp2, DXDraw::Instance()->GetUIY(64), Lv, (int)(&cf - &Ld.m_ItemCraft.front()), true, false, craftcount) + DXDraw::Instance()->GetUIY(10);
																	break;
																}
															}
														}
													}
												}
											}
											//交換
											if (m_IsCheckBarter && !c.isFir) {
												for (auto& L : DataBase::Instance()->GetTraderData()->SetList()) {
													for (const auto& Ld : L.GetLvData()) {
														int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;

														for (const auto& cf : Ld.m_ItemBarters) {
															for (const auto& I : cf.m_ItemReward) {
																int craftcount = std::max(1, c.count / I.GetValue());
																if (I.GetID() == c.m_ID) {
																	yp2 = yp;
																	WindowSystem::SetMsg(xp2, yp2 + ysize2 / 2, ysize2, STRX_LEFT, White, Black, L.GetName() + " Lv" + std::to_string(Lv) + "x" + std::to_string(craftcount));
																	auto xl = xp2 + WindowSystem::GetMsgLen(ysize2, L.GetName() + " Lv" + std::to_string(Lv) + "x" + std::to_string(craftcount));
																	yp2 += ysize2 + DXDraw::Instance()->GetUIY(5);

																	for (const auto& r : cf.m_ItemReq) {
																		auto* ptr2 = DataBase::Instance()->GetItemData()->FindPtr(r.GetID());
																		if (ptr2) {
																			int count = r.GetValue() * craftcount;
																			xp2 += ptr2->Draw(xp2, yp2, xsize2, ysize2, (count >= 2) ? count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, true) + DXDraw::Instance()->GetUIY(5);
																		}
																	}
																	xp2 = std::max(xl, xp2);
																	xp2 += DXDraw::Instance()->GetUIY(10);
																	break;
																}
															}
														}
														//クラフトベース
													}
												}
											}
										}
										else {
											if (yp <= ypos) {
												WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypos - yp) / ysizeAdd, 0, 255));
											}
											else {
												WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
											}
											ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, false, c.isFir, false, false);
										}
									}
									yp += ysizeAdd;
									IsHit = true;
								}
							}
						}
					}
					if (IsHit) {
						yp += DXDraw::Instance()->GetUIY(10);
					}
				}
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);

				int ScrPosX = xpos + xsizeAdd;
				int ScrSizY = ypMax - ypos;
				m_Scroll.SetScrollBoxParam(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp - ypBase, ScrSizY) / (float)ScrSizY, !WindowMngr->PosHitCheck(nullptr));
				m_Scroll.ScrollBox();

				m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer() * (float)((yp - ypBase) - ScrSizY));
			}
		}
	}
	void ItemListBG::DrawFront_Sub(int, int, float) noexcept {
		//モードセレクト
		{
			int xp = DXDraw::Instance()->GetUIY(10) + DXDraw::Instance()->GetUIY(200) + DXDraw::Instance()->GetUIY(10);
			int yp = LineHeight + DXDraw::Instance()->GetUIY(10);
			if (WindowSystem::SetMsgClickBox(xp, yp, xp + DXDraw::Instance()->GetUIY(400), yp + LineHeight, LineHeight, Gray25, false, true, ListDrawModeStr[(int)m_Mode])) {
				m_Mode = (EnumListDrawMode)(((int)m_Mode + 1) % (int)EnumListDrawMode::Max);
				m_IsListChange = true;
			}
			int Max = (int)EnumListDrawMode::Max;
			for (int i = 0; i < Max; i++) {
				WindowSystem::SetBox(
					xp + DXDraw::Instance()->GetUIY(400) * i / Max + DXDraw::Instance()->GetUIY(5), yp + LineHeight + DXDraw::Instance()->GetUIY(4),
					xp + DXDraw::Instance()->GetUIY(400) * (i + 1) / Max - DXDraw::Instance()->GetUIY(5), yp + LineHeight + DXDraw::Instance()->GetUIY(4) + DXDraw::Instance()->GetUIY(6),
					(m_Mode == (EnumListDrawMode)i) ? Green : Gray25);
			}
		}
		//
		{
			int xp = DXDraw::Instance()->GetUIY(1920) - DXDraw::Instance()->GetUIY(16);
			int yp = DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(110) - DXDraw::Instance()->GetUIY(40) * 8;
			//背景
			{
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 32);
				WindowSystem::SetBox(xp - DXDraw::Instance()->GetUIY(490), yp, xp + DXDraw::Instance()->GetUIY(5), yp + DXDraw::Instance()->GetUIY(40) * 8 + DXDraw::Instance()->GetUIY(5), Black);
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
			}
			//サイド表示
			{
				DrawTab(xp, yp, "サイド表示:"); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "トレーダー交換", &m_IsCheckBarter); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "クラフト", &m_IsCheckCraft); yp += DXDraw::Instance()->GetUIY(40);
			}
			//タスク
			if (m_Mode == EnumListDrawMode::All || m_Mode == EnumListDrawMode::Task) {
				DrawTab(xp, yp, "タスク:"); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "クリア可能対象のみ", &m_DrawCanClearTask); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "必要品(鍵など)を追加", &m_IsNeedItem); yp += DXDraw::Instance()->GetUIY(40);
			}
			//ハイドアウト
			if (m_Mode == EnumListDrawMode::All || m_Mode == EnumListDrawMode::Hideout) {
				DrawTab(xp, yp, "ハイドアウト:"); yp += DXDraw::Instance()->GetUIY(40);
				DrawCheckBox(xp, yp, "開放可能対象のみ", &m_DrawCanClearHideout); yp += DXDraw::Instance()->GetUIY(40);
			}
		}
		//検索
		m_SearchBox.Draw(DXDraw::Instance()->GetUIY(1910) - DXDraw::Instance()->GetUIY(500), DXDraw::Instance()->GetUIY(1080) - DXDraw::Instance()->GetUIY(110) - DXDraw::Instance()->GetUIY(40) * 8 - DXDraw::Instance()->GetUIY(40));
		//
	}
};
