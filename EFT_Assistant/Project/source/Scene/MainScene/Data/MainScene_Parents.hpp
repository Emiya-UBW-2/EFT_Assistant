#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
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
			if (this->m_Path != "") {
				if (!m_IsTrans) {
					GraphFilter(this->m_Handle.get(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 1, TRUE, Black, 255);
				}
				this->m_Handle.GetSize(&this->m_X, &this->m_Y);
				this->m_Loaded = true;
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
	static const int InvalidID{ -1 };
	static const int ElseSelectID{ -2 };
	static const auto STR_LEFT{ FontHandle::FontXCenter::LEFT };
	static const auto STR_MID{ FontHandle::FontXCenter::MIDDLE };
	static const auto STR_RIGHT{ FontHandle::FontXCenter::RIGHT };
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
		void Draw() noexcept {
			int xp = 0;
			int yp = LineHeight;
			for (auto& m : m_Mes) {
				WindowSystem::SetMsg(xp, yp, xp, yp, LineHeight, STR_LEFT, GetColor(255, 50, 50), GetColor(1, 1, 1), m);
				yp += LineHeight;
			}
		}
		void AddLog(const char* Mes) noexcept {
			m_Mes.emplace_back(Mes);
		}
	};
	//
	template <class ID>
	class ListParent {
	private:
		ID						m_ID{ InvalidID };
		std::string				m_Name;
		std::array<int, 3>		m_Color{ 0,0,0 };

		Graphs					m_Icon;
	private:
		const auto		GetArgs(const std::string& RIGHT) const noexcept {
			std::vector<std::string> Args;
			auto L = RIGHT.find("[");
			auto R = RIGHT.find("]");
			if (L != std::string::npos && R != std::string::npos) {
				std::string RIGHTBuf = RIGHT;
				RIGHTBuf = RIGHTBuf.substr(L + 1);
				while (true) {
					auto div = RIGHTBuf.find(",");
					if (div != std::string::npos) {
						Args.emplace_back(RIGHTBuf.substr(0, div));
						RIGHTBuf = RIGHTBuf.substr(div + 1);
					}
					else {
						Args.emplace_back(RIGHTBuf.substr(0, RIGHTBuf.find("]")));
						break;
					}
				}
			}
			return Args;
		}
		void			SetCommon(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
			if (LEFT == "Name") {
				m_Name = RIGHT;
			}
			else if (LEFT == "Color") {
				m_Color[0] = std::stoi(Args[0]);
				m_Color[1] = std::stoi(Args[1]);
				m_Color[2] = std::stoi(Args[2]);
			}
		}
	protected:
		virtual void	Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept {}
		virtual void	Load_Sub() noexcept {}
		virtual void	WhenAfterLoad_Sub() noexcept {}
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetName() const noexcept { return m_Name; }
		const auto		GetColors(int colorAdd) const noexcept {
			return DxLib::GetColor(std::clamp(m_Color[0] + colorAdd, 0, 255), std::clamp(m_Color[1] + colorAdd, 0, 255), std::clamp(m_Color[2] + colorAdd, 0, 255));
		}
		const auto&		GetIcon() const noexcept { return m_Icon; }
	public:
		void			Set(const char* FilePath, ID id, const char* IconPath = nullptr) noexcept {
			m_ID = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				//コメントアウト
				if (ALL.find("//") != std::string::npos) {
					ALL = ALL.substr(0, ALL.find("//"));
				}
				//
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				auto Args = GetArgs(RIGHT);
				SetCommon(LEFT, RIGHT, Args);
				Set_Sub(LEFT, RIGHT, Args);
			}
			FileRead_close(mdata);
			if (IconPath) {
				m_Icon.SetPath(IconPath);
			}
		}
		void			Load() noexcept {
			m_Icon.LoadByPath(false);
			Load_Sub();
		}
		void			WhenAfterLoad() noexcept {
			m_Icon.WhenAfterLoad();
			WhenAfterLoad_Sub();
		}
	};
	//
	template <class ID, class List>
	class DataParent {
	protected:
		std::vector<List>	m_List;
	protected:
		void SetList(const char* DirPath) noexcept {
			auto data_t = GetFileNamesInDirectory(DirPath);
			std::vector<std::pair<std::string, bool>> DirNames;
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Tmp = d.cFileName;
					auto txtpos = Tmp.find(".txt");
					if (txtpos != std::string::npos) {
						std::pair<std::string, bool> tmp;
						tmp.first = Tmp.substr(0, txtpos);
						tmp.second = false;
						DirNames.emplace_back(tmp);
					}
				}
			}
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Tmp = d.cFileName;
					auto txtpos = Tmp.find(".txt");
					if (txtpos == std::string::npos) {
						auto pngpos = Tmp.find(".png");
						if (pngpos != std::string::npos) {
							for (auto& d2 : DirNames) {
								if (d2.first == Tmp.substr(0, pngpos)) {
									d2.second = true;
									break;
								}
							}
						}
					}
				}
			}
			int baseIndex = (int)m_List.size();
			m_List.resize(baseIndex + DirNames.size());
			for (auto& d : DirNames) {
				int index = (int)(&d - &DirNames.front()) + baseIndex;
				m_List[index].Set((DirPath + d.first + ".txt").c_str(), (ID)index, d.second ? (DirPath + d.first + ".png").c_str() : nullptr);
			}
			DirNames.clear();

		}
	public:
		void LoadList(void) noexcept {
			for (auto&t : m_List) {
				t.Load();
			}
		}
		void WhenAfterLoadList(void) noexcept {
			for (auto&t : m_List) {
				t.WhenAfterLoad();
			}
		}
	public:
		const ID		FindID(const char* name) const noexcept {
			for (const auto&t : m_List) {
				if (t.GetName() == name) {
					return t.GetID();
				}
			}
			std::string ErrMes = "Error : Not Find Item : ";
			ErrMes += name;
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			return InvalidID;
		}
		List*		FindPtr(ID id) const noexcept {
			for (auto&t : m_List) {
				if (t.GetID() == id) {
					return (List*)&t;
				}
			}
			std::string ErrMes = "Error : Not Find ID :";
			ErrMes += std::to_string(id);
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			return nullptr;
		}
		const auto&		GetList(void) const noexcept { return this->m_List; }
	};
};