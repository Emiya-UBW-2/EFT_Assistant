#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	enum class DrawType : int {
		Alpha,
		Box,
		Circle,
		Line,
		String,
		RotaGraph,
	};
	class DrawData {
		DrawType								m_type{ DrawType::Box };
		std::array<int, 6>						m_intParam;
		std::array<unsigned int, 2>				m_UintParam;
		std::array<float, 6>					m_floatParam;
		std::array<bool, 1>						m_boolParam;
		std::array<const GraphHandle*, 1>		m_GraphHandleParam;
		std::string								m_string;
	public:
		void InputType(DrawType type) noexcept { m_type = type; }
		void InputintParam(int ID, int param) noexcept { m_intParam.at(ID) = param; }
		void InputUintParam(int ID, unsigned int param) noexcept { m_UintParam.at(ID) = param; }
		void InputfloatParam(int ID, float param) noexcept { m_floatParam.at(ID) = param; }
		void InputboolParam(int ID, bool param) noexcept { m_boolParam.at(ID) = param; }
		void InputGraphHandleParam(int ID, const GraphHandle* param) noexcept { m_GraphHandleParam.at(ID) = param; }
		void InputStringParam(std::string_view param) noexcept { m_string = param; }
	public:
		void Output() const noexcept {
			switch (m_type) {
			case DrawType::Alpha:
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_intParam[0]);
				break;
			case DrawType::Box:
				DxLib::DrawBox(m_intParam[0], m_intParam[1], m_intParam[2], m_intParam[3], m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE);
				break;
			case DrawType::Circle:
				DxLib::DrawCircle(m_intParam[0], m_intParam[1], m_intParam[2], m_UintParam[0], (m_boolParam[0]) ? TRUE : FALSE, m_intParam[3]);
				break;
			case DrawType::Line:
				DxLib::DrawLine(m_intParam[0], m_intParam[1], m_intParam[2], m_intParam[3], m_UintParam[0], m_intParam[4]);
				break;
			case DrawType::String:
				FontPool::Instance()->Get((FontPool::FontType)m_intParam[0], m_intParam[1]).DrawString(
					-1,
					(FontHandle::FontXCenter)m_intParam[2], (FontHandle::FontYCenter)m_intParam[3],
					m_intParam[4], m_intParam[5],
					m_UintParam[0],
					m_UintParam[1],
					m_string.c_str()
				);
				break;
			case DrawType::RotaGraph:
				if (m_GraphHandleParam.at(0)) {
					if (m_floatParam[0] < 0.9f && 1.1f < m_floatParam[0]) {
						auto prev = GetDrawMode();
						SetDrawMode(DX_DRAWMODE_BILINEAR);
						m_GraphHandleParam.at(0)->DrawRotaGraph(m_intParam[0], m_intParam[1], m_floatParam[0], m_floatParam[1], m_boolParam[0]);
						SetDrawMode(prev);
					}
					else {
						m_GraphHandleParam.at(0)->DrawRotaGraph(m_intParam[0], m_intParam[1], m_floatParam[0], m_floatParam[1], m_boolParam[0]);
					}
				}
				break;
			default:
				break;
			}
		}
	};
	//
	class Rect2D {
		int			m_PosX{ 0 };
		int			m_PosY{ 0 };
		int			m_SizeX{ 0 };
		int			m_SizeY{ 0 };
	public:
		const auto&		GetPosX() const noexcept { return m_PosX; }
		const auto&		GetPosY() const noexcept { return m_PosY; }
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
		void	LoadByPath(bool isUseTex) noexcept {
			if (this->m_Path != "") {
				if (GetFileAttributes(this->m_Path.c_str()) != INVALID_FILE_ATTRIBUTES) {
					if (isUseTex) {
						this->m_Handle = GraphHandle::Load_Tex(this->m_Path.c_str());
					}
					else {
						this->m_Handle = GraphHandle::Load(this->m_Path.c_str());
					}
					this->m_Loaded = false;
				}
				else {
					this->m_Path = "";
				}
			}
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

		Graphs					FirGraph;
		Graphs					LockGraph;
	private:
		DrawControl() noexcept {
			FirGraph.SetPath("data/FiR.png");
			FirGraph.SetIsTrans(true);
			FirGraph.LoadByPath(false);
			FirGraph.WhenAfterLoad();

			LockGraph.SetPath("data/Lock.png");
			LockGraph.SetIsTrans(true);
			LockGraph.LoadByPath(false);
			LockGraph.WhenAfterLoad();
		}
		~DrawControl() noexcept {
			ClearList();

			FirGraph.DisposeGraph();
			LockGraph.DisposeGraph();
		}

		DrawData* GetBack(DrawLayer Layer) noexcept {
			m_DrawDatas.at((int)Layer).resize(m_DrawDatas.at((int)Layer).size() + 1);
			return &m_DrawDatas.at((int)Layer).back();
		}
	public:
		//
		void	SetAlpha(DrawLayer Layer, int Alpha) {
			DrawData*Back = GetBack(Layer);
			Back->InputType(DrawType::Alpha);
			Back->InputintParam(0, Alpha);
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
			if(!(0 <= std::max(x1, x2) && std::min(x1, x2) <= DrawParts->m_DispXSize && 0 <= std::max(y1, y2) && std::min(y1, y2) <= DrawParts->m_DispYSize)) { return; }				//画面外は表示しない


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
		template <typename... Args>
		void	SetString(DrawLayer Layer, FontPool::FontType type, int fontSize, FontHandle::FontXCenter FontX, FontHandle::FontYCenter FontY, int x, int y, unsigned int Color, unsigned int EdgeColor, const std::string& String, Args&&... args) noexcept {
			if (String == "") { return; }
			auto* DrawParts = DXDraw::Instance();
			auto* Fonts = FontPool::Instance();

			int xSize = Fonts->Get(type, fontSize).GetStringWidth(-1, String.c_str(), args...);

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
			snprintfDx(ptr, 1024, String.c_str(), args...);
			Back->InputStringParam(ptr);
		}
		//
	public:
		void	ClearList() noexcept {
			for (auto& d : m_DrawDatas) {
				d.clear();
			}
			m_DrawDatas.clear();
			m_DrawDatas.resize((int)DrawLayer::Max);
		}
		void	Draw() noexcept {
			for (auto& ds : m_DrawDatas) {
				for (auto& da : ds) {
					da.Output();
				}
			}
		}
	};
};
