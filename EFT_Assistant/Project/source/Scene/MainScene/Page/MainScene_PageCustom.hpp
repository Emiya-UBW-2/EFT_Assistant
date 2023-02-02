#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class CustomBG :public BGParent {
	private:
		ItemID	m_SelectBuffer{ InvalidID };
		ItemList* m_BaseWeapon{ nullptr };
		float xpos_add{ 0.f };

		bool m_EnableMount = false;
		bool m_EnableMag = false;
		bool m_EnableSight = false;
		int m_Recoil = 50;
		int m_Ergonomics = 50;

		std::vector<Rect2D>								m_ItemRect;
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_SelectBuffer = InvalidID;
			m_BaseWeapon = nullptr;
			xpos_add = 0.f;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}

		std::vector<int> DrawChild(int xpbase, int ypbase, const ItemList* Ptr, float Scale) noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			std::vector<int>	childID;
			childID.resize(Ptr->GetChildParts().size());

			int MountID = ItemTypeData::Instance()->FindID("Mount");

			int MagID = ItemTypeData::Instance()->FindID("Magazine");
			int RefSightID = ItemTypeData::Instance()->FindID("Reflex sight");
			int CompRefSightID = ItemTypeData::Instance()->FindID("Compact reflex sight");
			int AsScopeID = ItemTypeData::Instance()->FindID("Assault scope");
			int ScopeID = ItemTypeData::Instance()->FindID("Scope");
			int SpScopeID = ItemTypeData::Instance()->FindID("Special scope");
			int IromSightID = ItemTypeData::Instance()->FindID("Iron sight");


			for (const auto& c : Ptr->GetChildParts()) {
				const auto& cD = c.Data[childID.at(&c - &Ptr->GetChildParts().front())];

				if (!m_EnableMount) {
					if (
						cD.first->GetTypeID() == MountID
						) {
						continue;
					}
				}
				if (!m_EnableMag) {
					if (
						cD.first->GetTypeID() == MagID
						) {
						continue;
					}
				}
				if (!m_EnableSight) {
					if (
						cD.first->GetTypeID() == RefSightID
						|| cD.first->GetTypeID() == CompRefSightID
						|| cD.first->GetTypeID() == AsScopeID
						|| cD.first->GetTypeID() == ScopeID
						|| cD.first->GetTypeID() == SpScopeID
						|| cD.first->GetTypeID() == IromSightID
						) {
						continue;
					}
				}


				int xp = xpbase + (int)(Scale * c.xpos);
				int yp = ypbase + (int)(Scale * c.ypos);
				float len = std::hypotf((float)xp, (float)yp);
				int xp2 = xp + (int)((float)xp * (float)y_r(384) / len);
				int yp2 = yp + (int)((float)yp * (float)y_r(384) / len);

				Rect2D P_Next;

				int xsize = y_r(300);
				int ysize = y_r(80);
				int xbase = (xp2 > 0) ? (y_r(960) + xp2) : (y_r(960) + xp2 - xsize);
				int ybase = y_r(540) + yp2 - ysize / 2;

				if (xbase < 0) {
					xp2 += (-xbase);
					xbase += (-xbase);
					//端にあるのではけさせる
					if (ybase + ysize / 2 > y_r(540)) {
						yp2 += ysize + y_r(5);
					}
					else {
						yp2 -= ysize + y_r(5);
					}
					ybase = y_r(540) + yp2 - ysize / 2;
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
				cD.first->Draw(P_Next.GetPosX(), P_Next.GetPosY(), xsize, ysize, 0, Gray10, !WindowMngr->PosHitCheck(nullptr));

				cD.first->GetRecoil();
				cD.first->GetErgonomics();


				DrawChild(xp, yp, cD.first, Scale);
			}
			return childID;
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