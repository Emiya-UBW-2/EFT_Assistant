#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	class ItemBG :public BGParent {
	private:
		std::vector<std::pair<int, bool>>	m_ItemIDs;
		ItemTypeID						m_ItemTypeSel{ InvalidID };
		MapID							m_MapTypeSel{ InvalidID };
		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
		float							m_XChild{ 0.f };

		//bool							m_NotUseInRaid{ false };
	private:
		bool MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*)>& ListSet) {
			auto& NowSel = m_ItemIDs.at(Layer);
			NowSel.second = ((NowSel.first != InvalidID) && AndNext);
			if (Layer == 0 || (Layer >= 1 && m_ItemIDs.at(Layer - 1).second)) {
				ListSet(&NowSel);
			}
			else {
				NowSel.first = InvalidID;
			}
		}
		template<class ListChild>
		void MakeList(int xp1, int yp1, const std::vector<ListChild>& List, int*Select, bool isActive, bool isElseSelect, const std::function<bool(const ListChild*)>& CheckLocal) {
			int xsize = y_r(400);
			int ysize = LineHeight;
			int count = 0;
			int yp_t = yp1;
			yp_t += ysize + y_r(5);
			//
			int IDBuf = InvalidID;
			bool NotSelect = (*Select == InvalidID);
			for (const auto& L2 : List) {
				if (!CheckLocal(&L2)) { continue; }
				IDBuf = L2.GetID();
				bool SelectIt = (*Select == IDBuf);
				auto color = NotSelect ? Gray25 : (SelectIt ? Gray10 : Gray50);
				if (WindowSystem::ClickCheckBox(xp1 - (SelectIt ? y_r(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, false, isActive || (!isActive && SelectIt), color, L2.GetName().c_str())) {
					*Select = (isActive) ? IDBuf : InvalidID;
				}
				yp_t += ysize + y_r(5);
				count++;
			}
			if (count > 0 && isElseSelect) {//‚»‚Ì‘¼
				bool ElseSelect = (*Select == ElseSelectID);
				auto color = ElseSelect ? Gray25 : Gray50;
				if (WindowSystem::ClickCheckBox(xp1 - (ElseSelect ? y_r(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, false, isActive, color, "Else")) {
					*Select = ElseSelectID;
				}
			}
			//‘S•”‘I‘ð
			if (count > 1) {
				auto color = NotSelect ? Gray10 : Gray50;
				if (WindowSystem::ClickCheckBox(xp1 - (NotSelect ? y_r(25) : 0), yp1, xp1 + xsize, yp1 + ysize, false, isActive, color, "ALL")) {
					*Select = InvalidID;
				}
			}
			else {
				if (IDBuf != InvalidID) {
					*Select = IDBuf;
				}
			}
			if (count == 0) {
				if (WindowSystem::ClickCheckBox(xp1, yp1, xp1 + xsize, yp1 + ysize, false, false, Gray50, "None")) {}
			}
		};
	private:
		void Init_Sub(int *, int *, float*) noexcept override {
			m_XChild = 0.f;
			m_ItemIDs.clear();
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
		}
		void LateExecute_Sub(void) noexcept override {
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			auto* DrawParts = DXDraw::Instance();

			int xpos = y_r(40);
			int ypos = LineHeight + y_r(10) + LineHeight;
			int ysize = (int)((float)y_r(80));

			int yp = ypos - (int)m_YNow;
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
						L.Draw(xpos, yp, ysize, 0);
						yp += ysize;
					}
				}
			}
			yp -= ypos - (int)m_YNow;

			int xs = 400;
			int ScrPosX = y_r(1920 - xs * 3 / 2 - 10) - y_r(80);

			int ScrSizY = (DrawParts->m_DispYSize - (y_r(10) + LineHeight)) - ypos;
			m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp, ScrSizY) / (float)ScrSizY, true);

			m_YNow = std::max(0.f, m_Scroll.GetNowScrollYPer()*(float)(yp - ScrSizY));
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
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
						MakeList<ItemCategoryList>(xp + xgoal, yp, ItemCategoryData::Instance()->GetList(), &IDs->first, !IDs->second, false, [&](const auto *) { return true; });
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
						MakeList<ItemTypeList>(xp + xgoal, yp, ItemTypeData::Instance()->GetList(), &IDs->first, !IDs->second, false, [&](const auto *ptr) { return (ptr->GetCategoryID() == m_ItemIDs.at(Layer - 1).first); });
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
						MakeList<MapList>(xp + xgoal, yp, MapData::Instance()->GetList(), &IDs->first, !IDs->second, true, [&](const auto *) { return true; });
					});
				}
				//
				if (isChild) {
					xgoal -= xs_add / 2;
				}
				Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
			}
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