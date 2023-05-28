#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
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
	class TraderList : public ListParent<TraderID> {
		std::string									Information_Eng;
		std::string									PayItem;
		std::vector<TraderLvData>					m_LvData;
	public:
		//追加設定
		void	SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override {
			std::string LEFTBuf = LEFT.substr(3);
			std::string NumBuf2 = LEFT.substr(2, 1);
			if (std::all_of(NumBuf2.cbegin(), NumBuf2.cend(), isdigit)) {
				int ID = std::stoi(NumBuf2) - 1;
				if (m_LvData.size() <= ID) { this->m_LvData.resize(ID + 1); }
				//開放データ
				if (LEFTBuf == "Reputation") { this->m_LvData.at(ID).Reputation = std::stof(Args[0]); }
				if (LEFTBuf == "NeedLv") { this->m_LvData.at(ID).NeedLv = std::stoi(Args[0]); }
				if (LEFTBuf == "Commerce") { this->m_LvData.at(ID).Commerce = std::stoi(Args[0]); }
				if (LEFTBuf == "payRate") { this->m_LvData.at(ID).payRate = std::stof(Args[0]); }
				if (LEFTBuf == "insuranceRate") { this->m_LvData.at(ID).insuranceRate = std::stof(Args[0]); }
				if (LEFTBuf == "repairCostMultiplier") { this->m_LvData.at(ID).repairCostMultiplier = std::stof(Args[0]); }
				//
				if (LEFTBuf == "BarteritemReq") {
					m_LvData.at(ID).m_ItemBarters.resize(m_LvData.at(ID).m_ItemBarters.size() + 1);
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&m_LvData.at(ID).m_ItemBarters.back().m_ItemReq, a, "x");
					}
				}
				if (LEFTBuf == "BarteritemReward") {
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&m_LvData.at(ID).m_ItemBarters.back().m_ItemReward, a, "x");
					}
				}
			}
			else {
				if (LEFT == "Information_Eng") { Information_Eng = Args[0]; }
				if (LEFT == "PayItem") { PayItem = Args[0]; }
			}
		}
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto&	GetInformation() const noexcept { return Information_Eng; }
		const auto&	GetPayItem() const noexcept { return PayItem; }
		const auto&	GetLvData() const noexcept { return this->m_LvData; }
	};
	//
	class TraderJsonData :public JsonDataParent {
	public:
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
			SetDirList("data/trader/");
		}
		~TraderData() noexcept {}
	public:
		void InitDatabyJson() noexcept {
			ResetDataJson();
		}
		void UpdateData(int ofset, int size) noexcept {
			for (auto& L : this->m_List) {
				for (int loop = ofset; loop < ofset + size; loop++) {
					if (loop >= (int)GetJsonDataList().size()) { break; }
					auto& jd = GetJsonDataList().at(loop);
					if (L.GetIDstr() == jd->m_id) {
						jd->OutputData(L.GetFilePath());
						break;
					}
				}
			}
		}
		void CheckThroughJson(void) noexcept {
		}
	};
};
