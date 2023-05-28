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
		ID								m_ID{ InvalidID };
		std::string						m_IDstr;
		std::string						m_Name;
		std::string						m_ShortName;
		std::string						m_FilePath;
		std::array<int, 3>				m_Color{ 0,0,0 };

		std::unique_ptr<std::thread>	m_SetJob{ nullptr };
		bool							m_SetFinish{ false };

		Graphs							m_Icon;
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
		const auto&		GetIsSetFinish() const noexcept { return this->m_SetFinish; }
		const auto&		GetID() const noexcept { return this->m_ID; }
		const auto&		GetIDstr() const noexcept { return this->m_IDstr; }
		const auto&		GetName() const noexcept { return this->m_Name; }
		const auto&		GetShortName() const noexcept { return this->m_ShortName; }
		const auto&		GetFilePath() const noexcept { return this->m_FilePath; }

		const auto		GetColorRGB(int no) const noexcept { return std::clamp(m_Color[no], 1, 255); }
		const auto		GetColors(int colorAdd) const noexcept { return DxLib::GetColor(std::clamp(m_Color[0] + colorAdd, 1, 255), std::clamp(m_Color[1] + colorAdd, 1, 255), std::clamp(m_Color[2] + colorAdd, 1, 255)); }
		const auto&		GetIcon() const noexcept { return this->m_Icon; }
	public:
		void			Set(const char* FilePath, ID id, const char* IconPath = nullptr) noexcept {
			m_FilePath = FilePath;
			m_ID = id;

			m_SetFinish = false;
			m_SetJob = std::make_unique<std::thread>([&]() {
				std::ifstream File(m_FilePath);
				std::string line;
				while (std::getline(File, line)) {
					SubRIGHTStrs(&line, "//");				//コメントアウト
					//
					if (line == "") { continue; }
					auto RIGHT = getparams::getright(line);
					if (RIGHT == "[") {
						std::vector<std::string> Args;
						std::string line2;
						while (std::getline(File, line2)) {
							if (line2.find("]") != std::string::npos) { break; }
							SubRIGHTStrs(&line2, "//");				//コメントアウト
							SubStrs(&line2, "\t");
							SubStrs(&line2, DIV_STR);
							SubStrs(&line2, ",");
							Args.emplace_back(line2);
						}
						SetCommon(getparams::getleft(line), Args);
					}
					else {
						SetCommon(getparams::getleft(line), GetArgs(RIGHT));
					}
				}
				File.close();
				m_SetFinish = true;
			});

			if (IconPath) {
				m_Icon.SetPath(IconPath);
			}
		}
		void			Load(bool IsPushLog) noexcept {
			m_SetJob->detach();
			m_SetJob.release();

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
			while (true) {
				bool isHit = false;
				for (auto&t : this->m_List) {
					if (!t.GetIsSetFinish()) {
						isHit = true;
						break;
					}
				}
				if (!isHit) { break; }
			}
		}
	public:
		void			LoadList(bool IsPushLog) noexcept {
			for (auto&t : this->m_List) {
				t.Load(IsPushLog);
			}
		}
		void			WhenAfterLoadListCommon(void) noexcept {
			for (auto&t : this->m_List) {
				t.WhenAfterLoadCommon();
			}
		}
		void			WhenAfterLoadList(void) noexcept {
			for (auto&t : this->m_List) {
				t.WhenAfterLoad();
			}
		}
	public:
		const ID		FindID(std::string_view name) const noexcept {
			for (const auto&t : this->m_List) {
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
			for (auto&t : this->m_List) {
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
	class IDParents {
		std::string								m_Name;
		ID										m_ID{ InvalidID };
	public:
		const auto& GetName() const noexcept { return this->m_Name; }
		const auto& GetID() const noexcept { return this->m_ID; }
	public:
		void		SetName(std::string_view value) noexcept { this->m_Name = value; }
		void		SetID(ID value) noexcept { this->m_ID = value; }
	public:
		template <class List>
		void		CheckID(const DataParent<ID, List>* taskList, bool DrawErrorLog = true) noexcept {
			bool isHit = false;
			for (const auto& t : taskList->GetList()) {
				if (m_Name == t.GetName()) {
					m_ID = t.GetID();
					isHit = true;
					return;
				}
			}
			if (!isHit && DrawErrorLog) {
				std::string ErrMes = "Error : Invalid ID by CheckID";
				ErrMes += "[";
				ErrMes += this->m_Name;
				ErrMes += "]";
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
	};

	//該当IDのデータの数値格納
	template <class ID>
	class GetDataParent {
		IDParents<ID>	m_ID;
		int				m_Value{ 0 };
	public:
		const auto&		GetName() const noexcept { return this->m_ID.GetName(); }
		const auto&		GetID() const noexcept { return this->m_ID.GetID(); }
		const auto&		GetValue() const noexcept { return this->m_Value; }
		void			Set(std::string_view name, int lv) noexcept {
			m_ID.SetName(name);
			m_Value = lv;
		}

		template <class List>
		void			CheckID(const DataParent<ID, List>* taskList, bool DrawErrorLog = true) noexcept {
			m_ID.CheckID<List>(taskList, DrawErrorLog);
		}
	};

	//文字列(例)"AAAx1"からAAAと1を分離してデータに格納
	template <class GetDataParentT>
	void			SetGetData(std::vector<GetDataParentT>* Data, const std::string& Arg, const char* DivStr) noexcept {
		auto L = Arg.rfind(DivStr);
		if (L != std::string::npos) {
			auto id = Arg.substr(0, L);
			if (std::find_if(Data->begin(), Data->end(), [&](const auto& obj) { return obj.GetName() == id; }) == Data->end()) {
				Data->resize(Data->size() + 1);
				Data->back().Set(id, std::stoi(Arg.substr(L + strlenDx(DivStr))));
			}
		}
	};

	//Json読み取り
	class JsonDataParent {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									m_id;
		std::string									m_name;
		std::string									m_shortName;
		std::string									m_description;
	public:
		virtual void	GetJsonSub(const nlohmann::json&) noexcept {}
		virtual void	OutputDataSub(std::ofstream&) noexcept {}
	public:
		void GetJson(const nlohmann::json& data) noexcept {
			m_IsFileOpened = false;
			m_id = data["id"];
			m_name = data["name"];

			m_shortName = "";
			if (data.contains("shortName")) {
				if (!data["shortName"].is_null()) {
					m_shortName = data["shortName"];
				}
			}
			m_description = "";
			if (data.contains("description")) {
				if (!data["description"].is_null()) {
					m_description = data["description"];
				}
			}

			GetJsonSub(data);
		}
		void OutputData(const std::string& Path) noexcept {
			m_IsFileOpened = true;
			std::ofstream outputfile(Path);
			outputfile << "IDstr=" + this->m_id + "\n";
			outputfile << "Name=" + this->m_name + "\n";
			if (this->m_shortName != "") {
				outputfile << "ShortName=" + this->m_shortName + "\n";
			}
			if (this->m_description != "") {
				outputfile << "Information_Eng=" + this->m_description + "\n";
			}
			OutputDataSub(outputfile);
			outputfile.close();
		}
	};
	template <class JsonDataParentT>
	class JsonListParent {
		std::vector<std::unique_ptr<JsonDataParent>> m_JsonData;
	public:
		auto&		GetJsonDataList() noexcept { return this->m_JsonData; }
		void GetDataJson(nlohmann::json& data, std::string ListName) {
			m_JsonData.clear();
			for (const auto& d : data["data"][ListName]) {
				m_JsonData.emplace_back(std::make_unique<JsonDataParentT>());
				m_JsonData.back()->GetJson(d);
			}
		}
		void SaveAsNewData(std::string ParentPath) noexcept {
			bool maked = false;
			for (auto& jd : this->m_JsonData) {
				if (!jd->m_IsFileOpened) {
					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}

					std::string ChildPath = ParentPath + "/";

					std::string FileName = jd->m_name;
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
