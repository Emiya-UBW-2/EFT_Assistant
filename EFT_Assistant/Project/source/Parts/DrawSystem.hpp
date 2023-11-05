#pragma once
#include "../Header.hpp"

namespace FPS_n2 {
	//
	class Rect2D {
		int			m_PosX{ 0 };
		int			m_PosY{ 0 };
		int			m_SizeX{ 0 };
		int			m_SizeY{ 0 };
	public:
		const auto&		GetPosX() const noexcept { return this->m_PosX; }
		const auto&		GetPosY() const noexcept { return this->m_PosY; }
		void			Set(int posx, int posy, int sizex, int sizey) noexcept {
			m_PosX = posx;
			m_PosY = posy;
			m_SizeX = sizex;
			m_SizeY = sizey;
		}
	public:
		bool			IsHit(const Rect2D& target) noexcept {
			return (
				((this->m_PosX >= target.m_PosX && this->m_PosX < (target.m_PosX + target.m_SizeX)) || (target.m_PosX > this->m_PosX && target.m_PosX <= (this->m_PosX + this->m_SizeX))) &&
				((this->m_PosY >= target.m_PosY && this->m_PosY < (target.m_PosY + target.m_SizeY)) || (target.m_PosY > this->m_PosY && target.m_PosY <= (this->m_PosY + this->m_SizeY)))
				);
		}
	};

