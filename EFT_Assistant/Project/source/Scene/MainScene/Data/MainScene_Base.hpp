#pragma once
#include "../../../Header.hpp"
//*
#include "MainScene_Parents.hpp"
#include "MainScene_Common.hpp"
#include "MainScene_Map.hpp"
#include "MainScene_ItemCategory.hpp"
#include "MainScene_ItemType.hpp"
#include "MainScene_Trader.hpp"
//#include "MainScene_Item.hpp"
#include "MainScene_Preset.hpp"
#include "MainScene_Enemy.hpp"
#include "MainScene_Task.hpp"
#include "MainScene_Hideout.hpp"
//*/
namespace FPS_n2 {
	class DataBase : public SingletonBase<DataBase> {
	private:
		friend class SingletonBase<DataBase>;
	private:
		std::unique_ptr<ItemCategoryData>	m_ItemCategoryData;
		std::unique_ptr<ItemTypeData>		m_ItemTypeData;
		std::unique_ptr<PresetData>			m_PresetData;
		std::unique_ptr<EnemyData>			m_EnemyData;
		std::unique_ptr<ItemData>			m_ItemData;
		std::unique_ptr<TraderData>			m_TraderData;
		std::unique_ptr<MapData>			m_MapData;
		std::unique_ptr<TaskData>			m_TaskData;
		std::unique_ptr<HideoutData>		m_HideoutData;

		const int							m_MaxCount{ 9 };
		int									m_SetCount{ 0 };
	private:
		DataBase() noexcept;
		~DataBase() noexcept {}
	public:
		const auto		GetMaxCount(void) noexcept { return m_MaxCount; }
		const auto		GetSetCount(void) noexcept { return m_SetCount; }

		const bool		SetDataList(void) noexcept;
		void			LoadList(bool IsPushLog) noexcept;
		void			WhenAfterLoadListCommon(void) noexcept;
		void			WhenAfterLoadList(void) noexcept;
	public:
		auto& GetItemCategoryData(void) noexcept { return this->m_ItemCategoryData; }
		auto& GetItemTypeData(void) noexcept { return this->m_ItemTypeData; }
		auto& GetPresetData(void) noexcept { return this->m_PresetData; }
		auto& GetEnemyData(void) noexcept { return this->m_EnemyData; }
		auto& GetItemData(void) noexcept { return this->m_ItemData; }
		auto& GetTraderData(void) noexcept { return this->m_TraderData; }
		auto& GetMapData(void) noexcept { return this->m_MapData; }
		auto& GetTaskData(void) noexcept { return this->m_TaskData; }
		auto& GetHideoutData(void) noexcept { return this->m_HideoutData; }
	public:
		const bool DataUpdate(EnumDataType EnumDataType_t, int XPos, int YPos, int Xsize, int Ysize, bool IsActive, unsigned int Color, const char* mes,
			const char* queryPath,
			const char* OutputPath,
			const char* dataJsonName,
			int ofsetValue = 20,
			bool fastDo = false
		) noexcept;
	private:
		std::vector<std::pair<ItemID, std::vector<std::string>>>	m_WikiTex;
	public:
		const bool WikiDataUpdate(EnumWikiDataType EnumWikiDataType_t, int XPos, int YPos, int Xsize, int Ysize, bool IsActive, unsigned int Color, const char* mes,
			const char* InputFilePath,
			const char* InputPath,
			const char* OutputPath,
			bool fastDo = false
		) noexcept;
	};

};
