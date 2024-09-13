#include "WindowParts.hpp"
#include "../PartsHeader.hpp"

const FPS_n2::WindowMySystem::WindowManager* SingletonBase<FPS_n2::WindowMySystem::WindowManager>::m_Singleton = nullptr;
const FPS_n2::DataErrorLog* SingletonBase<FPS_n2::DataErrorLog>::m_Singleton = nullptr;
namespace FPS_n2 {
	namespace WindowMySystem {
		//
		extern bool CloseButton(int xp1, int yp1) {
			auto* Pad = PadControl::Instance();
			int xp3 = xp1 + EdgeSize;
			int yp3 = yp1 + EdgeSize;
			int xp4 = xp1 + LineHeight - EdgeSize;
			int yp4 = yp1 + LineHeight - EdgeSize;
			unsigned int color = Red25;
			bool ans = false;
			if (IntoMouse(xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize)) {
				color = Red;
				if (Pad->GetMouseClick().trigger()) {
					ans = true;
				}
				HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
				SetCursor(hCursor);
			}
			WindowSystem::SetBox(xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize, color);
			WindowSystem::SetMsg((xp3 + xp4) / 2, (yp3 + yp4) / 2, LineHeight - EdgeSize * 2 - DXDraw::Instance()->GetUIY(6), STRX_MID, White, Black, "X");
			return ans;
		}
		//
		void		ScrollBoxClass::ScrollBox(int xp1, int yp1, int xp2, int yp2, float TotalPer, bool IsActive) {
			auto* Pad = PadControl::Instance();
			unsigned int color = Gray25;

			int length = (int)((float)(yp2 - yp1) / TotalPer);
			float Total = (float)(yp2 - yp1 - length);
			int Yp_t = (int)(Total * this->m_NowScrollYPer);
			int Yp_s = std::max(yp1, yp1 + Yp_t);
			int Yp_e = std::min(yp2, Yp_s + length);

			if (IsActive) {
				if (IntoMouse(xp1, yp1, xp2, yp2)) {
					if (Pad->GetMouseWheelRot() != 0.f) {
						m_NowScrollYPer = std::clamp(m_NowScrollYPer + (float)(-Pad->GetMouseWheelRot() * 3) / Total, 0.f, 1.f);
					}
				}
				if (IntoMouse(xp2 - DXDraw::Instance()->GetUIY(24), yp1, xp2, yp2)) {
					if (Pad->GetMouseClick().trigger()) {
						m_IsChangeScrollY = true;
					}

					if (!m_IsChangeScrollY) {
						HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
						SetCursor(hCursor);
					}
				}
				if (m_IsChangeScrollY) {
					if (Pad->GetMouseClick().press()) {
						color = White;
						m_NowScrollYPer = std::clamp((float)(Pad->GetMS_Y() - this->m_BaseScrollY) / Total, 0.f, 1.f);

						HCURSOR hCursor = LoadCursor(NULL, IDC_SIZENS);
						SetCursor(hCursor);
					}
					else {
						m_IsChangeScrollY = false;
					}
				}
				else {
					m_BaseScrollY = Pad->GetMS_Y() - Yp_t;
					if (Pad->GetMS_Y() < Yp_s) {
						m_BaseScrollY += Yp_s - Pad->GetMS_Y();
					}

					if (Pad->GetMS_Y() > Yp_e) {
						m_BaseScrollY += Yp_e - Pad->GetMS_Y();
					}
				}
			}
			WindowSystem::SetBox(xp2 - DXDraw::Instance()->GetUIY(24), yp1, xp2, yp2, Gray50);
			WindowSystem::SetBox(xp2 - DXDraw::Instance()->GetUIY(24) + DXDraw::Instance()->GetUIY(1), Yp_s, xp2 - DXDraw::Instance()->GetUIY(1), Yp_e, color);
		};
		//
		const bool	WindowControl::GetIsEditing(void) const noexcept {
			int xp1 = this->m_PosX;
			int yp1 = this->m_PosY;
			int xp2 = this->m_PosX + this->m_SizeX;
			int yp2 = this->m_PosY + this->m_SizeY;
			return
				IntoMouse(xp1, yp1, xp2, yp2) ||
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

			int widthLimit = WindowSystem::GetMsgLen(LineHeight - DXDraw::Instance()->GetUIY(6), this->m_TabName) + EdgeSize * 2 + LineHeight * 2;
			WindowSystem::SetMsg(0, 0 + LineHeight / 2, LineHeight - DXDraw::Instance()->GetUIY(6), STRX_LEFT, White, Black, this->m_TabName);
			this->m_SizeX = std::max(this->m_SizeX, widthLimit);

			this->m_CanChageSize = canChageSize;
			this->m_CanPressXButton = canPressXButton;
			this->m_DoingOnWindow = DoingOnWindow;
		}
		void WindowControl::Draw(void) noexcept {
			auto* DrawParts = DXDraw::Instance();
			auto* Pad = PadControl::Instance();
			int xp1 = this->m_PosX;
			int yp1 = this->m_PosY;
			int xp2 = this->m_PosX + this->m_SizeX;
			int yp2 = this->m_PosY + this->m_SizeY;
			int widthLimit = 0;
			//背景
			{
				int add = DXDraw::Instance()->GetUIY(5);
				WindowSystem::SetBox(xp1 + add, yp1 + add, xp2 + add, yp2 + add, Black);
				WindowSystem::SetBox(xp1, yp1, xp2, yp2, Gray10);
			}

			m_ActiveSwitch = false;
			if (IntoMouse(xp1, yp1, xp2, yp1 + LineHeight)) {
				if (Pad->GetMouseClick().trigger()) {
					m_ActiveSwitch = true;
				}
			}
			//内容
			m_DoingOnWindow(this);
			//スクロールバー
			{
				float Total = (float)this->m_TotalSizeY / (m_SizeY - LineHeight);
				if (Total > 1.f) {
					m_Scroll.ScrollBox(xp2 - DXDraw::Instance()->GetUIY(24), yp1 + LineHeight, xp2, yp2, Total, this->m_IsActive);
				}
			}
			//タブ
			{
				int xp3 = this->m_PosX + this->m_SizeX - LineHeight - LineHeight;
				unsigned int color = Gray25;
				WindowSystem::SetBox(xp1, yp1, xp2, yp1 + LineHeight, color);
				widthLimit = WindowSystem::GetMsgLen(LineHeight - DXDraw::Instance()->GetUIY(6), this->m_TabName) + EdgeSize * 2 + LineHeight * 2;
				WindowSystem::SetMsg((xp1 + xp3) / 2, yp1 + LineHeight / 2, LineHeight - DXDraw::Instance()->GetUIY(6), STRX_LEFT, White, Black, this->m_TabName);
			}
			//最大化ボタン
			if (m_CanChageSize) {
				int xp3 = this->m_PosX + this->m_SizeX - LineHeight - LineHeight;
				int yp3 = this->m_PosY + EdgeSize;
				int xp4 = this->m_PosX + this->m_SizeX - LineHeight - EdgeSize;
				int yp4 = this->m_PosY + LineHeight - EdgeSize;
				unsigned int color = Gray25;

				if (IntoMouse(xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize)) {
					color = White;
					if (Pad->GetMouseClick().trigger()) {
						this->m_isMaxSize ^= 1;
						if (this->m_isMaxSize) {
							this->m_PosXSave = this->m_PosX;
							this->m_PosYSave = this->m_PosY;
							this->m_SizeXSave = this->m_SizeX;
							this->m_SizeYSave = this->m_SizeY;

							this->m_PosX = DXDraw::Instance()->GetUIY(0);
							this->m_PosY = DXDraw::Instance()->GetUIY(0);
							this->m_SizeX = DrawParts->GetScreenX(1920);
							this->m_SizeY = DrawParts->GetScreenY(1080);
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
				WindowSystem::SetBox(xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize, color);
				WindowSystem::SetMsg((xp3 + xp4) / 2, (yp3 + yp4) / 2, LineHeight - EdgeSize * 2 - DXDraw::Instance()->GetUIY(6), STRX_MID, White, Black, !this->m_isMaxSize ? "□" : "ﾛ");
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
					auto radius = DXDraw::Instance()->GetUIY(3);
					if (IntoMouse(xp2 - EdgeSize - radius, yp2 - EdgeSize - radius, xp2 - EdgeSize + radius, yp2 - EdgeSize + radius)) {
						color = White;
						if (Pad->GetMouseClick().trigger()) {
							this->m_IsChangeScaleXY = true;
						}
					}
					if (this->m_IsChangeScaleXY) {
						if (Pad->GetMouseClick().press()) {
							color = Gray50;
							this->m_SizeX = std::max((Pad->GetMS_X() - this->m_BaseScaleX) - this->m_PosX, widthLimit);
							this->m_SizeY = std::max((Pad->GetMS_Y() - this->m_BaseScaleY) - this->m_PosY, LineHeight + DXDraw::Instance()->GetUIY(10));
						}
						else {
							this->m_IsChangeScaleXY = false;
						}
					}
					else {
						this->m_BaseScaleX = Pad->GetMS_X() - (this->m_PosX + this->m_SizeX);
						this->m_BaseScaleY = Pad->GetMS_Y() - (this->m_PosY + this->m_SizeY);
					}
					if (color != Gray25) {
						WindowSystem::DrawControl::Instance()->SetDrawCircle(WindowSystem::DrawLayer::Normal, xp2 - EdgeSize, yp2 - EdgeSize, radius, color);
					}
				}
				//yサイズ下
				{
					unsigned int color = Gray25;
					auto radius = DXDraw::Instance()->GetUIY(3);
					if (IntoMouse(xp1 + EdgeSize - radius, yp2 - EdgeSize - radius, xp2 - EdgeSize - radius, yp2 - EdgeSize + radius)) {
						color = White;
						if (Pad->GetMouseClick().trigger()) {
							this->m_IsChangeScale2Y = true;
						}
					}
					if (this->m_IsChangeScale2Y) {
						if (Pad->GetMouseClick().press()) {
							color = Gray50;
							this->m_SizeY = std::max((Pad->GetMS_Y() - this->m_BaseScale2Y) - this->m_PosY, LineHeight + DXDraw::Instance()->GetUIY(10));
						}
						else {
							this->m_IsChangeScale2Y = false;
						}
					}
					else {
						this->m_BaseScale2Y = Pad->GetMS_Y() - (this->m_PosY + this->m_SizeY);
					}
					if (color != Gray25) {
						WindowSystem::SetBox(xp1 + EdgeSize, yp2 - EdgeSize - radius, xp2 - EdgeSize, yp2 - EdgeSize + radius, color);
					}
				}
				//xサイズ右
				{
					unsigned int color = Gray25;
					auto radius = DXDraw::Instance()->GetUIY(3);
					if (IntoMouse(xp2 + EdgeSize - radius, yp1 - EdgeSize - radius, xp2 - EdgeSize + radius, yp2 - EdgeSize - radius)) {
						color = White;
						if (Pad->GetMouseClick().trigger()) {
							this->m_IsChangeScale2X = true;
						}
					}
					if (this->m_IsChangeScale2X) {
						if (Pad->GetMouseClick().press()) {
							color = Gray50;
							this->m_SizeX = std::max((Pad->GetMS_X() - this->m_BaseScale2X) - this->m_PosX, widthLimit);
						}
						else {
							this->m_IsChangeScale2X = false;
						}
					}
					else {
						this->m_BaseScale2X = Pad->GetMS_X() - (this->m_PosX + this->m_SizeX);
					}
					if (color != Gray25) {
						WindowSystem::SetBox(xp2 - EdgeSize, yp1, xp2 + EdgeSize, yp2 - EdgeSize + radius, color);
					}
				}
				//yサイズ上
				{
					unsigned int color = Gray25;
					auto radius = DXDraw::Instance()->GetUIY(3);
					if (IntoMouse(xp1 + EdgeSize - radius, yp1 - radius * 2, xp2 - EdgeSize - radius, yp1)) {
						color = White;
						if (Pad->GetMouseClick().trigger()) {
							this->m_IsChangeScale1Y = true;
						}
					}
					if (this->m_IsChangeScale1Y) {
						if (Pad->GetMouseClick().press()) {
							color = Gray50;
							this->m_PosY = std::min((Pad->GetMS_Y() - this->m_BasePos1Y), this->m_BaseScale1Y - (LineHeight + DXDraw::Instance()->GetUIY(10)));
							this->m_SizeY = this->m_BaseScale1Y - this->m_PosY;
						}
						else {
							this->m_IsChangeScale1Y = false;
						}
					}
					else {
						this->m_BasePos1Y = Pad->GetMS_Y() - this->m_PosY;
						this->m_BaseScale1Y = this->m_PosY + this->m_SizeY;
					}
					if (color != Gray25) {
						WindowSystem::SetBox(xp1 + EdgeSize, yp1 - radius * 2, xp2 - EdgeSize, yp1, color);
					}
				}
				//xサイズ左
				{
					unsigned int color = Gray25;
					auto radius = DXDraw::Instance()->GetUIY(3);
					if (IntoMouse(xp1 + EdgeSize - radius, yp1 - EdgeSize - radius, xp1 + EdgeSize + radius, yp2 - EdgeSize - radius)) {
						color = White;
						if (Pad->GetMouseClick().trigger()) {
							this->m_IsChangeScale1X = true;
						}
					}
					if (this->m_IsChangeScale1X) {
						if (Pad->GetMouseClick().press()) {
							color = Gray50;
							this->m_PosX = std::min((Pad->GetMS_X() - this->m_BasePos1X), this->m_BaseScale1X - widthLimit);
							this->m_SizeX = this->m_BaseScale1X - this->m_PosX;
						}
						else {
							this->m_IsChangeScale1X = false;
						}
					}
					else {
						this->m_BasePos1X = Pad->GetMS_X() - this->m_PosX;
						this->m_BaseScale1X = this->m_PosX + this->m_SizeX;
					}
					if (color != Gray25) {
						WindowSystem::SetBox(xp1 - EdgeSize, yp1, xp1 + EdgeSize, yp2 - EdgeSize + radius, color);
					}
				}
			}
			//非アクティブ
			if (!this->m_IsActive) {
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 24);
				WindowSystem::SetBox(xp1, yp1, xp2, yp2, Black);
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
			}

			//タブ演算
			{
				int xp3 = this->m_PosX + this->m_SizeX - LineHeight - LineHeight;
				unsigned int color = Gray25;

				if (this->m_IsActive && IntoMouse(xp1 + EdgeSize, yp1 + EdgeSize, xp3 - EdgeSize, yp1 + LineHeight - EdgeSize)) {
					if (Pad->GetMouseClick().trigger()) {
						this->m_IsMoving = true;
					}
					HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
					SetCursor(hCursor);
				}
				if (this->m_IsMoving) {
					if (Pad->GetMouseClick().press()) {
						color = Gray50;
						this->m_PosX = Pad->GetMS_X() - this->m_PosAddX;
						this->m_PosY = Pad->GetMS_Y() - this->m_PosAddY;


						HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
						SetCursor(hCursor);
					}
					else {
						if (this->m_CanChageSize) {
							if (DXDraw::Instance()->GetUIY(10) < Pad->GetMS_Y() && Pad->GetMS_Y() < DrawParts->GetScreenY(1080) - DXDraw::Instance()->GetUIY(10)) {
								if (Pad->GetMS_X() < DXDraw::Instance()->GetUIY(10)) {
									this->m_PosX = DXDraw::Instance()->GetUIY(0);
									this->m_PosY = DXDraw::Instance()->GetUIY(0);
									this->m_SizeX = DrawParts->GetScreenX(1920) / 2;
									this->m_SizeY = DrawParts->GetScreenY(1080);
								}
								if (Pad->GetMS_X() > DrawParts->GetScreenX(1920) - DXDraw::Instance()->GetUIY(10)) {
									this->m_PosX = DrawParts->GetScreenX(1920) / 2;
									this->m_PosY = DXDraw::Instance()->GetUIY(0);
									this->m_SizeX = DrawParts->GetScreenX(1920) / 2;
									this->m_SizeY = DrawParts->GetScreenY(1080);
								}
							}
							else {
								if (Pad->GetMS_X() < DXDraw::Instance()->GetUIY(10)) {
									this->m_PosX = DXDraw::Instance()->GetUIY(0);
									this->m_PosY = (Pad->GetMS_Y() < DrawParts->GetScreenY(1080) / 2) ? DXDraw::Instance()->GetUIY(0) : DrawParts->GetScreenY(1080) / 2;
									this->m_SizeX = DrawParts->GetScreenX(1920) / 2;
									this->m_SizeY = DrawParts->GetScreenY(1080) / 2;
								}
								if (Pad->GetMS_X() > DrawParts->GetScreenX(1920) - DXDraw::Instance()->GetUIY(10)) {
									this->m_PosX = DrawParts->GetScreenX(1920) / 2;
									this->m_PosY = (Pad->GetMS_Y() < DrawParts->GetScreenY(1080) / 2) ? DXDraw::Instance()->GetUIY(0) : DrawParts->GetScreenY(1080) / 2;
									this->m_SizeX = DrawParts->GetScreenX(1920) / 2;
									this->m_SizeY = DrawParts->GetScreenY(1080) / 2;
								}
							}
						}
						this->m_IsMoving = false;
					}
				}
				else {
					this->m_PosAddX = Pad->GetMS_X() - this->m_PosX;
					this->m_PosAddY = Pad->GetMS_Y() - this->m_PosY;
				}
			}

			if (this->m_CanChageSize && this->m_IsMoving && Pad->GetMouseClick().press()) {
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 24);
				if (DXDraw::Instance()->GetUIY(10) < Pad->GetMS_Y() && Pad->GetMS_Y() < DrawParts->GetScreenY(1080) - DXDraw::Instance()->GetUIY(10)) {
					if (Pad->GetMS_X() < DXDraw::Instance()->GetUIY(10)) {
						WindowSystem::SetBox(0, 0, DrawParts->GetScreenX(1920) / 2, DrawParts->GetScreenY(1080), Black);
					}
					if (Pad->GetMS_X() > DrawParts->GetScreenX(1920) - DXDraw::Instance()->GetUIY(10)) {
						WindowSystem::SetBox(DrawParts->GetScreenX(1920) / 2, 0, DrawParts->GetScreenX(1920), DrawParts->GetScreenY(1080), Black);
					}
				}
				else {
					if (Pad->GetMS_X() < DXDraw::Instance()->GetUIY(10)) {
						WindowSystem::SetBox(
							DXDraw::Instance()->GetUIY(0),
							(Pad->GetMS_Y() < DrawParts->GetScreenY(1080) / 2) ? DXDraw::Instance()->GetUIY(0) : DrawParts->GetScreenY(1080) / 2,
							DrawParts->GetScreenX(1920) / 2,
							(Pad->GetMS_Y() < DrawParts->GetScreenY(1080) / 2) ? DrawParts->GetScreenY(1080) / 2 : DrawParts->GetScreenY(1080),
							Black);
					}
					if (Pad->GetMS_X() > DrawParts->GetScreenX(1920) - DXDraw::Instance()->GetUIY(10)) {
						WindowSystem::SetBox(DrawParts->GetScreenX(1920) / 2,
							(Pad->GetMS_Y() < DrawParts->GetScreenY(1080) / 2) ? DXDraw::Instance()->GetUIY(0) : DrawParts->GetScreenY(1080) / 2,
							DrawParts->GetScreenX(1920),
							(Pad->GetMS_Y() < DrawParts->GetScreenY(1080) / 2) ? DrawParts->GetScreenY(1080) / 2 : DrawParts->GetScreenY(1080),
							Black);
					}
				}
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
			}
		};
		//
		void		SearchBox::Draw(int xp1, int yp1) noexcept {
			auto* Pad = PadControl::Instance();
			int xp2 = xp1 + DXDraw::Instance()->GetUIY(500);
			int yp2 = yp1 + LineHeight + DXDraw::Instance()->GetUIY(6);
			m_Search = IntoMouse(xp1, yp1, xp2, yp2);

			if (m_Search) {
				for (char az = 'a'; az <= 'z'; az++) {
					if (Pad->GetAtoZKey(az).trigger()) {
						m_SearchWord += az;
					}
				}
				for (char az = 0; az <= 9; az++) {
					if (Pad->GetNumKey((int)az).trigger()) {
						m_SearchWord += ('0' + az);
					}
				}
				if (Pad->GetBSorDel().trigger() && m_SearchWord != "") {
					m_SearchWord.pop_back();
				}
			}

			WindowSystem::SetBox(xp1, yp1, xp2, yp2, m_Search ? Gray15 : Gray25);
			WindowSystem::SetMsg((xp1 + xp2) / 2, (yp1 + yp2) / 2, LineHeight, STRX_LEFT, White, Black, (m_SearchWord != "") ? (m_SearchWord + (m_Search ? "|" : "")).c_str() : "マウスをかざして入力…");
			if (WindowMySystem::CloseButton(xp2 - LineHeight - 1, yp1 + 1)) {
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
			WindowSystem::SetMsg(xp, yp, LineHeight * 7 / 10, STRX_LEFT, GetColor(255, 150, 150), Black, m);
			yp += LineHeight * 7 / 10;
			if (yp > DXDraw::Instance()->GetUIY(1080)) { break; }
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
