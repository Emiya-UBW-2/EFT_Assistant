#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	struct TraderLvData {
		struct BartersData {
			std::vector<TaskID>						m_TaskReq;
			std::vector<ItemGetData>				m_ItemReq;
			std::vector<ItemGetData>				m_ItemReward;

			std::vector<std::string>					m_ItemReqArgs;
			std::vector<std::string>					m_ItemRewardArgs;
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
	class TraderList : public ListParent<TraderID> {
		std::string									Information_Eng;
		std::string									PayItem;
		std::vector<TraderLvData>					m_LvData;
	public:
		//追加設定
		void			SetSub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override {
			std::string LEFTBuf = LEFT.substr(3);
			std::string NumBuf2 = LEFT.substr(2, 1);
			if (std::all_of(NumBuf2.cbegin(), NumBuf2.cend(), isdigit)) {
				int ID = std::stoi(NumBuf2) - 1;
				if (m_LvData.size() <= ID) { m_LvData.resize(ID + 1); }
				//開放データ
				if (LEFTBuf == "Reputation") { m_LvData.at(ID).Reputation = std::stof(RIGHT); }
				if (LEFTBuf == "NeedLv") { m_LvData.at(ID).NeedLv = std::stoi(RIGHT); }
				if (LEFTBuf == "Commerce") { m_LvData.at(ID).Commerce = std::stoi(RIGHT); }
				if (LEFTBuf == "payRate") { m_LvData.at(ID).payRate = std::stof(RIGHT); }
				if (LEFTBuf == "insuranceRate") { m_LvData.at(ID).insuranceRate = std::stof(RIGHT); }
				if (LEFTBuf == "repairCostMultiplier") { m_LvData.at(ID).repairCostMultiplier = std::stof(RIGHT); }
				//
				if (LEFTBuf == "BarteritemReq") {
					m_LvData.at(ID).m_ItemBarters.resize(m_LvData.at(ID).m_ItemBarters.size() + 1);
					if (Args.size() > 0) {
						for (auto&a : Args) {
							m_LvData.at(ID).m_ItemBarters.back().m_ItemReqArgs.emplace_back(a);
						}
					}
					else {
						m_LvData.at(ID).m_ItemBarters.back().m_ItemReqArgs.emplace_back(RIGHT);
					}
				}
				if (LEFTBuf == "BarteritemReward") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							m_LvData.at(ID).m_ItemBarters.back().m_ItemRewardArgs.emplace_back(a);
						}
					}
					else {
						m_LvData.at(ID).m_ItemBarters.back().m_ItemRewardArgs.emplace_back(RIGHT);
					}
				}
			}
			else {
				if (LEFT == "Information_Eng") { Information_Eng = RIGHT; }
				if (LEFT == "PayItem") { PayItem = RIGHT; }
			}
		}
		void	Load_Sub() noexcept override {
			for (auto& L : m_LvData) {
				for (auto& C : L.m_ItemBarters) {
					for (auto& a : C.m_ItemReqArgs) {
						SetGetData<ItemGetData, ItemData>(&C.m_ItemReq, a);
					}
					for (auto& a : C.m_ItemRewardArgs) {
						SetGetData<ItemGetData, ItemData>(&C.m_ItemReward, a);
					}
				}
			}

		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	public:
		const auto&	GetInformation() const noexcept { return Information_Eng; }
		const auto&	GetPayItem() const noexcept { return PayItem; }
		const auto&	GetLvData() const noexcept { return m_LvData; }
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
		std::array<int, 3>							m_Color{ 0,0,0 };
		std::vector<TraderLvData>					m_LvData;
	public:
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];
			Information_Eng = data["description"];
			PayItem = ItemData::Instance()->FindID((std::string)(data["currency"]["name"]));
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
								buf.Set(ItemData::Instance()->FindID((std::string)(I["item"]["name"])), I["count"]);
								B.m_ItemReq.emplace_back(buf);
							}
							for (const auto&I : m["rewardItems"]) {
								ItemGetData buf;
								buf.Set(ItemData::Instance()->FindID((std::string)(I["item"]["name"])), I["count"]);
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
								buf.Set(ItemData::Instance()->FindID((std::string)(m["currencyItem"]["name"])), m["price"]);
								B.m_ItemReq.emplace_back(buf);
							}
							{
								ItemGetData buf;
								buf.Set(ItemData::Instance()->FindID((std::string)(m["item"]["name"])), 1);
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
						{
							auto* ptr = TraderData::Instance()->FindPtr(TraderData::Instance()->FindID(jd.name));
							outputfile << "Color=[" + std::to_string(ptr->GetColorRGB(0)) + DIV_STR + std::to_string(ptr->GetColorRGB(1)) + DIV_STR + std::to_string(ptr->GetColorRGB(2)) + "]\n";
						}
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
										outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
										outputfile << "x" + std::to_string(m.GetValue());
										if (&m != &c.m_ItemReq.back()) {
											outputfile << DIV_STR;
										}
									}
									outputfile << "]\n";
								}
								{
									outputfile << LV + "BarteritemReward=[";
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
			for (auto& jd : m_TraderJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string ParentPath = "data/trader/Maked";
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
					{
						auto* ptr = TraderData::Instance()->FindPtr(TraderData::Instance()->FindID(jd.name));
						outputfile << "Color=[" + std::to_string(ptr->GetColorRGB(0)) + DIV_STR + std::to_string(ptr->GetColorRGB(1)) + DIV_STR + std::to_string(ptr->GetColorRGB(2)) + "]\n";
					}
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
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetValue());
									if (&m != &c.m_ItemReq.back()) {
										outputfile << DIV_STR;
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "BarteritemReward=[";
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
	};
};