	enum class DrawType : int {
		Alpha,
		Bright,
		Box,
		Circle,
		Line,
		String,
		StringAutoFit,
		RotaGraph,
	};
	class DrawData {
		DrawType								m_type{ DrawType::Box };
		std::array<int, 6>						m_intParam{ 0,0,0,0,0,0 };
		std::array<unsigned int, 2>				m_UintParam{0,0};
		std::array<float, 6>					m_floatParam{ 0,0,0,0,0,0 };
		std::array<bool, 1>						m_boolParam{ false };
		std::array<const GraphHandle*, 1>		m_GraphHandleParam{ nullptr };
		std::string								m_string;
	public:
		void InputType(DrawType type) noexcept { this->m_type = type; }
		void InputintParam(int ID, int param) noexcept { this->m_intParam.at(ID) = param; }
		void InputUintParam(int ID, unsigned int param) noexcept { this->m_UintParam.at(ID) = param; }
		void InputfloatParam(int ID, float param) noexcept { this->m_floatParam.at(ID) = param; }
		void InputboolParam(int ID, bool param) noexcept { this->m_boolParam.at(ID) = param; }
		void InputGraphHandleParam(int ID, const GraphHandle* param) noexcept { this->m_GraphHandleParam.at(ID) = param; }
		void InputStringParam(std::string_view param) noexcept { this->m_string = param; }
	public:
		void Output() const noexcept {
			Rect2D Widow; Widow.Set(y_r(0), y_r(0), y_r(1920), y_r(1080));

			switch (m_type) {
			case DrawType::Alpha:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->m_intParam[0]);
				break;
			case DrawType::Bright:
				SetDrawBright(this->m_intParam[0], this->m_intParam[1], this->m_intParam[2]);
				break;
			case DrawType::Box:
			{
				Rect2D One; One.Set(std::min(this->m_intParam[0], this->m_intParam[2]), std::min(this->m_intParam[1], this->m_intParam[3]),
					std::abs(this->m_intParam[0] - this->m_intParam[2]), std::abs(this->m_intParam[1] - this->m_intParam[3]));
				if (Widow.IsHit(One)) {
					DxLib::DrawBox(m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_intParam[3], this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE);
				}
			}
				break;
			case DrawType::Circle:
			{
				Rect2D One; One.Set(this->m_intParam[0] - this->m_intParam[2] / 2, this->m_intParam[1] - this->m_intParam[2] / 2,
					this->m_intParam[2] * 2, this->m_intParam[2] * 2);
				if (Widow.IsHit(One)) {
					DxLib::DrawCircle(m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE, this->m_intParam[3]);
				}
			}
				break;
			case DrawType::Line:
			{
				Rect2D One; One.Set(std::min(this->m_intParam[0], this->m_intParam[2]), std::min(this->m_intParam[1], this->m_intParam[3]),
					std::abs(this->m_intParam[0] - this->m_intParam[2]), std::abs(this->m_intParam[1] - this->m_intParam[3]));
				if (Widow.IsHit(One)) {
					DxLib::DrawLine(m_intParam[0], this->m_intParam[1], this->m_intParam[2], this->m_intParam[3], this->m_UintParam[0], this->m_intParam[4]);
				}
			}
				break;
			case DrawType::String:
				FontPool::Instance()->Get((FontPool::FontType)m_intParam[0], this->m_intParam[1]).DrawString(
					-1,
					(FontHandle::FontXCenter)m_intParam[2], (FontHandle::FontYCenter)m_intParam[3],
					m_intParam[4], this->m_intParam[5],
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::StringAutoFit:
				FontPool::Instance()->Get((FontPool::FontType)m_intParam[0], this->m_intParam[1]).DrawStringAutoFit(
					m_intParam[2], m_intParam[3],
					m_intParam[4], this->m_intParam[5],
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::RotaGraph:
				if (m_GraphHandleParam.at(0)) {
					if (m_floatParam[0] < 0.9f && 1.1f < this->m_floatParam[0]) {
						auto prev = GetDrawMode();
						SetDrawMode(DX_DRAWMODE_BILINEAR);
						m_GraphHandleParam.at(0)->DrawRotaGraph(m_intParam[0], this->m_intParam[1], this->m_floatParam[0], this->m_floatParam[1], this->m_boolParam[0]);
						SetDrawMode(prev);
					}
					else {
						m_GraphHandleParam.at(0)->DrawRotaGraph(m_intParam[0], this->m_intParam[1], this->m_floatParam[0], this->m_floatParam[1], this->m_boolParam[0]);
					}
				}
				break;
			default:
				break;
			}
		}
	public:
		void		operator=(const DrawData& tgt) noexcept {
			this->m_type = tgt.m_type;
			this->m_intParam = tgt.m_intParam;
			this->m_UintParam = tgt.m_UintParam;
			this->m_floatParam = tgt.m_floatParam;
			this->m_boolParam = tgt.m_boolParam;
			this->m_GraphHandleParam = tgt.m_GraphHandleParam;
			this->m_string = tgt.m_string;
		}
		bool		operator==(const DrawData& tgt) const noexcept {
			return (
				this->m_type == tgt.m_type &&
				this->m_intParam == tgt.m_intParam &&
				this->m_UintParam == tgt.m_UintParam &&
				this->m_floatParam == tgt.m_floatParam &&
				this->m_boolParam == tgt.m_boolParam &&
				this->m_GraphHandleParam == tgt.m_GraphHandleParam &&
				this->m_string == tgt.m_string
				);
		}
	};
	//
	class Graphs {
		std::string				m_Path{ "" };
		GraphHandle				m_Handle;
		bool					m_Loaded{ false };
		int						m_X{ -1 };
		int						m_Y{ -1 };
		bool					m_IsTrans{ false };
	public:
		void	SetPath(const char* path) noexcept { this->m_Path = path; }
		void	SetIsTrans(bool isTrans) noexcept { this->m_IsTrans = isTrans; }
		bool	LoadByPath(bool isUseTex) noexcept {
			if (this->m_Path != "") {
				if (GetFileAttributes(this->m_Path.c_str()) != 0xFFFFFFFF) {
					if (isUseTex) {
						this->m_Handle = GraphHandle::Load_Tex(this->m_Path.c_str());
					}
					else {
						this->m_Handle = GraphHandle::Load(this->m_Path.c_str());
					}
					this->m_Loaded = false;
					return true;
				}
				else {
					this->m_Path = "";
				}
			}
			return false;
		}
		void	WhenAfterLoad() noexcept {
			if (!this->m_Loaded) {
				if (CheckHandleASyncLoad(this->m_Handle.get()) == FALSE) {
					if (this->m_Path != "") {
						if (!m_IsTrans) {
							GraphFilter(this->m_Handle.get(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 1, TRUE, GetColor(1, 1, 1), 255);
						}
						this->m_Handle.GetSize(&this->m_X, &this->m_Y);
						this->m_Loaded = true;
					}
				}
			}
		}

		void	DisposeGraph() noexcept {
			if (this->m_Loaded) {
				this->m_Handle.Dispose();
			}
			else {
				if (this->m_Handle.IsActive()) {
					SetASyncLoadFinishDeleteFlag(this->m_Handle.get());
				}
			}
		}
	public:
		const auto&	GetPath() const noexcept { return this->m_Path; }
		const auto*	GetGraph() const noexcept { return (this->m_Loaded) ? &this->m_Handle : nullptr; }
		const auto	GetXSize() const noexcept { return (this->m_Loaded) ? this->m_X : -1; }
		const auto	GetYSize() const noexcept { return (this->m_Loaded) ? this->m_Y : -1; }
	};
	//
	enum class DrawLayer : int {
		BackGround,
		Back,
		Back2,
		Back3,
		Back4,
		Normal,
		Front,

