#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TaskList : public ListParent<TaskID> {
		class TaskNeedData {
		private:
			TraderID								m_Trader{ InvalidID };
			int										m_Level{ -1 };
			int										m_LL{ -1 };
			std::vector<IDParents<TaskID, TaskList>>m_Parenttask;
			std::vector<ItemGetData>				m_Item;
			bool									m_kappaRequired{ false };
			bool									m_lightkeeperRequired{ false };

			std::string									m_TraderArg;
			std::vector<std::string>					m_ItemArgs;
		public:
			const auto& GetTrader() const noexcept { return m_Trader; }
			const auto& GetLevel() const noexcept { return m_Level; }
			const auto& GetLL() const noexcept { return m_LL; }
			const auto& GetParenttaskID() const noexcept { return this->m_Parenttask; }
			const auto& GetItem() const noexcept { return m_Item; }
			const auto& GetKappaRequired() const noexcept { return m_kappaRequired; }
			const auto& GetLightKeeperRequired() const noexcept { return m_lightkeeperRequired; }
		public:
			void Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Trader") {
					m_TraderArg = Args[0];
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
						m_ItemArgs.emplace_back(a);
					}
				}
				else if (LEFT == "NeedKappa") {
					this->m_kappaRequired = (Args[0] == "true");
				}
				else if (LEFT == "NeedLightkeeper") {
					this->m_lightkeeperRequired = (Args[0] == "true");
				}
			}
			void		SetAfter() noexcept {
				m_Trader = TraderData::Instance()->FindID(m_TraderArg);
				for (auto& a : m_ItemArgs) {
					SetGetData<ItemGetData, ItemData>(&this->m_Item, a);
				}
			}
			void		SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
				for (auto& t : m_Parenttask) {
					t.CheckID(taskList);
				}
			}
		};
		class TaskWorkData {
			class EnemyKill {
				EnemyID								m_EnemyID{ InvalidID };
				MapID								m_MapID{ InvalidID };
				int									m_KillCount{ 0 };
			public:
				const auto& GetEnemyID() const noexcept { return m_EnemyID; }
				const auto& GetMapID() const noexcept { return m_MapID; }
				const auto& GetKillCount() const noexcept { return m_KillCount; }
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

			std::vector<std::string>					m_FiR_ItemArgs;
			std::vector<std::string>					m_NotFiR_ItemArgs;

			std::vector<std::string>					m_PresetArgs;
			std::vector<std::string>					m_MapArgs;
		public:
			const auto& GetMap() const noexcept { return m_Map; }
			const auto& GetKill() const noexcept { return m_Kill; }
			const auto& GetFiR_Item() const noexcept { return m_FiR_Item; }
			const auto& GetNotFiR_Item() const noexcept { return m_NotFiR_Item; }
			const auto& GetWeaponPreset() const noexcept { return m_PresetID; }
			const auto& GetElseMsg() const noexcept { return m_ElseMsg; }
			const auto& GetPin() const noexcept { return m_Pin; }

			auto& SetPin() noexcept { return m_Pin; }
		public:
			void Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Task_Map") {//ロケーション追加
					for (auto&a : Args) {
						m_MapArgs.emplace_back(a);
					}
				}
				else if (LEFT == "Task_Kill") {
					for (auto&a : Args) {
						m_EnemyKillArgs.emplace_back(a);
					}
				}
				else if (LEFT == "Task_FiR_HandOver") {
					for (auto&a : Args) {
						m_FiR_ItemArgs.emplace_back(a);
					}
				}
				else if (LEFT == "Task_NotFiR_HandOver") {
					for (auto&a : Args) {
						m_NotFiR_ItemArgs.emplace_back(a);
					}
				}
				else if (LEFT == "Task_WeaponPreset_HandOver") {
					for (auto&a : Args) {
						m_PresetArgs.emplace_back(a);
					}
				}
				else if (LEFT == "Task_Else") {//特殊　メッセージ
					this->m_ElseMsg.emplace_back(Args[0]);
				}
				else if (LEFT == "Task_PinPoint") {//特殊　メッセージ
					this->m_Pin.resize(this->m_Pin.size()+1);
					this->m_Pin.back().m_Point.Set(std::stof(Args[0]), std::stof(Args[1]), std::stof(Args[2]));
				}
				else if (LEFT == "Task_PinMap") {//特殊　メッセージ
					this->m_Pin.back().m_MapArg = Args[0];
					this->m_Pin.back().m_MapSel = std::atoi(Args[1].c_str());
				}
			}
			void		SetAfter() noexcept {
				auto SetKill = [&](const std::string& mes) {
					auto L = mes.rfind("x");
					if (L != std::string::npos) {
						EnemyKill tmp;
						auto MP = mes.rfind("-");
						if (MP != std::string::npos) {
							std::string Name = mes.substr(MP + 1, L - (MP + 1));
							auto kakko = Name.find("{");
							if (kakko != std::string::npos) {
								Name = Name.substr(0, kakko);
							}

							tmp.Set(
								MapData::Instance()->FindID(mes.substr(0, MP).c_str()),
								EnemyData::Instance()->FindID(Name.c_str()),
								std::stoi(mes.substr(L + 1)));
						}
						else {
							std::string Name = mes.substr(0, L);
							auto kakko = Name.find("{");
							if (kakko != std::string::npos) {
								Name = Name.substr(0, kakko);
							}

							tmp.Set(
								InvalidID,
								EnemyData::Instance()->FindID(Name.c_str()),
								std::stoi(mes.substr(L + 1)));
						}
						this->m_Kill.emplace_back(tmp);
					}
					else {
						//int a = 0;
					}
				};
				for (auto& a : m_EnemyKillArgs) {
					SetKill(a);
				}

				for (auto& a : m_FiR_ItemArgs) {
					SetGetData<ItemGetData, ItemData>(&this->m_FiR_Item, a);
				}
				for (auto& a : m_NotFiR_ItemArgs) {
					SetGetData<ItemGetData, ItemData>(&this->m_NotFiR_Item, a);
				}


				for (auto& a : m_PresetArgs) {
					m_PresetID.emplace_back(PresetData::Instance()->FindID(a));
				}
				for (auto& a : this->m_Pin) {
					a.m_MapID = MapData::Instance()->FindID(a.m_MapArg);
				}
				for (auto& a : m_MapArgs) {
					this->m_Map.emplace_back(MapData::Instance()->FindID(a));
				}
			}
		};
		class TaskRewardData {
			std::vector<TraderLLData>	m_LLAdd;
			std::vector<ItemGetData>	m_Item;

			std::vector<std::pair<std::string, int>>		m_LLAddArgs;
			std::vector<std::string>					m_ItemArgs;
		public:
			const auto& GetLLAdd() const noexcept { return m_LLAdd; }
			const auto& GetItem() const noexcept { return m_Item; }
		public:
			void Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Reward_Rep") {
					auto plus = Args[0].find("+");
					if (plus != std::string::npos) {
						m_LLAddArgs.emplace_back(std::make_pair(Args[0].substr(0, plus), (int)(std::stof(Args[0].substr(plus + 1)) * 100.f)));
					}
					auto minus = Args[0].find("-");
					if (minus != std::string::npos) {
						m_LLAddArgs.emplace_back(std::make_pair(Args[0].substr(0, minus), (int)(std::stof(Args[0].substr(minus + 1)) * 100.f)));
					}
				}
				else if (LEFT == "Reward_Item") {
					for (auto&a : Args) {
						m_ItemArgs.emplace_back(a);
					}
				}
			}
			void		SetAfter() noexcept {
				for (auto& L : m_LLAddArgs) {
					TraderLLData tmp;
					tmp.Set(TraderData::Instance()->FindID(L.first.c_str()), L.second);
					this->m_LLAdd.emplace_back(tmp);
				}
				for (auto& a : m_ItemArgs) {
					SetGetData<ItemGetData, ItemData>(&this->m_Item, a);
				}
			}
		};
	private:
		TaskNeedData				m_TaskNeedData;
		TaskWorkData				m_TaskWorkData;
		TaskRewardData				m_TaskRewardData;
	public:
		int							m_CheckJson{ 0 };
	public:
		const auto&		GetTrader() const noexcept { return m_TaskNeedData.GetTrader(); }
		const auto&		GetTaskNeedData() const noexcept { return m_TaskNeedData; }
		const auto&		GetTaskWorkData() const noexcept { return m_TaskWorkData; }
		const auto&		GetTaskRewardData() const noexcept { return m_TaskRewardData; }

		auto&			SetTaskWorkData() noexcept { return m_TaskWorkData; }
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

		void			SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
			m_TaskNeedData.SetNeedTasktoID(taskList);
		}
		const int		Draw(int xp, int yp, int xsize, int ysize) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* window, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* InterParts = InterruptParts::Instance();
			int xofs = 0;
			int yofs = LineHeight;
			int sizy = LineHeight * 7 / 10;
			//必要
			{
				auto* ptr = TraderData::Instance()->FindPtr(GetTrader());
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "トレーダー:%s Lv %d", ptr->GetName().c_str(), std::max(m_TaskNeedData.GetLL(), 1))); yofs += sizy;
			}
			if (m_TaskNeedData.GetLevel() >= 1) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "必要レベル:%d", m_TaskNeedData.GetLevel())); yofs += sizy;
			}
			if (m_TaskNeedData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "必要アイテム:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskNeedData.GetItem()) {
					auto ID = LL.GetID();
					auto* ptr = ItemData::Instance()->FindPtr(ID);
					int total_size = y_r(48);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), false, true, false) + y_r(30));
					yofs += total_size;
				}
			}
			//タスク内容
			if (m_TaskWorkData.GetMap().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "マップ指定:")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetMap()) {
					auto* ptr = MapData::Instance()->FindPtr(LL);
					xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STRX_LEFT, ptr->GetColors(0), Black, "%s", ptr->GetName().c_str()) + y_r(30)); yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetKill().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "敵をキル:")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetKill()) {
					auto* eny = EnemyData::Instance()->FindPtr(LL.GetEnemyID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STRX_LEFT, eny->GetColors(0), Black, "%s x%2d", eny->GetName().c_str(), LL.GetKillCount()) + y_r(30));
					if (LL.GetMapID() != InvalidID) {
						auto* ptr = MapData::Instance()->FindPtr(LL.GetMapID());
						xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(250), yp + yofs, xp + y_r(250), yp + sizy + yofs, LineHeight * 8 / 10, STRX_LEFT, ptr->GetColors(0), Black, " in %s", ptr->GetName().c_str()));
					}
					yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "Firアイテムの納品:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskWorkData.GetFiR_Item()) {
					auto ID = LL.GetID();
					auto* ptr = ItemData::Instance()->FindPtr(ID);
					int total_size = y_r(48);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), true, true, false) + y_r(30));
					yofs += total_size;
				}
			}
			if (m_TaskWorkData.GetNotFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "Firでなくてよいアイテムの納品:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskWorkData.GetNotFiR_Item()) {
					auto ID = LL.GetID();
					auto* ptr = ItemData::Instance()->FindPtr(ID);
					int total_size = y_r(48);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), false, true, false) + y_r(30));
					yofs += total_size;
				}
			}

			if (m_TaskWorkData.GetWeaponPreset().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "カスタム品の納品:")); yofs += sizy;
				yofs += LineHeight;
				//
				for (const auto& LL : m_TaskWorkData.GetWeaponPreset()) {
					auto* ptr = PresetData::Instance()->FindPtr(LL);
					int total_size = LineHeight;
					int XSize = WindowSystem::GetMsgLen(total_size, ptr->GetName());

					if (WindowSystem::ClickCheckBox(xp + y_r(30), yp + yofs, xp + y_r(30) + XSize, yp + yofs + total_size, false, true, Gray10, ptr->GetName())) {
						InterParts->GotoNext(BGSelect::Custom);
						InterParts->SetInitParam(0, (int)ptr->GetBase()->GetID());//武器ID
						InterParts->SetInitParam(1, (int)ptr->GetID());//プリセットID
					}
					xofs = std::max(xofs, XSize + y_r(30));
					yofs += total_size;
				}
			}
			if (m_TaskWorkData.GetElseMsg().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "メモ:")); yofs += sizy;
				for (auto& m : m_TaskWorkData.GetElseMsg()) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, m.c_str())); yofs += sizy;
				}
			}
			//
			if ((m_TaskRewardData.GetLLAdd().size() > 0) || (m_TaskRewardData.GetItem().size() > 0)) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "報酬アイテム:")); yofs += sizy;
				yofs += LineHeight;
			}
			if (m_TaskRewardData.GetLLAdd().size() > 0) {
				for (auto& LL : m_TaskRewardData.GetLLAdd()) {
					auto* trader2 = TraderData::Instance()->FindPtr(LL.GetID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STRX_LEFT, trader2->GetColors(0), Black, "%s %s%4.2f",
						trader2->GetName().c_str(),
						(LL.GetValue() >= 0.f) ? "+" : "-",
						(float)(LL.GetValue()) / 100.f)); yofs += sizy;
				}
			}
			if (m_TaskRewardData.GetItem().size() > 0) {
				for (const auto& LL : m_TaskRewardData.GetItem()) {
					auto ID = LL.GetID();
					auto* ptr = ItemData::Instance()->FindPtr(ID);
					int total_size = y_r(48);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), true, true, false) + y_r(30));
					yofs += total_size;
				}
				//yofs += sizy;
			}
			//
			if (xs) {
				*xs = std::max(*xs, xofs + LineHeight / 10);
			}
			if (ys) {
				*ys = std::max(*ys, yofs + LineHeight / 10);
			}
		}
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
		struct TaskObjective
		{
			EnumTaskObjective							TaskObjectiveType{ EnumTaskObjective::Max };

			std::string									type;
			std::string									description;
			std::vector<MapID>							Maps;
			bool										optional{ false };
			//TaskObjectiveBuildItem
			ItemID										Items{ InvalidID };
			std::vector<std::string>					containsAll;
			std::vector<std::string>					containsCategory;
			std::vector<std::pair<std::string, Compare>>attributes;
			//TaskObjectiveExperience
			std::vector<HealthEffect>					healthEffect;
			//TaskObjectiveExtract
			std::vector<std::string>					exitStatus;
			std::string									exitName;
			std::vector<std::string>					zoneNames;
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
			std::string									questItem;
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
			void GetJsonData(const nlohmann::json& data) {
				if (data.contains("__typename")) {
					if (!data["__typename"].is_null()) {
						std::string buf = data["__typename"];
						for (int i = 0; i < sizeof(TypesStr) / sizeof(TypesStr[0]); i++) {
							if (buf == TypesStr[i]) {
								TaskObjectiveType = (EnumTaskObjective)i;
							}
						}
					}
				}
				if (data.contains("type")) {
					if (!data["type"].is_null()) {
						type = data["type"];
					}
				}
				if (data.contains("description")) {
					if (!data["description"].is_null()) {
						description = data["description"];
					}
				}
				if (data.contains("maps")) {
					if (!data["maps"].is_null()) {
						for (const auto&m : data["maps"]) {
							std::string buf = m["name"];
							Maps.emplace_back(MapData::Instance()->FindID(buf.c_str()));
						}
					}
				}
				if (data.contains("optional")) {
					if (!data["optional"].is_null()) {
						optional = data["optional"];
					}
				}
				if (data.contains("item")) {
					if (!data["item"].is_null()) {
						std::string buf = data["item"]["name"];
						Items = ItemData::Instance()->FindID(buf.c_str());
					}
				}
				if (data.contains("containsAll")) {
					if (!data["containsAll"].is_null()) {
						for (const auto&m : data["containsAll"]) {
							std::string buf1 = m["name"];
							containsAll.emplace_back(buf1);
						}
					}
				}
				if (data.contains("containsCategory")) {
					if (!data["containsCategory"].is_null()) {
						for (const auto&m : data["containsCategory"]) {
							std::string buf1 = m["name"];
							containsCategory.emplace_back(buf1);
						}
					}
				}
				if (data.contains("attributes")) {
					if (!data["attributes"].is_null()) {
						for (const auto&m : data["attributes"]) {
							std::string buf1 = m["name"];
							Compare buf2; buf2.GetJsonData(m["requirement"]);
							attributes.emplace_back(std::make_pair(buf1, buf2));
						}
					}
				}
				if (data.contains("healthEffect")) {
					if (!data["healthEffect"].is_null()) {
						for (const auto&m : data["healthEffect"]) {
							HealthEffect buf1; buf1.GetJsonData(m);
							healthEffect.emplace_back(buf1);
						}
					}
				}
				if (data.contains("exitStatus")) {
					if (!data["exitStatus"].is_null()) {
						for (const auto&m : data["exitStatus"]) {
							exitStatus.emplace_back(m);
						}
					}
				}
				if (data.contains("exitName")) {
					if (!data["exitName"].is_null()) {
						exitName = data["exitName"];
					}
				}
				if (data.contains("zoneNames")) {
					if (!data["zoneNames"].is_null()) {
						for (const auto&m : data["zoneNames"]) {
							std::string buf1 = m;
							zoneNames.emplace_back(buf1);
						}
					}
				}
				if (data.contains("count")) {
					if (!data["count"].is_null()) {
						count = data["count"];
					}
				}
				if (data.contains("foundInRaid")) {
					if (!data["foundInRaid"].is_null()) {
						foundInRaid = data["foundInRaid"];
					}
				}
				if (data.contains("dogTagLevel")) {
					if (!data["dogTagLevel"].is_null()) {
						dogTagLevel = data["dogTagLevel"];
					}
				}
				if (data.contains("maxDurability")) {
					if (!data["maxDurability"].is_null()) {
						maxDurability = data["maxDurability"];
					}
				}
				if (data.contains("minDurability")) {
					if (!data["minDurability"].is_null()) {
						minDurability = data["minDurability"];
					}
				}

				if (data.contains("markerItem")) {
					if (!data["markerItem"].is_null()) {
						markerItem = data["markerItem"]["name"];
					}
				}

				if (data.contains("questItem")) {
					if (!data["questItem"].is_null()) {
						questItem = data["questItem"]["name"];
					}
				}

				if (data.contains("target")) {
					if (!data["target"].is_null()) {
						std::string buf = data["target"];
						target = EnemyData::Instance()->FindID(buf.c_str());
					}
				}
				if (data.contains("shotType")) {
					if (!data["shotType"].is_null()) {
						shotType = data["shotType"];
					}
				}
				if (data.contains("bodyParts")) {
					if (!data["bodyParts"].is_null()) {
						for (const auto&m : data["bodyParts"]) {
							bodyParts.emplace_back(m);
						}
					}
				}
				if (data.contains("usingWeapon")) {
					if (!data["usingWeapon"].is_null()) {
						for (const auto&m : data["usingWeapon"]) {
							std::string buf1 = m["name"];
							usingWeapon.emplace_back(buf1);
						}
					}
				}
				if (data.contains("usingWeaponMods")) {
					if (!data["usingWeaponMods"].is_null()) {
						for (const auto&m : data["usingWeaponMods"]) {
							for (const auto&m2 : m) {
								std::string buf1 = m2["name"];
								usingWeaponMods.emplace_back(buf1);
							}
						}
					}
				}
				if (data.contains("wearing")) {
					if (!data["wearing"].is_null()) {
						for (const auto&m : data["wearing"]) {
							for (const auto&m2 : m) {
								std::string buf1 = m2["name"];
								wearing.emplace_back(buf1);
							}
						}
					}
				}
				if (data.contains("notWearing")) {
					if (!data["notWearing"].is_null()) {
						for (const auto&m : data["notWearing"]) {
							std::string buf1 = m["name"];
							notWearing.emplace_back(buf1);
						}
					}
				}

				if (data.contains("distance")) {
					if (!data["distance"].is_null()) {
						distance.GetJsonData(data["distance"]);
					}
				}


				if (data.contains("playerHealthEffect")) {
					if (!data["playerHealthEffect"].is_null()) {
						playerHealthEffect.GetJsonData(data["playerHealthEffect"]);
					}
				}
				if (data.contains("enemyHealthEffect")) {
					if (!data["enemyHealthEffect"].is_null()) {
						enemyHealthEffect.GetJsonData(data["enemyHealthEffect"]);
					}
				}

				if (data.contains("skillLevel")) {
					if (!data["skillLevel"].is_null()) {
						std::string buf1 = data["skillLevel"]["name"];
						float buf2 = data["skillLevel"]["level"];
						skillLevel.emplace_back(std::make_pair(buf1, buf2));
					}
				}

				if (data.contains("task")) {
					if (!data["task"].is_null()) {
						task = data["task"]["name"];
					}
				}
				if (data.contains("status")) {
					if (!data["status"].is_null()) {
						for (const auto&m : data["status"]) {
							std::string buf1 = m;
							status.emplace_back(buf1);
						}
					}
				}

				if (data.contains("trader")) {
					if (!data["trader"].is_null()) {
						trader = data["trader"]["name"];
					}
				}
				if (data.contains("level")) {
					if (!data["level"].is_null()) {
						level = data["level"];
					}
				}
				Compares.GetJsonData(data);
			}
		};
		struct traderStanding {
			std::string							trader;
			float								standing;
			void GetJsonData(const nlohmann::json& data) {
				trader = data["trader"]["name"];
				standing = data["standing"];
			}
		};
		struct skillLevelReward {
			std::string							name;
			int									level{ 0 };
			void GetJsonData(const nlohmann::json& data) {
				name = data["name"];
				level = data["level"];
			}
		};
		struct TaskRewardItems {
			ItemID ID{ InvalidID };
			std::string Name;
			int count{ 0 };
		};
		struct TaskRewards {
			std::vector<traderStanding>						m_traderStanding;
			std::vector<TaskRewardItems>					Items;
			std::vector<skillLevelReward>					m_skillLevelReward;
			std::vector<std::string>						traderUnlock;
		public:
			void GetJsonData(const nlohmann::json& data) {
				if (data.contains("traderStanding")) {
					if (!data["traderStanding"].is_null()) {
						for (const auto&m : data["traderStanding"]) {
							traderStanding buf1; buf1.GetJsonData(m);
							m_traderStanding.emplace_back(buf1);
						}
					}
				}
				if (data.contains("items")) {
					if (!data["items"].is_null()) {
						for (const auto&m : data["items"]) {
							TaskRewardItems buf;

							buf.Name = m["item"]["name"];
							buf.ID = ItemData::Instance()->FindID(buf.Name.c_str());
							buf.count = m["count"];
							Items.emplace_back(buf);
						}
					}
				}
				if (data.contains("skillLevelReward")) {
					if (!data["skillLevelReward"].is_null()) {
						for (const auto&m : data["skillLevelReward"]) {
							skillLevelReward buf1; buf1.GetJsonData(m);
							m_skillLevelReward.emplace_back(buf1);
						}
					}
				}
				if (data.contains("traderUnlock")) {
					if (!data["traderUnlock"].is_null()) {
						for (const auto&m : data["traderUnlock"]) {
							std::string buf1 = m["name"];
							traderUnlock.emplace_back(buf1);
						}
					}
				}
			}
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

	class TaskData : public SingletonBase<TaskData>, public DataParent<TaskID, TaskList>, public JsonListParent<TaskJsonData> {
	private:
		friend class SingletonBase<TaskData>;
	private:
		TaskData() noexcept {
			std::string Path = "data/task/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				SetDirList((Path + RetPath2 + "/").c_str());
			});
			for (auto& i : ItemData::Instance()->SetList()) {
				i.ResetTaskUseID();
				for (const auto& t : m_List) {
					for (const auto& n : t.GetTaskNeedData().GetItem()) {
						if (i.GetID() == n.GetID()) {
							i.AddTaskUseID(t.GetID());
						}
					}
					for (const auto& w : t.GetTaskWorkData().GetFiR_Item()) {
						if (i.GetID() == w.GetID()) {
							i.AddTaskUseID(t.GetID());
						}
					}
					for (const auto& w : t.GetTaskWorkData().GetNotFiR_Item()) {
						if (i.GetID() == w.GetID()) {
							i.AddTaskUseID(t.GetID());
						}
					}
				}
			}
		}
		~TaskData() noexcept {}
	private:
		
	private:
		std::vector<int> TraderIDs;
	public:
		void			SetNeedTasktoID() noexcept {
			for (auto& t : m_List) {
				t.SetNeedTasktoID(m_List);
			}
		}
		void InitDatabyJson() noexcept {
			TraderIDs.resize(TraderData::Instance()->GetList().size());
			for (auto&i : TraderIDs) { i = 0; }
		}
		void SaveDatabyJson() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : GetJsonDataList()) {
					if (L.GetIDstr() == jd->id) {
						L.m_CheckJson++;

						jd->OutputData(L.GetFilePath());

						//既存のものを保持しておく
						std::ofstream outputfile(L.GetFilePath(), std::ios::app);
						for (const auto& p : L.GetTaskWorkData().GetPin()) {
							outputfile << "Task_PinPoint=[" + std::to_string(p.m_Point.x()) + DIV_STR + std::to_string(p.m_Point.y()) + DIV_STR + std::to_string(p.m_Point.z())+ "]\n";
							outputfile << "Task_PinMap=[" + MapData::Instance()->FindPtr(p.m_MapID)->GetName() + DIV_STR + std::to_string(p.m_MapSel) + "]\n";
						}
						outputfile.close();
						break;
					}
				}
			}

			bool maked = false;
			std::vector<bool> maked_t;
			maked_t.resize(TraderData::Instance()->GetList().size());
			for (auto&&i : maked_t) { i = false; }

			for (auto& jd : GetJsonDataList()) {
				TraderID trID = (dynamic_cast<TaskJsonData*>(jd.get()))->traderID;
				if (!jd->m_IsFileOpened && (trID != InvalidID)) {
					std::string ParentPath = "data/task/Maked/";
					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}
					std::string ChildPath = ParentPath;
					{
						char tID[64];
						sprintfDx(tID, "%02d", trID);
						ChildPath += tID;
						ChildPath += "_" + TraderData::Instance()->FindPtr(trID)->GetName() + "/";
						if (!maked_t[trID]) {
							CreateDirectory(ChildPath.c_str(), NULL);
							maked_t[trID] = true;
						}
					}
					std::string FileName;
					{
						char tID[64];
						sprintfDx(tID, "%02d%03d", trID, TraderIDs[trID]);
						TraderIDs[trID]++;
						FileName = tID;
						FileName += "_" + jd->name + ".txt";

					}
					SubStrs(&FileName, ".");
					std::string Name = FileName + ".txt";
					jd->OutputData(ChildPath + Name);
				}
			}
		}
		void CheckThroughJson(void) noexcept {
			for (auto& t : m_List) {
				if (t.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : ";
					ErrMes += t.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			for (auto& t : m_List) {
				if (t.m_CheckJson >= 2) {
					std::string ErrMes = "Error : Be repeated ";
					ErrMes += std::to_string(t.m_CheckJson);
					ErrMes += " : ";
					ErrMes += t.GetName();

					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}

	};
};