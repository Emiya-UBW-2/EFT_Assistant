#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class BGParent {
	private:
		bool					m_GoNextBG{ false };
	protected:
		virtual void Init_Sub(int *, int *, float*) noexcept {}
		virtual void LateExecute_Sub(int *, int *, float*) noexcept {}
		virtual void Draw_Back_Sub(int, int, float) noexcept {}
		virtual void DrawFront_Sub(int, int, float) noexcept {}
		virtual void Dispose_Sub(void) noexcept {}
	public:
		void TurnOnGoNextBG() noexcept { this->m_GoNextBG = true; }
		const auto& IsGoNextBG() const noexcept { return this->m_GoNextBG; }
	public:
		void Init(int *posx, int *posy, float* Scale) noexcept {
			*posx = y_r(50);
			*posy = LineHeight + y_r(50);
			*Scale = 0.8f;
			m_GoNextBG = false;
			Init_Sub(posx, posy, Scale);
		}
		void LateExecute(int *posx, int *posy, float* Scale) noexcept {
			LateExecute_Sub(posx, posy, Scale);
		}
		void Draw_Back(int posx, int posy, float Scale) noexcept {
			Draw_Back_Sub(posx, posy, Scale);
		}
		void DrawFront(int posx, int posy, float Scale) noexcept {
			DrawFront_Sub(posx, posy, Scale);
		}
		void Dispose(void) noexcept {
			Dispose_Sub();
		}
	protected:
		int										m_ListXPos{ 0 };
		int										m_ListYPos{ 0 };
		int										m_ListXSize{ 0 };
		std::vector<std::pair<int, bool>>		m_ItemIDs;
		float									m_XChild{ 0.f };
		template<class ListChild>
		void MakeList(int xp1, int yp1, const std::vector<ListChild>& List, std::string_view Name, int*Select, bool isActive, bool isElseSelect, bool isAllSelect, const std::function<bool(const ListChild*)>& CheckLocal) noexcept {
			xp1 -= (int)m_XChild;
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			int xsize = (m_ListXSize != 0) ? m_ListXSize : y_r(400);
			int ysize = LineHeight - y_r(3);
			int count = 0;
			//
			{
				int yp_t = yp1;
				if (isAllSelect) {
					yp_t += ysize + y_r(3);
				}
				for (const auto& L2 : List) {
					if (!CheckLocal(&L2)) { continue; }
					yp_t += ysize + y_r(3);
					count++;
				}
				if (yp_t > y_r(1080 - 24)) {
					ysize = (y_r(1080 - 24) - yp1) / (count) - y_r(3);
				}
			}

			WindowSystem::SetMsg(xp1, yp1, xp1 + xsize, yp1 + ysize - y_r(5), ysize - y_r(5), STRX_MID, White, Black, Name);
			yp1 += ysize;
			int yp_t = yp1;
			if (isAllSelect) {
				yp_t += ysize + y_r(3);
			}
			//
			int IDBuf = InvalidID;
			bool NotSelect = (*Select == InvalidID);
			for (const auto& L2 : List) {
				if (!CheckLocal(&L2)) { continue; }
				IDBuf = L2.GetID();
				bool SelectIt = (*Select == IDBuf);
				auto color = NotSelect ? Gray25 : (SelectIt ? Gray10 : Gray50);
				if (WindowSystem::ClickCheckBox(xp1 - (SelectIt ? y_r(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, false, (isActive || (!isActive && SelectIt)) && !WindowMngr->PosHitCheck(nullptr), color, L2.GetShortName().c_str())) {
					*Select = (isActive) ? IDBuf : InvalidID;
				}
				yp_t += ysize + y_r(3);
			}
			if (count > 0 && isElseSelect) {//‚»‚Ì‘¼
				bool ElseSelect = (*Select == ElseSelectID);
				auto color = ElseSelect ? Gray25 : Gray50;
				if (WindowSystem::ClickCheckBox(xp1 - (ElseSelect ? y_r(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, false, isActive && !WindowMngr->PosHitCheck(nullptr), color, "Else")) {
					*Select = ElseSelectID;
				}
			}
			//‘S•”‘I‘ð
			if (isAllSelect) {
				if (count > 1) {
					auto color = NotSelect ? Gray10 : Gray50;
					if (WindowSystem::ClickCheckBox(xp1 - (NotSelect ? y_r(25) : 0), yp1, xp1 + xsize, yp1 + ysize, false, isActive && !WindowMngr->PosHitCheck(nullptr), color, "ALL")) {
						*Select = InvalidID;
					}
				}
				else {
					if (IDBuf != InvalidID) {
						*Select = IDBuf;
					}
				}
			}
			if (count == 0) {
				if (WindowSystem::ClickCheckBox(xp1, yp1, xp1 + xsize, yp1 + ysize, false, false, Gray50, "None")) {}
			}
		};
	protected:
		const auto& ListsSel(int Sel) const noexcept { return this->m_ItemIDs.at(Sel).first; }

		auto BackLists() noexcept {
			for (auto it = this->m_ItemIDs.rbegin(); it != this->m_ItemIDs.rend(); ++it) {
				if (it->first != InvalidID) {
					it->first = InvalidID;
					return true;
				}
			}
			return false;
		}

		bool MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*, bool)>& ListSet) noexcept {
			auto& NowSel = this->m_ItemIDs.at(Layer);
			NowSel.second = ((ListsSel(Layer) != InvalidID) && AndNext);
			if (Layer == 0 || (Layer >= 1 && this->m_ItemIDs.at(Layer - 1).second)) {
				ListSet(&NowSel, (Layer >= 1));
				return (Layer >= 1);
			}
			else {
				NowSel.first = InvalidID;
			}
			return false;
		}

		void InitLists(int listtotal, int xpos, int ypos, int xsize) noexcept {
			m_XChild = 0.f;
			//
			m_ListXPos = xpos;
			m_ListYPos = ypos;
			m_ListXSize = xsize;
			m_ItemIDs.clear();
			for (int i = 0; i < listtotal; i++) {
				m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			}
		}
		void ExecuteLists(bool isChild, int xgoal) noexcept {
			int xs_add = m_ListXSize + y_r(50);
			if (isChild) {
				xgoal += xs_add / 2;
			}
			Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
		}


	};

};
