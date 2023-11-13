#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"

namespace FPS_n2 {
	enum class EnumTaskDrawMode {
		Normal,
		List = Normal,
		Tree,
		Fir,
		NotFir,
		Max,
	};
	static const char* TaskDrawModeStr[(int)EnumTaskDrawMode::Max] = {
	"タスク一覧",
	"ツリー",
	"タスクFirアイテム",
	"タスクNotFirアイテム",
	};


	class TaskBG :public PageParent {
	private:
		int															m_posxMaxBuffer{ 0 };
		int															m_posyMaxBuffer{ 0 };
		std::vector<Rect2D>											m_TaskRect;
		struct LinePos {
			TaskID	m_ID{ InvalidID };
			int		m_XPos{ 0 };
			int		m_YPos{ 0 };
		public:
			LinePos(TaskID ID, int xp, int yp) noexcept {
				this->m_ID = ID;
				this->m_XPos = xp;
				this->m_YPos = yp;
			}
		};
		std::vector<LinePos>										m_ParentLinePos;
		std::vector<TaskID>											m_Drawed;
		EnumTaskDrawMode											m_Mode{ EnumTaskDrawMode::Normal };

		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };

		bool							m_DrawClearTask{ true };
		bool							m_DrawNotClearTask{ true };
		bool							m_DrawCanClearTask{ false };
	private:
		void DrawChildTaskClickBox(float Scale, TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) noexcept;

		void DrawTab(int xp, int yp, std::string_view Info) noexcept;
		void DrawCheckBox(int xp, int yp, std::string_view Info, bool* Check) noexcept;
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override;

		void LateExecute_Sub(int*posx, int*posy, float*Scale) noexcept override;
		void Draw_Back_Sub(int posx, int posy, float Scale) noexcept override;
		void DrawFront_Sub(int posx, int posy, float) noexcept override;
		void Dispose_Sub(void) noexcept override {}
	};
};
