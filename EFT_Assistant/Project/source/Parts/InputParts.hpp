#pragma once
#include "../Header.hpp"

namespace FPS_n2 {

	class InputControl : public SingletonBase<InputControl> {
	private:
		friend class SingletonBase<InputControl>;
	private:
		int						m_mouse_x{ 0 }, m_mouse_y{ 0 };

		switchs					m_LeftClick;
		float					m_LeftPressTimer{ 0.f };

		switchs					m_MiddleClick;

		switchs					m_RightClick;

		switchs					m_ShiftKey;
		switchs					m_SpaceKey;
		switchs					m_CtrlKey;
		switchs					m_BackSpaceKey;
		std::array<switchs, 26>	m_AtoZKey;
		std::array<switchs, 10>	m_NumKey;

		int						m_WheelAdd{ 0 };
		bool					m_ScaleActive{ true };
	private:
		InputControl(void) noexcept {
			m_LeftClick.Set(false);
			m_MiddleClick.Set(false);
			m_RightClick.Set(false);
			m_ShiftKey.Set(false);
			m_SpaceKey.Set(false);
			m_CtrlKey.Set(false);
			m_BackSpaceKey.Set(false);
			for (auto& k : this->m_AtoZKey) {
				k.Set(false);
			}
			for (auto& k : this->m_NumKey) {
				k.Set(false);
			}
		}
		~InputControl(void) noexcept { }
	public:
		const auto&		GetMouseX(void) const noexcept { return this->m_mouse_x; }
		const auto&		GetMouseY(void) const noexcept { return this->m_mouse_y; }
		const auto&		GetLeftClick(void) const noexcept { return this->m_LeftClick; }
		const auto&		GetLeftPressTimer(void) const noexcept { return this->m_LeftPressTimer; }
		const auto&		GetMiddleClick(void) const noexcept { return this->m_MiddleClick; }
		const auto&		GetRightClick(void) const noexcept { return this->m_RightClick; }
		const auto&		GetShiftKey(void) const noexcept { return this->m_ShiftKey; }
		const auto&		GetSpaceKey(void) const noexcept { return this->m_SpaceKey; }
		const auto&		GetCtrlKey(void) const noexcept { return this->m_CtrlKey; }
		const auto&		GetBackSpaceKey(void) const noexcept { return this->m_BackSpaceKey; }
		const auto&		GetKey(char AtoZ) const noexcept {
			if ('A' <= AtoZ && AtoZ <= 'Z') {
				return this->m_AtoZKey.at(AtoZ - 'A');
			}
			if ('a' <= AtoZ && AtoZ <= 'z') {
				return this->m_AtoZKey.at(AtoZ - 'a');
			}
			return this->m_AtoZKey.at(0);
		}
		const auto&		GetNumKey(int Num) const noexcept {
			if (0 <= Num && Num <= 9) {
				return this->m_NumKey.at(Num);
			}
			return this->m_NumKey.at(0);
		}
		const auto&		GetWheelAdd(void) const noexcept { return this->m_WheelAdd; }
		const auto&		GetScaleActive(void) const noexcept { return this->m_ScaleActive; }
		void			SetScaleActive(bool value) noexcept { this->m_ScaleActive = value; }
		void	SetMouse(void) noexcept {
			GetMousePoint(&this->m_mouse_x, &this->m_mouse_y);
		}
	public:
		void	Execute(void) noexcept {
			SetMouse();
			this->m_LeftClick.Execute((GetMouseInputWithCheck() & MOUSE_INPUT_LEFT) != 0);
			if (this->m_LeftClick.press()) {
				this->m_LeftPressTimer += 1.f / FPS;
			}
			else {
				this->m_LeftPressTimer = 0.f;
			}
			this->m_MiddleClick.Execute((GetMouseInputWithCheck() & MOUSE_INPUT_MIDDLE) != 0);
			this->m_RightClick.Execute((GetMouseInputWithCheck() & MOUSE_INPUT_RIGHT) != 0);
			this->m_ShiftKey.Execute(CheckHitKeyWithCheck(KEY_INPUT_LSHIFT) != 0);
			this->m_SpaceKey.Execute(CheckHitKeyWithCheck(KEY_INPUT_SPACE) != 0);
			this->m_CtrlKey.Execute(CheckHitKeyWithCheck(KEY_INPUT_LCONTROL) != 0);
			this->m_BackSpaceKey.Execute(CheckHitKeyWithCheck(KEY_INPUT_BACK) != 0);

			int KEYS[26] = {
				KEY_INPUT_A,
				KEY_INPUT_B,
				KEY_INPUT_C,
				KEY_INPUT_D,
				KEY_INPUT_E,
				KEY_INPUT_F,
				KEY_INPUT_G,
				KEY_INPUT_H,
				KEY_INPUT_I,
				KEY_INPUT_J,
				KEY_INPUT_K,
				KEY_INPUT_L,
				KEY_INPUT_M,
				KEY_INPUT_N,
				KEY_INPUT_O,
				KEY_INPUT_P,
				KEY_INPUT_Q,
				KEY_INPUT_R,
				KEY_INPUT_S,
				KEY_INPUT_T,
				KEY_INPUT_U,
				KEY_INPUT_V,
				KEY_INPUT_W,
				KEY_INPUT_X,
				KEY_INPUT_Y,
				KEY_INPUT_Z,
			};
			for (int loop = 0; loop < this->m_AtoZKey.size(); loop++) {
				m_AtoZKey[loop].Execute(CheckHitKeyWithCheck(KEYS[loop]) != 0);
			}
			int NUMKEYS[10] = {
				KEY_INPUT_0,
				KEY_INPUT_1,
				KEY_INPUT_2,
				KEY_INPUT_3,
				KEY_INPUT_4,
				KEY_INPUT_5,
				KEY_INPUT_6,
				KEY_INPUT_7,
				KEY_INPUT_8,
				KEY_INPUT_9,
			};
			for (int loop = 0; loop < this->m_NumKey.size(); loop++) {
				m_NumKey[loop].Execute(CheckHitKeyWithCheck(NUMKEYS[loop]) != 0);
			}
			m_WheelAdd = GetMouseWheelRotVolWithCheck();
		}
	};

};