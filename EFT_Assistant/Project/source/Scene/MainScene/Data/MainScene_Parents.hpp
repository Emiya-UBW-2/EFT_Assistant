#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	static const int InvalidID{ -1 };
	static const int ElseSelectID{ -2 };
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
		void			SetCommon(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
			if (LEFT == "IDstr") {
				m_IDstr = Args[0];
			}
			else if (LEFT == "Name") {
				m_Name = Args[0];
				m_ShortName = Args[0];
			}
			else if (LEFT == "Color") {
				m_Color[0] = std::stoi(Args[0]);
				m_Color[1] = std::stoi(Args[1]);
				m_Color[2] = std::stoi(Args[2]);
			}
			else if (LEFT == "ShortName") {
				m_ShortName = Args[0];
			}
			SetSub(LEFT, Args);
		}
	protected:
		virtual void	SetSub(const std::string&, const std::vector<std::string>&) noexcept {}
		virtual void	Load_Sub() noexcept {}
		virtual void	WhenAfterLoad_Sub() noexcept {}
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetIDstr() const noexcept { return m_IDstr; }
		const auto&		GetName() const noexcept { return m_Name; }
		const auto&		GetShortName() const noexcept { return m_ShortName; }
		const auto&		GetFilePath() const noexcept { return m_FilePath; }

		const auto		GetColorRGB(int no) const noexcept { return std::clamp(m_Color[no], 1, 255); }
		const auto		GetColors(int colorAdd) const noexcept { return DxLib::GetColor(std::clamp(m_Color[0] + colorAdd, 1, 255), std::clamp(m_Color[1] + colorAdd, 1, 255), std::clamp(m_Color[2] + colorAdd, 1, 255)); }
		const auto&		GetIcon() const noexcept { return m_Icon; }
	public:
		void			Set(const char* FilePath, ID id, const char* IconPath = nullptr) noexcept {
			m_ID = id;
			int mdata = FileRead_open(FilePath, FALSE);
			m_FilePath = FilePath;
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				SubRIGHTStrs(&ALL, "//");				//コメントアウト
				//
				if (ALL == "") { continue; }
				auto RIGHT = getparams::getright(ALL);
				if (RIGHT == "[") {
					std::vector<std::string> Args;
					while (true) {
						if (FileRead_eof(mdata) != 0) { break; }
						auto ALL2 = getparams::Getstr(mdata);
						if (ALL2.find("]") != std::string::npos) { break; }
						SubRIGHTStrs(&ALL2, "//");				//コメントアウト
						SubStrs(&ALL2, "\t");
						SubStrs(&ALL2, DIV_STR);
						SubStrs(&ALL2, ",");
						Args.emplace_back(ALL2);
					}
					SetCommon(getparams::getleft(ALL), Args);
				}
				else {
					SetCommon(getparams::getleft(ALL), GetArgs(RIGHT));
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
		void			SetDirList(const char* DirPath) noexcept {
			std::vector<std::string> DirNames;

			GetDirList(DirPath, [&](const char* RetPath4) {
				std::string Tmp = RetPath4;
				auto txtpos = Tmp.find(".txt");
				if (txtpos != std::string::npos) {
					DirNames.emplace_back(DirPath + Tmp.substr(0, txtpos));
				}
			});
			int baseIndex = (int)m_List.size();
			m_List.resize(baseIndex + DirNames.size());
			for (auto& d : DirNames) {
				int index = (int)(&d - &DirNames.front()) + baseIndex;
				m_List[index].Set((d + ".txt").c_str(), (ID)index, (d + ".png").c_str());
			}
			DirNames.clear();
		}
	public:
		void			LoadList(bool IsPushLog) noexcept {
			for (auto&t : m_List) {
				t.Load(IsPushLog);
			}
		}
		void			WhenAfterLoadListCommon(void) noexcept {
			for (auto&t : m_List) {
				t.WhenAfterLoadCommon();
			}
		}
		void			WhenAfterLoadList(void) noexcept {
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
		List*			FindPtr(ID id) const noexcept {
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
	template <class ID, class List>
	class IDParents {
		std::string								m_Name;
		ID										m_ID{ InvalidID };
	public:
		const auto& GetName() const noexcept { return m_Name; }
		const auto& GetID() const noexcept { return m_ID; }
	public:
		void		SetName(std::string_view value) noexcept { m_Name = value; }
		void		SetID(ID value) noexcept { m_ID = value; }
	public:
		void		CheckID(const std::vector<List>& taskList, bool DrawErrorLog=true) noexcept {
			bool isHit = false;
			for (const auto& t : taskList) {
				if (m_Name == t.GetName()) {
					m_ID = t.GetID();
					isHit = true;
					break;
				}
			}
			if (!DrawErrorLog) { return; }
			if (!isHit) {
				std::string ErrMes = "Error : Invalid ID by CheckID";
				ErrMes += "[";
				ErrMes += m_Name;
				ErrMes += "]";
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
	};

	//該当IDのデータの数値格納
	template <class ID>
	class GetDataParent {
		ID			m_ID;
		int			m_Value{ 0 };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetValue() const noexcept { return m_Value; }
		void			Set(ID id, int lv) noexcept {
			m_ID = id;
			m_Value = lv;
		}
	};

	//文字列(例)"AAAx1"からAAAと1を分離してデータに格納
	template <class GetDataParentT, class DataParentT>
	void			SetGetData(std::vector<GetDataParentT>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto id = mes.substr(0, L);
			if (
				std::find_if(Data->begin(), Data->end(), [&](const GetDataParentT& obj) {
				return DataParentT::Instance()->FindPtr(obj.GetID())->GetName() == id;
			}) == Data->end()
				) {
				auto ID = DataParentT::Instance()->FindID(id);
				if (ID != InvalidID) {
					GetDataParentT tmp;
					tmp.Set(ID, std::stoi(mes.substr(L + 1)));
					Data->emplace_back(tmp);
				}
				else {

				}
			}
		}
		else {
			//int a = 0;
		}
	};

	//Json読み取り
	class JsonDataParent {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
	public:
		virtual void	GetJsonSub(const nlohmann::json&) noexcept {}
		virtual void	OutputDataSub(std::ofstream&) noexcept {}
	public:
		void GetJson(const nlohmann::json& data) noexcept {
			m_IsFileOpened = false;
			id = data["id"];
			name = data["name"];
			GetJsonSub(data);
		}
		void OutputData(const std::string& Path) noexcept {
			m_IsFileOpened = true;
			std::ofstream outputfile(Path);
			outputfile << "IDstr=" + id + "\n";
			outputfile << "Name=" + name + "\n";
			OutputDataSub(outputfile);
			outputfile.close();
		}
	};
	template <class JsonDataParentT>
	class JsonListParent {
		std::vector<std::unique_ptr<JsonDataParent>> m_JsonData;
	public:
		auto&		GetJsonDataList() noexcept { return m_JsonData; }
		void GetDataJson(nlohmann::json& data, std::string ListName) {
			m_JsonData.clear();
			for (const auto& d : data["data"][ListName]) {
				m_JsonData.emplace_back(std::make_unique<JsonDataParentT>());
				m_JsonData.back()->GetJson(d);
			}
		}
		void SaveAsNewData(std::string ParentPath) noexcept {
			bool maked = false;
			for (auto& jd : m_JsonData) {
				if (!jd->m_IsFileOpened) {
					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}

					std::string ChildPath = ParentPath + "/";

					std::string FileName = jd->name;
					SubStrs(&FileName, ".");
					SubStrs(&FileName, "\\");
					SubStrs(&FileName, "/");
					SubStrs(&FileName, ":");
					SubStrs(&FileName, "*");
					SubStrs(&FileName, "?");
					SubStrs(&FileName, "\"");
					SubStrs(&FileName, ">");
					SubStrs(&FileName, "<");
					SubStrs(&FileName, "|");
					std::string Name = FileName + ".txt";

					jd->OutputData(ChildPath + Name);
					//RemoveDirectory(Path.c_str());
				}
			}
		}
	};
};
