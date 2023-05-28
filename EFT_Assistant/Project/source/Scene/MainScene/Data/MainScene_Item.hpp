#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	//
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

	//
	class ItemList : public ListParent<ItemID> {
	public:
		class ItemProperties {
			class ChildItemSettings {
				std::vector<ItemTypeID>									m_TypeID;
			public:
				std::vector<IDParents<ItemID>>				m_Data;
			public:
				void			SetTypeID(ItemTypeID TypeID) noexcept {
					bool isHit = false;
					for (const auto& t : this->m_TypeID) {
						if (t == TypeID) {
							isHit = true;
							break;
						}
					}
					if (!isHit) {
						this->m_TypeID.emplace_back(TypeID);
					}
				}
				const auto&		GetTypeID() const noexcept { return this->m_TypeID; }
			};
		private:
			EnumItemProperties						m_Type{ EnumItemProperties::Max };
			std::array<int, 4>						m_IntParams{ 0,0,0,0 };
			std::array<float, 4>					m_floatParams{ 0,0,0,0 };
			std::vector<ChildItemSettings>			m_ChildPartsID;
			std::vector<IDParents<ItemID>>			m_ConflictPartsID;
		public:
			const auto*		GetTypeName() const noexcept { return (m_Type != EnumItemProperties::Max) ? ItemPropertiesStr[(int)m_Type] : ""; }
			const auto&		GetType() const noexcept { return this->m_Type; }
		public:
			const auto		GetStackMaxSize() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesAmmo) ? this->m_IntParams[0] : 0; }
			const auto		GetArmerClass() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesArmor:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesArmorAttachment:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesChestRig:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesGlasses:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesHelmet:
					return this->m_IntParams[0];
				default:
					return 0;
				}
			}
			const auto		GetCapacity() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBackpack:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesChestRig:
					return this->m_IntParams[1];
				case EnumItemProperties::ItemPropertiesContainer:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesMagazine:
					return this->m_IntParams[0];
				default:
					return 0;
				}
			}
			const auto		GetEnergy() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesFoodDrink) ? this->m_IntParams[0] : 0; }
			const auto		GetHydration() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesFoodDrink) ? this->m_IntParams[1] : 0; }
			const auto		GetBlindnessProtection() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesGlasses) ? this->m_floatParams[0] : 0.f; }
			const auto		GetFragments() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesGrenade) ? this->m_IntParams[0] : 0; }
			const auto		GetUses() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesKey) ? this->m_IntParams[0] : 0; }
			const auto		GetUseTime() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesMedicalItem:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesPainkiller:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesSurgicalKit:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesStim:
					return this->m_IntParams[0];
				default:
					return 0;
				}
			}
			const auto		GetSightingRange() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesScope:
					return this->m_IntParams[1];
				case EnumItemProperties::ItemPropertiesWeapon:
					return this->m_IntParams[1];
				default:
					return -100;
				}
			}
			const auto		GetSlashDamage() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? this->m_IntParams[0] : 0; }

			const auto		GetHitpoints() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[0] : 0; }
			const auto		GetIntensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? this->m_IntParams[0] : 0; }
			const auto		GetDefault() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesPreset) ? this->m_IntParams[0] : 0; }
			const auto		GetWeaponRecoilVertical() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[0] : 0; }
			const auto		GetWeaponErgonomics() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[0] : 0; }

			const auto		GetModRecoil() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
					return  this->m_floatParams[0];
				case EnumItemProperties::ItemPropertiesMagazine:
					return  this->m_floatParams[0];
				case EnumItemProperties::ItemPropertiesScope:
					return  this->m_floatParams[0];
				case EnumItemProperties::ItemPropertiesWeaponMod:
					return  this->m_floatParams[0];
				default:
					return 0.f;
				}
			}
			const auto		GetModErgonomics() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
					return  this->m_floatParams[1];
				case EnumItemProperties::ItemPropertiesMagazine:
					return  this->m_floatParams[1];
				case EnumItemProperties::ItemPropertiesScope:
					return  this->m_floatParams[1];
				case EnumItemProperties::ItemPropertiesWeaponMod:
					return  this->m_floatParams[1];
				default:
					return 0.f;
				}
			}

			auto&			SetModSlots() noexcept { return this->m_ChildPartsID; }
			const auto&		GetModSlots() const noexcept { return this->m_ChildPartsID; }

			auto&			SetConflictPartsID() noexcept { return this->m_ConflictPartsID; }
			const auto&		GetConflictPartsID() const noexcept { return this->m_ConflictPartsID; }
		private:
			void			SetType(std::string_view value) noexcept {
				for (int i = 0; i < (int)EnumItemProperties::Max; i++) {
					if (value == ItemPropertiesStr[i]) {
						m_Type = (EnumItemProperties)i;
						break;
					}
				}
			}
			void			SetRecoil(float value) noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
					m_floatParams[0] = value / 100.f;
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					m_floatParams[0] = value / 100.f;
					break;
				case EnumItemProperties::ItemPropertiesScope:
					m_floatParams[0] = value / 100.f;
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					m_floatParams[0] = value / 100.f;
					break;
					//
				case EnumItemProperties::ItemPropertiesWeapon:
					m_IntParams[0] = (int)value;
					break;
					//
				default:
					break;
				}
			}
			void			SetErgonomics(float value) noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
					m_floatParams[1] = value;
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					m_floatParams[1] = value;
					break;
				case EnumItemProperties::ItemPropertiesScope:
					m_floatParams[1] = value;
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					m_floatParams[1] = value;
					break;
					//
				case EnumItemProperties::ItemPropertiesWeapon:
					m_floatParams[0] = value;
					break;
					//
				default:
					break;
				}
			}
			void			SetSightingRange(int value) noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesScope:
					m_IntParams[1] = value;
					break;
				case EnumItemProperties::ItemPropertiesWeapon:
					m_IntParams[1] = value;
					break;
				default:
					break;
				}
			}
		public:
			void			GetJsonData(const nlohmann::json& data) {
				if (data.contains("__typename") && !data["__typename"].is_null()) {
					SetType((std::string)data["__typename"]);
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
					m_ChildPartsID.clear();
					for (const auto& s : data["slots"]) {
						m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
						for (const auto& f : s["filters"]) {
							for (const auto& a : f) {
								for (const auto& n : a) {
									m_ChildPartsID.back().m_Data.resize(m_ChildPartsID.back().m_Data.size() + 1);
									m_ChildPartsID.back().m_Data.back().SetName(n);
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

					m_ChildPartsID.clear();
					for (const auto& s : data["slots"]) {
						m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
						for (const auto& f : s["filters"]) {
							for (const auto& a : f) {
								for (const auto& n : a) {
									m_ChildPartsID.back().m_Data.resize(m_ChildPartsID.back().m_Data.size() + 1);
									m_ChildPartsID.back().m_Data.back().SetName(n);
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
					m_ChildPartsID.clear();
					for (const auto& s : data["slots"]) {
						m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
						for (const auto& f : s["filters"]) {
							for (const auto& a : f) {
								for (const auto& n : a) {
									m_ChildPartsID.back().m_Data.resize(m_ChildPartsID.back().m_Data.size() + 1);
									m_ChildPartsID.back().m_Data.back().SetName(n);
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
					m_ChildPartsID.clear();
					for (const auto& s : data["slots"]) {
						m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
						for (const auto& f : s["filters"]) {
							for (const auto& a : f) {
								for (const auto& n : a) {
									m_ChildPartsID.back().m_Data.resize(m_ChildPartsID.back().m_Data.size() + 1);
									m_ChildPartsID.back().m_Data.back().SetName(n);
								}
							}
						}
					}
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					m_floatParams[0] = data["recoilModifier"];
					m_floatParams[1] = data["ergonomics"];
					m_ChildPartsID.clear();
					for (const auto& s : data["slots"]) {
						m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
						for (const auto& f : s["filters"]) {
							for (const auto& a : f) {
								for (const auto& n : a) {
									m_ChildPartsID.back().m_Data.resize(m_ChildPartsID.back().m_Data.size() + 1);
									m_ChildPartsID.back().m_Data.back().SetName(n);
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
			void			SetData(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "propertiestype") { this->SetType(Args[0]); }
				else if (LEFT == "Recoil") { this->SetRecoil(std::stof(Args[0])); }
				else if (LEFT == "Ergonomics") { this->SetErgonomics(std::stof(Args[0])); }
				else if (LEFT == "SightRange") { this->SetSightingRange(std::stoi(Args[0])); }
				else if (LEFT == "ChildParts") {
					this->m_ChildPartsID.resize(this->m_ChildPartsID.size() + 1);
					auto& CP = this->m_ChildPartsID.back().m_Data;
					for (auto&a : Args) {
						bool isHit = false;
						for (auto& d : CP) {
							if (d.GetName() == a) {
								isHit = true;
								break;
							}
						}
						if (!isHit) {
							CP.resize(CP.size() + 1);
							CP.back().SetName(a);
						}
					}
				}
				else if (LEFT == "Conflict") {
					for (auto&a : Args) {
						bool isHit = false;
						for (auto& d : this->m_ConflictPartsID) {
							if (d.GetName() == a) {
								isHit = true;
								break;
							}
						}
						if (!isHit) {
							m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
							m_ConflictPartsID.back().SetName(a);
						}
					}
				}
			}
			void			OutputData(std::ofstream& outputfile) noexcept;
		};
		struct ItemsData {
			//ÉfÅ[É^
			IDParents<ItemTypeID>								m_TypeID;
			std::vector<IDParents<MapID>>						m_MapID;
			int													m_width{ 0 };
			int													m_height{ 0 };
			float												m_weight{ 0.f };
			std::vector<std::pair<IDParents<TraderID>, int>>	m_sellFor;
			int													m_fleaMarketFee{ 0 };
			ItemList::ItemProperties							m_properties;
			//í«â¡èÓïÒ
			std::vector<ItemID>									m_ParentPartsID;
			bool												m_isWeapon{ false };
			bool												m_isWeaponMod{ false };
			std::vector<TaskID>									m_UseTaskID;
		};
	private:
		ItemsData												m_ItemsData;
	public:
		int														m_CheckJson{ 0 };
	private:
		//í«â¡ê›íË
		void		SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override;
		void		Load_Sub() noexcept override;
		void		WhenAfterLoad_Sub() noexcept override;
	public:
		const auto&	GetTypeID() const noexcept { return this->m_ItemsData.m_TypeID.GetID(); }
		const auto&	GetMapID() const noexcept { return this->m_ItemsData.m_MapID; }
		const auto&	GetChildParts() const noexcept { return this->m_ItemsData.m_properties.GetModSlots(); }
		const auto&	GetConflictParts() const noexcept { return this->m_ItemsData.m_properties.GetConflictPartsID(); }
		const auto&	Getwidth() const noexcept { return this->m_ItemsData.m_width; }
		const auto&	Getheight() const noexcept { return this->m_ItemsData.m_height; }
		const auto&	GetsellFor() const noexcept { return this->m_ItemsData.m_sellFor; }
		const auto&	Getweight() const noexcept { return this->m_ItemsData.m_weight; }
		const auto&	GetfleaMarketFee() const noexcept { return this->m_ItemsData.m_fleaMarketFee; }
		const auto&	GetUseTaskID() const noexcept { return this->m_ItemsData.m_UseTaskID; }
	public:
		const auto	GetRecoil() const noexcept {
			switch (this->m_ItemsData.m_properties.GetType()) {
			case EnumItemProperties::ItemPropertiesBarrel:
			case EnumItemProperties::ItemPropertiesMagazine:
			case EnumItemProperties::ItemPropertiesScope:
			case EnumItemProperties::ItemPropertiesWeaponMod:
				return ((float)m_ItemsData.m_properties.GetModRecoil()*100.f);
				//
			case EnumItemProperties::ItemPropertiesWeapon:
				return (float)m_ItemsData.m_properties.GetWeaponRecoilVertical();
				//
			default:
				return 0.f;
			}
		}
		const auto	GetErgonomics() const noexcept {
			switch (this->m_ItemsData.m_properties.GetType()) {
			case EnumItemProperties::ItemPropertiesBarrel:
			case EnumItemProperties::ItemPropertiesMagazine:
			case EnumItemProperties::ItemPropertiesScope:
			case EnumItemProperties::ItemPropertiesWeaponMod:
				return this->m_ItemsData.m_properties.GetModErgonomics();
				//
			case EnumItemProperties::ItemPropertiesWeapon:
				return this->m_ItemsData.m_properties.GetWeaponErgonomics();
				//
			default:
				return 0.f;
			}
		}
		const auto	GetSightRange() const noexcept { return this->m_ItemsData.m_properties.GetSightingRange(); }
		const auto	GetIsPreset() const noexcept { return this->m_ItemsData.m_properties.GetType() == EnumItemProperties::ItemPropertiesPreset; }
	public:
		void		SetParent() noexcept;
		const auto	GetSellValue(TraderID* ID, int* pValue) const noexcept {
			*ID = InvalidID;
			*pValue = -1;
			for (const auto& sf : GetsellFor()) {
				auto basev = sf.second;
				if (sf.first.GetID() == InvalidID) {
					basev -= GetfleaMarketFee();
				}
				if (*pValue < basev) {
					*pValue = basev;
					*ID = sf.first.GetID();
				}
			}
			return (*pValue != -1);
		}
		void		ResetTaskUseID(void) noexcept { this->m_ItemsData.m_UseTaskID.clear(); }
		void		AddTaskUseID(TaskID ID) noexcept { this->m_ItemsData.m_UseTaskID.emplace_back(ID); }
	public:
		const int	Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) const noexcept;
		void		DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
	};

	class ItemJsonData :public JsonDataParent {
	public:
		ItemList::ItemsData							m_ItemsData;
		std::string									m_categorytypes;
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
			for (auto& t : this->m_List) {
				t.m_CheckJson = 0;
			}
		}
		~ItemData() noexcept {}
	private:
		std::vector<ItemJsonData>	m_ItemJsonData;
	public:
		void SetParent()noexcept {
			for (auto& L : this->m_List) {
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
			for (auto& L : this->m_List) {
				for (auto& jd : this->m_ItemJsonData) {
					if (L.GetIDstr() == jd.m_id) {
						L.m_CheckJson++;

						jd.OutputData(L.GetFilePath());

						//ä˘ë∂ÇÃÇ‡ÇÃÇï€éùÇµÇƒÇ®Ç≠
						std::ofstream outputfile(L.GetFilePath(), std::ios::app);
						for (auto& m : L.GetMapID()) {
							auto* ptr = MapData::Instance()->FindPtr(m.GetID());
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
			for (auto& jd : this->m_ItemJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string ParentPath = Path + jd.m_categorytypes;

					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}

					std::string ChildPath = ParentPath + "/";

					std::string FileName = jd.m_name;
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
			for (auto& t : this->m_List) {
				if (t.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : ";
					ErrMes += t.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			for (auto& t : this->m_List) {
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