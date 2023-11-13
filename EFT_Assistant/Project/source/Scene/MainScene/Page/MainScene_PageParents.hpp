#pragma once
#include "../../../Header.hpp"
#include "../../../PartsHeader.hpp"
#include "../Data/MainScene_Parents.hpp"

namespace FPS_n2 {
	class ListControl {
	protected:
		int										m_ListXPos{ 0 };
		int										m_ListYPos{ 0 };
		int										m_ListXSize{ 0 };
		std::vector<std::pair<int, bool>>		m_ItemIDs;
		float									m_XChild{ 0.f };
	protected:
		template<class TaskList>
		void DrawLists(int xp1, DataParent<TaskList>* List, std::string_view Name, int*Select, bool isActive, bool isElseSelect, bool isAllSelect, const std::function<bool(const TaskList*)>& CheckLocal) noexcept {
			int xsize = (m_ListXSize != 0) ? m_ListXSize : y_r(400);
			List->DrawList(m_ListXPos - (int)m_XChild + xp1, m_ListYPos, xsize, Name, Select, isActive, isElseSelect, isAllSelect, CheckLocal);
		};

		bool MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*, bool)>& ListSet) noexcept;

		const auto& ListsSel(int Sel) const noexcept { return this->m_ItemIDs.at(Sel).first; }
		bool BackLists() noexcept;

		void SetList(int listSel , int sel) noexcept {
			m_ItemIDs.at(listSel).first = sel;
			m_ItemIDs.at(listSel).second = true;
		}

		void InitLists(int listtotal, int xpos, int ypos, int xsize) noexcept;
		void ExecuteLists(bool isChild, int xgoal) noexcept;
	};

	class PageParent :public ListControl {
	protected:
		std::function<void()>					m_ReturnButtonPress;
	private:
		void DrawReturnButton() noexcept {
			int xp = y_r(10);
			int yp = LineHeight + y_r(10);
			if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "–ß‚é")) {
				if (m_ReturnButtonPress) {
					m_ReturnButtonPress();
				}
			}
		}
	protected:
		virtual void Init_Sub(int *, int *, float*) noexcept {}
		virtual void LateExecute_Sub(int *, int *, float*) noexcept {}
		virtual void Draw_Back_Sub(int, int, float) noexcept {}
		virtual void DrawFront_Sub(int, int, float) noexcept {}
		virtual void Dispose_Sub(void) noexcept {}
	public:
		void Init(int *posx, int *posy, float* Scale) noexcept;
		void LateExecute(int *posx, int *posy, float* Scale) noexcept;
		void Draw_Back(int posx, int posy, float Scale) noexcept;
		void DrawFront(int posx, int posy, float Scale) noexcept;
		void Dispose(void) noexcept;
	};
};
