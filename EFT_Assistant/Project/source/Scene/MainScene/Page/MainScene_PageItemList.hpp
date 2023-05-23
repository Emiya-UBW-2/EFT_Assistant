#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemListBG :public BGParent {
	private:
		bool														m_IsCheckCraft{ true };//タスクに必要なアイテムを省くか否か
		bool														m_IsNeedItem{ true };//タスクに必要なアイテムを省くか否か
		bool														m_IsCheckOpenCraft{ true };
		bool														m_IsCheckTask{ true };
		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}

		void Draw_Back_Sub(int, int, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* DrawParts = DXDraw::Instance();
			//auto* Input = InputControl::Instance();

			int xpos = y_r(50);
			int ypos = LineHeight + y_r(10) + LineHeight + y_r(10);
			{
				struct counts
				{
					ItemID m_ID{ InvalidID };
					int count;
					bool isFir{ true };
					bool isNeed{ false };
				};
				std::vector<std::vector<counts>> Counter;
				Counter.resize(ItemTypeData::Instance()->GetList().size());
				//タスク
				if (m_IsCheckTask) {
					for (const auto& tasks : TaskData::Instance()->GetList()) {
						bool IsChecktask = true;
						if (PlayerData::Instance()->GetIsNeedKappa()) {//河童必要タスクだけ書く
							if (!tasks.GetTaskNeedData().GetKappaRequired()) {
								IsChecktask = false;
							}
							if (IsChecktask) {
								if (tasks.GetName() == "Collector") {
									//IsChecktask = false;
								}
							}
						}
						if (PlayerData::Instance()->GetIsNeedLightKeeper()) {//ライトキーパー
							if (!tasks.GetTaskNeedData().GetLightKeeperRequired()) {
								IsChecktask = false;
							}
						}
						if (PlayerData::Instance()->GetMaxLevel() < tasks.GetTaskNeedData().GetLevel()) {//最大レベル
							IsChecktask = false;
						}
						if (PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str())) {
							IsChecktask = false;
						}
						if (!IsChecktask) { continue; }
						for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
							auto ID = ItemData::Instance()->FindID(w.GetName());
							auto* ptr = ItemData::Instance()->FindPtr(ID);
							if (ptr) {
								auto& Types = Counter.at(ptr->GetTypeID());
								auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == true); });
								if (Find != Types.end()) {
									Find->count += w.GetValue();
								}
								else {
									counts tmp;
									tmp.m_ID = ID;
									tmp.count = w.GetValue();
									tmp.isFir = true;
									tmp.isNeed = false;
									Types.emplace_back(tmp);
								}
							}
						}
						for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
							auto ID = ItemData::Instance()->FindID(w.GetName());
							auto* ptr = ItemData::Instance()->FindPtr(ID);
							if (ptr) {
								auto& Types = Counter.at(ptr->GetTypeID());
								auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == false); });
								if (Find != Types.end()) {
									Find->count += w.GetValue();
								}
								else {
									counts tmp;
									tmp.m_ID = ID;
									tmp.count = w.GetValue();
									tmp.isFir = false;
									tmp.isNeed = false;
									Types.emplace_back(tmp);
								}
							}
						}
						if (m_IsNeedItem) {
							for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
								auto ID = ItemData::Instance()->FindID(w.GetName());
								auto* ptr = ItemData::Instance()->FindPtr(ID);
								if (ptr) {
									auto& Types = Counter.at(ptr->GetTypeID());
									auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isNeed == true); });
									if (Find != Types.end()) {
										Find->count += w.GetValue();
									}
									else {
										auto Find2 = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isNeed == false); });
										if (Find2 == Types.end()) {
											counts tmp;
											tmp.m_ID = ID;
											tmp.count = w.GetValue();
											tmp.isFir = false;
											tmp.isNeed = true;
											Types.emplace_back(tmp);
										}
									}
								}
							}
						}
					}
				}
				//ハイドアウト開放
				if (m_IsCheckOpenCraft) {
					for (const auto&L : HideoutData::Instance()->GetList()) {
						for (const auto& Ld : L.GetLvData()) {
							bool IsChecktask = true;
							if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
								IsChecktask = false;
							}
							if (!IsChecktask) { continue; }
							for (const auto& w : Ld.m_ItemReq) {
								auto ID = ItemData::Instance()->FindID(w.GetName());
								auto* ptr = ItemData::Instance()->FindPtr(ID);
								if (ptr) {
									auto& Types = Counter.at(ptr->GetTypeID());
									auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == false) && (obj.isNeed == false); });
									if (Find != Types.end()) {
										Find->count += w.GetValue();
									}
									else {
										counts tmp;
										tmp.m_ID = ID;
										tmp.count = w.GetValue();
										tmp.isFir = false;
										tmp.isNeed = true;
										Types.emplace_back(tmp);
									}
								}
							}
						}
					}
				}
				//描画
				{
					int xpBase = xpos;
					int ypBase = ypos - (int)m_YNow;
					int xp = xpBase;
					int yp = ypBase;
					int ypMax = (y_r(1080) - y_r(200));
					int xsize = (int)((float)y_r(600));
					int ysize = (int)((float)y_r(60));
					int xsizeAdd = xsize + y_r(30);
					int ysizeAdd = ysize + y_r(5);
					for (auto& Cat : ItemCategoryData::Instance()->GetList()) {
						bool IsHit = false;
						for (auto& Type : ItemTypeData::Instance()->GetList()) {
							if (Type.GetCategoryID() == Cat.GetID()) {
								auto& Types = Counter.at(Type.GetID());
								for (auto& c : Types) {
									auto* ptr = ItemData::Instance()->FindPtr(c.m_ID);
									if (ptr) {
										if (ypos - ysizeAdd < yp && yp < ypMax) {
											if (ypos < yp && yp < ypMax - ysizeAdd) {
												DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
												ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), c.isFir, false, false);

												//ハイドアウトクラフト
												if (m_IsCheckCraft) {
													int xp2 = xp + xsizeAdd + y_r(10);
													int yp2 = yp;
													int xsize2 = y_r(140);
													int ysize2 = ysize / 2 - y_r(3);
													for (const auto&L : HideoutData::Instance()->GetList()) {
														for (const auto& Ld : L.GetLvData()) {
															int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
															/*
															bool IsChecktask = true;
															if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
																IsChecktask = false;
															}
															if (!IsChecktask) { continue; }
															//*/
															for (const auto& C : Ld.m_ItemCraft) {
																for (const auto& R : C.m_ItemReward) {
																	auto RID = ItemData::Instance()->FindID(R.GetName());
																	int craftcount = std::max(1, c.count / R.GetValue());

																	if (RID == c.m_ID) {
																		yp2 = yp;
																		std::string NameTmp = L.GetName();
																		NameTmp += " Lv" + std::to_string(Lv);
																		NameTmp += " x" + std::to_string(craftcount);
																		WindowSystem::SetMsg(xp2, yp2, xp2, yp2 + ysize2, ysize2, STRX_LEFT, White, Black, NameTmp);
																		yp2 += ysize2 + y_r(5);
																		for (const auto& w : C.m_ItemReq) {
																			auto ID = ItemData::Instance()->FindID(w.GetName());
																			auto* ptr2 = ItemData::Instance()->FindPtr(ID);
																			if (ptr2) {
																				int count = w.GetValue()*craftcount;
																				ptr2->Draw(xp2, yp2, xsize2, ysize2, (count >= 2) ? count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, true);
																				xp2 += xsize2 + y_r(5);
																			}
																		}
																		xp2 += y_r(10);
																		break;
																	}
																}
															}
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
					m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp- ypBase, ScrSizY) / (float)ScrSizY, true);

					m_YNow = std::max(0.f, m_Scroll.GetNowScrollYPer()*(float)((yp - ypBase) - ScrSizY));
				}
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* Input = InputControl::Instance();

			//タスクに必要なアイテム(サプレッサーなど)を追加
			{
				int xp = y_r(48);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42) - y_r(42) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &m_IsNeedItem);
				WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "タスクに必要なアイテム(サプ等)を追加");
			}
			//ライトキーパーに必要か
			{
				auto tmp = PlayerData::Instance()->GetIsNeedLightKeeper();
				int xp = y_r(48);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &tmp);
				WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "ライトキーパー開放までに絞る");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedKappa(false);
				}
				PlayerData::Instance()->SetIsNeedLightKeeper(tmp);
			}
			//カッパに必要か
			{
				auto tmp = PlayerData::Instance()->GetIsNeedKappa();
				int xp = y_r(48);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &tmp);
				WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "カッパー開放までに絞る");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedLightKeeper(false);
				}
				PlayerData::Instance()->SetIsNeedKappa(tmp);
			}
			//レベル操作
			{
				int xp = y_r(5);
				int yp = y_r(1080) - y_r(48) - y_r(5);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(100), yp + y_r(48), true, true, Red, "DOWN")) {
					PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() - 1);
				}
				xp += y_r(105);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(100), yp + y_r(48), true, true, Green, "UP")) {
					PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() + 1);
				}
				PlayerData::Instance()->SetMaxLevel(std::clamp(PlayerData::Instance()->GetMaxLevel(), 1, 71));
				xp += y_r(105);
				WindowSystem::SetMsg(xp, yp + y_r(12), xp, yp + y_r(12) + y_r(36), y_r(36), STRX_LEFT, White, Black, "MaxLevel");
				xp += y_r(250);
				WindowSystem::SetMsg(xp, yp, xp, yp + y_r(48), y_r(48), STRX_RIGHT, White, Black, "%d", PlayerData::Instance()->GetMaxLevel());
			}

			//ハイドアウトクラフトを確認
			{
				int xp = y_r(1920) - y_r(48) - y_r(200);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42) - y_r(42) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &m_IsCheckCraft);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "ハイドアウトクラフト対象をサイド表示");
			}
			//トレーダー交換を確認
			{
				int xp = y_r(1920) - y_r(48) - y_r(200);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &m_IsCheckCraft);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "トレーダー交換をサイド表示(未実装)");
			}
			//ハイドアウトのみを確認
			{
				int xp = y_r(1920) - y_r(48) - y_r(200);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &m_IsCheckOpenCraft);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "ハイドアウト開放アイテムをリスト表示");
			}
			//タスクのみを確認
			{
				int xp = y_r(1920) - y_r(48) - y_r(200);
				int yp = y_r(1080) - y_r(48) - y_r(5);
				WindowSystem::CheckBox(xp, yp, &m_IsCheckTask);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "タスクに使用するアイテムをリスト表示");
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
		}
	};
};