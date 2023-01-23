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
	private:
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
		void Init_Sub(int *, int *, float*) noexcept override {
			m_XChild = 0.f;
			m_ItemIDs.clear();
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}
		void Draw_Back_Sub(int, int, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* DrawParts = DXDraw::Instance();

			int xpos = y_r(40);
			int ypos = LineHeight + y_r(10) + LineHeight;
			int ysize = (int)((float)y_r(80));

			int xs = 400;
			int ScrPosX = y_r(1920 - xs * 3 / 2 - 10) - y_r(80);

			int yp0 = ypos - (int)m_YNow;
			for (auto& L : ItemData::Instance()->GetList()) {//todo
				if (m_ItemIDs[1].first == InvalidID) {
					bool isHit = false;
					for (auto& TL : ItemTypeData::Instance()->GetList()) {
						if (TL.GetCategoryID() == m_ItemIDs[0].first || m_ItemIDs[0].first == InvalidID) {
							isHit = (L.GetTypeID() == TL.GetID());
							if (isHit) { break; }
						}
					}
					if (!isHit) { continue; }
				}
				if (L.GetTypeID() == m_ItemIDs[1].first || m_ItemIDs[1].first == InvalidID) {
					bool ishit = false;
					for (auto& m : L.GetMapID()) {
						if (m == m_ItemIDs[2].first) {
							ishit = true;
							break;
						}
					}
					if (m_ItemIDs[2].first == ElseSelectID) {
						ishit = (L.GetMapID().size() == 0);
					}
					if (ishit || m_ItemIDs[2].first == InvalidID) {
						L.Draw(xpos, yp0, ScrPosX - xpos, ysize, 0, Gray75, !WindowMngr->PosHitCheck(nullptr));
						yp0 += ysize;
					}
				}
			}
			yp0 -= ypos - (int)m_YNow;


			int ScrSizY = (DrawParts->m_DispYSize - (y_r(10) + LineHeight)) - ypos;
			m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp0, ScrSizY) / (float)ScrSizY, true);

			m_YNow = std::max(0.f, m_Scroll.GetNowScrollYPer()*(float)(yp0 - ScrSizY));
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
						MakeList<ItemCategoryList>(xp + xgoal, yp, ItemCategoryData::Instance()->GetList(), &IDs->first, !IDs->second, false, true, [&](const auto *) { return true; });
					});
				}
				//
				{
					Layer = 1;
					bool CanGoNext = ((m_ItemIDs.at(Layer).first != InvalidID) && (ItemTypeData::Instance()->FindPtr(m_ItemIDs.at(Layer).first)->GetName() == "Key"));
					MakeLists(Layer, CanGoNext, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemTypeList>(xp + xgoal, yp, ItemTypeData::Instance()->GetList(), &IDs->first, !IDs->second, false, true, [&](const auto *ptr) { return (ptr->GetCategoryID() == m_ItemIDs.at(Layer - 1).first); });
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
						MakeList<MapList>(xp + xgoal, yp, MapData::Instance()->GetList(), &IDs->first, !IDs->second, true, true, [&](const auto *) { return true; });
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
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "–ß‚é")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};