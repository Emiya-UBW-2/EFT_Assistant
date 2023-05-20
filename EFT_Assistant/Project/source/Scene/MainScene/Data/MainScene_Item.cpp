#include"../../../Header.hpp"

namespace FPS_n2 {
	void			SetItem(std::vector<ItemGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto ID = ItemData::Instance()->FindID(mes.substr(0, L).c_str());
			if (std::find_if(Data->begin(), Data->end(), [&](const ItemGetData& obj) {return obj.GetID() == ID; }) == Data->end()) {
				ItemGetData tmp;
				tmp.Set(ID, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};

	const int		ItemList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) const noexcept {
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
		bool IsLocked = PlayerData::Instance()->GetItemLock(this->GetIDstr().c_str());
		int FirSize = (IsFir || IsLocked) ? 36 : 0;
		xg += FirSize;
		auto Name = this->GetShortName();
		if (!IsIconOnly) {
			if (xsize > 0) {
				int countbuf = 0;
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
					countbuf++;
					if (countbuf > 100) {
						Name = "…";
						Xsize = LineHeight * 9 / 10;
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
		else {
			Name = "";
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
			if (Input->GetMouseY() <= LineHeight) {
				isHit = true;
			}

			if (!isHit) {
				//ウィンドウ追加
				WindowMngr->Add()->Set(Input->GetMouseX() - sizeXBuf / 2, Input->GetMouseY(), sizeXBuf, sizeYBuf, 0, GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
					ItemData::Instance()->FindPtr((ItemID)(win->m_FreeID - 0xFFFF))->DrawWindow(win, Gray10, win->GetPosX(), win->GetPosY());
				});
			}
		}
		if (!IsIconOnly) {
			if (count > 0) {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s x%2d", Name.c_str(), count);
			}
			else {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s", Name.c_str());
			}
		}
		else {
			if (count > 0) {
				WindowSystem::SetMsg(xp + xg, yp, xp + xg, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "x%2d", count);
			}
		}
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(), xp + FirSize + Xsize + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			Xsize += xg;
		}

		if (IsLocked) {
			DrawControl::Instance()->SetDrawRotaLock(DrawLayer::Front, xp + FirSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}
		if (IsFir) {
			DrawControl::Instance()->SetDrawRotaFiR(DrawLayer::Normal, xp + FirSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}

		if (IsDrawBuy) {
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp, yp, xp + xs, yp + ysize)) {
				TraderID ID = InvalidID;
				int Value = -1;
				if (GetSellValue(&ID, &Value)) {
					auto Color = Green;
					std::string TraderName = "Flea Market";
					if (ID != InvalidID) {
						auto* ptr = TraderData::Instance()->FindPtr(ID);
						if (ptr) {
							TraderName = ptr->GetName();
							Color = ptr->GetColors(50);
						}
					}
					DrawControl::Instance()->SetString(DrawLayer::Front,
						FontPool::FontType::Nomal_Edge, LineHeight,
						STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), Color, Black,
						"最高値:%s = %d", TraderName.c_str(), Value
					);
					DrawControl::Instance()->SetString(DrawLayer::Front,
						FontPool::FontType::Nomal_Edge, LineHeight,
						STRX_RIGHT, STRY_TOP, Input->GetMouseX(), Input->GetMouseY(), Color, Black,
						"マス単価: %d", Value / (Getwidth()*Getheight())
					);
				}
			}
		}
		Xsize = std::max(Xsize, xs);
		return Xsize;
	}
	void			ItemList::DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs, int* ys) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* InterParts = InterruptParts::Instance();
		int xofs = 0;
		int yofs = LineHeight;
		int yofs2 = yofs;
		{
			yofs += LineHeight + y_r(5);
			//タスク
			{
				for (const auto& tasks : TaskData::Instance()->GetList()) {
					bool IsChecktask = true;
					if (false) {//河童必要タスクだけ書く
						if (!tasks.GetTaskNeedData().GetKappaRequired()) {
							IsChecktask = false;
						}
						if (IsChecktask) {
							if (tasks.GetName() == "Collector") {
								//IsChecktask = false;
							}
						}
					}
					if (false) {
						if (!tasks.GetTaskNeedData().GetLightKeeperRequired()) {
							IsChecktask = false;
						}
					}
					if (71 < tasks.GetTaskNeedData().GetLevel()) {
						IsChecktask = false;
					}
					if (PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str())) {
						IsChecktask = false;
					}
					if (!IsChecktask) { continue; }
					bool isHit = false;
					for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
						if (w.GetID() == GetID()) {
							std::string Str = "FiR : ";
							Str += tasks.GetName();
							Str += " x" + std::to_string(w.GetCount());
							if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, Str)) {

								auto sizeXBuf = y_r(800);
								auto sizeYBuf = y_r(0);
								tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
								//
								signed long long FreeID = tasks.GetID();
								WindowMngr->Add()->Set(xp + y_r(800) / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
									TaskData::Instance()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
								});

							}
							yofs += LineHeight + y_r(5);
							isHit = true;
							break;
						}
					}
					if (isHit) { continue; }
					for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
						if (w.GetID() == GetID()) {
							std::string Str = "Not FiR : ";
							Str += tasks.GetName();
							Str += " x" + std::to_string(w.GetCount());
							if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, Str)) {

								auto sizeXBuf = y_r(800);
								auto sizeYBuf = y_r(0);
								tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
								//
								signed long long FreeID = tasks.GetID();
								WindowMngr->Add()->Set(xp + y_r(800) / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
									TaskData::Instance()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
								});

							}
							yofs += LineHeight + y_r(5);
							isHit = true;
							break;
						}
					}
					if (isHit) { continue; }
					for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
						if (w.GetID() == GetID()) {
							std::string Str = "Need : ";
							Str += tasks.GetName();
							Str += " x" + std::to_string(w.GetCount());
							if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, Str)) {

								auto sizeXBuf = y_r(800);
								auto sizeYBuf = y_r(0);
								tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
								//
								signed long long FreeID = tasks.GetID();
								WindowMngr->Add()->Set(xp + y_r(800) / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
									TaskData::Instance()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
								});

							}
							yofs += LineHeight + y_r(5);
							isHit = true;
							break;
						}
					}
					if (isHit) { continue; }
					//
				}
			}
			//
			if (m_isWeapon) {
				if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, "GotoPreset")) {
					InterParts->GotoNext(BGSelect::Custom);
					InterParts->SetInitParam(0, GetID());//武器ID
					InterParts->SetInitParam(1, InvalidID);//プリセットID
				}
				yofs += LineHeight + y_r(5);
			}
			//
			if (m_isWeapon || m_isWeaponMod) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, (m_Recoil < 0.f) ? Green : Red, Black,
					"Recoil:%s%3.1f %%", (m_Recoil > 0.f) ? "+" : "", m_Recoil) + y_r(30)); yofs += LineHeight + y_r(5);
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, (m_Ergonomics >= 0.f) ? Green : Red, Black,
					"Ergonomics:%s%3.1f", (m_Ergonomics > 0.f) ? "+" : "", m_Ergonomics) + y_r(30)); yofs += LineHeight + y_r(5);
			}
			//
			{
				if (m_ChildPartsID.size() > 0) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ChildrenMods:") + y_r(30)); yofs += LineHeight + y_r(5);
				}
				int ysize = (int)((float)y_r(42));
				for (const auto& cp : m_ChildPartsID) {
					for (const auto& c : cp.Data) {
						auto* ptr = c.first;
						xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false) + y_r(30));
						yofs += ysize + y_r(5);
					}
				}
				if (m_ParentPartsID.size() > 0) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ParentMods:") + y_r(30)); yofs += LineHeight + y_r(5);
				}
				for (const auto& c : m_ParentPartsID) {
					auto* ptr = c;
					xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false) + y_r(30));
					yofs += ysize + y_r(5);
				}
			}
			//
		}
		if (GetIcon().GetGraph()) {
			DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 64);
			int ysize = GetIcon().GetYSize();

			float Scale = 1.f;
			float rad = 0.f;

			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(),
				xp + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale),
				yp + LineHeight + ysize / 2, Scale, rad, false);
			xofs = std::max<int>(xofs, GetIcon().GetXSize());
			yofs2 += std::max(ysize, 64 * 2);
			DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
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

};