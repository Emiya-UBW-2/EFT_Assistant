#pragma once
#include "Header.hpp"

namespace FPS_n2 {
	//
	enum class BGSelect {
		Title,
		Task,
		HideOut,
		Item,
		Map,
		Custom,
		Trader,
		ItemList,
		PlayerInfo,
		Max,
	};
	//
	enum class EnumDataType {
		ITEMCATEGORYDATA,
		ITEMTYPEDATA,
		PRESETDATA,
		ENEMYDATA,
		ITEMDATA,
		ITEMDATAJP,
		TRADERDATA,
		MAPDATA,
		TASKDATA,
		HIDEOUTDATA,
	};
	enum class EnumWikiDataType {
		LOOT_WIKI_HTML,
		ITEMDATA_KEY_WIKI_HTML,
		ITEMDATA_KEY,
	};
	//
	enum class EnumEdition : int {
		STANDARD,
		LEFT_BEHIND,
		PREPARE_FOR_ESCAPE,
		EDGE_OF_DARKNESS,
		Max,
	};
	static const char* EnumEditionStr[(int)EnumEdition::Max] = {
		"STANDARD",
		"LEFT BEHIND",
		"PREPARE FOR ESCAPE",
		"EDGE OF DARKNESS",
	};
	//
	enum class EnumTaskObjective {
		TaskObjectiveBasic,
		TaskObjectiveBuildItem,
		TaskObjectiveExperience,
		TaskObjectiveExtract,
		TaskObjectiveItem,
		TaskObjectiveMark,
		TaskObjectivePlayerLevel,
		TaskObjectiveQuestItem,
		TaskObjectiveShoot,
		TaskObjectiveSkill,
		TaskObjectiveTaskStatus,
		TaskObjectiveTraderLevel,
		TaskObjectiveTraderStanding,
		TaskObjectiveUseItem,
		Max,
	};
	static const char* TypesStr[(int)EnumTaskObjective::Max] = {
		"TaskObjectiveBasic",
		"TaskObjectiveBuildItem",
		"TaskObjectiveExperience",
		"TaskObjectiveExtract",
		"TaskObjectiveItem",
		"TaskObjectiveMark",
		"TaskObjectivePlayerLevel",
		"TaskObjectiveQuestItem",
		"TaskObjectiveShoot",
		"TaskObjectiveSkill",
		"TaskObjectiveTaskStatus",
		"TaskObjectiveTraderLevel",
		"TaskObjectiveTraderStanding",
		"TaskObjectiveUseItem",
	};
	//
	enum class EnumCompareMethod {
		Equal,//=
		Higher,//>=
		Lower,//<=
		More,//>
		Less,//<
		Max,
	};
	static const char* CompareMethodStr[] = {
	"=",
	">=",
	"<=",
	">",
	"<",
	};
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
};
