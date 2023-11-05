#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageBase.hpp"

namespace FPS_n2 {
	struct MapGraphParam {
		Graphs	m_Graph;
		int		m_Degree{ 0 };
		float	m_SizetoMeter{ 1.f };
	};
	class MapList : public ListParent<MapID> {
		std::vector<MapGraphParam>		m_MapGraph;
	private:
		//追加設定
		void			SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override {
			if (LEFT == "InGamePath") {
				m_MapGraph.resize(m_MapGraph.size() + 1);
				m_MapGraph.back().m_Graph.SetPath(Args[0].c_str());
			}
			else if (LEFT == "2DPath") {
				m_MapGraph.resize(m_MapGraph.size() + 1);
				m_MapGraph.back().m_Graph.SetPath(Args[0].c_str());
			}
			else if (LEFT == "ElsePath") {
				if (m_MapGraph.size() < 2) {
					DataErrorLog::Instance()->AddLog(("必要なマップ画像を設定してません:" + GetName()).c_str());
				}
				m_MapGraph.resize(m_MapGraph.size() + 1);
				m_MapGraph.back().m_Graph.SetPath(Args[0].c_str());
			}
			else if (LEFT == "NorthDegree") {
				m_MapGraph.back().m_Degree = std::stoi(Args[0]);
			}
			else if (LEFT == "SizetoMeter") {
				m_MapGraph.back().m_SizetoMeter = std::stof(Args[0]);
			}
		}
		void	Load_Sub() noexcept override {}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		void	LoadMapPics() noexcept {
			for (auto& m : this->m_MapGraph) {
				m.m_Graph.LoadByPath(true);
			}
		}
		void	WhenAfterLoadMapPics() noexcept {
			for (auto& m : this->m_MapGraph) {
				m.m_Graph.WhenAfterLoad();
			}
		}
		void	DisposeMapPics() noexcept {
			for (auto& m : this->m_MapGraph) {
				m.m_Graph.DisposeGraph();
			}
		}
		const auto	GetMapPicNum() const noexcept { return this->m_MapGraph.size(); }
		const auto*	GetMapGraph(int ID) const noexcept { return this->m_MapGraph.at(ID).m_Graph.GetGraph(); }
		const auto	GetMapXSize(int ID) const noexcept { return this->m_MapGraph.at(ID).m_Graph.GetXSize(); }
		const auto	GetMapYSize(int ID) const noexcept { return this->m_MapGraph.at(ID).m_Graph.GetYSize(); }
		const auto	GetMapNorthRad(int ID) const noexcept { return deg2rad(m_MapGraph.at(ID).m_Degree); }
		const auto&	GeSizetoMeter(int ID) const noexcept { return this->m_MapGraph.at(ID).m_SizetoMeter; }
	};
	class MapData : public DataParent<MapID, MapList> {
	public:
		MapData() noexcept {
			SetDirList("data/map/");
		}
		~MapData() noexcept {}
	};
};
