#include"../../../Header.hpp"

namespace FPS_n2 {
	void			ItemList::SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
		if (LEFT == "Itemtype") { m_TypeArg = Args[0]; }

		else if (LEFT == "Map") { m_MapArgs.emplace_back(Args[0]); }

		else if (LEFT == "ChildParts") {
			m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
			for (auto&a : Args) {
				bool isHit = false;
				for (auto& d : m_ChildPartsID.back().Data) {
					if (d.second == a) {
						isHit = true;
						break;
					}
				}
				if (!isHit) {
					m_ChildPartsID.back().Data.resize(m_ChildPartsID.back().Data.size() + 1);
					m_ChildPartsID.back().Data.back().second = a;
				}
			}
		}
		else if (LEFT == "Conflict") {
			for (auto&a : Args) {
				bool isHit = false;
				for (auto& d : m_ConflictPartsID) {
					if (d.second == a) {
						isHit = true;
						break;
					}
				}
				if (!isHit) {
					m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
					m_ConflictPartsID.back().second = a;
				}
			}
		}
		else if (LEFT == "Recoil") {
			auto div = Args[0].find("+");
			if (div != std::string::npos) {
				m_Recoil = std::stof(Args[0].substr(div + 1));
			}
			else {
				m_Recoil = std::stof(Args[0]);
			}
		}
		else if (LEFT == "Ergonomics") {
			auto div = Args[0].find("+");
			if (div != std::string::npos) {
				m_Ergonomics = std::stof(Args[0].substr(div + 1));
			}
			else {
				m_Ergonomics = std::stof(Args[0]);
			}
		}
		else if (LEFT == "SightRange") { m_SightRange = std::stoi(Args[0]); }

