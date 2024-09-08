#include "MainScene_Item.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	//
	void			ItemList::ItemProperties::ChildItemSettings::CheckData() noexcept {
		for (auto& c : this->m_Data) {
			c.CheckID(DataBase::Instance()->GetItemData().get());
		}
	}
	void			ItemList::ItemProperties::SetParent() noexcept {
		for (auto& cp : this->SetModSlots()) {
			cp.CheckData();
		}
		for (auto& cp : this->SetContainsItem()) {
			cp.CheckID(DataBase::Instance()->GetItemData().get());
		}
		//
		for (auto& cp : this->SetModSlots()) {
			for (const auto& c : cp.GetData()) {
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.GetID());
				if(ptr) {
					cp.SetTypeID(ptr->GetTypeID());
				}
			}
		}
		//干渉
		for (auto& cp : this->SetConflictPartsID()) {
			cp.CheckID(DataBase::Instance()->GetItemData().get());
		}
	}
	const int		ItemList::ItemProperties::GetArmerClass() const noexcept {
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
	const int		ItemList::ItemProperties::GetCapacity() const noexcept {
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
	void			ItemList::ItemProperties::DrawInfoMelee(int xp, int yp, int* xofs, int* yofs) const noexcept {
		int ysiz = LineHeight * 6 / 10;

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"切り付けダメージ :%3d", this->GetSlashDamage());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"切り付けダメージ :%3d", this->GetSlashDamage()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"突きダメージ :%3d", this->GetstabDamage());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"突きダメージ :%3d", this->GetstabDamage()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"判定判定 :%3.1f", this->GethitRadius());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"判定判定 :%3.1f", this->GethitRadius()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
	}
	void			ItemList::ItemProperties::DrawInfoKey(int xp, int yp, int* xofs, int* yofs) const noexcept {
		int ysiz = LineHeight * 6 / 10;
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"使用回数 :%3d", this->GetUses());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"使用回数 :%3d", this->GetUses()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
	}
	void			ItemList::ItemProperties::DrawInfoNightVision(int xp, int yp, int* xofs, int* yofs) const noexcept {
		int ysiz = LineHeight * 6 / 10;
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"ノイズ強度 :%3.2f", this->GetnoiseIntensity());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"ノイズ強度 :%3.2f", this->GetnoiseIntensity()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"ノイズスケール :%3.1f", this->GetnoiseScale());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"ノイズスケール :%3.1f", this->GetnoiseScale()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"拡散強度 :%3.3f", this->GetdiffuseIntensity());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"拡散強度 :%3.3f", this->GetdiffuseIntensity()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"強さ : % 3.2f", this->Getintensity());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"強さ :%3.2f", this->Getintensity()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
	}
	void			ItemList::ItemProperties::DrawInfoPreset(int xp, int yp, int* xofs, int* yofs) const noexcept {
		WindowSystem::SetMsg(xp, yp + LineHeight / 2 + *yofs, LineHeight, STRX_LEFT, White, Black,
			"デフォルトプリセットかどうか:%s", this->GetDefault() ? "TRUE" : "FALSE");
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
			"デフォルトプリセットかどうか:%s", this->GetDefault() ? "TRUE" : "FALSE") + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
		WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, White, Black,
			"パーツリスト:");
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
			"パーツリスト:") + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
		for (const auto& c : this->m_ContainsItemID) {
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, White, Black,
				" %s", c.GetName().c_str());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				" %s", c.GetName().c_str()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
		}
	}
	void			ItemList::ItemProperties::DrawInfoWeaponMod(int xp, int yp, int* xofs, int* yofs) const noexcept {
		WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetModRecoil() < 0.f) ? Green : Red, Black,
			"Recoil(リコイル変動値):%3.1f %%", this->GetModRecoil());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
			"Recoil(リコイル変動値):%3.1f %%", this->GetModRecoil()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + LineHeight / 2 + *yofs, LineHeight, STRX_LEFT, (this->GetModErgonomics() >= 0.f) ? Green : Red, Black,
			"Ergonomics(エルゴノミクス変動値):%3.1f", this->GetModErgonomics());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
			"Ergonomics(エルゴノミクス変動値):%3.1f", this->GetModErgonomics()) + DXDraw::Instance()->GetUIY(30));
		*yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

		switch (this->GetType()) {
		case EnumItemProperties::ItemPropertiesBarrel:
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetcenterOfImpact() >= 0.f) ? Green : Red, Black,
				"centerOfImpact(跳ね上がり？):%3.2f", this->GetcenterOfImpact());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"centerOfImpact(跳ね上がり？):%3.2f", this->GetcenterOfImpact()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetdeviationMax() >= 0.f) ? Green : Red, Black,
				"deviationMax(偏差の最大値？):%3d", this->GetdeviationMax());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"deviationMax(偏差の最大値？):%3d", this->GetdeviationMax()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			break;
		case EnumItemProperties::ItemPropertiesMagazine:
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, White, Black,
				"Capacity(マガジン容量):%3d", this->GetModCapacity());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"Capacity(マガジン容量):%3d", this->GetModCapacity()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetloadModifier() >= 0.f) ? Green : Red, Black,
				"loadModifier(装弾変動値):%3.1f", this->GetloadModifier());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"loadModifier(装弾変動値):%3.1f", this->GetloadModifier()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetammoCheckModifier() >= 0.f) ? Green : Red, Black,
				"ammoCheckModifier(弾数チェック変動値):%3.1f", this->GetammoCheckModifier());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"ammoCheckModifier(弾数チェック変動値):%3.1f", this->GetammoCheckModifier()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetmalfunctionChance() <= 0.f) ? Green : Red, Black,
				"malfunctionChance(ジャム変動値):%3.1f", this->GetmalfunctionChance());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"malfunctionChance(ジャム変動値):%3.1f", this->GetmalfunctionChance()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			break;
		case EnumItemProperties::ItemPropertiesScope:
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, White, Black,
				"SightingRange(照準距離):%3d", this->GetSightingRange());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"SightingRange(照準距離):%3d", this->GetSightingRange()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			break;
		case EnumItemProperties::ItemPropertiesWeaponMod:
			WindowSystem::SetMsg(xp, yp + LineHeight/2 + *yofs, LineHeight, STRX_LEFT, (this->GetaccuracyModifier() <= 0.f) ? Green : Red, Black,
				"accuracyModifier(精度変動値):%3.1f", this->GetaccuracyModifier());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(LineHeight,
				"accuracyModifier(精度変動値):%3.1f", this->GetaccuracyModifier()) + DXDraw::Instance()->GetUIY(30)); *yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			break;
		default:
			break;
		}
	}
	void			ItemList::ItemProperties::DrawInfoWeapon(int xp, int yp, int* xofs, int* yofs) const noexcept {
		int ysiz = LineHeight * 6 / 10;
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"RecoilVertical    (縦リコイル)     :%3d %%", this->GetWeaponRecoilVertical());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"RecoilVertical    (縦リコイル)     :%3d %%", this->GetWeaponRecoilVertical()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + ysiz / 2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"RecoilHorizontal  (横リコイル)     :%3d %%", this->GetWeaponRecoilHorizontal());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"RecoilHorizontal  (横リコイル)     :%3d %%", this->GetWeaponRecoilHorizontal()) + DXDraw::Instance()->GetUIY(30));
		*yofs += ysiz + DXDraw::Instance()->GetUIY(5);

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"Ergonomics        (エルゴノミクス) :%3.1f", this->GetWeaponErgonomics());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"Ergonomics        (エルゴノミクス) :%3.1f", this->GetWeaponErgonomics()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"SightRange        (照準距離)       :%3d %%", this->GetWeaponSightingRange());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"SightRange        (照準距離)       :%3d %%", this->GetWeaponSightingRange()) + DXDraw::Instance()->GetUIY(30));
		*yofs += ysiz + DXDraw::Instance()->GetUIY(5);

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"FireRate          (発射速度)       :%3d %%", this->GetWeaponFireRate());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"FireRate          (発射速度)       :%3d %%", this->GetWeaponFireRate()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"CenterOfImpact    (跳ね上がり？)   :%3.2f %%", this->GetWeaponcenterOfImpact());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"CenterOfImpact    (跳ね上がり？)   :%3.2f %%", this->GetWeaponcenterOfImpact()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"deviationCurve    (偏差の曲線？)   :%3.1f %%", this->GetWeapondeviationCurve());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"deviationCurve    (偏差の曲線？)   :%3.1f %%", this->GetWeapondeviationCurve()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"recoilDispersion  (リコイルの分散？):%3d %%", this->GetWeaponrecoilDispersion());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"recoilDispersion  (リコイルの分散？):%3d %%", this->GetWeaponrecoilDispersion()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"recoilAngle       (リコイルの角度？):%3d %%", this->GetWeaponrecoilAngle());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"recoilAngle       (リコイルの角度？):%3d %%", this->GetWeaponrecoilAngle()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"cameraRecoil      (カメラリコイル？):%3.1f %%", this->GetWeaponcameraRecoil());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"cameraRecoil      (カメラリコイル？):%3.1f %%", this->GetWeaponcameraRecoil()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"cameraSnap        (カメラスナップ？):%3.1f %%", this->GetWeaponcameraSnap());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"cameraSnap        (カメラスナップ？):%3.1f %%", this->GetWeaponcameraSnap()) + DXDraw::Instance()->GetUIY(30));
		WindowSystem::SetMsg(xp + *xofs / 2, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"deviationMax      (偏差の最大値？)  :%3d %%", this->GetWeapondeviationMax());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"deviationMax      (偏差の最大値？)  :%3d %%", this->GetWeapondeviationMax()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);

		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"convergence       (収束？)          :%3.1f %%", this->GetWeaponconvergence());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"convergence       (収束？)          :%3.1f %%", this->GetWeaponconvergence()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
	}
	void			ItemList::ItemProperties::DrawInfoMed(int xp, int yp, int* xofs, int* yofs) const noexcept {
		int ysiz = LineHeight * 6 / 10;
		WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
			"使用時間 :%3.1f", this->GetMedUseTime());
		*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
			"使用時間 :%3.1f", this->GetMedUseTime()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		for (auto& m : m_cures) {
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"治療内容 :%s", m.c_str());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"治療内容 :%s", m.c_str()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
		}
		switch (m_Type) {
		case EnumItemProperties::ItemPropertiesMedicalItem:
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"使用回数 :%3d", this->GetMedUses());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"使用回数 :%3d", this->GetMedUses()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			break;
		case EnumItemProperties::ItemPropertiesMedKit:
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"HP :%3d", this->GetHitpoints());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"HP :%3d", this->GetHitpoints()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);

			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"回復消費最大値 :%3d", this->GetmaxHealPerUse());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"回復消費最大値 :%3d", this->GetmaxHealPerUse()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			if (this->GethpCostLightBleeding() > 0) {
				WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
					"軽出血消費 :%3d", this->GethpCostLightBleeding());
				*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
					"軽出血消費 :%3d", this->GethpCostLightBleeding()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			}
			if (this->GethpCostHeavyBleeding() > 0) {
				WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
					"重出血消費 :%3d", this->GethpCostHeavyBleeding());
				*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
					"重出血消費 :%3d", this->GethpCostHeavyBleeding()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			}
			break;
		case EnumItemProperties::ItemPropertiesPainkiller:
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"使用回数 :%3d", this->GetMedUses());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"使用回数 :%3d", this->GetMedUses()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"鎮痛時間 :%3d", this->GetpainkillerDuration());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"鎮痛時間 :%3d", this->GetpainkillerDuration()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"エネルギーへの影響 :%3d", this->GetenergyImpact());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"エネルギーへの影響 :%3d", this->GetenergyImpact()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"水分への影響 :%3d", this->GethydrationImpact());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"水分への影響 :%3d", this->GethydrationImpact()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			break;
		case EnumItemProperties::ItemPropertiesSurgicalKit:
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"使用回数 :%3d", this->GetMedUses());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"使用回数 :%3d", this->GetMedUses()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"最小回復 :%3.1f", this->GetminLimbHealth());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"最小回復 :%3.1f", this->GetminLimbHealth()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
				"最高回復 :%3.1f", this->GetmaxLimbHealth());
			*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
				"最高回復 :%3.1f", this->GetmaxLimbHealth()) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
			break;
		case EnumItemProperties::ItemPropertiesStim:
			for (const auto& s : m_stimEffects) {
				if (s.GetskillName() != "") {
					WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"スキルへの影響 :%s", s.GetskillName().c_str());
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"スキルへの影響 :%s", s.GetskillName().c_str()) + DXDraw::Instance()->GetUIY(30));
					WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(300), yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"%s%3.2f", (s.Getvalue() >= 0) ? "+" : "", s.Getvalue());
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"%s%3.2f", (s.Getvalue() >= 0) ? "+" : "", s.Getvalue()) + DXDraw::Instance()->GetUIY(600) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
				}
				else {
					WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"効果タイプ :%s", s.Gettype().c_str());
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"効果タイプ :%s", s.Gettype().c_str()) + DXDraw::Instance()->GetUIY(30));
					WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(300), yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"%3.2f%s", s.Getvalue() * (s.Getpercent() ? 100.f : 1.f), (s.Getpercent() ? "%%" : ""));
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"%3.2f%s", s.Getvalue()*(s.Getpercent() ? 100.f : 1.f), (s.Getpercent() ? "%%" : "")) + DXDraw::Instance()->GetUIY(600) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
				}
				if (s.Getchance() < 1.f) {
					WindowSystem::SetMsg(xp, yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"確率 :%3.1f%%", s.Getchance() * 100.f);
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"確率 :%3.1f%%", s.Getchance()*100.f) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
				}
				{
					WindowSystem::SetMsg(xp, yp + ysiz / 2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"効果までの遅延 :%3d", s.Getdelay());
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"効果までの遅延 :%3d", s.Getdelay()) + DXDraw::Instance()->GetUIY(30));
					WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(300), yp + ysiz/2 + *yofs, ysiz, STRX_LEFT, White, Black,
						"%3d秒", s.Getduration());
					*xofs = std::max(*xofs, WindowSystem::GetMsgLen(ysiz,
						"%3d秒", s.Getduration()) + DXDraw::Instance()->GetUIY(600) + DXDraw::Instance()->GetUIY(30)); *yofs += ysiz + DXDraw::Instance()->GetUIY(5);
				}
			}
			break;
		default:
			break;
		}
	}
	//
	void			ItemList::SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
		if (LEFT == "Itemtype") { this->m_ItemsData.m_TypeID.SetName(Args[0]); }
		else if (LEFT == "Map") {
			this->m_ItemsData.m_MapID.resize(this->m_ItemsData.m_MapID.size() + 1);
			this->m_ItemsData.m_MapID.back().SetName(Args[0]);
		}
		else if (LEFT == "width") { this->m_ItemsData.m_width = std::stoi(Args[0]); }
		else if (LEFT == "height") { this->m_ItemsData.m_height = std::stoi(Args[0]); }
		else if (LEFT == "weight") { this->m_ItemsData.m_weight = std::stof(Args[0]); }
		else if (LEFT == "fleaMarketFee") { this->m_ItemsData.m_fleaMarketFee = std::stoi(Args[0]); }

		m_ItemsData.m_properties.SetData(LEFT, Args);
	}
	void			ItemList::Load_Sub() noexcept {
		if (this->m_ItemsData.m_TypeID.GetName() == "") {
			std::string ErrMes = "Error : Invalid ID by CheckID";
			ErrMes += "[";
			ErrMes += this->GetFilePath();
			ErrMes += "]";
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
		}
		this->m_ItemsData.m_TypeID.CheckID(DataBase::Instance()->GetItemTypeData().get());
		for (auto& m : this->m_ItemsData.m_MapID) {
			m.CheckID(DataBase::Instance()->GetMapData().get());
		}
		//
		{
			auto* typePtr = DataBase::Instance()->GetItemTypeData()->FindPtr(this->GetTypeID());
			if (typePtr) {
				auto* catPtr = DataBase::Instance()->GetItemCategoryData()->FindPtr(typePtr->GetCategoryID());
				if (catPtr->GetName() == "Weapons") {
					this->m_ItemsData.m_isWeapon = true;
				}
				else if (catPtr->GetName() == "WeaponMods") {
					this->m_ItemsData.m_isWeaponMod = true;
				}
				else if (catPtr->GetName() == "Medical") {
					this->m_ItemsData.m_isMedItem = true;
				}
			}
		}
	}
	void			ItemList::SetParent() noexcept {
		for (auto& s : this->m_ItemsData.m_sellFor) {
			s.CheckID(DataBase::Instance()->GetTraderData().get(), false);//Invalidはフリマなのでエラー出さない
		}
		this->m_ItemsData.m_properties.SetParent();
	}
	void			ItemList::SetParent2() noexcept {
		//自分を干渉相手にしている奴を探してそいつもリストに入れる　相思相愛
		for (const auto& t : DataBase::Instance()->GetItemData()->GetList()) {
			for (auto& cp : t.GetConflictParts()) {
				if (cp.GetID() == this->GetID()) {
					this->m_ItemsData.m_properties.SetConflictPartsID().resize(this->m_ItemsData.m_properties.SetConflictPartsID().size() + 1);
					this->m_ItemsData.m_properties.SetConflictPartsID().back().SetID(t.GetID());
					this->m_ItemsData.m_properties.SetConflictPartsID().back().SetName(t.GetName());
				}
			}
		}
		//
		this->m_ItemsData.m_ParentPartsID.clear();
		for (const auto& t : DataBase::Instance()->GetItemData()->GetList()) {
			for (auto& cp : t.GetChildParts()) {
				for (auto& c : cp.GetData()) {
					if (c.GetID() == this->GetID()) {
						this->m_ItemsData.m_ParentPartsID.emplace_back(t.GetID());
					}
				}
			}
		}
	}
	void			ItemList::WhenAfterLoad_Sub() noexcept {}
	const int		ItemList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		auto* Pad = PadControl::Instance();
		int xs = xsize;
		int  Xsize = 0;

		int GraphXsize = (GetIcon().GetGraph()) ? ((GetIcon().GetXSize() >= GetIcon().GetYSize()) ? (ysize * GetIcon().GetXSize() / GetIcon().GetYSize()) : (ysize * GetIcon().GetYSize() / GetIcon().GetXSize())) : 0;
		bool IsLocked = PlayerData::Instance()->GetItemLock(this->GetIDstr().c_str());
		int FirXSize = (IsFir || IsLocked) ? 36 : 0;
		int  StrXSize = 0;

		std::string Name = "";
		std::string AfterName = "";
		if (count > 0) {
			AfterName += " x" + std::to_string(count);
		}
		if (!IsIconOnly) {
			Name = this->GetName();
			if (xsize > 0) {
				int countbuf = 0;
				while (true) {
					StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName);
					if ((xs - (GraphXsize + FirXSize)) < StrXSize) {
						Name = Name.substr(0, (size_t)(((int)(Name.size()) * (xs - (GraphXsize + FirXSize)) / StrXSize) - static_cast<int>(AfterName.size()) - 2)) + "…";
					}
					else {
						break;
					}
					countbuf++;
					if (countbuf > 10) {
						Name = "…";
						StrXSize = LineHeight * 9 / 10;
						break;
					}
				}
			}
			else {
				StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName);
			}
			Xsize += StrXSize;
		}
		int GraphXPos = xp + FirXSize + Xsize;
		if (GetIcon().GetGraph()) {
			Xsize += (GraphXsize + FirXSize);
		}
		if (!IsIconOnly) {
			Xsize = std::max(Xsize, FirXSize + WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName));
		}
		else {
			if (count > 0) {
				Xsize = std::max(Xsize, (GraphXsize + FirXSize) + WindowSystem::GetMsgLen(LineHeight * 9 / 10, AfterName));
			}
		}


		xs = std::max(xs, Xsize);

		if (WindowSystem::SetMsgClickBox(xp, yp, xp + xs, yp + ysize, ysize, defaultcolor, false, Clickactive, "")) {
			auto sizeXBuf = DXDraw::Instance()->GetUIY(900);
			auto sizeYBuf = DXDraw::Instance()->GetUIY(0);
			DrawWindow(nullptr, 0, DXDraw::Instance()->GetUIY(1920), DXDraw::Instance()->GetUIY(1080), &sizeXBuf, &sizeYBuf);//試しにサイズ計測
			//
			signed long long FreeID = (signed long long)GetID() + 0xFFFF;
			//同じIDの奴いたら消そうぜ
			int Size = (int)WindowMngr->Get().size();
			bool isHit = false;
			for (int i = 0; i < Size; i++) {
				if (WindowMngr->Get()[i]->m_FreeID == FreeID) {
					isHit = true;
					WindowMngr->Get()[i]->SetActiveSwitch(true);
					break;
				}
			}
			if (Pad->GetMS_Y() <= LineHeight) {
				isHit = true;
			}

			if (!isHit) {
				//ウィンドウ追加
				WindowMngr->Add()->Set(Pad->GetMS_X() - sizeXBuf / 2, Pad->GetMS_Y(), sizeXBuf, sizeYBuf, 0, GetName().c_str(), false, true, FreeID, [&](WindowMySystem::WindowControl* win) {
					DataBase::Instance()->GetItemData()->FindPtr((ItemID)(win->m_FreeID - 0xFFFF))->DrawWindow(win, Gray10, win->GetPosX(), win->GetPosY());
				});
			}
		}
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, GetIcon().GetGraph(), GraphXPos + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
		}
		if (!IsIconOnly) {
			if (count > 0) {
				WindowSystem::SetMsg(xp + FirXSize, yp + ysize / 2, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s x%1d", Name.c_str(), count);
			}
			else {
				WindowSystem::SetMsg(xp + FirXSize, yp + ysize / 2, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s", Name.c_str());
			}
		}
		else {
			if (count > 0) {
				WindowSystem::SetMsg(xp + (GraphXsize + FirXSize), yp + ysize / 2, LineHeight * 9 / 10, STRX_LEFT, White, Black, "x%1d", count);
			}
		}

		if (IsLocked) {
			if (DrawGraphs::Instance()->GetLockGraph().GetGraph()) {
				WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Front, DrawGraphs::Instance()->GetLockGraph().GetGraph(), xp + FirXSize / 2, yp + ysize / 2, 1.f, 0.f, true);
			}
		}
		if (IsFir) {
			if (DrawGraphs::Instance()->GetFirGraph().GetGraph()) {
				WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, DrawGraphs::Instance()->GetFirGraph().GetGraph(), xp + FirXSize / 2, yp + ysize / 2, 1.f, 0.f, true);
			}
		}

		if (IsDrawBuy) {
			if (IntoMouse(xp, yp, xp + xs, yp + ysize)) {
				TraderID ID = InvalidID;
				int Value = -1;
				if (GetSellValue(&ID, &Value)) {
					auto Color = Green;
					std::string TraderName = "Flea Market";
					if (ID != InvalidID) {
						auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(ID);
						if (ptr) {
							TraderName = ptr->GetName();
							Color = ptr->GetColors(50);
						}
					}
					WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
						FontPool::FontType::MS_Gothic, LineHeight,
						STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), Color, Black,
						"最高値:%s = %d", TraderName.c_str(), Value
					);
					WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
						FontPool::FontType::MS_Gothic, LineHeight,
						STRX_RIGHT, STRY_TOP, Pad->GetMS_X(), Pad->GetMS_Y(), Color, Black,
						"マス単価: %d", Value / (Getwidth()*Getheight())
					);
				}
			}
		}
		return Xsize;
	}
	void			ItemList::DrawWindow(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs, int* ys) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		auto* InterParts = InterruptParts::Instance();
		int xofs = DXDraw::Instance()->GetUIY(600) * 2;
		int yofs = LineHeight + DXDraw::Instance()->GetUIY(5);
		int yofs2 = yofs;
		if (GetIcon().GetGraph()) {
			WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 64);
			float Scale = 1.f;
			float rad = 0.f;
			WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, GetIcon().GetGraph(),
				xp + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale),
				yp + LineHeight + GetIcon().GetYSize() / 2, Scale, rad, false);
			WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
		}
		{
			//タスク
			{
				{
					int xofsBuf = DXDraw::Instance()->GetUIY(10);
					int OLDyofs = yofs;
					int yofsBuf = OLDyofs;
					int count = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									count++;
									break;
								}
							}
						}
					}

					int count2 = 0;
					for (auto& tasks : DataBase::Instance()->GetTaskData()->SetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									if (count2 == 0) {
										WindowSystem::SetMsg(xp + xofsBuf, yp + LineHeight / 2 + yofsBuf, LineHeight, STRX_LEFT, White, Black, "Firタスク納品:"); yofsBuf += LineHeight + DXDraw::Instance()->GetUIY(5);
									}
									if (count > 1 && (count / 2) <= count2) {
										xofsBuf = DXDraw::Instance()->GetUIY(600) + DXDraw::Instance()->GetUIY(10);
										yofs = std::max(yofs, yofsBuf);
										yofsBuf = OLDyofs + LineHeight + DXDraw::Instance()->GetUIY(5);
									}
									tasks.Draw(xp + xofsBuf, yp + yofsBuf, DXDraw::Instance()->GetUIY(600) - DXDraw::Instance()->GetUIY(10) * 2, LineHeight, w.GetValue(), !WindowMngr->PosHitCheck(window)); yofsBuf += LineHeight + DXDraw::Instance()->GetUIY(5);
									count2++;
									break;
								}
							}
						}
					}
					yofs = std::max(yofs, yofsBuf);
				}
				{
					int xofsBuf = DXDraw::Instance()->GetUIY(10);
					int OLDyofs = yofs;
					int yofsBuf = OLDyofs;
					int count = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									count++;
									break;
								}
							}
						}
					}

					int count2 = 0;
					for (auto& tasks : DataBase::Instance()->GetTaskData()->SetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									if (count2 == 0) {
										WindowSystem::SetMsg(xp + xofsBuf, yp + LineHeight / 2 + yofsBuf, LineHeight, STRX_LEFT, White, Black, "Firでないタスク納品:"); yofsBuf += LineHeight + DXDraw::Instance()->GetUIY(5);
									}
									if (count > 1 && (count / 2) <= count2) {
										xofsBuf = DXDraw::Instance()->GetUIY(600) + DXDraw::Instance()->GetUIY(10);
										yofs = std::max(yofs, yofsBuf);
										yofsBuf = OLDyofs + LineHeight + DXDraw::Instance()->GetUIY(5);
									}
									tasks.Draw(xp + xofsBuf, yp + yofsBuf, DXDraw::Instance()->GetUIY(600) - DXDraw::Instance()->GetUIY(10) * 2, LineHeight, w.GetValue(), !WindowMngr->PosHitCheck(window)); yofsBuf += LineHeight + DXDraw::Instance()->GetUIY(5);
									count2++;
									break;
								}
							}
						}
					}
					yofs = std::max(yofs, yofsBuf);
				}
				{
					int xofsBuf = DXDraw::Instance()->GetUIY(10);
					int OLDyofs = yofs;
					int yofsBuf = OLDyofs;
					int count = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						bool isHit = false;
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (isHit) {
							continue;
						}
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
								if (w.GetID() == GetID()) {
									count++;
									break;
								}
							}
						}
					}

					int count2 = 0;
					for (auto& tasks : DataBase::Instance()->GetTaskData()->SetList()) {
						bool isHit = false;
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (isHit) {
							continue;
						}
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
								if (w.GetID() == GetID()) {
									if (count2 == 0) {
										WindowSystem::SetMsg(xp + xofsBuf, yp + LineHeight / 2 + yofsBuf, LineHeight, STRX_LEFT, White, Black, "タスクに必要:"); yofsBuf += LineHeight + DXDraw::Instance()->GetUIY(5);
									}
									if (count > 1 && (count / 2) <= count2) {
										xofsBuf = DXDraw::Instance()->GetUIY(600) + DXDraw::Instance()->GetUIY(10);
										yofs = std::max(yofs, yofsBuf);
										yofsBuf = OLDyofs + LineHeight + DXDraw::Instance()->GetUIY(5);
									}
									tasks.Draw(xp + xofsBuf, yp + yofsBuf, DXDraw::Instance()->GetUIY(600) - DXDraw::Instance()->GetUIY(10) * 2, LineHeight, w.GetValue(), !WindowMngr->PosHitCheck(window)); yofsBuf += LineHeight + DXDraw::Instance()->GetUIY(5);
									count2++;
									break;
								}
							}
						}
					}
					yofs = std::max(yofs, yofsBuf);
				}
			}
			//ハイドアウト開放
			{
				bool isFirst = true;
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						for (const auto& w : Ld.m_ItemReq) {
							if (w.GetID() == GetID()) {
								if (isFirst) {
									isFirst = false;
									WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(10), yp + LineHeight/2 + yofs, LineHeight, STRX_LEFT, White, Black, "ハイドアウト開放:");
									xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "ハイドアウト開放:")); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
								}
								L.Draw(xp + DXDraw::Instance()->GetUIY(10), yp + yofs, DXDraw::Instance()->GetUIY(600) - DXDraw::Instance()->GetUIY(10) * 2, LineHeight, (int)(&Ld - &L.GetLvData().front()) + 1, defaultcolor, !WindowMngr->PosHitCheck(window), w.GetValue());
								yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
								break;
							}
						}
					}
				}
			}
			//ハイドアウトクラフト素材
			{
				bool isFirst = true;
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
						for (const auto& cf : Ld.m_ItemCraft) {
							bool isHit = false;
							if (!isHit) {
								for (const auto& I : cf.m_ItemReq) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (!isHit) {
								for (const auto& I : cf.m_ItemReward) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (isHit) {
								if (isFirst) {
									isFirst = false;
									WindowSystem::SetMsg(xp, yp + LineHeight/2 + yofs, LineHeight, STRX_LEFT, White, Black, "ハイドアウトクラフト:");
									xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "ハイドアウトクラフト:")); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
								}
								xofs = std::max(xofs, L.DrawCraft(window, defaultcolor, xp, yp + yofs, DXDraw::Instance()->GetUIY(64), Lv, (int)(&cf - &Ld.m_ItemCraft.front()), true, true, 0));
								yofs += DXDraw::Instance()->GetUIY(64);
							}
						}
					}
				}
			}
			//トレーダー交換
			{
				bool isFirst = true;
				for (auto&L : DataBase::Instance()->GetTraderData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
						for (const auto& cf : Ld.m_ItemBarters) {
							bool isHit = false;
							if (!isHit) {
								for (const auto& I : cf.m_ItemReq) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (!isHit) {
								for (const auto& I : cf.m_ItemReward) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (isHit) {
								if (isFirst) {
									isFirst = false;
									WindowSystem::SetMsg(xp, yp + LineHeight/2 + yofs, LineHeight, STRX_LEFT, White, Black, "トレーダー交換:");
									xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "トレーダー交換:")); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
								}
								xofs = std::max(xofs, L.DrawBarter(window, defaultcolor, xp, yp + yofs, DXDraw::Instance()->GetUIY(64), Lv, (int)(&cf - &Ld.m_ItemBarters.front()), true, true, 0));
								yofs += DXDraw::Instance()->GetUIY(64);
							}
						}
					}
				}
			}
			//武器ならば
			if (this->m_ItemsData.m_isWeapon) {
				if (WindowSystem::SetMsgClickBox(xp, yp + yofs, xp + DXDraw::Instance()->GetUIY(800), yp + LineHeight + yofs, LineHeight, Green, false, !WindowMngr->PosHitCheck(window), "GotoPreset")) {
					InterParts->GotoNext(BGSelect::Custom);
					InterParts->SetInitParam(0, GetID());//武器ID
					InterParts->SetInitParam(1, InvalidID);//プリセットID
				}
				yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
			}
			//プロパティ
			this->m_ItemsData.m_properties.DrawInfo(xp, yp, &xofs, &yofs);
			//
			{
				WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(10), yp + LineHeight/2 + yofs, LineHeight, STRX_LEFT, White, Black, "説明:");
				xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "説明:")); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
				int ysize = LineHeight * 6 / 10;
				int ysizetotal = (int)WindowSystem::DrawControl::Instance()->SetStringAutoFit(WindowSystem::DrawLayer::Normal, FontPool::FontType::MS_Gothic, ysize, xp + DXDraw::Instance()->GetUIY(10), yp + yofs, xp + xofs - DXDraw::Instance()->GetUIY(10), yp + yofs + ysize * 15,
					White, Black, this->GetInformation_Jpn()); yofs += ysizetotal + DXDraw::Instance()->GetUIY(5);
			}
			//
			{
				int Max = 0;
				for (const auto& cp : this->GetChildParts()) {
					Max += (int)cp.GetData().size();
				}
				if (Max > 0) {
					WindowSystem::SetMsg(xp, yp + LineHeight / 2 + yofs, LineHeight, STRX_LEFT, White, Black, "ChildrenMods:");
					xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "ChildrenMods:") + DXDraw::Instance()->GetUIY(30)); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

					int ysize = DXDraw::Instance()->GetUIY(36);
					int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);

					int ofset = (int)(this->m_Scroll.at(0).GetNowScrollYPer()*(std::max(0, Max - 4 + 1)*ysizeAdd));
					int yofs_t = yofs;
					yofs_t += LineHeight + DXDraw::Instance()->GetUIY(5);
					int ypMin = yp + yofs_t;
					int ypMax = yp + yofs_t + ysizeAdd * 4;
					int yp1 = yp + yofs_t - ofset;
					for (const auto& cp : this->GetChildParts()) {
						for (const auto& c : cp.GetData()) {
							if (ypMin - ysizeAdd < yp1 && yp1 < ypMax) {
								if (ypMin < yp1 && yp1 < ypMax - ysizeAdd) {
									WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
								}
								else {
									if (yp1 <= ypMin) {
										WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypMin - yp1) / ysizeAdd, 0, 255));
									}
									else {
										WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp1 - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
									}
								}
								auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.GetID());
								ptr->Draw(xp + DXDraw::Instance()->GetUIY(30), yp1, DXDraw::Instance()->GetUIY(800), ysize, 0, Gray25, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false);
							}
							yofs_t += ysizeAdd;
							yp1 += ysizeAdd;
						}
					}
					WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
					//スクロールバー
					{
						float Total = (float)(yofs_t - yofs) / (ypMax - ypMin);
						if (Total > 1.f) {
							this->m_Scroll.at(0).ScrollBox(xp + DXDraw::Instance()->GetUIY(30), ypMin, xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30), ypMax, Total, !WindowMngr->PosHitCheck(window));
						}
					}
					yofs = ypMax - yp;
					//
					yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
				}
			}
			{
				int Max = (int)this->m_ItemsData.m_ParentPartsID.size();
				if (Max > 0) {
					WindowSystem::SetMsg(xp, yp + LineHeight/2 + yofs, LineHeight, STRX_LEFT, White, Black, "ParentMods:");
					xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "ParentMods:") + DXDraw::Instance()->GetUIY(30)); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

					int ysize = DXDraw::Instance()->GetUIY(36);
					int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);

					int ofset = (int)(this->m_Scroll.at(1).GetNowScrollYPer()*(std::max(0, Max - 4 + 1)*ysizeAdd));
					int yofs_t = yofs;
					yofs_t += LineHeight + DXDraw::Instance()->GetUIY(5);
					int ypMin = yp + yofs_t;
					int ypMax = yp + yofs_t + ysizeAdd * 4;
					int yp1 = yp + yofs_t - ofset;
					for (const auto& c : this->m_ItemsData.m_ParentPartsID) {
						if (ypMin - ysizeAdd < yp1 && yp1 < ypMax) {
							if (ypMin < yp1 && yp1 < ypMax - ysizeAdd) {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
							}
							else {
								if (yp1 <= ypMin) {
									WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypMin - yp1) / ysizeAdd, 0, 255));
								}
								else {
									WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp1 - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
								}
							}
							auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c);
							ptr->Draw(xp + DXDraw::Instance()->GetUIY(30), yp1, DXDraw::Instance()->GetUIY(800), ysize, 0, Gray25, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false);
						}
						yofs_t += ysizeAdd;
						yp1 += ysizeAdd;
					}
					WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
					//スクロールバー
					{
						float Total = (float)(yofs_t - yofs) / (ypMax - ypMin);
						if (Total > 1.f) {
							this->m_Scroll.at(1).ScrollBox(xp + DXDraw::Instance()->GetUIY(30), ypMin, xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30), ypMax, Total, !WindowMngr->PosHitCheck(window));
						}
					}
					yofs = ypMax - yp;
					//
					yofs += LineHeight + DXDraw::Instance()->GetUIY(5);
				}
			}
			//
		}
		if (GetIcon().GetGraph()) {
			xofs = std::max<int>(xofs, GetIcon().GetXSize());
			yofs2 += std::max(GetIcon().GetYSize(), 64 * 2);
		}
		else {
			yofs2 += 64 * 2;
		}
		yofs = std::max(yofs, yofs2);
		//
		if (xs) {
			*xs = std::max(*xs, xofs);
		}
		if (ys) {
			*ys = std::max(*ys, yofs - LineHeight);
		}
	}
	//
	void ItemJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		m_categorytypes = data["category"]["normalizedName"];
		if (data.contains("width")) { this->m_ItemsData.m_width = data["width"]; }
		if (data.contains("height")) { this->m_ItemsData.m_height = data["height"]; }
		m_ItemsData.m_weight = data["weight"];
		for (const auto& sf : data["sellFor"]) {
			m_ItemsData.m_sellFor.resize(m_ItemsData.m_sellFor.size() + 1);
			m_ItemsData.m_sellFor.back().Set(sf["vendor"]["name"], sf["price"]);
		}
		for (const auto& ts : data["conflictingItems"]) {
			m_ItemsData.m_properties.SetConflictPartsID().resize(m_ItemsData.m_properties.SetConflictPartsID().size() + 1);
			m_ItemsData.m_properties.SetConflictPartsID().back().SetName(ts["name"]);
		}
		if (data.contains("properties")) {
			if (!data["properties"].is_null()) {
				m_ItemsData.m_properties.GetJsonData(data);
			}
		}
	}
	void ItemJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		outputfile << "Itemtype=" + this->m_categorytypes + "\n";
		outputfile << "width=" + std::to_string(this->m_ItemsData.m_width) + "\n";
		outputfile << "height=" + std::to_string(this->m_ItemsData.m_height) + "\n";
		outputfile << "weight=" + std::to_string(this->m_ItemsData.m_weight) + "\n";
		outputfile << "fleaMarketFee=" + std::to_string(this->m_ItemsData.m_fleaMarketFee) + "\n";
		this->m_ItemsData.m_properties.OutputData(outputfile);
	}
	//
	void ItemData::UpdateAfterbyJson(void) noexcept {
		for (auto& L : this->m_List) {
			for (auto& jd : GetJsonDataList()) {
				if (L.GetIDstr() == jd->m_id) {
					//既存のものを保持しておく
					std::ofstream outputfile(L.GetFilePath(), std::ios::app);
					auto size = L.GetMapID().size();
					for (size_t loop = 0; loop < size; loop++) {
						auto& m = L.GetMapID().at(loop);

						bool isHit = false;
						for (size_t loop2 = loop + 1; loop2 < size; loop2++) {
							auto& m2 = L.GetMapID().at(loop2);
							if (m.GetID() == m2.GetID()) {
								isHit = true;
								break;
							}
						}
						if (isHit) { continue; }
						auto* ptr = DataBase::Instance()->GetMapData()->FindPtr(m.GetID());
						if (ptr) {
							outputfile << "Map=" + ptr->GetName() + "\n";
						}
					}
					outputfile.close();
					break;
				}
			}
		}
		UpdateAfterbyJson_Sub();
	}

	void ItemData::OutputPresetbyJson(void) noexcept {
		for (const auto& jd : GetJsonDataList()) {
			auto& itemdata = (dynamic_cast<ItemJsonData*>(jd.get()))->GetItemsData();
			if (itemdata.m_properties.GetType() == EnumItemProperties::ItemPropertiesPreset) {
				ItemList* ptr = nullptr;
				for (const auto& I : itemdata.m_properties.GetContainsItem()) {
					ItemList* ptrBuf = DataBase::Instance()->GetItemData()->FindPtr(DataBase::Instance()->GetItemData()->FindID(I.GetName()));
					if (ptrBuf && ptrBuf->GetIsWeapon()) {
						ptr = ptrBuf;
						break;
					}
				}
				if (ptr) {
					std::string ChildPath = "data/Preset/";

					auto* typePtr = DataBase::Instance()->GetItemTypeData()->FindPtr(ptr->GetTypeID());
					if (typePtr) {
						ChildPath += typePtr->GetName() + "/";

						CreateDirectory(ChildPath.c_str(), NULL);
					}
					std::string Name = jd->m_CanUseFileName + ".txt";

					{
						std::ofstream outputfile(ChildPath + Name);
						outputfile << "Name=" + jd->m_name + "\n";
						outputfile << "Base=" + ptr->GetName() + "\n";
						for (const auto& I : itemdata.m_properties.GetContainsItem()) {
							ItemList* ptrBuf = DataBase::Instance()->GetItemData()->FindPtr(DataBase::Instance()->GetItemData()->FindID(I.GetName()));
							if (ptrBuf && !ptrBuf->GetIsWeapon()) {
								outputfile << "Parts=" + ptrBuf->GetName() + "\n";
							}
						}
						outputfile.close();
					}
				}
			}
		}
	}
	
};