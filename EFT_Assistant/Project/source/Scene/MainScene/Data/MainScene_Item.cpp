#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	void ItemList::ItemProperties::OutputData(std::ofstream& outputfile) noexcept {
		outputfile << "propertiestype=" + (std::string)(this->GetTypeName()) + "\n";
		if (this->GetSightingRange() >= 0) {
			outputfile << "SightRange=" + std::to_string(this->GetSightingRange()) + "\n";
		}
		if (this->m_ChildPartsID.size() > 0) {
			for (const auto& m : this->m_ChildPartsID) {
				if (m.m_Data.size() > 0) {
					outputfile << "ChildParts=[\n";
					std::vector<std::string> Names;
					for (auto& d : m.m_Data) {
						auto NmBuf = d.GetName();
						if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
							outputfile << "\t" + NmBuf + ((&d != &m.m_Data.back()) ? DIV_STR : "") + "\n";
							Names.emplace_back(NmBuf);
						}
					}
					outputfile << "]\n";
				}
			}
		}
		if (this->m_ConflictPartsID.size() > 0) {
			bool isHit = false;
			std::vector<std::string> Names;
			for (auto& m : this->m_ConflictPartsID) {
				auto NmBuf = m.GetName();
				if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
					if (!isHit) {
						isHit = true;
						outputfile << "Conflict=[\n";
					}
					outputfile << "\t" + NmBuf + ((&m != &this->m_ConflictPartsID.back()) ? DIV_STR : "") + "\n";
					Names.emplace_back(NmBuf);
				}
			}
			if (isHit) {
				outputfile << "]\n";
			}
		}
		if (this->GetType() == EnumItemProperties::ItemPropertiesWeapon) {
			outputfile << "Recoil=" + std::to_string((float)this->GetWeaponRecoilVertical()) + "\n";
			outputfile << "Ergonomics=" + std::to_string(this->GetWeaponErgonomics()) + "\n";
		}
		else {
			switch (this->GetType()) {
			case EnumItemProperties::ItemPropertiesBarrel:
			case EnumItemProperties::ItemPropertiesMagazine:
			case EnumItemProperties::ItemPropertiesScope:
			case EnumItemProperties::ItemPropertiesWeaponMod:
				outputfile << "Recoil=" + std::to_string((float)this->GetModRecoil()*100.f) + "\n";
				outputfile << "Ergonomics=" + std::to_string(this->GetModErgonomics()) + "\n";
				break;
			default:
				break;
			}
		}
	}
	//
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
		for (auto s : this->m_ItemsData.m_sellFor) {
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
			}
		}
	}
	void			ItemList::SetParent() noexcept {
		for (auto& cp : this->m_ItemsData.m_properties.SetModSlots()) {
			for (auto& c : cp.m_Data) {
				c.CheckID(DataBase::Instance()->GetItemData().get());
			}
		}
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
		//
		for (auto& cp : this->m_ItemsData.m_properties.SetModSlots()) {
			for (const auto& c : cp.m_Data) {
				cp.SetTypeID(DataBase::Instance()->GetItemData()->FindPtr(c.GetID())->GetTypeID());
			}
		}
		//
		for (auto& cp : this->m_ItemsData.m_properties.SetConflictPartsID()) {
			cp.CheckID(DataBase::Instance()->GetItemData().get());
		}
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
		if (!IsIconOnly) {
			Name = this->GetShortName();
			if (xsize > 0) {
				int countbuf = 0;
				while (true) {
					if (count > 0) {
						StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s x%2d", Name.c_str(), count);
					}
					else {
						StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
					}
					if ((xs - (GraphXsize + FirXSize)) < StrXSize) {
						Name = Name.substr(0, (size_t)((int)(Name.size()) * (xs - (GraphXsize + FirXSize)) / StrXSize - 2)) + "…";
					}
					else {
						break;
					}
					countbuf++;
					if (countbuf > 100) {
						Name = "…";
						StrXSize = LineHeight * 9 / 10;
						break;
					}
				}
			}
			else {
				if (count > 0) {
					StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s x%2d", Name.c_str(), count);
				}
				else {
					StrXSize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
				}
			}
			Xsize += StrXSize;
		}
		int GraphXPos = xp + FirXSize + Xsize;
		if (GetIcon().GetGraph()) {
			Xsize += (GraphXsize + FirXSize);
		}
		if (!IsIconOnly) {
			if (count > 0) {
				Xsize = std::max(Xsize, FirXSize + WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + " x%1d", count));
			}
			else {
				Xsize = std::max(Xsize, FirXSize + WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name));
			}
		}
		else {
			if (count > 0) {
				Xsize = std::max(Xsize, (GraphXsize + FirXSize) + WindowSystem::GetMsgLen(LineHeight * 9 / 10, "x%1d", count));
			}
		}


		xs = std::max(xs, Xsize);

		if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ysize, false, Clickactive, defaultcolor, "")) {
			auto sizeXBuf = y_r(800);
			auto sizeYBuf = y_r(0);
			DrawWindow(nullptr, 0, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
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
		int xofs = 0;
		int yofs = LineHeight;
		int yofs2 = yofs;
		{
			yofs += LineHeight + y_r(5);
			//タスク
			{
				for (const auto& tasks : DataBase::Instance()->GetTaskData()->GetList()) {
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
						auto ID = w.GetID();
						if (ID == GetID()) {
							std::string Str = "FiR : ";
							Str += tasks.GetName();
							Str += " x" + std::to_string(w.GetValue());
							if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, Str)) {

								auto sizeXBuf = y_r(800);
								auto sizeYBuf = y_r(0);
								tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
								//
								signed long long FreeID = tasks.GetID();
								WindowMngr->Add()->Set(xp + y_r(800) / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
									DataBase::Instance()->GetTaskData()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
								});

							}
							yofs += LineHeight + y_r(5);
							isHit = true;
							break;
						}
					}
					if (isHit) { continue; }
					for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
						auto ID = w.GetID();
						if (ID == GetID()) {
							std::string Str = "Not FiR : ";
							Str += tasks.GetName();
							Str += " x" + std::to_string(w.GetValue());
							if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, Str)) {

								auto sizeXBuf = y_r(800);
								auto sizeYBuf = y_r(0);
								tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
								//
								signed long long FreeID = tasks.GetID();
								WindowMngr->Add()->Set(xp + y_r(800) / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
									DataBase::Instance()->GetTaskData()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
								});

							}
							yofs += LineHeight + y_r(5);
							isHit = true;
							break;
						}
					}
					if (isHit) { continue; }
					for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
						auto ID = w.GetID();
						if (ID == GetID()) {
							std::string Str = "Need : ";
							Str += tasks.GetName();
							Str += " x" + std::to_string(w.GetValue());
							if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, Str)) {

								auto sizeXBuf = y_r(800);
								auto sizeYBuf = y_r(0);
								tasks.DrawWindow(nullptr, 0, 0, &sizeXBuf, &sizeYBuf);//試しにサイズ計測
								//
								signed long long FreeID = tasks.GetID();
								WindowMngr->Add()->Set(xp + y_r(800) / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, tasks.GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
									DataBase::Instance()->GetTaskData()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
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
			//ハイドアウト
			{
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black,
					"ハイドアウト開放:")); yofs += LineHeight + y_r(5);
				//ハイドアウト開放
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						/*
						bool IsChecktask = true;
						if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
							IsChecktask = false;
						}
						if (!IsChecktask) { continue; }
						//*/
						//開放
						for (const auto& w : Ld.m_ItemReq) {
							if (w.GetID() == GetID()) {
								L.Draw(xp, yp + yofs, y_r(400), LineHeight, (int)(&Ld - &L.GetLvData().front()) + 1, defaultcolor, true);
								WindowSystem::SetMsg(xp + y_r(400), yp + yofs, xp + y_r(800), yp + yofs + LineHeight, LineHeight, STRX_LEFT, White, Black, " x%d", w.GetValue());
								yofs += LineHeight + y_r(5);
								break;
							}
						}
					}
				}
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black,
					"ハイドアウトクラフト:")); yofs += LineHeight + y_r(5);
				//ハイドアウトクラフト素材
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (y_r(64));

						for (const auto& cf : Ld.m_ItemCraft) {
							bool isHit = false;
							for (const auto& I : cf.m_ItemReq) {
								if (I.GetID() == GetID()) {
									isHit = true;
									break;
								}
							}
							if (isHit) {
								xofs_buf = y_r(10);
								{
									for (const auto& I : cf.m_ItemReq) {
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(I.GetID());
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								{
									//xofs_buf = std::max(xofs_buf, y_r(10) + ysize * 8);
									xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs, xp + 0, yp + yofs + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
										">%01d:%02d:%02d>", (cf.durationTime / 60 / 60), (cf.durationTime / 60) % 60, cf.durationTime % 60);
									xofs_buf += y_r(30);
								}
								{
									for (const auto& I : cf.m_ItemReward) {
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(I.GetID());
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								xofs = std::max(xofs, xofs_buf + y_r(10));
								yofs += ysize + y_r(5);
							}
						}
						//クラフトベース
					}
				}
				//ハイドアウトクラフト結果
				for (auto&L : DataBase::Instance()->GetHideoutData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (y_r(64));

						for (const auto& cf : Ld.m_ItemCraft) {
							bool isHit = false;
							for (const auto& I : cf.m_ItemReward) {
								auto ID = I.GetID();
								if (ID == GetID()) {
									isHit = true;
									break;
								}
							}
							if (isHit) {
								xofs_buf = y_r(10);
								{
									for (const auto& I : cf.m_ItemReq) {
										auto ID = I.GetID();
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								{
									//xofs_buf = std::max(xofs_buf, y_r(10) + ysize * 8);
									xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs, xp + 0, yp + yofs + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
										">%01d:%02d:%02d>", (cf.durationTime / 60 / 60), (cf.durationTime / 60) % 60, cf.durationTime % 60);
									xofs_buf += y_r(30);
								}
								{
									for (const auto& I : cf.m_ItemReward) {
										auto ID = I.GetID();
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								xofs = std::max(xofs, xofs_buf + y_r(10));
								yofs += ysize + y_r(5);
							}
						}
						//クラフトベース
					}
				}
			}
			//交換
			{
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black,
					"トレーダー交換:")); yofs += LineHeight + y_r(5);
				//ハイドアウトクラフト素材
				for (auto&L : DataBase::Instance()->GetTraderData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (y_r(64));

						for (const auto& cf : Ld.m_ItemBarters) {
							bool isHit = false;
							for (const auto& I : cf.m_ItemReq) {
								auto ID = I.GetID();
								if (ID == GetID()) {
									isHit = true;
									break;
								}
							}
							if (isHit) {
								xofs_buf = y_r(10);
								{
									for (const auto& I : cf.m_ItemReq) {
										auto ID = I.GetID();
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								{
									//xofs_buf = std::max(xofs_buf, y_r(10) + ysize * 8);
									xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs, xp + 0, yp + yofs + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
										"->");
									xofs_buf += y_r(30);
								}
								{
									for (const auto& I : cf.m_ItemReward) {
										auto ID = I.GetID();
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								xofs = std::max(xofs, xofs_buf + y_r(10));
								yofs += ysize + y_r(5);
							}
						}
						//クラフトベース
					}
				}
				//ハイドアウトクラフト結果
				for (auto&L : DataBase::Instance()->GetTraderData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (y_r(64));

						for (const auto& cf : Ld.m_ItemBarters) {
							bool isHit = false;
							for (const auto& I : cf.m_ItemReward) {
								auto ID = I.GetID();
								if (ID == GetID()) {
									isHit = true;
									break;
								}
							}
							if (isHit) {
								xofs_buf = y_r(10);
								{
									for (const auto& I : cf.m_ItemReq) {
										auto ID = I.GetID();
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								{
									//xofs_buf = std::max(xofs_buf, y_r(10) + ysize * 8);
									xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs, xp + 0, yp + yofs + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
										"->");
									xofs_buf += y_r(30);
								}
								{
									for (const auto& I : cf.m_ItemReward) {
										auto ID = I.GetID();
										auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
										if (ptr) {
											int xstart = xp + xofs_buf;
											xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs, y_r(200), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
											DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs, xp + xofs_buf, yp + yofs + ysize, Gray75, false);
										}
									}
								}
								xofs = std::max(xofs, xofs_buf + y_r(10));
								yofs += ysize + y_r(5);
							}
						}
						//クラフトベース
					}
				}
			}
			//
			if (this->m_ItemsData.m_isWeapon) {
				if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + y_r(800), yp + LineHeight + yofs, false, true, Green, "GotoPreset")) {
					InterParts->GotoNext(BGSelect::Custom);
					InterParts->SetInitParam(0, GetID());//武器ID
					InterParts->SetInitParam(1, InvalidID);//プリセットID
				}
				yofs += LineHeight + y_r(5);
			}
			//
			if (this->m_ItemsData.m_isWeapon || this->m_ItemsData.m_isWeaponMod) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, (this->GetRecoil() < 0.f) ? Green : Red, Black,
					"Recoil:%3.1f %%", this->GetRecoil()) + y_r(30)); yofs += LineHeight + y_r(5);
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, (this->GetErgonomics() >= 0.f) ? Green : Red, Black,
					"Ergonomics:%3.1f", this->GetErgonomics()) + y_r(30)); yofs += LineHeight + y_r(5);
			}
			//
			{
				if (this->GetChildParts().size() > 0) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ChildrenMods:") + y_r(30)); yofs += LineHeight + y_r(5);
				}
				int ysize = (y_r(42));
				for (const auto& cp : this->GetChildParts()) {
					for (const auto& c : cp.m_Data) {
						auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c.GetID());
						xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true, false) + y_r(30));
						yofs += ysize + y_r(5);
					}
				}
				if (this->m_ItemsData.m_ParentPartsID.size() > 0) {
					xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black, "ParentMods:") + y_r(30)); yofs += LineHeight + y_r(5);
				}
				for (const auto& c : this->m_ItemsData.m_ParentPartsID) {
					auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(c);
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
				m_ItemsData.m_properties.GetJsonData(data["properties"]);
			}
		}
	}
	void ItemJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		outputfile << "Itemtype=" + this->m_categorytypes + "\n";
		outputfile << "width=" + std::to_string(this->m_ItemsData.m_width) + "\n";
		outputfile << "height=" + std::to_string(this->m_ItemsData.m_height) + "\n";
		outputfile << "weight=" + std::to_string(this->m_ItemsData.m_weight) + "\n";
		for (auto& sf : this->m_ItemsData.m_sellFor) {
			outputfile << "Sell_" + sf.GetName() + "=" + std::to_string(sf.GetValue()) + "\n";
		}
		outputfile << "fleaMarketFee=" + std::to_string(this->m_ItemsData.m_fleaMarketFee) + "\n";
		this->m_ItemsData.m_properties.OutputData(outputfile);
	}
	//
	void ItemData::SaveAsNewData2(std::string Path) noexcept {
		bool maked = false;
		for (auto& jd : GetJsonDataList()) {
			if (!jd->m_IsFileOpened) {
				std::string ParentPath = Path + (dynamic_cast<ItemJsonData*>(jd.get()))->m_categorytypes;

				if (!maked) {
					CreateDirectory(ParentPath.c_str(), NULL);
					maked = true;
				}

				std::string ChildPath = ParentPath + "/";

				std::string FileName = jd->m_name;
				SubStrs(&FileName, ".");
				SubStrs(&FileName, "\\");
				SubStrs(&FileName, "/");
				SubStrs(&FileName, ":");
				SubStrs(&FileName, "*");
				SubStrs(&FileName, "?");
				SubStrs(&FileName, "\"");
				SubStrs(&FileName, ">");
				SubStrs(&FileName, "<");
				SubStrs(&FileName, "|");
				std::string Name = FileName + ".txt";

				jd->OutputData(ChildPath + Name);
				//RemoveDirectory(Path.c_str());
			}
		}
	}
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