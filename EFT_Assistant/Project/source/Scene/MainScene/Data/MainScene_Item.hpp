#pragma once
#include"Header.hpp"

namespace FPS_n2 {
	typedef int ItemID;
	class ItemList : public ListParent<ItemID> {
		ItemTypeID	m_TypeID{ InvalidID };
		std::vector<MapID>	m_MapID;
	private:
		//í«â¡ê›íË
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "Itemtype") {
				m_TypeID = ItemTypeData::Instance()->FindID(RIGHT.c_str());
			}
			if (LEFT == "Map") {
				m_MapID.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
			}
		}
		void	Load_Sub() noexcept override {
			if (m_TypeID == InvalidID) {
				std::string ErrMes = "Error : Not Setting ItemType in Item ";
				ErrMes += GetName();
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	public:
		const auto&	GetTypeID() const noexcept { return m_TypeID; }
		const auto&	GetMapID() const noexcept { return m_MapID; }
	};
	class ItemData : public SingletonBase<ItemData>, public DataParent<ItemID, ItemList> {
	private:
		friend class SingletonBase<ItemData>;
	private:
		ItemData() noexcept {
			std::string Path = "data/item/";
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					SetList((Path + d.cFileName + "/").c_str());
				}
			}
		}
		~ItemData() noexcept {}
	};

	class ItemGetData {
		ItemID				m_ID{ InvalidID };
		int					m_Count{ 0 };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetCount() const noexcept { return m_Count; }
		void			Set(ItemID id, int count) noexcept {
			m_ID = id;
			m_Count = count;
		}
	};
	void			SetItem(std::vector<ItemGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			ItemGetData tmp;
			tmp.Set(ItemData::Instance()->FindID(mes.substr(0, L).c_str()), std::stoi(mes.substr(L + 1)));
			Data->emplace_back(tmp);
		}
		else {
			//int a = 0;
		}
	};
};