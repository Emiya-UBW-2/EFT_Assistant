#pragma once
#include"../../../Header.hpp"
#include "MainScene_Parents.hpp"
#include "MainScene_Common.hpp"
#include "MainScene_Map.hpp"
#include "MainScene_ItemCategory.hpp"
#include "MainScene_ItemType.hpp"
#include "MainScene_Trader.hpp"
#include "MainScene_Item.hpp"
#include "MainScene_Preset.hpp"
#include "MainScene_Enemy.hpp"
#include "MainScene_Task.hpp"
#include "MainScene_Hideout.hpp"

namespace FPS_n2 {
	class DataBase : public SingletonBase<DataBase> {
	private:
		friend class SingletonBase<DataBase>;
	private:
		std::unique_ptr<ItemCategoryData> m_ItemCategoryData;
		std::unique_ptr<ItemTypeData> m_ItemTypeData;
		std::unique_ptr<PresetData> m_PresetData;
		std::unique_ptr<EnemyData> m_EnemyData;
		std::unique_ptr<ItemData> m_ItemData;
		std::unique_ptr<TraderData> m_TraderData;
		std::unique_ptr<MapData> m_MapData;
		std::unique_ptr<TaskData> m_TaskData;
		std::unique_ptr<HideoutData> m_HideoutData;
	private:
		DataBase() noexcept {}
		~DataBase() noexcept {}
	public:
		void			SetList(void) noexcept {
			m_ItemCategoryData = std::make_unique<ItemCategoryData>();
			m_ItemTypeData = std::make_unique<ItemTypeData>();
			m_PresetData = std::make_unique<PresetData>();
			m_EnemyData = std::make_unique<EnemyData>();
			m_ItemData = std::make_unique<ItemData>();
			m_TraderData = std::make_unique<TraderData>();
			m_MapData = std::make_unique<MapData>();
			m_TaskData = std::make_unique<TaskData>();
			m_HideoutData = std::make_unique<HideoutData>();
		}
		void			LoadList(bool IsPushLog) noexcept {
			SetUseASyncLoadFlag(TRUE);

			m_ItemCategoryData->LoadList(false);
			m_ItemTypeData->LoadList(false);
			m_PresetData->LoadList(false);
			m_EnemyData->LoadList(false);
			m_ItemData->LoadList(IsPushLog);
			m_ItemData->SetParent();
			m_TraderData->LoadList(false);
			m_MapData->LoadList(false);
			m_TaskData->LoadList(false);
			m_TaskData->SetNeedTasktoID();
			m_HideoutData->LoadList(IsPushLog);

			SetUseASyncLoadFlag(FALSE);
		}
		void			WhenAfterLoadListCommon(void) noexcept {
			m_ItemCategoryData->WhenAfterLoadListCommon();
			m_ItemTypeData->WhenAfterLoadListCommon();
			m_PresetData->WhenAfterLoadListCommon();
			m_EnemyData->WhenAfterLoadListCommon();
			m_ItemData->WhenAfterLoadListCommon();;
			m_TraderData->WhenAfterLoadListCommon();
			m_MapData->WhenAfterLoadListCommon();
			m_TaskData->WhenAfterLoadListCommon();
			m_HideoutData->WhenAfterLoadListCommon();
		}
		void			WhenAfterLoadList(void) noexcept {
			m_ItemCategoryData->WhenAfterLoadList();
			m_ItemTypeData->WhenAfterLoadList();
			m_PresetData->WhenAfterLoadList();
			m_EnemyData->WhenAfterLoadList();
			m_ItemData->WhenAfterLoadList();;
			m_TraderData->WhenAfterLoadList();
			m_MapData->WhenAfterLoadList();
			m_TaskData->WhenAfterLoadList();
			m_HideoutData->WhenAfterLoadList();
		}
	public:
		auto& GetItemCategoryData(void) noexcept { return m_ItemCategoryData; }
		auto& GetItemTypeData(void) noexcept { return m_ItemTypeData; }
		auto& GetPresetData(void) noexcept { return m_PresetData; }
		auto& GetEnemyData(void) noexcept { return m_EnemyData; }
		auto& GetItemData(void) noexcept { return m_ItemData; }
		auto& GetTraderData(void) noexcept { return m_TraderData; }
		auto& GetMapData(void) noexcept { return m_MapData; }
		auto& GetTaskData(void) noexcept { return m_TaskData; }
		auto& GetHideoutData(void) noexcept { return m_HideoutData; }
	};

};
