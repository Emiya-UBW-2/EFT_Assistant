#pragma once
#include "../Header.hpp"

namespace FPS_n2 {
	//
	class ItemLockData {
		std::string		m_ID;
		bool			m_IsLock{ false };
	public:
		const auto&		GetID() const noexcept { return this->m_ID; }
		const auto&		GetIsLock() const noexcept { return this->m_IsLock; }

		void	SetID(const char* Name) noexcept { this->m_ID = Name; }
		void	SetIsLock(bool value) noexcept { this->m_IsLock = value; }
	};
	//
	class PlayerData : public SingletonBase<PlayerData> {
	private:
		friend class SingletonBase<PlayerData>;
	private:
		std::string					m_LastDataReceive;
		std::vector<ItemLockData>	m_ItemLockData;
		std::vector<ItemLockData>	m_ItemBlackListData;
		std::vector<std::string>	m_TaskClearData;
		std::vector<std::pair<std::string, int>>	m_HideoutClearData;
	private:
		EnumEdition					m_Edition{ EnumEdition::STANDARD };
		bool						m_IsNeedLightKeeper{ false };
		bool						m_IsOpenFreaMarket{ false };
		bool						m_IsNeedKappa{ false };
		int							m_MaxLevel{ 71 };
	private:
		bool						m_IsUSEC{ true };
	private:
		PlayerData() noexcept {
			Load();
		}
		~PlayerData() noexcept {
		}
	public:
		void			Load(void) noexcept;
		void			Save(void) noexcept;
	private:
		ItemLockData* GetItemLockPtr(const char* ID) const noexcept {
			auto Point = std::find_if(m_ItemLockData.begin(), this->m_ItemLockData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != this->m_ItemLockData.end()) {
				return (ItemLockData*)&*Point;
			}
			return nullptr;
		}
		ItemLockData* GetItemBlackListPtr(const char* ID) const noexcept {
			auto Point = std::find_if(m_ItemBlackListData.begin(), this->m_ItemBlackListData.end(), [&](const ItemLockData& Data) {return (Data.GetID() == ID); });
			if (Point != this->m_ItemBlackListData.end()) {
				return (ItemLockData*)&*Point;
			}
			return nullptr;
		}
	public:
		void OnOffItemLock(const char* ID) noexcept {
			auto* Point = GetItemLockPtr(ID);
			if (Point) {
				Point->SetIsLock(!Point->GetIsLock());
			}
			else {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(true);
			}
		}
		void SetItemLock(const char* ID, bool value) noexcept {
			auto* Point = GetItemLockPtr(ID);
			if (Point) {
				Point->SetIsLock(value);
			}
			else if (value) {
				m_ItemLockData.resize(m_ItemLockData.size() + 1);
				m_ItemLockData.back().SetID(ID);
				m_ItemLockData.back().SetIsLock(value);
			}
		}
		const auto GetItemLock(const char* ID) noexcept {
			auto* Point = GetItemLockPtr(ID);
			if (Point) {
				return Point->GetIsLock();
			}
			else {
				return false;
			}
		}
		//
		void OnOffItemBlackList(const char* ID) noexcept {
			auto* Point = GetItemBlackListPtr(ID);
			if (Point) {
				Point->SetIsLock(!Point->GetIsLock());
			}
			else {
				m_ItemBlackListData.resize(m_ItemBlackListData.size() + 1);
				m_ItemBlackListData.back().SetID(ID);
				m_ItemBlackListData.back().SetIsLock(!m_ItemBlackListData.back().GetIsLock());
			}
		}
		void SetItemBlackList(const char* ID, bool value) noexcept {
			auto* Point = GetItemBlackListPtr(ID);
			if (Point) {
				Point->SetIsLock(value);
			}
			else if (value) {
				m_ItemBlackListData.resize(m_ItemBlackListData.size() + 1);
				m_ItemBlackListData.back().SetID(ID);
				m_ItemBlackListData.back().SetIsLock(value);
			}
		}
		const auto GetItemBlackList(const char* ID) noexcept {
			auto* Point = GetItemBlackListPtr(ID);
			if (Point) {
				return Point->GetIsLock();
			}
			else {
				return false;
				//m_ItemBlackListData.resize(m_ItemBlackListData.size() + 1);
				//m_ItemBlackListData.back().SetID(ID);
				//m_ItemBlackListData.back().SetIsLock(false);
				//return this->m_ItemBlackListData.back().GetIsLock();
			}
		}
		//
		void OnOffTaskClear(const char* ID) noexcept {
			auto Point = std::find_if(m_TaskClearData.begin(), this->m_TaskClearData.end(), [&](const std::string& Data) {return (Data == ID); });
			if (Point != this->m_TaskClearData.end()) {
				m_TaskClearData.erase(Point);
			}
			else {
				m_TaskClearData.resize(m_TaskClearData.size() + 1);
				m_TaskClearData.back() = ID;
			}
		}
		void SetTaskClear(const char* ID, bool value) noexcept {
			auto Point = std::find_if(m_TaskClearData.begin(), this->m_TaskClearData.end(), [&](const std::string& Data) {return (Data == ID); });
			if (Point != this->m_TaskClearData.end()) {
				if (!value) {
					m_TaskClearData.erase(Point);
				}
			}
			else {
				if (value) {
					m_TaskClearData.resize(m_TaskClearData.size() + 1);
					m_TaskClearData.back() = ID;
				}
			}
		}
		void ResetTaskClear() noexcept {
			m_TaskClearData.clear();
		}
		const auto GetTaskClear(const char* ID) noexcept {
			auto Point = std::find_if(m_TaskClearData.begin(), this->m_TaskClearData.end(), [&](const std::string& Data) {return (Data == ID); });
			return (Point != this->m_TaskClearData.end());
		}

