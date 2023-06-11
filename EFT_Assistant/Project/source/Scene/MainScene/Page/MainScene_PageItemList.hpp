#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	enum class EnumListDrawMode {
		Normal,
		All = Normal,
		Task,
		Hideout,
		None,
		Max,
	};
	static const char* ListDrawModeStr[(int)EnumListDrawMode::Max] = {
	"全て",
	"タスクのみ",
	"ハイドアウトのみ",
	"未使用",
	};

	class ItemListBG :public BGParent {
	private:
		bool							m_IsCheckCraft{ true };//タスクに必要なアイテムを省くか否か
		bool							m_IsCheckBarter{ true };//タスクに必要なアイテムを省くか否か
		bool							m_IsNeedItem{ true };//タスクに必要なアイテムを省くか否か

		bool							m_DrawCanClearTask{ false };
		bool							m_DrawCanClearHideout{ false };

		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };

		EnumListDrawMode				m_Mode{ EnumListDrawMode::Normal };
		bool							m_IsListChange{ true };

		struct counts
		{
			ItemID	m_ID{ InvalidID };
			int		count{ 0 };
			bool	isFir{ true };
			bool	isNeed{ false };
		};
		std::vector<std::vector<counts>> Counter;
	private:
		void AddItemList(std::vector<counts>* Types,ItemID ID,int value,bool isFiR, bool isNeed) noexcept {
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
		void SetItemList() noexcept {
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
			for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
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
							for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
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
							for (auto&L : DataBase::Instance()->GetTraderData()->SetList()) {
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

		void DrawTab(int xp, int yp, std::string_view Info) noexcept {
			xp -= y_r(64 + 4);
			auto size = WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, Info);
			DrawControl::Instance()->SetDrawLine(DrawLayer::Normal, xp - size, yp + LineHeight, xp + y_r(64), yp + LineHeight, Gray25, y_r(4));
		}
		void DrawCheckBox(int xp, int yp, std::string_view Info,bool* Check) noexcept {
			xp -= y_r(64 + 4);
			auto OLD = *Check;
			WindowSystem::CheckBox(xp, yp, Check);
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, Info);
			if (OLD != *Check) {
				m_IsListChange = true;
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Mode = EnumListDrawMode::Normal;
			m_IsListChange = true;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {}

		void Draw_Back_Sub(int, int, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* DrawParts = DXDraw::Instance();
			//auto* Input = InputControl::Instance();

			int xpos = y_r(50);
			int ypos = LineHeight + y_r(10) + LineHeight + y_r(10);
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
					int ypMax = (y_r(1080) - y_r(20));
					int xsize = (y_r(600));
					int ysize = (y_r(64));
					int xsizeAdd = xsize + y_r(30);
					int ysizeAdd = ysize + y_r(5);
					for (auto& Cat : DataBase::Instance()->GetItemCategoryData()->GetList()) {
						bool IsHit = false;
						for (auto& Type : DataBase::Instance()->GetItemTypeData()->GetList()) {
							if (Type.GetCategoryID() == Cat.GetID()) {
								auto& Types = Counter.at(Type.GetID());
								for (auto& c : Types) {
									auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.m_ID);
									if (ptr) {
										if (ypos - ysizeAdd < yp && yp < ypMax) {
											if (ypos - 1 < yp && yp < ypMax - ysizeAdd + 1) {
												DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
												ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), c.isFir, false, false);
												//サイド描画
												int xp2 = xp + xsizeAdd + y_r(10);
												int yp2 = yp;
												int xsize2 = ysize / 2;
												int ysize2 = ysize / 2 - y_r(3);
												//ハイドアウトクラフト
												if (m_IsCheckCraft) {
													for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
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
																		xp2 += L.DrawCraft(nullptr, Gray15, xp2, yp2, y_r(64), Lv, (int)(&cf - &Ld.m_ItemCraft.front()), true, false, craftcount) + y_r(10);
																		break;
																	}
																}
															}
														}
													}
												}
												//交換
												if (m_IsCheckBarter && !c.isFir) {
													for (auto&L : DataBase::Instance()->GetTraderData()->SetList()) {
														for (const auto& Ld : L.GetLvData()) {
															int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;

															for (const auto& cf : Ld.m_ItemBarters) {
																for (const auto& I : cf.m_ItemReward) {
																	int craftcount = std::max(1, c.count / I.GetValue());
																	if (I.GetID() == c.m_ID) {
																		yp2 = yp;
																		auto xl = xp2 + WindowSystem::SetMsg(xp2, yp2, xp2, yp2 + ysize2, ysize2, STRX_LEFT, White, Black, L.GetName() + " Lv" + std::to_string(Lv) + "x" + std::to_string(craftcount));
																		yp2 += ysize2 + y_r(5);

																		for (const auto& r : cf.m_ItemReq) {
																			auto* ptr2 = DataBase::Instance()->GetItemData()->FindPtr(r.GetID());
																			if (ptr2) {
																				int count = r.GetValue()*craftcount;
																				xp2 += ptr2->Draw(xp2, yp2, xsize2, ysize2, (count >= 2) ? count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, true) + y_r(5);
																			}
																		}
																		xp2 = std::max(xl, xp2);
																		xp2 += y_r(10);
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
													DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (ypos - yp) / ysizeAdd, 0, 255));
												}
												else {
													DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (yp - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
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
							yp += y_r(10);
						}
					}
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);

					int ScrPosX = xpos + xsizeAdd;
					int ScrSizY = ypMax - ypos;
					m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp - ypBase, ScrSizY) / (float)ScrSizY, true);

					m_YNow = std::max(0.f, this->m_Scroll.GetNowScrollYPer()*(float)((yp - ypBase) - ScrSizY));
				}
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					TurnOnGoNextBG();
				}
			}
			//モードセレクト
			{
				int xp = y_r(10) + y_r(200) + y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(400), yp + LineHeight, false, true, Gray25, ListDrawModeStr[(int)m_Mode])) {
					m_Mode = (EnumListDrawMode)(((int)m_Mode + 1) % (int)EnumListDrawMode::Max);
					m_IsListChange = true;
				}
				int Max = (int)EnumListDrawMode::Max;
				for (int i = 0; i < Max; i++) {
					WindowSystem::SetBox(
						xp + y_r(400)*i / Max + y_r(5), yp + LineHeight + y_r(4),
						xp + y_r(400)*(i + 1) / Max - y_r(5), yp + LineHeight + y_r(4) + y_r(6),
						(m_Mode == (EnumListDrawMode)i) ? Green : Gray25);
				}
			}
			//
			{
				int xp = y_r(1920) - y_r(16);
				int yp = y_r(1080) - y_r(110) - y_r(40)*8;
				//背景
				{
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 32);
					DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp - y_r(490), yp, xp + y_r(5), yp + y_r(40) * 8 + y_r(5), Black, true);
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
				}
				//サイド表示
				{
					DrawTab(xp, yp, "サイド表示:"); yp += y_r(40);
					DrawCheckBox(xp, yp, "トレーダー交換", &m_IsCheckBarter); yp += y_r(40);
					DrawCheckBox(xp, yp, "クラフト", &m_IsCheckCraft); yp += y_r(40);
				}
				//タスク
				if (m_Mode == EnumListDrawMode::All || m_Mode == EnumListDrawMode::Task) {
					DrawTab(xp, yp, "タスク:"); yp += y_r(40);
					DrawCheckBox(xp, yp, "クリア可能対象のみ", &m_DrawCanClearTask); yp += y_r(40);
					DrawCheckBox(xp, yp, "必要品(鍵など)を追加", &m_IsNeedItem); yp += y_r(40);
				}
				//ハイドアウト
				if (m_Mode == EnumListDrawMode::All || m_Mode == EnumListDrawMode::Hideout) {
					DrawTab(xp, yp, "ハイドアウト:"); yp += y_r(40);
					DrawCheckBox(xp, yp, "開放可能対象のみ", &m_DrawCanClearHideout); yp += y_r(40);
				}
			}
			//
		}
		void Dispose_Sub(void) noexcept override {}
	};
};
