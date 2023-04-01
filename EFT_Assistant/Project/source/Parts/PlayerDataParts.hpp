#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	static const auto		GetArgs(const std::string& RIGHT) noexcept {
		std::vector<std::string> Args;
		auto L = RIGHT.find("[");
		auto R = RIGHT.find("]");
		if (L != std::string::npos && R != std::string::npos) {
			std::string RIGHTBuf = RIGHT;
			RIGHTBuf = RIGHTBuf.substr(L + 1);
			while (true) {
				auto div = RIGHTBuf.find(",");
				if (div != std::string::npos) {
					Args.emplace_back(RIGHTBuf.substr(0, div));
					RIGHTBuf = RIGHTBuf.substr(div + 1);
				}
				else {
					Args.emplace_back(RIGHTBuf.substr(0, RIGHTBuf.find("]")));
					break;
				}
			}
		}
		return Args;
	}

	class ItemLockData {
		std::string		m_ID;
		bool			m_IsLock{ false };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetIsLock() const noexcept { return m_IsLock; }

		void	SetID(const char* Name) noexcept { m_ID = Name; }
		void	SetIsLock(bool value) noexcept { m_IsLock = value; }
	};

	class PlayerData : public SingletonBase<PlayerData> {
	private:
		friend class SingletonBase<PlayerData>;
	private:
		std::string					m_LastDataReceive;
		std::vector<ItemLockData> m_ItemLockData;
	private:
		PlayerData() noexcept {
			Load();
		}
		~PlayerData() noexcept {
		}
	public:
		void			Load(void) noexcept {
			SetOutApplicationLogValidFlag(FALSE);
			int mdata = FileRead_open("data/PlayerData.txt", FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				//コメントアウト
				if (ALL.find("//") != std::string::npos) {
					ALL = ALL.substr(0, ALL.find("//"));
				}
				//
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				auto Args = GetArgs(RIGHT);
				//アイテムデータ読みとり
				if (LEFT == "LastDataReceive") {
					m_LastDataReceive = RIGHT;
				}
				if (LEFT == "ItemData") {
					SetItemLock(Args.at(0).c_str(), Args.at(1) == "true");
				}
			}
			FileRead_close(mdata);
			SetOutApplicationLogValidFlag(TRUE);
		}
		void			Save(void) noexcept {
			std::ofstream outputfile("data/PlayerData.txt");
			outputfile << "LastDataReceive=" + m_LastDataReceive + "\n";

			for (auto& LD : m_ItemLockData) {
				outputfile << "ItemData=[" + LD.GetID() + "," + (LD.GetIsLock() ? "true" : "false") + "]\n";
			}
			//outputfile << "grass_level=" + std::to_string(grass_level) + "\n";
			outputfile.close();
		}
	public:
		void OnOffItemLock(const char* ID) noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != m_ItemLockData.end()) {
				Point->SetIsLock(!Point->GetIsLock());
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(!m_ItemLockData.back().GetIsLock());
			}
		}
		void SetItemLock(const char* ID, bool value) noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != m_ItemLockData.end()) {
				Point->SetIsLock(value);
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(value);
			}
		}
		const auto GetItemLock(const char* ID) noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != m_ItemLockData.end()) {
				return Point->GetIsLock();
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(false);
				return m_ItemLockData.back().GetIsLock();
			}
		}

		void SetLastDataReceive(const char* date) noexcept { m_LastDataReceive = date; }
		const auto& GetLastDataReceive() const noexcept { return m_LastDataReceive; }
	};
};
