#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	void			ItemList::ItemProperties::SetParent() noexcept {
		for (auto& cp : this->SetModSlots()) {
			for (auto& c : cp.m_Data) {
				c.CheckID(DataBase::Instance()->GetItemData().get());
			}
		}
		for (auto& cp : this->SetContainsItem()) {
			cp.CheckID(DataBase::Instance()->GetItemData().get());
		}
		//
		for (auto& cp : this->SetModSlots()) {
			for (const auto& c : cp.m_Data) {
				cp.SetTypeID(DataBase::Instance()->GetItemData()->FindPtr(c.GetID())->GetTypeID());
			}
		}
		//干渉
		for (auto& cp : this->SetConflictPartsID()) {
			cp.CheckID(DataBase::Instance()->GetItemData().get());
		}
	}
	void			ItemList::SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
		if (LEFT == "Itemtype") { this->m_ItemsData.m_TypeID.SetName(Args[0]); }
		else if (LEFT == "Map") {
			this->m_ItemsData.m_MapID.resize(this->m_ItemsData.m_MapID.size() + 1);
			this->m_ItemsData.m_MapID.back().SetName(Args[0]);
		}
		else if (LEFT == "width") { this->m_ItemsData.m_width = std::stoi(Args[0]); }
		else if (LEFT == "height") { this->m_ItemsData.m_height = std::stoi(Args[0]); }
		else if (LEFT.find("Sell_") != std::string::npos) {
			this->m_ItemsData.m_sellFor.resize(this->m_ItemsData.m_sellFor.size() + 1);
			this->m_ItemsData.m_sellFor.back().Set(LEFT.substr(strlenDx("Sell_")), std::stoi(Args[0]));
		}
		else if (LEFT == "weight") { this->m_ItemsData.m_weight = std::stof(Args[0]); }
		else if (LEFT == "fleaMarketFee") { this->m_ItemsData.m_fleaMarketFee = std::stoi(Args[0]); }

		m_ItemsData.m_properties.SetData(LEFT, Args);
	}
	void			ItemList::Load_Sub() noexcept {
		this->m_ItemsData.m_TypeID.CheckID(DataBase::Instance()->GetItemTypeData().get());
		for (auto& m : this->m_ItemsData.m_MapID) {
			m.CheckID(DataBase::Instance()->GetMapData().get());
		}
		for (auto& s : this->m_ItemsData.m_sellFor) {
			s.CheckID(DataBase::Instance()->GetTraderData().get(), false);//Invalidはフリマなのでエラー出さない
		}
		//
		{
			auto* typePtr = DataBase::Instance()->GetItemTypeData()->FindPtr(this->GetTypeID());
			if (typePtr) {
				auto* catPtr = DataBase::Instance()->GetItemCategoryData()->FindPtr(typePtr->GetCategoryID());
				if (catPtr->GetName() == "Weapons") {
					this->m_ItemsData.m_isWeapon = true;
				}
				else if (catPtr->GetName() == "WeaponMods") {
					this->m_ItemsData.m_isWeaponMod = true;
				}
				else if (catPtr->GetName() == "Medical") {
					this->m_ItemsData.m_isMedItem = true;
				}
			}
		}
	}
	void			ItemList::SetParent() noexcept {
		this->m_ItemsData.m_properties.SetParent();
		//自分を干渉相手にしている奴を探してそいつもリストに入れる　相思相愛
		for (const auto& t : DataBase::Instance()->GetItemData()->GetList()) {
			for (auto& cp : t.GetConflictParts()) {
				if (cp.GetID() == this->GetID()) {
					this->m_ItemsData.m_properties.SetConflictPartsID().resize(this->m_ItemsData.m_properties.SetConflictPartsID().size() + 1);
					this->m_ItemsData.m_properties.SetConflictPartsID().back().SetID(t.GetID());
					this->m_ItemsData.m_properties.SetConflictPartsID().back().SetName(t.GetName());
				}
			}
		}
		//
		this->m_ItemsData.m_ParentPartsID.clear();
		for (const auto& t : DataBase::Instance()->GetItemData()->GetList()) {
			for (auto& cp : t.GetChildParts()) {
				for (auto& c : cp.m_Data) {
					if (c.GetID() == this->GetID()) {
						this->m_ItemsData.m_ParentPartsID.emplace_back(t.GetID());
					}
				}
			}
		}
	}
	void			ItemList::WhenAfterLoad_Sub() noexcept {}
	const int		ItemList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy, bool IsIconOnly) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* Input = InputControl::Instance();
		int xs = xsize;
		int  Xsize = 0;

		int GraphXsize = (GetIcon().GetGraph()) ? ((GetIcon().GetXSize() >= GetIcon().GetYSize()) ? (ysize * GetIcon().GetXSize() / GetIcon().GetYSize()) : (ysize * GetIcon().GetYSize() / GetIcon().GetXSize())) : 0;
		bool IsLocked = PlayerData::Instance()->GetItemLock(this->GetIDstr().c_str());
		int FirXSize = (IsFir || IsLocked) ? 36 : 0;
		int  StrXSize = 0;

		std::string Name = "";
		std::string AfterName = "";
		if (count > 0) {
			AfterName += " x" + std::to_string(count);
		}
		if (!IsIconOnly) {
			Name = this->GetName();
			if (xsize > 0) {
				int countbuf = 0;
				while (true) {
					StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName);
					if ((xs - (GraphXsize + FirXSize)) < StrXSize) {
						Name = Name.substr(0, (size_t)(((int)(Name.size()) * (xs - (GraphXsize + FirXSize)) / StrXSize) - AfterName.size() - 2)) + "…";
					}
					else {
						break;
					}
					countbuf++;
					if (countbuf > 10) {
						Name = "…";
						StrXSize = LineHeight * 9 / 10;
						break;
					}
				}
			}
			else {
				StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName);
			}
			Xsize += StrXSize;
		}
		int GraphXPos = xp + FirXSize + Xsize;
		if (GetIcon().GetGraph()) {
			Xsize += (GraphXsize + FirXSize);
		}
		if (!IsIconOnly) {
			Xsize = std::max(Xsize, FirXSize + WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName));
		}
		else {
			if (count > 0) {
				Xsize = std::max(Xsize, (GraphXsize + FirXSize) + WindowSystem::GetMsgLen(LineHeight * 9 / 10, AfterName));
			}
		}


		xs = std::max(xs, Xsize);

		if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ysize, false, Clickactive, defaultcolor, "")) {
			auto sizeXBuf = y_r(900);
			auto sizeYBuf = y_r(0);
			DrawWindow(nullptr, 0, y_r(1920), y_r(1080), &sizeXBuf, &sizeYBuf);//試しにサイズ計測
			//
			signed long long FreeID = (signed long long)GetID() + 0xFFFF;
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
					DataBase::Instance()->GetItemData()->FindPtr((ItemID)(win->m_FreeID - 0xFFFF))->DrawWindow(win, Gray10, win->GetPosX(), win->GetPosY());
				});
			}
		}
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(), GraphXPos + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
		}
		if (!IsIconOnly) {
			if (count > 0) {
				WindowSystem::SetMsg(xp + FirXSize, yp, xp + FirXSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s x%1d", Name.c_str(), count);
			}
			else {
				WindowSystem::SetMsg(xp + FirXSize, yp, xp + FirXSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s", Name.c_str());
			}
		}
		else {
			if (count > 0) {
				WindowSystem::SetMsg(xp + (GraphXsize + FirXSize), yp, xp + (GraphXsize + FirXSize), yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "x%1d", count);
			}
		}

		if (IsLocked) {
			DrawControl::Instance()->SetDrawRotaLock(DrawLayer::Front, xp + FirXSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}
		if (IsFir) {
			DrawControl::Instance()->SetDrawRotaFiR(DrawLayer::Normal, xp + FirXSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}

		if (IsDrawBuy) {
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xp, yp, xp + xs, yp + ysize)) {
				TraderID ID = InvalidID;
				int Value = -1;
				if (GetSellValue(&ID, &Value)) {
					auto Color = Green;
					std::string TraderName = "Flea Market";
					if (ID != InvalidID) {
						auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(ID);
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
		return Xsize;
	}
	void			ItemList::DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs, int* ys) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* InterParts = InterruptParts::Instance();
		int xofs = y_r(600) * 2;
		int yofs = LineHeight + y_r(5);
		int yofs2 = yofs;
		if (GetIcon().GetGraph()) {
			DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 64);
			float Scale = 1.f;
			float rad = 0.f;
			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(),
				xp + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale),
				yp + LineHeight + GetIcon().GetYSize() / 2, Scale, rad, false);
			DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
		}
		{
			//タスク
			{
				{
					int xofsBuf = y_r(10);
					int OLDyofs = yofs;
					int yofsBuf = OLDyofs;
					int count = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									count++;
									break;
								}
							}
						}
					}

					int count2 = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									if (count2 == 0) {
										WindowSystem::SetMsg(xp + xofsBuf, yp + yofsBuf, xp + xofsBuf, yp + LineHeight + yofsBuf, LineHeight, STRX_LEFT, White, Black, "Firタスク納品:"); yofsBuf += LineHeight + y_r(5);
									}
									if (count > 1 && (count / 2) <= count2) {
										xofsBuf = y_r(600) + y_r(10);
										yofs = std::max(yofs, yofsBuf);
										yofsBuf = OLDyofs + LineHeight + y_r(5);
									}
									tasks.Draw(xp + xofsBuf, yp + yofsBuf, y_r(600) - y_r(10) * 2, LineHeight, w.GetValue(), !WindowMngr->PosHitCheck(window)); yofsBuf += LineHeight + y_r(5);
									count2++;
									break;
								}
							}
						}
					}
					yofs = std::max(yofs, yofsBuf);
				}
				{
					int xofsBuf = y_r(10);
					int OLDyofs = yofs;
					int yofsBuf = OLDyofs;
					int count = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									count++;
									break;
								}
							}
						}
					}

					int count2 = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									if (count2 == 0) {
										WindowSystem::SetMsg(xp + xofsBuf, yp + yofsBuf, xp + xofsBuf, yp + LineHeight + yofsBuf, LineHeight, STRX_LEFT, White, Black, "Firでないタスク納品:"); yofsBuf += LineHeight + y_r(5);
									}
									if (count > 1 && (count / 2) <= count2) {
										xofsBuf = y_r(600) + y_r(10);
										yofs = std::max(yofs, yofsBuf);
										yofsBuf = OLDyofs + LineHeight + y_r(5);
									}
									tasks.Draw(xp + xofsBuf, yp + yofsBuf, y_r(600) - y_r(10) * 2, LineHeight, w.GetValue(), !WindowMngr->PosHitCheck(window)); yofsBuf += LineHeight + y_r(5);
									count2++;
									break;
								}
							}
						}
					}
					yofs = std::max(yofs, yofsBuf);
				}
				{
					int xofsBuf = y_r(10);
					int OLDyofs = yofs;
					int yofsBuf = OLDyofs;
					int count = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						bool isHit = false;
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (isHit) {
							continue;
						}
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
								if (w.GetID() == GetID()) {
									count++;
									break;
								}
							}
						}
					}

					int count2 = 0;
					for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
						bool isHit = false;
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (!isHit) {
							for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
								if (w.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
						}
						if (isHit) {
							continue;
						}
						if (tasks.GetIsUSECorBEAR()) {
							for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
								if (w.GetID() == GetID()) {
									if (count2 == 0) {
										WindowSystem::SetMsg(xp + xofsBuf, yp + yofsBuf, xp + xofsBuf, yp + LineHeight + yofsBuf, LineHeight, STRX_LEFT, White, Black, "タスクに必要:"); yofsBuf += LineHeight + y_r(5);
									}
									if (count > 1 && (count / 2) <= count2) {
										xofsBuf = y_r(600) + y_r(10);
										yofs = std::max(yofs, yofsBuf);
										yofsBuf = OLDyofs + LineHeight + y_r(5);
									}
									tasks.Draw(xp + xofsBuf, yp + yofsBuf, y_r(600) - y_r(10) * 2, LineHeight, w.GetValue(), !WindowMngr->PosHitCheck(window)); yofsBuf += LineHeight + y_r(5);
									count2++;
									break;
								}
							}
						}
					}
					yofs = std::max(yofs, yofsBuf);
				}
			}
			//ハイドアウト開放
			{
				bool isFirst = true;
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						for (const auto& w : Ld.m_ItemReq) {
							if (w.GetID() == GetID()) {
								if (isFirst) {
									isFirst = false;
									xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(10), yp + yofs, xp + y_r(10), yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black,"ハイドアウト開放:")); yofs += LineHeight + y_r(5);
								}
								L.Draw(xp + y_r(10), yp + yofs, y_r(600) - y_r(10) * 2, LineHeight, (int)(&Ld - &L.GetLvData().front()) + 1, defaultcolor, !WindowMngr->PosHitCheck(window), w.GetValue());
								yofs += LineHeight + y_r(5);
								break;
							}
						}
					}
				}
			}
			//ハイドアウトクラフト素材
			{
				bool isFirst = true;
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
						for (const auto& cf : Ld.m_ItemCraft) {
							bool isHit = false;
							if (!isHit) {
								for (const auto& I : cf.m_ItemReq) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (!isHit) {
								for (const auto& I : cf.m_ItemReward) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (isHit) {
								if (isFirst) {
									isFirst = false;
									xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ハイドアウトクラフト:")); yofs += LineHeight + y_r(5);
								}
								xofs = std::max(xofs, L.DrawCraft(window, defaultcolor, xp, yp + yofs, y_r(64), Lv, (int)(&cf - &Ld.m_ItemCraft.front()), true, true, 0));
								yofs += y_r(64);
							}
						}
					}
				}
			}
			//トレーダー交換
			{
				bool isFirst = true;
				for (auto&L : DataBase::Instance()->GetTraderData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
						for (const auto& cf : Ld.m_ItemBarters) {
							bool isHit = false;
							if (!isHit) {
								for (const auto& I : cf.m_ItemReq) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (!isHit) {
								for (const auto& I : cf.m_ItemReward) {
									if (I.GetID() == GetID()) {
										isHit = true;
										break;
									}
								}
							}
							if (isHit) {
								if (isFirst) {
									isFirst = false;
									xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "トレーダー交換:")); yofs += LineHeight + y_r(5);
								}
								xofs = std::max(xofs, L.DrawBarter(window, defaultcolor, xp, yp + yofs, y_r(64), Lv, (int)(&cf - &Ld.m_ItemBarters.front()), true, true, 0));
								yofs += y_r(64);
							}
						}
					}
				}
			}
			//武器ならば
			if (this->m_ItemsData.m_isWeapon) {
				if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, !WindowMngr->PosHitCheck(window), Green, "GotoPreset")) {
					InterParts->GotoNext(BGSelect::Custom);
					InterParts->SetInitParam(0, GetID());//武器ID
					InterParts->SetInitParam(1, InvalidID);//プリセットID
				}
				yofs += LineHeight + y_r(5);
			}
			//プロパティ
			this->m_ItemsData.m_properties.DrawInfo(xp, yp, &xofs, &yofs);
			//
			{
				bool isFirst = true;
				for (const auto& cp : this->GetChildParts()) {
					for (const auto& c : cp.m_Data) {
						if (isFirst) {
							isFirst = false;
							xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ChildrenMods:") + y_r(30)); yofs += LineHeight + y_r(5);
						}
						auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.GetID());
						xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), y_r(42), 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false) + y_r(30));
						yofs += y_r(42) + y_r(5);
					}
				}
			}
			{
				bool isFirst = true;
				for (const auto& c : this->m_ItemsData.m_ParentPartsID) {
					if (isFirst) {
						isFirst = false;
						xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ParentMods:") + y_r(30)); yofs += LineHeight + y_r(5);
					}
					auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c);
					xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), y_r(42), 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false) + y_r(30));
					yofs += y_r(42) + y_r(5);
				}
			}
			//
		}
		if (GetIcon().GetGraph()) {
			xofs = std::max<int>(xofs, GetIcon().GetXSize());
			yofs2 += std::max(GetIcon().GetYSize(), 64 * 2);
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
	//
	void ItemJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		m_categorytypes = data["category"]["name"];
		if (data.contains("width")) { this->m_ItemsData.m_width = data["width"]; }
		if (data.contains("height")) { this->m_ItemsData.m_height = data["height"]; }
		m_ItemsData.m_weight = data["weight"];
		for (const auto& sf : data["sellFor"]) {
			m_ItemsData.m_sellFor.resize(m_ItemsData.m_sellFor.size() + 1);
			m_ItemsData.m_sellFor.back().Set(sf["vendor"]["name"], sf["price"]);
		}
		for (const auto& ts : data["conflictingItems"]) {
			m_ItemsData.m_properties.SetConflictPartsID().resize(m_ItemsData.m_properties.SetConflictPartsID().size() + 1);
			m_ItemsData.m_properties.SetConflictPartsID().back().SetName(ts["name"]);
		}
		if (data.contains("properties")) {
			if (!data["properties"].is_null()) {
				m_ItemsData.m_properties.GetJsonData(data);
			}
		}
	}
	void ItemJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		outputfile << "Itemtype=" + this->m_categorytypes + "\n";
		outputfile << "width=" + std::to_string(this->m_ItemsData.m_width) + "\n";
		outputfile << "height=" + std::to_string(this->m_ItemsData.m_height) + "\n";
		outputfile << "weight=" + std::to_string(this->m_ItemsData.m_weight) + "\n";
		for (auto& sf : this->m_ItemsData.m_sellFor) {
			if (sf.GetName() != "Flea Market") {
				outputfile << "Sell_" + sf.GetName() + "=" + std::to_string(sf.GetValue()) + "\n";
			}
		}
		outputfile << "fleaMarketFee=" + std::to_string(this->m_ItemsData.m_fleaMarketFee) + "\n";
		this->m_ItemsData.m_properties.OutputData(outputfile);
	}
	//
	void ItemData::UpdateAfterbyJson(void) noexcept {
		for (auto& L : this->m_List) {
			for (auto& jd : GetJsonDataList()) {
				if (L.GetIDstr() == jd->m_id) {
					//既存のものを保持しておく
					std::ofstream outputfile(L.GetFilePath(), std::ios::app);
					for (auto& m : L.GetMapID()) {
						auto* ptr = DataBase::Instance()->GetMapData()->FindPtr(m.GetID());
						if (ptr) {
							outputfile << "Map=" + ptr->GetName() + "\n";
						}
					}
					outputfile.close();
					break;
				}
			}
		}
		UpdateAfterbyJson_Sub();
	}
};