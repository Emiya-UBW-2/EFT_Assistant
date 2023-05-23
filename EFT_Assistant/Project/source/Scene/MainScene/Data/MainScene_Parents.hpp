#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	static const int InvalidID{ -1 };
	static const int ElseSelectID{ -2 };
	static const auto STRX_LEFT{ FontHandle::FontXCenter::LEFT };
	static const auto STRX_MID{ FontHandle::FontXCenter::MIDDLE };
	static const auto STRX_RIGHT{ FontHandle::FontXCenter::RIGHT };
	static const auto STRY_TOP{ FontHandle::FontYCenter::TOP };
	static const auto STRY_MIDDLE{ FontHandle::FontYCenter::MIDDLE };
	static const auto STRY_BOTTOM{ FontHandle::FontYCenter::BOTTOM };
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
				WindowSystem::SetMsg(xp, yp, xp, yp, LineHeight * 7 / 10, STRX_LEFT, GetColor(255, 150, 150), GetColor(1, 1, 1), m);
				yp += LineHeight * 7 / 10;
				if (yp > y_r(1080)) { break; }
			}
		}
		void AddLog(const char* Mes) noexcept {
			m_Mes.emplace_back(Mes);
		}
		void Save() noexcept {
			std::ofstream outputfile("data/ErrorLog.txt");
			for (auto& LD : m_Mes) {
				outputfile << LD + "\n";
			}
			outputfile.close();
		}
	};
	//
	template <class ID>
	class ListParent {
	private:
		ID						m_ID{ InvalidID };
		std::string				m_IDstr;
		std::string				m_Name;
		std::string				m_ShortName;
		std::string				m_FilePath;
		std::array<int, 3>		m_Color{ 0,0,0 };

		Graphs					m_Icon;
	private:
		void			SetCommon(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
			if (LEFT == "IDstr") {
				m_IDstr = RIGHT;
			}
			if (LEFT == "Name") {
				m_Name = RIGHT;
				m_ShortName = RIGHT;
			}
			else if (LEFT == "Color") {
				m_Color[0] = std::stoi(Args[0]);
				m_Color[1] = std::stoi(Args[1]);
				m_Color[2] = std::stoi(Args[2]);
			}
			else if (LEFT == "ShortName") {
				m_ShortName = RIGHT;
			}
		}
	protected:
		virtual void	Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept {}
		virtual void	Load_Sub() noexcept {}
		virtual void	WhenAfterLoad_Sub() noexcept {}
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetIDstr() const noexcept { return m_IDstr; }
		const auto&		GetName() const noexcept { return m_Name; }
		const auto&		GetShortName() const noexcept { return m_ShortName; }
		const auto&		GetFilePath() const noexcept { return m_FilePath; }

		const auto		GetColors(int colorAdd) const noexcept {
			return DxLib::GetColor(std::clamp(m_Color[0] + colorAdd, 1, 255), std::clamp(m_Color[1] + colorAdd, 1, 255), std::clamp(m_Color[2] + colorAdd, 1, 255));
		}
		const auto&		GetIcon() const noexcept { return m_Icon; }
	public:
		void			Set(const char* FilePath, ID id, const char* IconPath = nullptr) noexcept {
			m_ID = id;
			int mdata = FileRead_open(FilePath, FALSE);
			m_FilePath = FilePath;
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
				if (RIGHT == "[") {
					std::vector<std::string> Args;
					while (true) {
						if (FileRead_eof(mdata) != 0) { break; }
						auto ALL2 = getparams::Getstr(mdata);
						if (ALL2.find("]") != std::string::npos) { break; }
						{
							auto ALL2Len = ALL2.find("\t");
							if (ALL2Len != std::string::npos) {
								ALL2 = ALL2.substr(0, ALL2Len) + ALL2.substr(ALL2Len + 1);
							}
						}
						{
							auto ALL2Len = ALL2.find(",");
							if (ALL2Len != std::string::npos) {
								ALL2 = ALL2.substr(0, ALL2Len) + ALL2.substr(ALL2Len + 1);
							}
						}
						Args.emplace_back(ALL2);
					}
					SetCommon(LEFT, RIGHT, Args);
					Set_Sub(LEFT, RIGHT, Args);
				}
				else {
					auto Args = GetArgs(RIGHT);
					SetCommon(LEFT, RIGHT, Args);
					Set_Sub(LEFT, RIGHT, Args);
				}
			}
			FileRead_close(mdata);
			if (IconPath) {
				m_Icon.SetPath(IconPath);
			}
		}
		void			Load(bool IsPushLog) noexcept {
			if (!m_Icon.LoadByPath(false) && IsPushLog) {
				std::string ErrMes = "Error : Not Find Image : ";
				ErrMes += this->GetName();
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
			Load_Sub();
		}
		void			WhenAfterLoadCommon() noexcept {
			m_Icon.WhenAfterLoad();
		}
		void			WhenAfterLoad() noexcept {
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
			std::vector<std::string> DirNames;
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Tmp = d.cFileName;
					auto txtpos = Tmp.find(".txt");
					if (txtpos != std::string::npos) {
						DirNames.emplace_back(Tmp.substr(0, txtpos));
					}
				}
			}
			int baseIndex = (int)m_List.size();
			m_List.resize(baseIndex + DirNames.size());
			for (auto& d : DirNames) {
				int index = (int)(&d - &DirNames.front()) + baseIndex;
				m_List[index].Set((DirPath + d + ".txt").c_str(), (ID)index, (DirPath + d + ".png").c_str());
			}
			DirNames.clear();

		}
	public:
		void LoadList(bool IsPushLog) noexcept {
			for (auto&t : m_List) {
				t.Load(IsPushLog);
			}
		}
		void WhenAfterLoadListCommon(void) noexcept {
			for (auto&t : m_List) {
				t.WhenAfterLoadCommon();
			}
		}
		void WhenAfterLoadList(void) noexcept {
			for (auto&t : m_List) {
				t.WhenAfterLoad();
			}
		}
	public:
		const ID		FindID(std::string_view name) const noexcept {
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
		auto&			SetList(void) noexcept { return this->m_List; }
	};
	//
	template <class ID>
	class GetDataParent {
		std::string m_name;
		int			m_Value{ 0 };
	public:
		const auto&		GetName() const noexcept { return m_name; }
		const auto&		GetValue() const noexcept { return m_Value; }
		void			Set(const std::string& name, int lv) noexcept {
			m_name = name;
			m_Value = lv;
		}
	};
	template <class GetDataParent, class DataParent>
	void			SetGetData(std::vector<GetDataParent>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto id = mes.substr(0, L);
			if (std::find_if(Data->begin(), Data->end(), [&](const GetDataParent& obj) {return obj.GetName() == id; }) == Data->end()) {
				GetDataParent tmp;
				tmp.Set(id, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};
};
