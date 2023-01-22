#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int ItemID;

	class ItemList : public ListParent<ItemID> {
		ItemTypeID	m_TypeID{ InvalidID };
		std::vector<MapID>	m_MapID;
		std::vector<std::pair<const ItemList*,std::string>>	m_ChildPartsID;
	private:
		//追加設定
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override {
			if (LEFT == "Itemtype") {
				m_TypeID = ItemTypeData::Instance()->FindID(RIGHT.c_str());
			}
			if (LEFT == "Map") {
				m_MapID.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
			}
			if (LEFT == "ChildParts") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
							m_ChildPartsID.back().second = a;
						}
					}
				}
				else {
					m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
					m_ChildPartsID.back().second = RIGHT;
				}
			}
		}
		void	Load_Sub() noexcept override {
			if (m_TypeID == InvalidID) {
				std::string ErrMes = "Error : Not Setting ItemType in Item ";
				ErrMes += GetName();
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	public:
		const auto&	GetTypeID() const noexcept { return m_TypeID; }
		const auto&	GetMapID() const noexcept { return m_MapID; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept {
			auto* Windowup = WindowSystem::WindowManager::Instance();
			int xofs = 0;
			int yofs = LineHeight;
			int yofs2 = yofs;
			{
				yofs += LineHeight;
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, White, Black, "Mods:")); yofs += LineHeight;
				int ysize = (int)((float)y_r(80));
				for (const auto& c : m_ChildPartsID) {
					auto* ptr = c.first;
					xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, ysize, 0, defaultcolor, (!Windowup->PosHitCheck(window) && !(xp == 0 && yp == 0)))); yofs += ysize;
				}
			}
			if (GetIcon().GetGraph()) {
				DrawControl::Instance()->SetAlpha(96);
				int ysize = GetIcon().GetYSize();

				float Scale = 1.f;
				float rad = 0.f;

				DrawControl::Instance()->SetDrawRotaGraph(GetIcon().GetGraph(),
					xp + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale),
					yp + LineHeight + ysize / 2, Scale, rad, false);
				yofs2 += std::max(ysize, 64 * 2);
				DrawControl::Instance()->SetAlpha(255);
			}
			else {
				yofs2 += 64 * 2;
			}
			yofs = std::max(yofs, yofs2);
			//
			if (xs) {
				*xs = std::max(*xs, xofs + LineHeight / 10);
			}
			if (ys) {
				*ys = std::max(*ys, yofs - LineHeight);
			}
		}
		void			SetNeedTasktoID(const std::vector<ItemList>& itemList) noexcept {
			for (auto& c : m_ChildPartsID) {
				for (const auto& t : itemList) {
					if (c.second == t.GetName()) {
						c.first = &t;
						break;
					}
				}
				if (c.first == nullptr) {
					std::string ErrMes = "Error : Not Setting ChildPartsID in Item ";
					ErrMes += GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
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
					SetList((Path + d.cFileName + "/").c_str());
				}
			}
			for (auto& t : m_List) {
				t.SetNeedTasktoID(m_List);
			}
		}
		~ItemData() noexcept {}
	};

	const int		ItemList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) const noexcept {
		auto* Windowup = WindowSystem::WindowManager::Instance();
		int xs = xsize;
		{
			auto* Fonts = FontPool::Instance();
			int xSize = Fonts->Get(FontPool::FontType::Nomal_Edge, LineHeight * 9 / 10).GetStringWidth(-1, "%s", this->GetName().c_str()) + y_r(6) + 2;//エッジ分:
			if (GetIcon().GetGraph()) {
				xSize += (ysize * GetIcon().GetXSize() / GetIcon().GetYSize());
			}
			xs = std::max(xs, xSize);
		}

		if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ysize, false, Clickactive, defaultcolor, "")) {
			auto sizeXBuf = y_r(800);
			auto sizeYBuf = y_r(0);
			DrawWindow(nullptr, 0, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
			//
			unsigned long long FreeID = GetID();
			Windowup->Add()->Set(xp + xs / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
				ItemData::Instance()->FindPtr((ItemID)win->m_FreeID)->DrawWindow(win, Gray10, win->GetPosX(), win->GetPosY());
			});
		}

		int  Xsize = 0;
		if (count > 0) {
			Xsize = WindowSystem::SetMsg(xp, yp, xp, yp + ysize, LineHeight * 9 / 10, STR_LEFT, White, Black, "%s x%2d", this->GetName().c_str(), count);
		}
		else {
			Xsize = WindowSystem::SetMsg(xp, yp, xp, yp + ysize, LineHeight * 9 / 10, STR_LEFT, White, Black, "%s", this->GetName().c_str());
		}
		xp += Xsize;
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);

			DrawControl::Instance()->SetDrawRotaGraph(GetIcon().GetGraph(), xp + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			return (int)(Xsize + ysize * GetIcon().GetXSize() / GetIcon().GetYSize());
		}
		else {
			return (int)Xsize;
		}
	}


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