		void OnOffHideoutClear(const char* ID, int Lv) noexcept {
			auto Point = std::find_if(m_HideoutClearData.begin(), this->m_HideoutClearData.end(), [&](const std::pair<std::string, int>& Data) {return (Data.first == ID); });
			if (Point != this->m_HideoutClearData.end()) {
				if (Point->second >= Lv) {
					if (Lv > 1) {
						Point->second = Lv - 1;
					}
					else {
						m_HideoutClearData.erase(Point);
					}
				}
				else {
					Point->second = Lv;
				}
			}
			else {
				m_HideoutClearData.resize(m_HideoutClearData.size() + 1);
				m_HideoutClearData.back().first = ID;
				m_HideoutClearData.back().second = Lv;
			}
		}
		void ResetHideoutClear() noexcept {
			m_HideoutClearData.clear();
		}
		const auto GetHideoutClear(const char* ID, int Lv) noexcept {
			auto Point = std::find_if(m_HideoutClearData.begin(), this->m_HideoutClearData.end(), [&](const std::pair<std::string, int>& Data) {return (Data.first == ID && Data.second >= Lv); });
			if (Point != this->m_HideoutClearData.end()) {
				return true;
			}
			else {
				std::string IDStr = ID;
				if (IDStr.find("Stash") != std::string::npos) {
					Point = std::find_if(m_HideoutClearData.begin(), this->m_HideoutClearData.end(), [&](const std::pair<std::string, int>& Data) {return (Data.first == ID); });
					if (Point != this->m_HideoutClearData.end()) {
						Point->second = (int)m_Edition + 1;
					}
					else {
						m_HideoutClearData.resize(m_HideoutClearData.size() + 1);
						m_HideoutClearData.back().first = ID;
						m_HideoutClearData.back().second = (int)m_Edition + 1;
					}
					return (((int)m_Edition + 1) >= Lv);
				}
				else {
					return false;
				}
			}
		}

		void SetLastDataReceive() noexcept {
			time_t t = time(NULL);				// Œ»Ý“úŽž‚ðŽæ“¾‚·‚é
			tm local;							// “úŽžî•ñ‚ðŠi”[‚·‚é•Ï”‚ð—pˆÓ‚·‚é
			localtime_s(&local, &t);			// ƒ[ƒJƒ‹“úŽž‚ð•Ï”‚ÉŠi”[‚·‚é
			char buffer[256];
			strftime(buffer, sizeof(buffer), "%Y %m/%d %H:%M", &local);
			this->m_LastDataReceive = buffer;
		}
		const auto& GetLastDataReceive() const noexcept { return this->m_LastDataReceive; }

		void SetIsNeedLightKeeper(bool date) noexcept { this->m_IsNeedLightKeeper = date; }
		const auto& GetIsNeedLightKeeper() const noexcept { return this->m_IsNeedLightKeeper; }

		void SetEdition(EnumEdition date) noexcept { this->m_Edition = date; }
		const auto& GetEdition() const noexcept { return this->m_Edition; }

		void SetIsOpenFreaMarket(bool date) noexcept { this->m_IsOpenFreaMarket = date; }
		const auto& GetIsOpenFreaMarket() const noexcept { return this->m_IsOpenFreaMarket; }
		void SetIsNeedKappa(bool date) noexcept { this->m_IsNeedKappa = date; }
		const auto& GetIsNeedKappa() const noexcept { return this->m_IsNeedKappa; }
		void SetMaxLevel(int date) noexcept { this->m_MaxLevel = date; }
		const auto& GetMaxLevel() const noexcept { return this->m_MaxLevel; }
		void SetIsUSEC(bool date) noexcept { this->m_IsUSEC = date; }
		const auto& GetIsUSEC() const noexcept { return this->m_IsUSEC; }
	};
	//
	static void GetDirList(const char* DirPath, const std::function<void(const char*)>& Doing) noexcept {
		std::string Path = DirPath;
		std::vector<WIN32_FIND_DATA> data_t;
		GetFileNamesInDirectory((Path + "*").c_str(), &data_t);
		for (auto& d : data_t) {
			if (d.cFileName[0] != '.') {
				Doing(d.cFileName);
			}
		}
		data_t.clear();
	}
	//
};
