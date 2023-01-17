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
	static const int ElseSelectID{ -2 };
	static const auto STR_LEFT{ FontHandle::FontXCenter::LEFT };
	static const auto STR_MID{ FontHandle::FontXCenter::MIDDLE };
	static const auto STR_RIGHT{ FontHandle::FontXCenter::RIGHT };
	//
	class DataErrorLog : public SingletonBase<DataErrorLog> {
	private:
		friend class SingletonBase<DataErrorLog>;
	private:
		std::vector<std::string>	m_Mes;
	private:
		DataErrorLog() noexcept {
			setPrintColorDx(GetColor(255, 0, 0), GetColor(218, 218, 218));
		}
		~DataErrorLog() noexcept {}
	public:
		void Draw() noexcept {
			int xp = 0;
			int yp = LineHeight;
			for (auto& m : m_Mes) {
				WindowSystem::SetMsg(xp, yp, xp, yp, LineHeight, STR_LEFT, GetColor(255, 50, 50), GetColor(0, 0, 0), m);
				yp+= LineHeight;
			}
		}
		void AddLog(const char* Mes) noexcept {
			m_Mes.emplace_back(Mes);
		}
	};
	//
	template <class ID>
	class ListParent {
	private:
		ID						m_ID{ InvalidID };
		std::string				m_Name;
		std::array<int, 3>		m_Color{ 0,0,0 };
		GraphHandle				m_Icon;
		int						IconX{ -1 };
		int						IconY{ -1 };
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
		const auto&		GetIcon() const noexcept { return m_Icon; }
	public:
		void			Set(const char* FilePath, ID id, const char* IconPath = nullptr) noexcept {
			m_ID = id;
			int mdata = FileRead_open(FilePath, FALSE);
			while (true) {
				if (FileRead_eof(mdata) != 0) { break; }
				auto ALL = getparams::Getstr(mdata);
				//�R�����g�A�E�g
				if (ALL.find("//") != std::string::npos) {
					ALL = ALL.substr(0, ALL.find("//"));
				}
				//
				if (ALL == "") { continue; }
				auto LEFT = getparams::getleft(ALL);
				auto RIGHT = getparams::getright(ALL);
				auto Args = GetArgs(RIGHT);
				SetCommon(LEFT, RIGHT, Args);
				Set_Sub(LEFT, RIGHT, Args);
			}
			FileRead_close(mdata);
			if (IconPath) {
				m_Icon = GraphHandle::Load(IconPath);
				GraphFilter(m_Icon.get(), DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 1, TRUE, Black, 255);
				this->m_Icon.GetSize(&IconX, &IconY);
			}
		}

		const auto		Draw(int xp, int yp, int ysize, int count) const noexcept {
			int  Xsize = 0;
			if (count > 0) {
				Xsize = WindowSystem::SetMsg(xp, yp, xp, yp + ysize, LineHeight * 7 / 10, STR_LEFT, White, Black, "��%s x%2d", this->GetName().c_str(), count);
			}
			else {
				Xsize = WindowSystem::SetMsg(xp, yp, xp, yp + ysize, LineHeight * 7 / 10, STR_LEFT, White, Black, "��%s", this->GetName().c_str());
			}
			xp += Xsize;

			float Scale = (float)ysize / (float)(std::min(IconX, IconY));
			float rad = (IconX >= IconY) ? deg2rad(0) : deg2rad(90);

			this->m_Icon.DrawRotaGraph(xp + (int)(((float)IconX * std::cos(rad) + (float)IconY * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			return (int)(Xsize + ysize * IconX / IconY);
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
			std::vector<std::pair<std::string, bool>> DirNames;
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Tmp = d.cFileName;
					auto txtpos = Tmp.find(".txt");
					if (txtpos != std::string::npos) {
						std::pair<std::string, bool> tmp;
						tmp.first = Tmp.substr(0, txtpos);
						tmp.second = false;
						DirNames.emplace_back(tmp);
					}
				}
			}
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Tmp = d.cFileName;
					auto txtpos = Tmp.find(".txt");
					if (txtpos == std::string::npos) {
						auto pngpos = Tmp.find(".png");
						if (pngpos != std::string::npos) {
							for (auto& d2 : DirNames) {
								if (d2.first == Tmp.substr(0, pngpos)) {
									d2.second = true;
									break;
								}
							}
						}
					}
				}
			}
			int baseIndex = (int)m_List.size();
			m_List.resize(baseIndex + DirNames.size());
			for (auto& d : DirNames) {
				int index = (int)(&d - &DirNames.front()) + baseIndex;
				m_List[index].Set((DirPath + d.first + ".txt").c_str(), (ID)index, d.second ? (DirPath + d.first + ".png").c_str() : nullptr);
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
			std::string ErrMes = "Error : Not Find Item : ";
			ErrMes += name;
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			return InvalidID;
		}
		const List*		FindPtr(ID id) const noexcept {
			for (const auto&t : m_List) {
				if (t.GetID() == id) {
					return &t;
				}
			}
			std::string ErrMes = "Error : Not Find ID :";
			ErrMes += std::to_string(id);
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			return nullptr;
		}
		const auto&		GetList(void) const noexcept { return this->m_List; }
	};
	//
	typedef int MapID;
	class MapList : public ListParent<MapID> {
		//�ǉ��ݒ�
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
	typedef int ItemCategoryID;
	class ItemCategoryList : public ListParent<ItemCategoryID> {
		//�ǉ��ݒ�
		void			Set_Sub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept override {}
	};
	class ItemCategoryData : public SingletonBase<ItemCategoryData>, public DataParent<ItemCategoryID, ItemCategoryList> {
	private:
		friend class SingletonBase<ItemCategoryData>;
	private:
		ItemCategoryData() noexcept {
			SetList("data/itemcategory/");
		}
		~ItemCategoryData() noexcept {}
	};
	//
	typedef int ItemTypeID;
	class ItemTypeList : public ListParent<ItemTypeID> {
		ItemCategoryID	m_CategoryID{ InvalidID };
	private:
		//�ǉ��ݒ�
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "Category") {
				m_CategoryID = ItemCategoryData::Instance()->FindID(RIGHT.c_str());
			}
		}
	public:
		const auto&	GetCategoryID() const noexcept { return m_CategoryID; }
	};
	class ItemTypeData : public SingletonBase<ItemTypeData>, public DataParent<ItemTypeID, ItemTypeList> {
	private:
		friend class SingletonBase<ItemTypeData>;
	private:
		ItemTypeData() noexcept {
			SetList("data/itemtype/");
		}
		~ItemTypeData() noexcept {}
	};
	//
	typedef int ItemID;
	class ItemList : public ListParent<ItemID> {
		ItemTypeID	m_ID{ InvalidID };
		std::vector<MapID>	m_MapID;
	private:
		//�ǉ��ݒ�
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "Itemtype") {
				m_ID = ItemTypeData::Instance()->FindID(RIGHT.c_str());
			}
			if (LEFT == "Map") {
				m_MapID.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
			}
		}
	public:
		const auto&	GetTypeID() const noexcept { return m_ID; }
		const auto&	GetMapID() const noexcept { return m_MapID; }
	};
	class ItemData : public SingletonBase<ItemData>, public DataParent<ItemID, ItemList> {
	private:
		friend class SingletonBase<ItemData>;
	private:
		ItemData() noexcept {
			std::string Path = "data/item/";
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					SetList((Path + d.cFileName+"/").c_str());
				}
			}
		}
		~ItemData() noexcept {}
	};
	//
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
	//
	typedef int EnemyID;
	class EnemyList : public ListParent<EnemyID> {
		//�ǉ��ݒ�
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
		//�ǉ��ݒ�
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
				if (LEFT == "Task_Map") {//���P�[�V�����ǉ�
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
				else if (LEFT == "Task_Else") {//����@���b�Z�[�W
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
		void			SetNeedTasktoID(const std::vector<TaskList>& taskList) noexcept {
			m_TaskNeedData.SetNeedTasktoID(taskList);
		}
		void			DrawWindow(int xp, int yp, int *xs, int* ys) const noexcept {
			int xofs = 0;
			int yofs = LineHeight;
			int sizy = LineHeight * 7 / 10;
			//�K�v
			{
				auto* trader = TraderData::Instance()->FindPtr(GetTrader());
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "�g���[�_�[:%s Lv %d", trader->GetName().c_str(), std::max(m_TaskNeedData.GetLL(), 1))); yofs += sizy;
			}
			if (m_TaskNeedData.GetLevel() >= 1) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "�K�v���x��:%d", m_TaskNeedData.GetLevel())); yofs += sizy;
			}
			if (m_TaskNeedData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "�K�v�A�C�e��")); yofs += sizy;
				for (const auto& LL : m_TaskNeedData.GetItem()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp, yp + yofs, total_size, LL.GetCount()));
					yofs += total_size;
				}
			}
			//�^�X�N���e
			if (m_TaskWorkData.GetMap().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "�}�b�v�w��")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetMap()) {
					auto* ptr = MapData::Instance()->FindPtr(LL);
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, ptr->GetColors(0), Black, "��%s", ptr->GetName().c_str())); yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetKill().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "�G���L��")); yofs += sizy;
				for (auto& LL : m_TaskWorkData.GetKill()) {
					auto* eny = EnemyData::Instance()->FindPtr(LL.GetEnemyID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, eny->GetColors(0), Black, "��%s x%2d", eny->GetName().c_str(), LL.GetKillCount()));
					if (LL.GetMapID() != InvalidID) {
						auto* ptr = MapData::Instance()->FindPtr(LL.GetMapID());
						xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(250), yp + yofs, xp + y_r(250), yp + sizy + yofs, LineHeight * 8 / 10, STR_LEFT, ptr->GetColors(0), Black, " in %s", ptr->GetName().c_str()));
					}
					yofs += sizy;
				}
			}
			if (m_TaskWorkData.GetFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "Fir�A�C�e���̔[�i")); yofs += sizy;
				for (const auto& LL : m_TaskWorkData.GetFiR_Item()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp, yp + yofs, total_size, LL.GetCount()));
					yofs += total_size;
				}
			}
			if (m_TaskWorkData.GetNotFiR_Item().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "Fir�łȂ��Ă悢�A�C�e���̔[�i")); yofs += sizy;
				for (const auto& LL : m_TaskWorkData.GetNotFiR_Item()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp, yp + yofs, total_size, LL.GetCount()));
					yofs += total_size;
				}
			}
			if (m_TaskWorkData.GetElseMsg().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "����:")); yofs += sizy;
				for (auto& m : m_TaskWorkData.GetElseMsg()) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, m.c_str())); yofs += sizy;
				}
			}
			//
			if (m_TaskRewardData.GetItem().size() > 0) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STR_LEFT, White, Black, "��V�A�C�e��")); yofs += sizy;
				for (const auto& LL : m_TaskRewardData.GetItem()) {
					auto* ptr = ItemData::Instance()->FindPtr(LL.GetID());
					int total_size = y_r(92);
					xofs = std::max(xofs, ptr->Draw(xp, yp + yofs, total_size, LL.GetCount()));
					yofs += total_size;
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
	class BGParent {
	private:
		bool					m_GoNextBG{ false };
	protected:
		virtual void Init_Sub(int *, int *, float*) noexcept {}
		virtual void LateExecute_Sub(void) noexcept {}
		virtual void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept {}
		virtual void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept {}
		virtual void Dispose_Sub(void) noexcept {}
		void TurnOnGoNextBG() noexcept { m_GoNextBG = true; }
	public:
		const auto& IsGoNextBG() const noexcept { return m_GoNextBG; }
	public:
		void Init(int *posx, int *posy, float* Scale) noexcept {
			*posx = y_r(50);
			*posy = LineHeight + y_r(50);
			*Scale = 0.8f;
			m_GoNextBG = false;
			Init_Sub(posx, posy, Scale);
		}
		void LateExecute(void) noexcept {
			LateExecute_Sub();
		}
		void Draw_Back(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float Scale) noexcept {
			Draw_Back_Sub(Windowup, posx, posy, Scale);
		}
		void DrawFront(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float Scale) noexcept {
			DrawFront_Sub(Windowup, posx, posy, Scale);
		}
		void Dispose(void) noexcept {
			Dispose_Sub();
		}
	};
	//
	enum class BGSelect {
		Task,
		Item,
	};
	//�^�C�g��
	class TitleBG :public BGParent {
	private:
		BGSelect m_Select{ (BGSelect)0 };
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Select = (BGSelect)0;
		}
		void LateExecute_Sub(void) noexcept override {
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			int xsize = y_r(420);
			int ysize = y_r(52);
			int ypos = y_r(540);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray25, "�^�X�N")) {
				m_Select = BGSelect::Task;
				TurnOnGoNextBG();
			}
			ypos += y_r(100);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray50, "�n�C�h�A�E�g")) {

			}
			ypos += y_r(100);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray25, "�A�C�e��")) {
				m_Select = BGSelect::Item;
				TurnOnGoNextBG();
			}
			ypos += y_r(100);
			if (WindowSystem::ClickCheckBox(y_r(960) - xsize / 2, ypos - ysize / 2, y_r(960) + xsize / 2, ypos + ysize / 2, false, true, Gray50, "�}�b�v")) {

			}
			ypos += y_r(100);
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		const auto& GetNextSelect() const noexcept { return m_Select; }
	};
	//�^�X�N
	class TaskBG :public BGParent {
	private:
		int															m_MyLevel{ 1 };
		int															m_posxMaxBuffer{ 0 };
		int															m_posyMaxBuffer{ 0 };
		std::vector<Rect2D>											m_TaskRect;
		std::vector<GraphHandle>									m_TaskGraph;
		std::vector<std::shared_ptr<WindowSystem::WindowControl>>	m_TaskPtr;
	private:
		void DrawChildTaskClickBox(std::unique_ptr<WindowSystem::WindowManager>& Windowup, float Scale, TaskID ParentID, int start_x, int start_y, int xp, int yp, int xs, int ys, bool parentCanDo = true) noexcept {
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
					//�M���x�A�b�v�_�E����Ή�
					std::vector<float> PrevRep;
					PrevRep.resize(tasks.GetTaskRewardData().GetLLAdd().size());
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()] = *traderRep;
						*traderRep += LL.GetAdd();
					}
					//�M���x�`�F�b�N
					if (
						(
						(this->m_MyLevel < tasks.GetTaskNeedData().GetLevel())
							) || !parentCanDo) {
						color = trader->GetColors(-100);
						parentCanDo_t = false;
					}
					if (ParentID != InvalidID) {
						DrawLine(start_x, start_y, xp, yp + ys / 2, Red, (int)(5.f * Scale));
					}
					if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ys, false, !Windowup->PosHitCheck(), color, (Scale > 0.5f) ? tasks.GetName() : "")) {
						auto sizeXBuf = y_r(640);
						auto sizeYBuf = y_r(0);
						tasks.DrawWindow(0, 0, &sizeXBuf, &sizeYBuf);//�����ɃT�C�Y�v��
						m_TaskGraph.emplace_back(GraphHandle::Make(sizeXBuf, sizeYBuf, true));
						m_TaskGraph.back().SetDraw_Screen();
						{
							tasks.DrawWindow(0, 0, &sizeXBuf, &sizeYBuf);
						}
						SetDrawScreen(DX_SCREEN_BACK);
						//
						m_TaskPtr.emplace_back(Windowup->Add());
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
						if (Scale > 0.5f) {
							WindowSystem::SetMsg(xp, yp + suby, xp + xs, yp + ys + suby, ys, STR_LEFT, trader2->GetColors(0), Black, "[%4.2f->%4.2f]", PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()], *traderRep);
						}
						suby += ys;
					}
					m_posxMaxBuffer = std::max(m_posxMaxBuffer, xp + xs);
					m_posyMaxBuffer = std::max(m_posyMaxBuffer, yp + ys + suby);
					{
						Rect2D tmp; tmp.Set(xp, yp, xs, suby);
						m_TaskRect.emplace_back(tmp);
					}
					Rect2D P_Next;
					P_Next.Set(xp + (xs + (int)((float)y_r(100) * Scale)), yp, xs, suby);
					//xs, ys
					//����Ă��牺�ɉ�����
					while (true) {
						bool isHit = false;
						for (auto&r : m_TaskRect) {
							if (r.IsHit(P_Next)) {
								isHit = true;
								P_Next.Set(xp + (xs + (int)((float)y_r(50) * Scale)), P_Next.GetPosY() + ys, xs, suby);
								break;
							}
						}
						if (!isHit) { break; }
					}
					DrawChildTaskClickBox(Windowup, Scale, tasks.GetID(), xp + xs, yp + ys / 2, P_Next.GetPosX(), P_Next.GetPosY(), xs, ys, parentCanDo_t);
					//�e�Ȃ̂ł�������M���x��߂�
					for (auto& LL : tasks.GetTaskRewardData().GetLLAdd()) {
						float* traderRep = TraderData::Instance()->FindTraderRep(LL.GetTraderID());
						*traderRep = PrevRep[&LL - &tasks.GetTaskRewardData().GetLLAdd().front()];
					}
					//
					if (ParentID == InvalidID) {
						yp += (ys + (int)((float)y_r(400) * Scale));
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
	private:
		void LateExecute_Sub(void) noexcept override {
			//�E�B���h�E�Ƃ̓���
			for (int i = 0; i < m_TaskPtr.size(); i++) {
				if (m_TaskPtr[i].use_count() <= 1) {
					std::swap(m_TaskGraph[i], m_TaskGraph.back());
					std::swap(m_TaskPtr[i], m_TaskPtr.back());
					m_TaskGraph.back().Dispose();
					m_TaskPtr.back().reset();
					m_TaskGraph.pop_back();
					m_TaskPtr.pop_back();
					i--;
				}
			}
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float Scale) noexcept override {
			int xs = (int)((float)y_r(640) * Scale);
			int ys = (int)((float)LineHeight * Scale);
			DrawChildTaskClickBox(Windowup, Scale, InvalidID, posx + xs, posy + ys / 2, posx, posy, xs, ys);
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>& Windowup, int posx, int posy, float) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			//���x������
			WindowSystem::SetMsg(y_r(0), y_r(1080) - y_r(36), y_r(0), y_r(1080), y_r(36), STR_LEFT, White, Black, "Level");
			WindowSystem::SetMsg(y_r(200), y_r(1080) - y_r(48), y_r(200), y_r(1080), y_r(48), STR_RIGHT, White, Black, "%d", this->m_MyLevel);
			if (WindowSystem::ClickCheckBox(y_r(0), y_r(1080) - y_r(48) - LineHeight, y_r(100), y_r(1080) - y_r(48), true, !Windowup->PosHitCheck(), Red, "DOWN")) {
				this->m_MyLevel--;
			}
			if (WindowSystem::ClickCheckBox(y_r(100), y_r(1080) - y_r(48) - LineHeight, y_r(200), y_r(1080) - y_r(48), true, !Windowup->PosHitCheck(), Green, "UP")) {
				this->m_MyLevel++;
			}
			this->m_MyLevel = std::clamp(this->m_MyLevel, 1, 71);
			//�ꏊ�K�C�h
			{
				int xp = y_r(1440);
				int yp = y_r(820);

				int xs = y_r(320);
				int ys = y_r(180);

				int x_p1 = std::max(posx * xs / DrawParts->m_DispXSize, -xs / 2);
				int y_p1 = std::max(posy * ys / DrawParts->m_DispYSize, -ys / 2);
				int x_p2 = std::min(this->m_posxMaxBuffer * xs / DrawParts->m_DispXSize, xs + xs / 2);
				int y_p2 = std::min(this->m_posyMaxBuffer * ys / DrawParts->m_DispYSize, ys + ys / 2);

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64);
				DrawBox(xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Black, TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawBox(xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Green, FALSE);
				DrawBox(xp, yp, xp + xs, yp + ys, Red, FALSE);
			}
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "�߂�")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
			for (int i = 0; i < m_TaskPtr.size(); i++) {
				m_TaskGraph.back().Dispose();
				m_TaskPtr.back().reset();
				m_TaskGraph.pop_back();
				m_TaskPtr.pop_back();
			}
		}
	};
	//�A�C�e��
	class ItemBG :public BGParent {
	private:
		std::vector<std::pair<int, bool>>	m_ItemIDs;
		ItemTypeID						m_ItemTypeSel{ InvalidID };
		MapID							m_MapTypeSel{ InvalidID };
		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
		float							m_XChild{ 0.f };

		//bool							m_NotUseInRaid{ false };
	private:
		bool MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*)>& ListSet) {
			auto& NowSel = m_ItemIDs.at(Layer);
			NowSel.second = ((NowSel.first != InvalidID) && AndNext);
			if (Layer == 0 || (Layer >= 1 && m_ItemIDs.at(Layer - 1).second)) {
				ListSet(&NowSel);
			}
			else {
				NowSel.first = InvalidID;
			}
		}
		template<class ListChild>
		void MakeList(int xp1, int yp1, const std::vector<ListChild>& List, int*Select, bool isActive, bool isElseSelect, const std::function<bool(const ListChild*)>& CheckLocal) {
			int xsize = y_r(400);
			int ysize = LineHeight;
			int count = 0;
			int yp_t = yp1;
			yp_t += ysize + y_r(5);
			//
			int IDBuf = InvalidID;
			bool NotSelect = (*Select == InvalidID);
			for (const auto& L2 : List) {
				if (!CheckLocal(&L2)) { continue; }
				IDBuf = L2.GetID();
				bool SelectIt = (*Select == IDBuf);
				auto color = NotSelect ? Gray25 : (SelectIt ? Gray10 : Gray50);
				if (WindowSystem::ClickCheckBox(xp1 - (SelectIt ? y_r(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, false, isActive || (!isActive && SelectIt), color, L2.GetName().c_str())) {
					*Select = (isActive) ? IDBuf : InvalidID;
				}
				yp_t += ysize + y_r(5);
				count++;
			}
			if (count > 0 && isElseSelect) {//���̑�
				bool ElseSelect = (*Select == ElseSelectID);
				auto color = ElseSelect ? Gray25 : Gray50;
				if (WindowSystem::ClickCheckBox(xp1 - (ElseSelect ? y_r(25) : 0), yp_t, xp1 + xsize, yp_t + ysize, false, isActive, color, "Else")) {
					*Select = ElseSelectID;
				}
			}
			//�S���I��
			if (count > 1) {
				auto color = NotSelect ? Gray10 : Gray50;
				if (WindowSystem::ClickCheckBox(xp1 - (NotSelect ? y_r(25) : 0), yp1, xp1 + xsize, yp1 + ysize, false, isActive, color, "ALL")) {
					*Select = InvalidID;
				}
			}
			else {
				if (IDBuf != InvalidID) {
					*Select = IDBuf;
				}
			}
			if (count == 0) {
				if (WindowSystem::ClickCheckBox(xp1, yp1, xp1 + xsize, yp1 + ysize, false, false, Gray50, "None")) {}
			}
		};
	private:
		void Init_Sub(int *, int *, float*) noexcept override {
			m_XChild = 0.f;
			m_ItemIDs.clear();
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
		}
		void LateExecute_Sub(void) noexcept override {
		}
		void Draw_Back_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			auto* DrawParts = DXDraw::Instance();

			int xpos = y_r(40);
			int ypos = LineHeight + y_r(10) + LineHeight;
			int ysize = (int)((float)y_r(80));

			int yp = ypos - (int)m_YNow;
			for (auto& L : ItemData::Instance()->GetList()) {//todo
				if (m_ItemIDs[1].first == InvalidID) {
					bool isHit = false;
					for (auto& TL : ItemTypeData::Instance()->GetList()) {
						if (TL.GetCategoryID() == m_ItemIDs[0].first || m_ItemIDs[0].first == InvalidID) {
							isHit = (L.GetTypeID() == TL.GetID());
							if (isHit) { break; }
						}
					}
					if (!isHit) { continue; }
				}
				if (L.GetTypeID() == m_ItemIDs[1].first || m_ItemIDs[1].first == InvalidID) {
					bool ishit = false;
					for (auto& m : L.GetMapID()) {
						if (m == m_ItemIDs[2].first) {
							ishit = true;
							break;
						}
					}
					if (m_ItemIDs[2].first == ElseSelectID) {
						ishit = (L.GetMapID().size() == 0);
					}
					if (ishit || m_ItemIDs[2].first == InvalidID) {
						L.Draw(xpos, yp, ysize, 0);
						yp += ysize;
					}
				}
			}
			yp -= ypos - (int)m_YNow;

			int xs = 400;
			int ScrPosX = y_r(1920 - xs * 3 / 2 - 10) - y_r(80);

			int ScrSizY = (DrawParts->m_DispYSize - (y_r(10) + LineHeight)) - ypos;
			m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp, ScrSizY) / (float)ScrSizY, true);

			m_YNow = std::max(0.f, m_Scroll.GetNowScrollYPer()*(float)(yp - ScrSizY));
		}
		void DrawFront_Sub(std::unique_ptr<WindowSystem::WindowManager>&, int, int, float) noexcept override {
			//
			{
				int xgoal = 0;
				int xsize = y_r(400);
				int xs_add = -(xsize + y_r(50));
				int xp = y_r(1920 - 10) - xsize - (int)m_XChild;
				int yp = LineHeight + y_r(10);
				bool isChild = false;
				int Layer = 0;
				//
				{
					Layer = 0;
					MakeLists(Layer, true, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemCategoryList>(xp + xgoal, yp, ItemCategoryData::Instance()->GetList(), &IDs->first, !IDs->second, false, [&](const auto *) { return true; });
					});
				}
				//
				{
					Layer = 1;
					bool CanGoNext = ((m_ItemIDs.at(Layer).first != InvalidID) && (ItemTypeData::Instance()->FindPtr(m_ItemIDs.at(Layer).first)->GetName() == "Key"));
					MakeLists(Layer, CanGoNext, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemTypeList>(xp + xgoal, yp, ItemTypeData::Instance()->GetList(), &IDs->first, !IDs->second, false, [&](const auto *ptr) { return (ptr->GetCategoryID() == m_ItemIDs.at(Layer - 1).first); });
					});
				}
				//
				{
					Layer = 2;
					MakeLists(Layer, false, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<MapList>(xp + xgoal, yp, MapData::Instance()->GetList(), &IDs->first, !IDs->second, true, [&](const auto *) { return true; });
					});
				}
				//
				if (isChild) {
					xgoal -= xs_add / 2;
				}
				Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
			}
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "�߂�")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
	//
	class MAINLOOP : public TEMPSCENE {
	private:
		std::unique_ptr<WindowSystem::WindowManager>				m_Window;
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		float														m_Scale{ 0.6f };
		bool														m_IsPullDown{ false };
		float														m_PullDown{ 1.f };

		std::shared_ptr<BGParent>									m_BGPtr;
		std::shared_ptr<TitleBG>									m_TitleBG;
		std::shared_ptr<TaskBG>										m_TaskBG;
		std::shared_ptr<ItemBG>										m_ItemBG;
	public:
		void Load_Sub(void) noexcept override {}

		void Set_Sub(void) noexcept override {
			//
			InputControl::Create();
			DataErrorLog::Create();
			m_Window = std::make_unique<WindowSystem::WindowManager>();
			//
			ItemData::Create();
			EnemyData::Create();
			TraderData::Create();
			MapData::Create();
			TaskData::Create();
			//
			m_TitleBG = std::make_shared<TitleBG>();
			m_TaskBG = std::make_shared<TaskBG>();
			m_ItemBG = std::make_shared<ItemBG>();
			//
			m_BGPtr = m_TitleBG;
			//
			m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
		}

		bool Update_Sub(void) noexcept override {
			//FirstDoing
			if (GetIsFirstLoop()) {
				SetWindowPosition(0, 0);
			}
			auto* Input = InputControl::Instance();
			auto mouse_moveX = Input->GetMouseX();							//�h���b�O�O�̃}�E�X���W�i�[
			auto mouse_moveY = Input->GetMouseY();
			Input->Execute();
			mouse_moveX = Input->GetMouseX() - mouse_moveX;
			mouse_moveY = Input->GetMouseY() - mouse_moveY;
			//�h���b�O�J�n���̏���
			if (Input->GetMiddleClick().press()) {
				int start_windowX = 0, start_windowY = 0;
				GetWindowPosition(&start_windowX, &start_windowY);			//�E�B���h�E�̈ʒu���i�[
				start_windowX += mouse_moveX;
				start_windowY += mouse_moveY;
				SetWindowPosition(start_windowX, start_windowY);			//�}�E�X�ʒu�̍����Z�o���A�E�B���h�E�𓮂���
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
					this->m_posy -= (int)((float)(Input->GetMouseY() - this->m_posy) * ScaleChange / this->m_Scale);
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
			m_BGPtr->LateExecute();
			if (m_BGPtr->IsGoNextBG()) {
				m_Window->DeleteAll();
				m_BGPtr->Dispose();
				if (m_BGPtr == m_TitleBG) {
					switch (m_TitleBG->GetNextSelect()) {
					case BGSelect::Task:
						m_BGPtr = m_TaskBG;
						break;
					case BGSelect::Item:
						m_BGPtr = m_ItemBG;
						break;
					default:
						m_BGPtr = m_TaskBG;
						break;
					}
				}
				else {
					m_BGPtr = m_TitleBG;
				}
				m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
			}
			return true;
		}
		void Dispose_Sub(void) noexcept override {
			m_Window.reset();
			m_BGPtr.reset();
			m_TaskBG.reset();
			m_ItemBG.reset();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}

		//UI�\��
		void DrawUI_Base_Sub(void) noexcept  override {
			auto* DrawParts = DXDraw::Instance();

			int Xsize = DrawParts->m_DispXSize;
			int Ysize = DrawParts->m_DispYSize;

			int Xmin = y_r(320);
			int Ymin = LineHeight;

			DrawBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), (int)(Lerp((float)Ymin, (float)Ysize, m_PullDown)), Gray75, TRUE);
			if (m_PullDown >= 1.f) {
				//Back
				m_BGPtr->Draw_Back(this->m_Window, this->m_posx, this->m_posy, this->m_Scale);
				//�E�B���h�E
				m_Window->Draw();
			}
			//�^�C�g���o�b�N
			WindowSystem::SetBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), LineHeight, Gray50);
			//�^�C�g��
			if (m_PullDown >= 1.f) {
				WindowSystem::SetMsg(0, 0, y_r(1920), LineHeight, LineHeight, STR_MID, White, Black, "EFT Assistant");
				WindowSystem::SetMsg(y_r(1280), LineHeight * 3 / 10, y_r(1280), LineHeight, LineHeight * 7 / 10, STR_LEFT, White, Black, "ver %d.%d.%d", 0, 0, 10);
				if (WindowSystem::CloseButton(y_r(1920) - LineHeight, 0)) { SetisEnd(true); }
			}
			//�W�J
			if (WindowSystem::ClickCheckBox(0, 0, Xmin, Ymin, false, true, Gray25, !m_IsPullDown ? "�܂肽����" : "�W�J")) { m_IsPullDown ^= 1; }
			Easing(&m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
			if (m_PullDown >= 0.95f) { m_PullDown = 1.f; }
			if (m_PullDown <= 0.05f) { m_PullDown = 0.f; }
			//
			if (m_PullDown >= 1.f) {
				//Front
				m_BGPtr->DrawFront(this->m_Window, this->m_posx, this->m_posy, this->m_Scale);
				//�����ʒu����̂��߂̏��~
				DrawCircle(Xsize, Ysize, y_r(100), TransColor, TRUE);
			}
			//

			DataErrorLog::Instance()->Draw();
		}
	};
};
