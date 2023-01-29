#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int ItemID;

	class ItemList : public ListParent<ItemID> {
		ItemTypeID	m_TypeID{ InvalidID };
		std::vector<MapID>	m_MapID;
		std::vector<std::pair<const ItemList*, std::string>>	m_ChildPartsID;
		std::vector<const ItemList*>							m_ParentPartsID;
		float		m_Recoil{ 0.f };
		float		m_Ergonomics{ 0.f };
		bool		m_isWeaponMod{ false };
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
			if (LEFT == "Recoil") {
				if (RIGHT.find("+") != std::string::npos) {
					m_Recoil = std::stof(RIGHT.substr(RIGHT.find("+") + 1));
				}
				else {
					if (RIGHT == "") {
						int a = 0;
					}
					m_Recoil = std::stof(RIGHT);
				}
				m_isWeaponMod = true;
			}
			if (LEFT == "Ergonomics") {
				if (RIGHT.find("+") != std::string::npos) {
					m_Ergonomics = std::stof(RIGHT.substr(RIGHT.find("+") + 1));
				}
				else {
					if (RIGHT == "") {
						int a = 0;
					}
					m_Ergonomics = std::stof(RIGHT);
				}
				m_isWeaponMod = true;
			}
		}
		void	Load_Sub() noexcept override {
			if (m_TypeID == InvalidID) {
				std::string ErrMes = "Error : Not Setting ItemType in Item ";
				ErrMes += GetName();
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto&	GetTypeID() const noexcept { return m_TypeID; }
		const auto&	GetMapID() const noexcept { return m_MapID; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			int xofs = 0;
			int yofs = LineHeight;
			int yofs2 = yofs;
			{
				yofs += LineHeight;
				if (m_isWeaponMod) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, (m_Recoil < 0.f) ? Green : Red, Black,
						"Recoil:%s%3.1f %%", (m_Recoil > 0.f) ? "+" : "", m_Recoil) + y_r(30)); yofs += LineHeight;
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, (m_Ergonomics >= 0.f) ? Green : Red, Black,
						"Ergonomics:%s%3.1f", (m_Ergonomics > 0.f) ? "+" : "", m_Ergonomics) + y_r(30)); yofs += LineHeight;
				}
				if (m_ChildPartsID.size() > 0 || m_ParentPartsID.size() > 0) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, White, Black, "Mods:") + y_r(30)); yofs += LineHeight;
				}
				int ysize = (int)((float)y_r(80));
				for (const auto& c : m_ChildPartsID) {
					auto* ptr = c.first;
					xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0))) + y_r(30)); yofs += ysize;
				}
				for (const auto& c : m_ParentPartsID) {
					auto* ptr = c;
					xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0))) + y_r(30)); yofs += ysize;
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
				xofs = std::max<int>(xofs, GetIcon().GetXSize());
				yofs2 += std::max(ysize, 64 * 2);
				DrawControl::Instance()->SetAlpha(255);
			}
			else {
				yofs2 += 64 * 2;
			}
			yofs = std::max(yofs, yofs2);
			//
			if (xs) {
				*xs = std::max(*xs, xofs);
			}
			if (ys) {
				*ys = std::max(*ys, yofs - LineHeight);
			}
		}
		void			SetOtherPartsID(const std::vector<ItemList>& itemList) noexcept {
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
			m_ParentPartsID.clear();
			for (const auto& t : itemList) {
				for (auto& c : t.m_ChildPartsID) {
					if (c.first == this) {
						m_ParentPartsID.emplace_back(&t);
					}
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
				t.SetOtherPartsID(m_List);
			}
		}
		~ItemData() noexcept {}
	};

	const int		ItemList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		int xs = xsize;
		int  Xsize = 0;

		int xg = 0;
		if (GetIcon().GetGraph()) {
			xg = (GetIcon().GetXSize() >= GetIcon().GetYSize())
				? (ysize * GetIcon().GetXSize() / GetIcon().GetYSize())
				: (ysize * GetIcon().GetYSize() / GetIcon().GetXSize());
		}

		auto Name = this->GetName();
		{
			if (xsize > 0) {
				while (true) {
					if (count > 0) {
						Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s x%2d", Name.c_str(), count);
					}
					else {
						Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
					}
					if ((xs - xg) < Xsize) {
						Name = Name.substr(0, Name.size() * (xs - xg) / Xsize - 2) + "…";
					}
					else {
						break;
					}
				}
			}
			else {
				if (count > 0) {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s x%2d", Name.c_str(), count);
				}
				else {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
				}
			}
			xs = std::max(xs, Xsize);
		}

		if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ysize, false, Clickactive, defaultcolor, "")) {
			auto sizeXBuf = y_r(800);
			auto sizeYBuf = y_r(0);
			DrawWindow(nullptr, 0, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
			//
			signed long long FreeID = GetID() + 0xFFFF;
			//同じIDの奴いたら消そうぜ
			int Size = (int)WindowMngr->Get().size();
			bool isHit = false;
			for (int i = 0; i < Size; i++) {
				if (WindowMngr->Get()[i]->m_FreeID == FreeID) {
					isHit = true;
					WindowMngr->Get()[i]->SetActiveSwitch(true);
					break;
				}
			}
			if (!isHit) {
				//ウィンドウ追加
				WindowMngr->Add()->Set(xp + xs / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
					ItemData::Instance()->FindPtr((ItemID)(win->m_FreeID - 0xFFFF))->DrawWindow(win, Gray10, win->GetPosX(), win->GetPosY());
				});
			}
		}
		if (count > 0) {
			WindowSystem::SetMsg(xp, yp, xp, yp + ysize, LineHeight * 9 / 10, STR_LEFT, White, Black, "%s x%2d", Name.c_str(), count);
		}
		else {
			WindowSystem::SetMsg(xp, yp, xp, yp + ysize, LineHeight * 9 / 10, STR_LEFT, White, Black, "%s", Name.c_str());
		}
		if (GetIcon().GetGraph()) {
			xp += Xsize;
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			DrawControl::Instance()->SetDrawRotaGraph(GetIcon().GetGraph(), xp + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			Xsize += xg;
		}
		return Xsize;
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