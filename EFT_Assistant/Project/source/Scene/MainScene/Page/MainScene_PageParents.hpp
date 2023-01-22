#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class BGParent {
	private:
		bool					m_GoNextBG{ false };
	protected:
		virtual void Init_Sub(int *, int *, float*) noexcept {}
		virtual void LateExecute_Sub(int *, int *, float*) noexcept {}
		virtual void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept {}
		virtual void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept {}
		virtual void Dispose_Sub(void) noexcept {}
		void TurnOnGoNextBG() noexcept { m_GoNextBG = true; }
	public:
		const auto& IsGoNextBG() const noexcept { return m_GoNextBG; }
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
		void Draw_Back(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float Scale) noexcept {
			Draw_Back_Sub(Windowup, posx, posy, Scale);
		}
		void DrawFront(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float Scale) noexcept {
			DrawFront_Sub(Windowup, posx, posy, Scale);
		}
		void Dispose(void) noexcept {
			Dispose_Sub();
		}
	};

	template<class ListChild>
	static void MakeList(int xp1, int yp1, const std::vector<ListChild>& List, int*Select, bool isActive, bool isElseSelect,bool isAllSelect, const std::function<bool(const ListChild*)>& CheckLocal) noexcept {
		int xsize = y_r(450);
		int ysize = LineHeight;
		int count = 0;
		int yp_t = yp1;
		if (isAllSelect) {
			yp_t += ysize + y_r(5);
		}
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
		if (isAllSelect) {
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
		}
		if (count == 0) {
			if (WindowSystem::ClickCheckBox(xp1, yp1, xp1 + xsize, yp1 + ysize, false, false, Gray50, "None")) {}
		}
	};
};