#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	struct HideoutLvData {
		struct CraftData {
			int											durationTime{ 0 };
			std::vector<ItemGetData>					m_ItemReq;
			std::vector<ItemGetData>					m_ItemReward;
		public:
			std::vector<std::string>					m_ItemReqArgs;
			std::vector<std::string>					m_ItemRewardArgs;
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
	public:
		std::vector<std::string>					m_ItemReqArgs;
		std::vector<std::string>					m_ParentArgs;
		std::vector<std::string>					m_TraderReqArgs;
	};
	//
	class HideoutList : public ListParent<HideoutID> {
		std::vector<HideoutLvData> m_LvData;
		int m_DrawWindowLv{ 1 };
	private:
		//追加設定
		void	SetSub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override;
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		int										m_CheckJson{ 0 };
	public:
		const auto&	GetLvData() const noexcept { return m_LvData; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) noexcept;
		void			DrawUnlockWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
		void			DrawCraftWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs = nullptr, int* ys = nullptr, int size = 10) noexcept;
		void			SetOtherPartsID(const std::vector<HideoutList>& HideoutList) noexcept;
	};
	//
	class HideoutJsonData {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
		std::vector<HideoutLvData>							m_LvData;
	public:
		void GetJsonData(const nlohmann::json& data);
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
		}
		~HideoutData() noexcept {}
	private:
		std::vector<HideoutJsonData> m_HideoutJsonData;
	public:
		void SetParent() noexcept {
			for (auto& t : m_List) {
				t.SetOtherPartsID(m_List);
			}
			for (auto& t : m_List) {
				t.m_CheckJson = 0;
			}
		}
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
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &L2.m_ItemReq.back()) {
										outputfile << DIV_STR;
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "stationLevelReq=[";
								for (auto& m : L2.m_Parent) {
									outputfile << HideoutData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &L2.m_Parent.back()) {
										outputfile << DIV_STR;
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "traderRequirements=[";
								for (auto& m : L2.m_TraderReq) {
									outputfile << TraderData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &L2.m_TraderReq.back()) {
										outputfile << DIV_STR;
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
										outputfile << "x" + std::to_string(m.GetValue());
										if (&m != &c.m_ItemReq.back()) {
											outputfile << DIV_STR;
										}
									}
									outputfile << "]\n";
								}
								{
									outputfile << LV + "craftitemReward=[";
									for (auto& m : c.m_ItemReward) {
										outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
										outputfile << "x" + std::to_string(m.GetValue());
										if (&m != &c.m_ItemReward.back()) {
											outputfile << DIV_STR;
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
					SubStrs(&HideoutName, ".");

					SubStrs(&HideoutName, "\\");
					SubStrs(&HideoutName, "/");
					SubStrs(&HideoutName, ":");
					SubStrs(&HideoutName, "*");
					SubStrs(&HideoutName, "?");
					SubStrs(&HideoutName, "\"");
					SubStrs(&HideoutName, ">");
					SubStrs(&HideoutName, "<");
					SubStrs(&HideoutName, "|");

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
								outputfile << "x" + std::to_string(m.GetValue());
								if (&m != &L.m_ItemReq.back()) {
									outputfile << DIV_STR;
								}
							}
							outputfile << "]\n";
						}
						{
							outputfile << LV + "stationLevelReq=[";
							for (auto& m : L.m_Parent) {
								outputfile << HideoutData::Instance()->FindPtr(m.GetID())->GetName();
								outputfile << "x" + std::to_string(m.GetValue());
								if (&m != &L.m_Parent.back()) {
									outputfile << DIV_STR;
								}
							}
							outputfile << "]\n";
						}
						{
							outputfile << LV + "traderRequirements=[";
							for (auto& m : L.m_TraderReq) {
								outputfile << TraderData::Instance()->FindPtr(m.GetID())->GetName();
								outputfile << "x" + std::to_string(m.GetValue());
								if (&m != &L.m_TraderReq.back()) {
									outputfile << DIV_STR;
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
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &c.m_ItemReq.back()) {
										outputfile << DIV_STR;
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "craftitemReward=[";
								for (auto& m : c.m_ItemReward) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &c.m_ItemReward.back()) {
										outputfile << DIV_STR;
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