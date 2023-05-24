#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {

	class TraderList : public ListParent<TraderID> {
		std::array<int, 4>		m_Lv{ 0,0,0,0 };
		std::array<float, 4>	m_Rep{ 0,0,0,0 };
	public:
		//追加設定
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			for (int i = 0; i < 4; i++) {
				if (LEFT == "Lv" + std::to_string(i + 1) + "Reputation") {
					m_Rep[i] = std::stof(RIGHT);
					break;
				}
				else if (LEFT == "Lv" + std::to_string(i + 1) + "NeedLv") {
					m_Lv[i] = std::stoi(RIGHT);
					break;
				}
			}
		}
		void	Load_Sub() noexcept override {
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	};
	//
	struct TraderLvData {
		struct BartersData {
			std::vector<TaskID>						m_TaskReq;
			std::vector<ItemGetData>				m_ItemReq;
			std::vector<ItemGetData>				m_ItemReward;
		};
		//開放データ
		int											NeedLv{ 1 };
		float										Reputation{ 1.f };
		int											Commerce{ 0 };
		float										payRate{ 0.f };
		float										insuranceRate{ 0.f };
		float										repairCostMultiplier{ 0.f };
		//クラフトデータ
		std::vector<BartersData>					m_ItemBarters;
	};
	//
	class TraderJsonData {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
		std::string									Information_Eng;
		ItemID										PayItem;
		std::vector<TraderLvData>					m_LvData;
	public:
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];
			Information_Eng = data["description"];
			PayItem = ItemData::Instance()->FindID(data["currency"]["name"]);
			m_LvData.clear();
			for (auto& Ld : data["levels"]) {
				m_LvData.resize(m_LvData.size() + 1);
				auto& L = m_LvData.back();

				L.NeedLv = Ld["requiredPlayerLevel"];
				L.Reputation = Ld["requiredReputation"];
				L.Commerce = Ld["requiredCommerce"];
				L.payRate = Ld["payRate"];

				if (!Ld["insuranceRate"].is_null()) {
					L.insuranceRate = Ld["insuranceRate"];
				}
				else {
					L.insuranceRate = -1.f;
				}

				if (!Ld["repairCostMultiplier"].is_null()) {
					L.repairCostMultiplier = Ld["repairCostMultiplier"];
				}
				else {
					L.repairCostMultiplier = -1.f;
				}

				L.m_ItemBarters.clear();
				if (Ld.contains("barters")) {
					if (!Ld["barters"].is_null()) {
						for (const auto&m : Ld["barters"]) {
							L.m_ItemBarters.resize(L.m_ItemBarters.size() + 1);
							auto& B = L.m_ItemBarters.back();
							for (const auto&I : m["requiredItems"]) {
								ItemGetData buf;
								buf.Set(I["item"]["name"], I["count"]);
								B.m_ItemReq.emplace_back(buf);
							}
							for (const auto&I : m["rewardItems"]) {
								ItemGetData buf;
								buf.Set(I["item"]["name"], I["count"]);
								B.m_ItemReward.emplace_back(buf);
							}
						}
					}
				}

				if (Ld.contains("cashOffers")) {
					if (!Ld["cashOffers"].is_null()) {
						for (const auto&m : Ld["cashOffers"]) {
							L.m_ItemBarters.resize(L.m_ItemBarters.size() + 1);
							auto& B = L.m_ItemBarters.back();
							{
								ItemGetData buf;
								buf.Set(m["currencyItem"]["name"], m["price"]);
								B.m_ItemReq.emplace_back(buf);
							}
							{
								ItemGetData buf;
								buf.Set(m["item"]["name"], 1);
								B.m_ItemReward.emplace_back(buf);
							}
						}
					}
				}
			}
		}
	};

	class TraderData : public SingletonBase<TraderData>, public DataParent<TraderID, TraderList> {
	private:
		friend class SingletonBase<TraderData>;
	private:
		TraderData() noexcept {
			SetList("data/trader/");
		}
		~TraderData() noexcept {
		}
	private:
		std::vector<TraderJsonData> m_TraderJsonData;
	public:
		void GetJsonData(nlohmann::json& data) {
			m_TraderJsonData.clear();
			for (const auto& d : data["data"]["traders"]) {
				m_TraderJsonData.resize(m_TraderJsonData.size() + 1);
				m_TraderJsonData.back().GetJsonData(d);
				m_TraderJsonData.back().m_IsFileOpened = false;
			}
		}
		void SaveDatabyJson() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : m_TraderJsonData) {
					if (L.GetIDstr() == jd.id) {
						jd.m_IsFileOpened = true;
						std::ofstream outputfile(L.GetFilePath());

						outputfile << "IDstr=" + jd.id + "\n";
						outputfile << "Name=" + jd.name + "\n";
						outputfile << "Information_Eng=" + jd.Information_Eng + "\n";
						outputfile << "PayItem=" + ItemData::Instance()->FindPtr(jd.PayItem)->GetName() + "\n";
						for (auto& L2 : jd.m_LvData) {
							auto LV = "Lv" + std::to_string((&L2 - &jd.m_LvData.front()) + 1);

							outputfile << LV + "NeedLv=" + std::to_string(L2.NeedLv) + "\n";
							outputfile << LV + "Reputation=" + std::to_string(L2.Reputation) + "\n";
							outputfile << LV + "Commerce=" + std::to_string(L2.Commerce) + "\n";
							outputfile << LV + "payRate=" + std::to_string(L2.payRate) + "\n";
							if (L2.insuranceRate >= 0.f) {
								outputfile << LV + "insuranceRate=" + std::to_string(L2.insuranceRate) + "\n";
							}
							if (L2.repairCostMultiplier >= 0.f) {
								outputfile << LV + "repairCostMultiplier=" + std::to_string(L2.repairCostMultiplier) + "\n";
							}
							for (auto& c : L2.m_ItemBarters) {
								//m_TaskReq
								{
									outputfile << LV + "BarteritemReq=[";
									for (auto& m : c.m_ItemReq) {
										outputfile << m.GetName();
										outputfile << "x" + std::to_string(m.GetValue());
										if (&m != &c.m_ItemReq.back()) {
											outputfile << ",";
										}
									}
									outputfile << "]\n";
								}
								{
									outputfile << LV + "BarteritemReward=[";
									for (auto& m : c.m_ItemReward) {
										outputfile << m.GetName();
										outputfile << "x" + std::to_string(m.GetValue());
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
			for (auto& jd : m_TraderJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string ParentPath = "data/trader/Maked";
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
					outputfile << "Information_Eng=" + jd.Information_Eng + "\n";
					outputfile << "PayItem=" + ItemData::Instance()->FindPtr(jd.PayItem)->GetName() + "\n";
					for (auto& L2 : jd.m_LvData) {
						auto LV = "Lv" + std::to_string((&L2 - &jd.m_LvData.front()) + 1);

						outputfile << LV + "NeedLv=" + std::to_string(L2.NeedLv) + "\n";
						outputfile << LV + "Reputation=" + std::to_string(L2.Reputation) + "\n";
						outputfile << LV + "Commerce=" + std::to_string(L2.Commerce) + "\n";
						outputfile << LV + "payRate=" + std::to_string(L2.payRate) + "\n";
						if (L2.insuranceRate >= 0.f) {
							outputfile << LV + "insuranceRate=" + std::to_string(L2.insuranceRate) + "\n";
						}
						if (L2.repairCostMultiplier >= 0.f) {
							outputfile << LV + "repairCostMultiplier=" + std::to_string(L2.repairCostMultiplier) + "\n";
						}

						for (auto& c : L2.m_ItemBarters) {
							//m_TaskReq
							{
								outputfile << LV + "BarteritemReq=[";
								for (auto& m : c.m_ItemReq) {
									outputfile << m.GetName();
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &c.m_ItemReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "BarteritemReward=[";
								for (auto& m : c.m_ItemReward) {
									outputfile << m.GetName();
									outputfile << "x" + std::to_string(m.GetValue());
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
	};
};