		Max,
	};
	//
	class DrawControl : public SingletonBase<DrawControl> {
	private:
		friend class SingletonBase<DrawControl>;
	private:
		std::vector<std::vector<DrawData>>	m_DrawDatas;
		std::vector<std::vector<DrawData>>	m_PrevDrawDatas;

		Graphs					FirGraph;
		Graphs					LockGraph;
		std::vector<Graphs>		GuideIcon;
		GraphHandle				m_BufferScreen;
	private:
		DrawControl() noexcept {
			FirGraph.SetPath(u8"data/UI/FiR.png");
			FirGraph.SetIsTrans(true);
			FirGraph.LoadByPath(false);
			FirGraph.WhenAfterLoad();

			LockGraph.SetPath(u8"data/UI/Lock.png");
			LockGraph.SetIsTrans(true);
			LockGraph.LoadByPath(false);
			LockGraph.WhenAfterLoad();

			auto data_t = GetFileNamesInDirectory("data/UI/icon/");
			for (auto& d : data_t) {
				std::string Name = d.cFileName;
				GuideIcon.resize(GuideIcon.size() + 1);
				GuideIcon.back().SetPath(("data/UI/icon/" + Name).c_str());
				GuideIcon.back().SetIsTrans(true);
				GuideIcon.back().LoadByPath(false);
			}
			for (auto& G : GuideIcon) {
				G.WhenAfterLoad();
			}

			this->m_DrawDatas.resize((int)DrawLayer::Max);
			this->m_PrevDrawDatas.resize((int)DrawLayer::Max);

			m_BufferScreen = GraphHandle::Make(y_r(1920), y_r(1080), false);
		}
		~DrawControl() noexcept {
			for (auto& d : this->m_DrawDatas) {
				d.clear();
			}
			this->m_DrawDatas.clear();

			for (auto& d : this->m_PrevDrawDatas) {
				d.clear();
			}
			this->m_PrevDrawDatas.clear();

			FirGraph.DisposeGraph();
			LockGraph.DisposeGraph();
			for (auto& G : GuideIcon) {
				G.DisposeGraph();
			}
			GuideIcon.clear();
		}

