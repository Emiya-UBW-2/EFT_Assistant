#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	class BGParent {
	private:
		bool					m_GoNextBG{ false };
	protected:
		virtual void Init_Sub(int *, int *, float*) noexcept {}
		virtual void LateExecute_Sub(void) noexcept {}
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
		void LateExecute(void) noexcept {
			LateExecute_Sub();
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
};