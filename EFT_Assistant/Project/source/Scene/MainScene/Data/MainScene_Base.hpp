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
	private:
		DataBase() noexcept {
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
		~DataBase() noexcept {}
	public:
		void			SetDataList(void) noexcept {
			auto BaseTime = GetNowHiPerformanceCount();
			m_ItemCategoryData->SetDataList();
			m_ItemTypeData->SetDataList();
			m_PresetData->SetDataList();
			m_EnemyData->SetDataList();
			m_ItemData->SetDataList();;
			m_TraderData->SetDataList();
			m_MapData->SetDataList();
			m_TaskData->SetDataList();
			m_HideoutData->SetDataList();
			{
				std::string ErrMes = "Load Data Time:" + std::to_string((float)((GetNowHiPerformanceCount() - BaseTime) / 1000) / 1000.f) + " s";
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		void			WaitDataList(void) noexcept {
			auto BaseTime = GetNowHiPerformanceCount();
			m_ItemCategoryData->WaitDataList();
			m_ItemTypeData->WaitDataList();
			m_PresetData->WaitDataList();
			m_EnemyData->WaitDataList();
			m_ItemData->WaitDataList();;
			m_TraderData->WaitDataList();
			m_MapData->WaitDataList();
			m_TaskData->WaitDataList();
			m_HideoutData->WaitDataList();
			//
			m_TaskData->AddTaskUseID();
			{
				std::string ErrMes = "Wait Load Time:" + std::to_string((float)((GetNowHiPerformanceCount() - BaseTime) / 1000) / 1000.f) + " s";
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		void			LoadList(bool IsPushLog) noexcept {
			auto BaseTime = GetNowHiPerformanceCount();
			SetUseASyncLoadFlag(TRUE);

			m_ItemCategoryData->LoadList(false);
			m_ItemTypeData->LoadList(false);
			m_PresetData->LoadList(false);
			m_EnemyData->LoadList(false);
			m_ItemData->LoadList(IsPushLog);
			m_ItemData->AfterLoadList();
			m_TraderData->LoadList(false);
			m_MapData->LoadList(false);
			m_TaskData->LoadList(false);
			m_TaskData->AfterLoadList();
			m_HideoutData->LoadList(IsPushLog);

			SetUseASyncLoadFlag(FALSE);
			{
				std::string ErrMes = "Load Image:" + std::to_string((float)((GetNowHiPerformanceCount() - BaseTime) / 1000) / 1000.f) + " s";
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
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
		const auto DataUpdate(EnumDataType EnumDataType_t, int XPos, int YPos, int Xsize, int Ysize, bool IsActive, unsigned int Color, const char* mes,
			const char* queryPath,
			const char* OutputPath,
			const char* dataJsonName,
			int ofsetValue = 20,
			bool fastDo = false
		) noexcept {
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, IsActive, Color, mes) || fastDo) {
				auto BaseTime = GetNowHiPerformanceCount();
				int count = 0;
				switch (EnumDataType_t) {
				case FPS_n2::EnumDataType::ITEMDATA:
					this->m_ItemData->ResetDataJson();
					this->m_ItemData->InitDatabyJson();
					break;
				case FPS_n2::EnumDataType::ITEMDATAJP:
					this->m_ItemData->InitJpDatabyJson();
					break;
				case FPS_n2::EnumDataType::TRADERDATA:
					this->m_TraderData->ResetDataJson();
					this->m_TraderData->InitDatabyJson();
					break;
				case FPS_n2::EnumDataType::TASKDATA:
					this->m_TaskData->ResetDataJson();
					this->m_TaskData->InitDatabyJson();
					break;
				case FPS_n2::EnumDataType::HIDEOUTDATA:
					this->m_HideoutData->ResetDataJson();
					this->m_HideoutData->InitDatabyJson();
					break;
				default:
					break;
				}
				while (true) {
					std::string	strResult;
					if (CommonDataRequest(queryPath, ofsetValue * count, ofsetValue, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						switch (EnumDataType_t) {
						case FPS_n2::EnumDataType::ITEMDATA:
							this->m_ItemData->GetDataJson(data["data"][dataJsonName]);
							this->m_ItemData->UpdateData(ofsetValue * count, ofsetValue, this->m_ItemData->SetList());
							this->m_ItemData->SaveAsNewData2(OutputPath);
							break;
						case FPS_n2::EnumDataType::ITEMDATAJP:
							this->m_ItemData->GetJpDataJson(data["data"][dataJsonName]);
							this->m_ItemData->UpdateJpData(ofsetValue * count, ofsetValue, this->m_ItemData->SetList());
							this->m_ItemData->SaveAsNewJpData(OutputPath);
							break;
						case FPS_n2::EnumDataType::TRADERDATA:
							this->m_TraderData->GetDataJson(data["data"][dataJsonName]);
							this->m_TraderData->UpdateData(ofsetValue * count, ofsetValue, this->m_TraderData->SetList());
							this->m_TraderData->SaveAsNewData(OutputPath);
							break;
						case FPS_n2::EnumDataType::TASKDATA:
							this->m_TaskData->GetDataJson(data["data"][dataJsonName]);
							this->m_TaskData->UpdateData(ofsetValue * count, ofsetValue, this->m_TaskData->SetList());
							this->m_TaskData->SaveAsNewData2(OutputPath);
							break;
						case FPS_n2::EnumDataType::HIDEOUTDATA:
							this->m_HideoutData->GetDataJson(data["data"][dataJsonName]);
							this->m_HideoutData->UpdateData(ofsetValue * count, ofsetValue, this->m_HideoutData->SetList());
							this->m_HideoutData->SaveAsNewData(OutputPath);
							break;
						default:
							break;
						}

						if (data["data"][dataJsonName].size() != ofsetValue) {
							break;
						}
					}
					else {
						break;
					}
					count++;
				}
				switch (EnumDataType_t) {
				case FPS_n2::EnumDataType::ITEMDATA:
					this->m_ItemData->WaitToAllClear();
					this->m_ItemData->CheckThroughJson();
					this->m_ItemData->UpdateAfterbyJson();
					break;
				case FPS_n2::EnumDataType::ITEMDATAJP:
					this->m_ItemData->WaitToAllClearJp();
					break;
				case FPS_n2::EnumDataType::TRADERDATA:
					this->m_TraderData->WaitToAllClear();
					this->m_TraderData->CheckThroughJson();
					this->m_TraderData->UpdateAfterbyJson();
					break;
				case FPS_n2::EnumDataType::TASKDATA:
					this->m_TaskData->WaitToAllClear();
					this->m_TaskData->CheckThroughJson();
					this->m_TaskData->UpdateAfterbyJson();
					break;
				case FPS_n2::EnumDataType::HIDEOUTDATA:
					this->m_HideoutData->WaitToAllClear();
					this->m_HideoutData->CheckThroughJson();
					this->m_HideoutData->UpdateAfterbyJson();
					break;
				default:
					break;
				}
				{
					std::string ErrMes = "Update Json Time:" + std::to_string((float)((GetNowHiPerformanceCount() - BaseTime) / 1000) / 1000.f) + " s";
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
				return true;
			}
			return false;
		}
	};

};
