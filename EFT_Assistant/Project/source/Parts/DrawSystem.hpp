#pragma once
#include "../Header.hpp"

namespace FPS_n2 {
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
	class DrawGraphs : public SingletonBase<DrawGraphs> {
	private:
		friend class SingletonBase<DrawGraphs>;
	private:
		Graphs					FirGraph;
		Graphs					LockGraph;
		std::vector<Graphs>		GuideIcon;
	private:
		DrawGraphs() noexcept {
			FirGraph.SetPath(u8"data/UI/FiR.png");
			FirGraph.SetIsTrans(true);
			FirGraph.LoadByPath(false);
			FirGraph.WhenAfterLoad();

			LockGraph.SetPath(u8"data/UI/Lock.png");
			LockGraph.SetIsTrans(true);
			LockGraph.LoadByPath(false);
			LockGraph.WhenAfterLoad();

			std::vector<WIN32_FIND_DATA> data_t;
			GetFileNamesInDirectory("data/UI/icon/*", &data_t);
			for (auto& d : data_t) {
				std::string Name = d.cFileName;
				GuideIcon.resize(GuideIcon.size() + 1);
				GuideIcon.back().SetPath(("data/UI/icon/" + Name + ".png").c_str());
				GuideIcon.back().SetIsTrans(true);
				GuideIcon.back().LoadByPath(false);
			}
			data_t.clear();

			for (auto& G : GuideIcon) {
				G.WhenAfterLoad();
			}
		}
		~DrawGraphs() noexcept {
			FirGraph.DisposeGraph();
			LockGraph.DisposeGraph();
			for (auto& G : GuideIcon) {
				G.DisposeGraph();
			}
			GuideIcon.clear();
		}
	public:
		const auto& GetFirGraph() const noexcept { return FirGraph; }
		const auto& GetLockGraph() const noexcept { return LockGraph; }
		const auto& GetGuide(std::string_view name)  const noexcept {
			for (auto& G : GuideIcon) {
				if (!G.GetGraph()) { continue; }
				if (G.GetPath().find(name) == std::string::npos) { continue; }
				return G;
			}
			return GuideIcon[0];
		}
	public:
	};
	//
};
