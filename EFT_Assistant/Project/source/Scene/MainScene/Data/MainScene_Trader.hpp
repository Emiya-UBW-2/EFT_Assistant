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
	class TraderJsonData :public JsonDataParent {
	public:
		std::string									Information_Eng;
		ItemID										PayItem;
		std::array<int, 3>							m_Color{ 0,0,0 };
		std::vector<TraderLvData>					m_LvData;
	public:
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};

	class TraderData : public SingletonBase<TraderData>, public DataParent<TraderID, TraderList>, public JsonListParent<TraderJsonData> {
	private:
		friend class SingletonBase<TraderData>;
	private:
		TraderData() noexcept {
			SetList("data/trader/");
		}
		~TraderData() noexcept {}
	public:
		void UpdateData() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : GetJsonDataList()) {
					if (L.GetIDstr() == jd->id) {
						jd->OutputData(L.GetFilePath());
						break;
					}
				}
			}
		}
	};
};