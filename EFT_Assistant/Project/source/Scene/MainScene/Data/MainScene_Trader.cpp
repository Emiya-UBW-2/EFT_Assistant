#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	void	TraderList::Load_Sub() noexcept {
		for (auto& L : this->m_LvData) {
			for (auto& C : L.m_ItemBarters) {
				for (auto& T : C.m_ItemReq) {
					T.CheckID(ItemData::Instance());
				}
				for (auto& T : C.m_ItemReward) {
					T.CheckID(ItemData::Instance());
				}
			}
		}

	}
	//
	void TraderJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		PayItem = ItemData::Instance()->FindID((std::string)(data["currency"]["name"]));
		m_LvData.clear();
		for (auto& Ld : data["levels"]) {
			m_LvData.resize(m_LvData.size() + 1);
			auto& L = this->m_LvData.back();

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
			for (auto& B : L.m_ItemBarters) {
				for (auto& T : B.m_ItemReq) {
					T.CheckID(ItemData::Instance());
				}
				for (auto& T : B.m_ItemReward) {
					T.CheckID(ItemData::Instance());
				}
			}

		}
	}
	void TraderJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		{
			auto* ptr = TraderData::Instance()->FindPtr(TraderData::Instance()->FindID(this->m_name));
			outputfile << "Color=[" + std::to_string(ptr->GetColorRGB(0)) + DIV_STR + std::to_string(ptr->GetColorRGB(1)) + DIV_STR + std::to_string(ptr->GetColorRGB(2)) + "]\n";
		}
		outputfile << "PayItem=" + ItemData::Instance()->FindPtr(this->PayItem)->GetName() + "\n";
		for (auto& L2 : this->m_LvData) {
			auto LV = "Lv" + std::to_string((&L2 - &this->m_LvData.front()) + 1);

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

	}
	//
};
