#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	typedef int MapID;
	class MapList : public ListParent<MapID> {
		std::string				m_InGameMapPath{ "" };
		GraphHandle				m_InGameMap;
		bool					m_InGameMapLoaded{ false };
		int						InGameMapX{ -1 };
		int						InGameMapY{ -1 };
	private:
		//í«â¡ê›íË
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "InGamePath") {
				m_InGameMapPath = RIGHT;
			}
		}
		void	Load_Sub() noexcept override {
			if (m_InGameMapPath != "") {
				m_InGameMap = GraphHandle::Load(m_InGameMapPath.c_str(), false);
				m_InGameMapLoaded = false;
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {
			if (m_InGameMapPath != "") {
				GraphFilter(this->m_InGameMap.get(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 1, TRUE, Black, 255);
				this->m_InGameMap.GetSize(&InGameMapX, &InGameMapY);
				m_InGameMapLoaded = true;
			}
		}
	public:
		const auto*	GetInGameMapGraph() const noexcept { return (m_InGameMapLoaded) ? &m_InGameMap : nullptr; }
		const auto	GetInGameMapXSize() const noexcept { return (m_InGameMapLoaded) ? InGameMapX : -1; }
		const auto	GetInGameMapYSize() const noexcept { return (m_InGameMapLoaded) ? InGameMapY : -1; }
	};
	class MapData : public SingletonBase<MapData>, public DataParent<MapID, MapList> {
	private:
		friend class SingletonBase<MapData>;
	private:
		MapData() noexcept {
			SetList("data/map/");
		}
		~MapData() noexcept {}
	};
};