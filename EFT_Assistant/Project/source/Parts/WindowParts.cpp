#include "WindowParts.hpp"
#include "../PartsHeader.hpp"

const FPS_n2::WindowSystem::WindowManager* SingletonBase<FPS_n2::WindowSystem::WindowManager>::m_Singleton = nullptr;
const FPS_n2::DataErrorLog* SingletonBase<FPS_n2::DataErrorLog>::m_Singleton = nullptr;
namespace FPS_n2 {
	namespace WindowSystem {
		//箱
		extern void SetBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet) {
			DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp1, yp1, xp2, yp2, colorSet, true);
		};
		//
		extern bool CloseButton(int xp1, int yp1) {
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
			WindowSystem::SetMsg(xp3, yp3, xp4, yp4, LineHeight - EdgeSize * 2 - y_r(6), STRX_MID, White, Black, "X");
			return ans;
		}
		//
		extern void CheckBox(int xp1, int yp1, bool* switchturn) {
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
		//
		extern void	UpDownBar(int xmin, int xmax, int yp, int* value, int valueMin, int valueMax) {
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
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_MID, White, Black, "%03d", *value);
			xp = xmin;
			if (WindowSystem::ClickCheckBox(xp, yp, xp + LineHeight, yp + LineHeight, true, true, Gray25, "▽")) {
				*value = std::max(*value - 1, valueMin);
			}
		}
		//
		void		ScrollBoxClass::ScrollBox(int xp1, int yp1, int xp2, int yp2, float TotalPer, bool IsActive) {
			auto* Input = InputControl::Instance();
			unsigned int color = Gray25;

			int length = (int)((float)(yp2 - yp1) / TotalPer);
			float Total = (float)(yp2 - yp1 - length);
			int Yp_t = (int)(Total * this->m_NowScrollYPer);
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
						m_NowScrollYPer = std::clamp((float)(Input->GetMouseY() - this->m_BaseScrollY) / Total, 0.f, 1.f);

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
		//
		const bool	WindowControl::GetIsEditing(void) const noexcept {
			auto* Input = InputControl::Instance();
			int xp1 = this->m_PosX;
			int yp1 = this->m_PosY;
			int xp2 = this->m_PosX + this->m_SizeX;
			int yp2 = this->m_PosY + this->m_SizeY;
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
		void WindowControl::Set(int posx, int posy, int sizex, int sizey, int Totalsizey, const char* tabName, bool canChageSize, bool canPressXButton, signed long long FreeID, const std::function<void(WindowControl*)>& DoingOnWindow) noexcept {

			this->m_FreeID = FreeID;

			this->m_PosX = posx;
			this->m_PosY = posy;

			this->m_SizeX = sizex;
			SetSizeY(sizey);
			this->m_TotalSizeY = Totalsizey;
			this->m_TabName = tabName;

			int widthLimit = SetMsg(0, 0, 0, 0 + LineHeight, LineHeight - y_r(6), STRX_LEFT, White, Black, this->m_TabName) + EdgeSize * 2 + LineHeight * 2;
			this->m_SizeX = std::max(this->m_SizeX, widthLimit);

			this->m_CanChageSize = canChageSize;
			this->m_CanPressXButton = canPressXButton;
			this->m_DoingOnWindow = DoingOnWindow;
		}
		void WindowControl::Draw(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* Input = InputControl::Instance();
			int xp1 = this->m_PosX;
			int yp1 = this->m_PosY;
			int xp2 = this->m_PosX + this->m_SizeX;
			int yp2 = this->m_PosY + this->m_SizeY;
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
					m_Scroll.ScrollBox(xp2 - y_r(24), yp1 + LineHeight, xp2, yp2, Total, this->m_IsActive);
				}
			}
			//タブ
			{
				int xp3 = this->m_PosX + this->m_SizeX - LineHeight - LineHeight;
				unsigned int color = Gray25;
				SetBox(xp1, yp1, xp2, yp1 + LineHeight, color);
				widthLimit = SetMsg(xp1, yp1, xp3, yp1 + LineHeight, LineHeight - y_r(6), STRX_LEFT, White, Black, this->m_TabName) + EdgeSize * 2 + LineHeight * 2;
			}
			//最大化ボタン
			if (m_CanChageSize) {
				int xp3 = this->m_PosX + this->m_SizeX - LineHeight - LineHeight;
				int yp3 = this->m_PosY + EdgeSize;
				int xp4 = this->m_PosX + this->m_SizeX - LineHeight - EdgeSize;
				int yp4 = this->m_PosY + LineHeight - EdgeSize;
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
				SetMsg(xp3, yp3, xp4, yp4, LineHeight - EdgeSize * 2 - y_r(6), STRX_MID, White, Black, !this->m_isMaxSize ? "□" : "ﾛ");
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
		//
		void		SearchBox::Draw(int xp1, int yp1) noexcept {
			auto* Input = InputControl::Instance();
			int xp2 = xp1 + y_r(500);
			int yp2 = yp1 + LineHeight + y_r(6);
			m_Search = in2_(Input->GetMouseX(), Input->GetMouseY(), xp1, yp1, xp2, yp2);

			if (m_Search) {
				for (char az = 'a'; az <= 'z'; az++) {
					if (Input->GetKey(az).trigger()) {
						m_SearchWord += az;
					}
				}
				for (char az = 0; az <= 9; az++) {
					if (Input->GetNumKey((int)az).trigger()) {
						m_SearchWord += ('0' + az);
					}
				}
				if ((Input->GetBackSpaceKey().trigger() || Input->GetBackSpaceKey().trigger()) && m_SearchWord != "") {
					m_SearchWord.pop_back();
				}
			}

			WindowSystem::SetBox(xp1, yp1, xp2, yp2, m_Search ? Gray15 : Gray25);
			WindowSystem::SetMsg(xp1, yp1, xp2, yp2, LineHeight, STRX_LEFT, White, Black, (m_SearchWord != "") ? (m_SearchWord + (m_Search ? "|" : "")).c_str() : "マウスをかざして入力…");
			if (WindowSystem::CloseButton(xp2 - LineHeight - 1, yp1 + 1)) {
				m_SearchWord = "";
			}
		}
		const bool	SearchBox::GetIsHit(std::string Name) const noexcept {
			if (m_SearchWord != "") {
				SubStrs(&Name, ",");
				SubStrs(&Name, ".");
				SubStrs(&Name, "-");
				SubStrs(&Name, " ");
				SubStrs(&Name, "?");
				SubStrs(&Name, "’");
				SubStrs(&Name, "．");
				std::transform(Name.begin(), Name.end(), Name.begin(), [](unsigned char c) { return (char)(std::tolower(c)); });
				return (Name.find(m_SearchWord) != std::string::npos);
			}
			return true;//何も入力されていないので一応true
		}
	};

	void DataErrorLog::Draw() noexcept {
		int xp = 0;
		int yp = LineHeight;
		for (auto& m : this->m_Mes) {
			WindowSystem::SetMsg(xp, yp, xp, yp, LineHeight * 7 / 10, STRX_LEFT, GetColor(255, 150, 150), Black, m);
			yp += LineHeight * 7 / 10;
			if (yp > y_r(1080)) { break; }
		}
	}
	void DataErrorLog::AddLog(const char* Mes) noexcept {
		m_Mes.emplace_back(Mes);
	}
	void DataErrorLog::Save() noexcept {
		std::ofstream outputfile("data/ErrorLog.txt");
		for (auto& LD : this->m_Mes) {
			outputfile << LD + "\n";
		}
		outputfile.close();
	}
};
