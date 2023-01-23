#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int TaskID;
	class TaskList : public ListParent<TaskID> {
		class TaskNeedData {
			TraderID								m_Trader{ InvalidID };
			int										m_Level{ -1 };
			int										m_LL{ -1 };
			std::string								m_task;
			TaskID									m_taskID{ InvalidID };
			std::vector<ItemGetData>				m_Item;
		public:
			const auto& GetTrader() const noexcept { return m_Trader; }
			const auto& GetLevel() const noexcept { return m_Level; }
			const auto& GetLL() const noexcept { return m_LL; }
			const auto& GettaskID() const noexcept { return m_taskID; }
			const auto& GetItem() const noexcept { return m_Item; }
		public:
			void Set(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
				if (LEFT == "Trader") {
					m_Trader = TraderData::Instance()->FindID(RIGHT.c_str());
				}
				else if (LEFT == "NeedTask") {
					this->m_task = RIGHT;
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
				for (const auto& t : taskList) {
					if (m_task == t.GetName()) {
						m_taskID = t.GetID();
						break;
					}
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
			std::vector<std::string>				m_ElseMsg;
		public:
			const auto& GetMap() const noexcept { return m_Map; }
			const auto& GetKill() const noexcept { return m_Kill; }
			const auto& GetFiR_Item() const noexcept { return m_FiR_Item; }
			const auto& GetNotFiR_Item() const noexcept { return m_NotFiR_Item; }
			const auto& GetElseMsg() const noexcept { return m_ElseMsg; }
		public:
			void Set(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
				auto SetKill = [&](const std::string& mes) {
					auto L = mes.rfind("x");
					if (L != std::string::npos) {
						EnemyKill tmp;
						auto MP = mes.rfind("-");
						if (MP != std::string::npos) {
							tmp.Set(
								MapData::Instance()->FindID(mes.substr(0, MP).c_str()),
								EnemyData::Instance()->FindID(mes.substr(MP + 1, L - (MP + 1)).c_str()),
								std::stoi(mes.substr(L + 1)));
						}
						else {
							tmp.Set(
								InvalidID,
								EnemyData::Instance()->FindID(mes.substr(0, L).c_str()),
								std::stoi(mes.substr(L + 1)));
						}
						this->m_Kill.emplace_back(tmp);
					}
					else {
						//int a = 0;
					}
				};
				if (LEFT == "Task_Map") {//ロケーション追加
					this->m_Map.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
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
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window)) + y_r(30));
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
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window)) + y_r(30));
					yofs += total_size;
				}
			}
			if (m_TaskWorkData.GetNotFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "Firでなくてよいアイテムの納品:")); yofs += sizy;
				yofs += LineHeight;
				for (const auto& LL : m_TaskWorkData.GetNotFiR_Item()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window)) + y_r(30));
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
					xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetCount(), Gray10, !WindowMngr->PosHitCheck(window)) + y_r(30));
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
		}
		~TaskData() noexcept {
		}
	public:
		const auto&		GetTaskList() const noexcept { return m_List; }
	};
};