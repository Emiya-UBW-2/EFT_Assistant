#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"

namespace FPS_n2 {
	class ItemList : public ListParent<ItemID> {
	public:
		class ItemProperties {
		public:
			class ChildItemSettings {
				std::vector<ItemTypeID>						m_TypeID;
				std::vector<IDParents<ItemID>>				m_Data;
			public:
				bool										m_Required{ false };
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
				void			SetData(std::string_view Data) noexcept {
					this->m_Data.resize(this->m_Data.size() + 1);
					this->m_Data.back().SetName(Data);
				}
				const auto&		GetData() const noexcept { return this->m_Data; }
				void			CheckData() noexcept;
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
			class stimEffects {
			public:
				std::string		m_type{};
				float			m_chance{ 0.f };
				int				m_delay{ 0 };
				int				m_duration{ 0 };
				float			m_value{ 0.f };
				bool			m_percent{ false };
				std::string		m_skillName{};
			public:
				const auto&		Gettype() const noexcept { return this->m_type; }
				const auto&		Getchance() const noexcept { return this->m_chance; }
				const auto&		Getdelay() const noexcept { return this->m_delay; }
				const auto&		Getduration() const noexcept { return this->m_duration; }
				const auto&		Getvalue() const noexcept { return this->m_value; }
				const auto&		Getpercent() const noexcept { return this->m_percent; }
				const auto&		GetskillName() const noexcept { return this->m_skillName; }
			};
		private:
			EnumItemProperties						m_Type{ EnumItemProperties::Max };
			std::array<int, 7>						m_IntParams{ 0,0,0,0,0,0,0 };
			std::array<float, 6>					m_floatParams{ 0,0,0,0,0,0 };
			std::vector<ChildItemSettings>			m_ChildPartsID;
			std::vector<IDParents<ItemID>>			m_ConflictPartsID;
			std::vector<ItemGetData>				m_ContainsItemID;
			std::vector<float>						m_sightModesID;
			std::vector<std::string>				m_cures;
			std::vector<stimEffects>				m_stimEffects;
		public:
			void			SetParent() noexcept;
		public:
			const auto*		GetTypeName() const noexcept { return (m_Type != EnumItemProperties::Max) ? ItemPropertiesStr[(int)m_Type] : ""; }
			const auto&		GetType() const noexcept { return this->m_Type; }
		public:
			const auto		GetStackMaxSize() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesAmmo) ? this->m_IntParams[0] : 0; }
			const int		GetArmerClass() const noexcept;
			const auto		GetEnergy() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesFoodDrink) ? this->m_IntParams[0] : 0; }
			const auto		GetHydration() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesFoodDrink) ? this->m_IntParams[1] : 0; }
			const auto		GetBlindnessProtection() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesGlasses) ? this->m_floatParams[0] : 0.f; }
			const auto		GetFragments() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesGrenade) ? this->m_IntParams[0] : 0; }
			const int		GetCapacity() const noexcept;
		private://Common
		public://Melee
			const auto		GetSlashDamage() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? this->m_IntParams[0] : 0; }
			const auto		GetstabDamage() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? this->m_IntParams[1] : 0; }
			const auto		GethitRadius() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? this->m_floatParams[0] : 0.f; }
		private:
			void			GetJsonDataMelee(const nlohmann::json& data) {
				m_IntParams[0] = data["slashDamage"];
				m_IntParams[1] = data["stabDamage"];
				m_floatParams[0] = data["hitRadius"];
			}
			void			SetDataMelee(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "slashDamage") { m_IntParams[0] = std::stoi(Args[0]); }
				else if (LEFT == "stabDamage") { m_IntParams[1] = std::stoi(Args[0]); }
				else if (LEFT == "hitRadius") { m_floatParams[0] = std::stof(Args[0]); }
			}
			void			OutputDataMelee(std::ofstream& outputfile) const noexcept {
				outputfile << "slashDamage=" + std::to_string(GetSlashDamage()) + "\n";
				outputfile << "stabDamage=" + std::to_string(GetstabDamage()) + "\n";
				outputfile << "hitRadius=" + std::to_string(GethitRadius()) + "\n";
			}
			void			DrawInfoMelee(int xp, int yp, int* xofs, int* yofs) const noexcept;
		public://key
			const auto		GetUses() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesKey) ? this->m_IntParams[0] : 0; }
		private:
			void			GetJsonDataKey(const nlohmann::json& data) {
				m_IntParams[0] = data["uses"];
			}
			void			SetDataKey(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "uses") { m_IntParams[0] = std::stoi(Args[0]); }
			}
			void			OutputDataKey(std::ofstream& outputfile) const noexcept {
				outputfile << "uses=" + std::to_string(GetUses()) + "\n";
			}
			void			DrawInfoKey(int xp, int yp, int* xofs, int* yofs) const noexcept;

		public://NightVision
			const auto		GetnoiseIntensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? this->m_floatParams[0] : 0.f; }
			const auto		GetnoiseScale() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? this->m_floatParams[1] : 0.f; }
			const auto		GetdiffuseIntensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? this->m_floatParams[2] : 0.f; }
			const auto		Getintensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? this->m_floatParams[3] : 0.f; }
		private:
			void			GetJsonDataNightVision(const nlohmann::json& data) {
				m_floatParams[0] = data["noiseIntensity"];
				m_floatParams[1] = data["noiseScale"];
				m_floatParams[2] = data["diffuseIntensity"];
				m_floatParams[3] = data["intensity"];
			}
			void			SetDataNightVision(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "noiseIntensity") { m_floatParams[0] = std::stof(Args[0]); }
				else if (LEFT == "noiseScale") { m_floatParams[1] = std::stof(Args[0]); }
				else if (LEFT == "diffuseIntensity") { m_floatParams[2] = std::stof(Args[0]); }
				else if (LEFT == "intensity") { m_floatParams[3] = std::stof(Args[0]); }
			}
			void			OutputDataNightVision(std::ofstream& outputfile) const noexcept {
				outputfile << "noiseIntensity=" + std::to_string(GetnoiseIntensity()) + "\n";
				outputfile << "noiseScale=" + std::to_string(GetnoiseScale()) + "\n";
				outputfile << "diffuseIntensity=" + std::to_string(GetdiffuseIntensity()) + "\n";
				outputfile << "intensity=" + std::to_string(Getintensity()) + "\n";
			}
			void			DrawInfoNightVision(int xp, int yp, int* xofs, int* yofs) const noexcept;

		public://Preset
			const auto		GetDefault() const noexcept { return ((m_Type == EnumItemProperties::ItemPropertiesPreset) ? this->m_IntParams[0] : 0) == 1; }
			auto&			SetContainsItem() noexcept { return this->m_ContainsItemID; }
			const auto&		GetContainsItem() const noexcept { return this->m_ContainsItemID; }
		private:
			void			GetJsonDataPreset(const nlohmann::json& data) {
				m_IntParams[0] = data["properties"]["default"];
				if (data.contains("containsItems") && !data["containsItems"].is_null()) {
					m_ContainsItemID.clear();
					for (const auto& s : data["containsItems"]) {
						if (!s.is_null()) {
							m_ContainsItemID.resize(m_ContainsItemID.size() + 1);
							m_ContainsItemID.back().Set(s["item"]["name"], s["count"]);
						}
					}
				}

			}
			void			SetDataPreset(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "IsDefault") { m_IntParams[0] = (Args[0] == "TRUE") ? 1 : 0; }
				else if (LEFT == "containsItems") {
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&m_ContainsItemID, a, "x", false);
					}
				}
			}
			void			OutputDataPreset(std::ofstream& outputfile) noexcept {
				outputfile << "IsDefault=" + (std::string)(GetDefault() ? "TRUE" : "FALSE") + "\n";
				if (this->m_ContainsItemID.size() > 0) {
					bool isHit = false;
					std::vector<std::string> Names;
					for (auto& m : this->m_ContainsItemID) {
						std::string NmBuf = m.GetName();
						if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
							if (!isHit) {
								isHit = true;
								outputfile << "containsItems=[\n";
							}
							outputfile << "\t" + m.GetOutputStr() + ((&m != &this->m_ContainsItemID.back()) ? DIV_STR : "") + "\n";
							Names.emplace_back(NmBuf);
						}
					}
					if (isHit) {
						outputfile << "]\n";
					}
				}
			}
			void			DrawInfoPreset(int xp, int yp, int* xofs, int* yofs) const noexcept;

		public://WeaponMod/Weapon Slots
			auto&			SetModSlots() noexcept { return this->m_ChildPartsID; }
			const auto&		GetModSlots() const noexcept { return this->m_ChildPartsID; }
			auto&			SetConflictPartsID() noexcept { return this->m_ConflictPartsID; }
			const auto&		GetConflictPartsID() const noexcept { return this->m_ConflictPartsID; }

		public://WeaponMod
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
			const auto		GetcenterOfImpact() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesBarrel) ? this->m_floatParams[2] : 0; }
			const auto		GetdeviationMax() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesBarrel) ? this->m_IntParams[0] : 0; }
			const auto		GetModCapacity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_IntParams[0] : 0; }
			const auto		GetloadModifier() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_floatParams[2] : 0; }
			const auto		GetammoCheckModifier() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_floatParams[3] : 0; }
			const auto		GetmalfunctionChance() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_floatParams[4] : 0; }
			const auto		GetSightingRange() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesScope) ? this->m_IntParams[1] : -100; }
			const auto&		GetsightModesID() const noexcept { return this->m_sightModesID; }
			const auto		GetaccuracyModifier() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeaponMod) ? this->m_floatParams[2] : 0; }
		private:
			void			GetJsonDataWeaponMod(const nlohmann::json& data) {
				m_floatParams[0] = data["recoilModifier"];
				m_floatParams[1] = data["ergonomics"];
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
					m_floatParams[2] = data["centerOfImpact"];
					m_IntParams[0] = data["deviationMax"];
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					m_IntParams[0] = data["capacity"];
					m_floatParams[2] = data["loadModifier"];
					m_floatParams[3] = data["ammoCheckModifier"];
					m_floatParams[4] = data["malfunctionChance"];
					break;
				case EnumItemProperties::ItemPropertiesScope:
					m_IntParams[1] = (data.contains("sightingRange")) ? (int)data["sightingRange"] : -100;
					{
						auto& modes = data["zoomLevels"];
						for (const auto& m : modes) {
							for (const auto& m2 : m) {
								this->m_sightModesID.emplace_back(m2);
							}
						}
					}
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					m_floatParams[2] = data["accuracyModifier"];
					break;
				default:
					break;
				}
			}
			void			SetDataWeaponMod(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Recoil") { m_floatParams[0] = std::stof(Args[0]) / 100.f; }
				else if (LEFT == "Ergonomics") { m_floatParams[1] = std::stof(Args[0]); }

				else if (LEFT == "centerOfImpact") { m_floatParams[2] = std::stof(Args[0]); }
				else if (LEFT == "deviationMax") { m_IntParams[0] = std::stoi(Args[0]); }

				else if (LEFT == "capacity") { m_IntParams[0] = std::stoi(Args[0]); }
				else if (LEFT == "loadModifier") { m_floatParams[2] = std::stof(Args[0]); }
				else if (LEFT == "ammoCheckModifier") { m_floatParams[3] = std::stof(Args[0]); }
				else if (LEFT == "malfunctionChance") { m_floatParams[4] = std::stof(Args[0]); }

				else if (LEFT == "SightRange") { m_IntParams[1] = std::stoi(Args[0]); }
				else if (LEFT == "sightZooms") { this->m_sightModesID.emplace_back(std::stof(Args[0])); }

				else if (LEFT == "accuracyModifier") { m_floatParams[2] = std::stof(Args[0]); }

			}
			void			OutputDataWeaponMod(std::ofstream& outputfile) noexcept {
				outputfile << "Recoil=" + std::to_string((float)this->GetModRecoil()*100.f) + "\n";
				outputfile << "Ergonomics=" + std::to_string(this->GetModErgonomics()) + "\n";
				switch (this->GetType()) {
				case EnumItemProperties::ItemPropertiesBarrel:
					outputfile << "centerOfImpact=" + std::to_string(this->GetcenterOfImpact()) + "\n";
					outputfile << "deviationMax=" + std::to_string(this->GetdeviationMax()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					outputfile << "capacity=" + std::to_string(this->GetModCapacity()) + "\n";
					outputfile << "loadModifier=" + std::to_string(this->GetloadModifier()) + "\n";
					outputfile << "ammoCheckModifier=" + std::to_string(this->GetammoCheckModifier()) + "\n";
					outputfile << "malfunctionChance=" + std::to_string(this->GetmalfunctionChance()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesScope:
					outputfile << "SightRange=" + std::to_string(this->GetSightingRange()) + "\n";
					for (const auto& m : this->m_sightModesID) {
						outputfile << "sightZooms=" + std::to_string(m) + "\n";
					}
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					outputfile << "accuracyModifier=" + std::to_string(this->GetaccuracyModifier()) + "\n";
					break;
				default:
					break;
				}
			}
			void			DrawInfoWeaponMod(int xp, int yp, int* xofs, int* yofs) const noexcept;

		public://Weapon
			const auto		GetWeaponRecoilVertical() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[0] : 0; }
			const auto		GetWeaponRecoilHorizontal() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[2] : 0; }
			const auto		GetWeaponErgonomics() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[0] : 0; }
			const auto		GetWeaponSightingRange() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[1] : -100; }
			const auto		GetWeaponFireRate() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[3] : 0; }
			const auto		GetWeaponcenterOfImpact() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[1] : 0.f; }
			const auto		GetWeapondeviationCurve() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[2] : 0.f; }
			const auto		GetWeaponrecoilDispersion() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[4] : 0; }
			const auto		GetWeaponrecoilAngle() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[5] : 0; }
			const auto		GetWeaponcameraRecoil() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[3] : 0; }
			const auto		GetWeaponcameraSnap() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[4] : 0; }
			const auto		GetWeapondeviationMax() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_IntParams[6] : 0; }
			const auto		GetWeaponconvergence() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesWeapon) ? this->m_floatParams[5] : 0; }
		private:
			void			GetJsonDataWeapon(const nlohmann::json& data) {
				m_IntParams[0] = (data.contains("recoilVertical")) ? (int)data["recoilVertical"] : 0;
				m_IntParams[2] = (data.contains("recoilHorizontal")) ? (int)data["recoilHorizontal"] : 0;
				m_floatParams[0] = (data.contains("ergonomics")) ? (float)data["ergonomics"] : 0.f;
				m_IntParams[1] = (data.contains("sightingRange")) ? (int)data["sightingRange"] : -100;
				m_IntParams[3] = (data.contains("fireRate")) ? (int)data["fireRate"] : 0;
				m_floatParams[1] = (data.contains("centerOfImpact")) ? (float)data["centerOfImpact"] : 0.f;
				m_floatParams[2] = (data.contains("deviationCurve")) ? (float)data["deviationCurve"] : 0.f;
				m_IntParams[4] = (data.contains("recoilDispersion")) ? (int)data["recoilDispersion"] : 0;
				m_IntParams[5] = (data.contains("recoilAngle")) ? (int)data["recoilAngle"] : 0;
				m_floatParams[3] = (data.contains("cameraRecoil") && data["cameraRecoil"].is_number()) ? (float)data["cameraRecoil"] : 0.f;
				m_floatParams[4] = (data.contains("cameraSnap")) ? (float)data["cameraSnap"] : 0.f;
				m_IntParams[6] = (data.contains("deviationMax")) ? (int)data["deviationMax"] : 0;
				m_floatParams[5] = (data.contains("convergence") && data["convergence"].is_number()) ? (float)data["convergence"] : 0.f;
			}
			void			SetDataWeapon(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Recoil") { this->m_IntParams[0] = std::stoi(Args[0]); }
				else if (LEFT == "RecoilHorizontal") { this->m_IntParams[2] = std::stoi(Args[0]); }
				else if (LEFT == "Ergonomics") { this->m_floatParams[0] = std::stof(Args[0]); }
				else if (LEFT == "SightRange") { this->m_IntParams[1] = std::stoi(Args[0]); }
				else if (LEFT == "FireRate") { this->m_IntParams[3] = (std::stoi(Args[0])); }
				else if (LEFT == "centerOfImpact") { this->m_floatParams[1] = (std::stof(Args[0])); }
				else if (LEFT == "deviationCurve") { this->m_floatParams[2] = (std::stof(Args[0])); }
				else if (LEFT == "recoilDispersion") { this->m_IntParams[4] = (std::stoi(Args[0])); }
				else if (LEFT == "recoilAngle") { this->m_IntParams[5] = (std::stoi(Args[0])); }
				else if (LEFT == "cameraRecoil") { this->m_floatParams[3] = (std::stof(Args[0])); }
				else if (LEFT == "cameraSnap") { this->m_floatParams[4] = (std::stof(Args[0])); }
				else if (LEFT == "deviationMax") { this->m_IntParams[6] = (std::stoi(Args[0])); }
				else if (LEFT == "convergence") { this->m_floatParams[5] = (std::stof(Args[0])); }
			}
			void			OutputDataWeapon(std::ofstream& outputfile) const noexcept {
				outputfile << "Recoil=" + std::to_string((float)this->GetWeaponRecoilVertical()) + "\n";
				outputfile << "RecoilHorizontal=" + std::to_string((float)this->GetWeaponRecoilHorizontal()) + "\n";
				outputfile << "Ergonomics=" + std::to_string(this->GetWeaponErgonomics()) + "\n";
				outputfile << "SightRange=" + std::to_string(this->GetSightingRange()) + "\n";
				outputfile << "FireRate=" + std::to_string(this->GetWeaponFireRate()) + "\n";
				outputfile << "centerOfImpact=" + std::to_string(this->GetWeaponcenterOfImpact()) + "\n";
				outputfile << "deviationCurve=" + std::to_string(this->GetWeapondeviationCurve()) + "\n";
				outputfile << "recoilDispersion=" + std::to_string(this->GetWeaponrecoilDispersion()) + "\n";
				outputfile << "recoilAngle=" + std::to_string(this->GetWeaponrecoilAngle()) + "\n";
				outputfile << "cameraRecoil=" + std::to_string(this->GetWeaponcameraRecoil()) + "\n";
				outputfile << "cameraSnap=" + std::to_string(this->GetWeaponcameraSnap()) + "\n";
				outputfile << "deviationMax=" + std::to_string(this->GetWeapondeviationMax()) + "\n";
				outputfile << "convergence=" + std::to_string(this->GetWeaponconvergence()) + "\n";
			}
			void			DrawInfoWeapon(int xp, int yp, int* xofs, int* yofs) const noexcept;

		public://Med
			const auto		GetMedUseTime() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesMedicalItem:
				case EnumItemProperties::ItemPropertiesMedKit:
				case EnumItemProperties::ItemPropertiesPainkiller:
				case EnumItemProperties::ItemPropertiesSurgicalKit:
				case EnumItemProperties::ItemPropertiesStim:
					return this->m_IntParams[0];
				default:
					return 0;
				}
			}
			const auto&		GetMedCures() const noexcept { return m_cures; }
			const auto		GetMedUses() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesMedicalItem:
				case EnumItemProperties::ItemPropertiesPainkiller:
				case EnumItemProperties::ItemPropertiesSurgicalKit:
					return this->m_IntParams[1];
				default:
					return 0;
				}
			}
			const auto		GetHitpoints() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[1] : 0; }
			const auto		GetmaxHealPerUse() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[2] : 0; }
			const auto		GethpCostLightBleeding() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[3] : -1; }
			const auto		GethpCostHeavyBleeding() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[4] : -1; }
			const auto		GetpainkillerDuration() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[2] : 0; }
			const auto		GetenergyImpact() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[3] : 0; }
			const auto		GethydrationImpact() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[4] : 0; }
			const auto		GetminLimbHealth() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesSurgicalKit) ? this->m_floatParams[0] : 0; }
			const auto		GetmaxLimbHealth() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesSurgicalKit) ? this->m_floatParams[1] : 0; }
		private:
			void			GetJsonDataMed(const nlohmann::json& data) {
				m_IntParams[0] = data["useTime"];
				for (const auto& c : data["cures"]) {
					m_cures.emplace_back(c);
				}
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesMedicalItem:
					m_IntParams[1] = data["uses"];
					break;
				case EnumItemProperties::ItemPropertiesMedKit:
					m_IntParams[1] = data["hitpoints"];
					m_IntParams[2] = data["maxHealPerUse"];
					if (!data["hpCostLightBleeding"].is_null()) {
						m_IntParams[3] = data["hpCostLightBleeding"];
					}
					else {
						m_IntParams[3] = -1;
					}
					if (!data["hpCostHeavyBleeding"].is_null()) {
						m_IntParams[4] = data["hpCostHeavyBleeding"];
					}
					else {
						m_IntParams[4] = -1;
					}
					break;
				case EnumItemProperties::ItemPropertiesPainkiller:
					m_IntParams[1] = data["uses"];
					m_IntParams[2] = data["painkillerDuration"];
					m_IntParams[3] = data["energyImpact"];
					m_IntParams[4] = data["hydrationImpact"];
					break;
				case EnumItemProperties::ItemPropertiesSurgicalKit:
					m_IntParams[1] = data["uses"];
					m_floatParams[0] = data["minLimbHealth"];
					m_floatParams[1] = data["maxLimbHealth"];
					break;
				case EnumItemProperties::ItemPropertiesStim:
					if (!data["stimEffects"].is_null()) {
						for (const auto& c : data["stimEffects"]) {
							m_stimEffects.resize(m_stimEffects.size() + 1);
							m_stimEffects.back().m_type = c["type"];
							m_stimEffects.back().m_chance = c["chance"];
							m_stimEffects.back().m_delay = c["delay"];
							m_stimEffects.back().m_duration = c["duration"];
							m_stimEffects.back().m_value = c["value"];
							m_stimEffects.back().m_percent = c["percent"];
							if (!c["skillName"].is_null()) {
								m_stimEffects.back().m_skillName = c["skillName"];
							}
							else {
								m_stimEffects.back().m_skillName = "";
							}
						}
					}
					break;
				default:
					break;
				}
			}
			void			SetDataMed(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "useTime") { this->m_IntParams[0] = std::stoi(Args[0]); }
				else if (LEFT == "cures") {
					for (auto& m : Args) {
						m_cures.emplace_back(m);
					}
				}
				//
				else if (LEFT == "uses") { this->m_IntParams[1] = std::stoi(Args[0]); }
				//
				else if (LEFT == "hitpoints") { this->m_IntParams[1] = std::stoi(Args[0]); }
				else if (LEFT == "maxHealPerUse") { this->m_IntParams[2] = std::stoi(Args[0]); }
				else if (LEFT == "hpCostLightBleeding") { this->m_IntParams[3] = std::stoi(Args[0]); }
				else if (LEFT == "hpCostHeavyBleeding") { this->m_IntParams[4] = std::stoi(Args[0]); }
				//
				else if (LEFT == "painkillerDuration") { this->m_IntParams[2] = std::stoi(Args[0]); }
				else if (LEFT == "energyImpact") { this->m_IntParams[3] = std::stoi(Args[0]); }
				else if (LEFT == "hydrationImpact") { this->m_IntParams[4] = std::stoi(Args[0]); }
				//
				else if (LEFT == "minLimbHealth") { this->m_floatParams[0] = std::stof(Args[0]); }
				else if (LEFT == "maxLimbHealth") { this->m_floatParams[1] = std::stof(Args[0]); }
				//stim
				else if (LEFT == "stimEffectsType") {
					m_stimEffects.resize(m_stimEffects.size() + 1);
					m_stimEffects.back().m_type = Args[0];
				}
				else if (LEFT == "stimEffectschance") { m_stimEffects.back().m_chance = std::stof(Args[0]); }
				else if (LEFT == "stimEffectsdelay") { m_stimEffects.back().m_delay = std::stoi(Args[0]); }
				else if (LEFT == "stimEffectsduration") { m_stimEffects.back().m_duration = std::stoi(Args[0]); }
				else if (LEFT == "stimEffectsvalue") { m_stimEffects.back().m_value = std::stof(Args[0]); }
				else if (LEFT == "stimEffectspercent") { m_stimEffects.back().m_percent = (Args[0] == "TRUE"); }
				else if (LEFT == "stimEffectsSkiilName") { m_stimEffects.back().m_skillName = Args[0]; }
			}
			void			OutputDataMed(std::ofstream& outputfile) noexcept {
				outputfile << "useTime=" + std::to_string((float)this->GetMedUseTime()) + "\n";
				if (m_cures.size() > 0) {
					outputfile << "cures=[";
					for (auto& m : m_cures) {
						outputfile << m;
						if (&m != &m_cures.back()) {
							outputfile << DIV_STR;
						}
					}
					outputfile << "]\n";
				}
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesMedicalItem:
					outputfile << "uses=" + std::to_string(this->GetMedUses()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesMedKit:
					outputfile << "hitpoints=" + std::to_string(this->GetHitpoints()) + "\n";
					outputfile << "maxHealPerUse=" + std::to_string(this->GetmaxHealPerUse()) + "\n";
					if (this->GethpCostLightBleeding() >= 0) {
						outputfile << "hpCostLightBleeding=" + std::to_string(this->GethpCostLightBleeding()) + "\n";
					}
					if (this->GethpCostHeavyBleeding() >= 0) {
						outputfile << "hpCostHeavyBleeding=" + std::to_string(this->GethpCostHeavyBleeding()) + "\n";
					}
					break;
				case EnumItemProperties::ItemPropertiesPainkiller:
					outputfile << "uses=" + std::to_string(this->GetMedUses()) + "\n";
					outputfile << "painkillerDuration=" + std::to_string(this->GetpainkillerDuration()) + "\n";
					outputfile << "energyImpact=" + std::to_string(this->GetenergyImpact()) + "\n";
					outputfile << "hydrationImpact=" + std::to_string(this->GethydrationImpact()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesSurgicalKit:
					outputfile << "uses=" + std::to_string(this->GetMedUses()) + "\n";
					outputfile << "minLimbHealth=" + std::to_string(this->GetminLimbHealth()) + "\n";
					outputfile << "maxLimbHealth=" + std::to_string(this->GetmaxLimbHealth()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesStim:
					for (const auto& s : m_stimEffects) {
						outputfile << "stimEffectsType=" + s.Gettype() + "\n";
						outputfile << "stimEffectschance=" + std::to_string(s.Getchance()) + "\n";
						outputfile << "stimEffectsdelay=" + std::to_string(s.Getdelay()) + "\n";
						outputfile << "stimEffectsduration=" + std::to_string(s.Getduration()) + "\n";
						outputfile << "stimEffectsvalue=" + std::to_string(s.Getvalue()) + "\n";
						outputfile << "stimEffectspercent=" + (std::string)(s.Getpercent() ? "TRUE" : "FALSE") + "\n";
						outputfile << "stimEffectsSkiilName=" + s.GetskillName() + "\n";
					}
					break;
				default:
					break;
				}
			}
			void			DrawInfoMed(int xp, int yp, int* xofs, int* yofs) const noexcept;

		public://total
			void			GetJsonData(const nlohmann::json& data) {
				auto& d = data["properties"];
				//
				if (d.contains("__typename") && !d["__typename"].is_null()) {
					m_Type = GetStringToEnumItemProperties((std::string)d["__typename"]);
				}
				//個別
				if (d.contains("slots") && !d["slots"].is_null()) {

					m_ChildPartsID.clear();
					for (const auto& s : d["slots"]) {
						m_ChildPartsID.resize(m_ChildPartsID.size() + 1);

						m_ChildPartsID.back().m_Required = s["required"];

						for (const auto& f : s["filters"]) {
							for (const auto& a : f) {
								for (const auto& n : a) {
									m_ChildPartsID.back().SetData(n);
								}
							}
						}
					}
				}
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesAmmo:
					m_IntParams[0] = d["stackMaxSize"];
					break;
				case EnumItemProperties::ItemPropertiesArmor:
					if (!d["class"].is_null()) {
						m_IntParams[0] = d["class"];
					}
					else {
						m_IntParams[0] = 0;
					}
					break;
				case EnumItemProperties::ItemPropertiesArmorAttachment:
					if (!d["class"].is_null()) {
						m_IntParams[0] = d["class"];
					}
					else {
						m_IntParams[0] = 0;
					}
					break;
				case EnumItemProperties::ItemPropertiesBackpack:
					m_IntParams[0] = d["capacity"];
					break;
				case EnumItemProperties::ItemPropertiesChestRig:
					if (!d["class"].is_null()) {
						m_IntParams[0] = d["class"];
					}
					else {
						m_IntParams[0] = 0;
					}
					m_IntParams[1] = d["capacity"];
					break;
				case EnumItemProperties::ItemPropertiesContainer:
					m_IntParams[0] = d["capacity"];
					break;
				case EnumItemProperties::ItemPropertiesFoodDrink:
					m_IntParams[0] = d["energy"];
					m_IntParams[1] = d["hydration"];
					break;
				case EnumItemProperties::ItemPropertiesGlasses:
					if (!d["class"].is_null()) {
						m_IntParams[0] = d["class"];
					}
					else {
						m_IntParams[0] = 0;
					}
					m_floatParams[0] = d["blindnessProtection"];
					break;
				case EnumItemProperties::ItemPropertiesGrenade:
					m_IntParams[0] = d["fragments"];
					break;
				case EnumItemProperties::ItemPropertiesHelmet:
					if (!d["class"].is_null()) {
						m_IntParams[0] = d["class"];
					}
					else {
						m_IntParams[0] = 0;
					}
					break;
				case EnumItemProperties::ItemPropertiesKey:
					GetJsonDataKey(d);
					break;
				case EnumItemProperties::ItemPropertiesMelee:
					GetJsonDataMelee(d);
					break;
				case EnumItemProperties::ItemPropertiesNightVision:
					GetJsonDataNightVision(d);
					break;
				case EnumItemProperties::ItemPropertiesPreset:
					GetJsonDataPreset(data);
					break;
				case EnumItemProperties::ItemPropertiesMedicalItem:
				case EnumItemProperties::ItemPropertiesMedKit:
				case EnumItemProperties::ItemPropertiesPainkiller:
				case EnumItemProperties::ItemPropertiesSurgicalKit:
				case EnumItemProperties::ItemPropertiesStim:
					GetJsonDataMed(d);
					break;
				case EnumItemProperties::ItemPropertiesWeapon:
					GetJsonDataWeapon(d);
					break;
				case EnumItemProperties::ItemPropertiesBarrel:
				case EnumItemProperties::ItemPropertiesMagazine:
				case EnumItemProperties::ItemPropertiesScope:
				case EnumItemProperties::ItemPropertiesWeaponMod:
					GetJsonDataWeaponMod(d);
					break;
				default:
					break;
				}
				//
				SetParent();
			}
			void			SetData(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "propertiestype") {
					this->m_Type = GetStringToEnumItemProperties(Args[0]);
				}
				else {
					{
						if (LEFT == "ChildParts") {
							this->m_ChildPartsID.resize(this->m_ChildPartsID.size() + 1);
							for (auto& a : Args) {
								bool isHit = false;
								for (auto& d : this->m_ChildPartsID.back().GetData()) {
									if (d.GetName() == a) {
										isHit = true;
										break;
									}
								}
								if (!isHit) {
									this->m_ChildPartsID.back().SetData(a);
								}
							}
						}
						else if (LEFT == "ChildParts_required") {
							this->m_ChildPartsID.back().m_Required = (Args[0] == "TRUE");
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
					//個別
					switch (m_Type) {
					case EnumItemProperties::ItemPropertiesAmmo:
						break;
					case EnumItemProperties::ItemPropertiesArmor:
						break;
					case EnumItemProperties::ItemPropertiesArmorAttachment:
						break;
					case EnumItemProperties::ItemPropertiesBackpack:
						break;
					case EnumItemProperties::ItemPropertiesChestRig:
						break;
					case EnumItemProperties::ItemPropertiesContainer:
						break;
					case EnumItemProperties::ItemPropertiesFoodDrink:
						break;
					case EnumItemProperties::ItemPropertiesGlasses:
						break;
					case EnumItemProperties::ItemPropertiesGrenade:
						break;
					case EnumItemProperties::ItemPropertiesHelmet:
						break;
					case EnumItemProperties::ItemPropertiesKey:
						SetDataKey(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesMelee:
						SetDataMelee(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesNightVision:
						SetDataNightVision(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesPainkiller:
						break;
					case EnumItemProperties::ItemPropertiesPreset:
						SetDataPreset(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesMedicalItem:
					case EnumItemProperties::ItemPropertiesMedKit:
					case EnumItemProperties::ItemPropertiesSurgicalKit:
					case EnumItemProperties::ItemPropertiesStim:
						SetDataMed(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesWeapon:
						SetDataWeapon(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesBarrel:
					case EnumItemProperties::ItemPropertiesMagazine:
					case EnumItemProperties::ItemPropertiesScope:
					case EnumItemProperties::ItemPropertiesWeaponMod:
						SetDataWeaponMod(LEFT, Args);
						break;
					default:
						break;
					}
				}
			}
			void			OutputData(std::ofstream& outputfile) noexcept {
				outputfile << "propertiestype=" + (std::string)(this->GetTypeName()) + "\n";

				if (this->m_ChildPartsID.size() > 0) {
					for (const auto& m : this->m_ChildPartsID) {
						if (m.GetData().size() > 0) {
							outputfile << "ChildParts=[\n";
							std::vector<std::string> Names;
							for (const auto& d : m.GetData()) {
								std::string NmBuf = d.GetName();
								if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
									outputfile << "\t" + NmBuf + ((&d != &m.GetData().back()) ? DIV_STR : "") + "\n";
									Names.emplace_back(NmBuf);
								}
							}
							outputfile << "]\n";
							
							outputfile << "ChildParts_required=" + (std::string)(m.m_Required ? "TRUE" : "FALSE") + "\n";
						}
					}
				}
				if (this->m_ConflictPartsID.size() > 0) {
					bool isHit = false;
					std::vector<std::string> Names;
					for (auto& m : this->m_ConflictPartsID) {
						std::string NmBuf = m.GetName();
						if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
							if (!isHit) {
								isHit = true;
								outputfile << "Conflict=[\n";
							}
							outputfile << "\t" + NmBuf + ((&m != &this->m_ConflictPartsID.back()) ? DIV_STR : "") + "\n";
							Names.emplace_back(NmBuf);
						}
					}
					if (isHit) {
						outputfile << "]\n";
					}
				}

				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesAmmo:
					break;
				case EnumItemProperties::ItemPropertiesArmor:
					break;
				case EnumItemProperties::ItemPropertiesArmorAttachment:
					break;
				case EnumItemProperties::ItemPropertiesBackpack:
					break;
				case EnumItemProperties::ItemPropertiesChestRig:
					break;
				case EnumItemProperties::ItemPropertiesContainer:
					break;
				case EnumItemProperties::ItemPropertiesFoodDrink:
					break;
				case EnumItemProperties::ItemPropertiesGlasses:
					break;
				case EnumItemProperties::ItemPropertiesGrenade:
					break;
				case EnumItemProperties::ItemPropertiesHelmet:
					break;
				case EnumItemProperties::ItemPropertiesKey:
					OutputDataKey(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesMelee:
					OutputDataMelee(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesNightVision:
					OutputDataNightVision(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesPreset:
					OutputDataPreset(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesMedicalItem:
				case EnumItemProperties::ItemPropertiesMedKit:
				case EnumItemProperties::ItemPropertiesPainkiller:
				case EnumItemProperties::ItemPropertiesSurgicalKit:
				case EnumItemProperties::ItemPropertiesStim:
					OutputDataMed(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesWeapon:
					OutputDataWeapon(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesBarrel:
				case EnumItemProperties::ItemPropertiesMagazine:
				case EnumItemProperties::ItemPropertiesScope:
				case EnumItemProperties::ItemPropertiesWeaponMod:
					OutputDataWeaponMod(outputfile);
					break;
				default:
					break;
				}
			}
			void			DrawInfo(int xp, int yp, int* xofs, int* yofs) const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesAmmo:
					break;
				case EnumItemProperties::ItemPropertiesArmor:
					break;
				case EnumItemProperties::ItemPropertiesArmorAttachment:
					break;
				case EnumItemProperties::ItemPropertiesBackpack:
					break;
				case EnumItemProperties::ItemPropertiesChestRig:
					break;
				case EnumItemProperties::ItemPropertiesContainer:
					break;
				case EnumItemProperties::ItemPropertiesFoodDrink:
					break;
				case EnumItemProperties::ItemPropertiesGlasses:
					break;
				case EnumItemProperties::ItemPropertiesGrenade:
					break;
				case EnumItemProperties::ItemPropertiesHelmet:
					break;
				case EnumItemProperties::ItemPropertiesKey:
					this->DrawInfoKey(xp, yp, xofs, yofs);
					break;
				case EnumItemProperties::ItemPropertiesMelee:
					this->DrawInfoMelee(xp, yp, xofs, yofs);
					break;
				case EnumItemProperties::ItemPropertiesNightVision:
					this->DrawInfoNightVision(xp, yp, xofs, yofs);
					break;
				case EnumItemProperties::ItemPropertiesPreset:
					this->DrawInfoPreset(xp, yp, xofs, yofs);
					break;
				case EnumItemProperties::ItemPropertiesMedicalItem:
				case EnumItemProperties::ItemPropertiesMedKit:
				case EnumItemProperties::ItemPropertiesPainkiller:
				case EnumItemProperties::ItemPropertiesSurgicalKit:
				case EnumItemProperties::ItemPropertiesStim:
					this->DrawInfoMed(xp, yp, xofs, yofs);
					break;
				case EnumItemProperties::ItemPropertiesWeapon:
					this->DrawInfoWeapon(xp, yp, xofs, yofs);
					break;
				case EnumItemProperties::ItemPropertiesBarrel:
				case EnumItemProperties::ItemPropertiesMagazine:
				case EnumItemProperties::ItemPropertiesScope:
				case EnumItemProperties::ItemPropertiesWeaponMod:
					this->DrawInfoWeaponMod(xp, yp, xofs, yofs);
					break;
				default:
					break;
				}
			}
		public:
			void			operator=(const ItemProperties& tgt) noexcept {
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
				this->m_ContainsItemID.resize(tgt.m_ContainsItemID.size());
				for (const auto& m : tgt.m_ContainsItemID) {
					this->m_ContainsItemID.at(&m - &tgt.m_ContainsItemID.front()) = m;
				}
				this->m_sightModesID.resize(tgt.m_sightModesID.size());
				for (const auto& m : tgt.m_sightModesID) {
					this->m_sightModesID.at(&m - &tgt.m_sightModesID.front()) = m;
				}
			}
		};
		struct ItemsData {
			//データ
			IDParents<ItemTypeID>								m_TypeID;
			std::vector<IDParents<MapID>>						m_MapID;
			int													m_width{ 0 };
			int													m_height{ 0 };
			float												m_weight{ 0.f };
			std::vector<TraderGetData>							m_sellFor;
			int													m_fleaMarketFee{ 0 };
			ItemList::ItemProperties							m_properties;
			//追加情報
			std::vector<ItemID>									m_ParentPartsID;
			bool												m_isWeapon{ false };
			bool												m_isWeaponMod{ false };
			bool												m_isMedItem{ false };
			std::vector<TaskID>									m_UseTaskID;
		public:
			void SetOtherData(const ItemsData& Data) noexcept {
				//this->m_TypeID = Data.m_TypeID;
				//this->m_MapID.resize(Data.m_MapID.size());
				//for (const auto& m : Data.m_MapID) {
				//	this->m_MapID.at(&m - &Data.m_MapID.front()) = m;
				//}
				this->m_width = Data.m_width;
				this->m_height = Data.m_height;
				this->m_weight = Data.m_weight;
				this->m_sellFor.resize(Data.m_sellFor.size());
				for (const auto& m : Data.m_sellFor) {
					this->m_sellFor.at(&m - &Data.m_sellFor.front()) = m;
				}
				this->m_fleaMarketFee = Data.m_fleaMarketFee;
				//this->m_properties;//
				//追加情報
				this->m_ParentPartsID.resize(Data.m_ParentPartsID.size());
				for (const auto& m : Data.m_ParentPartsID) {
					this->m_ParentPartsID.at(&m - &Data.m_ParentPartsID.front()) = m;
				}
				this->m_isWeapon = Data.m_isWeapon;
				this->m_isWeaponMod = Data.m_isWeaponMod;
				this->m_isMedItem = Data.m_isMedItem;
				this->m_UseTaskID.resize(Data.m_UseTaskID.size());
				for (const auto& m : Data.m_UseTaskID) {
					this->m_UseTaskID.at(&m - &Data.m_UseTaskID.front()) = m;
				}
			}
		};
	private:
		ItemsData												m_ItemsData;


		std::array<WindowMySystem::ScrollBoxClass, 2>				m_Scroll;
	private:
		//追加設定
		void		SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override;
		void		Load_Sub() noexcept override;
		void		WhenAfterLoad_Sub() noexcept override;
	public:
		const auto&	GetTypeID() const noexcept { return this->m_ItemsData.m_TypeID.GetID(); }
		const auto&	GetMapID() const noexcept { return this->m_ItemsData.m_MapID; }
		const auto&	Getwidth() const noexcept { return this->m_ItemsData.m_width; }
		const auto&	Getheight() const noexcept { return this->m_ItemsData.m_height; }
		const auto&	GetsellFor() const noexcept { return this->m_ItemsData.m_sellFor; }
		auto&	SetsellFor() noexcept { return this->m_ItemsData.m_sellFor; }
		const auto&	Getweight() const noexcept { return this->m_ItemsData.m_weight; }
		const auto&	GetfleaMarketFee() const noexcept { return this->m_ItemsData.m_fleaMarketFee; }
		const auto&	GetUseTaskID() const noexcept { return this->m_ItemsData.m_UseTaskID; }
	public://Key
		const int	GetKeyUses() const noexcept { return this->m_ItemsData.m_properties.GetUses(); }
	public://WeaponMod/Weapon Slots
		const auto&	GetChildParts() const noexcept { return this->m_ItemsData.m_properties.GetModSlots(); }
		const auto&	GetConflictParts() const noexcept { return this->m_ItemsData.m_properties.GetConflictPartsID(); }
	public://WeaponMod
		const auto	GetRecoil() const noexcept {
			switch (this->m_ItemsData.m_properties.GetType()) {
			case EnumItemProperties::ItemPropertiesBarrel:
			case EnumItemProperties::ItemPropertiesMagazine:
			case EnumItemProperties::ItemPropertiesScope:
			case EnumItemProperties::ItemPropertiesWeaponMod:
				return ((float)m_ItemsData.m_properties.GetModRecoil()*100.f);
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
			default:
				return 0.f;
			}
		}
	public://Weapon
		const auto	GetRecoilVertical() const noexcept { return m_ItemsData.m_properties.GetWeaponRecoilVertical(); }
		const auto	GetRecoilHorizontal() const noexcept { return m_ItemsData.m_properties.GetWeaponRecoilHorizontal(); }
		const auto	GetWeaponErgonomics() const noexcept { return this->m_ItemsData.m_properties.GetWeaponErgonomics(); }
		const auto	GetIsPreset() const noexcept { return this->m_ItemsData.m_properties.GetType() == EnumItemProperties::ItemPropertiesPreset; }
		const auto	GetIsWeapon() const noexcept { return this->m_ItemsData.m_properties.GetType() == EnumItemProperties::ItemPropertiesWeapon; }
	public:
		void		SetParent() noexcept;
		void		SetParent2() noexcept;
		const auto	GetSellValue(TraderID* ID, int* pValue) const noexcept {
			*ID = InvalidID;
			*pValue = -1;
			for (const auto& sf : GetsellFor()) {
				int basev = 0;
				if (sf.GetID() == InvalidID) {
					if (!PlayerData::Instance()->GetIsOpenFreaMarket()) {
						continue;
					}
					basev = sf.GetValue();
					basev -= GetfleaMarketFee();
				}
				else {
					basev = sf.GetValue();
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
		const int	Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) noexcept;
		void		DrawWindow(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) noexcept;
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

	class ItemData : public DataParent<ItemList>, public JsonListParent<ItemJsonData> {
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
			//
			{
				std::ifstream File("data/item/TraderSell.txt");
				std::string line;
				while (std::getline(File, line)) {
					if (line == "") { continue; }
					auto LEFT = getparams::getleft(line);
					auto Args = GetArgs(getparams::getright(line));
					for (auto& L : this->m_List) {
						if (L.GetIDstr() == LEFT) {
							L.SetsellFor().resize(L.SetsellFor().size() + 1);
							L.SetsellFor().back().Set(Args[0], std::stoi(Args[1]));
							break;
						}
					}
				}
				File.close();
			}
			//
			for (auto& L : this->m_List) {
				for (auto& L2 : this->m_List) {
					if (L.GetID() != L2.GetID() && L.GetIDstr() == L2.GetIDstr()) {
						std::string ErrMes = "Error : アイテム登録被り:";
						ErrMes += "[";
						ErrMes += L.GetName();
						ErrMes += "]";
						ErrMes += "/[";
						ErrMes += L2.GetName();
						ErrMes += "]";
						DataErrorLog::Instance()->AddLog(ErrMes.c_str());
					}
				}
			}
			for (auto& L : this->m_List) {
				L.SetParent();
			}
			for (auto& L : this->m_List) {
				L.SetParent2();
			}
		}
	public:
		void SaveAsNewData2(std::string Path) noexcept {
			bool maked = false;
			for (auto& jd : GetJsonDataList()) {
				if (!jd->m_IsFileOpened) {
					std::string ParentPath = Path + (dynamic_cast<ItemJsonData*>(jd.get()))->m_categorytypes;

					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}

					std::string ChildPath = ParentPath + "/";
					std::string Name = jd->m_CanUseFileName + std::to_string((int)(&jd - &GetJsonDataList().front())) + ".txt";

					jd->OutputData(ChildPath + Name);
					//RemoveDirectory(Path.c_str());
				}
			}
		}
		void UpdateAfterbyJson(void) noexcept override;
		void UpdateAfterbyJson_Sub(void) noexcept {
			for (auto& L : this->m_List) {
				for (const auto& D : GetJsonDataList()) {
					if (L.GetIDstr() == D->m_id) {
						L.SetItemsDataByOtherData((dynamic_cast<ItemJsonData*>(D.get()))->m_ItemsData);//
						break;
					}
				}
			}

			std::ofstream outputfile("data/item/TraderSell.txt");
			for (auto& L : this->m_List) {
				for (const auto& jd : GetJsonDataList()) {
					if (L.GetIDstr() == jd->m_id) {
						for (auto& sf : (dynamic_cast<ItemJsonData*>(jd.get()))->GetItemsData().m_sellFor) {
							outputfile << L.GetIDstr() + "=[" + sf.GetName() + DIV_STR + std::to_string(sf.GetValue()) + "]\n";
						}
						break;
					}
				}
			}
			outputfile.close();

			//プリセットデータに反映
			OutputPresetbyJson();
		}
		void OutputPresetbyJson(void) noexcept;
	public:
	};
};