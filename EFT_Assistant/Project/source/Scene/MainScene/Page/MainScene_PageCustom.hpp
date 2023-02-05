#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class CustomBG :public BGParent {
		class ChildData {
			const ItemList*	m_ParentPtr{ nullptr };
			int				m_ParentSlot{ 0 };
			int				ChildSel{ 0 };
			int				watchCounter{ 0 };
		public:
			const auto& GetMySlotData() const noexcept { return this->m_ParentPtr->GetChildParts().at(this->m_ParentSlot); }
			const auto GetPtrIsParentSlot(const ItemList* parentptr, int parentslot) const noexcept { return (this->m_ParentPtr == parentptr) && (this->m_ParentSlot == parentslot); }
			const auto GetIsSelected(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					return (this->ChildSel < (int)(GetMySlotData().Data.size()));
				}
				return false;
			}
			void	AddSelect() noexcept {
				++this->ChildSel %= (GetMySlotData().Data.size() + 1);
			}
			const ItemList* GetChildPtr(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					if (GetIsSelected(parentslot)) {
						return this->GetMySlotData().Data.at(this->ChildSel).first;
					}
				}
				return nullptr;
			}
			const auto ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter) const noexcept {
				bool IsHit = false;
				auto* ptr = GetChildPtr(parentslot);
				if (ptr) {
					if (MagFilter) {
						if (
							ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Magazine")
							) {
							IsHit = true;
						}
					}
					if (MountFilter) {
						if (
							ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Mount")
							) {
							IsHit = true;
						}
					}
					if (SightFilter) {
						if (
							ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Reflex sight")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Compact reflex sight")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Assault scope")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Scope")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Special scope")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Iron sight")
							) {
							IsHit = true;
						}
					}
				}
				return IsHit;
			}
		public:
		public:
			const auto IsWatch() const noexcept { return this->watchCounter > 0; }
			void	ResetWatchCounter() noexcept { this->watchCounter = 0; }
			void	AddWatchCounter() noexcept { ++this->watchCounter; }
		public:
			void	Set(const ItemList* parentptr, int parentslot, int Select) noexcept {
				this->m_ParentPtr = parentptr;
				this->m_ParentSlot = parentslot;
				this->ChildSel = Select;
				this->watchCounter = 1;
			}
		};
	private://アイテムのやつ
		std::vector<std::pair<int, bool>>	m_ItemIDs;
		float							m_XChild{ 0.f };
		void MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*)>& ListSet) noexcept {
			auto& NowSel = m_ItemIDs.at(Layer);
			NowSel.second = ((NowSel.first != InvalidID) && AndNext);
			if (Layer == 0 || (Layer >= 1 && m_ItemIDs.at(Layer - 1).second)) {
				ListSet(&NowSel);
			}
			else {
				NowSel.first = InvalidID;
			}
		}
	private:
		ItemID	m_SelectBuffer{ InvalidID };
		ItemList* m_BaseWeapon{ nullptr };

		bool m_EnableMag = true;
		bool m_EnableMount = false;
		bool m_EnableSight = false;
		int m_Recoil = 50;
		int m_Ergonomics = 50;

		std::vector<Rect2D>				m_ItemRect;
		std::vector<ChildData>			m_ChildData;
	private:
		//プリセットを適応
		void AttachPreset(const PresetList& Preset, const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				m_ChildData.clear();
				Ptr_Buf = m_BaseWeapon;
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				bool IsHit = false;
				for (auto& cID : m_ChildData) {
					if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
						if (cID.GetIsSelected() && cID.GetChildPtr()) {
							AttachPreset(Preset, cID.GetChildPtr());
						}
						IsHit = true;
						break;
					}
				}
				if (!IsHit) {
					//設定ベクターに追加
					int ChildSel = -1;
					for (const auto& P : Preset.GetParts()) {
						bool IsHit2 = false;
						for (auto& cptr : c.Data) {
							if (P == cptr.first) {
								IsHit2 = true;
								ChildSel = (int)(&cptr - &c.Data.front());
								break;
							}
						}
						if (IsHit2) { break; }
					}
					if (ChildSel != -1) {
						//設定vectorに追加
						m_ChildData.resize(m_ChildData.size() + 1);
						m_ChildData.back().Set(Ptr_Buf, Index, ChildSel);
						AttachPreset(Preset, m_ChildData.back().GetChildPtr());
					}
				}
			}
		}
		//設定
		bool CheckConflict(const ItemList* MyPtr, const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				Ptr_Buf = m_BaseWeapon;
			}
			for (const auto& cP : Ptr_Buf->GetConflictParts()) {
				if (cP.first == MyPtr) {
					return true;
				}
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				for (const auto& cID : m_ChildData) {
					if (cID.IsWatch() && cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
						if (cID.GetIsSelected() && CheckConflict(MyPtr, cID.GetChildPtr())) {
							return true;
						}
						break;
					}
				}
			}
			return false;
		}
		void CalcChild(const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				for (auto& cID : m_ChildData) {
					cID.ResetWatchCounter();
				}
				Ptr_Buf = m_BaseWeapon;
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				bool IsHit = false;
				for (auto& cID : m_ChildData) {
					if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
						cID.AddWatchCounter();
						if (cID.GetIsSelected()) {
							CalcChild(cID.GetChildPtr());
						}
						IsHit = true;
						break;
					}
				}
				if (!IsHit) {
					//設定vectorに追加
					m_ChildData.resize(m_ChildData.size() + 1);
					m_ChildData.back().Set(Ptr_Buf, Index, (int)(c.Data.size()));
				}
			}
			if (Ptr == nullptr) {
				//未使用品を除外
				for (int count = 0; count < m_ChildData.size(); count++) {
					if (!m_ChildData[count].IsWatch()) {
						m_ChildData.erase(m_ChildData.begin() + count);
						count--;
					}
				}
				//フィルターに引っかかっていたら大丈夫なものにする
				for (auto& cID : m_ChildData) {
					while (true) {
						if (cID.GetIsSelected() && cID.ChecktoBeFiltered(-1, !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
							cID.AddSelect();
							continue;
						}
						break;
					}
				}
				//性能更新
				{
					float RecoilPer = 0.f;
					float ErgonomicsPer = 0.f;
					for (const auto& cID : m_ChildData) {
						if (cID.GetIsSelected()) {
							auto* ChildPtr = cID.GetChildPtr();
							RecoilPer += ChildPtr->GetRecoil();
							ErgonomicsPer += ChildPtr->GetErgonomics();
						}
					}
					m_Recoil = (int)(m_BaseWeapon->GetRecoil()*(100.f + RecoilPer) / 100.f);
					m_Ergonomics = (int)(m_BaseWeapon->GetErgonomics() + ErgonomicsPer);
				}
			}
		}
		//描画
		void DrawChild(int xpbase, int ypbase, const ItemList* Ptr, float Scale) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();

			for (const auto& c : Ptr->GetChildParts()) {
				auto Index = (int)(&c - &Ptr->GetChildParts().front());
				for (auto& cID : m_ChildData) {
					if (cID.GetPtrIsParentSlot(Ptr, Index)) {
						int xp = xpbase + (int)(Scale * c.xpos);
						int yp = ypbase + (int)(Scale * c.ypos);
						float len = std::hypotf((float)xp, (float)yp);
						int xp2 = xp + (int)((float)xp * (float)y_r(384) / len);
						int yp2 = yp + (int)((float)yp * (float)y_r(384) / len);

						Rect2D P_Next;

						int xsize = y_r(400);
						int ysize = y_r(80);
						int xbase = (xp2 > 0) ? (y_r(960) + xp2) : (y_r(960) + xp2 - xsize);
						int ybase = y_r(540) + yp2 - ysize / 2;

						if (xbase < 0) {
							xp2 += (-xbase);
							xbase += (-xbase);
						}
						else if ((xbase + xsize - y_r(1920)) > 0) {
							xp2 += (-(xbase + xsize - y_r(1920)));
							xbase += (-(xbase + xsize - y_r(1920)));
						}
						if (ybase - LineHeight < 0) {
							yp2 += (-(ybase - LineHeight));
							ybase += (-(ybase - LineHeight));
						}
						else if ((ybase + ysize - y_r(1080)) > 0) {
							yp2 += (-(ybase + ysize - y_r(1080)));
							ybase += (-(ybase + ysize - y_r(1080)));
						}

						xbase = (xp2 > 0) ? (y_r(960) + xp2) : (y_r(960) + xp2 - xsize);
						ybase = y_r(540) + yp2 - ysize / 2;
						P_Next.Set(xbase, ybase, xsize, ysize);
						int counter = 0;
						while (true) {
							bool isHit = false;
							for (auto&r : m_ItemRect) {
								if (r.IsHit(P_Next)) {
									isHit = true;

									if (counter < 100) {
										//中央によるように配置
										if (xbase + xsize / 2 > y_r(960)) {
											xp2 -= xsize + y_r(5);
										}
										else {
											xp2 += xsize + y_r(5);
										}

										if (ybase + ysize / 2 > y_r(540)) {
											yp2 -= ysize + y_r(5);
										}
										else {
											yp2 += ysize + y_r(5);
										}
									}
									else {
										xp2 -= xsize + y_r(5);
										yp2 -= ysize + y_r(5);
									}
									xbase = (xp2 > 0) ? (y_r(960) + xp2) : (y_r(960) + xp2 - xsize);
									ybase = y_r(540) + yp2 - ysize / 2;

									if (xbase < 0) {
										xp2 += (-xbase);
										xbase += (-xbase);
									}
									else if ((xbase + xsize - y_r(1920)) > 0) {
										xp2 += (-(xbase + xsize - y_r(1920)));
										xbase += (-(xbase + xsize - y_r(1920)));
									}
									if (ybase - LineHeight < 0) {
										yp2 += (-(ybase - LineHeight));
										ybase += (-(ybase - LineHeight));
									}
									else if ((ybase + ysize - y_r(1080)) > 0) {
										yp2 += (-(ybase + ysize - y_r(1080)));
										ybase += (-(ybase + ysize - y_r(1080)));
									}

									xbase = (xp2 > 0) ? (y_r(960) + xp2) : (y_r(960) + xp2 - xsize);
									ybase = y_r(540) + yp2 - ysize / 2;
									P_Next.Set(xbase, ybase, xsize, ysize);
									break;
								}
							}
							counter++;
							if (counter > 200) {
								break;
							}
							if (!isHit) { break; }
						}
						m_ItemRect.emplace_back(P_Next);

						DrawControl::Instance()->SetDrawLine(y_r(960) + xp, y_r(540) + yp, y_r(960) + xp2, y_r(540) + yp2, Red, y_r(3));

						if (WindowSystem::ClickCheckBox(P_Next.GetPosX(), P_Next.GetPosY(), P_Next.GetPosX() + xsize, P_Next.GetPosY() + ysize, true, !WindowMngr->PosHitCheck(nullptr), Gray10, "None")) {
							cID.AddSelect();
							while (true) {
								if (cID.GetIsSelected() && CheckConflict(cID.GetChildPtr())) {
									cID.AddSelect();
									continue;
								}
								break;
							}
						}
						if (cID.GetIsSelected()) {
							cID.GetChildPtr()->Draw(P_Next.GetPosX(), P_Next.GetPosY(), xsize, ysize, 0, Gray10, false);
							DrawChild(xp, yp, cID.GetChildPtr(), Scale);
						}
						break;
					}
				}
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_SelectBuffer = InvalidID;
			m_BaseWeapon = nullptr;

			m_EnableMag = true;
			m_EnableMount = false;
			m_EnableSight = false;
			m_Recoil = 50;
			m_Ergonomics = 50;

			m_ItemRect.clear();
			m_ChildData.clear();

			m_XChild = 0.f;
			m_ItemIDs.clear();
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
			if (m_BaseWeapon) {
				//プリセットを適応
				if (m_ChildData.size() == 0) {
					for (const auto& L : PresetData::Instance()->GetList()) {
						if (m_BaseWeapon == L.GetBase()) {
							AttachPreset(L);
							break;
						}
					}
				}
				//設定
				CalcChild();
			}
		}

		void Draw_Back_Sub(int, int, float) noexcept override {
			if (m_BaseWeapon) {
				if (m_BaseWeapon->GetIcon().GetGraph()) {
					float Scale = (float)y_r(1920) / m_BaseWeapon->GetIcon().GetXSize();
					DrawControl::Instance()->SetDrawRotaGraph(m_BaseWeapon->GetIcon().GetGraph(), y_r(960), y_r(540), Scale, 0.f, false);
					DrawChild(0, 0, m_BaseWeapon, Scale);
					m_ItemRect.clear();
				}
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					if (m_ItemIDs.back().first!= InvalidID) {
						m_ItemIDs.back().first = InvalidID;
					}
					else {
						TurnOnGoNextBG();
					}
				}
			}
			//
			{
				int xgoal = 0;
				int xsize = y_r(400);
				int xs_add = -(xsize + y_r(50));
				int xp = y_r(1920 - 10) - xsize - (int)m_XChild;
				int yp = LineHeight + y_r(10);
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
						MakeList<ItemTypeList>(xp + xgoal, yp, ItemTypeData::Instance()->GetList(), &IDs->first, !IDs->second, false, true, [&](const auto *ptr) { return (ptr->GetCategoryID() == ItemCategoryData::Instance()->FindID("Weapons")); });
					});
				}
				//
				{
					Layer = 1;
					MakeLists(Layer, false, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemList>(xp + xgoal, yp, ItemData::Instance()->GetList(), &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (ptr->GetTypeID() == m_ItemIDs.at(Layer - 1).first); });
					});
				}
				//決定
				{
					auto prev = m_SelectBuffer;
					m_SelectBuffer = m_ItemIDs.back().first;
					if (m_SelectBuffer != prev) {
						m_BaseWeapon = (m_SelectBuffer != InvalidID) ? ItemData::Instance()->FindPtr(m_SelectBuffer) : nullptr;
					}
					if (m_BaseWeapon == nullptr) {
						m_ChildData.clear();
					}
				}
				//
				if (isChild) {
					xgoal -= xs_add / 2;
				}
				if (m_SelectBuffer != InvalidID) {
					xgoal += xs_add*2;
				}
				Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
			}
			//下から上に
			{
				int xp = LineHeight;
				int yp = y_r(1080) - LineHeight;
				//
				yp -= y_r(80);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "エルゴノミクス");
				WindowSystem::UpDownBar(xp, y_r(360), yp + LineHeight + y_r(5), &m_Ergonomics, 0, 100);
				//
				yp -= y_r(80);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "縦リコイル");
				WindowSystem::UpDownBar(xp, y_r(360), yp + LineHeight + y_r(5), &m_Recoil, 10, 200);
				//
				yp -= y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableSight);
				WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "サイトを含む");
				//
				yp -= y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableMount);
				WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "マウントを含む");
				//
				yp -= y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableMag);
				WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "マガジンを含む");
				//
				if (m_EnableSight) {
					m_EnableMount = true;
				}
				if (!m_EnableMount) {
					m_EnableSight = false;
				}
			}
			//
		}
		void Dispose_Sub(void) noexcept override {
			m_ItemRect.clear();
			m_ChildData.clear();
		}
	};
};
