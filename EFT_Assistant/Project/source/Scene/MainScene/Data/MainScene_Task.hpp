#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TaskList : public ListParent<TaskID> {
		class TaskNeedData {
		private:
			IDParents<TraderID>				m_Trader;
			int								m_Level{ -1 };
			int								m_LL{ -1 };
			std::vector<IDParents<TaskID>>	m_Parenttask;
			std::vector<ItemGetData>		m_Item;
			bool							m_kappaRequired{ false };
			bool							m_lightkeeperRequired{ false };
		public:
			const auto& GetTrader() const noexcept { return this->m_Trader.GetID(); }
			const auto& GetLevel() const noexcept { return this->m_Level; }
			const auto& GetLL() const noexcept { return this->m_LL; }
			const auto& GetParenttaskID() const noexcept { return this->m_Parenttask; }
			const auto& GetItem() const noexcept { return this->m_Item; }
			const auto& GetKappaRequired() const noexcept { return this->m_kappaRequired; }
			const auto& GetLightKeeperRequired() const noexcept { return this->m_lightkeeperRequired; }
		public:
			void		Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Trader") {
					m_Trader.SetName(Args[0]);
				}
				else if (LEFT == "NeedTask") {
					this->m_Parenttask.resize(this->m_Parenttask.size() + 1);
					this->m_Parenttask.back().SetName(Args[0]);
				}
				else if (LEFT == "NeedLevel") {
					this->m_Level = std::stoi(Args[0]);
				}
				else if (LEFT == "NeedLL") {
					this->m_LL = std::stoi(Args[0]);
				}
				else if (LEFT == "NeedItem") {
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&this->m_Item, a, "x");
					}
				}
				else if (LEFT == "NeedKappa") {
					this->m_kappaRequired = (Args[0] == "true");
				}
				else if (LEFT == "NeedLightkeeper") {
					this->m_lightkeeperRequired = (Args[0] == "true");
				}
			}
			void		SetAfter() noexcept;
			void		SetNeedTasktoID() noexcept;
		};
		class TaskWorkData {
			class EnemyKill {
				EnemyID								m_EnemyID{ InvalidID };
				MapID								m_MapID{ InvalidID };
				int									m_KillCount{ 0 };
			public:
				const auto& GetEnemyID() const noexcept { return this->m_EnemyID; }
				const auto& GetMapID() const noexcept { return this->m_MapID; }
				const auto& GetKillCount() const noexcept { return this->m_KillCount; }
			public:
				void	Set(MapID mapID, EnemyID enemyID, int count) noexcept {
					this->m_EnemyID = enemyID;
					this->m_MapID = mapID;
					this->m_KillCount = count;
				}
			};
			class TaskPin {
			public:
				VECTOR_ref	m_Point;
				MapID		m_MapID{ InvalidID };
				int			m_MapSel{ 0 };

				std::string	m_MapArg;
			};
		private:
			std::vector<MapID>						m_Map;
			std::vector<EnemyKill>					m_Kill;
			std::vector<ItemGetData>				m_FiR_Item;
			std::vector<ItemGetData>				m_NotFiR_Item;
			std::vector<PresetID>					m_PresetID;
			std::vector<std::string>				m_ElseMsg;
			std::vector<TaskPin>					m_Pin;
		private:
			std::vector<std::string>					m_EnemyKillArgs;

			std::vector<std::string>					m_PresetArgs;
			std::vector<std::string>					m_MapArgs;
		public:
			const auto& GetMap() const noexcept { return this->m_Map; }
			const auto& GetKill() const noexcept { return this->m_Kill; }
			const auto& GetFiR_Item() const noexcept { return this->m_FiR_Item; }
			const auto& GetNotFiR_Item() const noexcept { return this->m_NotFiR_Item; }
			const auto& GetWeaponPreset() const noexcept { return this->m_PresetID; }
			const auto& GetElseMsg() const noexcept { return this->m_ElseMsg; }
			const auto& GetPin() const noexcept { return this->m_Pin; }

			auto&		SetPin() noexcept { return this->m_Pin; }
		public:
			void		Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept;
			void		SetAfter() noexcept;
		};
		class TaskRewardData {
			std::vector<TraderLLData>	m_LLAdd;
			std::vector<ItemGetData>	m_Item;
		public:
			const auto& GetLLAdd() const noexcept { return this->m_LLAdd; }
			const auto& GetItem() const noexcept { return this->m_Item; }
		public:
			void		Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Reward_Rep") {
					SetGetData(&this->m_LLAdd, Args[0], "+");
					SetGetData(&this->m_LLAdd, Args[0], "-");
				}
				else if (LEFT == "Reward_Item") {
					for (auto&a : Args) {
						SetGetData<ItemGetData>(&this->m_Item, a, "x");
					}
				}
			}
			void		SetAfter() noexcept;
		};
	private:
		TaskNeedData				m_TaskNeedData;
		TaskWorkData				m_TaskWorkData;
		TaskRewardData				m_TaskRewardData;
	public:
		const auto&		GetTrader() const noexcept { return this->m_TaskNeedData.GetTrader(); }
		const auto&		GetTaskNeedData() const noexcept { return this->m_TaskNeedData; }
		const auto&		GetTaskWorkData() const noexcept { return this->m_TaskWorkData; }
		const auto&		GetTaskRewardData() const noexcept { return this->m_TaskRewardData; }

		auto&			SetTaskWorkData() noexcept { return this->m_TaskWorkData; }
	public:
		void			SetSub(const std::string& LEFT, const std::vector<std::string>&Args) noexcept override {
			//Need
			m_TaskNeedData.Set(LEFT, Args);
			//Work
			m_TaskWorkData.Set(LEFT, Args);
			//Reward
			m_TaskRewardData.Set(LEFT, Args);
		}
		void			Load_Sub() noexcept override {
			//Need
			m_TaskNeedData.SetAfter();
			//Work
			m_TaskWorkData.SetAfter();
			//Reward
			m_TaskRewardData.SetAfter();
		}
		void			WhenAfterLoad_Sub() noexcept override {}

		void			SetNeedTasktoID() noexcept { m_TaskNeedData.SetNeedTasktoID(); }
		const int		Draw(int xp, int yp, int xsize, int ysize) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* window, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
	};

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

	class TaskJsonData :public JsonDataParent {
	public:
		struct TraderRequirements {
			std::string		m_name;
			std::string		m_requirementType;
			std::string		m_compareMethod;
			int				m_Value{ 0 };
		};
		struct Compare {
			EnumCompareMethod		compareMethod{ EnumCompareMethod::Max };
			int						value{ 0 };
		public:
			const auto IsActive(void) const noexcept { return compareMethod != EnumCompareMethod::Max; }
			void GetJsonData(const nlohmann::json& data) {
				if (data.contains("compareMethod")) {
					if (!data["compareMethod"].is_null()) {
						std::string buf = data["compareMethod"];
						for (int i = 0; i < sizeof(CompareMethodStr) / sizeof(CompareMethodStr[0]); i++) {
							if (buf == CompareMethodStr[i]) {
								compareMethod = (EnumCompareMethod)i;
							}
						}
					}
				}
				if (data.contains("value")) {
					if (!data["value"].is_null()) {
						value = data["value"];
					}
				}
			}
		};
		struct HealthEffect {
			std::vector<std::string>		bodyParts;
			std::vector<std::string>		effects;
			Compare							m_time;
		public:
			void GetJsonData(const nlohmann::json& data) {
				if (data.contains("bodyParts")) {
					if (!data["bodyParts"].is_null()) {
						for (auto& m : data["bodyParts"]) {
							std::string buf = m;
							bodyParts.emplace_back(buf);
						}
					}
				}
				if (data.contains("effects")) {
					if (!data["effects"].is_null()) {
						for (auto& m : data["effects"]) {
							std::string buf = m;
							effects.emplace_back(buf);
						}
					}
				}
				if (data.contains("time")) {
					if (!data["time"].is_null()) {
						m_time.GetJsonData(data["time"]);
					}
				}
			}
		};
		struct TaskObjective {
			EnumTaskObjective							m_TaskObjectiveType{ EnumTaskObjective::Max };

			std::string									m_type;
			std::string									m_description;
			std::vector<MapID>							m_Maps;
			bool										m_optional{ false };
			//TaskObjectiveBuildItem
			ItemGetData									m_Items;

			std::vector<std::string>					m_containsAll;
			std::vector<std::string>					m_containsCategory;
			std::vector<std::pair<std::string, Compare>>m_attributes;
			//TaskObjectiveExperience
			std::vector<HealthEffect>					m_healthEffect;
			//TaskObjectiveExtract
			std::vector<std::string>					m_exitStatus;
			std::string									m_exitName;
			std::vector<std::string>					m_zoneNames;
			//TaskObjectiveItem
			int											count{ 0 };
			bool										foundInRaid{ false };
			int											dogTagLevel{ 0 };
			int											maxDurability{ 0 };
			int											minDurability{ 0 };
			//TaskObjectiveMark
			std::string									markerItem;
			//
			int											playerLevel{ 0 };
			//TaskObjectiveQuestItem
			ItemGetData									m_QuestItem;
			//TaskObjectiveShoot
			EnemyID										target{ InvalidID };
			std::string									shotType;
			std::vector<std::string>					bodyParts;
			std::vector<std::string>					usingWeapon;
			std::vector<std::string>					usingWeaponMods;
			std::vector<std::string>					wearing;
			std::vector<std::string>					notWearing;
			Compare										distance;
			HealthEffect								playerHealthEffect;
			HealthEffect								enemyHealthEffect;
			//
			std::vector<std::pair<std::string, float>>	skillLevel;
			//
			std::string									task;
			std::vector<std::string>					status;
			//
			std::string									trader;
			int											level{ 0 };
			//
			Compare										Compares;
			//
		public:
			void GetJsonData(const nlohmann::json& data);
		};
		struct skillLevelReward {
			std::string							name;
			int									level{ 0 };
			void GetJsonData(const nlohmann::json& data) {
				name = data["name"];
				level = data["level"];
			}
		};
		struct TaskRewards {
			std::vector<TraderGetData>						m_traderStanding;
			std::vector<ItemGetData>						m_Items;
			std::vector<skillLevelReward>					m_skillLevelReward;
			std::vector<std::string>						m_traderUnlock;
		public:
			void GetJsonData(const nlohmann::json& data);
		};
	public:
		TraderID									traderID{ InvalidID };
		MapID										MapID{ InvalidID };
		int											experience{ 0 };
		int											minPlayerLevel{ 0 };
		std::vector<std::pair<std::string, std::string>>	taskRequirements;
		std::vector<TraderRequirements>				traderRequirements;
		std::vector<TaskObjective>					objectives;
		TaskRewards									startRewards;
		TaskRewards									finishRewards;
		std::vector<TaskObjective>					failConditions;
		TaskRewards									failureOutcome;
		bool										restartable;
		std::vector<std::string>					neededKeys;
		bool										kappaRequired{ false };
		bool										lightkeeperRequired{ false };
	public:
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};

	class TaskData : public DataParent<TaskID, TaskList>, public JsonListParent<TaskJsonData> {
	public:
		TaskData() noexcept {
			std::string Path = "data/task/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				SetDirList((Path + RetPath2 + "/").c_str());
			});
			AddTaskUseID();
		}
		~TaskData() noexcept {}
	private:
		std::vector<int>	TraderIDs;
		void			AddTaskUseID() noexcept;
	public:
		void			AfterLoadList() noexcept {
			for (auto& t : this->m_List) {
				t.SetNeedTasktoID();
			}
		}
	public:
		void			InitDatabyJson() noexcept;
		void			UpdateData(int ofset, int size) noexcept {
			for (auto& L : this->m_List) {
				for (int loop = ofset; loop < ofset + size; loop++) {
					if (loop >= (int)GetJsonDataList().size()) { break; }
					auto& jd = GetJsonDataList().at(loop);
					if (L.GetIDstr() == jd->m_id) {
						L.m_CheckJson++;
						jd->OutputData(L.GetFilePath());
						break;
					}
				}
			}
		}
		void			SaveAsNewData2(std::string Path) noexcept;
		void			UpdateAfterbyJson(void) noexcept;
	};
};
