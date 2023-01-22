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
		std::array<int,6>						m_intParam;
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

	class DrawControl : public SingletonBase<DrawControl> {
	private:
		friend class SingletonBase<DrawControl>;
	private:
		std::vector<DrawData>	m_DrawData;
	private:
		DrawControl() noexcept {

		}
		~DrawControl() noexcept {
			m_DrawData.clear();
		}
	public:
		//
		void	SetAlpha(int Alpha) {
			m_DrawData.resize(m_DrawData.size() + 1);
			m_DrawData.back().InputType(DrawType::Alpha);
			m_DrawData.back().InputintParam(0, Alpha);
		}
		//
		void	SetDrawBox(int x1, int y1, int x2, int y2, unsigned int color1, bool IsFill) {
			m_DrawData.resize(m_DrawData.size() + 1);
			m_DrawData.back().InputType(DrawType::Box);
			m_DrawData.back().InputintParam(0, x1);
			m_DrawData.back().InputintParam(1, y1);
			m_DrawData.back().InputintParam(2, x2);
			m_DrawData.back().InputintParam(3, y2);
			m_DrawData.back().InputUintParam(0, color1);
			m_DrawData.back().InputboolParam(0, IsFill);
		}
		//
		void	SetDrawCircle(int x1, int y1, int radius, unsigned int color1, bool IsFill = true, int LineThickness = 1) {
			m_DrawData.resize(m_DrawData.size() + 1);
			m_DrawData.back().InputType(DrawType::Circle);
			m_DrawData.back().InputintParam(0, x1);
			m_DrawData.back().InputintParam(1, y1);
			m_DrawData.back().InputintParam(2, radius);
			m_DrawData.back().InputUintParam(0, color1);
			m_DrawData.back().InputboolParam(0, IsFill);
			m_DrawData.back().InputintParam(3, LineThickness);
		}
		//
		void	SetDrawLine(int x1, int y1, int x2, int y2, unsigned int color1, int   Thickness = 1) {
			m_DrawData.resize(m_DrawData.size() + 1);
			m_DrawData.back().InputType(DrawType::Line);
			m_DrawData.back().InputintParam(0, x1);
			m_DrawData.back().InputintParam(1, y1);
			m_DrawData.back().InputintParam(2, x2);
			m_DrawData.back().InputintParam(3, y2);
			m_DrawData.back().InputUintParam(0, color1);
			m_DrawData.back().InputintParam(4, Thickness);
		}
		//
		void SetDrawRotaGraph(const GraphHandle* pGraphHandle,int posx, int posy, float Exrate, float rad, bool trns) noexcept {
			m_DrawData.resize(m_DrawData.size() + 1);
			m_DrawData.back().InputType(DrawType::RotaGraph);
			m_DrawData.back().InputGraphHandleParam(0, pGraphHandle);
			m_DrawData.back().InputintParam(0, posx);
			m_DrawData.back().InputintParam(1, posy);
			m_DrawData.back().InputfloatParam(0, Exrate);
			m_DrawData.back().InputfloatParam(1, rad);
			m_DrawData.back().InputboolParam(0, trns);
		}
		//
		template <typename... Args>
		void	SetString(FontPool::FontType type, int fontSize, FontHandle::FontXCenter FontX, FontHandle::FontYCenter FontY, int x, int y, unsigned int Color, unsigned int EdgeColor, const std::string& String, Args&&... args) noexcept {
			m_DrawData.resize(m_DrawData.size() + 1);
			m_DrawData.back().InputType(DrawType::String);

			m_DrawData.back().InputintParam(0, (int)type);
			m_DrawData.back().InputintParam(1, fontSize);
			m_DrawData.back().InputintParam(2, (int)FontX);
			m_DrawData.back().InputintParam(3, (int)FontY);
			m_DrawData.back().InputintParam(4, x);
			m_DrawData.back().InputintParam(5, y);
			m_DrawData.back().InputUintParam(0, Color);
			m_DrawData.back().InputUintParam(1, EdgeColor);

			char ptr[1024];
			snprintfDx(ptr, 1024, String.c_str(), args...);
			m_DrawData.back().InputStringParam(ptr);
		}
		//
	public:
		void	ClearList() noexcept {
			m_DrawData.clear();
		}
		void	Draw() noexcept{
			for (auto& d : m_DrawData) {
				d.Output();
			}
		}
	};
};