		DrawData* GetBack(DrawLayer Layer) noexcept {
			this->m_DrawDatas.at((int)Layer).resize(this->m_DrawDatas.at((int)Layer).size() + 1);
			return &this->m_DrawDatas.at((int)Layer).back();
		}
	public:
		//
		void	SetAlpha(DrawLayer Layer, int Alpha) {
			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::Alpha);
			Back->InputintParam(0, Alpha);
		}
		//
		void	SetBright(DrawLayer Layer, int valueR, int valueG, int valueB) {
			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::Bright);
			Back->InputintParam(0, valueR);
			Back->InputintParam(1, valueG);
			Back->InputintParam(2, valueB);
		}
		//
		void	SetDrawBox(DrawLayer Layer, int x1, int y1, int x2, int y2, unsigned int color1, bool IsFill) {
			auto* DrawParts = DXDraw::Instance();
			if (!(0 <= std::max(x1, x2) && std::min(x1, x2) <= DrawParts->m_DispXSize && 0 <= std::max(y1, y2) && std::min(y1, y2) <= DrawParts->m_DispYSize)) { return; }				//画面外は表示しない


			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::Box);
			Back->InputintParam(0, x1);
			Back->InputintParam(1, y1);
			Back->InputintParam(2, x2);
			Back->InputintParam(3, y2);
			Back->InputUintParam(0, color1);
			Back->InputboolParam(0, IsFill);
		}
		//
		void	SetDrawCircle(DrawLayer Layer, int x1, int y1, int radius, unsigned int color1, bool IsFill = true, int LineThickness = 1) {
			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::Circle);
			Back->InputintParam(0, x1);
			Back->InputintParam(1, y1);
			Back->InputintParam(2, radius);
			Back->InputUintParam(0, color1);
			Back->InputboolParam(0, IsFill);
			Back->InputintParam(3, LineThickness);
		}
		//
		void	SetDrawLine(DrawLayer Layer, int x1, int y1, int x2, int y2, unsigned int color1, int   Thickness = 1) {
			auto* DrawParts = DXDraw::Instance();
			if (!(0 <= std::max(x1, x2) && std::min(x1, x2) <= DrawParts->m_DispXSize && 0 <= std::max(y1, y2) && std::min(y1, y2) <= DrawParts->m_DispYSize)) { return; }				//画面外は表示しない


			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::Line);
			Back->InputintParam(0, x1);
			Back->InputintParam(1, y1);
			Back->InputintParam(2, x2);
			Back->InputintParam(3, y2);
			Back->InputUintParam(0, color1);
			Back->InputintParam(4, Thickness);
		}
		//
		void SetDrawRotaGraph(DrawLayer Layer, const GraphHandle* pGraphHandle, int posx, int posy, float Exrate, float rad, bool trns) noexcept {
			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::RotaGraph);
			Back->InputGraphHandleParam(0, pGraphHandle);
			Back->InputintParam(0, posx);
			Back->InputintParam(1, posy);
			Back->InputfloatParam(0, Exrate);
			Back->InputfloatParam(1, rad);
			Back->InputboolParam(0, trns);
		}
		//
		void SetDrawRotaFiR(DrawLayer Layer, int posx, int posy, float Exrate, float rad, bool trns) noexcept {
			if (FirGraph.GetGraph()) {
				SetDrawRotaGraph(Layer, FirGraph.GetGraph(), posx, posy, Exrate, rad, trns);
			}
		}
		//
		void SetDrawRotaLock(DrawLayer Layer, int posx, int posy, float Exrate, float rad, bool trns) noexcept {
			if (LockGraph.GetGraph()) {
				SetDrawRotaGraph(Layer, LockGraph.GetGraph(), posx, posy, Exrate, rad, trns);
			}
		}
		//
		void SetDrawRotaGuide(std::string_view name, DrawLayer Layer, int posx, int posy, float Exrate, float rad, bool trns) noexcept {
			for (auto& G : GuideIcon) {
				if (!G.GetGraph()) { continue; }
				if (G.GetPath().find(name) == std::string::npos) { continue; }
				SetDrawRotaGraph(Layer, G.GetGraph(), posx, posy, Exrate, rad, trns);
				break;
			}
		}
		//
		template <typename... Args>
		void	SetString(DrawLayer Layer, FontPool::FontType type, int fontSize, FontHandle::FontXCenter FontX, FontHandle::FontYCenter FontY, int x, int y, unsigned int Color, unsigned int EdgeColor, const std::string& Str, Args&&... args) noexcept {
			if (Str == "") { return; }
			auto* DrawParts = DXDraw::Instance();
			auto* Fonts = FontPool::Instance();

			int xSize = Fonts->Get(type, fontSize).GetStringWidth(-1, Str.c_str(), args...);

			if ((y - fontSize) > DrawParts->m_DispYSize || (y + fontSize) < 0) { return; }				//画面外は表示しない

			switch (FontX) {
			case FontHandle::FontXCenter::LEFT:
				if ((x) > DrawParts->m_DispXSize || (x + xSize) < 0) { return; }						//画面外は表示しない
				break;
			case FontHandle::FontXCenter::MIDDLE:
				if ((x - xSize / 2) > DrawParts->m_DispXSize || (x + xSize / 2) < 0) { return; }		//画面外は表示しない
				break;
			case FontHandle::FontXCenter::RIGHT:
				if ((x - xSize) > DrawParts->m_DispXSize || (x) < 0) { return; }						//画面外は表示しない
				break;
			default:
				break;
			}



			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::String);

			Back->InputintParam(0, (int)type);
			Back->InputintParam(1, fontSize);
			Back->InputintParam(2, (int)FontX);
			Back->InputintParam(3, (int)FontY);
			Back->InputintParam(4, x);
			Back->InputintParam(5, y);
			Back->InputUintParam(0, Color);
			Back->InputUintParam(1, EdgeColor);

			char ptr[1024];
			snprintfDx(ptr, 1024, Str.c_str(), args...);
			Back->InputStringParam(ptr);
		}
		//
		const auto	SetStringAutoFit(DrawLayer Layer, FontPool::FontType type, int fontSize, int x1, int y1, int x2, int y2, unsigned int Color, unsigned int EdgeColor, const std::string& Str) noexcept {
			if (Str == "") { return 0.f; }
			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::StringAutoFit);

			Back->InputintParam(0, (int)type);
			Back->InputintParam(1, fontSize);
			Back->InputintParam(2, x1);
			Back->InputintParam(3, y1);
			Back->InputintParam(4, x2);
			Back->InputintParam(5, y2);
			Back->InputUintParam(0, Color);
			Back->InputUintParam(1, EdgeColor);

			Back->InputStringParam(Str);

			return FontPool::Instance()->Get((FontPool::FontType)type, fontSize).DrawStringAutoFit(
				x1 + 1920, y1 + 1080,
				x2 + 1920, y2 + 1080,
				Color,
				EdgeColor,
				Str
			);
		}
		//
	public:
		void	ClearList() noexcept {
			for (auto& d : this->m_DrawDatas) {
				auto& pd = this->m_PrevDrawDatas.at(&d - &this->m_DrawDatas.front());
				pd.clear();
				for (auto& d2 : d) {
					pd.resize(pd.size() + 1);
					pd.back() = d2;
				}
			}
			for (auto& d : this->m_DrawDatas) {
				d.clear();
			}
		}
		void	Draw() noexcept {
			bool IsHit = false;
			//同じかどうかチェック
			for (auto& d : this->m_DrawDatas) {
				auto& pd = this->m_PrevDrawDatas.at(&d - &this->m_DrawDatas.front());
				if (pd.size() == d.size()) {
					for (auto& d2 : d) {
						auto& pd2 = pd.at(&d2 - &d.front());
						if (!(pd2 == d2)) {
							IsHit = true;
							break;
						}
					}
				}
				else {
					IsHit = true;
				}
			}
			//
			if (IsHit) {
				{
					auto NowScreen = GetDrawScreen();
					m_BufferScreen.SetDraw_Screen(true);
					{
						for (auto& d : this->m_DrawDatas) {
							for (auto& d2 : d) {
								d2.Output();
							}
							SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
							SetDrawBright(255, 255, 255);
						}
					}
					GraphHandle::SetDraw_Screen(NowScreen, false);
				}
			}
			//前に描画したものをそのまま出す
			m_BufferScreen.DrawGraph(0, 0, false);
		}
	};
	//
};
