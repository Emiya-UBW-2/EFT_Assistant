#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemListBG :public BGParent {
	private:
		bool														m_IsNeedItem{ true };//タスクに必要なアイテムを省くか否か
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
		}
		void LateExecute_Sub(int* xpos, int*, float*) noexcept override {
			*xpos = std::min(*xpos, y_r(50));
		}

		void Draw_Back_Sub(int xpos, int, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* Input = InputControl::Instance();

			Input->SetScaleActive(false);

			//int xpos = y_r(50);
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
						auto* ptr = ItemData::Instance()->FindPtr(w.GetID());
						if (ptr) {
							auto& Types = Counter.at(ptr->GetTypeID());
							auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == w.GetID()) && (obj.isFir == true); });
							if (Find != Types.end()) {
								Find->count += w.GetCount();
							}
							else {
								counts tmp;
								tmp.m_ID = w.GetID();
								tmp.count = w.GetCount();
								tmp.isFir = true;
								tmp.isNeed = false;
								Types.emplace_back(tmp);
							}
						}
					}
					for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
						auto* ptr = ItemData::Instance()->FindPtr(w.GetID());
						if (ptr) {
							auto& Types = Counter.at(ptr->GetTypeID());
							auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == w.GetID()) && (obj.isFir == false); });
							if (Find != Types.end()) {
								Find->count += w.GetCount();
							}
							else {
								counts tmp;
								tmp.m_ID = w.GetID();
								tmp.count = w.GetCount();
								tmp.isFir = false;
								tmp.isNeed = false;
								Types.emplace_back(tmp);
							}
						}
					}
					if (m_IsNeedItem) {
						for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
							auto* ptr = ItemData::Instance()->FindPtr(w.GetID());
							if (ptr) {
								auto& Types = Counter.at(ptr->GetTypeID());
								auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == w.GetID()) && (obj.isNeed == true); });
								if (Find != Types.end()) {
									Find->count += w.GetCount();
								}
								else {
									auto Find2 = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == w.GetID()) && (obj.isNeed == false); });
									if (Find2 == Types.end()) {
										counts tmp;
										tmp.m_ID = w.GetID();
										tmp.count = w.GetCount();
										tmp.isFir = false;
										tmp.isNeed = true;
										Types.emplace_back(tmp);
									}
								}
							}
						}
					}
				}
				//ハイドアウト開放
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
								auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == w.GetID()) && (obj.isFir == false) && (obj.isNeed == false); });
								if (Find != Types.end()) {
									Find->count += w.GetCount();
								}
								else {
									counts tmp;
									tmp.m_ID = w.GetID();
									tmp.count = w.GetCount();
									tmp.isFir = false;
									tmp.isNeed = true;
									Types.emplace_back(tmp);
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
									auto* ptr = ItemData::Instance()->FindPtr(c.m_ID);
									if (ptr) {
										if (yp >= (y_r(1080) - y_r(200))) {
											xp += xsizeAdd;
											yp = ypBase;
										}
										ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), c.isFir, false, false);
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
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* Input = InputControl::Instance();

			//タスクに必要なアイテム(サプレッサーなど)を追加
			{
				int xp = y_r(48);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(42) - y_r(42) - y_r(42);
				WindowSystem::CheckBox(xp, yp, &m_IsNeedItem);
				WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "タスクに必要なアイテム(サプレッサーなど)を追加");
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