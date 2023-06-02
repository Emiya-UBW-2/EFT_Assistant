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
				std::vector<ItemTypeID>						m_TypeID;
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
			public:
				void		operator=(const ChildItemSettings& tgt) noexcept {
					this->m_TypeID.resize(tgt.m_TypeID.size());
					for (const auto& m : tgt.m_TypeID) {
						this->m_TypeID.at(&m - &tgt.m_TypeID.front()) = m;
					}
					this->m_Data.resize(tgt.m_Data.size());
					for (const auto& m : tgt.m_Data) {
						this->m_Data.at(&m - &tgt.m_Data.front()) = m;
					}
				}
			};
		private:
			EnumItemProperties						m_Type{ EnumItemProperties::Max };
			std::array<int, 7>						m_IntParams{ 0,0,0,0,0,0,0 };
			std::array<float, 6>					m_floatParams{ 0,0,0,0,0,0 };
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
			const auto		GetWeaponRecoilHorizontal() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[2] : 0; }
			const auto		GetWeaponFireRate() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[3] : 0; }
			const auto		GetWeaponcenterOfImpact() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[1] : 0.f; }
			const auto		GetWeapondeviationCurve() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[2] : 0.f; }
			const auto		GetWeaponrecoilDispersion() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[4] : 0; }
			const auto		GetWeaponrecoilAngle() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[5] : 0; }
			const auto		GetWeaponcameraRecoil() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[3] : 0; }
			const auto		GetWeaponcameraSnap() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[4] : 0; }
			const auto		GetWeapondeviationMax() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[6] : 0; }
			const auto		GetWeaponconvergence() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[5] : 0; }
			const auto		GetWeaponErgonomics() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[0] : 0; }

			const auto		GetModRecoil() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
				case EnumItemProperties::ItemPropertiesMagazine:
				case EnumItemProperties::ItemPropertiesScope:
				case EnumItemProperties::ItemPropertiesWeaponMod:
					return  this->m_floatParams[0];
				default:
					return 0.f;
				}
			}
			const auto		GetModErgonomics() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
				case EnumItemProperties::ItemPropertiesMagazine:
				case EnumItemProperties::ItemPropertiesScope:
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
				case EnumItemProperties::ItemPropertiesMagazine:
				case EnumItemProperties::ItemPropertiesScope:
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
			void			SetRecoilHorizontal(int value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_IntParams[2] = value; } }
			void			SetFireRate(int value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_IntParams[3] = value; } }
			void			SetcenterOfImpact(float value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_floatParams[1] = value; } }
			void			SetdeviationCurve(float value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_floatParams[2] = value; } }
			void			SetrecoilDispersion(int value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_IntParams[4] = value; } }
			void			SetrecoilAngle(int value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_IntParams[5] = value; } }
			void			SetcameraRecoil(float value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_floatParams[3] = value; } }
			void			SetcameraSnap(float value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_floatParams[4] = value; } }
			void			SetdeviationMax(int value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_IntParams[6] = value; } }
			void			Setconvergence(float value) noexcept { if (m_Type == EnumItemProperties::ItemPropertiesWeapon) { m_floatParams[5] = value; } }

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
					m_IntParams[2] = data["recoilHorizontal"];
					m_floatParams[0] = data["ergonomics"];
					m_IntParams[1] = (data.contains("sightingRange")) ? (int)data["sightingRange"] : -100;
					m_IntParams[3] = data["fireRate"];
					m_floatParams[1] = data["centerOfImpact"];
					m_floatParams[2] = data["deviationCurve"];
					m_IntParams[4] = data["recoilDispersion"];
					m_IntParams[5] = data["recoilAngle"];
					m_floatParams[3] = data["cameraRecoil"];
					m_floatParams[4] = data["cameraSnap"];
					m_IntParams[6] = data["deviationMax"];
					m_floatParams[5] = data["convergence"];

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
				else if (LEFT == "RecoilHorizontal") { this->SetRecoilHorizontal(std::stoi(Args[0])); }
				else if (LEFT == "FireRate") { this->SetFireRate(std::stoi(Args[0])); }

				else if (LEFT == "centerOfImpact") { this->SetcenterOfImpact(std::stof(Args[0])); }
				else if (LEFT == "deviationCurve") { this->SetdeviationCurve(std::stof(Args[0])); }
				else if (LEFT == "recoilDispersion") { this->SetrecoilDispersion(std::stoi(Args[0])); }
				else if (LEFT == "recoilAngle") { this->SetrecoilAngle(std::stoi(Args[0])); }
				else if (LEFT == "cameraRecoil") { this->SetcameraRecoil(std::stof(Args[0])); }
				else if (LEFT == "cameraSnap") { this->SetcameraSnap(std::stof(Args[0])); }
				else if (LEFT == "deviationMax") { this->SetdeviationMax(std::stoi(Args[0])); }
				else if (LEFT == "convergence") { this->Setconvergence(std::stof(Args[0])); }

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
		public:
			void		operator=(const ItemProperties& tgt) noexcept {
				this->m_Type = tgt.m_Type;
				this->m_IntParams = tgt.m_IntParams;
				this->m_floatParams = tgt.m_floatParams;

				this->m_ChildPartsID.resize(tgt.m_ChildPartsID.size());
				for (const auto& m : tgt.m_ChildPartsID) {
					this->m_ChildPartsID.at(&m - &tgt.m_ChildPartsID.front()) = m;
				}
				this->m_ConflictPartsID.resize(tgt.m_ConflictPartsID.size());
				for (const auto& m : tgt.m_ConflictPartsID) {
					this->m_ConflictPartsID.at(&m - &tgt.m_ConflictPartsID.front()) = m;
				}
			}
		};
		struct ItemsData {
			//ÉfÅ[É^
			IDParents<ItemTypeID>								m_TypeID;
			std::vector<IDParents<MapID>>						m_MapID;
			int													m_width{ 0 };
			int													m_height{ 0 };
			float												m_weight{ 0.f };
			std::vector<TraderGetData>							m_sellFor;
			int													m_fleaMarketFee{ 0 };
			ItemList::ItemProperties							m_properties;
			//í«â¡èÓïÒ
			std::vector<ItemID>									m_ParentPartsID;
			bool												m_isWeapon{ false };
			bool												m_isWeaponMod{ false };
			std::vector<TaskID>									m_UseTaskID;
		public:
			void SetOtherData(const ItemsData& Data) noexcept {
				this->m_TypeID = Data.m_TypeID;
				this->m_MapID.resize(Data.m_MapID.size());
				for (const auto& m : Data.m_MapID) {
					this->m_MapID.at(&m - &Data.m_MapID.front()) = m;
				}
				this->m_width = Data.m_width;
				this->m_height = Data.m_height;
				this->m_weight = Data.m_weight;
				this->m_sellFor.resize(Data.m_sellFor.size());
				for (const auto& m : Data.m_sellFor) {
					this->m_sellFor.at(&m - &Data.m_sellFor.front()) = m;
				}
				this->m_fleaMarketFee = Data.m_fleaMarketFee;
				this->m_properties;//
				//í«â¡èÓïÒ
				this->m_ParentPartsID.resize(Data.m_ParentPartsID.size());
				for (const auto& m : Data.m_ParentPartsID) {
					this->m_ParentPartsID.at(&m - &Data.m_ParentPartsID.front()) = m;
				}
				this->m_isWeapon = Data.m_isWeapon;
				this->m_isWeaponMod = Data.m_isWeaponMod;
				this->m_UseTaskID.resize(Data.m_UseTaskID.size());
				for (const auto& m : Data.m_UseTaskID) {
					this->m_UseTaskID.at(&m - &Data.m_UseTaskID.front()) = m;
				}
			}
		};
	private:
		ItemsData												m_ItemsData;
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
		const auto	GetRecoilHorizontal() const noexcept { return m_ItemsData.m_properties.GetWeaponRecoilHorizontal(); }
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
		const auto	GetFireRate() const noexcept { return this->m_ItemsData.m_properties.GetWeaponFireRate(); }
		const auto	GetcenterOfImpact() const noexcept { return this->m_ItemsData.m_properties.GetWeaponcenterOfImpact(); }
		const auto	GetdeviationCurve() const noexcept { return this->m_ItemsData.m_properties.GetWeapondeviationCurve(); }
		const auto	GetrecoilDispersion() const noexcept { return this->m_ItemsData.m_properties.GetWeaponrecoilDispersion(); }
		const auto	GetrecoilAngle() const noexcept { return this->m_ItemsData.m_properties.GetWeaponrecoilAngle(); }
		const auto	GetcameraRecoil() const noexcept { return this->m_ItemsData.m_properties.GetWeaponcameraRecoil(); }
		const auto	GetcameraSnap() const noexcept { return this->m_ItemsData.m_properties.GetWeaponcameraSnap(); }
		const auto	GetdeviationMax() const noexcept { return this->m_ItemsData.m_properties.GetWeapondeviationMax(); }
		const auto	Getconvergence() const noexcept { return this->m_ItemsData.m_properties.GetWeaponconvergence(); }
		const auto	GetIsPreset() const noexcept { return this->m_ItemsData.m_properties.GetType() == EnumItemProperties::ItemPropertiesPreset; }
	public:
		void		SetParent() noexcept;
		const auto	GetSellValue(TraderID* ID, int* pValue) const noexcept {
			*ID = InvalidID;
			*pValue = -1;
			for (const auto& sf : GetsellFor()) {
				auto basev = sf.GetValue();
				if (sf.GetID() == InvalidID) {
					basev -= GetfleaMarketFee();
				}
				if (*pValue < basev) {
					*pValue = basev;
					*ID = sf.GetID();
				}
			}
			return (*pValue != -1);
		}
		void		ResetTaskUseID(void) noexcept { this->m_ItemsData.m_UseTaskID.clear(); }
		void		AddTaskUseID(TaskID ID) noexcept { this->m_ItemsData.m_UseTaskID.emplace_back(ID); }
	public:
		void		SetItemsDataByOtherData(const ItemsData& Data) noexcept { this->m_ItemsData.SetOtherData(Data); }
	public:
		const int	Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) const noexcept;
		void		DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
	};

	class ItemJsonData :public JsonDataParent {
	public:
		ItemList::ItemsData							m_ItemsData;
		std::string									m_categorytypes;
	public:
		const auto&	GetItemsData() const noexcept { return this->m_ItemsData; }
	public:
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};

	class ItemData : public DataParent<ItemID, ItemList>, public JsonListParent<ItemJsonData> {
	public:
		ItemData() noexcept {
			std::string Path = "data/item/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				std::string Path2 = Path + RetPath2 + "/";
				GetDirList(Path2.c_str(), [&](const char* RetPath3) {
					SetDirList((Path2 + RetPath3 + "/").c_str());
				});
			});
		}
		~ItemData() noexcept {}
	public:
		void AfterLoadList()noexcept {
			for (auto& L : this->m_List) {
				L.SetParent();
			}
		}
		void InitDatabyJson() noexcept {}
		void UpdateData(int ofset, int size) noexcept {
			for (auto& L : this->m_List) {
				for (int loop = ofset; loop < ofset + size; loop++) {
					if (loop >= (int)GetJsonDataList().size()) { break; }
					auto& jd = GetJsonDataList().at(loop);
					if (L.GetIDstr() == jd->m_id || L.GetName() == jd->m_name) {
						L.m_CheckJson++;
						jd->OutputData(L.GetFilePath());
						break;
					}
				}
			}
		}
		void SaveAsNewData2(std::string Path) noexcept;
		void UpdateAfterbyJson(void) noexcept;

		void UpdateAfterbyJson_Sub(void) noexcept {
			for (auto& L : this->m_List) {
				for (const auto& D : GetJsonDataList()) {
					if (L.GetIDstr() == D->m_id) {
						L.SetItemsDataByOtherData((dynamic_cast<ItemJsonData*>(D.get()))->m_ItemsData);//
						break;
					}
				}
			}
		}
	};
};