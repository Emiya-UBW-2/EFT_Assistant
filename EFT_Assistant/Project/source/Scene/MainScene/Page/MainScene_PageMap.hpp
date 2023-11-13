#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"
#include "../Data/MainScene_Parents.hpp"


namespace FPS_n2 {
	enum class EnumWatchMapType {
		Normal,
		InGameMap = Normal,
		Map2D,
		Else,
		Max,
	};
	static const char* WatchMapTypeStr[(int)EnumWatchMapType::Max] = {
		"InGameMap",
		"2DMap",
		"Else",
	};

	class MapBG :public PageParent {
	private:
		MapID		m_SelectBuffer{ InvalidID };
		MapID		m_MapSelect{ InvalidID };
		int			m_WatchMapSelect{ 0 };
		float		m_Rad{ 0.f };
		float		m_Rad_Goal{ 0.f };
		float		m_BaseRad{ 0.f };
		bool		isLoaded{ false };

		int			mouse_moveX{ 0 };
		int			mouse_moveY{ 0 };

		VECTOR_ref	m_BaseLength;

		Graphs		ComPass;
		float		m_ComPassRad{ 0.f };

		TraderID	m_EditTraderID{ InvalidID };
		TaskID		m_EditTaskID{ InvalidID };
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override;
		void LateExecute_Sub(int *, int *, float*) noexcept override;
		void Draw_Back_Sub(int posx, int posy, float Scale) noexcept override;
		void DrawFront_Sub(int posx, int posy, float Scale) noexcept override;
		void Dispose_Sub(void) noexcept override;
	};
};
