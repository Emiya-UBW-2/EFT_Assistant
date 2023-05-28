#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
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

		int														m_SightRange{ -100 };

		int														m_basePrice{ 0 };
		int														m_width{ 0 };
		int														m_height{ 0 };
		std::vector<std::string>								m_types;
		std::vector<std::pair<TraderID, int>>					m_sellFor;
		float													m_weight{ 0.f };
		int														m_fleaMarketFee{ 0 };
		bool													m_IsPreset{ false };
		std::vector<TaskID>										m_UseTaskID;
	private:
		std::string												m_TypeArg;
		std::vector<std::string>								m_MapArgs;
		std::vector<std::pair<std::string, std::string>>		m_selArgs;
	public:
		int														m_CheckJson{ 0 };
	private:
		//í«â¡ê›íË
		void		SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override;
		void		Load_Sub() noexcept override;
		void		WhenAfterLoad_Sub() noexcept override;
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
		const auto&	GetsellFor() const noexcept { return m_sellFor; }
		const auto&	Getweight() const noexcept { return m_weight; }
		const auto&	GetfleaMarketFee() const noexcept { return m_fleaMarketFee; }
		const auto&	GetIsPreset() const noexcept { return m_IsPreset; }
		const auto&	GetUseTaskID() const noexcept { return m_UseTaskID; }
	public:
		void		SetParent() noexcept;
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
		void		AddTaskUseID(TaskID ID) noexcept { this->m_UseTaskID.emplace_back(ID); }
	public:
		const int	Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) const noexcept;
		void		DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
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
		const auto		GetSightingRange() const noexcept {
			switch (m_Type) {
			case EnumItemProperties::ItemPropertiesScope:
				return m_IntParams[1];
			case EnumItemProperties::ItemPropertiesWeapon:
				return m_IntParams[1];
			default:
				return -100;
			}
		}
		const auto		GetSlashDamage() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? m_IntParams[0] : 0; }

		const auto		GetHitpoints() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? m_IntParams[0] : 0; }
		const auto		GetIntensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? m_IntParams[0] : 0; }
		const auto		GetDefault() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesPreset) ? m_IntParams[0] : 0; }
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
				m_IntParams[1] = (data.contains("sightingRange")) ? (int)data["sightingRange"] : -100;
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

	class ItemJsonData :public JsonDataParent {
	public:
		std::string									shortName;
		std::string									description;
		int											basePrice{ 0 };
		int											width{ 0 };
		int											height{ 0 };
		std::vector<std::string>					types;
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
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};

	class ItemData : public SingletonBase<ItemData>, public DataParent<ItemID, ItemList> {
	private:
		friend class SingletonBase<ItemData>;
	private:
		ItemData() noexcept {
			std::string Path = "data/item/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				std::string Path2 = Path + RetPath2 + "/";
				GetDirList(Path2.c_str(), [&](const char* RetPath3) {
					SetDirList((Path2 + RetPath3 + "/").c_str());
				});
			});
			for (auto& t : m_List) {
				t.m_CheckJson = 0;
			}
		}
		~ItemData() noexcept {}
	private:
		std::vector<ItemJsonData> m_ItemJsonData;
	public:
		void SetParent()noexcept {
			for (auto& L : m_List) {
				L.SetParent();
			}
		}
		void GetJsonData(nlohmann::json& data) {
			m_ItemJsonData.clear();
			for (const auto& d : data["data"]["items"]) {
				m_ItemJsonData.resize(m_ItemJsonData.size() + 1);
				m_ItemJsonData.back().GetJson(d);
			}
		}
		void UpdateData() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : m_ItemJsonData) {
					if (L.GetIDstr() == jd.id) {
						L.m_CheckJson++;

						jd.OutputData(L.GetFilePath());

						//ä˘ë∂ÇÃÇ‡ÇÃÇï€éùÇµÇƒÇ®Ç≠
						std::ofstream outputfile(L.GetFilePath(), std::ios::app);
						for (auto& m : L.GetMapID()) {
							auto* ptr = MapData::Instance()->FindPtr(m);
							if (ptr) {
								outputfile << "Map=" + ptr->GetName() + "\n";
							}
						}
						outputfile.close();
						break;
					}
				}
			}
		}
		void SaveAsNewData2(std::string Path) noexcept {
			bool maked = false;
			for (auto& jd : m_ItemJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string ParentPath = Path + jd.categorytypes;

					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}

					std::string ChildPath = ParentPath + "/";

					std::string FileName = jd.name;
					SubStrs(&FileName, ".");
					SubStrs(&FileName, "\\");
					SubStrs(&FileName, "/");
					SubStrs(&FileName, ":");
					SubStrs(&FileName, "*");
					SubStrs(&FileName, "?");
					SubStrs(&FileName, "\"");
					SubStrs(&FileName, ">");
					SubStrs(&FileName, "<");
					SubStrs(&FileName, "|");
					std::string Name = FileName + ".txt";

					jd.OutputData(ChildPath + Name);
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
};