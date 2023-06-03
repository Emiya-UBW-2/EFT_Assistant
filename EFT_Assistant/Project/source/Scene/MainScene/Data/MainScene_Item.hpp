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
				case EnumItemProperties::ItemPropertiesArmorAttachment:
				case EnumItemProperties::ItemPropertiesChestRig:
				case EnumItemProperties::ItemPropertiesGlasses:
				case EnumItemProperties::ItemPropertiesHelmet:
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
				case EnumItemProperties::ItemPropertiesPainkiller:
				case EnumItemProperties::ItemPropertiesSurgicalKit:
				case EnumItemProperties::ItemPropertiesStim:
					return this->m_IntParams[0];
				default:
					return 0;
				}
			}
			const auto		GetSlashDamage() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMelee) ? this->m_IntParams[0] : 0; }
			const auto		GetCapacity() const noexcept {
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBackpack:
				case EnumItemProperties::ItemPropertiesContainer:
					return this->m_IntParams[0];
				case EnumItemProperties::ItemPropertiesChestRig:
					return this->m_IntParams[1];
				default:
					return 0;
				}
			}
			const auto		GetHitpoints() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMedKit) ? this->m_IntParams[0] : 0; }
			const auto		GetIntensity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesNightVision) ? this->m_IntParams[0] : 0; }
			const auto		GetDefault() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesPreset) ? this->m_IntParams[0] : 0; }

		private://Common
			void			SetType(std::string_view value) noexcept {
				for (int i = 0; i < (int)EnumItemProperties::Max; i++) {
					if (value == ItemPropertiesStr[i]) {
						m_Type = (EnumItemProperties)i;
						break;
					}
				}
			}
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
			const auto		GetModCapacity() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_IntParams[0] : 0; }

			const auto		GetloadModifier() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_floatParams[2] : 0; }
			const auto		GetammoCheckModifier() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_floatParams[3] : 0; }
			const auto		GetmalfunctionChance() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesMagazine) ? this->m_floatParams[4] : 0; }

			const auto		GetSightingRange() const noexcept { return (m_Type == EnumItemProperties::ItemPropertiesScope) ? this->m_IntParams[1] : -100; }
			void			DrawInfoWeaopnMod(int xp, int yp, int* xofs, int* yofs) const noexcept {
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, (this->GetModRecoil() < 0.f) ? Green : Red, Black,
					"Recoil(リコイル変動値):%3.1f %%", this->GetModRecoil()) + y_r(30)); *yofs += LineHeight + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, (this->GetModErgonomics() >= 0.f) ? Green : Red, Black,
					"Ergonomics(エルゴノミクス変動値):%3.1f", this->GetModErgonomics()) + y_r(30)); *yofs += LineHeight + y_r(5);
				switch (this->GetType()) {
				case EnumItemProperties::ItemPropertiesBarrel:
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, White, Black,
						"Capacity(マガジン容量):%3d", this->GetModCapacity()) + y_r(30)); *yofs += LineHeight + y_r(5);
					*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, (this->GetloadModifier() >= 0.f) ? Green : Red, Black,
						"loadModifier(装弾変動値):%3.1f", this->GetloadModifier()) + y_r(30)); *yofs += LineHeight + y_r(5);
					*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, (this->GetammoCheckModifier() >= 0.f) ? Green : Red, Black,
						"ammoCheckModifier(弾数チェック変動値):%3.1f", this->GetammoCheckModifier()) + y_r(30)); *yofs += LineHeight + y_r(5);
					*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, (this->GetmalfunctionChance() <= 0.f) ? Green : Red, Black,
						"malfunctionChance(ジャム変動値):%3.1f", this->GetmalfunctionChance()) + y_r(30)); *yofs += LineHeight + y_r(5);
					break;
				case EnumItemProperties::ItemPropertiesScope:
					*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + LineHeight + *yofs, LineHeight, STRX_LEFT, White, Black,
						"SightingRange(照準距離):%3d", this->GetSightingRange()) + y_r(30)); *yofs += LineHeight + y_r(5);
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					break;
				default:
					break;
				}
			}
		private:
			void			GetJsonDataWeaponMod(const nlohmann::json& data) {
				m_floatParams[0] = data["recoilModifier"];
				m_floatParams[1] = data["ergonomics"];
				switch (m_Type) {
				case EnumItemProperties::ItemPropertiesBarrel:
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					m_IntParams[0] = data["capacity"];
					m_floatParams[2] = data["loadModifier"];
					m_floatParams[3] = data["ammoCheckModifier"];
					m_floatParams[4] = data["malfunctionChance"];
					break;
				case EnumItemProperties::ItemPropertiesScope:
					m_IntParams[0] = (data.contains("sightingRange")) ? (int)data["sightingRange"] : -100;
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					break;
				default:
					break;
				}
			}
			void			SetDataWeaponMod(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Recoil") { m_floatParams[0] = std::stof(Args[0]) / 100.f; }
				else if (LEFT == "Ergonomics") { m_floatParams[1] = std::stof(Args[0]); }

				else if (LEFT == "capacity") { m_IntParams[0] = std::stoi(Args[0]); }
				else if (LEFT == "loadModifier") { m_floatParams[2] = std::stof(Args[0]); }
				else if (LEFT == "ammoCheckModifier") { m_floatParams[3] = std::stof(Args[0]); }
				else if (LEFT == "malfunctionChance") { m_floatParams[4] = std::stof(Args[0]); }

				else if (LEFT == "SightRange") { m_IntParams[1] = std::stoi(Args[0]); }
			}
			void			OutputDataWeaponMod(std::ofstream& outputfile) noexcept {
				outputfile << "Recoil=" + std::to_string((float)this->GetModRecoil()*100.f) + "\n";
				outputfile << "Ergonomics=" + std::to_string(this->GetModErgonomics()) + "\n";
				switch (this->GetType()) {
				case EnumItemProperties::ItemPropertiesBarrel:
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					outputfile << "capacity=" + std::to_string(this->GetModCapacity()) + "\n";
					outputfile << "loadModifier=" + std::to_string(this->GetloadModifier()) + "\n";
					outputfile << "ammoCheckModifier=" + std::to_string(this->GetammoCheckModifier()) + "\n";
					outputfile << "malfunctionChance=" + std::to_string(this->GetmalfunctionChance()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesScope:
					outputfile << "SightRange=" + std::to_string(this->GetSightingRange()) + "\n";
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					break;
				default:
					break;
				}
			}
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
			void			DrawInfoWeaopn(int xp, int yp, int* xofs, int* yofs) const noexcept {
				int ysiz = LineHeight * 6 / 10;
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"RecoilVertical    (縦リコイル)     :%3d %%", this->GetWeaponRecoilVertical()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"RecoilHorizontal  (横リコイル)     :%3d %%", this->GetWeaponRecoilHorizontal()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"Ergonomics        (エルゴノミクス) :%3.1f", this->GetWeaponErgonomics()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"SightRange        (照準距離)       :%3d %%", this->GetWeaponSightingRange()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"FireRate          (発射速度)       :%3d %%", this->GetWeaponFireRate()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"CenterOfImpact    (跳ね上がり？)   :%3.2f %%", this->GetWeaponcenterOfImpact()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"deviationCurve    (偏差の曲線？)   :%3.1f %%", this->GetWeapondeviationCurve()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"recoilDispersion  (リコイルの分散？):%3d %%", this->GetWeaponrecoilDispersion()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"recoilAngle       (リコイルの角度？):%3d %%", this->GetWeaponrecoilAngle()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"cameraRecoil      (カメラリコイル？):%3.1f %%", this->GetWeaponcameraRecoil()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"cameraSnap        (カメラスナップ？):%3.1f %%", this->GetWeaponcameraSnap()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"deviationMax      (偏差の最大値？)  :%3d %%", this->GetWeapondeviationMax()) + y_r(30)); *yofs += ysiz + y_r(5);
				*xofs = std::max(*xofs, WindowSystem::SetMsg(xp, yp + *yofs, xp, yp + ysiz + *yofs, ysiz, STRX_LEFT, White, Black,
					"convergence       (収束？)          :%3.1f %%", this->GetWeaponconvergence()) + y_r(30)); *yofs += ysiz + y_r(5);
			}
		private:
			void			GetJsonDataWeapon(const nlohmann::json& data) {
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
			void			OutputDataWeapon(std::ofstream& outputfile) noexcept {
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
		public://total
			void			GetJsonData(const nlohmann::json& data) {
				if (data.contains("__typename") && !data["__typename"].is_null()) {
					SetType((std::string)data["__typename"]);
				}
				//個別
				if (data.contains("slots") && !data["slots"].is_null()) {
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
				}
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
					GetJsonDataWeaponMod(data);
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
					GetJsonDataWeaponMod(data);
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
					GetJsonDataWeaponMod(data);
					break;
				case EnumItemProperties::ItemPropertiesSurgicalKit:
					m_IntParams[0] = data["useTime"];
					break;
				case EnumItemProperties::ItemPropertiesWeapon:
					GetJsonDataWeapon(data);
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					GetJsonDataWeaponMod(data);
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
				else {
					{
						if (LEFT == "ChildParts") {
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
					case EnumItemProperties::ItemPropertiesBarrel:
						SetDataWeaponMod(LEFT, Args);
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
						break;
					case EnumItemProperties::ItemPropertiesMagazine:
						SetDataWeaponMod(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesMedicalItem:
						break;
					case EnumItemProperties::ItemPropertiesMelee:
						break;
					case EnumItemProperties::ItemPropertiesMedKit:
						break;
					case EnumItemProperties::ItemPropertiesNightVision:
						break;
					case EnumItemProperties::ItemPropertiesPainkiller:
						break;
					case EnumItemProperties::ItemPropertiesPreset:
						break;
					case EnumItemProperties::ItemPropertiesScope:
						SetDataWeaponMod(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesSurgicalKit:
						break;
					case EnumItemProperties::ItemPropertiesWeapon:
						SetDataWeapon(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesWeaponMod:
						SetDataWeaponMod(LEFT, Args);
						break;
					case EnumItemProperties::ItemPropertiesStim:
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
						if (m.m_Data.size() > 0) {
							outputfile << "ChildParts=[\n";
							std::vector<std::string> Names;
							for (auto& d : m.m_Data) {
								auto NmBuf = d.GetName();
								if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
									outputfile << "\t" + NmBuf + ((&d != &m.m_Data.back()) ? DIV_STR : "") + "\n";
									Names.emplace_back(NmBuf);
								}
							}
							outputfile << "]\n";
						}
					}
				}
				if (this->m_ConflictPartsID.size() > 0) {
					bool isHit = false;
					std::vector<std::string> Names;
					for (auto& m : this->m_ConflictPartsID) {
						auto NmBuf = m.GetName();
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
				case EnumItemProperties::ItemPropertiesBarrel:
					OutputDataWeaponMod(outputfile);
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
					break;
				case EnumItemProperties::ItemPropertiesMagazine:
					OutputDataWeaponMod(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesMedicalItem:
					break;
				case EnumItemProperties::ItemPropertiesMelee:
					break;
				case EnumItemProperties::ItemPropertiesMedKit:
					break;
				case EnumItemProperties::ItemPropertiesNightVision:
					break;
				case EnumItemProperties::ItemPropertiesPainkiller:
					break;
				case EnumItemProperties::ItemPropertiesPreset:
					break;
				case EnumItemProperties::ItemPropertiesScope:
					OutputDataWeaponMod(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesSurgicalKit:
					break;
				case EnumItemProperties::ItemPropertiesWeapon:
					OutputDataWeapon(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesWeaponMod:
					OutputDataWeaponMod(outputfile);
					break;
				case EnumItemProperties::ItemPropertiesStim:
					break;
				default:
					break;
				}
			}
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
				//追加情報
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
		const auto&	Getweight() const noexcept { return this->m_ItemsData.m_weight; }
		const auto&	GetfleaMarketFee() const noexcept { return this->m_ItemsData.m_fleaMarketFee; }
		const auto&	GetUseTaskID() const noexcept { return this->m_ItemsData.m_UseTaskID; }
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