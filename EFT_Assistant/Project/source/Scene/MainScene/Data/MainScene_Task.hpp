#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int TaskID;
	class TaskList : public ListParent<TaskID> {
		class TaskParents {
			std::string								m_Parenttask;
			TaskID									m_ParenttaskID{ InvalidID };
		public:
			void	SetParentName(const char* value) noexcept { m_Parenttask = value; }
			const auto& GetParenttaskID() const noexcept { return m_ParenttaskID; }
			void		SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
				for (const auto& t : taskList) {
					if (m_Parenttask == t.GetName()) {
						m_ParenttaskID = t.GetID();
						break;
					}
				}
			}
		};
		class TaskNeedData {
			TraderID								m_Trader{ InvalidID };
			int										m_Level{ -1 };
			int										m_LL{ -1 };
			std::vector<TaskParents>				m_Parenttask;
			std::vector<ItemGetData>				m_Item;
		public:
			const auto& GetTrader() const noexcept { return m_Trader; }
			const auto& GetLevel() const noexcept { return m_Level; }
			const auto& GetLL() const noexcept { return m_LL; }
			const auto& GetParenttaskID() const noexcept { return this->m_Parenttask; }
			const auto& GetItem() const noexcept { return m_Item; }
		public:
			void Set(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Trader") {
					m_Trader = TraderData::Instance()->FindID(RIGHT.c_str());
				}
				else if (LEFT == "NeedTask") {
					this->m_Parenttask.resize(this->m_Parenttask.size() + 1);
					this->m_Parenttask.back().SetParentName(RIGHT.c_str());
				}
				else if (LEFT == "NeedLevel") {
					this->m_Level = std::stoi(RIGHT);
				}
				else if (LEFT == "NeedLL") {
					this->m_LL = std::stoi(RIGHT);
				}
				else if (LEFT == "NeedItem") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&this->m_Item, a);
							}
						}
					}
					else {
						SetItem(&this->m_Item, RIGHT);
					}
				}
			}
			void		SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
				for (auto& t : m_Parenttask) {
					t.SetNeedTasktoID(taskList);
				}
			}
		};
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
		class TaskWorkData {
			std::vector<MapID>						m_Map;
			std::vector<EnemyKill>					m_Kill;
			std::vector<ItemGetData>				m_FiR_Item;
			std::vector<ItemGetData>				m_NotFiR_Item;
			std::vector<PresetID>					m_PresetID;
			std::vector<std::string>				m_ElseMsg;
		public:
			const auto& GetMap() const noexcept { return m_Map; }
			const auto& GetKill() const noexcept { return m_Kill; }
			const auto& GetFiR_Item() const noexcept { return m_FiR_Item; }
			const auto& GetNotFiR_Item() const noexcept { return m_NotFiR_Item; }
			const auto& GetWeaponPreset() const noexcept { return m_PresetID; }
			const auto& GetElseMsg() const noexcept { return m_ElseMsg; }
		public:
			void Set(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
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
				if (LEFT == "Task_Map") {//ロケーション追加
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								this->m_Map.emplace_back(MapData::Instance()->FindID(a.c_str()));
							}
						}
					}
					else {
						this->m_Map.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
					}
				}
				else if (LEFT == "Task_Kill") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {
							}
							else {
								SetKill(a);
							}
						}
					}
					else {
						SetKill(RIGHT);
					}
				}
				else if (LEFT == "Task_FiR_HandOver") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&this->m_FiR_Item, a);
							}
						}
					}
					else {
						SetItem(&this->m_FiR_Item, RIGHT);
					}
				}
				else if (LEFT == "Task_NotFiR_HandOver") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&this->m_NotFiR_Item, a);
							}
						}
					}
					else {
						SetItem(&this->m_NotFiR_Item, RIGHT);
					}
				}
				else if (LEFT == "Task_WeaponPreset_HandOver") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								m_PresetID.emplace_back(PresetData::Instance()->FindID(a.c_str()));
							}
						}
					}
					else {
						m_PresetID.emplace_back(PresetData::Instance()->FindID(RIGHT.c_str()));
					}
				}
				else if (LEFT == "Task_Else") {//特殊　メッセージ
					this->m_ElseMsg.emplace_back(RIGHT);
				}
			}
		};
		class TaskRewardData {
			class LLAdd {
				TraderID							m_TraderID{ InvalidID };
				float								m_Add{ 0.f };
			public:
				const auto& GetTraderID() const noexcept { return m_TraderID; }
				const auto& GetAdd() const noexcept { return m_Add; }
			public:
				void	Set(TraderID id, float add) noexcept {
					this->m_TraderID = id;
					this->m_Add = add;
				}

			};
			std::vector<LLAdd>						m_LLAdd;
			std::vector<ItemGetData>				m_Item;
		public:
			const auto& GetLLAdd() const noexcept { return m_LLAdd; }
			const auto& GetItem() const noexcept { return m_Item; }
		public:
			void Set(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Reward_Rep") {
					auto plus = RIGHT.find("+");
					auto minus = RIGHT.find("-");
					if (plus != std::string::npos) {
						TaskRewardData::LLAdd tmp;
						tmp.Set(TraderData::Instance()->FindID(RIGHT.substr(0, plus).c_str()), std::stof(RIGHT.substr(plus + 1)));
						this->m_LLAdd.emplace_back(tmp);
					}
					if (minus != std::string::npos) {
						TaskRewardData::LLAdd tmp;
						tmp.Set(TraderData::Instance()->FindID(RIGHT.substr(0, minus).c_str()), std::stof(RIGHT.substr(minus)));
						this->m_LLAdd.emplace_back(tmp);
					}
				}
				else if (LEFT == "Reward_Item") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&this->m_Item, a);
							}
						}
					}
					else {
						SetItem(&this->m_Item, RIGHT);
					}
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
	public:
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&Args) noexcept override {
			//Need
			m_TaskNeedData.Set(LEFT, RIGHT, Args);
			//Work
			m_TaskWorkData.Set(LEFT, RIGHT, Args);
			//Reward
			m_TaskRewardData.Set(LEFT, RIGHT, Args);
		}
		void	Load_Sub() noexcept override {
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}

		void			SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
			m_TaskNeedData.SetNeedTasktoID(taskList);
		}
		void			DrawWindow(WindowSystem::WindowControl* window, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* InterParts = InterruptParts::Instance();
			int xofs = 0;
			int yofs = LineHeight;
			int sizy = LineHeight * 7 / 10;
			//必要
			{
				auto* ptr = TraderData::Instance()->FindPtr(GetTrader());
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "トレーダー:%s Lv %d", ptr->GetName().c_str(), std::max(m_TaskNeedData.GetLL(), 1))); yofs += sizy;
			}
			if (m_TaskNeedData.GetLevel() >= 1) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "必要レベル:%d", m_TaskNeedData.GetLevel())); yofs += sizy;
			}
			if (m_TaskNeedData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "必要アイテム:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskNeedData.GetItem()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window), false, true) + y_r(30));
					yofs += total_size;
				}
			}
			//タスク内容
			if (m_TaskWorkData.GetMap().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "マップ指定:")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetMap()) {
					auto* ptr = MapData::Instance()->FindPtr(LL);
					xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STR_LEFT, ptr->GetColors(0), Black, "%s", ptr->GetName().c_str()) + y_r(30)); yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetKill().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "敵をキル:")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetKill()) {
					auto* eny = EnemyData::Instance()->FindPtr(LL.GetEnemyID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STR_LEFT, eny->GetColors(0), Black, "%s x%2d", eny->GetName().c_str(), LL.GetKillCount()) + y_r(30));
					if (LL.GetMapID() != InvalidID) {
						auto* ptr = MapData::Instance()->FindPtr(LL.GetMapID());
						xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(250), yp + yofs, xp + y_r(250), yp + sizy + yofs, LineHeight * 8 / 10, STR_LEFT, ptr->GetColors(0), Black, " in %s", ptr->GetName().c_str()));
					}
					yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "Firアイテムの納品:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskWorkData.GetFiR_Item()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window), true, true) + y_r(30));
					yofs += total_size;
				}
			}
			if (m_TaskWorkData.GetNotFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "Firでなくてよいアイテムの納品:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskWorkData.GetNotFiR_Item()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window), false, true) + y_r(30));
					yofs += total_size;
				}
			}
			
			if (m_TaskWorkData.GetWeaponPreset().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "カスタム品の納品:")); yofs += sizy;
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
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "メモ:")); yofs += sizy;
				for (auto& m : m_TaskWorkData.GetElseMsg()) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, m.c_str())); yofs += sizy;
				}
			}
			//
			if (m_TaskRewardData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "報酬アイテム:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskRewardData.GetItem()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window), true, true) + y_r(30));
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

	class TaskJsonData {
	public:
		bool										m_IsFileOpened{ false };
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
		std::string									id;
		std::string									name;
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
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];
			{
				std::string buf = data["trader"]["name"];
				traderID = TraderData::Instance()->FindID(buf.c_str());
			}
			if (data.contains("map")) {
				if (!data["map"].is_null()) {
					std::string buf = data["map"]["name"];
					MapID = MapData::Instance()->FindID(buf.c_str());
				}
			}
			if (data.contains("experience")) {
				if (!data["experience"].is_null()) {
					experience = data["experience"];
				}
			}
			if (data.contains("minPlayerLevel")) {
				if (!data["minPlayerLevel"].is_null()) {
					minPlayerLevel = data["minPlayerLevel"];
				}
			}
			if (data.contains("taskRequirements")) {
				for (const auto&m : data["taskRequirements"]) {
					std::string buf1 = m["task"]["name"];
					std::string buf2 = m["status"][0];
					taskRequirements.emplace_back(std::make_pair(buf1, buf2));
				}
			}
			if (data.contains("traderRequirements")) {
				for (const auto&m : data["traderRequirements"]) {
					TraderRequirements TMP;
					TMP.m_name = m["trader"]["name"];
					TMP.m_requirementType = m["requirementType"];
					TMP.m_compareMethod = m["compareMethod"];
					TMP.m_Value = m["value"];
					traderRequirements.emplace_back(TMP);
				}
			}
			if (data.contains("objectives")) {
				if (!data["objectives"].is_null()) {
					for (const auto&m : data["objectives"]) {
						TaskObjective buf1; buf1.GetJsonData(m);
						objectives.emplace_back(buf1);
					}
				}
			}
			startRewards.GetJsonData(data["startRewards"]);
			finishRewards.GetJsonData(data["finishRewards"]);
			if (data.contains("failConditions")) {
				if (!data["failConditions"].is_null()) {
					for (const auto&m : data["failConditions"]) {
						TaskObjective buf1; buf1.GetJsonData(m);
						failConditions.emplace_back(buf1);
					}
				}
			}
			failureOutcome.GetJsonData(data["failureOutcome"]);
			restartable = data["restartable"];
			if (data.contains("neededKeys")) {
				if (!data["neededKeys"].is_null()) {
					for (const auto&m : data["neededKeys"]) {
						for (const auto&k : m["keys"]) {
							neededKeys.emplace_back(k["name"]);
						}
					}
				}
			}
			kappaRequired = data["kappaRequired"];;
			lightkeeperRequired = data["lightkeeperRequired"];
		}
	public:
		TaskJsonData(){}
		~TaskJsonData(){}
	};

	class TaskData : public SingletonBase<TaskData>, public DataParent<TaskID, TaskList> {
	private:
		friend class SingletonBase<TaskData>;
	private:
		TaskData() noexcept {
			std::string Path = "data/task/";
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					SetList((Path + d.cFileName + "/").c_str());
				}
			}
			for (auto& t : m_List) {
				t.SetNeedTasktoID(m_List);
			}
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
		~TaskData() noexcept {
		}
	public:
		const auto&		GetTaskList() const noexcept { return m_List; }

	private:
		std::vector<TaskJsonData> m_TaskJsonData;

		std::vector<int> TraderIDs;
	public:
		void InitDatabyJson() noexcept {
			TraderIDs.resize(TraderData::Instance()->GetList().size());
			for (auto&i : TraderIDs) { i = 0; }
		}
		void GetJsonData(nlohmann::json& data) {
			m_TaskJsonData.clear();
			for (const auto& d : data["data"]["tasks"]) {
				m_TaskJsonData.resize(m_TaskJsonData.size() + 1);
				m_TaskJsonData.back().GetJsonData(d);
				m_TaskJsonData.back().m_IsFileOpened = false;
			}
		}
		void SaveDatabyJson() noexcept {
			auto SetTaskObjectiveCommon = [&](std::ofstream& outputfile, const TaskJsonData::TaskObjective& obj) {
				outputfile << "[\n";
				outputfile << "\tTaskType=" + obj.type + "\n";
				outputfile << "\tTaskText=" + obj.description + "\n";
				outputfile << "\tTaskOptional=" + (std::string)(obj.optional ? "true" : "false") + "\n";
				outputfile << "]\n";
			};
			auto SetTaskObjective = [&](std::ofstream& outputfile, const TaskJsonData::TaskObjective& obj, EnumTaskObjective /*prev*/) {
				switch ((EnumTaskObjective)obj.TaskObjectiveType) {
				case FPS_n2::EnumTaskObjective::TaskObjectiveBasic:
				{
					outputfile << "Task_Else=TaskType:" + obj.type + "\n";
					outputfile << "Task_Else=" + obj.description + "\n";
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveBuildItem:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					if (obj.Items != InvalidID) {
						outputfile << "NeedItem=" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x1\n";
					}
					for (auto& m : obj.containsAll) {
						outputfile << "ContainsAll=" + m + "\n";
					}
					for (auto& m : obj.containsCategory) {
						outputfile << "ContainsCategory=" + m + "\n";
					}
					for (auto& m : obj.attributes) {
						if (m.second.IsActive()) {
							outputfile << "Task_Else=" + m.first + " " + (std::string)(CompareMethodStr[(int)m.second.compareMethod]) + " " + std::to_string(m.second.value) + "\n";
						}
						else {
							outputfile << "Task_Else=" + m.first + "\n";
						}
					}
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveExperience:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					for (auto& m : obj.healthEffect) {
						if (m.bodyParts.size() > 0) {
							outputfile << "Task_Else=自分の状態異常箇所:[";
							for (auto& m2 : m.bodyParts) {
								outputfile << m2;
								if (&m2 != &m.bodyParts.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						if (m.effects.size() > 0) {
							outputfile << "Task_Else=自分の状態異常内容:[";
							for (auto& m2 : m.effects) {
								outputfile << m2;
								if (&m2 != &m.effects.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						if (m.m_time.IsActive()) {
							outputfile << "Task_Else=耐久時間: " + (std::string)(CompareMethodStr[(int)m.m_time.compareMethod]) + " " + std::to_string(m.m_time.value) + "\n";
						}
					}
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveExtract:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					if (obj.exitStatus.size() > 0) {
						outputfile << "Task_Else=脱出ステータス:[";
						for (auto& m : obj.exitStatus) {
							outputfile << m;
							if (&m != &obj.exitStatus.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.exitName != "") {
						outputfile << "Task_Else=脱出地点:" + obj.exitName + "\n";
					}
					if (obj.zoneNames.size() > 0) {
						outputfile << "Task_Else=脱出ゾーン:[";
						for (auto& m : obj.zoneNames) {
							outputfile << m;
							if (&m != &obj.zoneNames.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveItem:
				{
					if (obj.type == "findItem") {
						if (obj.Items != InvalidID) {
							outputfile << "NeedItem=" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x" + std::to_string(obj.count) + "\n";
						}
					}
					else {
						if (obj.Items != InvalidID) {
							if (obj.foundInRaid) {
								outputfile << "Task_FiR_HandOver=[" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x" + std::to_string(obj.count) + "]\n";
							}
							else {
								outputfile << "Task_NotFiR_HandOver=[" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x" + std::to_string(obj.count) + "]\n";
							}
						}
						outputfile << "DogTagLv=" + std::to_string(obj.dogTagLevel) + "\n";
						//outputfile << "Max=" + std::to_string(obj.maxDurability) + "\n";
						//outputfile << "Min=" + std::to_string(obj.minDurability) + "\n";
					}
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveMark:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					outputfile << "NeedItem=" + obj.markerItem + "x1\n";
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectivePlayerLevel:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					outputfile << "Task_Else=レベル:" + std::to_string(obj.playerLevel) + "\n";
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveQuestItem:
					for (auto& m : obj.Maps) {
						outputfile << "Task_Else=拾って納品:" + MapData::Instance()->FindPtr(m)->GetName() + "-" + obj.questItem + "x" + std::to_string(obj.count) + "\n";
					}
					break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveShoot:
				{
					outputfile << "Task_Kill=[\n";
					if (obj.Maps.size() == 0) {
						outputfile << "\t" + EnemyData::Instance()->FindPtr(obj.target)->GetName();
						if (obj.bodyParts.size() > 0) {
							outputfile << "{";
							for (auto& m2 : obj.bodyParts) {
								outputfile << m2;
								if (&m2 != &obj.bodyParts.back()) {
									outputfile << "/";
								}
							}
							outputfile << "}";
						}
						outputfile << "x" + std::to_string(obj.count);
						outputfile << "\n";
					}
					else {
						for (auto& m : obj.Maps) {
							outputfile << "\t" + MapData::Instance()->FindPtr(m)->GetName() + "-" + EnemyData::Instance()->FindPtr(obj.target)->GetName();
							if (obj.bodyParts.size() > 0) {
								outputfile << "{";
								for (auto& m2 : obj.bodyParts) {
									outputfile << m2;
									if (&m2 != &obj.bodyParts.back()) {
										outputfile << "/";
									}
								}
								outputfile << "}";
							}
							outputfile << "x" + std::to_string(obj.count);
							if (&m != &obj.Maps.back()) {
								outputfile << ",";
							}
							outputfile << "\n";
						}
					}
					outputfile << "]\n";

					//outputfile << "shotType=" + obj.shotType + "\n";
					if (obj.zoneNames.size() > 0) {
						outputfile << "Task_Else=ゾーン:[";
						for (auto& m : obj.zoneNames) {
							outputfile << m;
							if (&m != &obj.zoneNames.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.usingWeapon.size() > 0) {
						outputfile << "NeedItem=[\n";
						for (auto& m : obj.usingWeapon) {
							outputfile << "\t" + m + "x0";
							if (&m != &obj.usingWeapon.back()) {
								outputfile << ",";
							}
							outputfile << "\n";
						}
						outputfile << "]\n";
					}
					if (obj.usingWeaponMods.size() > 0) {
						outputfile << "NeedItem=[\n";
						for (auto& m : obj.usingWeaponMods) {
							outputfile << "\t" + m + "x0";
							if (&m != &obj.usingWeaponMods.back()) {
								outputfile << ",";
							}
							outputfile << "\n";
						}
						outputfile << "]\n";
					}
					if (obj.wearing.size() > 0) {
						outputfile << "Task_Else=装備が必要:{\n";
						for (auto& m : obj.wearing) {
							outputfile << "Task_Else=  " + m + "\n";
						}
						outputfile << "Task_Else=}\n";
					}
					if (obj.notWearing.size() > 0) {
						outputfile << "Task_Else=装備してはいけない:{\n";
						for (auto& m : obj.notWearing) {
							outputfile << "Task_Else=  " + m + "\n";
						}
						outputfile << "Task_Else=}\n";
					}
					if (obj.distance.IsActive()) {
						outputfile << "Task_Else=距離: " + (std::string)(CompareMethodStr[(int)obj.distance.compareMethod]) + " " + std::to_string(obj.distance.value) + "m\n";
					}
					{
						if (obj.playerHealthEffect.bodyParts.size() > 0) {
							outputfile << "Task_Else=自分の該当箇所に異常状態:[";
							for (auto& m2 : obj.playerHealthEffect.bodyParts) {
								outputfile << m2;
								if (&m2 != &obj.playerHealthEffect.bodyParts.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						if (obj.playerHealthEffect.effects.size() > 0) {
							outputfile << "Task_Else=自分の異常状態:[";
							for (auto& m2 : obj.playerHealthEffect.effects) {
								outputfile << m2;
								if (&m2 != &obj.playerHealthEffect.effects.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						if (obj.playerHealthEffect.m_time.IsActive()) {
							outputfile << "Task_Else=閾値: " + (std::string)(CompareMethodStr[(int)obj.playerHealthEffect.m_time.compareMethod]) + " " + std::to_string(obj.playerHealthEffect.m_time.value) + "\n";
						}
					}
					{
						if (obj.enemyHealthEffect.bodyParts.size() > 0) {
							outputfile << "Task_Else=敵の該当箇所に異常状態:[";
							for (auto& m2 : obj.enemyHealthEffect.bodyParts) {
								outputfile << m2;
								if (&m2 != &obj.enemyHealthEffect.bodyParts.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						if (obj.enemyHealthEffect.effects.size() > 0) {
							outputfile << "Task_Else=敵の異常状態:[";
							for (auto& m2 : obj.enemyHealthEffect.effects) {
								outputfile << m2;
								if (&m2 != &obj.enemyHealthEffect.effects.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						if (obj.enemyHealthEffect.m_time.IsActive()) {
							outputfile << "Task_Else=閾値: " + (std::string)(CompareMethodStr[(int)obj.enemyHealthEffect.m_time.compareMethod]) + " "+ std::to_string(obj.enemyHealthEffect.m_time.value) + "\n";
						}
					}
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveSkill:
					for (auto& m : obj.skillLevel) {
						outputfile << "Task_Else=Skillを上げる:" + m.first + "+" + std::to_string(m.second) + "\n";
					}
					break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveTaskStatus:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					outputfile << "Task_Else=該当タスク" + obj.task + "\n";
					if (obj.status.size() > 0) {
						outputfile << "Task_Else=該当タスク状態:";
						for (auto& m : obj.status) {
							outputfile << m;
							if (&m != &obj.status.back()) {
								outputfile << ",";
							}
						}
						outputfile << "\n";
					}
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveTraderLevel:
				{
					outputfile << "Task_Else=traderLL:" + obj.trader + "LL" + std::to_string(obj.level) + "\n";
				}
				break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveTraderStanding:
					if (obj.Compares.IsActive()) {
						outputfile << "Task_Else=TraderStandingの閾値: " + (std::string)(CompareMethodStr[(int)obj.Compares.compareMethod]) + " " + std::to_string(obj.Compares.value) + "\n";
					}
					break;
				case FPS_n2::EnumTaskObjective::TaskObjectiveUseItem:
				{
					for (auto& m : obj.Maps) {
						outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
					}
					if (obj.Compares.IsActive()) {
						outputfile << "Task_Else=閾値: " + (std::string)(CompareMethodStr[(int)obj.Compares.compareMethod]) + " " + std::to_string(obj.Compares.value) + "\n";
					}
					if (obj.zoneNames.size() > 0) {
						outputfile << "Task_Else=ゾーン:[";
						for (auto& m : obj.zoneNames) {
							outputfile << m;
							if (&m != &obj.zoneNames.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
				}
				break;
				default:
					break;
				}
			};
			auto SetTaskRewards = [&](std::ofstream& outputfile, const TaskJsonData::TaskRewards& obj) {
				for (auto& m : obj.m_traderStanding) {
					outputfile << "Reward_Rep=" + m.trader + ((m.standing >= 0) ? "+" : "") + std::to_string(m.standing) + "\n";
				}
				if (obj.Items.size() > 0) {
					outputfile << "Reward_Item=[\n";
					for (auto& m : obj.Items) {
						if (m.ID != InvalidID) {
							outputfile << "\t" + ItemData::Instance()->FindPtr(m.ID)->GetName() + "x" + std::to_string(m.count);
						}
						else {
							outputfile << "\t" + m.Name + "x" + std::to_string(m.count);
						}
						if (&m != &obj.Items.back()) {
							outputfile << ",";
						}
						outputfile << "\n";
					}
					outputfile << "]\n";
				}
				for (auto& m : obj.m_skillLevelReward) {
					outputfile << "Reward_SkillLevelUp=" + m.name + ((m.level >= 0) ? "+" : "") + std::to_string(m.level) + "\n";
				}
				for (auto& m : obj.traderUnlock) {
					outputfile << "Reward_TraderUnlock=" + m + "\n";
				}
			};
			auto WriteText = [&](std::ofstream& outputfile, const TaskJsonData& jd) {
				outputfile << "IDstr=" + jd.id + "\n";
				outputfile << "Name=" + jd.name + "\n";
				outputfile << "Trader=" + TraderData::Instance()->FindPtr(jd.traderID)->GetName() + "\n";
				outputfile << "\n";
				//
				for (auto& tr : jd.taskRequirements) {
					outputfile << "NeedTask=" + tr.first + "\n";
				}
				outputfile << "NeedLevel=" + std::to_string(jd.minPlayerLevel) + "\n";
				if (jd.neededKeys.size() > 0) {
					outputfile << "NeedItem=[";
					for (auto& n : jd.neededKeys) {
						outputfile << n + "x1";
						if (&n != &jd.neededKeys.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
				outputfile << "\n";
				//
				outputfile << "CanRestart=" + (std::string)(jd.restartable ? "true" : "false") + "\n";
				outputfile << "NeedKappa=" + (std::string)(jd.kappaRequired ? "true" : "false") + "\n";
				outputfile << "NeedLightkeeper=" + (std::string)(jd.lightkeeperRequired ? "true" : "false") + "\n";
				outputfile << "\n";
				//
				if (jd.MapID != InvalidID) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(jd.MapID)->GetName() + "\n";
				}
				for (auto& tr : jd.traderRequirements) {
					outputfile << "traderRequirementsName=" + tr.m_name + "\n";
					outputfile << "traderRequirementsType=" + tr.m_requirementType + "\n";
					outputfile << "traderRequirements=" + tr.m_compareMethod + "\n";
					outputfile << "traderRequirementsValue=" + std::to_string(tr.m_Value) + "\n";
				}
				if (jd.objectives.size() > 0) {
					EnumTaskObjective Prev = EnumTaskObjective::Max;
					for (auto& ob : jd.objectives) {
						SetTaskObjective(outputfile, ob, Prev);
						Prev = ob.TaskObjectiveType;
					}
				}
				if (jd.objectives.size() > 0) {
					outputfile << "\n";
					for (auto& ob : jd.objectives) {
						SetTaskObjectiveCommon(outputfile, ob);
					}
				}
				outputfile << "\n";
				SetTaskRewards(outputfile, jd.startRewards);
				SetTaskRewards(outputfile, jd.finishRewards);
				if (jd.failConditions.size() > 0) {
					outputfile << "\n";
					EnumTaskObjective Prev = EnumTaskObjective::Max;
					for (auto& ob : jd.failConditions) {
						SetTaskObjective(outputfile, ob, Prev);
						Prev = ob.TaskObjectiveType;
					}
				}
				if (jd.failConditions.size() > 0) {
					outputfile << "\n";
					for (auto& ob : jd.failConditions) {
						SetTaskObjectiveCommon(outputfile, ob);
					}
				}
				outputfile << "\n";
				SetTaskRewards(outputfile, jd.failureOutcome);
				outputfile << "Reward_EXP=" + std::to_string(jd.experience) + "\n";
			};

			for (auto& L : m_List) {
				for (auto& jd : m_TaskJsonData) {
					if (L.GetIDstr() == jd.id) {
						L.m_CheckJson++;

						jd.m_IsFileOpened = true;

						std::ofstream outputfile(L.GetFilePath());
						WriteText(outputfile, jd);
						outputfile.close();
						break;
					}
				}
			}
			bool maked = false;

			std::vector<bool> maked_t;
			maked_t.resize(TraderData::Instance()->GetList().size());
			for (auto&&i : maked_t) { i = false; }


			for (auto& jd : m_TaskJsonData) {
				if (!jd.m_IsFileOpened && (jd.traderID != InvalidID)) {
					std::string ParentPath = "data/task/Maked/";
					if (!maked) {
						CreateDirectory(ParentPath.c_str(), NULL);
						maked = true;
					}
					char tID[64];
					sprintfDx(tID, "%02d", jd.traderID);


					std::string ChildPath = ParentPath + tID + "_" + TraderData::Instance()->FindPtr(jd.traderID)->GetName() + "/";
					if (!maked_t[jd.traderID]) {
						CreateDirectory(ChildPath.c_str(), NULL);
						maked_t[jd.traderID] = true;
					}
					std::string TaskName = jd.name;
					while (true) {
						auto now = TaskName.find(".");
						if (now != std::string::npos) {
							TaskName = TaskName.substr(0, now) + TaskName.substr(now + 1);
						}
						else {
							break;
						}
					}

					char ID[64];
					sprintfDx(ID, "%03d", TraderIDs[jd.traderID]);
					TraderIDs[jd.traderID]++;

					std::string Name = ChildPath + tID + ID + "_" + TaskName + ".txt";
					std::ofstream outputfile(Name);
					WriteText(outputfile, jd);
					outputfile.close();
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