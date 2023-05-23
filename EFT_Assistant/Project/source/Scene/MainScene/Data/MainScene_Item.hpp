#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int ItemID;


	class ItemList : public ListParent<ItemID> {
		class ChildItemSettings {
		public:
			std::vector<std::pair<const ItemList*, std::string>>	Data;
			std::vector<ItemTypeID>									TypeID;
		};
		typedef std::vector<std::pair<const ItemList*, std::string>> ItemSettings;

		ItemTypeID												m_TypeID{ InvalidID };
		std::vector<MapID>										m_MapID;
		std::vector<ChildItemSettings>							m_ChildPartsID;
		std::vector<const ItemList*>							m_ParentPartsID;
		std::vector<std::pair<const ItemList*, std::string>>	m_ConflictPartsID;
		float													m_Recoil{ 0.f };
		float													m_Ergonomics{ 0.f };
		bool													m_isWeapon{ false };
		bool													m_isWeaponMod{ false };

		int											m_SightRange{ -100 };

		int											m_basePrice{ 0 };
		int											m_width{ 0 };
		int											m_height{ 0 };
		std::vector<std::string>					m_types;
		int											m_avg24hPrice{ 0 };
		int											m_low24hPrice{ 0 };
		int											m_lastOfferCount{ 0 };
		std::vector<std::pair<TraderID, int>>		m_sellFor;
		float										m_weight{ 0.f };
		int											m_fleaMarketFee{ 0 };
		bool										m_IsPreset{ false };
		std::vector<int>							m_UseTaskID;
	private:
		//í«â¡ê›íË
		void	Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override {
			if (LEFT == "Itemtype") {
				m_TypeID = ItemTypeData::Instance()->FindID(RIGHT.c_str());
			}
			if (LEFT == "Map") {
				m_MapID.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
			}
			if (LEFT == "ChildParts") {
				m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							bool isHit = false;
							for (auto& d : m_ChildPartsID.back().Data) {
								if (d.second == a) {
									isHit = true;
									break;
								}
							}
							if (!isHit) {
								m_ChildPartsID.back().Data.resize(m_ChildPartsID.back().Data.size() + 1);
								m_ChildPartsID.back().Data.back().second = a;
							}
						}
					}
				}
				else {
					bool isHit = false;
					for (auto& d : m_ChildPartsID.back().Data) {
						if (d.second == RIGHT) {
							isHit = true;
							break;
						}
					}
					if (!isHit) {
						m_ChildPartsID.back().Data.resize(m_ChildPartsID.back().Data.size() + 1);
						m_ChildPartsID.back().Data.back().second = RIGHT;
					}
				}
			}
			if (LEFT == "Conflict") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							bool isHit = false;
							for (auto& d : m_ConflictPartsID) {
								if (d.second == a) {
									isHit = true;
									break;
								}
							}
							if (!isHit) {
								m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
								m_ConflictPartsID.back().second = a;
							}
						}
					}
				}
				else {
					bool isHit = false;
					for (auto& d : m_ConflictPartsID) {
						if (d.second == RIGHT) {
							isHit = true;
							break;
						}
					}
					if (!isHit) {
						m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
						m_ConflictPartsID.back().second = RIGHT;
					}
				}
			}

			if (LEFT == "Recoil") {
				if (RIGHT.find("+") != std::string::npos) {
					m_Recoil = std::stof(RIGHT.substr(RIGHT.find("+") + 1));
				}
				else {
					if (RIGHT == "") {
						//int a = 0;
					}
					m_Recoil = std::stof(RIGHT);
				}
			}
			if (LEFT == "Ergonomics") {
				if (RIGHT.find("+") != std::string::npos) {
					m_Ergonomics = std::stof(RIGHT.substr(RIGHT.find("+") + 1));
				}
				else {
					if (RIGHT == "") {
						//int a = 0;
					}
					m_Ergonomics = std::stof(RIGHT);
				}
			}

			if (LEFT == "SightRange") { m_SightRange = std::stoi(RIGHT); }

			if (LEFT == "basePrice") { m_basePrice = std::stoi(RIGHT); }
			if (LEFT == "width") { m_width = std::stoi(RIGHT); }
			if (LEFT == "height") { m_height = std::stoi(RIGHT); }
			if (LEFT == "avg24hPrice") { m_avg24hPrice = std::stoi(RIGHT); }
			if (LEFT == "low24hPrice") { m_low24hPrice = std::stoi(RIGHT); }
			if (LEFT == "lastOfferCount") { m_lastOfferCount = std::stoi(RIGHT); }
			for (auto& sf : TraderData::Instance()->GetList()) {
				if (LEFT == "Sell_" + sf.GetName()) {
					m_sellFor.emplace_back(std::make_pair(sf.GetID(), std::stoi(RIGHT)));
				}
			}
			if (LEFT == "Sell_Flea Market") {
				m_sellFor.emplace_back(std::make_pair(InvalidID, std::stoi(RIGHT)));
			}

			if (LEFT == "weight") { m_weight = std::stof(RIGHT); }
			if (LEFT == "fleaMarketFee") { m_fleaMarketFee = std::stoi(RIGHT); }
			if (LEFT == "propertiestype") { m_IsPreset = (RIGHT == "ItemPropertiesPreset"); }
		}
		void	Load_Sub() noexcept override {
			if (m_TypeID == InvalidID) {
				std::string ErrMes = "Error : Not Setting ItemType in Item ";
				ErrMes += GetShortName();
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
			else {
				auto* typePtr = ItemTypeData::Instance()->FindPtr(this->m_TypeID);
				auto* catPtr = ItemCategoryData::Instance()->FindPtr(typePtr->GetCategoryID());
				if (catPtr->GetName() == "Weapons") {
					m_isWeapon = true;
				}
				else if (catPtr->GetName() == "WeaponMods") {
					m_isWeaponMod = true;
				}
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		int										m_CheckJson{ 0 };
	public:
		const auto&	GetTypeID() const noexcept { return m_TypeID; }
		const auto&	GetMapID() const noexcept { return m_MapID; }
		const auto&	GetChildParts() const noexcept { return m_ChildPartsID; }
		const auto&	GetConflictParts() const noexcept { return m_ConflictPartsID; }

		const auto&	GetRecoil() const noexcept { return m_Recoil; }
		const auto&	GetErgonomics() const noexcept { return m_Ergonomics; }
		const auto&	GetSightRange() const noexcept { return m_SightRange; }



		const auto&	GetbasePrice() const noexcept { return m_basePrice; }
		const auto&	Getwidth() const noexcept { return m_width; }
		const auto&	Getheight() const noexcept { return m_height; }
		const auto&	Gettypes() const noexcept { return m_types; }
		const auto&	Getavg24hPrice() const noexcept { return m_avg24hPrice; }
		const auto&	Getlow24hPrice() const noexcept { return m_low24hPrice; }
		const auto&	GetlastOfferCount() const noexcept { return m_lastOfferCount; }
		const auto&	GetsellFor() const noexcept { return m_sellFor; }
		const auto&	Getweight() const noexcept { return m_weight; }
		const auto&	GetfleaMarketFee() const noexcept { return m_fleaMarketFee; }
		const auto&	GetIsPreset() const noexcept { return m_IsPreset; }
		const auto&	GetUseTaskID() const noexcept { return m_UseTaskID; }
	public:
		const auto	GetSellValue(TraderID* ID, int* pValue) const noexcept {
			*ID = InvalidID;
			*pValue = -1;
			for (const auto& sf : GetsellFor()) {
				auto basev = sf.second;
				if (sf.first == InvalidID) {
					basev -= GetfleaMarketFee();
				}
				if (*pValue < basev) {
					*pValue = basev;
					*ID = sf.first;
				}
			}
			return (*pValue != -1);
		}
		void		ResetTaskUseID(void) noexcept { this->m_UseTaskID.clear(); }
		void		AddTaskUseID(int ID) noexcept { this->m_UseTaskID.emplace_back(ID); }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
		void			SetOtherPartsID(const std::vector<ItemList>& itemList) noexcept {
			for (auto& cp : m_ChildPartsID) {
				for (auto& c : cp.Data) {
					for (const auto& t : itemList) {
						if (c.second == t.GetName()) {
							c.first = &t;
							break;
						}
					}
					if (c.first == nullptr) {
						std::string ErrMes = "Error : Invalid ChildPartsID[";
						ErrMes += GetShortName();
						ErrMes += "][";
						ErrMes += c.second;
						ErrMes += "]";

						DataErrorLog::Instance()->AddLog(ErrMes.c_str());
					}
				}
			}
			m_ParentPartsID.clear();
			for (const auto& t : itemList) {
				for (auto& cp : t.m_ChildPartsID) {
					for (auto& c : cp.Data) {
						if (c.first == this) {
							m_ParentPartsID.emplace_back(&t);
						}
					}
				}
			}
			//
			for (auto& cp : m_ChildPartsID) {
				for (const auto& c : cp.Data) {
					bool isHit = false;
					for (const auto& t : cp.TypeID) {
						if (c.first && t == c.first->GetTypeID()) {
							isHit = true;
							break;
						}
					}
					if (!isHit && c.first) {
						cp.TypeID.emplace_back(c.first->GetTypeID());
					}
				}
			}
			//
			for (auto& cp : m_ConflictPartsID) {
				for (const auto& t : itemList) {
					if (cp.second == t.GetName()) {
						cp.first = &t;
						break;
					}
				}
				if (cp.first == nullptr) {
					std::string ErrMes = "Error : Invalid ConflictPartsID[";
					ErrMes += GetShortName();
					ErrMes += "][";
					ErrMes += cp.second;
					ErrMes += "]";

					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
		void			SetOtherPartsID_After(const std::vector<ItemList>& itemList) noexcept {
			//é©ï™Çä±è¬ëäéËÇ…ÇµÇƒÇ¢ÇÈìzÇíTÇµÇƒÇªÇ¢Ç¬Ç‡ÉäÉXÉgÇ…ì¸ÇÍÇÈÅ@ëäévëäà§
			for (const auto& t : itemList) {
				for (auto& cp : t.GetConflictParts()) {
					if (cp.first == this) {
						m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
						m_ConflictPartsID.back().first = &t;
						m_ConflictPartsID.back().second = t.GetName();
					}
				}
			}
		}
	};

	enum class EnumItemProperties {
		ItemPropertiesAmmo,
		ItemPropertiesArmor,
		ItemPropertiesArmorAttachment,
		ItemPropertiesBackpack,
		ItemPropertiesBarrel,
		ItemPropertiesChestRig,
		ItemPropertiesContainer,
		ItemPropertiesFoodDrink,
		ItemPropertiesGlasses,
		ItemPropertiesGrenade,
		ItemPropertiesHelmet,
		ItemPropertiesKey,
		ItemPropertiesMagazine,
		ItemPropertiesMedicalItem,
		ItemPropertiesMelee,
		ItemPropertiesMedKit,
		ItemPropertiesNightVision,
		ItemPropertiesPainkiller,
		ItemPropertiesPreset,
		ItemPropertiesScope,
		ItemPropertiesSurgicalKit,
		ItemPropertiesWeapon,
		ItemPropertiesWeaponMod,
		ItemPropertiesStim,
		Max,
	};
	static const char* ItemPropertiesStr[(int)EnumItemProperties::Max] = {
		"ItemPropertiesAmmo",
		"ItemPropertiesArmor",
		"ItemPropertiesArmorAttachment",
		"ItemPropertiesBackpack",
		"ItemPropertiesBarrel",
		"ItemPropertiesChestRig",
		"ItemPropertiesContainer",
		"ItemPropertiesFoodDrink",
		"ItemPropertiesGlasses",
		"ItemPropertiesGrenade",
		"ItemPropertiesHelmet",
		"ItemPropertiesKey",
		"ItemPropertiesMagazine",
		"ItemPropertiesMedicalItem",
		"ItemPropertiesMelee",
		"ItemPropertiesMedKit",
		"ItemPropertiesNightVision",
		"ItemPropertiesPainkiller",
		"ItemPropertiesPreset",
		"ItemPropertiesScope",
		"ItemPropertiesSurgicalKit",
		"ItemPropertiesWeapon",
		"ItemPropertiesWeaponMod",
		"ItemPropertiesStim",
	};

	class properties {
		EnumItemProperties						m_Type{ EnumItemProperties::Max };
		std::array<int, 4>						m_IntParams{ 0,0,0,0 };
		std::array<float, 4>					m_floatParams{ 0,0,0,0 };
		std::vector<std::vector<std::string>>	m_ItemSlots;
	public:
		const auto*		GetTypeName() const noexcept { return (m_Type != EnumItemProperties::Max) ? ItemPropertiesStr[(int)m_Type] : ""; }
		const auto&		GetType() const noexcept { return m_Type; }
	public:
		const auto		GetStackMaxSize() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesAmmo) ? m_IntParams[0] : 0; }
		const auto		GetArmerClass() const noexcept {
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesArmor:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesArmorAttachment:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesChestRig:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesGlasses:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesHelmet:
				return m_IntParams[0];
			default:
				return 0;
			}
		}
		const auto		GetCapacity() const noexcept {
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesBackpack:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesChestRig:
				return m_IntParams[1];
			case EnumItemProperties::ItemPropertiesContainer:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesMagazine:
				return m_IntParams[0];
			default:
				return 0;
			}
		}
		const auto		GetEnergy() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesFoodDrink) ? m_IntParams[0] : 0; }
		const auto		GetHydration() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesFoodDrink) ? m_IntParams[1] : 0; }
		const auto		GetBlindnessProtection() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesGlasses) ? m_floatParams[0] : 0.f; }
		const auto		GetFragments() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesGrenade) ? m_IntParams[0] : 0; }
		const auto		GetUses() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesKey) ? m_IntParams[0] : 0; }
		const auto		GetUseTime() const noexcept {
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesMedicalItem:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesPainkiller:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesSurgicalKit:
				return m_IntParams[0];
			case EnumItemProperties::ItemPropertiesStim:
				return m_IntParams[0];
			default:
				return 0;
			}
		}
		const auto		GetSlashDamage() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? m_IntParams[0] : 0; }

		const auto		GetHitpoints() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? m_IntParams[0] : 0; }
		const auto		GetIntensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? m_IntParams[0] : 0; }
		const auto		GetDefault() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesPreset) ? m_IntParams[0] : 0; }
		const auto		GetSightingRange() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesScope) ? m_IntParams[0] : 0; }
		const auto		GetWeaponRecoilVertical() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? m_IntParams[0] : 0; }
		const auto		GetWeaponErgonomics() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? m_floatParams[0] : 0; }

		const auto		GetModRecoil() const noexcept {
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesBarrel:
				return  m_floatParams[0];
			case EnumItemProperties::ItemPropertiesMagazine:
				return  m_floatParams[0];
			case EnumItemProperties::ItemPropertiesScope:
				return  m_floatParams[0];
			case EnumItemProperties::ItemPropertiesWeaponMod:
				return  m_floatParams[0];
			default:
				return 0.f;
			}
		}
		const auto		GetModErgonomics() const noexcept {
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesBarrel:
				return  m_floatParams[1];
			case EnumItemProperties::ItemPropertiesMagazine:
				return  m_floatParams[1];
			case EnumItemProperties::ItemPropertiesScope:
				return  m_floatParams[1];
			case EnumItemProperties::ItemPropertiesWeaponMod:
				return  m_floatParams[1];
			default:
				return 0.f;
			}
		}

		const auto&		GetModSlots() const noexcept { return m_ItemSlots; }
	public:
		void GetJsonData(const nlohmann::json& data) {
			if (data.contains("__typename")) {
				if (!data["__typename"].is_null()) {
					std::string buf = data["__typename"];
					for (int i = 0; i < sizeof(ItemPropertiesStr) / sizeof(ItemPropertiesStr[0]); i++) {
						if (buf == ItemPropertiesStr[i]) {
							m_Type = (EnumItemProperties)i;
						}
					}
				}
			}
			//å¬ï 
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesAmmo:
				m_IntParams[0] = data["stackMaxSize"];
				break;
			case EnumItemProperties::ItemPropertiesArmor:
				if (!data["class"].is_null()) {
					m_IntParams[0] = data["class"];
				}
				else {
					m_IntParams[0] = 0;
				}
				break;
			case EnumItemProperties::ItemPropertiesArmorAttachment:
				if (!data["class"].is_null()) {
					m_IntParams[0] = data["class"];
				}
				else {
					m_IntParams[0] = 0;
				}
				break;
			case EnumItemProperties::ItemPropertiesBackpack:
				m_IntParams[0] = data["capacity"];
				break;
			case EnumItemProperties::ItemPropertiesBarrel:
				m_floatParams[0] = data["recoilModifier"];
				m_floatParams[1] = data["ergonomics"];
				m_ItemSlots.clear();
				for (const auto& s : data["slots"]) {
					m_ItemSlots.resize(m_ItemSlots.size() + 1);
					for (const auto& f : s["filters"]) {
						for (const auto& a : f) {
							for (const auto& n : a) {
								m_ItemSlots.back().emplace_back(n);
							}
						}
					}
				}
				break;
			case EnumItemProperties::ItemPropertiesChestRig:
				if (!data["class"].is_null()) {
					m_IntParams[0] = data["class"];
				}
				else {
					m_IntParams[0] = 0;
				}
				m_IntParams[1] = data["capacity"];
				break;
			case EnumItemProperties::ItemPropertiesContainer:
				m_IntParams[0] = data["capacity"];
				break;
			case EnumItemProperties::ItemPropertiesFoodDrink:
				m_IntParams[0] = data["energy"];
				m_IntParams[1] = data["hydration"];
				break;
			case EnumItemProperties::ItemPropertiesGlasses:
				if (!data["class"].is_null()) {
					m_IntParams[0] = data["class"];
				}
				else {
					m_IntParams[0] = 0;
				}
				m_floatParams[0] = data["blindnessProtection"];
				break;
			case EnumItemProperties::ItemPropertiesGrenade:
				m_IntParams[0] = data["fragments"];
				break;
			case EnumItemProperties::ItemPropertiesHelmet:
				if (!data["class"].is_null()) {
					m_IntParams[0] = data["class"];
				}
				else {
					m_IntParams[0] = 0;
				}
				break;
			case EnumItemProperties::ItemPropertiesKey:
				m_IntParams[0] = data["uses"];
				break;
			case EnumItemProperties::ItemPropertiesMagazine:
				m_IntParams[0] = data["capacity"];
				m_floatParams[0] = data["recoilModifier"];
				m_floatParams[1] = data["ergonomics"];
				m_ItemSlots.clear();
				for (const auto& s : data["slots"]) {
					m_ItemSlots.resize(m_ItemSlots.size() + 1);
					for (const auto& f : s["filters"]) {
						for (const auto& a : f) {
							for (const auto& n : a) {
								m_ItemSlots.back().emplace_back(n);
							}
						}
					}
				}
				break;
			case EnumItemProperties::ItemPropertiesMedicalItem:
				m_IntParams[0] = data["useTime"];
				break;
			case EnumItemProperties::ItemPropertiesMelee:
				m_IntParams[0] = data["slashDamage"];
				break;
			case EnumItemProperties::ItemPropertiesMedKit:
				m_IntParams[0] = data["hitpoints"];
				break;
			case EnumItemProperties::ItemPropertiesNightVision:
				m_floatParams[0] = data["intensity"];
				break;
			case EnumItemProperties::ItemPropertiesPainkiller:
				m_IntParams[0] = data["useTime"];
				break;
			case EnumItemProperties::ItemPropertiesPreset:
				m_IntParams[0] = data["default"];
				break;
			case EnumItemProperties::ItemPropertiesScope:
				m_IntParams[0] = data["sightingRange"];
				m_floatParams[0] = data["recoilModifier"];
				m_floatParams[1] = data["ergonomics"];
				m_ItemSlots.clear();
				for (const auto& s : data["slots"]) {
					m_ItemSlots.resize(m_ItemSlots.size() + 1);
					for (const auto& f : s["filters"]) {
						for (const auto& a : f) {
							for (const auto& n : a) {
								m_ItemSlots.back().emplace_back(n);
							}
						}
					}
				}
				break;
			case EnumItemProperties::ItemPropertiesSurgicalKit:
				m_IntParams[0] = data["useTime"];
				break;
			case EnumItemProperties::ItemPropertiesWeapon:
				m_IntParams[0] = data["recoilVertical"];
				m_floatParams[0] = data["ergonomics"];
				m_ItemSlots.clear();
				for (const auto& s : data["slots"]) {
					m_ItemSlots.resize(m_ItemSlots.size() + 1);
					for (const auto& f : s["filters"]) {
						for (const auto& a : f) {
							for (const auto& n : a) {
								m_ItemSlots.back().emplace_back(n);
							}
						}
					}
				}
				break;
			case EnumItemProperties::ItemPropertiesWeaponMod:
				m_floatParams[0] = data["recoilModifier"];
				m_floatParams[1] = data["ergonomics"];
				m_ItemSlots.clear();
				for (const auto& s : data["slots"]) {
					m_ItemSlots.resize(m_ItemSlots.size() + 1);
					for (const auto& f : s["filters"]) {
						for (const auto& a : f) {
							for (const auto& n : a) {
								m_ItemSlots.back().emplace_back(n);
							}
						}
					}
				}
				break;
			case EnumItemProperties::ItemPropertiesStim:
				m_IntParams[0] = data["useTime"];
				break;
			default:
				break;
			}
		}
	};

	class ItemJsonData {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
		std::string									shortName;
		std::string									description;
		int											basePrice{ 0 };
		int											width{ 0 };
		int											height{ 0 };
		std::vector<std::string>					types;
		int											avg24hPrice{ 0 };
		int											low24hPrice{ 0 };
		int											lastOfferCount{ 0 };
		std::vector<std::pair<std::string, int>>	sellFor;
		std::vector<std::pair<std::string, int>>	buyFor;
		std::string									categorytypes;
		float										weight{ 0.f };

		std::vector<std::string>					conflictingItems;

		std::vector<std::string>					usedInTasks;
		std::vector<std::string>					receivedFromTasks;
		std::vector<std::string>					bartersFor;
		std::vector<std::string>					bartersUsing;
		std::vector<std::string>					craftsFor;
		std::vector<std::string>					craftsUsing;
		int											fleaMarketFee{ 0 };

		properties									m_properties;
	public:
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];
			shortName = data["shortName"];
			if (data.contains("description")) {
				if (!data["description"].is_null()) {
					description = data["description"];
				}
			}
			if (data.contains("basePrice")) {
				if (!data["basePrice"].is_null()) {
					basePrice = data["basePrice"];
				}
			}
			if (data.contains("width")) {
				width = data["width"];
			}
			if (data.contains("height")) {
				height = data["height"];
			}
			if (data.contains("types")) {
				for (const auto& ts : data["types"]) {
					types.emplace_back((std::string)ts);
				}
			}
			if (data.contains("avg24hPrice")) {
				avg24hPrice = data["avg24hPrice"];
			}

			if (data.contains("low24hPrice")) {
				if (!data["low24hPrice"].is_null()) {
					low24hPrice = data["low24hPrice"];
				}
			}

			if (data.contains("lastOfferCount")) {
				if (!data["lastOfferCount"].is_null()) {
					lastOfferCount = data["lastOfferCount"];
				}
			}
			for (const auto& sf : data["sellFor"]) {
				std::string vendor = sf["vendor"]["name"];
				int price = sf["price"];
				sellFor.emplace_back(std::make_pair(vendor, price));
			}
			for (const auto& sf : data["buyFor"]) {
				std::string vendor = sf["vendor"]["name"];
				int price = sf["price"];
				buyFor.emplace_back(std::make_pair(vendor, price));
			}
			categorytypes = data["category"]["name"];
			weight = data["weight"];
			for (const auto& ts : data["conflictingItems"]) {
				conflictingItems.emplace_back((std::string)ts["name"]);
			}
			for (const auto& ts : data["usedInTasks"]) {
				usedInTasks.emplace_back((std::string)ts["name"]);
			}
			for (const auto& ts : data["receivedFromTasks"]) {
				receivedFromTasks.emplace_back((std::string)ts["name"]);
			}
			for (const auto& ts : data["bartersFor"]) {
				bartersFor.emplace_back((std::string)ts["trader"]["name"]);
			}
			for (const auto& ts : data["bartersUsing"]) {
				bartersUsing.emplace_back((std::string)ts["trader"]["name"]);
			}
			for (const auto& ts : data["craftsFor"]) {
				craftsFor.emplace_back((std::string)ts["station"]["name"]);
			}
			for (const auto& ts : data["craftsUsing"]) {
				craftsUsing.emplace_back((std::string)ts["station"]["name"]);
			}
			if (data.contains("fleaMarketFee")) {
				if (!data["fleaMarketFee"].is_null()) {
					fleaMarketFee = data["fleaMarketFee"];
				}
			}

			if (data.contains("properties")) {
				if (!data["properties"].is_null()) {
					m_properties.GetJsonData(data["properties"]);
				}
			}
		}
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
					std::string Path2 = Path + d.cFileName + "/";
					auto data_t2 = GetFileNamesInDirectory(Path2.c_str());
					for (auto& d2 : data_t2) {
						if (d2.cFileName[0] != '.') {
							SetList((Path2 + d2.cFileName + "/").c_str());
						}
					}
				}
			}
			for (auto& t : m_List) {
				t.SetOtherPartsID(m_List);
			}
			for (auto& t : m_List) {
				t.SetOtherPartsID_After(m_List);
			}
			for (auto& t : m_List) {
				t.m_CheckJson = 0;
			}
		}
		~ItemData() noexcept {}
	private:
		std::vector<ItemJsonData> m_ItemJsonData;
	public:
		void GetJsonData(nlohmann::json& data) {
			m_ItemJsonData.clear();
			for (const auto& d : data["data"]["items"]) {
				m_ItemJsonData.resize(m_ItemJsonData.size() + 1);
				m_ItemJsonData.back().GetJsonData(d);
				m_ItemJsonData.back().m_IsFileOpened = false;
			}
		}
		void SaveDatabyJson() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : m_ItemJsonData) {
					if (L.GetIDstr() == jd.id) {
						L.m_CheckJson++;

						jd.m_IsFileOpened = true;

						std::ofstream outputfile(L.GetFilePath());
						outputfile << "IDstr=" + jd.id + "\n";
						outputfile << "Name=" + jd.name + "\n";
						outputfile << "ShortName=" + jd.shortName + "\n";
						outputfile << "Itemtype=" + jd.categorytypes + "\n";

						//ä˘ë∂ÇÃÇ‡ÇÃÇï€éùÇµÇƒÇ®Ç≠1
						for (auto& m : L.GetMapID()) {
							auto* ptr = MapData::Instance()->FindPtr(m);
							if (ptr) {
								outputfile << "Map=" + ptr->GetName() + "\n";
							}
						}
						if (L.GetSightRange() >= 0) {
							outputfile << "SightRange=" + std::to_string(L.GetSightRange()) + "\n";
						}


						if (jd.m_properties.GetModSlots().size() > 0) {
							for (const auto& m : jd.m_properties.GetModSlots()) {
								if (m.size() > 0) {
									outputfile << "ChildParts=[\n";
									std::vector<std::string> Names;
									for (auto& d : m) {
										auto NmBuf = d;
										if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
											outputfile << "\t" + NmBuf + ((&d != &m.back()) ? "," : "") + "\n";
											Names.emplace_back(NmBuf);
										}
									}
									outputfile << "]\n";
								}
							}
						}
						else {
							for (auto& m : L.GetChildParts()) {
								outputfile << "ChildParts=[\n";
								std::vector<std::string> Names;
								for (auto& d : m.Data) {
									if (d.first) {
										auto NmBuf = d.first->GetName();
										if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
											outputfile << "\t" + NmBuf + ((&d != &m.Data.back()) ? "," : "") + "\n";
											Names.emplace_back(NmBuf);
										}
									}
									else {
										//int a = 0;
									}
								}
								outputfile << "]\n";
							}
						}

						if (jd.conflictingItems.size() > 0) {
							outputfile << "Conflict=[\n";
							std::vector<std::string> Names;
							for (auto& m : jd.conflictingItems) {
								auto NmBuf = m;
								if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
									outputfile << "\t" + NmBuf + ((&m != &jd.conflictingItems.back()) ? "," : "") + "\n";
									Names.emplace_back(NmBuf);
								}
							}
							outputfile << "]\n";
						}
						else {
							if (L.GetConflictParts().size() > 0) {
								outputfile << "Conflict=[\n";
								std::vector<std::string> Names;
								for (auto& m : L.GetConflictParts()) {
									if (m.first) {
										auto NmBuf = m.first->GetName();
										if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
											outputfile << "\t" + NmBuf + ((&m != &L.GetConflictParts().back()) ? "," : "") + "\n";
											Names.emplace_back(NmBuf);
										}
									}
									else {
										//int a = 0;
									}
								}
								outputfile << "]\n";
							}
						}

						if (jd.m_properties.GetType() == EnumItemProperties::ItemPropertiesWeapon) {
							outputfile << "Recoil=" + std::to_string((float)jd.m_properties.GetWeaponRecoilVertical()) + "\n";
							outputfile << "Ergonomics=" + std::to_string(jd.m_properties.GetWeaponErgonomics()) + "\n";
						}
						else {
							switch (jd.m_properties.GetType()) {
							case EnumItemProperties::ItemPropertiesBarrel:
							case EnumItemProperties::ItemPropertiesMagazine:
							case EnumItemProperties::ItemPropertiesScope:
							case EnumItemProperties::ItemPropertiesWeaponMod:
								outputfile << "Recoil=" + std::to_string((float)jd.m_properties.GetModRecoil()*100.f) + "\n";
								outputfile << "Ergonomics=" + std::to_string(jd.m_properties.GetModErgonomics()) + "\n";
								break;
							default:
								break;
							}
						}

						outputfile << "Information_Eng=" + jd.description + "\n";
						outputfile << "basePrice=" + std::to_string(jd.basePrice) + "\n";
						outputfile << "width=" + std::to_string(jd.width) + "\n";
						outputfile << "height=" + std::to_string(jd.height) + "\n";
						outputfile << "avg24hPrice=" + std::to_string(jd.avg24hPrice) + "\n";
						outputfile << "low24hPrice=" + std::to_string(jd.low24hPrice) + "\n";
						outputfile << "lastOfferCount=" + std::to_string(jd.lastOfferCount) + "\n";
						for (auto& sf : jd.sellFor) {
							outputfile << "Sell_" + sf.first + "=" + std::to_string(sf.second) + "\n";
						}
						//for (auto& bf : jd.buyFor) { outputfile << "Buy_" + bf.first + "=" + std::to_string(bf.second) + "\n"; }
						outputfile << "weight=" + std::to_string(jd.weight) + "\n";
						//std::vector<std::string>					usedInTasks;
						//std::vector<std::string>					receivedFromTasks;
						//std::vector<std::string>					bartersFor;
						//std::vector<std::string>					bartersUsing;
						//std::vector<std::string>					craftsFor;
						//std::vector<std::string>					craftsUsing;
						outputfile << "fleaMarketFee=" + std::to_string(jd.fleaMarketFee) + "\n";
						outputfile << "propertiestype=" + (std::string)(jd.m_properties.GetTypeName()) + "\n";
						outputfile.close();
						break;
					}
				}
			}
			for (auto& jd : m_ItemJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string FolderPath = jd.categorytypes;

					std::string ParentPath = "data/item/Maked/Maked/" + jd.categorytypes;

					CreateDirectory(ParentPath.c_str(), NULL);

					std::string ItemName = jd.name;
					auto SubStrs = [&](const char* str) {
						while (true) {
							auto now = ItemName.find(str);
							if (now != std::string::npos) {
								ItemName = ItemName.substr(0, now) + ItemName.substr(now + 1);
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

					std::string Name = ParentPath + "/" + ItemName + ".txt";
					std::ofstream outputfile(Name);
					outputfile << "IDstr=" + jd.id + "\n";
					outputfile << "Name=" + jd.name + "\n";
					outputfile << "ShortName=" + jd.shortName + "\n";
					outputfile << "Itemtype=" + jd.categorytypes + "\n";

					//ä˘ë∂ÇÃÇ‡ÇÃÇï€éùÇµÇƒÇ®Ç≠1
					/*
					for (auto& m : L.GetMapID()) {
						auto* ptr = MapData::Instance()->FindPtr(m);
						if (ptr) {
							outputfile << "Map=" + ptr->GetName() + "\n";
						}
					}
					if (L.GetSightRange() >= 0) {
						outputfile << "SightRange=" + std::to_string(L.GetSightRange()) + "\n";
					}
					if (L.GetConflictParts().size() > 0) {
						outputfile << "Conflict=[\n";
						for (auto& m : L.GetConflictParts()) {
							outputfile << "\t" + m.first->GetName() + ((&m != &L.GetConflictParts().back()) ? "," : "") + "\n";
						}
						outputfile << "]\n";
					}
					//*/
					if (jd.m_properties.GetModSlots().size() > 0) {
						for (const auto& m : jd.m_properties.GetModSlots()) {
							if (m.size() > 0) {
								outputfile << "ChildParts=[\n";
								std::vector<std::string> Names;
								for (auto& d : m) {
									auto NmBuf = d;
									if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
										outputfile << "\t" + NmBuf + ((&d != &m.back()) ? "," : "") + "\n";
										Names.emplace_back(NmBuf);
									}
								}
								outputfile << "]\n";
							}
						}
					}

					if (jd.m_properties.GetType() == EnumItemProperties::ItemPropertiesWeapon) {
						outputfile << "Recoil=" + std::to_string((float)jd.m_properties.GetWeaponRecoilVertical()) + "\n";
						outputfile << "Ergonomics=" + std::to_string(jd.m_properties.GetWeaponErgonomics()) + "\n";
					}
					else {
						switch (jd.m_properties.GetType()) {
						case EnumItemProperties::ItemPropertiesBarrel:
						case EnumItemProperties::ItemPropertiesMagazine:
						case EnumItemProperties::ItemPropertiesScope:
						case EnumItemProperties::ItemPropertiesWeaponMod:
							outputfile << "Recoil=" + std::to_string((float)jd.m_properties.GetModRecoil()*100.f) + "\n";
							outputfile << "Ergonomics=" + std::to_string(jd.m_properties.GetModErgonomics()) + "\n";
							break;
						default:
							break;
						}
					}

					outputfile << "Information_Eng=" + jd.description + "\n";
					outputfile << "basePrice=" + std::to_string(jd.basePrice) + "\n";
					outputfile << "width=" + std::to_string(jd.width) + "\n";
					outputfile << "height=" + std::to_string(jd.height) + "\n";
					outputfile << "avg24hPrice=" + std::to_string(jd.avg24hPrice) + "\n";
					outputfile << "low24hPrice=" + std::to_string(jd.low24hPrice) + "\n";
					outputfile << "lastOfferCount=" + std::to_string(jd.lastOfferCount) + "\n";
					for (auto& sf : jd.sellFor) {
						outputfile << "Sell_" + sf.first + "=" + std::to_string(sf.second) + "\n";
					}
					//for (auto& bf : jd.buyFor) { outputfile << "Buy_" + bf.first + "=" + std::to_string(bf.second) + "\n"; }
					outputfile << "weight=" + std::to_string(jd.weight) + "\n";
					//std::vector<std::string>					usedInTasks;
					//std::vector<std::string>					receivedFromTasks;
					//std::vector<std::string>					bartersFor;
					//std::vector<std::string>					bartersUsing;
					//std::vector<std::string>					craftsFor;
					//std::vector<std::string>					craftsUsing;
					outputfile << "fleaMarketFee=" + std::to_string(jd.fleaMarketFee) + "\n";
					outputfile << "propertiestype=" + (std::string)(jd.m_properties.GetTypeName()) + "\n";
					outputfile.close();

					//RemoveDirectory(Path.c_str());
				}
			}
		}
		void CheckThroughJson(void) noexcept {
			for (auto& t : m_List) {
				if (t.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : ";
					ErrMes += t.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			for (auto& t : m_List) {
				if (t.m_CheckJson >= 2) {
					std::string ErrMes = "Error : Be repeated ";
					ErrMes += std::to_string(t.m_CheckJson);
					ErrMes += " : ";
					ErrMes += t.GetName();

					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
	};


	class ItemGetData : public GetDataParent<ItemID> {};
};