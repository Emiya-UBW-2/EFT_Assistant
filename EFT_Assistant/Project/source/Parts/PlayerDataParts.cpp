#include "PlayerDataParts.hpp"

#include "../PartsHeader.hpp"

const FPS_n2::PlayerData* SingletonBase<FPS_n2::PlayerData>::m_Singleton = nullptr;
namespace FPS_n2 {
	void			PlayerData::Load(void) noexcept {
		SetOutApplicationLogValidFlag(FALSE);
		int mdata = FileRead_open("data/PlayerData.txt", FALSE);
		while (true) {
			if (FileRead_eof(mdata) != 0) { break; }
			auto ALL = getparams::Getstr(mdata);
			//コメントアウト
			SubRIGHTStrs(&ALL, "//");
			//
			if (ALL == "") { continue; }
			auto LEFT = getparams::getleft(ALL);
			auto RIGHT = getparams::getright(ALL);
			auto Args = GetArgs(RIGHT);
			//アイテムデータ読みとり
			if (LEFT == "LastDataReceive") { this->m_LastDataReceive = RIGHT; }
			else if (LEFT == "ItemData") { SetItemLock(Args.at(0).c_str(), Args.at(1) == "true"); }
			else if (LEFT == "ClearTask") { this->m_TaskClearData.emplace_back(RIGHT); }
			else if (LEFT == "UnlockHideout") {
				auto L = RIGHT.rfind("x");
				if (L != std::string::npos) {
					m_HideoutClearData.emplace_back(std::make_pair(RIGHT.substr(0, L), std::stoi(RIGHT.substr(L + 1))));
				}
			}
			else if (LEFT == "Edition") { this->m_Edition = (EnumEdition)(std::stoi(RIGHT)); }
			else if (LEFT == "IsNeedLightKeeper") { this->m_IsNeedLightKeeper = (RIGHT == "TRUE"); }
			else if (LEFT == "IsNeedKappa") { this->m_IsNeedKappa = (RIGHT == "TRUE"); }
			else if (LEFT == "IsOpenFreaMarket") { this->m_IsOpenFreaMarket = (RIGHT == "TRUE"); }
			else if (LEFT == "MaxLevel") { this->m_MaxLevel = std::stoi(RIGHT); }
			else if (LEFT == "PMCType") { this->m_IsUSEC = (RIGHT == "USEC"); }
		}
		FileRead_close(mdata);
		SetOutApplicationLogValidFlag(TRUE);
	}
	void			PlayerData::Save(void) noexcept {
		std::ofstream outputfile("data/PlayerData.txt");
		outputfile << "LastDataReceive=" + this->m_LastDataReceive + "\n";

		for (auto& LD : this->m_ItemLockData) {
			outputfile << "ItemData=[" + LD.GetID() + DIV_STR + (LD.GetIsLock() ? "true" : "false") + "]\n";
		}
		for (auto& LD : this->m_TaskClearData) {
			outputfile << "ClearTask=" + LD + "\n";
		}
		for (auto& LD : this->m_HideoutClearData) {
			outputfile << "UnlockHideout=" + LD.first + "x" + std::to_string(LD.second) + "\n";
		}

		outputfile << "Edition=" + std::to_string((int)m_Edition) + "\n";
		outputfile << "IsNeedLightKeeper=" + (std::string)(m_IsNeedLightKeeper ? "TRUE" : "FALSE") + "\n";
		outputfile << "IsNeedKappa=" + (std::string)(m_IsNeedKappa ? "TRUE" : "FALSE") + "\n";
		outputfile << "IsOpenFreaMarket=" + (std::string)(m_IsOpenFreaMarket ? "TRUE" : "FALSE") + "\n";
		outputfile << "MaxLevel=" + std::to_string(m_MaxLevel) + "\n";
		outputfile << "PMCType=" + (std::string)(m_IsUSEC ? "USEC" : "BEAR") + "\n";

		//outputfile << "grass_level=" + std::to_string(grass_level) + "\n";
		outputfile.close();
	}
};
