#pragma once
#include"../Header.hpp"

#define EdgeSize	y_r(2)
#define LineHeight	y_r(36)

namespace FPS_n2 {
	//カラー指定
	static const unsigned int Red{ GetColor(255, 0, 0) };
	static const unsigned int Red25{ GetColor(192, 0, 0) };
	static const unsigned int Red50{ GetColor(128, 0, 0) };
	static const unsigned int RedPop{ GetColor(255, 50, 50) };
	static const unsigned int Blue{ GetColor(50, 50, 255) };
	static const unsigned int DarkGreen{ GetColor(21, 128, 45) };
	static const unsigned int Green{ GetColor(43, 255, 91) };
	static const unsigned int White{ GetColor(255, 255, 255) };
	static const unsigned int Gray10{ GetColor(230, 230, 230) };
	static const unsigned int Gray15{ GetColor(218, 218, 218) };
	static const unsigned int Gray25{ GetColor(192, 192, 192) };
	static const unsigned int Gray50{ GetColor(128, 128, 128) };
	static const unsigned int Gray75{ GetColor(64, 64, 64) };
	static const unsigned int Black{ GetColor(1, 1, 1) };
	static const unsigned int TransColor{ GetColor(0, 0, 0) };
	//
	namespace WindowSystem {
		//箱
		static void SetBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet) {
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1, yp1, xp2, yp2, colorSet, true);
		};
		//文字
		template <typename... Args>
		static const auto GetMsgLen(int size, std::string_view String, Args&&... args) {
			auto* Fonts = FontPool::Instance();
			return Fonts->Get(FontPool::FontType::Nomal_Edge, size).GetStringWidth(-1, ((std::string)String).c_str(), args...) + y_r(6) + 2;//エッジ分:
		}
		template <typename... Args>
		static const auto SetMsg(int xp1, int yp1, int xp2, int yp2, int size, FontHandle::FontXCenter FontX, unsigned int Color, unsigned int EdleColor, std::string_view String, Args&&... args) {
			if (String == "") { return 0; }
			auto* DrawParts = DXDraw::Instance();
			int xSize = GetMsgLen(size, String, args...);
			int xpos = 0;
			int ypos = yp1 + (yp2 - yp1) / 2;
			if ((ypos - size / 2) > DrawParts->m_DispYSize || (ypos + size / 2) < 0) { return 0; }				//画面外は表示しない
			switch (FontX) {
			case FontHandle::FontXCenter::LEFT:
				xpos = xp1 + y_r(6);
				if ((xpos) > DrawParts->m_DispXSize || (xpos + xSize) < 0) { return 0; }						//画面外は表示しない
				break;
			case FontHandle::FontXCenter::MIDDLE:
				xpos = xp1 + (xp2 - xp1) / 2;
				if ((xpos - xSize / 2) > DrawParts->m_DispXSize || (xpos + xSize / 2) < 0) { return 0; }		//画面外は表示しない
				break;
			case FontHandle::FontXCenter::RIGHT:
				xpos = xp2 - y_r(6);
				if ((xpos - xSize) > DrawParts->m_DispXSize || (xpos) < 0) { return 0; }						//画面外は表示しない
				break;
			default:
				break;
			}
			DrawControl::Instance()->SetString(DrawLayer::Normal, FontPool::FontType::Nomal_Edge, size, FontX, FontHandle::FontYCenter::MIDDLE, xpos, ypos, Color, EdleColor, ((std::string)String).c_str(), args...);
			return xSize;//エッジ分
		};
		//
		template <typename... Args>
		static bool ClickCheckBox(int xp1, int yp1, int xp2, int yp2, bool isrepeat, bool IsActive, unsigned int defaultcolor, std::string_view String, Args&&... args) {
			auto* Input = InputControl::Instance();
			unsigned int color = defaultcolor;
			bool isHit = false;
			if (IsActive && in2_(Input->GetMouseX(), Input->GetMouseY(), xp1, yp1, xp2, yp2)) {
				color = White;
				if (Input->GetLeftClick().press()) {
					if (Input->GetLeftClick().trigger()) {
						isHit = true;
					}
				}
				if (0 < Input->GetLeftPressTimer() && Input->GetLeftPressTimer() < 0.1f) {
					color = Black;
				}
				if (isrepeat && Input->GetLeftPressTimer() > 0.5f) {
					isHit = true;
					color = Gray50;
				}

				HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(hCursor);
			}
			SetBox(xp1, yp1, xp2, yp2, color);
			SetMsg(xp1, yp1, xp2, yp2, std::min(LineHeight, yp2 - yp1) - y_r(6), FontHandle::FontXCenter::MIDDLE, White, Black, String, args...);
			return isHit;
		};

		static bool CloseButton(int xp1, int yp1) {
			auto* Input = InputControl::Instance();
			int xp3 = xp1 + EdgeSize;
			int yp3 = yp1 + EdgeSize;
			int xp4 = xp1 + LineHeight - EdgeSize;
			int yp4 = yp1 + LineHeight - EdgeSize;
			unsigned int color = Red25;
			bool ans = false;
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize)) {
				color = Red;
				if (Input->GetLeftClick().trigger()) {
					ans = true;
				}
				HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(hCursor);
			}
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize, color, true);
			WindowSystem::SetMsg(xp3, yp3, xp4, yp4, LineHeight - EdgeSize * 2 - y_r(6), FontHandle::FontXCenter::MIDDLE, White, Black, "X");
			return ans;
		}

		static void CheckBox(int xp1, int yp1, bool* switchturn) {
			auto* Input = InputControl::Instance();
			int xp3 = xp1 + EdgeSize;
			int yp3 = yp1 + EdgeSize;
			int xp4 = xp1 + LineHeight * 2 - EdgeSize;
			int yp4 = yp1 + LineHeight - EdgeSize;
			unsigned int color = Gray25;
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize)) {
				color = White;
				if (Input->GetLeftClick().trigger()) {
					*switchturn ^= 1;
				}
				HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(hCursor);
			}
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp3 + y_r(5), yp3 + y_r(5), xp4 - y_r(5), yp4 - y_r(5), Black, true);
			xp4 = xp1 + LineHeight * (*switchturn ? 1 : 0) - EdgeSize;
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp3 + y_r(5), yp3 + y_r(5), xp4 + y_r(5), yp4 - y_r(5), Gray50, true);
			xp3 = xp1 + LineHeight * (*switchturn ? 1 : 0) + EdgeSize;
			xp4 = xp1 + LineHeight * (*switchturn ? 2 : 1) - EdgeSize;
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp3, yp3, xp4, yp4, color, true);
		}

		static void	UpDownBar(int xmin, int xmax, int yp, int* value, int valueMin, int valueMax) {
			int xp = 0;
			{
				int xpmin = xmin + LineHeight + 1;
				int xpmax = xmax - 1;
				WindowSystem::SetBox(xpmin, yp, xpmin + (xpmax - xpmin), yp + LineHeight, DarkGreen);
				WindowSystem::SetBox(xpmin, yp, xpmin + (xpmax - xpmin)*std::clamp(*value - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), yp + LineHeight, Green);
			}
			xp = xmax;
			if (WindowSystem::ClickCheckBox(xp, yp, xp + LineHeight, yp + LineHeight, true, true, Gray25, "△")) {
				*value = std::min(*value + 1, valueMax);
			}
			xp = (xmin + (xmax - xmin) / 2);
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::MIDDLE, White, Black, "%03d", *value);
			xp = xmin;
			if (WindowSystem::ClickCheckBox(xp, yp, xp + LineHeight, yp + LineHeight, true, true, Gray25, "▽")) {
				*value = std::max(*value - 1, valueMin);
			}
		}
		//
		class ScrollBoxClass {
			bool		m_IsChangeScrollY{ false };
			int			m_BaseScrollY{ 0 };
			float		m_NowScrollYPer{ 0.f };
		public:
			const auto&		GetNowScrollYPer(void) const noexcept { return this->m_NowScrollYPer; }
			void			ScrollBox(int xp1, int yp1, int xp2, int yp2, float TotalPer, bool IsActive) {
				auto* Input = InputControl::Instance();
				unsigned int color = Gray25;

				int length = (int)((float)(yp2 - yp1) / TotalPer);
				float Total = (float)(yp2 - yp1 - length);
				int Yp_t = (int)(Total * m_NowScrollYPer);
				int Yp_s = std::max(yp1, yp1 + Yp_t);
				int Yp_e = std::min(yp2, Yp_s + length);

				if (IsActive) {
					if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp1, yp1, xp2, yp2)) {
						if (Input->GetWheelAdd() != 0.f) {
							m_NowScrollYPer = std::clamp(m_NowScrollYPer + (float)(-Input->GetWheelAdd() * 3) / Total, 0.f, 1.f);
						}
					}
					if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp2 - y_r(24), yp1, xp2, yp2)) {
						if (Input->GetLeftClick().trigger()) {
							m_IsChangeScrollY = true;
						}

						if (!m_IsChangeScrollY) {
							HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
							SetCursor(hCursor);
						}
					}
					if (m_IsChangeScrollY) {
						if (Input->GetLeftClick().press()) {
							color = White;
							m_NowScrollYPer = std::clamp((float)(Input->GetMouseY() - m_BaseScrollY) / Total, 0.f, 1.f);

							HCURSOR hCursor = LoadCursor(NULL, IDC_SIZENS);
							SetCursor(hCursor);
						}
						else {
							m_IsChangeScrollY = false;
						}
					}
					else {
						m_BaseScrollY = Input->GetMouseY() - Yp_t;
						if (Input->GetMouseY() < Yp_s) {
							m_BaseScrollY += Yp_s - Input->GetMouseY();
						}

						if (Input->GetMouseY() > Yp_e) {
							m_BaseScrollY += Yp_e - Input->GetMouseY();
						}
					}
				}
				SetBox(xp2 - y_r(24), yp1, xp2, yp2, Gray50);
				SetBox(xp2 - y_r(24) + y_r(1), Yp_s, xp2 - y_r(1), Yp_e, color);
			};
		};
		class WindowControl {
		public:
			bool				m_isDelete{ false };
			signed long long	m_FreeID{ 0 };
		private:
			bool				m_ActiveSwitch{ false };
			bool				m_IsActive{ false };
			int					m_PosX{ 0 }, m_PosY{ 0 };
			int					m_SizeX{ 100 }, m_SizeY{ 100 };
			int					m_TotalSizeY{ 200 };
			std::string			m_TabName;
			std::function<void(WindowControl*)> m_DoingOnWindow;

			ScrollBoxClass		m_Scroll;
		private:
			bool				m_isMaxSize{ false };
			int					m_PosXSave{ 0 }, m_PosYSave{ 0 };
			int					m_SizeXSave{ 100 }, m_SizeYSave{ 100 };

			bool				m_CanChageSize{ false };
			bool				m_CanPressXButton{ false };

			bool				m_IsMoving{ false };
			int					m_PosAddX{ 0 }, m_PosAddY{ 0 };

			bool				m_IsChangeScaleXY{ false };
			bool				m_IsChangeScale1X{ false };
			bool				m_IsChangeScale1Y{ false };
			bool				m_IsChangeScale2X{ false };
			bool				m_IsChangeScale2Y{ false };
			int					m_BaseScaleX{ 0 }, m_BaseScaleY{ 0 };
			int					m_BasePos1X{ 0 }, m_BasePos1Y{ 0 };
			int					m_BaseScale1X{ 0 }, m_BaseScale1Y{ 0 };
			int					m_BaseScale2X{ 0 }, m_BaseScale2Y{ 0 };
		public:
			const auto		GetIsEditing(void) const noexcept {
				auto* Input = InputControl::Instance();
				int xp1 = m_PosX;
				int yp1 = m_PosY;
				int xp2 = m_PosX + m_SizeX;
				int yp2 = m_PosY + m_SizeY;
				return
					in2_(Input->GetMouseX(), Input->GetMouseY(), xp1, yp1, xp2, yp2) ||
					this->m_CanChageSize ||
					this->m_IsMoving ||
					this->m_IsChangeScaleXY ||
					this->m_IsChangeScale1X ||
					this->m_IsChangeScale1Y ||
					this->m_IsChangeScale2X ||
					this->m_IsChangeScale2Y;
			}

			const auto&		GetPosX(void) const noexcept { return this->m_PosX; }
			const auto&		GetPosY(void) const noexcept { return this->m_PosY; }
			const auto&		GetSizeX(void) const noexcept { return this->m_SizeX; }
			const auto&		GetSizeY(void) const noexcept { return this->m_SizeY; }
			void			SetSizeY(int value) noexcept { this->m_SizeY = LineHeight + value; }

			void			SetActiveSwitch(bool value) noexcept { this->m_ActiveSwitch = value; }
			const auto&		GetActiveSwitch(void) const noexcept { return this->m_ActiveSwitch; }
			void			SetIsActive(bool value) noexcept { this->m_IsActive = value; }
			const auto&		GetIsActive(void) const noexcept { return this->m_IsActive; }

			void			SetTotalSizeY(bool value) noexcept { this->m_TotalSizeY = value; }
			const auto&		GetNowScrollPer(void) const noexcept { return this->m_Scroll.GetNowScrollYPer(); }
		public:
			void Set(int posx, int posy, int sizex, int sizey, int Totalsizey, const char* tabName, bool canChageSize, bool canPressXButton, signed long long FreeID, const std::function<void(WindowControl*)>& DoingOnWindow) noexcept {

				this->m_FreeID = FreeID;

				this->m_PosX = posx;
				this->m_PosY = posy;

				this->m_SizeX = sizex;
				SetSizeY(sizey);
				this->m_TotalSizeY = Totalsizey;
				this->m_TabName = tabName;

				int widthLimit = SetMsg(0, 0, 0, 0 + LineHeight, LineHeight - y_r(6), FontHandle::FontXCenter::LEFT, White, Black, m_TabName) + EdgeSize * 2 + LineHeight * 2;
				this->m_SizeX = std::max(this->m_SizeX, widthLimit);

				this->m_CanChageSize = canChageSize;
				this->m_CanPressXButton = canPressXButton;
				this->m_DoingOnWindow = DoingOnWindow;
			}
			void Draw(void) noexcept {
				auto* DrawParts = DXDraw::Instance();
				auto* Input = InputControl::Instance();
				int xp1 = m_PosX;
				int yp1 = m_PosY;
				int xp2 = m_PosX + m_SizeX;
				int yp2 = m_PosY + m_SizeY;
				int widthLimit = 0;
				//背景
				{
					int add = y_r(5);
					DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1 + add, yp1 + add, xp2 + add, yp2 + add, Black, TRUE);
					SetBox(xp1, yp1, xp2, yp2, Gray10);
				}

				m_ActiveSwitch = false;
				if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp1, yp1, xp2, yp1 + LineHeight)) {
					if (Input->GetLeftClick().trigger()) {
						m_ActiveSwitch = true;
					}
				}
				//内容
				m_DoingOnWindow(this);
				//スクロールバー
				{
					float Total = (float)this->m_TotalSizeY / (m_SizeY - LineHeight);
					if (Total > 1.f) {
						m_Scroll.ScrollBox(xp2 - y_r(24), yp1 + LineHeight, xp2, yp2, Total, m_IsActive);
					}
				}
				//タブ
				{
					int xp3 = m_PosX + m_SizeX - LineHeight - LineHeight;
					unsigned int color = Gray25;
					SetBox(xp1, yp1, xp2, yp1 + LineHeight, color);
					widthLimit = SetMsg(xp1, yp1, xp3, yp1 + LineHeight, LineHeight - y_r(6), FontHandle::FontXCenter::LEFT, White, Black, m_TabName) + EdgeSize * 2 + LineHeight * 2;
				}
				//最大化ボタン
				if (m_CanChageSize) {
					int xp3 = m_PosX + m_SizeX - LineHeight - LineHeight;
					int yp3 = m_PosY + EdgeSize;
					int xp4 = m_PosX + m_SizeX - LineHeight - EdgeSize;
					int yp4 = m_PosY + LineHeight - EdgeSize;
					unsigned int color = Gray25;

					if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize)) {
						color = White;
						if (Input->GetLeftClick().trigger()) {
							this->m_isMaxSize ^= 1;
							if (this->m_isMaxSize) {
								this->m_PosXSave = this->m_PosX;
								this->m_PosYSave = this->m_PosY;
								this->m_SizeXSave = this->m_SizeX;
								this->m_SizeYSave = this->m_SizeY;

								this->m_PosX = y_r(0);
								this->m_PosY = y_r(0);
								this->m_SizeX = DrawParts->m_DispXSize;
								this->m_SizeY = DrawParts->m_DispYSize;
							}
							else {
								this->m_PosX = this->m_PosXSave;
								this->m_PosY = this->m_PosYSave;
								this->m_SizeX = this->m_SizeXSave;
								this->m_SizeY = this->m_SizeYSave;
							}
						}
						HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
						SetCursor(hCursor);
					}
					DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize, color, TRUE);
					SetMsg(xp3, yp3, xp4, yp4, LineHeight - EdgeSize * 2 - y_r(6), FontHandle::FontXCenter::MIDDLE, White, Black, !this->m_isMaxSize ? "□" : "ﾛ");
				}
				//×ボタン
				if (this->m_CanPressXButton) {
					if (CloseButton(this->m_PosX + this->m_SizeX - LineHeight, this->m_PosY)) {
						this->m_isDelete = true;
					}
				}
				if (this->m_CanChageSize && !this->m_isMaxSize && this->m_IsActive) {
					//xyサイズ
					{
						unsigned int color = Gray25;
						auto radius = y_r(3);
						if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp2 - EdgeSize - radius, yp2 - EdgeSize - radius, xp2 - EdgeSize + radius, yp2 - EdgeSize + radius)) {
							color = White;
							if (Input->GetLeftClick().trigger()) {
								this->m_IsChangeScaleXY = true;
							}
						}
						if (this->m_IsChangeScaleXY) {
							if (Input->GetLeftClick().press()) {
								color = Gray50;
								this->m_SizeX = std::max((Input->GetMouseX() - this->m_BaseScaleX) - this->m_PosX, widthLimit);
								this->m_SizeY = std::max((Input->GetMouseY() - this->m_BaseScaleY) - this->m_PosY, LineHeight + y_r(10));
							}
							else {
								this->m_IsChangeScaleXY = false;
							}
						}
						else {
							this->m_BaseScaleX = Input->GetMouseX() - (this->m_PosX + this->m_SizeX);
							this->m_BaseScaleY = Input->GetMouseY() - (this->m_PosY + this->m_SizeY);
						}
						if (color != Gray25) {
							DrawControl::Instance()->SetDrawCircle(DrawLayer::Normal, xp2 - EdgeSize, yp2 - EdgeSize, radius, color);
						}
					}
					//yサイズ下
					{
						unsigned int color = Gray25;
						auto radius = y_r(3);
						if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp1 + EdgeSize - radius, yp2 - EdgeSize - radius, xp2 - EdgeSize - radius, yp2 - EdgeSize + radius)) {
							color = White;
							if (Input->GetLeftClick().trigger()) {
								this->m_IsChangeScale2Y = true;
							}
						}
						if (this->m_IsChangeScale2Y) {
							if (Input->GetLeftClick().press()) {
								color = Gray50;
								this->m_SizeY = std::max((Input->GetMouseY() - this->m_BaseScale2Y) - this->m_PosY, LineHeight + y_r(10));
							}
							else {
								this->m_IsChangeScale2Y = false;
							}
						}
						else {
							this->m_BaseScale2Y = Input->GetMouseY() - (this->m_PosY + this->m_SizeY);
						}
						if (color != Gray25) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1 + EdgeSize, yp2 - EdgeSize - radius, xp2 - EdgeSize, yp2 - EdgeSize + radius, color, TRUE);
						}
					}
					//xサイズ右
					{
						unsigned int color = Gray25;
						auto radius = y_r(3);
						if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp2 + EdgeSize - radius, yp1 - EdgeSize - radius, xp2 - EdgeSize + radius, yp2 - EdgeSize - radius)) {
							color = White;
							if (Input->GetLeftClick().trigger()) {
								this->m_IsChangeScale2X = true;
							}
						}
						if (this->m_IsChangeScale2X) {
							if (Input->GetLeftClick().press()) {
								color = Gray50;
								this->m_SizeX = std::max((Input->GetMouseX() - this->m_BaseScale2X) - this->m_PosX, widthLimit);
							}
							else {
								this->m_IsChangeScale2X = false;
							}
						}
						else {
							this->m_BaseScale2X = Input->GetMouseX() - (this->m_PosX + this->m_SizeX);
						}
						if (color != Gray25) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp2 - EdgeSize, yp1, xp2 + EdgeSize, yp2 - EdgeSize + radius, color, TRUE);
						}
					}
					//yサイズ上
					{
						unsigned int color = Gray25;
						auto radius = y_r(3);
						if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp1 + EdgeSize - radius, yp1 - radius * 2, xp2 - EdgeSize - radius, yp1)) {
							color = White;
							if (Input->GetLeftClick().trigger()) {
								this->m_IsChangeScale1Y = true;
							}
						}
						if (this->m_IsChangeScale1Y) {
							if (Input->GetLeftClick().press()) {
								color = Gray50;
								this->m_PosY = std::min((Input->GetMouseY() - this->m_BasePos1Y), this->m_BaseScale1Y - (LineHeight + y_r(10)));
								this->m_SizeY = this->m_BaseScale1Y - this->m_PosY;
							}
							else {
								this->m_IsChangeScale1Y = false;
							}
						}
						else {
							this->m_BasePos1Y = Input->GetMouseY() - this->m_PosY;
							this->m_BaseScale1Y = this->m_PosY + this->m_SizeY;
						}
						if (color != Gray25) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1 + EdgeSize, yp1 - radius * 2, xp2 - EdgeSize, yp1, color, TRUE);
						}
					}
					//xサイズ左
					{
						unsigned int color = Gray25;
						auto radius = y_r(3);
						if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp1 + EdgeSize - radius, yp1 - EdgeSize - radius, xp1 + EdgeSize + radius, yp2 - EdgeSize - radius)) {
							color = White;
							if (Input->GetLeftClick().trigger()) {
								this->m_IsChangeScale1X = true;
							}
						}
						if (this->m_IsChangeScale1X) {
							if (Input->GetLeftClick().press()) {
								color = Gray50;
								this->m_PosX = std::min((Input->GetMouseX() - this->m_BasePos1X), this->m_BaseScale1X - widthLimit);
								this->m_SizeX = this->m_BaseScale1X - this->m_PosX;
							}
							else {
								this->m_IsChangeScale1X = false;
							}
						}
						else {
							this->m_BasePos1X = Input->GetMouseX() - this->m_PosX;
							this->m_BaseScale1X = this->m_PosX + this->m_SizeX;
						}
						if (color != Gray25) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1 - EdgeSize, yp1, xp1 + EdgeSize, yp2 - EdgeSize + radius, color, TRUE);
						}
					}
				}
				//非アクティブ
				if (!this->m_IsActive) {
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 24);
					DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1, yp1, xp2, yp2, Black, TRUE);
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
				}

				//タブ演算
				{
					int xp3 = this->m_PosX + this->m_SizeX - LineHeight - LineHeight;
					unsigned int color = Gray25;

					if (this->m_IsActive && in2_(Input->GetMouseX(), Input->GetMouseY(), xp1 + EdgeSize, yp1 + EdgeSize, xp3 - EdgeSize, yp1 + LineHeight - EdgeSize)) {
						if (Input->GetLeftClick().trigger()) {
							this->m_IsMoving = true;
						}
						HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
						SetCursor(hCursor);
					}
					if (this->m_IsMoving) {
						if (Input->GetLeftClick().press()) {
							color = Gray50;
							this->m_PosX = Input->GetMouseX() - this->m_PosAddX;
							this->m_PosY = Input->GetMouseY() - this->m_PosAddY;


							HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
							SetCursor(hCursor);
						}
						else {
							if (this->m_CanChageSize) {
								if (y_r(10) < Input->GetMouseY() && Input->GetMouseY() < DrawParts->m_DispYSize - y_r(10)) {
									if (Input->GetMouseX() < y_r(10)) {
										this->m_PosX = y_r(0);
										this->m_PosY = y_r(0);
										this->m_SizeX = DrawParts->m_DispXSize / 2;
										this->m_SizeY = DrawParts->m_DispYSize;
									}
									if (Input->GetMouseX() > DrawParts->m_DispXSize - y_r(10)) {
										this->m_PosX = DrawParts->m_DispXSize / 2;
										this->m_PosY = y_r(0);
										this->m_SizeX = DrawParts->m_DispXSize / 2;
										this->m_SizeY = DrawParts->m_DispYSize;
									}
								}
								else {
									if (Input->GetMouseX() < y_r(10)) {
										this->m_PosX = y_r(0);
										this->m_PosY = (Input->GetMouseY() < DrawParts->m_DispYSize / 2) ? y_r(0) : DrawParts->m_DispYSize / 2;
										this->m_SizeX = DrawParts->m_DispXSize / 2;
										this->m_SizeY = DrawParts->m_DispYSize / 2;
									}
									if (Input->GetMouseX() > DrawParts->m_DispXSize - y_r(10)) {
										this->m_PosX = DrawParts->m_DispXSize / 2;
										this->m_PosY = (Input->GetMouseY() < DrawParts->m_DispYSize / 2) ? y_r(0) : DrawParts->m_DispYSize / 2;
										this->m_SizeX = DrawParts->m_DispXSize / 2;
										this->m_SizeY = DrawParts->m_DispYSize / 2;
									}
								}
							}
							this->m_IsMoving = false;
						}
					}
					else {
						this->m_PosAddX = Input->GetMouseX() - this->m_PosX;
						this->m_PosAddY = Input->GetMouseY() - this->m_PosY;
					}
				}

				if (this->m_CanChageSize && this->m_IsMoving && Input->GetLeftClick().press()) {
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 24);
					if (y_r(10) < Input->GetMouseY() && Input->GetMouseY() < DrawParts->m_DispYSize - y_r(10)) {
						if (Input->GetMouseX() < y_r(10)) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, 0, 0, DrawParts->m_DispXSize / 2, DrawParts->m_DispYSize, Black, TRUE);
						}
						if (Input->GetMouseX() > DrawParts->m_DispXSize - y_r(10)) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, DrawParts->m_DispXSize / 2, 0, DrawParts->m_DispXSize, DrawParts->m_DispYSize, Black, TRUE);
						}
					}
					else {
						if (Input->GetMouseX() < y_r(10)) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal,
								y_r(0),
								(Input->GetMouseY() < DrawParts->m_DispYSize / 2) ? y_r(0) : DrawParts->m_DispYSize / 2,
								DrawParts->m_DispXSize / 2,
								(Input->GetMouseY() < DrawParts->m_DispYSize / 2) ? DrawParts->m_DispYSize / 2 : DrawParts->m_DispYSize,
								Black, TRUE);
						}
						if (Input->GetMouseX() > DrawParts->m_DispXSize - y_r(10)) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal,
								DrawParts->m_DispXSize / 2,
								(Input->GetMouseY() < DrawParts->m_DispYSize / 2) ? y_r(0) : DrawParts->m_DispYSize / 2,
								DrawParts->m_DispXSize,
								(Input->GetMouseY() < DrawParts->m_DispYSize / 2) ? DrawParts->m_DispYSize / 2 : DrawParts->m_DispYSize,
								Black, TRUE);
						}
					}
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
				}
			};
		};
		class WindowManager : public SingletonBase<WindowManager> {
		private:
			friend class SingletonBase<WindowManager>;
		private:
			std::vector<std::shared_ptr<WindowControl>> m_WindowControl;
		private:
			WindowManager() noexcept {}
			~WindowManager() noexcept {}
		public:
			const auto	PosHitCheck(WindowSystem::WindowControl* window) const noexcept {
				auto res = std::find_if(m_WindowControl.begin(), m_WindowControl.end(), [&](const std::shared_ptr<WindowControl>& tgt) { return(tgt.get() == window); });
				bool isSel = (window == nullptr);
				for (auto& w : m_WindowControl) {
					if (isSel) {
						if (w->GetIsEditing()) {
							return true;
						}
					}
					if (!isSel && (w == *res)) {
						isSel = true;
					}
				}
				return false;
			}
			void		DeleteAll() noexcept {
				for (auto& w : m_WindowControl) {
					w->m_isDelete = true;
				}
			}
		public:
			const auto&	Get() const noexcept { return m_WindowControl; }
			const auto&	Add() noexcept {
				m_WindowControl.emplace_back(std::make_shared<WindowControl>());
				return m_WindowControl.back();
			}
			void		Execute(void) noexcept {
				//ウィンドウアクティブチェック
				{
					int Size = (int)m_WindowControl.size();
					if ((Size > 1) && !m_WindowControl.back()->GetActiveSwitch()) {
						for (int i = (int)(Size)-2; i >= 0; i--) {
							if (m_WindowControl[i]->GetActiveSwitch()) {
								m_WindowControl.emplace_back(m_WindowControl[i]);
								m_WindowControl.erase(m_WindowControl.begin() + i);
								break;
							}
						}
					}
				}
				{
					int Size = (int)m_WindowControl.size();
					if (Size > 0) {
						for (int i = 0; i < Size; i++) {
							m_WindowControl[i]->SetIsActive(false);
						}
						m_WindowControl.back()->SetIsActive(true);
					}
				}
				//削除チェック
				{
					int Size = (int)m_WindowControl.size();
					for (int i = 0; i < Size; i++) {
						if (m_WindowControl[i]->m_isDelete) {
							std::swap(m_WindowControl[i], m_WindowControl.back());
							m_WindowControl.pop_back();
							Size = (int)m_WindowControl.size();
							i--;
						}
					}
				}
				//10ウィンドウ以上はキル
				{
					int Size = (int)m_WindowControl.size();
					for (int i = 0; i < Size - 10; i++) {
						m_WindowControl.erase(m_WindowControl.begin());
					}
				}
			}
			void		Draw(void) noexcept {
				int Size = (int)m_WindowControl.size();
				for (int i = 0; i < Size; i++) {
					m_WindowControl[i]->Draw();
				}
			}
			void		Dispose(void) noexcept {
				m_WindowControl.clear();
			}
		};
	};
	//
	static const auto STRX_LEFT{ FontHandle::FontXCenter::LEFT };
	static const auto STRX_MID{ FontHandle::FontXCenter::MIDDLE };
	static const auto STRX_RIGHT{ FontHandle::FontXCenter::RIGHT };
	static const auto STRY_TOP{ FontHandle::FontYCenter::TOP };
	static const auto STRY_MIDDLE{ FontHandle::FontYCenter::MIDDLE };
	static const auto STRY_BOTTOM{ FontHandle::FontYCenter::BOTTOM };
	//
	class DataErrorLog : public SingletonBase<DataErrorLog> {
	private:
		friend class SingletonBase<DataErrorLog>;
	private:
		std::vector<std::string>	m_Mes;
	private:
		DataErrorLog() noexcept {
			setPrintColorDx(GetColor(255, 0, 0), GetColor(218, 218, 218));
		}
		~DataErrorLog() noexcept {}
	public:
		void Draw() noexcept {
			int xp = 0;
			int yp = LineHeight;
			for (auto& m : m_Mes) {
				WindowSystem::SetMsg(xp, yp, xp, yp, LineHeight * 7 / 10, STRX_LEFT, GetColor(255, 150, 150), GetColor(1, 1, 1), m);
				yp += LineHeight * 7 / 10;
				if (yp > y_r(1080)) { break; }
			}
		}
		void AddLog(const char* Mes) noexcept {
			m_Mes.emplace_back(Mes);
		}
		void Save() noexcept {
			std::ofstream outputfile("data/ErrorLog.txt");
			for (auto& LD : m_Mes) {
				outputfile << LD + "\n";
			}
			outputfile.close();
		}
	};
	//
};