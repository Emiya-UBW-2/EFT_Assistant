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
	static const auto STRX_LEFT{ FontHandle::FontXCenter::LEFT };
	static const auto STRX_MID{ FontHandle::FontXCenter::MIDDLE };
	static const auto STRX_RIGHT{ FontHandle::FontXCenter::RIGHT };
	static const auto STRY_TOP{ FontHandle::FontYCenter::TOP };
	static const auto STRY_MIDDLE{ FontHandle::FontYCenter::MIDDLE };
	static const auto STRY_BOTTOM{ FontHandle::FontYCenter::BOTTOM };
	//
	namespace WindowSystem {
		//箱
		extern void SetBox(int xp1, int yp1, int xp2, int yp2, unsigned int colorSet);
		//文字
		template <typename... Args>
		extern int GetMsgLen(int size, std::string_view String, Args&&... args) {
			auto* Fonts = FontPool::Instance();
			return Fonts->Get(FontPool::FontType::Nomal_Edge, size).GetStringWidth(-1, ((std::string)String).c_str(), args...) + y_r(6) + 2;//エッジ分:
		}
		template <typename... Args>
		extern int SetMsg(int xp1, int yp1, int xp2, int yp2, int size, FontHandle::FontXCenter FontX, unsigned int Color, unsigned int EdleColor, std::string_view String, Args&&... args) {
			if (String == "") { return 0; }
			auto* DrawParts = DXDraw::Instance();
			int xSize = GetMsgLen(size, String, args...);
			int xpos = 0;
			int ypos = yp1 + (yp2 - yp1) / 2;
			if ((ypos - size / 2) > DrawParts->m_DispYSize || (ypos + size / 2) < 0) { return 0; }				//画面外は表示しない
			switch (FontX) {
			case STRX_LEFT:
				xpos = xp1 + y_r(6);
				if ((xpos) > DrawParts->m_DispXSize || (xpos + xSize) < 0) { return 0; }						//画面外は表示しない
				break;
			case STRX_MID:
				xpos = xp1 + (xp2 - xp1) / 2;
				if ((xpos - xSize / 2) > DrawParts->m_DispXSize || (xpos + xSize / 2) < 0) { return 0; }		//画面外は表示しない
				break;
			case STRX_RIGHT:
				xpos = xp2 - y_r(6);
				if ((xpos - xSize) > DrawParts->m_DispXSize || (xpos) < 0) { return 0; }						//画面外は表示しない
				break;
			default:
				break;
			}
			DrawControl::Instance()->SetString(DrawLayer::Normal, FontPool::FontType::Nomal_Edge, size, FontX, STRY_MIDDLE, xpos, ypos, Color, EdleColor, ((std::string)String).c_str(), args...);
			return xSize;//エッジ分
		}
		template <typename... Args>
		extern bool ClickCheckBox(int xp1, int yp1, int xp2, int yp2, bool isrepeat, bool IsActive, unsigned int defaultcolor, std::string_view String, Args&&... args) {
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
			SetMsg(xp1, yp1, xp2, yp2, std::min(LineHeight, yp2 - yp1) - y_r(6), STRX_MID, White, Black, String, args...);
			return isHit;
		};
		extern bool CloseButton(int xp1, int yp1);
		extern void CheckBox(int xp1, int yp1, bool* switchturn);
		extern void	UpDownBar(int xmin, int xmax, int yp, int* value, int valueMin, int valueMax);
		//
		class ScrollBoxClass {
			bool		m_IsChangeScrollY{ false };
			int			m_BaseScrollY{ 0 };
			float		m_NowScrollYPer{ 0.f };
		public:
			const auto&		GetNowScrollYPer(void) const noexcept { return this->m_NowScrollYPer; }
			void			ScrollBox(int xp1, int yp1, int xp2, int yp2, float TotalPer, bool IsActive);
		};
		class WindowControl {
		public:
			bool								m_isDelete{ false };
			signed long long					m_FreeID{ 0 };
		private:
			bool								m_ActiveSwitch{ false };
			bool								m_IsActive{ false };
			int									m_PosX{ 0 }, m_PosY{ 0 };
			int									m_SizeX{ 100 }, m_SizeY{ 100 };
			int									m_TotalSizeY{ 200 };
			std::string							m_TabName;
			std::function<void(WindowControl*)>	m_DoingOnWindow;

			ScrollBoxClass						m_Scroll;
		private:
			bool								m_isMaxSize{ false };
			int									m_PosXSave{ 0 }, m_PosYSave{ 0 };
			int									m_SizeXSave{ 100 }, m_SizeYSave{ 100 };

			bool								m_CanChageSize{ false };
			bool								m_CanPressXButton{ false };

			bool								m_IsMoving{ false };
			int									m_PosAddX{ 0 }, m_PosAddY{ 0 };

			bool								m_IsChangeScaleXY{ false };
			bool								m_IsChangeScale1X{ false };
			bool								m_IsChangeScale1Y{ false };
			bool								m_IsChangeScale2X{ false };
			bool								m_IsChangeScale2Y{ false };
			int									m_BaseScaleX{ 0 }, m_BaseScaleY{ 0 };
			int									m_BasePos1X{ 0 }, m_BasePos1Y{ 0 };
			int									m_BaseScale1X{ 0 }, m_BaseScale1Y{ 0 };
			int									m_BaseScale2X{ 0 }, m_BaseScale2Y{ 0 };
		public:
			const bool		GetIsEditing(void) const noexcept;

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
			void Set(int posx, int posy, int sizex, int sizey, int Totalsizey, const char* tabName, bool canChageSize, bool canPressXButton, signed long long FreeID, const std::function<void(WindowControl*)>& DoingOnWindow) noexcept;
			void Draw(void) noexcept;
		};
		class WindowManager : public SingletonBase<WindowManager> {
		private:
			friend class SingletonBase<WindowManager>;
		private:
			std::vector<std::shared_ptr<WindowControl>>	m_WindowControl;
		private:
			WindowManager() noexcept {}
			~WindowManager() noexcept {}
		public:
			const auto	PosHitCheck(WindowSystem::WindowControl* window) const noexcept {
				auto res = std::find_if(m_WindowControl.begin(), this->m_WindowControl.end(), [&](const std::shared_ptr<WindowControl>& tgt) { return(tgt.get() == window); });
				bool isSel = (window == nullptr);
				for (auto& w : this->m_WindowControl) {
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
				for (auto& w : this->m_WindowControl) {
					w->m_isDelete = true;
				}
			}
		public:
			const auto&	Get() const noexcept { return this->m_WindowControl; }
			const auto&	Add() noexcept {
				m_WindowControl.emplace_back(std::make_shared<WindowControl>());
				return this->m_WindowControl.back();
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
							std::swap(m_WindowControl[i], this->m_WindowControl.back());
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
		class SearchBox {
			bool							m_Search{ false };
			std::string						m_SearchWord;
		public:
			void		Init() noexcept {
				m_SearchWord = "";
			}
			void		Draw(int xp1, int yp1) noexcept;
			const bool	GetIsHit(std::string Name) const noexcept;
		};
	};
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
		void Draw() noexcept;
		void AddLog(const char* Mes) noexcept;
		void Save() noexcept;
	};
	//
};