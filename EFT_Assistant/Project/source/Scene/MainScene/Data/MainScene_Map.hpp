#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int MapID;
	class MapList : public ListParent<MapID> {
		std::vector<std::pair<Graphs,int>>		m_MapGraph;
	private:
		//追加設定
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "InGamePath") {
				m_MapGraph.resize(m_MapGraph.size() + 1);
				m_MapGraph.back().first.SetPath(RIGHT.c_str());
			}
			if (LEFT == "2DPath") {
				m_MapGraph.resize(m_MapGraph.size() + 1);
				m_MapGraph.back().first.SetPath(RIGHT.c_str());
			}
			if (LEFT == "ElsePath") {
				if (m_MapGraph.size() < 2) {
					DataErrorLog::Instance()->AddLog(("必要なマップ画像を設定してません:" + GetName()).c_str());
				}
				m_MapGraph.resize(m_MapGraph.size() + 1);
				m_MapGraph.back().first.SetPath(RIGHT.c_str());
			}
			if (LEFT == "NorthDegree") {
				m_MapGraph.back().second = std::stoi(RIGHT);
			}
		}
		void	Load_Sub() noexcept override {
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	public:
		void	LoadMapPics() noexcept {
			for (auto& m : m_MapGraph) {
				m.first.LoadByPath();
			}
		}
		void	WhenAfterLoadMapPics() noexcept {
			for (auto& m : m_MapGraph) {
				m.first.WhenAfterLoad();
			}
		}
		void	DisposeMapPics() noexcept {
			for (auto& m : m_MapGraph) {
				m.first.DisposeGraph();
			}
		}
		const auto	GetMapPicNum() const noexcept { return m_MapGraph.size(); }
		const auto*	GetMapGraph(int ID) const noexcept { return m_MapGraph.at(ID).first.GetGraph(); }
		const auto	GetMapXSize(int ID) const noexcept { return m_MapGraph.at(ID).first.GetXSize(); }
		const auto	GetMapYSize(int ID) const noexcept { return m_MapGraph.at(ID).first.GetYSize(); }
		const auto	GetMapNorthRad(int ID) const noexcept { return deg2rad(m_MapGraph.at(ID).second); }
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