		else if (LEFT == "basePrice") { m_basePrice = std::stoi(Args[0]); }
		else if (LEFT == "width") { m_width = std::stoi(Args[0]); }
		else if (LEFT == "height") { m_height = std::stoi(Args[0]); }
		else if (LEFT.find("Sell_") != std::string::npos) { m_selArgs.emplace_back(std::make_pair(LEFT, Args[0])); }
		else if (LEFT == "weight") { m_weight = std::stof(Args[0]); }
		else if (LEFT == "fleaMarketFee") { m_fleaMarketFee = std::stoi(Args[0]); }
		else if (LEFT == "propertiestype") { m_IsPreset = (Args[0] == ItemPropertiesStr[(int)EnumItemProperties::ItemPropertiesPreset]); }
	}
	void			ItemList::Load_Sub() noexcept {
		m_TypeID = ItemTypeData::Instance()->FindID(m_TypeArg.c_str());
		for (auto& m : m_MapArgs) {
			m_MapID.emplace_back(MapData::Instance()->FindID(m));
		}
		for (auto s : m_selArgs) {
			bool isHit = false;
			for (auto& sf : TraderData::Instance()->GetList()) {
				if (s.first == "Sell_" + sf.GetName()) {
					m_sellFor.emplace_back(std::make_pair(sf.GetID(), std::stoi(s.second)));
					isHit = true;
					break;
				}
			}
			if (!isHit) {
				if (s.first == "Sell_Flea Market") {
					m_sellFor.emplace_back(std::make_pair(InvalidID, std::stoi(s.second)));
				}
			}
		}
		//
		if (m_TypeID != InvalidID) {
			auto* typePtr = ItemTypeData::Instance()->FindPtr(this->m_TypeID);
			auto* catPtr = ItemCategoryData::Instance()->FindPtr(typePtr->GetCategoryID());
			if (catPtr->GetName() == "Weapons") {
				m_isWeapon = true;
			}
			else if (catPtr->GetName() == "WeaponMods") {
				m_isWeaponMod = true;
			}
		}
		else {
			std::string ErrMes = "Error : Not Setting ItemType in Item ";
			ErrMes += GetShortName();
			DataErrorLog::Instance()->AddLog(ErrMes.c_str());
		}
	}
	void			ItemList::SetParent() noexcept {
		for (auto& cp : m_ChildPartsID) {
			for (auto& c : cp.Data) {
				for (const auto& t : ItemData::Instance()->GetList()) {
					if (c.second == t.GetName()) {
						c.first = &t;
						break;
					}
				}
				if (c.first == nullptr) {
					std::string ErrMes = "Error : Invalid ChildPartsID[";
					ErrMes += GetShortName();
					ErrMes += "][";
					ErrMes += c.second;
					ErrMes += "]";

					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
		m_ParentPartsID.clear();
		for (const auto& t : ItemData::Instance()->GetList()) {
			for (auto& cp : t.m_ChildPartsID) {
				for (auto& c : cp.Data) {
					if (c.first == this) {
						m_ParentPartsID.emplace_back(&t);
					}
				}
			}
		}
		//
		for (auto& cp : m_ChildPartsID) {
			for (const auto& c : cp.Data) {
				bool isHit = false;
				for (const auto& t : cp.TypeID) {
					if (c.first && t == c.first->GetTypeID()) {
						isHit = true;
						break;
					}
				}
				if (!isHit && c.first) {
					cp.TypeID.emplace_back(c.first->GetTypeID());
				}
			}
		}
		//
		for (auto& cp : m_ConflictPartsID) {
			for (const auto& t : ItemData::Instance()->GetList()) {
				if (cp.second == t.GetName()) {
					cp.first = &t;
					break;
				}
			}
			if (cp.first == nullptr) {
				std::string ErrMes = "Error : Invalid ConflictPartsID[";
				ErrMes += GetShortName();
				ErrMes += "][";
				ErrMes += cp.second;
				ErrMes += "]";

				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		//自分を干渉相手にしている奴を探してそいつもリストに入れる　相思相愛
		for (const auto& t : ItemData::Instance()->GetList()) {
			for (auto& cp : t.GetConflictParts()) {
				if (cp.first == this) {
					m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
					m_ConflictPartsID.back().first = &t;
					m_ConflictPartsID.back().second = t.GetName();
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
						Name = Name.substr(0, Name.size() * (xs - (GraphXsize + FirXSize)) / StrXSize - 2) + "…";
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
			//ハイドアウト
			{
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STRX_LEFT, White, Black,
					"ハイドアウト開放:")); yofs += LineHeight + y_r(5);
				//ハイドアウト開放
				for (auto&L : HideoutData::Instance()->SetList()) {
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
				for (auto&L : HideoutData::Instance()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (int)((float)y_r(64));

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
										auto* ptr = ItemData::Instance()->FindPtr(I.GetID());
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
										auto* ptr = ItemData::Instance()->FindPtr(I.GetID());
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
				for (auto&L : HideoutData::Instance()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (int)((float)y_r(64));

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
										auto* ptr = ItemData::Instance()->FindPtr(ID);
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
										auto* ptr = ItemData::Instance()->FindPtr(ID);
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
				for (auto&L : TraderData::Instance()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (int)((float)y_r(64));

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
										auto* ptr = ItemData::Instance()->FindPtr(ID);
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
										auto* ptr = ItemData::Instance()->FindPtr(ID);
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
				for (auto&L : TraderData::Instance()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						int xofs_buf = y_r(10);
						int ysize = (int)((float)y_r(64));

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
										auto* ptr = ItemData::Instance()->FindPtr(ID);
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
										auto* ptr = ItemData::Instance()->FindPtr(ID);
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

	//
	void ItemJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		shortName = data["shortName"];
		if (data.contains("description")) {
			if (!data["description"].is_null()) {
				description = data["description"];
			}
		}
		if (data.contains("basePrice")) {
			if (!data["basePrice"].is_null()) {
				basePrice = data["basePrice"];
			}
		}
		if (data.contains("width")) {
			width = data["width"];
		}
		if (data.contains("height")) {
			height = data["height"];
		}
		if (data.contains("types")) {
			for (const auto& ts : data["types"]) {
				types.emplace_back((std::string)ts);
			}
		}
		
		for (const auto& sf : data["sellFor"]) {
			std::string vendor = sf["vendor"]["name"];
			int price = sf["price"];
			sellFor.emplace_back(std::make_pair(vendor, price));
		}
		for (const auto& sf : data["buyFor"]) {
			std::string vendor = sf["vendor"]["name"];
			int price = sf["price"];
			buyFor.emplace_back(std::make_pair(vendor, price));
		}
		categorytypes = data["category"]["name"];
		weight = data["weight"];
		for (const auto& ts : data["conflictingItems"]) {
			conflictingItems.emplace_back((std::string)ts["name"]);
		}
		for (const auto& ts : data["usedInTasks"]) {
			usedInTasks.emplace_back((std::string)ts["name"]);
		}
		for (const auto& ts : data["receivedFromTasks"]) {
			receivedFromTasks.emplace_back((std::string)ts["name"]);
		}
		for (const auto& ts : data["bartersFor"]) {
			bartersFor.emplace_back((std::string)ts["trader"]["name"]);
		}
		for (const auto& ts : data["bartersUsing"]) {
			bartersUsing.emplace_back((std::string)ts["trader"]["name"]);
		}
		for (const auto& ts : data["craftsFor"]) {
			craftsFor.emplace_back((std::string)ts["station"]["name"]);
		}
		for (const auto& ts : data["craftsUsing"]) {
			craftsUsing.emplace_back((std::string)ts["station"]["name"]);
		}
		if (data.contains("fleaMarketFee")) {
			if (!data["fleaMarketFee"].is_null()) {
				fleaMarketFee = data["fleaMarketFee"];
			}
		}

		if (data.contains("properties")) {
			if (!data["properties"].is_null()) {
				m_properties.GetJsonData(data["properties"]);
			}
		}
	}
	void ItemJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		outputfile << "ShortName=" + this->shortName + "\n";
		outputfile << "Itemtype=" + this->categorytypes + "\n";

		if (this->m_properties.GetSightingRange() >= 0) {
			outputfile << "SightRange=" + std::to_string(this->m_properties.GetSightingRange()) + "\n";
		}
		if (this->m_properties.GetModSlots().size() > 0) {
			for (const auto& m : this->m_properties.GetModSlots()) {
				if (m.size() > 0) {
					outputfile << "ChildParts=[\n";
					std::vector<std::string> Names;
					for (auto& d : m) {
						auto NmBuf = d;
						if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
							outputfile << "\t" + NmBuf + ((&d != &m.back()) ? DIV_STR : "") + "\n";
							Names.emplace_back(NmBuf);
						}
					}
					outputfile << "]\n";
				}
			}
		}

		if (this->conflictingItems.size() > 0) {
			bool isHit = false;
			std::vector<std::string> Names;
			for (auto& m : this->conflictingItems) {
				auto NmBuf = m;
				if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
					if (!isHit) {
						isHit = true;
						outputfile << "Conflict=[\n";
					}
					outputfile << "\t" + NmBuf + ((&m != &this->conflictingItems.back()) ? DIV_STR : "") + "\n";
					Names.emplace_back(NmBuf);
				}
			}
			if (isHit) {
				outputfile << "]\n";
			}
		}

		if (this->m_properties.GetType() == EnumItemProperties::ItemPropertiesWeapon) {
			outputfile << "Recoil=" + std::to_string((float)this->m_properties.GetWeaponRecoilVertical()) + "\n";
			outputfile << "Ergonomics=" + std::to_string(this->m_properties.GetWeaponErgonomics()) + "\n";
		}
		else {
			switch (this->m_properties.GetType()) {
			case EnumItemProperties::ItemPropertiesBarrel:
			case EnumItemProperties::ItemPropertiesMagazine:
			case EnumItemProperties::ItemPropertiesScope:
			case EnumItemProperties::ItemPropertiesWeaponMod:
				outputfile << "Recoil=" + std::to_string((float)this->m_properties.GetModRecoil()*100.f) + "\n";
				outputfile << "Ergonomics=" + std::to_string(this->m_properties.GetModErgonomics()) + "\n";
				break;
			default:
				break;
			}
		}

		outputfile << "Information_Eng=" + this->description + "\n";
		outputfile << "basePrice=" + std::to_string(this->basePrice) + "\n";
		outputfile << "width=" + std::to_string(this->width) + "\n";
		outputfile << "height=" + std::to_string(this->height) + "\n";
		for (auto& sf : this->sellFor) {
			outputfile << "Sell_" + sf.first + "=" + std::to_string(sf.second) + "\n";
		}
		//for (auto& bf : this->buyFor) { outputfile << "Buy_" + bf.first + "=" + std::to_string(bf.second) + "\n"; }
		outputfile << "weight=" + std::to_string(this->weight) + "\n";
		//std::vector<std::string>					usedInTasks;
		//std::vector<std::string>					receivedFromTasks;
		//std::vector<std::string>					bartersFor;
		//std::vector<std::string>					bartersUsing;
		//std::vector<std::string>					craftsFor;
		//std::vector<std::string>					craftsUsing;
		outputfile << "fleaMarketFee=" + std::to_string(this->fleaMarketFee) + "\n";
		outputfile << "propertiestype=" + (std::string)(this->m_properties.GetTypeName()) + "\n";
	}
	//
};