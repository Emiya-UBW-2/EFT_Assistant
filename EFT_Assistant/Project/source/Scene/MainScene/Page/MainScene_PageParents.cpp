#include "MainScene_PageParents.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	bool ListControl::MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*, bool)>& ListSet) noexcept {
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
	bool ListControl::BackLists() noexcept {
		for (auto it = this->m_ItemIDs.rbegin(); it != this->m_ItemIDs.rend(); ++it) {
			if (it->first != InvalidID) {
				it->first = InvalidID;
				return true;
			}
		}
		return false;
	}

	void ListControl::InitLists(int listtotal, int xpos, int ypos, int xsize) noexcept {
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
	void ListControl::ExecuteLists(bool isChild, int xgoal) noexcept {
		int xs_add = m_ListXSize + y_r(50);
		if (isChild) {
			xgoal += xs_add / 2;
		}
		Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
	}

	void PageParent::Init(int *posx, int *posy, float* Scale) noexcept {
		*posx = y_r(50);
		*posy = LineHeight + y_r(50);
		*Scale = 0.8f;
		m_ReturnButtonPress = [&]() {
			auto* PageMngr = PageManager::Instance();
			PageMngr->TurnOnGoNextPage();
		};
		Init_Sub(posx, posy, Scale);
	}
	void PageParent::LateExecute(int *posx, int *posy, float* Scale) noexcept {
		LateExecute_Sub(posx, posy, Scale);
	}
	void PageParent::Draw_Back(int posx, int posy, float Scale) noexcept {
		Draw_Back_Sub(posx, posy, Scale);
	}
	void PageParent::DrawFront(int posx, int posy, float Scale) noexcept {
		DrawFront_Sub(posx, posy, Scale);
		DrawReturnButton();
	}
	void PageParent::Dispose(void) noexcept {
		Dispose_Sub();
	}
};
