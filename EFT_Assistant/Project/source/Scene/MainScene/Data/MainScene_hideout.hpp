#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int HideoutID;
	//
	class HideoutGetData {
		std::string			m_Name;
		int					m_Lv{ 0 };
	public:
		const auto&		GetID() const noexcept { return m_Name; }
		const auto&		GetLv() const noexcept { return m_Lv; }
		void			Set(const std::string& name, int lv) noexcept {
			m_Name = name;
			m_Lv = lv;
		}
	};
	void			SetHideoutLv(std::vector<HideoutGetData>* Data, const std::string& mes) noexcept;
	//
	struct LvData {
		struct CraftData {
			int											durationTime{ 0 };
			std::vector<ItemGetData>					m_ItemReq;
			std::vector<ItemGetData>					m_ItemReward;
		};
		//開放データ
		int											constructionTime{ 0 };
		std::string									Information_Eng;
		std::vector<ItemGetData>					m_ItemReq;
		std::vector<TraderGetData>					m_TraderReq;
		std::vector<HideoutGetData>					m_Parent;
		std::vector<HideoutGetData>					m_Child;
		//クラフトデータ
		std::vector<CraftData>						m_ItemCraft;
	};
	//
	class HideoutList : public ListParent<HideoutID> {
		std::vector<LvData> m_LvData;
		int m_DrawWindowLv{ 1 };
	private:
		//追加設定
		void	Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override {
			std::string LEFTBuf = LEFT.substr(3);
			std::string NumBuf2 = LEFT.substr(2, 1);
			int ID = 0;
			if (std::all_of(NumBuf2.cbegin(), NumBuf2.cend(), isdigit)) {
				ID = std::stoi(NumBuf2) - 1;
				if (m_LvData.size() <= ID) { m_LvData.resize(ID + 1); }
				//開放データ
				if (LEFTBuf == "constructionTime") { m_LvData.at(ID).constructionTime = std::stoi(RIGHT); }
				if (LEFTBuf == "Information_Eng") { m_LvData.at(ID).Information_Eng = RIGHT; }
				if (LEFTBuf == "itemReq") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_LvData.at(ID).m_ItemReq, a);
							}
						}
					}
					else {
						SetItem(&m_LvData.at(ID).m_ItemReq, RIGHT);
					}
				}
				if (LEFTBuf == "stationLevelReq") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetHideoutLv(&m_LvData.at(ID).m_Parent, a);
							}
						}
					}
					else {
						SetHideoutLv(&m_LvData.at(ID).m_Parent, RIGHT);
					}
				}
				if (LEFTBuf == "traderRequirements") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetTraderLv(&m_LvData.at(ID).m_TraderReq, a);
							}
						}
					}
					else {
						SetTraderLv(&m_LvData.at(ID).m_TraderReq, RIGHT);
					}
				}
				//クラフトレシピ
				if (LEFTBuf == "craftduration") {
					m_LvData.at(ID).m_ItemCraft.resize(m_LvData.at(ID).m_ItemCraft.size() + 1);
					m_LvData.at(ID).m_ItemCraft.back().durationTime = std::stoi(RIGHT);
				}
				if (LEFTBuf == "craftitemReq") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReq, a);
							}
						}
					}
					else {
						SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReq, RIGHT);
					}
				}
				if (LEFTBuf == "craftitemReward") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReward, a);
							}
						}
					}
					else {
						SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReward, RIGHT);
					}
				}
				//
			}
		}
		void	Load_Sub() noexcept override {
		
		}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		int										m_CheckJson{ 0 };
	public:
		const auto&	GetLvData() const noexcept { return m_LvData; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) noexcept;
		void			DrawUnlockWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
		void			DrawCraftWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs = nullptr, int* ys = nullptr, int size = 10) noexcept;
		void			SetOtherPartsID(const std::vector<HideoutList>& HideoutList) noexcept {
			for (auto& D : m_LvData) {
				int DLv = (int)(&D - &m_LvData.front()) + 1;
				D.m_Child.clear();
				for (const auto&L : HideoutList) {
					for (const auto& C : L.GetLvData()) {
						int Lv = (int)(&C - &L.GetLvData().front()) + 1;
						for (const auto& P : C.m_Parent) {
							if (
								(GetName() == P.GetID()) && (DLv == P.GetLv())
								) {
								//自分が子のパーツの親です
								HideoutGetData buf;
								buf.Set(L.GetName(), Lv);
								D.m_Child.emplace_back(buf);
							}
						}
					}
				}
			}
			//
		}
	};
	//
	class HideoutJsonData {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
		std::vector<LvData>							m_LvData;
	public:
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];

			m_LvData.clear();
			for (auto& Ld : data["levels"]) {
				m_LvData.resize(m_LvData.size() + 1);
				auto& L = m_LvData.back();
				L.constructionTime = Ld["constructionTime"];
				L.Information_Eng = Ld["description"];
				L.m_ItemReq.clear();
				if (Ld.contains("itemRequirements")) {
					if (!Ld["itemRequirements"].is_null()) {
						for (const auto&m : Ld["itemRequirements"]) {
							ItemGetData buf;
							std::string Name = m["item"]["name"];
							buf.Set(ItemData::Instance()->FindID(Name.c_str()), m["count"]);
							L.m_ItemReq.emplace_back(buf);
						}
					}
				}
				L.m_Parent.clear();
				if (Ld.contains("stationLevelRequirements")) {
					if (!Ld["stationLevelRequirements"].is_null()) {
						for (const auto&m : Ld["stationLevelRequirements"]) {
							HideoutGetData buf;
							buf.Set(m["station"]["name"], m["level"]);
							L.m_Parent.emplace_back(buf);
						}
					}
				}
				L.m_TraderReq.clear();
				if (Ld.contains("traderRequirements")) {
					if (!Ld["traderRequirements"].is_null()) {
						for (const auto&m : Ld["traderRequirements"]) {
							TraderGetData buf;
							buf.Set(m["trader"]["name"], m["level"]);//todo:levelが廃止予定
							L.m_TraderReq.emplace_back(buf);
						}
					}
				}
				//
				L.m_ItemCraft.clear();
				for (auto& Cd : Ld["crafts"]) {
					L.m_ItemCraft.resize(L.m_ItemCraft.size() + 1);
					//
					L.m_ItemCraft.back().durationTime = Cd["duration"];
					//
					L.m_ItemCraft.back().m_ItemReq.clear();
					if (Cd.contains("requiredItems")) {
						if (!Cd["requiredItems"].is_null()) {
							for (const auto&m : Cd["requiredItems"]) {
								ItemGetData buf;
								std::string Name = m["item"]["name"];
								buf.Set(ItemData::Instance()->FindID(Name.c_str()), m["count"]);
								L.m_ItemCraft.back().m_ItemReq.emplace_back(buf);
							}
						}
					}
					//
					L.m_ItemCraft.back().m_ItemReward.clear();
					if (Cd.contains("rewardItems")) {
						if (!Cd["rewardItems"].is_null()) {
							for (const auto&m : Cd["rewardItems"]) {
								ItemGetData buf;
								std::string Name = m["item"]["name"];
								buf.Set(ItemData::Instance()->FindID(Name.c_str()), m["count"]);
								L.m_ItemCraft.back().m_ItemReward.emplace_back(buf);
							}
						}
					}
					//
				}
			}
		}
	};
	//
	class HideoutData : public SingletonBase<HideoutData>, public DataParent<HideoutID, HideoutList> {
	private:
		friend class SingletonBase<HideoutData>;
	private:
		HideoutData() noexcept {
			std::string Path = "data/Hideout/";
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Path2 = Path + d.cFileName + "/";
					auto data_t2 = GetFileNamesInDirectory(Path2.c_str());
					for (auto& d2 : data_t2) {
						if (d2.cFileName[0] != '.') {
							SetList((Path2 + d2.cFileName + "/").c_str());
						}
					}
				}
			}
			for (auto& t : m_List) {
				t.SetOtherPartsID(m_List);
			}
			for (auto& t : m_List) {
				t.m_CheckJson = 0;
			}
		}
		~HideoutData() noexcept {}
	private:
		std::vector<HideoutJsonData> m_HideoutJsonData;
	public:
		void GetJsonData(nlohmann::json& data) {
			m_HideoutJsonData.clear();
			for (const auto& d : data["data"]["hideoutStations"]) {
				m_HideoutJsonData.resize(m_HideoutJsonData.size() + 1);
				m_HideoutJsonData.back().GetJsonData(d);
				m_HideoutJsonData.back().m_IsFileOpened = false;
			}
		}
		void SaveDatabyJson() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : m_HideoutJsonData) {
					if (L.GetIDstr() == jd.id) {
						L.m_CheckJson++;

						jd.m_IsFileOpened = true;

						std::ofstream outputfile(L.GetFilePath());
						outputfile << "IDstr=" + jd.id + "\n";
						outputfile << "Name=" + jd.name + "\n";

						for (auto& L2 : jd.m_LvData) {
							auto LV = "Lv" + std::to_string((&L2 - &jd.m_LvData.front()) + 1);

							outputfile << LV + "constructionTime=" + std::to_string(L2.constructionTime) + "\n";
							outputfile << LV + "Information_Eng=" + L2.Information_Eng + "\n";
							{
								outputfile << LV + "itemReq=[";
								for (auto& m : L2.m_ItemReq) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetCount());
									if (&m != &L2.m_ItemReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "stationLevelReq=[";
								for (auto& m : L2.m_Parent) {
									outputfile << m.GetID();
									outputfile << "x" + std::to_string(m.GetLv());
									if (&m != &L2.m_Parent.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "traderRequirements=[";
								for (auto& m : L2.m_TraderReq) {
									outputfile << TraderData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetLv());
									if (&m != &L2.m_TraderReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							for (auto& c : L2.m_ItemCraft) {
								outputfile << LV + "craftduration=" + std::to_string(c.durationTime) + "\n";
								{
									outputfile << LV + "craftitemReq=[";
									for (auto& m : c.m_ItemReq) {
										outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
										outputfile << "x" + std::to_string(m.GetCount());
										if (&m != &c.m_ItemReq.back()) {
											outputfile << ",";
										}
									}
									outputfile << "]\n";
								}
								{
									outputfile << LV + "craftitemReward=[";
									for (auto& m : c.m_ItemReward) {
										outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
										outputfile << "x" + std::to_string(m.GetCount());
										if (&m != &c.m_ItemReward.back()) {
											outputfile << ",";
										}
									}
									outputfile << "]\n";
								}
							}
						}

						outputfile.close();
						break;
					}
				}
			}
			for (auto& jd : m_HideoutJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string ParentPath = "data/Hideout/Maked/Maked/";
					CreateDirectory(ParentPath.c_str(), NULL);
					std::string HideoutName = jd.name;
					auto SubStrs = [&](const char* str) {
						while (true) {
							auto now = HideoutName.find(str);
							if (now != std::string::npos) {
								HideoutName = HideoutName.substr(0, now) + HideoutName.substr(now + 1);
							}
							else {
								break;
							}
						}
					};
					SubStrs(".");

					SubStrs("\\");
					SubStrs("/");
					SubStrs(":");
					SubStrs("*");
					SubStrs("?");
					SubStrs("\"");
					SubStrs(">");
					SubStrs("<");
					SubStrs("|");

					std::string Name = ParentPath + "/" + HideoutName + ".txt";
					std::ofstream outputfile(Name);
					outputfile << "IDstr=" + jd.id + "\n";
					outputfile << "Name=" + jd.name + "\n";

					for (auto& L : jd.m_LvData) {
						auto LV = "Lv" + std::to_string((&L - &jd.m_LvData.front()) + 1);

						outputfile << LV + "constructionTime=" + std::to_string(L.constructionTime) + "\n";
						outputfile << LV + "Information_Eng=" + L.Information_Eng + "\n";
						{
							outputfile << LV + "itemReq=[";
							for (auto& m : L.m_ItemReq) {
								outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
								outputfile << "x" + std::to_string(m.GetCount());
								if (&m != &L.m_ItemReq.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						{
							outputfile << LV + "stationLevelReq=[";
							for (auto& m : L.m_Parent) {
								outputfile << m.GetID();
								outputfile << "x" + std::to_string(m.GetLv());
								if (&m != &L.m_Parent.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						{
							outputfile << LV + "traderRequirements=[";
							for (auto& m : L.m_TraderReq) {
								outputfile << TraderData::Instance()->FindPtr(m.GetID())->GetName();
								outputfile << "x" + std::to_string(m.GetLv());
								if (&m != &L.m_TraderReq.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						for (auto& c : L.m_ItemCraft) {
							outputfile << LV + "craftduration=" + std::to_string(c.durationTime) + "\n";
							{
								outputfile << LV + "craftitemReq=[";
								for (auto& m : c.m_ItemReq) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetCount());
									if (&m != &c.m_ItemReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "craftitemReward=[";
								for (auto& m : c.m_ItemReward) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetCount());
									if (&m != &c.m_ItemReward.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
						}
					}
					outputfile.close();

					//RemoveDirectory(Path.c_str());
				}
			}
		}
		void CheckThroughJson(void) noexcept {
			for (auto& t : m_List) {
				if (t.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : ";
					ErrMes += t.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			for (auto& t : m_List) {
				if (t.m_CheckJson >= 2) {
					std::string ErrMes = "Error : Be repeated ";
					ErrMes += std::to_string(t.m_CheckJson);
					ErrMes += " : ";
					ErrMes += t.GetName();

					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
	};
	//
};