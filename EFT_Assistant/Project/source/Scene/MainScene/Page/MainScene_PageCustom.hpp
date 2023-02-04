#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class CustomBG :public BGParent {
	private:
		ItemID	m_SelectBuffer{ InvalidID };
		ItemList* m_BaseWeapon{ nullptr };
		float xpos_add{ 0.f };

		bool m_EnableMag = true;
		bool m_EnableMount = false;
		bool m_EnableSight = false;
		int m_Recoil = 50;
		int m_Ergonomics = 50;
		float m_RecoilPer = 0.f;
		float m_ErgonomicsPer = 0.f;

		std::vector<Rect2D>								m_ItemRect;
	private:
		class ChildData {
		public:
			const ItemList*	Ptr{ nullptr };
			int				Slot{ 0 };
			int				ChildSel{ 0 };
			int				watchCounter{ 0 };
		};
		std::vector<ChildData> m_ChildData;
		//
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
					if ((cID.Ptr == Ptr_Buf) && (cID.Slot == Index)) {
						if (cID.ChildSel != (int)(c.Data.size())) {
							AttachPreset(Preset, c.Data.at(cID.ChildSel).first);
						}
						IsHit = true;
						break;
					}
				}
				if (!IsHit) {
					//設定ベクターに追加
					auto ChildSel = (int)(c.Data.size());
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
					if (ChildSel != (int)(c.Data.size())) {
						m_ChildData.resize(m_ChildData.size() + 1);
						auto& cID = m_ChildData.back();
						cID.Ptr = Ptr_Buf;
						cID.Slot = Index;
						cID.ChildSel = ChildSel;
						AttachPreset(Preset, c.Data.at(cID.ChildSel).first);
					}
				}
			}
			if (Ptr == nullptr) {
				CalcChild(m_BaseWeapon, true);
				//プリセット外のものを省く
			}
		}
		//
		bool CheckConflict(const ItemList* ParentPtr, const ItemList* MyPtr) {
			for (const auto& cP : ParentPtr->GetConflictParts()) {
				if (cP.first == MyPtr) {
					return true;
				}
			}
			for (const auto& c : ParentPtr->GetChildParts()) {
				auto Index = (int)(&c - &ParentPtr->GetChildParts().front());
				for (const auto& cID : m_ChildData) {
					if ((cID.watchCounter > 0) && (cID.Ptr == ParentPtr) && (cID.Slot == Index)) {
						if (cID.ChildSel < c.Data.size()) {
							if (CheckConflict(c.Data.at(cID.ChildSel).first, MyPtr)) {
								return true;
							}
						}
						break;
					}
				}
			}
			return false;
		}
		void CalcChild(const ItemList* Ptr, bool IsMakeNone = false) {
			for (const auto& c : Ptr->GetChildParts()) {
				auto Index = (int)(&c - &Ptr->GetChildParts().front());
				bool IsHit = false;
				for (auto& cID : m_ChildData) {
					if ((cID.Ptr == Ptr) && (cID.Slot == Index)) {
						cID.watchCounter++;
						if (cID.ChildSel < c.Data.size()) {
							CalcChild(c.Data.at(cID.ChildSel).first, IsMakeNone);
						}
						IsHit = true;
						break;
					}
				}
				if (!IsHit) {
					//設定ベクターに追加
					m_ChildData.resize(m_ChildData.size() + 1);
					auto& cID = m_ChildData.back();
					cID.Ptr = Ptr;
					cID.Slot = Index;
					cID.watchCounter = 1;
					if (IsMakeNone) {
						cID.ChildSel = (int)(c.Data.size());
					}
					cID.ChildSel = (int)(c.Data.size());
				}
			}
		}
		//
		void DrawChild(int xpbase, int ypbase, const ItemList* Ptr, float Scale) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();

			for (const auto& c : Ptr->GetChildParts()) {
				auto Index = (int)(&c - &Ptr->GetChildParts().front());
				for (auto& cID : m_ChildData) {
					if ((cID.Ptr == Ptr) && (cID.Slot == Index)) {
						{
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
								++cID.ChildSel %= (c.Data.size() + 1);
								while (true) {
									if (cID.ChildSel < c.Data.size()) {
										if (CheckConflict(m_BaseWeapon, c.Data.at(cID.ChildSel).first)) {
											++cID.ChildSel %= (c.Data.size() + 1);
											continue;
										}
									}
									break;
								}
							}
							if (cID.ChildSel < c.Data.size()) {
								const auto& cD = c.Data.at(cID.ChildSel);
								cD.first->Draw(P_Next.GetPosX(), P_Next.GetPosY(), xsize, ysize, 0, Gray10, false);
								DrawChild(xp, yp, cD.first, Scale);
							}
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
			xpos_add = 0.f;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
			if (m_BaseWeapon) {
				if (m_BaseWeapon->GetIcon().GetGraph()) {
					if (m_ChildData.size() == 0) {
						for (const auto& L : PresetData::Instance()->GetList()) {
							if (m_BaseWeapon == L.GetBase()) {
								//プリセットを適応
								AttachPreset(L);
								break;
							}
						}
					}
					//
					for (auto& cID : m_ChildData) {
						cID.watchCounter = 0;
					}
					CalcChild(m_BaseWeapon);
					//未使用品を除外
					{
						for (int count = 0; count < m_ChildData.size(); count++) {
							const auto& cID = m_ChildData[count];
							if (cID.watchCounter == 0) {
								m_ChildData.erase(m_ChildData.begin() + count);
								count--;
							}
						}
					}
					//フィルター
					{
						int MountID = ItemTypeData::Instance()->FindID("Mount");
						int MagID = ItemTypeData::Instance()->FindID("Magazine");
						int RefSightID = ItemTypeData::Instance()->FindID("Reflex sight");
						int CompRefSightID = ItemTypeData::Instance()->FindID("Compact reflex sight");
						int AsScopeID = ItemTypeData::Instance()->FindID("Assault scope");
						int ScopeID = ItemTypeData::Instance()->FindID("Scope");
						int SpScopeID = ItemTypeData::Instance()->FindID("Special scope");
						int IromSightID = ItemTypeData::Instance()->FindID("Iron sight");

						for (int count = 0; count < m_ChildData.size(); count++) {
							auto& cID = m_ChildData[count];
							auto& c = cID.Ptr->GetChildParts().at(cID.Slot);
							//
							{
								bool IsMatch = true;
								for (int count2 = 0; count2 < c.Data.size(); count2++) {
									auto* ptr = c.Data.at(count2).first;
									bool IsHit = false;
									if (!m_EnableMag) {
										if (
											ptr->GetTypeID() == MagID
											) {
											IsHit = true;
										}
									}
									if (!m_EnableMount) {
										if (
											ptr->GetTypeID() == MountID
											) {
											IsHit = true;
										}
									}
									if (!m_EnableSight) {
										if (
											ptr->GetTypeID() == RefSightID
											|| ptr->GetTypeID() == CompRefSightID
											|| ptr->GetTypeID() == AsScopeID
											|| ptr->GetTypeID() == ScopeID
											|| ptr->GetTypeID() == SpScopeID
											|| ptr->GetTypeID() == IromSightID
											) {
											IsHit = true;
										}
									}
									if (!IsHit) {
										IsMatch = false;
									}
								}
								if (IsMatch) {
									m_ChildData.erase(m_ChildData.begin() + count);
									count--;
								}
							}
							//
							while (true) {
								if (cID.ChildSel < c.Data.size()) {
									auto* ptr = c.Data.at(cID.ChildSel).first;
									bool IsHit = false;
									if (!m_EnableMag) {
										if (
											ptr->GetTypeID() == MagID
											) {
											IsHit = true;
										}
									}
									if (!m_EnableMount) {
										if (
											ptr->GetTypeID() == MountID
											) {
											IsHit = true;
										}
									}
									if (!m_EnableSight) {
										if (
											ptr->GetTypeID() == RefSightID
											|| ptr->GetTypeID() == CompRefSightID
											|| ptr->GetTypeID() == AsScopeID
											|| ptr->GetTypeID() == ScopeID
											|| ptr->GetTypeID() == SpScopeID
											|| ptr->GetTypeID() == IromSightID
											) {
											IsHit = true;
										}
									}
									if (IsHit) {
										++cID.ChildSel %= (c.Data.size() + 1);
									}
									else {
										break;
									}
								}
								else {
									break;
								}
							}
						}
					}
					//性能更新
					m_RecoilPer = 0.f;
					m_ErgonomicsPer = 0.f;
					for (const auto& cID : m_ChildData) {
						auto& c = cID.Ptr->GetChildParts().at(cID.Slot);
						if (cID.ChildSel < c.Data.size()) {
							auto* ChildPtr = c.Data.at(cID.ChildSel).first;
							m_RecoilPer += ChildPtr->GetRecoil();
							m_ErgonomicsPer += ChildPtr->GetErgonomics();
						}
					}
					m_Recoil = (int)(m_BaseWeapon->GetRecoil()*(100.f + m_RecoilPer) / 100.f);
					m_Ergonomics = (int)(m_BaseWeapon->GetErgonomics() + m_ErgonomicsPer);
				}
			}
			else {
				m_ChildData.clear();
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
					TurnOnGoNextBG();
				}
			}
			//
			auto prev = m_SelectBuffer;
			if (xpos_add <= y_r(500)) {
				int CatID = ItemTypeData::Instance()->FindID("Weapon");
				int xp = y_r(1920 - 400 - 10) + (int)xpos_add;
				int yp = LineHeight + y_r(10);
				MakeList<ItemList>(xp, yp, ItemData::Instance()->GetList(), (int*)&m_SelectBuffer, true, false, false, [&](const auto *ptr) { return (ptr->GetTypeID() == CatID); });
			}
			if (m_SelectBuffer != prev) {
				if (m_SelectBuffer != InvalidID) {
					m_BaseWeapon = ItemData::Instance()->FindPtr(m_SelectBuffer);
				}
				else {
					m_BaseWeapon = nullptr;
				}
			}
			Easing(&xpos_add, (m_SelectBuffer != InvalidID) ? (float)(y_r(1000)) : 0.f, 0.9f, EasingType::OutExpo);
			//
			{
				int xp = y_r(50);
				int yp = y_r(1080) - LineHeight - y_r(50) - y_r(80) - y_r(80) - y_r(50) - y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableMag);
				xp += LineHeight * 3;
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "マガジンを含む");
			}
			//
			{
				int xp = y_r(50);
				int yp = y_r(1080) - LineHeight - y_r(50) - y_r(80) - y_r(80) - y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableMount);
				xp += LineHeight * 3;
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "マウントを含む");
				if (!m_EnableMount) {
					m_EnableSight = false;
				}
			}
			//
			{
				int xp = y_r(50);
				int yp = y_r(1080) - LineHeight - y_r(50) - y_r(80) - y_r(80);
				WindowSystem::CheckBox(xp, yp, &m_EnableSight);
				xp += LineHeight * 3;
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "サイトを含む");
				if (m_EnableSight) {
					m_EnableMount = true;
				}
			}
			//
			{
				int xp = y_r(50);
				int yp = y_r(1080) - LineHeight - y_r(50) - y_r(80);
				WindowSystem::UpDownBar(xp, y_r(360), yp, &m_Recoil, 10, 200);
				yp -= LineHeight + y_r(5);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "縦リコイル");
			}
			//
			{
				int xp = y_r(50);
				int yp = y_r(1080) - LineHeight - y_r(50);
				WindowSystem::UpDownBar(xp, y_r(360), yp, &m_Ergonomics, 0, 100);
				yp -= LineHeight + y_r(5);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "エルゴノミクス");
			}
			//
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};