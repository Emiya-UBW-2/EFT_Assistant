#pragma once
#include"Header.hpp"

#define LineHeight	y_r(48)

namespace FPS_n2 {
	//
	class Rect2D {
		int			m_PosX{ 0 };
		int			m_PosY{ 0 };
		int			m_SizeX{ 0 };
		int			m_SizeY{ 0 };
	public:
		const auto&		GetPosX() const noexcept { return m_PosX; }
		const auto&		GetPosY() const noexcept { return m_PosY; }
		void			Set(int posx, int posy, int sizex, int sizey) noexcept {
			m_PosX = posx;
			m_PosY = posy;
			m_SizeX = sizex;
			m_SizeY = sizey;
		}
	public:
		bool			IsHit(const Rect2D& target) noexcept {
			return (
				((this->m_PosX >= target.m_PosX && this->m_PosX < (target.m_PosX + target.m_SizeX)) || (target.m_PosX > this->m_PosX && target.m_PosX <= (this->m_PosX + this->m_SizeX))) &&
				((this->m_PosY >= target.m_PosY && this->m_PosY < (target.m_PosY + target.m_SizeY)) || (target.m_PosY > this->m_PosY && target.m_PosY <= (this->m_PosY + this->m_SizeY)))
				);
		}
	};
	//
	static const int InvalidID{ -1 };
	//
	template <class ID>
	class ListParent {
	private:
		ID						m_ID{ InvalidID };
		std::string				m_Name;
		std::array<int, 3>		m_Color{ 0,0,0 };
	private:
		const auto		GetArgs(const std::string& RIGHT) const noexcept {
			std::vector<std::string> Args;
			auto L = RIGHT.find("[");
			auto R = RIGHT.find("]");
			if (L != std::string::npos && R != std::string::npos) {
				std::string RIGHTBuf = RIGHT;
				RIGHTBuf = RIGHTBuf.substr(L + 1);
				while (true) {
					auto div = RIGHTBuf.find(",");
					if (div != std::string::npos) {
						Args.emplace_back(RIGHTBuf.substr(0, div));
						RIGHTBuf = RIGHTBuf.substr(div + 1);
					}
					else {
						Args.emplace_back(RIGHTBuf.substr(0, RIGHTBuf.find("]")));
						break;
					}
				}
			}
			return Args;
		}
		void			SetCommon(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept {
			if (LEFT == "Name") {
				m_Name = RIGHT;
			}
			else if (LEFT == "Color") {
				m_Color[0] = std::stoi(Args[0]);
				m_Color[1] = std::stoi(Args[1]);
				m_Color[2] = std::stoi(Args[2]);
			}
		}
	protected:
		virtual void	Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept {}
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetName() const noexcept { return m_Name; }
		const auto		GetColors(int colorAdd) const noexcept {
			return DxLib::GetColor(std::clamp(m_Color[0] + colorAdd, 0, 255), std::clamp(m_Color[1] + colorAdd, 0, 255), std::clamp(m_Color[2] + colorAdd, 0, 255));
		}
	public:
		void			Set(const char* FilePath, ID id) noexcept {
			m_ID = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				auto Args = GetArgs(RIGHT);
				SetCommon(LEFT, RIGHT, Args);
				Set_Sub(LEFT, RIGHT, Args);
			}
			FileRead_close(mdata);
		}
	};
	//
	template <class ID, class List>
	class DataParent {
	protected:
		std::vector<List>	m_List;
	protected:
		void SetList(const char* DirPath) noexcept {
			auto data_t = GetFileNamesInDirectory(DirPath);
			std::vector<std::string> DirNames;
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					DirNames.emplace_back(d.cFileName);
				}
			}
			m_List.resize(DirNames.size());
			for (auto& d : DirNames) {
				int index = (int)(&d - &DirNames.front());
				m_List[index].Set((DirPath + d).c_str(), (ID)index);
			}
			DirNames.clear();
		}
	public:
		const ID		FindID(const char* name) const noexcept {
			for (const auto&t : m_List) {
				if (t.GetName() == name) {
					return t.GetID();
				}
			}
			return InvalidID;
		}
		const List*		FindPtr(ID id) const noexcept {
			for (const auto&t : m_List) {
				if (t.GetID() == id) {
					return &t;
				}
			}
			return nullptr;
		}
	};
	//
	typedef int MapID;
	class MapList : public ListParent<MapID> {
		//追加設定
		void			Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept override {}
	};
	class MapData : public SingletonBase<MapData>, public DataParent<MapID, MapList> {
	private:
		friend class SingletonBase<MapData>;
	private:
		MapData() noexcept {
			SetList("data/map/");
		}
		~MapData() noexcept {}
	};
	//
	typedef int ItemID;
	class ItemList : public ListParent<ItemID> {
		//追加設定
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "Itemtype") {
				RIGHT;
			}
		}
	};
	class ItemData : public SingletonBase<ItemData>, public DataParent<ItemID, ItemList> {
	private:
		friend class SingletonBase<ItemData>;
	private:
		ItemData() noexcept {
			SetList("data/item/");
		}
		~ItemData() noexcept {}
	};
	//
	namespace ItemDatas {
		class ItemGetData {
			ItemID				m_ID{ InvalidID };
			int					m_Count{ 0 };
		public:
			const auto&		GetID() const noexcept { return m_ID; }
			const auto&		GetCount() const noexcept { return m_Count; }
			void			Set(ItemID id, int count) noexcept {
				m_ID = id;
				m_Count = count;
			}
		};
		void			SetItem(std::vector<ItemGetData>* Data, const std::string& mes) noexcept {
			auto L = mes.rfind("x");
			if (L != std::string::npos) {
				ItemGetData tmp;
				tmp.Set(ItemData::Instance()->FindID(mes.substr(0, L).c_str()), std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
			else {
				//int a = 0;
			}
		};
	};
	//
	typedef int EnemyID;
	class EnemyList : public ListParent<EnemyID> {
		//追加設定
		void			Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept override {}
	};
	class EnemyData : public SingletonBase<EnemyData>, public DataParent<EnemyID, EnemyList> {
	private:
		friend class SingletonBase<EnemyData>;
	private:
		EnemyData() noexcept {
			SetList("data/enemy/");
		}
		~EnemyData() noexcept {}
	};
	//
	typedef int TraderID;
	class TraderList : public ListParent<TraderID> {
		std::array<int, 4>		m_Lv{ 0,0,0,0 };
		std::array<float, 4>	m_Rep{ 0,0,0,0 };
	public:
		//追加設定
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			for (int i = 0; i < 4; i++) {
				if (i == 0) {
					m_Lv[i] = 1;
					m_Rep[i] = 0.f;
				}
				else {
					if (LEFT == "LL" + std::to_string(i + 1) + "Rep") {
						m_Rep[i] = std::stof(RIGHT);
						break;
					}
					else if (LEFT == "LL" + std::to_string(i + 1) + "Lv") {
						m_Lv[i] = std::stoi(RIGHT);
						break;
					}
				}
			}
		}
	};
	class TraderData : public SingletonBase<TraderData>, public DataParent<TraderID, TraderList> {
	private:
		friend class SingletonBase<TraderData>;
	private:
		std::vector<float>		m_TraderRep;
	private:
		TraderData() noexcept {
			SetList("data/trader/");
			m_TraderRep.resize(m_List.size());
		}
		~TraderData() noexcept {
		}
	public:
		void			ResetRep() noexcept {
			if (true) {
				for (auto&t : this->m_TraderRep) {
					t = 0.f;
				}
			}
			else {
				for (auto&t : this->m_TraderRep) {
					t = 0.2f;
				}
			}
		}
		float*			FindTraderRep(TraderID ID) noexcept {
			for (const auto&t : m_List) {
				if (t.GetID() == ID) {
					return &m_TraderRep[&t - &m_List.front()];
				}
			}
			return nullptr;
		}
	};
	//
	typedef int TaskID;
	class TaskList : public ListParent<TaskID> {
		class TaskNeedData {
			TraderID								m_Trader{ InvalidID };
			int										m_Level{ -1 };
			int										m_LL{ -1 };
			std::string								m_task;
			TaskID									m_taskID{ InvalidID };
			std::vector<ItemDatas::ItemGetData>		m_Item;
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
								ItemDatas::SetItem(&this->m_Item, a);
							}
						}
					}
					else {
						ItemDatas::SetItem(&this->m_Item, RIGHT);
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
			std::vector<ItemDatas::ItemGetData>		m_FiR_Item;
			std::vector<ItemDatas::ItemGetData>		m_NotFiR_Item;
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
								ItemDatas::SetItem(&this->m_FiR_Item, a);
							}
						}
					}
					else {
						ItemDatas::SetItem(&this->m_FiR_Item, RIGHT);
					}
				}
				else if (LEFT == "Task_NotFiR_HandOver") {
					if (Args.size() > 0) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								ItemDatas::SetItem(&this->m_NotFiR_Item, a);
							}
						}
					}
					else {
						ItemDatas::SetItem(&this->m_NotFiR_Item, RIGHT);
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
			std::vector<ItemDatas::ItemGetData>		m_Item;
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
								ItemDatas::SetItem(&this->m_Item, a);
							}
						}
					}
					else {
						ItemDatas::SetItem(&this->m_Item, RIGHT);
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
		void			SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
			m_TaskNeedData.SetNeedTasktoID(taskList);
		}
		void			DrawWindow(int xp, int yp, int *xs, int* ys) const noexcept {
			int xofs = 0;
			int yofs = LineHeight;
			int sizy = LineHeight * 7 / 10;
			//
			if (m_TaskNeedData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "必要アイテム")); yofs += sizy;
				for (const auto& LL : m_TaskNeedData.GetItem()) {
					auto* map = ItemData::Instance()->FindPtr(LL.GetID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "└%s x%2d", map->GetName().c_str(), LL.GetCount())); yofs += sizy;
				}
				yofs += sizy;
			}
			//
			if (m_TaskWorkData.GetMap().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "マップ指定")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetMap()) {
					auto* map = MapData::Instance()->FindPtr(LL);
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, map->GetColors(0), Black, "└%s", map->GetName().c_str())); yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetKill().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "敵をキル")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetKill()) {
					auto* eny = EnemyData::Instance()->FindPtr(LL.GetEnemyID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, eny->GetColors(0), Black, "└%s x%2d", eny->GetName().c_str(), LL.GetKillCount()));
					if (LL.GetMapID() != InvalidID) {
						auto* map = MapData::Instance()->FindPtr(LL.GetMapID());
						xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(250), yp + yofs, xp + y_r(250), yp + sizy + yofs, LineHeight * 8 / 10, FontHandle::FontXCenter::LEFT, map->GetColors(0), Black, " in %s", map->GetName().c_str()));
					}
					yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "Firアイテムの納品")); yofs += sizy;
				for (const auto& LL : m_TaskWorkData.GetFiR_Item()) {
					auto* map = ItemData::Instance()->FindPtr(LL.GetID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "└%s x%2d", map->GetName().c_str(), LL.GetCount())); yofs += sizy;
				}
				yofs += sizy;
			}
			if (m_TaskWorkData.GetNotFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "Firでなくてよいアイテムの納品")); yofs += sizy;
				for (const auto& LL : m_TaskWorkData.GetNotFiR_Item()) {
					auto* map = ItemData::Instance()->FindPtr(LL.GetID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "└%s x%2d", map->GetName().c_str(), LL.GetCount())); yofs += sizy;
				}
				yofs += sizy;
			}
			if (m_TaskWorkData.GetElseMsg().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "メモ:")); yofs += sizy;
				for (auto& m : m_TaskWorkData.GetElseMsg()) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, m.c_str())); yofs += sizy;
				}
			}
			//
			if (m_TaskRewardData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "報酬アイテム")); yofs += sizy;
				for (const auto& LL : m_TaskRewardData.GetItem()) {
					auto* map = ItemData::Instance()->FindPtr(LL.GetID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, FontHandle::FontXCenter::LEFT, White, Black, "└%s x%2d", map->GetName().c_str(), LL.GetCount())); yofs += sizy;
				}
				yofs += sizy;
			}
			//
			*xs = std::max(*xs, xofs + LineHeight / 10);
			*ys = std::max(*ys, yofs + LineHeight / 10);
		}
	};
	class TaskData : public SingletonBase<TaskData>, public DataParent<TaskID, TaskList> {
	private:
		friend class SingletonBase<TaskData>;
	private:
		TaskData() noexcept {
			SetList("data/task/");
			for (auto& t : m_List) {
				t.SetNeedTasktoID(m_List);
			}
		}
		~TaskData() noexcept {
		}
	public:
		const auto&		GetTaskList() const noexcept { return m_List; }
	};
	//
	class MAINLOOP : public TEMPSCENE {
	private:
		std::unique_ptr<WindowSystem::WindowManager>				m_Window;
		std::vector<GraphHandle>									m_TaskGraph;
		std::vector<std::shared_ptr<WindowSystem::WindowControl>>	m_TaskPtr;
		int															m_MyLevel{ 1 };
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		int															m_posxMaxBuffer{ 0 };
		int															m_posyMaxBuffer{ 0 };
		float														m_Scale{ 0.6f };
		std::vector<Rect2D>											m_TaskRect;
		bool														m_IsPullDown{ false };
		float														m_PullDown{ 1.f };
	private:
		void DrawChildTaskClickBox(TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) noexcept {
			if (ParentID == InvalidID) {
				m_posxMaxBuffer = 0;
				m_posyMaxBuffer = 0;
			}
			for (const auto& tasks : TaskData::Instance()->GetTaskList()) {
				if (ParentID == InvalidID) {
					TraderData::Instance()->ResetRep();
				}
				if (tasks.GetTaskNeedData().GettaskID() == ParentID) {
					auto* trader = TraderData::Instance()->FindPtr(tasks.GetTrader());
					auto parentCanDo_t = parentCanDo;
					auto color = trader->GetColors(0);
					//信頼度アップダウンを対応
					std::vector<float> PrevRep;
					PrevRep.resize(tasks.GetTaskRewardData().GetLLAdd().size());
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()] = *traderRep;
						*traderRep += LL.GetAdd();
					}
					//信頼度チェック
					if (
						(
						(this->m_MyLevel < tasks.GetTaskNeedData().GetLevel())
							) || !parentCanDo) {
						color = trader->GetColors(-100);
						parentCanDo_t = false;
					}
					if (ParentID != InvalidID) {
						DrawLine(start_x, start_y, xp, yp + ys / 2, Red, (int)(5.f * m_Scale));
					}
					if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ys, false, !m_Window->PosHitCheck(), color, tasks.GetName())) {
						auto sizeXBuf = y_r(640);
						auto sizeYBuf = y_r(0);
						tasks.DrawWindow(0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
						m_TaskGraph.emplace_back(GraphHandle::Make(sizeXBuf, sizeYBuf, true));
						m_TaskGraph.back().SetDraw_Screen();
						{
							tasks.DrawWindow(0, 0, &sizeXBuf, &sizeYBuf);
						}
						SetDrawScreen(DX_SCREEN_BACK);
						//
						m_TaskPtr.emplace_back(this->m_Window->Add());
						m_TaskPtr.back()->Set(xp + xs / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, [&](WindowSystem::WindowControl* win) {
							for (auto& t : m_TaskPtr) {
								if (t.get() == win) {
									m_TaskGraph[&t - &m_TaskPtr.front()].DrawGraph(win->GetPosX(), win->GetPosY(), true);
									break;
								}
							}
						});
					}
					int suby = ys;
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						auto* trader2 = TraderData::Instance()->FindPtr(LL.GetTraderID());
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + ys + suby, ys, FontHandle::FontXCenter::LEFT, trader2->GetColors(0), Black, "[%4.2f->%4.2f]", PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()], *traderRep); suby += ys;
					}
					m_posxMaxBuffer = std::max(m_posxMaxBuffer, xp + xs);
					m_posyMaxBuffer = std::max(m_posyMaxBuffer, yp + ys + suby);
					{
						Rect2D tmp; tmp.Set(xp, yp, xs, suby);
						m_TaskRect.emplace_back(tmp);
					}
					Rect2D P_Next;
					P_Next.Set(xp + (xs + (int)((float)y_r(50) * m_Scale)), yp, xs, suby);
					//xs, ys
					//被ってたら下に下げる
					while (true) {
						bool isHit = false;
						for (auto&r : m_TaskRect) {
							if (r.IsHit(P_Next)) {
								isHit = true;
								P_Next.Set(xp + (xs + (int)((float)y_r(50) * m_Scale)), P_Next.GetPosY() + ys, xs, suby);
								break;
							}
						}
						if (!isHit) { break; }
					}
					DrawChildTaskClickBox(tasks.GetID(), xp + xs, yp + ys / 2, P_Next.GetPosX(), P_Next.GetPosY(), xs, ys, parentCanDo_t);
					//親なのでいったん信頼度を戻す
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						*traderRep = PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()];
					}
					//
					if (ParentID == InvalidID) {
						yp += (ys + (int)((float)y_r(400) * m_Scale));
					}
					else {
						yp += (ys + suby);
					}
				}
			}
			if (ParentID == InvalidID) {
				m_TaskRect.clear();
			}
		}
	public:
		void Load_Sub(void) noexcept override {}

		void Set_Sub(void) noexcept override {
			//
			InputControl::Create();
			m_Window = std::make_unique<WindowSystem::WindowManager>();
			//
			ItemData::Create();
			EnemyData::Create();
			TraderData::Create();
			MapData::Create();
			TaskData::Create();
			//
		}

		bool Update_Sub(void) noexcept override {
			//FirstDoing
			if (GetIsFirstLoop()) {
				SetWindowPosition(0, 0);
			}
			auto* Input = InputControl::Instance();
			auto mouse_moveX = Input->GetMouseX();							//ドラッグ前のマウス座標格納
			auto mouse_moveY = Input->GetMouseY();
			Input->Execute();
			mouse_moveX = Input->GetMouseX() - mouse_moveX;
			mouse_moveY = Input->GetMouseY() - mouse_moveY;
			//ドラッグ開始時の処理
			if (Input->GetMiddleClick().press()) {
				int start_windowX = 0, start_windowY = 0;
				GetWindowPosition(&start_windowX, &start_windowY);			//ウィンドウの位置を格納
				start_windowX += mouse_moveX;
				start_windowY += mouse_moveY;
				SetWindowPosition(start_windowX, start_windowY);			//マウス位置の差を算出し、ウィンドウを動かす
				Input->SetMouse();
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			//
			if (Input->GetWheelAdd() != 0) {
				auto PrevScale = this->m_Scale;
				this->m_Scale = std::clamp(this->m_Scale + (float)Input->GetWheelAdd() / 10.f, 0.1f, 2.f);
				auto ScaleChange = (this->m_Scale - PrevScale);
				if (ScaleChange != 0.f) {
					this->m_posx -= (int)((float)(Input->GetMouseX() - this->m_posx) * ScaleChange / this->m_Scale);
					this->m_posx -= (int)((float)(Input->GetMouseY() - this->m_posx) * ScaleChange / this->m_Scale);
				}
			}
			if (Input->GetRightClick().press()) {
				this->m_posx += mouse_moveX;
				this->m_posy += mouse_moveY;
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			//
			m_Window->Execute();
			for (int i = 0; i < m_TaskPtr.size(); i++) {
				if (m_TaskPtr[i].use_count() <= 1) {
					std::swap(m_TaskGraph[i], m_TaskGraph.back());
					m_TaskGraph.back().Dispose();
					m_TaskGraph.pop_back();
					std::swap(m_TaskPtr[i], m_TaskPtr.back());
					m_TaskPtr.back().reset();
					m_TaskPtr.pop_back();
				}
			}
			return true;
		}
		void Dispose_Sub(void) noexcept override {
			m_Window.reset();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}

		//UI表示
		void DrawUI_Base_Sub(void) noexcept  override {
			auto* DrawParts = DXDraw::Instance();
			DrawBox(0, 0, DrawParts->m_DispXSize, (int)((float)DrawParts->m_DispYSize*m_PullDown), Gray15, TRUE);
			if (m_PullDown >= 1.f) {
				//
				int xs = (int)((float)y_r(520) * m_Scale);
				int ys = (int)((float)LineHeight * m_Scale);
				DrawChildTaskClickBox(InvalidID, m_posx + xs, m_posy + ys / 2, m_posx, m_posy, xs, ys);
				//
				m_Window->Draw();
			}
			//
			WindowSystem::SetBox(y_r(0), y_r(0), y_r(1920), LineHeight, Gray50);
			WindowSystem::SetMsg(y_r(0), y_r(0), y_r(1920), LineHeight, LineHeight, FontHandle::FontXCenter::MIDDLE, White, Black, "EFT Assistant");
			if (WindowSystem::CloseButton(y_r(1920) - LineHeight, y_r(0))) {
				SetisEnd(true);
			}
			if (WindowSystem::ClickCheckBox(y_r(0), y_r(0), y_r(320), LineHeight, false, true, Gray25, !m_IsPullDown ? "折りたたむ" : "展開")) {
				m_IsPullDown ^= 1;
			}
			Easing(&m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
			if (m_PullDown >= 0.95f) { m_PullDown = 1.f; }
			if (m_PullDown <= 0.05f) { m_PullDown = 0.f; }
			if (m_PullDown >= 1.f) {
				//レベル操作
				WindowSystem::SetMsg(y_r(0), y_r(1080) - y_r(36), y_r(0), y_r(1080), y_r(36), FontHandle::FontXCenter::LEFT, White, Black, "Level");
				WindowSystem::SetMsg(y_r(200), y_r(1080) - y_r(48), y_r(200), y_r(1080), y_r(48), FontHandle::FontXCenter::RIGHT, White, Black, "%d", this->m_MyLevel);
				if (WindowSystem::ClickCheckBox(y_r(0), y_r(1080) - y_r(48) - LineHeight, y_r(100), y_r(1080) - y_r(48), true, !m_Window->PosHitCheck(), Red, "DOWN")) {
					this->m_MyLevel--;
				}
				if (WindowSystem::ClickCheckBox(y_r(100), y_r(1080) - y_r(48) - LineHeight, y_r(200), y_r(1080) - y_r(48), true, !m_Window->PosHitCheck(), Green, "UP")) {
					this->m_MyLevel++;
				}
				this->m_MyLevel = std::clamp(this->m_MyLevel, 1, 71);
				//場所ガイド
				{
					int xp = y_r(1440);
					int yp = y_r(820);

					int xs = y_r(320);
					int ys = y_r(180);

					int x_p1 = std::max(this->m_posx * xs / DrawParts->m_DispXSize, -xs / 2);
					int y_p1 = std::max(this->m_posy * ys / DrawParts->m_DispYSize, -ys / 2);
					int x_p2 = std::min(this->m_posxMaxBuffer * xs / DrawParts->m_DispXSize, xs + xs / 2);
					int y_p2 = std::min(this->m_posyMaxBuffer * ys / DrawParts->m_DispYSize, ys + ys / 2);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
					DrawBox(xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Black, TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					DrawBox(xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Green, FALSE);
					DrawBox(xp, yp, xp + xs, yp + ys, Red, FALSE);
				}
				//中央位置回避のための小円
				DrawCircle(DrawParts->m_DispXSize, DrawParts->m_DispYSize, y_r(100), TransColor, TRUE);
				//
			}
		}
	};
};
