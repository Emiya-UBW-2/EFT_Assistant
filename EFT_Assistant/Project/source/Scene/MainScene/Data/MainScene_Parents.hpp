#pragma once
#include "../../../Header.hpp"
#include "../../../PartsHeader.hpp"

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
		std::string						m_Name_Eng;
		std::string						m_CanUseFileName;
		std::string						m_ShortName;
		std::string						m_FilePath;
		std::array<int, 3>				m_Color{ 0,0,0 };

		std::string						m_Information_Eng;

		std::string						m_Name_Jpn;
		std::string						m_Information_Jpn;

		std::unique_ptr<std::thread>	m_SetJob{ nullptr };
		bool							m_SetFinish{ false };

		Graphs							m_Icon;
	public:
		int														m_CheckJson{ 0 };
	private:
		void			SetCommon(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
			if (LEFT == "IDstr") {
				m_IDstr = Args[0];
			}
			else if (LEFT == "Name") {
				m_Name_Eng = Args[0];
				m_ShortName = Args[0];

				m_CanUseFileName = m_Name_Eng;
				SubStrs(&m_CanUseFileName, ".");
				SubStrs(&m_CanUseFileName, "\\");
				SubStrs(&m_CanUseFileName, "/");
				SubStrs(&m_CanUseFileName, ":");
				SubStrs(&m_CanUseFileName, "*");
				SubStrs(&m_CanUseFileName, "?");
				SubStrs(&m_CanUseFileName, "\"");
				SubStrs(&m_CanUseFileName, ">");
				SubStrs(&m_CanUseFileName, "<");
				SubStrs(&m_CanUseFileName, "|");
			}
			else if (LEFT == "Color") {
				m_Color[0] = std::stoi(Args[0]);
				m_Color[1] = std::stoi(Args[1]);
				m_Color[2] = std::stoi(Args[2]);
			}
			else if (LEFT == "ShortName") {
				m_ShortName = Args[0];
			}
			else if (LEFT == "Information_Eng") {
				m_Information_Eng = Args[0];
			}
			else if (LEFT == "Name_Jpn") {
				m_Name_Jpn = Args[0];
			}
			else if (LEFT == "Information_Jpn") {
				m_Information_Jpn = Args[0];
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
		const auto&		GetName() const noexcept { return this->m_Name_Eng; }
		const auto&		GetCanUseFileName() const noexcept { return this->m_CanUseFileName; }
		const auto&		GetShortName() const noexcept { return this->m_ShortName; }
		const auto&		GetFilePath() const noexcept { return this->m_FilePath; }

		const auto		GetColorRGB(int no) const noexcept { return std::clamp(m_Color[no], 1, 255); }
		const auto		GetColors(int colorAdd) const noexcept { return DxLib::GetColor(std::clamp(m_Color[0] + colorAdd, 1, 255), std::clamp(m_Color[1] + colorAdd, 1, 255), std::clamp(m_Color[2] + colorAdd, 1, 255)); }
		const auto&		GetIcon() const noexcept { return this->m_Icon; }
		void			SetShortName(std::string_view value) noexcept { m_ShortName = value; }

		const auto&		GetName_Jpn() const noexcept { return m_Name_Jpn; }

		const auto&		GetInformation_Eng() const noexcept { return m_Information_Eng; }
		const auto&		GetInformation_Jpn() const noexcept { return m_Information_Jpn; }
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
							//SubStrs(&line2, ",");
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
	template <class List>
	class DataParent {
		std::vector<std::string>	m_ListPathBuffer;
	protected:
		std::vector<List>			m_List;
	protected:
		void			SetDirList(const char* DirPath) noexcept {
			GetDirList(DirPath, [&](const char* RetPath4) {
				std::string Tmp = RetPath4;
				auto txtpos = Tmp.find(".txt");
				if (txtpos != std::string::npos) {
					m_ListPathBuffer.emplace_back(DirPath + Tmp.substr(0, txtpos));
				}
			});
		}
	public:
		void			SetDataList() noexcept {
			m_List.resize(m_ListPathBuffer.size());
			for (auto& d : this->m_ListPathBuffer) {
				int index = (int)(&d - &m_ListPathBuffer.front());
				m_List[index].Set((d + ".txt").c_str(), index, (d + ".png").c_str());
			}
			m_ListPathBuffer.clear();
		}
		void			WaitDataList() noexcept {
			//待ち
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
		void			LoadList(bool IsPushLog) noexcept {
			for (auto&t : this->m_List) {
				t.Load(IsPushLog);
			}
			for (auto& t : this->m_List) {
				t.m_CheckJson = 0;
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
		const int		FindID(std::string_view name) const noexcept {
			if (name != "") {
				for (const auto& t : this->m_List) {
					if (t.GetName() == name) {
						return t.GetID();
					}
				}
			}
			std::string ErrMes = "Error : Not Find Item : ";
			ErrMes += name;
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			return InvalidID;
		}
		List*			FindPtr(int id) const noexcept {
			auto result = std::find_if(this->m_List.begin(), this->m_List.end(), [&](const List& t) {return (t.GetID() == id); });
			if (result != this->m_List.end()) {
				return (List*)&(*result);
			}
			std::string ErrMes = "Error : Not Find ID :";
			ErrMes += std::to_string(id);
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			return nullptr;
		}
		const auto&		GetList(void) const noexcept { return this->m_List; }
		auto&			SetList(void) noexcept { return this->m_List; }
	public:
		void CheckThroughJson(void) noexcept {
			for (auto& L : this->m_List) {
				if (L.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : " + L.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
				else if (L.m_CheckJson >= 2) {
					std::string ErrMes = "Error : Be repeated " + std::to_string(L.m_CheckJson) + " : " + L.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
	public:
		void DrawList(int xp1, int yp1, int xs1, std::string_view Name, int*Select, bool isActive, bool isElseSelect, bool isAllSelect, const std::function<bool(const List*)>& CheckLocal) noexcept {
			auto* WindowMngr = WindowMySystem::WindowManager::Instance();
			int xsize = xs1;
			int ysize = LineHeight + DXDraw::Instance()->GetUIY(6);
			int count = 0;
			//
			{
				int yp_t = yp1;
				if (isAllSelect) {
					yp_t += ysize + DXDraw::Instance()->GetUIY(3);
				}
				for (const auto& L2 : m_List) {
					if (!CheckLocal(&L2)) { continue; }
					yp_t += ysize + DXDraw::Instance()->GetUIY(3);
					count++;
				}
				if (yp_t > DXDraw::Instance()->GetUIY(1080 - 24)) {
					ysize = (DXDraw::Instance()->GetUIY(1080 - 24) - yp1) / (count)-DXDraw::Instance()->GetUIY(3);
				}
			}

			WindowSystem::SetMsg(xp1 + xsize / 2, yp1 + (ysize - DXDraw::Instance()->GetUIY(5)) / 2, ysize - DXDraw::Instance()->GetUIY(5), STRX_MID, White, Black, Name);
			yp1 += ysize;
			int yp_t = yp1;
			if (isAllSelect) {
				yp_t += ysize + DXDraw::Instance()->GetUIY(3);
			}
			//
			int IDBuf = InvalidID;
			bool NotSelect = (*Select == InvalidID);
			for (const auto& L2 : m_List) {
				if (!CheckLocal(&L2)) { continue; }
				IDBuf = L2.GetID();
				bool SelectIt = (*Select == IDBuf);
				auto color = NotSelect ? Gray25 : (SelectIt ? Gray10 : Gray50);
				if (WindowSystem::SetMsgClickBox(xp1 - (SelectIt ? DXDraw::Instance()->GetUIY(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, LineHeight, color, false, (isActive || (!isActive && SelectIt)) && !WindowMngr->PosHitCheck(nullptr), L2.GetShortName().c_str())) {
					*Select = (isActive) ? IDBuf : InvalidID;
				}
				yp_t += ysize + DXDraw::Instance()->GetUIY(3);
			}
			if (count > 0 && isElseSelect) {//その他
				bool ElseSelect = (*Select == ElseSelectID);
				auto color = ElseSelect ? Gray25 : Gray50;
				if (WindowSystem::SetMsgClickBox(xp1 - (ElseSelect ? DXDraw::Instance()->GetUIY(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, LineHeight, color, false, isActive && !WindowMngr->PosHitCheck(nullptr), "Else")) {
					*Select = ElseSelectID;
				}
			}
			//全部選択
			if (isAllSelect) {
				if (count > 1) {
					auto color = NotSelect ? Gray10 : Gray50;
					if (WindowSystem::SetMsgClickBox(xp1 - (NotSelect ? DXDraw::Instance()->GetUIY(25) : 0), yp1, xp1 + xsize, yp1 + ysize, LineHeight, color, false, isActive && !WindowMngr->PosHitCheck(nullptr), "ALL")) {
						*Select = InvalidID;
					}
				}
				else {
					if (IDBuf != InvalidID) {
						*Select = IDBuf;
					}
				}
			}
			if (count == 0) {
				if (WindowSystem::SetMsgClickBox(xp1, yp1, xp1 + xsize, yp1 + ysize, LineHeight, Gray50, false, false, "None")) {}
			}
		};

		void DrawList(int xp1, int yp1, int xs1, std::string_view Name, int*Select, bool isActive, bool isElseSelect, bool isAllSelect) noexcept {
			DrawList(xp1, yp1, xs1, Name, Select, isActive, isElseSelect, isAllSelect, [&](const auto *) { return true; });
		};
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
		void		CheckID(const DataParent<List>* taskList, bool DrawErrorLog = true) noexcept {
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

		void		operator=(const IDParents& tgt) noexcept {
			this->m_Name = tgt.m_Name;
			this->m_ID = tgt.m_ID;
		}
	};

	//該当IDのデータの数値格納
	template <class ID>
	class GetDataParent {
		IDParents<ID>	m_ID;
		int				m_Value{ 0 };
		std::string		m_OutputStr;
	public:
		const auto&		GetOutputStr() const noexcept { return this->m_OutputStr; }
		const auto&		GetName() const noexcept { return this->m_ID.GetName(); }
		const auto&		GetID() const noexcept { return this->m_ID.GetID(); }
		const auto&		GetValue() const noexcept { return this->m_Value; }
		void			Set(std::string_view name, int lv) noexcept {
			m_ID.SetName(name);
			m_Value = lv;
		}

		template <class List>
		void			CheckID(const DataParent<List>* taskList, bool DrawErrorLog = true) noexcept {
			m_ID.CheckID<List>(taskList, DrawErrorLog);

			if (this->GetID() != InvalidID) {
				m_OutputStr = taskList->FindPtr(this->GetID())->GetName() + "x" + std::to_string(this->GetValue());
			}
			else {
				m_OutputStr = this->GetName() + "x" + std::to_string(this->GetValue());
			}
		}

		void		operator=(const GetDataParent& tgt) noexcept {
			this->m_ID = tgt.m_ID;
			this->m_Value = tgt.m_Value;
			this->m_OutputStr = tgt.m_OutputStr;
		}
	};

	//文字列(例)"AAAx1"からAAAと1を分離してデータに格納
	template <class GetDataParentT>
	void			SetGetData(std::vector<GetDataParentT>* Data, const std::string& Arg, const char* DivStr, bool CheckSameName) noexcept {
		auto L = Arg.rfind(DivStr);
		if (L != std::string::npos) {
			auto id = Arg.substr(0, L);
			if (CheckSameName && (std::find_if(Data->begin(), Data->end(), [&](const auto& obj) { return obj.GetName() == id; }) != Data->end())) {
				return;
			}
			Data->resize(Data->size() + 1);
			Data->back().Set(id, std::stoi(Arg.substr(L + strlenDx(DivStr))));
		}
	};

	//Json読み取り
	class JpJsonData {
	public:
		bool										m_IsFileOpened{ false };
		std::string									m_Path;

		std::unique_ptr<std::thread>				m_SetJob{ nullptr };
		bool										m_SetFinish{ false };
	public:
		std::string									m_id;
		std::string									m_name_jp;
		std::string									m_CanUseFileNameJP;
		std::string									m_Information_Jpn;
		std::string									m_categorytypes;
	public:
		const auto&	Getname_jp() const noexcept { return this->m_name_jp; }
		const auto&	Getdescription_jp() const noexcept { return this->m_Information_Jpn; }
	public:
		void GetJson(const nlohmann::json& data) noexcept {
			m_IsFileOpened = false;

			m_id = data["id"];
			m_name_jp = data["name"];
			m_CanUseFileNameJP = m_name_jp;
			SubStrs(&m_CanUseFileNameJP, ".");
			SubStrs(&m_CanUseFileNameJP, "\\");
			SubStrs(&m_CanUseFileNameJP, "/");
			SubStrs(&m_CanUseFileNameJP, ":");
			SubStrs(&m_CanUseFileNameJP, "*");
			SubStrs(&m_CanUseFileNameJP, "?");
			SubStrs(&m_CanUseFileNameJP, "\"");
			SubStrs(&m_CanUseFileNameJP, ">");
			SubStrs(&m_CanUseFileNameJP, "<");
			SubStrs(&m_CanUseFileNameJP, "|");


			m_Information_Jpn = "";
			if (data.contains("description")) {
				if (!data["description"].is_null()) {
					m_Information_Jpn = data["description"];
				}
			}
			m_categorytypes = data["category"]["normalizedName"];
		}
		void OutputData(const std::string& Path) noexcept {
			m_IsFileOpened = true;
			m_Path = Path;

			SubStrs(&m_Path, ":");
			SubStrs(&m_Path, "*");
			SubStrs(&m_Path, "?");
			SubStrs(&m_Path, "\"");
			SubStrs(&m_Path, ">");
			SubStrs(&m_Path, "<");
			SubStrs(&m_Path, "|");
			SubStrs(&m_Path, "|");

			m_SetFinish = false;
			ResetDataJob();
			m_SetJob = std::make_unique<std::thread>([&]() {
				std::ofstream outputfile(m_Path, std::ios::app);
				outputfile << "Name_Jpn=" + this->m_name_jp + "\n";
				outputfile << "Information_Jpn=" + this->m_Information_Jpn + "\n";
				outputfile.close();
				m_SetFinish = true;
			});
		}
		void ResetDataJob() noexcept {
			if (m_SetJob) {
				m_SetJob->join();
				m_SetJob.release();
			}
		}
		const auto&		GetIsSetFinish() const noexcept { return this->m_SetFinish; }
	};
	class JsonDataParent {
	public:
		bool										m_IsFileOpened{ false };
		std::string									m_Path;

		std::unique_ptr<std::thread>	m_SetJob{ nullptr };
		bool							m_SetFinish{ false };
	public:
		std::string									m_id;
		std::string									m_name;
		std::string									m_CanUseFileName;
		std::string									m_shortName;
		std::string									m_Information_Eng;
	public:
		virtual void	GetJsonSub(const nlohmann::json&) noexcept {}
		virtual void	OutputDataSub(std::ofstream&) noexcept {}
	public:
		void GetJson(const nlohmann::json& data) noexcept {
			m_IsFileOpened = false;
			m_id = data["id"];
			if (data.contains("name") && !data["name"].is_null()) {
				m_name = data["name"];
			}
			else {
				m_name = m_id;
			}
			m_CanUseFileName = m_name;
			SubStrs(&m_CanUseFileName, ".");
			SubStrs(&m_CanUseFileName, "\\");
			SubStrs(&m_CanUseFileName, "/");
			SubStrs(&m_CanUseFileName, ":");
			SubStrs(&m_CanUseFileName, "*");
			SubStrs(&m_CanUseFileName, "?");
			SubStrs(&m_CanUseFileName, "\"");
			SubStrs(&m_CanUseFileName, ">");
			SubStrs(&m_CanUseFileName, "<");
			SubStrs(&m_CanUseFileName, "|");

			if (data.contains("shortName") && !data["shortName"].is_null()) {
				m_shortName = data["shortName"];
			}
			else {
				m_shortName = "";
			}
			if (data.contains("description") && !data["description"].is_null()) {
				m_Information_Eng = data["description"];
			}
			else {
				m_Information_Eng = "";
			}
			GetJsonSub(data);
		}
		void OutputData(const std::string& Path) noexcept {
			m_IsFileOpened = true;
			m_Path = Path;

			SubStrs(&m_Path, ":");
			SubStrs(&m_Path, "*");
			SubStrs(&m_Path, "?");
			SubStrs(&m_Path, "\"");
			SubStrs(&m_Path, ">");
			SubStrs(&m_Path, "<");
			SubStrs(&m_Path, "|");

			m_SetFinish = false;
			ResetDataJob();
			m_SetJob = std::make_unique<std::thread>([&]() {
				std::ofstream outputfile(m_Path);
				outputfile << "IDstr=" + this->m_id + "\n";
				outputfile << "Name=" + this->m_name + "\n";
				if (this->m_shortName != "") {
					outputfile << "ShortName=" + this->m_shortName + "\n";
				}
				if (this->m_Information_Eng != "") {
					outputfile << "Information_Eng=" + this->m_Information_Eng + "\n";
				}
				OutputDataSub(outputfile);
				outputfile.close();
				m_SetFinish = true;
			});
		}
		void ResetDataJob() noexcept {
			if (m_SetJob) {
				m_SetJob->join();
				m_SetJob.release();
			}
		}
		const auto&		GetIsSetFinish() const noexcept { return this->m_SetFinish; }
	};
	template <class JsonDataParentT>
	class JsonListParent {
		std::vector<std::unique_ptr<JsonDataParent>>	m_JsonData;
		std::vector<std::unique_ptr<JpJsonData>> m_JpJsonData;
	public:
		auto&		GetJsonDataList() noexcept { return this->m_JsonData; }
		void ResetDataJson() {
			m_JsonData.clear();
		}
		virtual void InitDatabyJson() noexcept {}
		void GetDataJson(nlohmann::json& data) {
			for (const auto& d : data) {
				if (d.is_null()) { continue; }
				m_JsonData.emplace_back(std::make_unique<JsonDataParentT>());
				m_JsonData.back()->GetJson(d);
			}
		}
		template <class List>
		void UpdateData(int ofset, int size, std::vector<List>& ListT) noexcept {
			for (auto& L : ListT) {
				for (int loop = ofset; loop < ofset + size; loop++) {
					if (loop >= (int)this->m_JsonData.size()) { break; }
					auto& jd = this->m_JsonData.at(loop);
					if (L.GetIDstr() == jd->m_id) {
						L.m_CheckJson++;
						jd->OutputData(L.GetFilePath());
						break;
					}
				}
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

					std::string Name = jd->m_CanUseFileName + ".txt";
					ReplaceStrs(&Name, ".txt.txt", ".txt");

					jd->OutputData(ChildPath + Name);
					//RemoveDirectory(Path.c_str());
				}
			}
		}
		void WaitToAllClear() noexcept {
			for(int i=0;i<10000;i++) {
				bool isHit = false;
				for (auto& jd : this->m_JsonData) {
					if (!jd->GetIsSetFinish()) {
						isHit = true;
						break;
					}
					else if (i == 10000 - 1) {
						std::string ErrMes = "Error : Job Not End : ";
						ErrMes += jd->m_Path;
						DataErrorLog::Instance()->AddLog(ErrMes.c_str());
					}
				}
				if (!isHit) { break; }
			}
			DataErrorLog::Instance()->Save();
			for (auto& jd : this->m_JsonData) {
				jd->ResetDataJob();
			}
		}
		virtual void UpdateAfterbyJson(void) noexcept {}
	public:
		auto&		GetJpJsonDataList() noexcept { return this->m_JpJsonData; }
		void InitJpDatabyJson() {
			m_JpJsonData.clear();
		}
		void GetJpDataJson(nlohmann::json& data) {
			for (const auto& d : data) {
				m_JpJsonData.emplace_back(std::make_unique<JpJsonData>());
				m_JpJsonData.back()->GetJson(d);
			}
		}
		template <class List>
		void UpdateJpData(int ofset, int size, std::vector<List>& ListT) noexcept {
			for (auto& L : ListT) {
				for (int loop = ofset; loop < ofset + size; loop++) {
					if (loop >= (int)m_JpJsonData.size()) { break; }
					auto& jd = m_JpJsonData.at(loop);
					if (L.GetIDstr() == jd->m_id) {
						L.m_CheckJson++;
						jd->OutputData(L.GetFilePath());
						break;
					}
				}
			}
		}
		void SaveAsNewJpData(std::string Path) noexcept {
			bool maked = false;
			for (auto& jd : m_JpJsonData) {
				if (!jd->m_IsFileOpened) {
					std::string ParentPath = Path + jd->m_categorytypes;

					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}

					std::string ChildPath = ParentPath + "/";

					std::string Name = jd->m_CanUseFileNameJP + std::to_string((int)(&jd - &m_JpJsonData.front())) + ".txt";

					jd->OutputData(ChildPath + Name);
					//RemoveDirectory(Path.c_str());
				}
			}
		}
		void WaitToAllClearJp() noexcept {
			while (true) {
				bool isHit = false;
				for (auto& jd : m_JpJsonData) {
					if (!jd->GetIsSetFinish()) {
						isHit = true;
						break;
					}
				}
				if (!isHit) { break; }
			}
			for (auto& jd : m_JpJsonData) {
				jd->ResetDataJob();
			}
		}
		//
	};
};
