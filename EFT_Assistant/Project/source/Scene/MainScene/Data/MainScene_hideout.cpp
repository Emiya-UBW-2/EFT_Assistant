#include"../../../Header.hpp"

namespace FPS_n2 {
	void			SetHideoutLv(std::vector<HideoutGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto ID = mes.substr(0, L);
			if (std::find_if(Data->begin(), Data->end(), [&](const HideoutGetData& obj) {return obj.GetID() == ID; }) == Data->end()) {
				HideoutGetData tmp;
				tmp.Set(ID, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};

	const int		HideoutList::Draw(int xp, int yp, int xsize, int ysize, int Lv, unsigned int defaultcolor, bool Clickactive) noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* Input = InputControl::Instance();
		int xs = xsize;
		int  Xsize = 0;

		int xg = 0;
		if (GetIcon().GetGraph()) {
			xg = (GetIcon().GetXSize() >= GetIcon().GetYSize())
				? (ysize * GetIcon().GetXSize() / GetIcon().GetYSize())
				: (ysize * GetIcon().GetYSize() / GetIcon().GetXSize());
		}
		bool IsLocked = false;// PlayerData::Instance()->GetHideoutLock(this->GetIDstr().c_str());
		int FirSize = (IsLocked) ? 36 : 0;
		xg += FirSize;
		auto Name = this->GetShortName();
		{
			if (xsize > 0) {
				int Lvbuf = 0;
				while (true) {
					if (Lv > 0) {
						Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s Lv%1d", Name.c_str(), Lv);
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
					Lvbuf++;
					if (Lvbuf > 100) {
						Name = "…";
						Xsize = LineHeight * 9 / 10;
						break;
					}
				}
			}
			else {
				if (Lv > 0) {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s Lv%1d", Name.c_str(), Lv);
				}
				else {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
				}
			}
			xs = std::max(xs, Xsize);
		}

		if (m_LvData.at(Lv - 1).m_ItemCraft.size() > 0) {
			defaultcolor = Green;//クラフトできるアイテムがあります//ビットコインは除外？
		}


		if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ysize, false, Clickactive, defaultcolor, "")) {
			auto sizeXBuf = y_r(800);
			auto sizeYBuf = y_r(0);

			if (Input->GetCtrlKey().press()) {
				DrawUnlockWindow(nullptr, 0, Lv, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
			}
			else {
				DrawCraftWindow(nullptr, 0, Lv, 0, 0, &sizeXBuf, &sizeYBuf, 100);//試しにサイズ計測
			}
			//
			signed long long FreeID = GetID() + 0xFFFF;
			//同じIDの奴いたら消そうぜ
			WindowMngr->DeleteAll();
			bool isHit = false;
			if (Input->GetMouseY() <= LineHeight) {
				isHit = true;
			}

			if (!isHit) {
				//ウィンドウ追加
				m_DrawWindowLv = Lv;

				std::string NameTmp = GetName();
				NameTmp += " Lv" + std::to_string(Lv);
				if (Input->GetCtrlKey().press()) {
					NameTmp += " Unlock";
					WindowMngr->Add()->Set(y_r(960) - sizeXBuf / 2, LineHeight + y_r(10), sizeXBuf, sizeYBuf, 0, NameTmp.c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
						HideoutData::Instance()->FindPtr((HideoutID)(win->m_FreeID - 0xFFFF))->DrawUnlockWindow(win, Gray10, m_DrawWindowLv, win->GetPosX(), win->GetPosY());
					});
				}
				else {
					NameTmp += " Craft";

					int ysizet = (int)((float)y_r(64));
					int Max = (int)(m_LvData.at(Lv - 1).m_ItemCraft.size());
					auto Total = (ysizet + y_r(5))*(int)(Max);

					WindowMngr->Add()->Set(y_r(960) - sizeXBuf / 2, LineHeight + y_r(10), sizeXBuf, sizeYBuf, Total, NameTmp.c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
						HideoutData::Instance()->FindPtr((HideoutID)(win->m_FreeID - 0xFFFF))->DrawCraftWindow(win, Gray10, m_DrawWindowLv, win->GetPosX(), win->GetPosY());
					});
				}
			}
		}
		{
			if (Lv > 0) {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s Lv%1d", Name.c_str(), Lv);
			}
			else {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s", Name.c_str());
			}
		}
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(), xp + FirSize + Xsize + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			Xsize += xg;
		}
		Xsize = std::max(Xsize, xs);

		if (IsLocked) {
			DrawControl::Instance()->SetDrawRotaLock(DrawLayer::Front, xp + FirSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}
		return Xsize;
	}
	void			HideoutList::DrawUnlockWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs, int* ys) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		//auto* InterParts = InterruptParts::Instance();
		int xofs = 0;
		int yofs = LineHeight;
		int yofs2 = yofs;
		{
			yofs += LineHeight;
		}
		if (Lv >= 1) {
			int yofs_OLD = yofs;
			int ysize = (int)((float)y_r(80));
			if (m_LvData.at(Lv - 1).m_ItemReq.size() > 0) {
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				{
					for (const auto& I : m_LvData.at(Lv - 1).m_ItemReq) {
						auto* ptr = ItemData::Instance()->FindPtr(I.GetID());
						if (ptr) {
							xofs_buf = xofs + y_r(10);
							int xstart = xp + xofs_buf;
							xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs_buf, y_r(300), ysize, I.GetCount(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs_buf, xp + xofs_buf, yp + yofs_buf + ysize, Gray75, false);
							yofs_buf += ysize + y_r(5);
						}
					}
				}
				{
					xofs_buf = std::max(xofs_buf, xofs + y_r(300));
				}
				xofs = std::max(xofs, xofs_buf + y_r(10));
				yofs = std::max(yofs, yofs_buf + y_r(10));
			}
			if (m_LvData.at(Lv - 1).m_Parent.size() > 0) {
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				{
					for (const auto& I : m_LvData.at(Lv - 1).m_Parent) {
						auto* ptr = HideoutData::Instance()->FindPtr(HideoutData::Instance()->FindID(I.GetID().c_str()));
						if (ptr) {
							xofs_buf = xofs + y_r(10);
							int xstart = xp + xofs_buf;
							xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs_buf, y_r(300), ysize, I.GetLv(), defaultcolor, !WindowMngr->PosHitCheck(window));
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs_buf, xp + xofs_buf, yp + yofs_buf + ysize, Gray75, false);
							yofs_buf += ysize + y_r(5);
						}
					}
				}
				{
					xofs_buf = std::max(xofs_buf, xofs + y_r(300));
				}
				xofs = std::max(xofs, xofs_buf + y_r(10));
				yofs = std::max(yofs, yofs_buf + y_r(10));
			}
			if (m_LvData.at(Lv - 1).m_TraderReq.size() > 0) {
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				{
					for (const auto& I : m_LvData.at(Lv - 1).m_TraderReq) {
						auto* ptr = TraderData::Instance()->FindPtr(I.GetID());
						if (ptr) {
							xofs_buf = xofs + y_r(10);
							int xstart = xp + xofs_buf;

							if (WindowSystem::ClickCheckBox(
								xp + xofs_buf, yp + yofs_buf,
								xp + xofs_buf + y_r(300), yp + yofs_buf + ysize,
								false, !WindowMngr->PosHitCheck(nullptr), defaultcolor, ptr->GetName() + " Lv" + std::to_string(I.GetLv()))) {
							}
							xofs_buf += y_r(300);
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs_buf, xp + xofs_buf, yp + yofs_buf + ysize, Gray75, false);
							yofs_buf += ysize + y_r(5);
						}
					}
				}
				{
					xofs_buf = std::max(xofs_buf, xofs + y_r(300));
				}
				xofs = std::max(xofs, xofs_buf + y_r(10));
				yofs = std::max(yofs, yofs_buf + y_r(10));
			}
			{
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs_buf, xp + 0, yp + yofs_buf + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
					"%01d:%02d:%02d", (m_LvData.at(Lv - 1).constructionTime / 60 / 60), (m_LvData.at(Lv - 1).constructionTime / 60) % 60, m_LvData.at(Lv - 1).constructionTime % 60);
				xofs_buf += y_r(30);

				xofs = std::max(xofs, xofs_buf + y_r(10));
			}
		}
		{
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
	void			HideoutList::DrawCraftWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs, int* ys, int size) noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		//auto* Input = InputControl::Instance();
		//auto* InterParts = InterruptParts::Instance();
		int xofs = 0;
		int yofs = LineHeight;
		int yofs2 = yofs;
		{
			yofs += LineHeight;
		}
		if (Lv >= 1) {
			if (m_LvData.at(Lv - 1).m_ItemCraft.size() > 0) {
				int xofs_buf = y_r(10);
				int ysize = (int)((float)y_r(64));

				int Max = (int)(m_LvData.at(Lv - 1).m_ItemCraft.size());
				int ofset = (window) ? (int)(window->GetNowScrollPer()*std::max(0, Max - 10 + 1)) : 0;

				auto OLD = yofs;
				for (int loop = 0; loop < std::min(size, Max - ofset); loop++) {
					int index = loop + ofset;
					const auto& cf = m_LvData.at(Lv - 1).m_ItemCraft[index];
					xofs_buf = y_r(10);
					{
						for (const auto& I : cf.m_ItemReq) {
							auto* ptr = ItemData::Instance()->FindPtr(I.GetID());
							if (ptr) {
								int xstart = xp + xofs_buf;
								xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetCount(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
								DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
							}
						}
					}
					{
						xofs_buf = std::max(xofs_buf, y_r(10) + ysize * 16);
						xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs, xp + 0, yp + yofs + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
							">%01d:%02d:%02d>", (cf.durationTime / 60 / 60), (cf.durationTime / 60) % 60, cf.durationTime % 60);
						xofs_buf += y_r(30);
					}
					{
						for (const auto& I : cf.m_ItemReward) {
							auto* ptr = ItemData::Instance()->FindPtr(I.GetID());
							if (ptr) {
								int xstart = xp + xofs_buf;
								xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetCount(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
								DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
							}
						}
					}
					xofs = std::max(xofs, xofs_buf + y_r(10));
					yofs += ysize + y_r(5);
				}
				yofs = OLD + (ysize + y_r(5)) * 10;
			}
		}
		{
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
};