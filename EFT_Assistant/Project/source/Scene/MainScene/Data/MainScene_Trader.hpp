#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"

namespace FPS_n2 {
	//
	struct TraderLvData {
		struct BartersData {
			std::vector<IDParents<TaskID>>			m_TaskReq;
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
		std::string									PayItem;
		std::vector<TraderLvData>					m_LvData;
	public:
		//追加設定
		void	SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override {
			std::string LEFTBuf = LEFT.substr(3);
			std::string NumBuf2 = LEFT.substr(2, 1);
			if (std::all_of(NumBuf2.cbegin(), NumBuf2.cend(), isdigit)) {
				size_t ID = (size_t)((int)std::stoi(NumBuf2) - 1);
				if (m_LvData.size() <= ID) { this->m_LvData.resize(ID + 1); }
				auto& data = this->m_LvData.at(ID);
				//開放データ
				if (LEFTBuf == "Reputation") { data.Reputation = std::stof(Args[0]); }
				if (LEFTBuf == "NeedLv") { data.NeedLv = std::stoi(Args[0]); }
				if (LEFTBuf == "Commerce") { data.Commerce = std::stoi(Args[0]); }
				if (LEFTBuf == "payRate") { data.payRate = std::stof(Args[0]); }
				if (LEFTBuf == "insuranceRate") { data.insuranceRate = std::stof(Args[0]); }
				if (LEFTBuf == "repairCostMultiplier") { data.repairCostMultiplier = std::stof(Args[0]); }
				//
				if (LEFTBuf == "taskUnlock") {
					data.m_ItemBarters.resize(data.m_ItemBarters.size() + 1);
					data.m_ItemBarters.back().m_TaskReq.clear();
					for (auto&a : Args) {
						if (a != "") {
							data.m_ItemBarters.back().m_TaskReq.resize(data.m_ItemBarters.back().m_TaskReq.size() + 1);
							data.m_ItemBarters.back().m_TaskReq.back().SetName(a);
						}
					}
				}
				if (LEFTBuf == "BarteritemReq") {
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&data.m_ItemBarters.back().m_ItemReq, a, "x", false);
					}
				}
				if (LEFTBuf == "BarteritemReward") {
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&data.m_ItemBarters.back().m_ItemReward, a, "x", false);
					}
				}
			}
			else {
				if (LEFT == "PayItem") { PayItem = Args[0]; }
			}
		}
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto&	GetPayItem() const noexcept { return PayItem; }
		const auto&	GetLvData() const noexcept { return this->m_LvData; }


		const int		DrawBarter(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int ysize, int Lv, int Select, bool isdrawName, bool isdrawAfter, int count) const noexcept;
	};
	//
	class TraderJsonData :public JsonDataParent {
	public:
		ItemID										PayItem{ InvalidID };
		std::array<int, 3>							m_Color{ 0,0,0 };
		std::vector<TraderLvData>					m_LvData;
	public:
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};

	class TraderData : public DataParent<TraderList>, public JsonListParent<TraderJsonData> {
	public:
		TraderData() noexcept {
			SetDirList("data/trader/");
		}
		~TraderData() noexcept {}
	public:
	};
};
