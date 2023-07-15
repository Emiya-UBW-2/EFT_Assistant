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

	enum class EnumWikiDataType {
		ITEMDATA_KEY,
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

		const int							m_MaxCount{ 9 };
		int									m_SetCount{ 0 };
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
			m_SetCount = 0;
		}
		~DataBase() noexcept {}
	public:
		const auto		GetMaxCount(void) noexcept { return m_MaxCount; }
		const auto		GetSetCount(void) noexcept { return m_SetCount; }

		const auto		SetDataList(void) noexcept {
			switch (m_SetCount) {
			case 0:
				m_ItemCategoryData->SetDataList();
				break;
			case 1:
				m_ItemTypeData->SetDataList();
				break;
			case 2:
				m_PresetData->SetDataList();
				break;
			case 3:
				m_EnemyData->SetDataList();
				break;
			case 4:
				m_ItemData->SetDataList();
				break;
			case 5:
				m_TraderData->SetDataList();
				break;
			case 6:
				m_MapData->SetDataList();
				break;
			case 7:
				m_TaskData->SetDataList();
				break;
			case 8:
				m_HideoutData->SetDataList();
				break;
			default:
				return true;
				break;
			}
			m_SetCount++;
			return false;
		}
		void			LoadList(bool IsPushLog) noexcept {
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
			//
			SetUseASyncLoadFlag(TRUE);
			{
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
			}
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
	private:
		std::vector<std::pair<ItemID, std::vector<std::string>>>	m_WikiTex;
	public:
		const auto WikiDataUpdate(EnumWikiDataType EnumWikiDataType_t, int XPos, int YPos, int Xsize, int Ysize, bool IsActive, unsigned int Color, const char* mes,
			const char* InputFilePath,
			const char* InputPath,
			const char* OutputPath,
			bool fastDo = false
		) noexcept {
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, IsActive, Color, mes) || fastDo) {
				auto BaseTime = GetNowHiPerformanceCount();
				std::vector<std::string> InputTex;
				//開始
				{
					//InputFilePathの中身を書き出す
					std::ifstream File(InputFilePath);
					std::string line;
					while (std::getline(File, line)) {
						InputTex.emplace_back(line);
					}
					File.close();
				}
				switch (EnumWikiDataType_t) {
				case FPS_n2::EnumWikiDataType::ITEMDATA_KEY:
					for (auto& L : GetItemData()->GetList()) {
						auto* typePtr = GetItemTypeData()->FindPtr(L.GetTypeID());
						if (typePtr) {
							auto* catPtr = DataBase::Instance()->GetItemCategoryData()->FindPtr(typePtr->GetCategoryID());
							if (catPtr->GetName() == "Keys_Intel") {
								m_WikiTex.emplace_back(std::make_pair(L.GetID(), InputTex));
							}
						}
					}
					break;
				default:
					break;
				}
				//処理
				std::string STR;
				std::string COMMENT;

				int IsUsePoint = -1;
				std::string USEPOINT_BEFORE;
				std::string USEPOINT_AFTER;

				int IsCommentPoint = -1;
				std::string COMMENTPOINT_BEFORE;
				std::string COMMENTPOINT_AFTER;

				int IsFindPoint = -1;
				std::string FINDPOINT_BEFORE;
				std::string FINDPOINT_AFTER;

				switch (EnumWikiDataType_t) {
				case FPS_n2::EnumWikiDataType::ITEMDATA_KEY:
					for (auto& W : m_WikiTex) {
						auto* ptr = GetItemData()->FindPtr(W.first);
						//トレーダー交換
						int BarterCount = 0;
						for (const auto&L : GetTraderData()->SetList()) {
							for (const auto& Ld : L.GetLvData()) {
								for (const auto& cf : Ld.m_ItemBarters) {
									for (const auto& I : cf.m_ItemReward) {
										if (I.GetID() == W.first) {
											BarterCount++;
											break;
										}
									}
								}
							}
						}
						//タスク報酬
						int TaskRewardCount = 0;
						for (const auto& tasks : GetTaskData()->GetList()) {
							for (const auto& I : tasks.GetTaskRewardData().GetItem()) {
								if (I.GetID() == W.first) {
									TaskRewardCount++;
									break;
								}
							}
						}
						//タスク報酬
						int TaskNeedCount = 0;
						for (const auto& tasks : GetTaskData()->GetList()) {
							for (const auto& I : tasks.GetTaskNeedData().GetItem()) {
								if (I.GetID() == W.first) {
									TaskNeedCount++;
									break;
								}
							}
						}
						//
						IsUsePoint = -1;
						IsCommentPoint = -1;
						IsFindPoint = -1;
						for (int loop = 0; loop < W.second.size(); loop++) {
							auto& L = W.second[loop];
							auto commendLine = L.find("//");
							if (commendLine != std::string::npos) {
								STR = L.substr(0, commendLine);
								COMMENT = L.substr(commendLine);
							}
							else {
								STR = L;
								COMMENT = "";
							}
							if (STR != "") {
								//トレーダー交換
								{
									auto BarterTrade = STR.find("<EFTA_BarterTrade_Check>");
									if (BarterTrade != std::string::npos) {
										if (BarterCount == 0) {
											W.second.erase(W.second.begin() + loop);
											loop--;
											continue;
										}
										SubStrs(&STR, "<EFTA_BarterTrade_Check>");
									}
								}
								//トレーダー交換for
								{
									auto BarterTrade = STR.find("<EFTA_BarterTrade_For>");
									if (BarterTrade != std::string::npos) {
										SubStrs(&STR, "<EFTA_BarterTrade_For>");

										W.second.erase(W.second.begin() + loop);

										for (const auto& L2 : GetTraderData()->SetList()) {
											for (const auto& Ld : L2.GetLvData()) {
												int Lv = (int)(&Ld - &L2.GetLvData().front()) + 1;
												for (const auto& cf : Ld.m_ItemBarters) {
													for (const auto& I : cf.m_ItemReward) {
														if (I.GetID() == W.first) {
															std::string STRB = STR;
															ReplaceStrs(&STRB, "EFTA_BarterTrade_TraderName", L2.GetName().c_str());			//EFTA_BarterTrade_TraderName
															ReplaceStrs(&STRB, "EFTA_BarterTrade_TraderLv", std::to_string(Lv).c_str());	//EFTA_BarterTrade_TraderLv
															std::string Need;
															for (const auto& I2 : cf.m_ItemReq) {
																Need += I2.GetName();
																Need += " x";
																Need += std::to_string(I2.GetValue());
																if ((int)(&I2 - &cf.m_ItemReq.front()) != (int)(cf.m_ItemReq.size()) - 1) {
																	Need += " &br;";
																}
															}
															ReplaceStrs(&STRB, "EFTA_BarterTrade_NeedItem", Need.c_str());		//EFTA_BarterTrade_NeedItem
															std::string Reward;
															for (const auto& I2 : cf.m_ItemReward) {
																Reward += I2.GetName();
																Reward += " x";
																Reward += std::to_string(I2.GetValue());
																if ((int)(&I2 - &cf.m_ItemReward.front()) != (int)(cf.m_ItemReward.size()) - 1) {
																	Reward += " &br;";
																}
															}
															ReplaceStrs(&STRB, "EFTA_BarterTrade_RewardItem", Reward.c_str());		//EFTA_BarterTrade_RewardItem
															std::string Unlock;
															for (const auto& I2 : cf.m_TaskReq) {
																Unlock += I2.GetName();
																if ((int)(&I2 - &cf.m_TaskReq.front()) != (int)(cf.m_TaskReq.size()) - 1) {
																	Unlock += " &br;";
																}
															}
															ReplaceStrs(&STRB, "EFTA_BarterTrade_UnlockTask", Unlock.c_str());		//EFTA_BarterTrade_UnlockTask

															W.second.insert(W.second.begin() + loop, STRB);
															break;
														}
													}
												}
											}
										}
										continue;
									}
								}
								//タスク報酬
								{
									auto BarterTrade = STR.find("<EFTA_TaskReward_Check>");
									if (BarterTrade != std::string::npos) {
										if (TaskRewardCount == 0) {
											W.second.erase(W.second.begin() + loop);
											loop--;
											continue;
										}
										SubStrs(&STR, "<EFTA_TaskReward_Check>");
									}
								}
								//タスク報酬for
								{
									auto BarterTrade = STR.find("<EFTA_TaskReward_For>");
									if (BarterTrade != std::string::npos) {
										SubStrs(&STR, "<EFTA_TaskReward_For>");

										W.second.erase(W.second.begin() + loop);

										for (const auto& tasks : GetTaskData()->GetList()) {
											for (const auto& I : tasks.GetTaskRewardData().GetItem()) {
												if (I.GetID() == W.first) {
													std::string STRB = STR;
													auto* trd = DataBase::Instance()->GetTraderData()->FindPtr(tasks.GetTrader());
													ReplaceStrs(&STRB, "EFTA_TaskReward_TraderName", trd->GetName().c_str());			//EFTA_TaskReward_TraderName
													ReplaceStrs(&STRB, "EFTA_TaskReward_TaskName", tasks.GetName().c_str());			//EFTA_TaskReward_TaskName
													W.second.insert(W.second.begin() + loop, STRB);
													break;
												}
											}
										}
										continue;
									}
								}
								//タスク必要
								{
									auto BarterTrade = STR.find("<EFTA_TaskNeed_Check>");
									if (BarterTrade != std::string::npos) {
										if (TaskNeedCount == 0) {
											W.second.erase(W.second.begin() + loop);
											loop--;
											continue;
										}
										SubStrs(&STR, "<EFTA_TaskNeed_Check>");
									}
								}
								//タスク必要for
								{
									auto BarterTrade = STR.find("<EFTA_TaskNeed_For>");
									if (BarterTrade != std::string::npos) {
										SubStrs(&STR, "<EFTA_TaskNeed_For>");

										W.second.erase(W.second.begin() + loop);

										for (const auto& tasks : GetTaskData()->GetList()) {
											for (const auto& I : tasks.GetTaskNeedData().GetItem()) {
												if (I.GetID() == W.first) {
													std::string STRB = STR;
													auto* trd = DataBase::Instance()->GetTraderData()->FindPtr(tasks.GetTrader());
													ReplaceStrs(&STRB, "EFTA_TaskNeed_TraderName", trd->GetName().c_str());			//EFTA_TaskNeed_TraderName
													ReplaceStrs(&STRB, "EFTA_TaskNeed_TaskName", tasks.GetName().c_str());			//EFTA_TaskNeed_TaskName
													W.second.insert(W.second.begin() + loop, STRB);
													break;
												}
											}
										}
										continue;
									}
								}
							}
							//レイド内発見箇所を代入
							{
								auto useline = L.find("<EFTA_FindPoint>");
								if (useline != std::string::npos) {
									SubStrs(&STR, "<EFTA_FindPoint>");
									//ひとつ前の行を取得
									if ((1 < loop) && (loop < W.second.size())) {
										FINDPOINT_BEFORE = FPS_n2::SjistoUTF8(W.second[loop - 1]);
										if (loop < W.second.size() - 1) {
											FINDPOINT_AFTER = FPS_n2::SjistoUTF8(W.second[loop + 1]);
										}
										else {
											FINDPOINT_AFTER = "";
										}
										IsFindPoint = loop;
									}
									W.second.erase(W.second.begin() + loop);
									loop--;
									continue;
								}
							}
							//使用できる箇所を代入
							{
								auto useline = L.find("<EFTA_UsePoint>");
								if (useline != std::string::npos) {
									SubStrs(&STR, "<EFTA_UsePoint>");
									//ひとつ前の行を取得
									if ((1 < loop) && (loop < W.second.size() - 1)) {
										USEPOINT_BEFORE = FPS_n2::SjistoUTF8(W.second[loop - 1]);
										if (loop < W.second.size() - 1) {
											USEPOINT_AFTER = FPS_n2::SjistoUTF8(W.second[loop + 1]);
										}
										else {
											USEPOINT_AFTER = "";
										}
										IsUsePoint = loop;
									}
									W.second.erase(W.second.begin() + loop);
									loop--;
									continue;
								}
							}
							//コメント箇所を代入
							{
								auto useline = L.find("<EFTA_CommentPoint>");
								if (useline != std::string::npos) {
									SubStrs(&STR, "<EFTA_CommentPoint>");
									//ひとつ前の行を取得
									if ((1 < loop) && (loop < W.second.size())) {
										COMMENTPOINT_BEFORE = FPS_n2::SjistoUTF8(W.second[loop - 1]);
										if (loop < W.second.size() - 1) {
											COMMENTPOINT_AFTER = FPS_n2::SjistoUTF8(W.second[loop + 1]);
										}
										else {
											COMMENTPOINT_AFTER = "";
										}
										IsCommentPoint = loop;
									}
									W.second.erase(W.second.begin() + loop);
									loop--;
									continue;
								}
							}
							//代入
							{
								ReplaceStrs(&STR, "EFTA_FullName", ptr->GetName().c_str());							//EFTA_FullName							//日本語フルネーム
								ReplaceStrs(&STR, "EFTA_ShortName", ptr->GetShortName().c_str());					//EFTA_ShortName						//日本語フルネーム
								ReplaceStrs(&STR, "EFTA_JpnFullName", UTF8toSjis(ptr->GetName_Jpn()).c_str());		//EFTA_ShortName						//日本語フルネーム
								ReplaceStrs(&STR, "EFTA_InfoJpn", UTF8toSjis(ptr->GetInformation_Jpn()).c_str());	//EFTA_InfoJpn							//日本語フルネーム
								char tID[64];
								sprintfDx(tID, "%5.3fkg", ptr->Getweight());
								ReplaceStrs(&STR, "EFTA_DefaultWeight", tID);										//EFTA_DefaultWeight					//デフォルト状態の重さ(X.XXXkg)
								ReplaceStrs(&STR, "EFTA_DefaultWidth", std::to_string(ptr->Getwidth()).c_str());	//EFTA_DefaultWidth						//デフォルト状態の横サイズ
								ReplaceStrs(&STR, "EFTA_DefaultHeight", std::to_string(ptr->Getheight()).c_str());	//EFTA_DefaultHeight					//デフォルト状態の縦サイズ
								ReplaceStrs(&STR, "EFTA_Usecount", std::to_string(ptr->GetKeyUses()).c_str());		//EFTA_Usecount							//使用回数
							}
							L = STR + COMMENT;
						}
						//使用できる箇所を代入
						if (
							IsUsePoint != -1 ||
							IsCommentPoint != -1 ||
							IsFindPoint != -1
							) {
							std::string FileStr = InputPath;
							std::string FileName = ptr->GetName();
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

							std::ifstream File(FPS_n2::UTF8toSjis(FileStr + FileName + ".html"));
							//std::ifstream File(FPS_n2::UTF8toSjis(FileStr + FileName + ".txt"));
							std::string line;
							bool startFindPoint = false;
							bool startUsePoint = false;
							bool startCommentPoint = false;

							bool isStart = false;
							while (std::getline(File, line)) {
								//line = FPS_n2::SjistoUTF8(line);
								//line += "\0";

								if (!isStart) {
									auto Start = line.find("<code>");
									if (Start != std::string::npos) {
										isStart = true;
										line = line.substr(Start + strlenDx("<code>"));
									}
								}
								else {
									auto Start = line.find("</code>");
									if (Start != std::string::npos) {
										isStart = false;
									}
								}
								if (isStart) {

									if (IsFindPoint != -1) {
										bool isInsFindPoint = true;
										if (!startFindPoint) {
											//次から終わりまでをW.secondに加える
											if (line.find(FINDPOINT_BEFORE) != std::string::npos) {
												startFindPoint = true;
												isInsFindPoint = false;
											}
										}
										else {
											//次から終わりまでをW.secondに加える
											if (line.find("*") == 0) {
												startFindPoint = false;
											}
											else if ((FINDPOINT_AFTER != "") && (line.find(FINDPOINT_AFTER) != std::string::npos)) {
												startFindPoint = false;
											}
										}
										if (startFindPoint && isInsFindPoint) {
											W.second.insert(W.second.begin() + IsFindPoint, FPS_n2::UTF8toSjis(line));
											IsFindPoint++;
											if (IsUsePoint != -1) {
												IsUsePoint++;
											}
											if (IsCommentPoint != -1) {
												IsCommentPoint++;
											}
										}
									}
									if (IsUsePoint != -1) {
										bool isInsUsePoint = true;
										if (!startUsePoint) {
											//次から終わりまでをW.secondに加える
											if (line.find(USEPOINT_BEFORE) != std::string::npos) {
												startUsePoint = true;
												isInsUsePoint = false;
											}
										}
										else {
											//次から終わりまでをW.secondに加える
											if (line.find("*") == 0) {
												startUsePoint = false;
											}
											else if ((USEPOINT_AFTER != "") && (line.find(USEPOINT_AFTER) != std::string::npos)) {
												startUsePoint = false;
											}
										}
										if (startUsePoint && isInsUsePoint) {
											W.second.insert(W.second.begin() + IsUsePoint, FPS_n2::UTF8toSjis(line));
											IsUsePoint++;
											if (IsCommentPoint != -1) {
												IsCommentPoint++;
											}
										}
									}
									if (IsCommentPoint != -1) {
										auto line_buf = line;
										bool isInsCommentPoint = true;
										if (!startCommentPoint) {
											//次から終わりまでをW.secondに加える
											if (line_buf.find(COMMENTPOINT_BEFORE) != std::string::npos) {
												startCommentPoint = true;
												isInsCommentPoint = false;
											}
										}
										else {
											//次から終わりまでをW.secondに加える
											if (line_buf.find("*") == 0) {
												startCommentPoint = false;
											}
											else if ((COMMENTPOINT_AFTER != "") && (line_buf.find(COMMENTPOINT_AFTER) != std::string::npos)) {
												startCommentPoint = false;
											}
										}
										if (startCommentPoint && isInsCommentPoint) {
											W.second.insert(W.second.begin() + IsCommentPoint, FPS_n2::UTF8toSjis(line_buf));
											IsCommentPoint++;
										}
									}
								}
							}
							File.close();
						}
						//int a = 0;
					}
					break;
				default:
					break;
				}
				//後始末
				switch (EnumWikiDataType_t) {
				case FPS_n2::EnumWikiDataType::ITEMDATA_KEY:
					for (auto& W : m_WikiTex) {
						auto* ptr = GetItemData()->FindPtr(W.first);
						std::string FileStr = OutputPath;
						std::string FileName = ptr->GetName();
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

						std::ofstream outputfile(FileStr + FileName + ".txt");
						for (auto& L : W.second) {
							outputfile << L + "\n";
						}
						outputfile.close();
					}
					break;
				default:
					break;
				}
				{
					std::string ErrMes = "Update Wiki Time:" + std::to_string((float)((GetNowHiPerformanceCount() - BaseTime) / 1000) / 1000.f) + " s";
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
				return true;
			}
			return false;
		}
	};

};
