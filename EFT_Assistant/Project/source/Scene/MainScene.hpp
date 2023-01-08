#pragma once
#include"Header.hpp"

#define LineHeight	y_r(48)

namespace FPS_n2 {

	static const unsigned int Red{ GetColor(255, 0, 0) };
	static const unsigned int Red25{ GetColor(192, 0, 0) };
	static const unsigned int Red50{ GetColor(128, 0, 0) };

	//static const unsigned int Blue{ GetColor(50, 50, 255) };
	static const unsigned int Green{ GetColor(43, 255, 91) };
	static const unsigned int White{ GetColor(255, 255, 255) };
	//static const unsigned int Gray10{ GetColor(230, 230, 230) };
	static const unsigned int Gray15{ GetColor(218, 218, 218) };
	static const unsigned int Gray25{ GetColor(192, 192, 192) };
	static const unsigned int Gray50{ GetColor(128, 128, 128) };
	static const unsigned int Gray75{ GetColor(64, 64, 64) };
	static const unsigned int Black{ GetColor(0, 0, 0) };
	//
	typedef int EnemyID;
	typedef int TraderID;
	typedef int MapID;
	typedef int ItemID;
	typedef int TaskID;
	//
	class MapList {
		MapID		m_Map{ -1 };
		std::string		m_MapName;
		std::array<int, 3> m_MapColor{ 0,0,0 };
	public:
		void	Set(const char* FilePath, MapID id) {
			m_Map = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				std::vector<std::string> Args;
				{
					auto L = RIGHT.find("[");
					auto R = RIGHT.find("]");
					if (L != std::string::npos && R != std::string::npos) {
						RIGHT = RIGHT.substr(L + 1);
						while (true) {
							auto div = RIGHT.find(",");
							if (div != std::string::npos) {
								Args.emplace_back(RIGHT.substr(0, div));
								RIGHT = RIGHT.substr(div + 1);
							}
							else {
								Args.emplace_back(RIGHT.substr(0, RIGHT.find("]")));
								break;
							}
						}
					}
				}
				if (LEFT == "Name") {
					m_MapName = RIGHT;
					continue;
				}
				if (LEFT == "Color") {
					m_MapColor[0] = std::stoi(Args[0]);
					m_MapColor[1] = std::stoi(Args[1]);
					m_MapColor[2] = std::stoi(Args[2]);
					continue;
				}
			}
			FileRead_close(mdata);
		}
	public:
		const auto& GetID() const noexcept { return m_Map; }
		const auto& GetName() const noexcept { return m_MapName; }
		const auto& GetColors() const noexcept { return m_MapColor; }
	};
	static std::vector<MapList>	g_MapList;
	void	SetMapList() {
		auto data_t = GetFileNamesInDirectory("data/map/");
		std::vector<std::string> DirNames;
		for (auto& d : data_t) {
			if (d.cFileName[0] != '.') {
				DirNames.emplace_back(d.cFileName);
			}
		}
		g_MapList.resize(DirNames.size());
		for (auto& d : DirNames) {
			int ID = (int)(&d - &DirNames.front());
			g_MapList[ID].Set(("data/map/" + d).c_str(), (MapID)ID);
		}
		DirNames.clear();
	}
	const MapID	FindMapID(const char* name) {
		for (const auto&t : g_MapList) {
			if (t.GetName() == name) {
				return t.GetID();
			}
		}
		return (MapID)-1;
	}
	const MapList&	FindMap(MapID ID) {
		for (const auto&t : g_MapList) {
			if (t.GetID() == ID) {
				return t;
			}
		}
		return g_MapList[0];
	}
	//
	class ItemList {
		ItemID		m_Item{ -1 };
		std::string		m_ItemName;
	public:
		void	Set(const char* FilePath, ItemID id) {
			m_Item = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				std::vector<std::string> Args;
				{
					auto L = RIGHT.find("[");
					auto R = RIGHT.find("]");
					if (L != std::string::npos && R != std::string::npos) {
						RIGHT = RIGHT.substr(L + 1);
						while (true) {
							auto div = RIGHT.find(",");
							if (div != std::string::npos) {
								Args.emplace_back(RIGHT.substr(0, div));
								RIGHT = RIGHT.substr(div + 1);
							}
							else {
								Args.emplace_back(RIGHT.substr(0, RIGHT.find("]")));
								break;
							}
						}
					}
				}
				if (LEFT == "Name") {
					m_ItemName = RIGHT;
					continue;
				}
				if (LEFT == "Itemtype") {
					//m_ItemName = RIGHT;
					continue;
				}
			}
			FileRead_close(mdata);
		}
	public:
		const auto& GetID() const noexcept { return m_Item; }
		const auto& GetName() const noexcept { return m_ItemName; }
	};
	static std::vector<ItemList>	g_ItemList;
	void	SetItemList() {
		auto data_t = GetFileNamesInDirectory("data/item/");
		std::vector<std::string> DirNames;
		for (auto& d : data_t) {
			if (d.cFileName[0] != '.') {
				DirNames.emplace_back(d.cFileName);
			}
		}
		g_ItemList.resize(DirNames.size());
		for (auto& d : DirNames) {
			int ID = (int)(&d - &DirNames.front());
			g_ItemList[ID].Set(("data/item/" + d).c_str(), (ItemID)ID);
		}
		DirNames.clear();
	}
	const ItemID	FindItemID(const char* name) {
		for (const auto&t : g_ItemList) {
			if (t.GetName() == name) {
				return t.GetID();
			}
		}
		return (ItemID)-1;
	}
	const ItemList&	FindItem(ItemID ID) {
		return g_ItemList[0];
		for (const auto&t : g_ItemList) {
			if (t.GetID() == ID) {
				return t;
			}
		}
		return g_ItemList[0];
	}
	//
	class EnemyList {
		EnemyID		m_Enemy{ -1 };
		std::string		m_EnemyName;
		std::array<int, 3> m_EnemyColor{ 0,0,0 };
	public:
		void	Set(const char* FilePath, EnemyID id) {
			m_Enemy = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				std::vector<std::string> Args;
				{
					auto L = RIGHT.find("[");
					auto R = RIGHT.find("]");
					if (L != std::string::npos && R != std::string::npos) {
						RIGHT = RIGHT.substr(L + 1);
						while (true) {
							auto div = RIGHT.find(",");
							if (div != std::string::npos) {
								Args.emplace_back(RIGHT.substr(0, div));
								RIGHT = RIGHT.substr(div + 1);
							}
							else {
								Args.emplace_back(RIGHT.substr(0, RIGHT.find("]")));
								break;
							}
						}
					}
				}
				if (LEFT == "Name") {
					m_EnemyName = RIGHT;
					continue;
				}
				if (LEFT == "Color") {
					m_EnemyColor[0] = std::stoi(Args[0]);
					m_EnemyColor[1] = std::stoi(Args[1]);
					m_EnemyColor[2] = std::stoi(Args[2]);
					continue;
				}
			}
			FileRead_close(mdata);
		}
	public:
		const auto& GetID() const noexcept { return m_Enemy; }
		const auto& GetName() const noexcept { return m_EnemyName; }
		const auto& GetColors() const noexcept { return m_EnemyColor; }
	};
	static std::vector<EnemyList>	g_EnemyList;
	void	SetEnemyList() {
		auto data_t = GetFileNamesInDirectory("data/Enemy/");
		std::vector<std::string> DirNames;
		for (auto& d : data_t) {
			if (d.cFileName[0] != '.') {
				DirNames.emplace_back(d.cFileName);
			}
		}
		g_EnemyList.resize(DirNames.size());
		for (auto& d : DirNames) {
			int ID = (int)(&d - &DirNames.front());
			g_EnemyList[ID].Set(("data/enemy/" + d).c_str(), (EnemyID)ID);
		}
		DirNames.clear();
	}
	const EnemyID	FindEnemyID(const char* name) {
		for (const auto&t : g_EnemyList) {
			if (t.GetName() == name) {
				return t.GetID();
			}
		}
		return (EnemyID)-1;
	}
	const EnemyList&	FindEnemy(EnemyID ID) {
		for (const auto&t : g_EnemyList) {
			if (t.GetID() == ID) {
				return t;
			}
		}
		return g_EnemyList[0];
	}
	//
	class TraderList {
		TraderID		m_Trader{ -1 };
		std::string		m_TraderName;
		std::array<int, 3> m_TraderColor{ 0,0,0 };

		std::array<int, 4> m_TraderLv{ 0,0,0,0 };
		std::array<float, 4> m_TraderRep{ 0,0,0,0 };
	public:
		void	Set(const char* FilePath, TraderID id) {
			m_Trader = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				std::vector<std::string> Args;
				{
					auto L = RIGHT.find("[");
					auto R = RIGHT.find("]");
					if (L != std::string::npos && R != std::string::npos) {
						RIGHT = RIGHT.substr(L + 1);
						while (true) {
							auto div = RIGHT.find(",");
							if (div != std::string::npos) {
								Args.emplace_back(RIGHT.substr(0, div));
								RIGHT = RIGHT.substr(div + 1);
							}
							else {
								Args.emplace_back(RIGHT.substr(0, RIGHT.find("]")));
								break;
							}
						}
					}
				}
				if (LEFT == "Name") {
					m_TraderName = RIGHT;
					continue;
				}
				if (LEFT == "Color") {
					m_TraderColor[0] = std::stoi(Args[0]);
					m_TraderColor[1] = std::stoi(Args[1]);
					m_TraderColor[2] = std::stoi(Args[2]);
					continue;
				}
				{
					bool isHit = false;
					for (int i = 0; i < 4; i++) {
						if (i == 0) {
							m_TraderLv[i] = 1;
							m_TraderRep[i] = 0.f;
						}
						else {
							if (LEFT == "LL" + std::to_string(i + 1) + "Rep") {
								m_TraderRep[i] = std::stof(RIGHT);
								isHit = true;
								break;
							}
							if (LEFT == "LL" + std::to_string(i + 1) + "Lv") {
								m_TraderLv[i] = std::stoi(RIGHT);
								isHit = true;
								break;
							}
						}
					}
					if (isHit) { continue; }
				}
			}
			FileRead_close(mdata);
		}
	public:
		const auto& GetID() const noexcept { return m_Trader; }
		const auto& GetName() const noexcept { return m_TraderName; }
		const auto& GetColors() const noexcept { return m_TraderColor; }
	};
	static std::vector<TraderList>	g_TraderList;
	static std::vector<float>	g_TraderRep;
	void	SetTraderList() {
		auto data_t = GetFileNamesInDirectory("data/trader/");
		std::vector<std::string> DirNames;
		for (auto& d : data_t) {
			if (d.cFileName[0] != '.') {
				DirNames.emplace_back(d.cFileName);
			}
		}
		g_TraderList.resize(DirNames.size());
		g_TraderRep.resize(DirNames.size());
		for (auto& d : DirNames) {
			int ID = (int)(&d - &DirNames.front());
			g_TraderList[ID].Set(("data/trader/" + d).c_str(), (TraderID)ID);
		}
		DirNames.clear();
	}
	const TraderID	FindTraderID(const char* name) {
		for (const auto&t : g_TraderList) {
			if (t.GetName()==name) {
				return t.GetID();
			}
		}
		return (TraderID)-1;
	}
	const TraderList&	FindTrader(TraderID ID) {
		for (const auto&t : g_TraderList) {
			if (t.GetID() == ID) {
				return t;
			}
		}
		return g_TraderList[0];
	}
	float*	FindTraderRep(TraderID ID) {
		for (const auto&t : g_TraderList) {
			if (t.GetID() == ID) {
				return &g_TraderRep[&t-&g_TraderList.front()];
			}
		}
		return nullptr;
	}
	//
	class TaskData {
		struct ItemGetData {
			ItemID				m_ItemID{ -1 };
			int					m_Count{ 0 };
		};
		struct TaskNeedData {
			int						m_Level{ -1 };
			int						m_LL{ -1 };
			std::string				m_task;
			TaskID					m_taskID{ -1 };
			std::vector<ItemGetData>m_Item;
		};
		struct TaskWorkData {
			std::vector<MapID>		m_Map;
			struct EnemyKill {
				EnemyID				m_EnemyID{ -1 };
				MapID				m_MapID{ -1 };
				int					m_KillCount{ 0 };
			};
			std::vector<EnemyKill>	m_Kill;
			std::vector<ItemGetData>m_FiR_Item;
			std::vector<ItemGetData>m_NotFiR_Item;
			std::vector<std::string>m_ElseMsg;
		};
		struct TaskRewardData {
			struct LLAdd {
				TraderID			m_TraderID{ -1 };
				float				Add{ 0.f };
			};
			std::vector<LLAdd>		m_LLAdd;
			std::vector<ItemGetData>m_Item;
		};
	private:
		TaskID			m_ID;
		std::string		m_Name;
		TraderID		m_Trader{ -1 };
		TaskNeedData	m_TaskNeedData;
		TaskWorkData	m_TaskWorkData;
		TaskRewardData	m_TaskRewardData;
	public:
		const auto& GetID() const noexcept { return m_ID; }
		const auto& GetName() const noexcept { return m_Name; }
		const auto& GetTrader() const noexcept { return m_Trader; }
		const auto& GetTaskNeedData() const noexcept { return m_TaskNeedData; }
		const auto& GetTaskWorkData() const noexcept { return m_TaskWorkData; }
		const auto& GetTaskRewardData() const noexcept { return m_TaskRewardData; }
	public:
		void		Set(const char* FilePath,TaskID id) {
			m_ID = id;
			int mdata = FileRead_open(FilePath, FALSE);

			auto SetItem = [&](std::vector<ItemGetData>* Data,const std::string& mes) {
				auto L = mes.rfind("x");
				if (L != std::string::npos) {
					ItemGetData tmp;
					std::string Left = mes.substr(0, L);
					tmp.m_ItemID = FindItemID(Left.c_str());
					tmp.m_Count = std::stoi(mes.substr(L + 1));
					Data->emplace_back(tmp);
				}
				else {
					//int a = 0;
				}
			};
			auto SetKill = [&](const std::string& mes) {
				auto L = mes.rfind("x");
				if (L != std::string::npos) {
					TaskWorkData::EnemyKill tmp;
					std::string Left = mes.substr(0, L);
					auto MP = mes.rfind("-");
					if (MP != std::string::npos) {
						tmp.m_MapID = FindMapID(mes.substr(0, MP).c_str());
						tmp.m_EnemyID = FindEnemyID(mes.substr(MP + 1, L - (MP + 1)).c_str());
					}
					else {
						tmp.m_EnemyID = FindEnemyID(Left.c_str());
					}
					tmp.m_KillCount = std::stoi(mes.substr(L + 1));
					m_TaskWorkData.m_Kill.emplace_back(tmp);
				}
				else {
					//int a = 0;
				}
			};

			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				std::vector<std::string> Args;
				bool isMulti = false;
				{
					auto RIGHT2 = RIGHT;
					auto L = RIGHT2.find("[");
					auto R = RIGHT2.find("]");
					if (L != std::string::npos && R != std::string::npos) {
						isMulti = true;
						RIGHT2 = RIGHT2.substr(L + 1);
						while (true) {
							auto div = RIGHT2.find(",");
							if (div != std::string::npos) {
								Args.emplace_back(RIGHT2.substr(0, div));
								RIGHT2 = RIGHT2.substr(div + 1);
							}
							else {
								Args.emplace_back(RIGHT2.substr(0, RIGHT2.find("]")));
								break;
							}
						}
					}
				}

				if (LEFT == "Name") {
					m_Name = RIGHT;
					continue;
				}
				if (LEFT == "Trader") {
					m_Trader = FindTraderID(RIGHT.c_str());
					continue;
				}
				//Need
				if (LEFT == "NeedTask") {
					m_TaskNeedData.m_task = RIGHT;
					continue;
				}
				if (LEFT == "NeedLevel") {
					m_TaskNeedData.m_Level = std::stoi(RIGHT);
					continue;
				}
				if (LEFT == "NeedLL") {
					m_TaskNeedData.m_LL = std::stoi(RIGHT);
					continue;
				}
				if (LEFT == "NeedItem") {
					if (isMulti) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_TaskNeedData.m_Item, a);
							}
						}
					}
					else {
						SetItem(&m_TaskNeedData.m_Item, RIGHT);
					}
					continue;
				}
				//Work
				if (LEFT == "Task_Map") {//ロケーション追加
					m_TaskWorkData.m_Map.emplace_back(FindMapID(RIGHT.c_str()));
					continue;
				}
				if (LEFT == "Task_Kill") {
					if (isMulti) {
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
					continue;
				}
				if (LEFT == "Task_FiR_HandOver") {
					if (isMulti) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_TaskWorkData.m_FiR_Item, a);
							}
						}
					}
					else {
						SetItem(&m_TaskWorkData.m_FiR_Item, RIGHT);
					}
					continue;
				}
				if (LEFT == "Task_NotFiR_HandOver") {
					if (isMulti) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_TaskWorkData.m_NotFiR_Item, a);
							}
						}
					}
					else {
						SetItem(&m_TaskWorkData.m_NotFiR_Item, RIGHT);
					}
					continue;
				}
				if (LEFT == "Task_Else") {//特殊　メッセージ
					m_TaskWorkData.m_ElseMsg.emplace_back(RIGHT);
					continue;
				}
				//Reward
				if (LEFT == "Reward_Rep") {
					auto plus = RIGHT.find("+");
					auto minus = RIGHT.find("-");
					if (plus != std::string::npos) {
						TaskRewardData::LLAdd tmp;
						tmp.m_TraderID = FindTraderID(RIGHT.substr(0, plus).c_str());
						tmp.Add = std::stof(RIGHT.substr(plus + 1));
						m_TaskRewardData.m_LLAdd.emplace_back(tmp);
					}
					if (minus != std::string::npos) {
						TaskRewardData::LLAdd tmp;
						tmp.m_TraderID = FindTraderID(RIGHT.substr(0, minus).c_str());
						tmp.Add = std::stof(RIGHT.substr(minus));
						m_TaskRewardData.m_LLAdd.emplace_back(tmp);
					}
					continue;
				}
				if (LEFT == "Reward_Item") {
					if (isMulti) {
						for (auto&a : Args) {
							if (a == "or") {

							}
							else {
								SetItem(&m_TaskRewardData.m_Item, a);
							}
						}
					}
					else {
						SetItem(&m_TaskRewardData.m_Item, RIGHT);
					}
					continue;
				}
			}
			FileRead_close(mdata);
		}
		void		SetNeedTasktoID(const std::vector<TaskData>& taskList) {
			for (const auto& t : taskList) {
				if (m_TaskNeedData.m_task == t.m_Name) {
					m_TaskNeedData.m_taskID = t.m_ID;
					break;
				}
			}
		}

		void		DrawWindow(int xp, int yp, int xs, int* ys) {
			int suby = LineHeight;
			int sizy = LineHeight * 7 / 10;
			//
			if (m_TaskNeedData.m_Item.size() > 0) {
				WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, White, Black, "必要アイテム");				suby += sizy;
				for (auto& LL : m_TaskNeedData.m_Item) {
					auto& map = FindItem(LL.m_ItemID);
					WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT,
						White, Black, "└%s x%2d", map.GetName().c_str(), LL.m_Count);suby += sizy;
				}
				suby += sizy;
			}
			//
			if (m_TaskWorkData.m_Map.size() > 0) {
				WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, White, Black, "マップ指定");suby += sizy;
				for (auto& LL : m_TaskWorkData.m_Map) {
					auto& map = FindMap(LL);
					WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, GetColor(map.GetColors()[0], map.GetColors()[1], map.GetColors()[2]), Black, "└%s", map.GetName().c_str());					suby += sizy;
				}
			}
			else {
				WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, White, Black, "マップ指定なし");suby += sizy;
			}
			if (m_TaskWorkData.m_Kill.size() > 0) {
				WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, White, Black, "敵をキル");suby += sizy;
				for (auto& LL : m_TaskWorkData.m_Kill) {
					auto& eny = FindEnemy(LL.m_EnemyID);
					WindowSystem::SetMsg(xp, yp + suby, xp, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT,
						GetColor(eny.GetColors()[0], eny.GetColors()[1], eny.GetColors()[2]), Black, "└%s x%2d", eny.GetName().c_str(), LL.m_KillCount);
					if (LL.m_MapID != (MapID)-1) {
						auto& map = FindMap(LL.m_MapID);
						WindowSystem::SetMsg(xp + y_r(250), yp + suby, xp + y_r(250), yp + sizy + suby, LineHeight * 8 / 10, FontHandle::FontXCenter::LEFT,
							GetColor(map.GetColors()[0], map.GetColors()[1], map.GetColors()[2]), Black, " in %s", map.GetName().c_str());
					}
					suby += sizy;
				}
			}
			//
			//
			if (m_TaskWorkData.m_ElseMsg.size() > 0) {
				WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, White, Black, "メモ:");suby += sizy;
				for (auto& m : m_TaskWorkData.m_ElseMsg) {
					WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + sizy + suby, sizy, FontHandle::FontXCenter::LEFT, White, Black, m.c_str());suby += sizy;
				}
			}
			*ys = std::max(*ys, suby + LineHeight / 10);
		}
	};
	//
	class MAINLOOP : public TEMPSCENE {
	private:
		std::unique_ptr<WindowSystem::WindowManager>	m_Window;
		std::vector<GraphHandle>									m_TaskGraph;
		std::vector<std::shared_ptr<WindowSystem::WindowControl>>	m_TaskPtr;
		std::vector<TaskData>							m_TaskList;
		int												m_MyLevel{ 1 };

		int start_mouseX = 0; //ドラッグ前のマウスのX、Y座標
		int start_mouseY = 0;
		float Scale{ 0.6f };

		int m_posx = y_r(100);
		int m_posy = y_r(100);
		int m_mposBaseX = y_r(100);
		int m_mposBaseY = y_r(100);
		struct Rect2D {
			int XStart{ 0 };
			int YStart{ 0 };
			int XEnd{ 0 };
			int YEnd{ 0 };

			bool IsHit(const Rect2D& target) {
				return (
					(
					(this->XStart >= target.XStart && this->XStart < target.XEnd) ||
						(target.XStart > this->XStart && target.XStart <= this->XEnd)
						) &&
						(
					(this->YStart >= target.YStart && this->YStart < target.YEnd) ||
							(target.YStart > this->YStart && target.YStart <= this->YEnd)
							));
			}
		};
		std::vector<Rect2D>	m_TaskRect;

		int m_posMaxBufx = y_r(0);
		int m_posMaxBufy = y_r(0);

		bool ispulldown{ false };
		float pulldown = 1.f;
	private:
		void DrawChildTaskClickBox(TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) {
			for (auto& tasks : this->m_TaskList) {
				if (ParentID == (TaskID)-1) {
					if (true) {
						for (auto&t : g_TraderRep) {
							t = 0.f;
						}
					}
					else {
						for (auto&t : g_TraderRep) {
							t = 0.2f;
						}
					}
				}
				if (tasks.GetTaskNeedData().m_taskID == ParentID) {
					auto& trader = FindTrader(tasks.GetTrader());
					auto parentCanDo_t = parentCanDo;
					auto color = GetColor(trader.GetColors()[0], trader.GetColors()[1], trader.GetColors()[2]);
					//信頼度アップダウンを対応
					std::vector<float> PrevRep;
					PrevRep.resize(tasks.GetTaskRewardData().m_LLAdd.size());
					for (auto& LL : tasks.GetTaskRewardData().m_LLAdd) {
						float* traderRep = FindTraderRep(LL.m_TraderID);
						PrevRep[&LL - &tasks.GetTaskRewardData().m_LLAdd.front()] = *traderRep;
						*traderRep += LL.Add;
					}
					//信頼度チェック
					if (
						(
						(this->m_MyLevel < tasks.GetTaskNeedData().m_Level)
							) || !parentCanDo) {
						color = GetColor(
							std::clamp(trader.GetColors()[0] - 100, 0, 255),
							std::clamp(trader.GetColors()[1] - 100, 0, 255),
							std::clamp(trader.GetColors()[2] - 100, 0, 255));
						parentCanDo_t = false;
					}
					if (ParentID != (TaskID)-1) {
						DrawLine(start_x, start_y, xp, yp + ys / 2, Red, (int)(5.f * Scale));
					}
					if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ys, false, !m_Window->PosHitCheck(), color, tasks.GetName())) {
						int xadd = y_r(240);
						//
						auto sizeXBuf = xs + xadd;
						auto sizeYBuf = y_r(480);
						tasks.DrawWindow(0, 0, sizeXBuf, &sizeYBuf);//試しにサイズ計測
						m_TaskGraph.emplace_back(GraphHandle::Make(sizeXBuf, sizeYBuf, true));
						m_TaskGraph.back().SetDraw_Screen();
						{
							tasks.DrawWindow(0, 0, sizeXBuf, &sizeYBuf);
						}
						SetDrawScreen(DX_SCREEN_BACK);
						//
						m_TaskPtr.emplace_back(this->m_Window->Add());
						m_TaskPtr.back()->Set(xp - xadd / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, [&](WindowSystem::WindowControl* win) {
							for (auto& t : m_TaskPtr) {
								if (t.get() == win) {
									m_TaskGraph[&t-&m_TaskPtr.front()].DrawGraph(win->GetPosX(), win->GetPosY(), true);
									break;
								}
							}
						});
					}
					int suby = ys;
					for (auto& LL : tasks.GetTaskRewardData().m_LLAdd) {
						auto& trader2 = FindTrader(LL.m_TraderID);
						float* traderRep = FindTraderRep(LL.m_TraderID);
						WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + ys + suby, ys, FontHandle::FontXCenter::LEFT,
							GetColor(trader2.GetColors()[0], trader2.GetColors()[1], trader2.GetColors()[2])
							, Black, "[%4.2f->%4.2f]", PrevRep[&LL - &tasks.GetTaskRewardData().m_LLAdd.front()], *traderRep);
						suby += ys;
					}
					m_posMaxBufx = std::max(m_posMaxBufx, xp + xs);
					m_posMaxBufy = std::max(m_posMaxBufy, yp + ys + suby);
					{
						Rect2D tmp;
						tmp.XStart = xp;
						tmp.YStart = yp;
						tmp.XEnd = xp + xs;
						tmp.YEnd = yp + suby;
						m_TaskRect.emplace_back(tmp);
					}
					Rect2D P_Next;
					P_Next.XStart = xp + (xs + (int)((float)y_r(50) * Scale));
					P_Next.YStart = yp;
					P_Next.XEnd = P_Next.XStart + xs;
					P_Next.YEnd = P_Next.YStart + suby;
					//xs, ys
					//被ってたら下に下げる
					while (true) {
						bool isHit = false;
						for (auto&r : m_TaskRect) {
							if (r.IsHit(P_Next)) {
								isHit = true;
								P_Next.YStart += ys;
								P_Next.YEnd = P_Next.YStart + suby;
								break;
							}
						}
						if (!isHit) { break; }
					}
					DrawChildTaskClickBox(tasks.GetID(), xp + xs, yp + ys / 2, P_Next.XStart, P_Next.YStart, xs, ys, parentCanDo_t);
					//親なのでいったん信頼度を戻す
					for (auto& LL : tasks.GetTaskRewardData().m_LLAdd) {
						float* traderRep = FindTraderRep(LL.m_TraderID);
						*traderRep = PrevRep[&LL - &tasks.GetTaskRewardData().m_LLAdd.front()];
					}
					//
					if (ParentID == (TaskID)-1) {
						yp += (ys + (int)((float)y_r(400) * Scale));
					}
					else {
						yp += (ys + suby);
					}
				}
			}
		}
	public:
		void Load_Sub(void) noexcept override {
		}

		void Set_Sub(void) noexcept override {
			//
			InputControl::Create();
			m_Window = std::make_unique<WindowSystem::WindowManager>();
			//
			SetItemList();
			SetEnemyList();
			SetTraderList();
			SetMapList();
			//
			{
				auto data_t = GetFileNamesInDirectory("data/task/");
				std::vector<std::string> DirNames;
				for (auto& d : data_t) {
					if (d.cFileName[0] != '.') {
						DirNames.emplace_back(d.cFileName);
					}
				}
				this->m_TaskList.resize(DirNames.size());
				for (auto& d : DirNames) {
					int ID = (int)(&d - &DirNames.front());
					this->m_TaskList[ID].Set(("data/task/" + d).c_str(), (TaskID)ID);
				}
				DirNames.clear();
				for (auto& t : this->m_TaskList) {
					t.SetNeedTasktoID(this->m_TaskList);
				}
			}
			//
		}

		bool Update_Sub(void) noexcept override {
			bool isend = false;
			//FirstDoing
			if (GetIsFirstLoop()) {
				SetMousePoint(DXDraw::Instance()->m_DispXSize / 2, DXDraw::Instance()->m_DispYSize / 2);
			}
			auto* Input = InputControl::Instance();
			Input->Execute();

			if (GetIsFirstLoop()) {
				SetWindowPosition(0, 0);
			}
			//ドラッグ開始時の処理
			if (Input->GetMiddleClick().press()) {
				if (!Input->GetMiddleClick().trigger()) {
					int start_windowX = 0, start_windowY = 0;
					GetWindowPosition(&start_windowX, &start_windowY);					//ウィンドウの位置を格納
					SetWindowPosition(start_windowX + Input->GetMouseX() - start_mouseX, start_windowY + Input->GetMouseY() - start_mouseY);	//マウス位置の差を算出し、ウィンドウを動かす
					Input->SetMouse();
				}
				start_mouseX = Input->GetMouseX();									//ドラッグ前のマウス座標格納
				start_mouseY = Input->GetMouseY();

				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			//
			if (Input->GetWheelAdd() != 0) {
				auto PrevScale = this->Scale;
				this->Scale = std::clamp(this->Scale + (float)Input->GetWheelAdd() / 10.f, 0.1f, 2.f);
				this->m_posx -= (int)((float)(Input->GetMouseX() - this->m_posx) * (this->Scale - PrevScale) / this->Scale);
				this->m_posx -= (int)((float)(Input->GetMouseY() - this->m_posx) * (this->Scale - PrevScale) / this->Scale);
			}
			if (Input->GetRightClick().press()) {
				this->m_posx = Input->GetMouseX() - this->m_mposBaseX;
				this->m_posy = Input->GetMouseY() - this->m_mposBaseY;

				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			else {
				this->m_mposBaseX = Input->GetMouseX() - this->m_posx;
				this->m_mposBaseY = Input->GetMouseY() - this->m_posy;
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
			return !isend;
		}
		void Dispose_Sub(void) noexcept override {
			m_Window.reset();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}

		//UI表示
		void DrawUI_Base_Sub(void) noexcept  override {
			SetDrawMode(DX_DRAWMODE_NEAREST);
			auto* Input = InputControl::Instance();
			auto* DrawParts = DXDraw::Instance();
			DrawBox(0, 0, DrawParts->m_DispXSize, (int)((float)DrawParts->m_DispYSize*pulldown), Gray15, TRUE);
			if (pulldown >= 1.f) {
				//
				//
				{
					m_posMaxBufx = 0;
					m_posMaxBufy = 0;
					int xp = m_posx;
					int yp = m_posy;
					int xs = (int)((float)y_r(520) * Scale);
					int ys = (int)((float)LineHeight * Scale);
					DrawChildTaskClickBox(-1, xp + xs, yp + ys / 2, xp, yp, xs, ys);
					m_TaskRect.clear();
				}
				//
				m_Window->Draw();
			}
			//
			WindowSystem::SetBox(y_r(0), y_r(0), y_r(1920), LineHeight, Gray50);
			WindowSystem::SetMsg(y_r(0), y_r(0), y_r(1920), LineHeight, LineHeight, FontHandle::FontXCenter::MIDDLE, White, Black, "EFT Assistant");
			{
				int xp3 = y_r(1920) - LineHeight;
				int yp3 = y_r(0) + EdgeSize;
				int xp4 = y_r(1920) - EdgeSize;
				int yp4 = y_r(0) + LineHeight - EdgeSize;
				unsigned int color = Red25;

				if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize)) {
					color = Red;
					if (Input->GetLeftClick().trigger()) {
						SetisEnd(true);
					}
					HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
					SetCursor(hCursor);
				}
				DrawBox(xp3 + EdgeSize, yp3 + EdgeSize, xp4 - EdgeSize, yp4 - EdgeSize, color, TRUE);
				WindowSystem::SetMsg(xp3, yp3, xp4, yp4, LineHeight - EdgeSize * 2 - y_r(6), FontHandle::FontXCenter::MIDDLE, White, Black, "X");
			}
			if (WindowSystem::ClickCheckBox(y_r(0), y_r(0), y_r(320), LineHeight, false, true, Gray25, !ispulldown ? "折りたたむ" : "展開")) {
				ispulldown ^= 1;
			}
			Easing(&pulldown, !ispulldown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
			if (pulldown >= 0.95f) { pulldown = 1.f; }
			if (pulldown <= 0.05f) { pulldown = 0.f; }
			if (pulldown >= 1.f) {
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

					int x_p1 = std::max(xp + this->m_posx * xs / DrawParts->m_DispXSize, xp - xs / 2);
					int y_p1 = std::max(yp + this->m_posy * ys / DrawParts->m_DispYSize, yp - ys / 2);
					int x_p2 = std::min(xp + this->m_posMaxBufx * xs / DrawParts->m_DispXSize, xp + xs + xs / 2);
					int y_p2 = std::min(yp + this->m_posMaxBufy * ys / DrawParts->m_DispYSize, yp + ys + ys / 2);

					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
					DrawBox(x_p1, y_p1, x_p2, y_p2, GetColor(0, 0, 0), TRUE);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
					DrawBox(x_p1, y_p1, x_p2, y_p2, GetColor(0, 200, 0), FALSE);
					DrawBox(xp, yp, xp + xs, yp + ys, Red, FALSE);
				}
			}
		}
	};
};

