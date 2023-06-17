#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	void CustomBG::CalcChild(const ItemList* Ptr) noexcept {
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			for (auto& cID : this->m_ChildData) {
				cID.ResetWatchCounter();
			}
			Ptr_Buf = this->m_BaseWeapon;
		}
		for (const auto& c : Ptr_Buf->GetChildParts()) {
			auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
			bool IsHit = false;
			for (auto& cID : this->m_ChildData) {
				if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
					cID.AddWatchCounter();
					if (cID.GetIsSelected()) {
						CalcChild(cID.GetChildPtr());
					}
					IsHit = true;
					break;
				}
			}
			if (!IsHit) {
				//設定vectorに追加
				m_ChildData.resize(m_ChildData.size() + 1);
				m_ChildData.back().Set(Ptr_Buf, Index, 0);
				//
				auto& cID = this->m_ChildData.back();
				for (const auto& cID2 : this->m_ChildData.back().GetMySlotData().GetData()) {
					if (PlayerData::Instance()->GetItemLock(DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())->GetIDstr().c_str())) {
						break;
					}
					cID.AddSelect();
				}
				//
				while (true) {
					if (cID.GetIsSelected() && CheckConflict(cID.GetChildPtr())) {
						cID.AddSelect();
						continue;
					}
					break;
				}
			}
		}
		if (Ptr == nullptr) {
			//未使用品を除外
			for (int count = 0; count < this->m_ChildData.size(); count++) {
				if (!m_ChildData[count].IsWatch()) {
					m_ChildData.erase(m_ChildData.begin() + count);
					count--;
				}
			}
			//フィルターに引っかかっていたら大丈夫なものにする
			for (auto& cID : this->m_ChildData) {
				while (true) {
					if (cID.GetIsSelected() && cID.ChecktoBeFiltered(-1, !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
						cID.AddSelect();
						continue;
					}
					break;
				}
			}
			//性能更新
			{
				float RecoilPer = 0.f;
				float ErgonomicsPer = 0.f;
				for (const auto& cID : this->m_ChildData) {
					if (cID.GetIsSelected()) {
						auto* ChildPtr = cID.GetChildPtr();
						RecoilPer += ChildPtr->GetRecoil();
						ErgonomicsPer += ChildPtr->GetErgonomics();
					}
				}
				m_Recoil = ((float)(m_BaseWeapon->GetRecoilVertical())*(100.f + RecoilPer) / 100.f);
				m_Ergonomics = (m_BaseWeapon->GetWeaponErgonomics() + ErgonomicsPer);
			}
		}
	}
	//
	void CustomBG::CalcChildErgRec(std::vector<std::vector<std::vector<PartsBaseData>>>* Data, int BaseID, int BaseNest, const ItemList* Ptr) noexcept {
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			Data->clear();
			Ptr_Buf = this->m_BaseWeapon;
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				Data->resize(Data->size() + 1);//こどもの分岐
				for (auto& cptr : c.GetData()) {
					//フィルターに引っかかってなければOK
					if (!ChildData::ItemPtrChecktoBeFiltered(DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()), !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
						Data->back().resize(Data->back().size() + 1);
						CalcChildErgRec(Data, (int)Data->back().size() - 1, (int)Data->back().back().size(), DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()));
					}
				}
				for (int i = 0; i < Data->back().size(); i++) {
					if (Data->back()[i].size() == 0) {
						std::swap(Data->back()[i], Data->back().back());
						Data->back().pop_back();
						i--;
					}
				}
			}
		}
		if (Ptr != nullptr) {
			Data->back().back().resize(Data->back().back().size() + 1);
			Data->back().back().back().m_PartsID.emplace_back(Ptr_Buf->GetName());
			Data->back().back().back().m_RecoilPer = Data->back().at(BaseID).back().m_RecoilPer + Ptr_Buf->GetRecoil();
			Data->back().back().back().m_ErgonomicsPer = Data->back().at(BaseID).back().m_ErgonomicsPer + Ptr_Buf->GetErgonomics();

			bool IsChild = false;
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				for (auto& cptr : c.GetData()) {
					//フィルターに引っかかってなければOK
					if (!ChildData::ItemPtrChecktoBeFiltered(DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()), !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
						IsChild = true;
						CalcChildErgRec(Data, (int)Data->back().size() - 1, (int)Data->back().back().size(), DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()));
					}
				}
			}
			if (!IsChild) {//次のパターンへ
				Data->back().resize(Data->back().size() + 1);
				for (int i = 0; i < BaseNest; i++) {
					Data->back().back().emplace_back(Data->back().at(BaseID)[i]);
				}
			}
		}
	}
	//
	void CustomBG::CalcChildErgRec(std::vector<PartsBaseData>* AnsData, const ItemList* Ptr) noexcept {
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			Ptr_Buf = this->m_BaseWeapon;
			AnsData->resize(AnsData->size() + 1);
		}
		int Now = 0;
		for (const auto& c : Ptr_Buf->GetChildParts()) {
			for (auto& cptr : c.GetData()) {
				//フィルターに引っかかってなければOK
				if (!ChildData::ItemPtrChecktoBeFiltered(DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()), !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
					CalcChildErgRec(AnsData, DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()));
					Now++;
				}
			}
		}
		AnsData->back().m_PartsID.emplace_back(Ptr_Buf->GetName());
		AnsData->back().m_RecoilPer += Ptr_Buf->GetRecoil();
		AnsData->back().m_ErgonomicsPer += Ptr_Buf->GetErgonomics();
		if (Now == 0) {
			AnsData->resize(AnsData->size() + 1);
		}
	}
	//描画
	bool CustomBG::DrawChild(int xposbase, int yposbase, int xpos, int ypos, float Scale, int* Lane, int Nest, const ItemList* Ptr) noexcept {
		auto* Input = InputControl::Instance();

		bool HaveChild = false;
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			for (auto& cID : this->m_ChildData) {
				cID.ResetWatchCounter();
			}
			Ptr_Buf = this->m_BaseWeapon;
			m_posxMaxBuffer = 0;
			m_posyMaxBuffer = 0;
		}
		for (const auto& c : Ptr_Buf->GetChildParts()) {
			auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
			for (auto& cID : this->m_ChildData) {
				if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
					HaveChild = true;

					int xsize = (int)((float)(600)*Scale);// y_r(400);
					int xsizeMin = (int)((float)(30)*Scale);// y_r(400);
					int ysize = (int)((float)(64) * Scale);// y_r(92);
					int xbase = xposbase + Nest * (xsize + (int)((float)(y_r(30))*Scale / 0.2f));
					int yp2 = yposbase + (*Lane) * (ysize + (int)((float)(y_r(10))*Scale / 0.2f));
					int ybase = yp2 - ysize / 2;
					if (Ptr != nullptr) {
						DrawControl::Instance()->SetDrawLine(DrawLayer::Normal, xpos, ypos, xbase, yp2, Red, y_r(3));
					}
					auto* WindowMngr = WindowSystem::WindowManager::Instance();
					if (WindowSystem::ClickCheckBox(xbase, ybase, xbase + xsizeMin, ybase + ysize, true, !WindowMngr->PosHitCheck(nullptr), Gray25, "<")) {
						cID.SubSelect();
						while (true) {
							if (cID.GetIsSelected() && CheckConflict(cID.GetChildPtr())) {
								cID.SubSelect();
								continue;
							}
							break;
						}
					}
					if (WindowSystem::ClickCheckBox(xbase + xsize - xsizeMin, ybase, xbase + xsize, ybase + ysize, true, !WindowMngr->PosHitCheck(nullptr), Gray25, ">")) {
						cID.AddSelect();
						while (true) {
							if (cID.GetIsSelected() && CheckConflict(cID.GetChildPtr())) {
								cID.AddSelect();
								continue;
							}
							break;
						}
					}
					if (WindowSystem::ClickCheckBox(xbase + xsizeMin, ybase, xbase + xsize - xsizeMin, ybase + ysize, true, !WindowMngr->PosHitCheck(nullptr), Gray10, "None")) {
						cID.OnOffSelect();
						while (true) {
							if (cID.GetIsSelected() && CheckConflict(cID.GetChildPtr())) {
								cID.AddSelect();
								continue;
							}
							break;
						}
					}

					m_posxMaxBuffer = std::max(m_posxMaxBuffer, xbase + xsize);
					m_posyMaxBuffer = std::max(m_posyMaxBuffer, ybase + ysize);

					//
					if (cID.GetIsSelected()) {
						const_cast<ItemList*>(cID.GetChildPtr())->Draw(xbase + xsizeMin, ybase, xsize - xsizeMin * 2, ysize, 0, Gray10, false, false, true, false);

						if (DrawChild(xposbase, yposbase, xbase + xsize, yp2, Scale, Lane, Nest + 1, cID.GetChildPtr())) {
							(*Lane)--;
						}

						if (in2_(Input->GetMouseX(), Input->GetMouseY(), xbase, ybase, xbase + xsize, ybase + ysize)) {
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xbase, ybase, xbase + xsize, ybase + ysize, RedPop, false);
							if (Input->GetKey('L').trigger()) {
								//ロックをかける
								for (const auto& cID2 : cID.GetMySlotData().GetData()) {
									if (cID.GetChildPtr() != DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())) {
										PlayerData::Instance()->SetItemLock(DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())->GetIDstr().c_str(), false);
									}
								}
								PlayerData::Instance()->OnOffItemLock(cID.GetChildPtr()->GetIDstr().c_str());
							}
							DrawControl::Instance()->SetString(DrawLayer::Front,
								FontPool::FontType::Nomal_Edge, LineHeight,
								STRX_RIGHT, STRY_BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
								"Lキーで優先パーツに設定"
							);
						}
					}
					//
					if (in2_(Input->GetMouseX(), Input->GetMouseY(), xbase, ybase, xbase + xsize, ybase + ysize)) {
						m_SpecChange = false;
						m_RecAddMin = 1000.f;
						m_RecAddMax = -1000.f;
						m_ErgAddMin = 1000.f;
						m_ErgAddMax = -1000.f;
						for (const auto& cID2 : cID.GetMySlotData().GetData()) {
							auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID());
							if (!CheckConflict(ptr)) {
								if (m_RecAddMin > ptr->GetRecoil()) {
									m_RecAddMin = ptr->GetRecoil();
								}
								if (m_RecAddMax < ptr->GetRecoil()) {
									m_RecAddMax = ptr->GetRecoil();
								}
								if (m_ErgAddMin > ptr->GetErgonomics()) {
									m_ErgAddMin = ptr->GetErgonomics();
								}
								if (m_ErgAddMax < ptr->GetErgonomics()) {
									m_ErgAddMax = ptr->GetErgonomics();
								}
								m_SpecChange = true;
							}
						}
						if (cID.GetIsSelected()) {
							m_RecAddMin -= cID.GetChildPtr()->GetRecoil();
							m_RecAddMax -= cID.GetChildPtr()->GetRecoil();
							m_ErgAddMin -= cID.GetChildPtr()->GetErgonomics();
							m_ErgAddMax -= cID.GetChildPtr()->GetErgonomics();
						}
					}
					//
					int xOfset = xsizeMin;
					int Hight = LineHeight * 3 / 4;
					for (const auto& t : cID.GetMySlotData().GetTypeID()) {
						int xOfsetAdd = WindowSystem::GetMsgLen(Hight, "[%s]", DataBase::Instance()->GetItemTypeData()->FindPtr(t)->GetName().c_str());
						if (xOfset + xOfsetAdd > (xsize - xsizeMin * 2)) {
							break;
						}
						WindowSystem::SetMsg(xbase + xOfset, ybase, xbase + xOfset, ybase + Hight, Hight, STRX_LEFT, White, Black,
							"[%s]", DataBase::Instance()->GetItemTypeData()->FindPtr(t)->GetName().c_str());
						xOfset += xOfsetAdd;
					}

					(*Lane)++;
					break;
				}
			}
		}
		return HaveChild;
	}

	void CustomBG::Init_Sub(int *posx, int *posy, float* Scale) noexcept {
		*posx = y_r(100);
		*posy = LineHeight + y_r(100);
		*Scale = 0.2f;
		m_SelectPreset = InvalidID;
		m_SelectBuffer = InvalidID;
		m_BaseWeapon = nullptr;

		m_EnableMag = true;
		m_EnableMount = true;
		m_EnableSight = true;
		m_Recoil = 50.f;
		m_Ergonomics = 50.f;

		m_ChildData.clear();

		InitLists(3, y_r(1920 - 10) - y_r(400), LineHeight + y_r(5), y_r(400));
	}
	void CustomBG::LateExecute_Sub(int*, int*, float*) noexcept {
		if (m_BaseWeapon) {
			//プリセットを適応
			if (m_ChildData.size() == 0) {
				if (m_SelectPreset != InvalidID) {
					AttachPreset(*DataBase::Instance()->GetPresetData()->FindPtr(m_SelectPreset));
				}
			}
			//
			m_SpecChange = false;
			//設定
			CalcChild();
			//パターンを検索
			m_PartsChange = false;//これで無効化
			if (m_PartsChange) {
				m_PartsChange = false;
				//float RecoilPer2 = this->m_Recoil * 100.f / (float)(this->m_BaseWeapon->GetRecoilVertical()) - 100.f;
				//float ErgonomicsPer2 = (m_Ergonomics - this->m_BaseWeapon->GetWeaponErgonomics());

				std::vector<std::vector<std::vector<PartsBaseData>>>	PartsDatas;
				CalcChildErgRec(&PartsDatas);

				std::vector<PartsBaseData> Tmp;
				CalcChildErgRec(&Tmp);

				m_PartsBaseData.clear();
				for (auto& parents : PartsDatas) {
					m_PartsBaseData.resize(m_PartsBaseData.size() + 1);
					auto& back = this->m_PartsBaseData.back();
					for (auto& Base : parents) {
						back.resize(back.size() + 1);
						back.back().m_RecoilPer = 0.f;
						back.back().m_ErgonomicsPer = 0.f;
						for (auto& p : Base) {
							back.back().m_PartsID.emplace_back(p.m_PartsID[0]);
							back.back().m_RecoilPer += p.m_RecoilPer;
							back.back().m_ErgonomicsPer += p.m_ErgonomicsPer;
						}
					}
				}
				//
				m_PartsResultData.clear();
				m_PartsResultData.reserve(5000000);
				m_PartsSeek.resize(m_PartsBaseData.size());
				for (auto& seeks : this->m_PartsSeek) { seeks = 0; }
				while (true) {
					m_PartsResultData.resize(m_PartsResultData.size() + 1);
					m_PartsResultData.back().m_PartsID.clear();
					m_PartsResultData.back().m_RecoilPer = 0.f;
					m_PartsResultData.back().m_ErgonomicsPer = 0.f;

					for (auto& parents : this->m_PartsBaseData) {
						auto& Base = parents[m_PartsSeek[(int)(&parents - &m_PartsBaseData.front())]];
						for (auto& p : Base.m_PartsID) {
							m_PartsResultData.back().m_PartsID.emplace_back(p);
						}
						m_PartsResultData.back().m_RecoilPer += Base.m_RecoilPer;
						m_PartsResultData.back().m_ErgonomicsPer += Base.m_ErgonomicsPer;
					}
					AddPartsSeek((int)(m_PartsSeek.size()) - 1);
					int count = 0;
					for (auto& seeks : this->m_PartsSeek) { count += seeks; }
					if (count == 0) { break; }

					ProcessMessage();
				}
			}
		}
		else {
			m_PartsResultData.clear();
			m_PartsChange = false;
		}
	}
	void CustomBG::Draw_Back_Sub(int xpos, int ypos, float scale) noexcept {
		if (m_BaseWeapon) {
			if (m_BaseWeapon->GetIcon().GetGraph()) {
				float Scale = ((float)y_r(1080) / 128) * scale;
				DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, this->m_BaseWeapon->GetIcon().GetGraph(), xpos + (int)((float)y_r(960)*scale / 0.2f), ypos + (int)((float)y_r(540)*scale / 0.2f), Scale, 0.f, false);
				int Lane = 0;
				DrawChild(xpos, ypos, 0, 0, Scale, &Lane);
			}
		}
		//
		{
			int xgoal = 0;
			int xs_add = m_ListXSize + y_r(50);
			bool isChild = false;
			isChild |= MakeLists(0, true, [&](std::pair<int, bool>* IDs, bool IsChild) {
				if (IsChild) { xgoal -= xs_add; }
				MakeList<ItemTypeList>(m_ListXPos + xgoal, m_ListYPos, DataBase::Instance()->GetItemTypeData()->GetList(), "ItemType", &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (ptr->GetCategoryID() == DataBase::Instance()->GetItemCategoryData()->FindID("Weapons")); });
			});
			isChild |= MakeLists(1, true, [&](std::pair<int, bool>* IDs, bool IsChild) {
				if (IsChild) { xgoal -= xs_add; }
				MakeList<ItemList>(m_ListXPos + xgoal, m_ListYPos, DataBase::Instance()->GetItemData()->GetList(), "Item", &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (!ptr->GetIsPreset()) && (ptr->GetTypeID() == ListsSel(1 - 1)); });
			});
			isChild |= MakeLists(2, false, [&](std::pair<int, bool>* IDs, bool IsChild) {
				if (IsChild) { xgoal -= xs_add; }
				MakeList<PresetList>(m_ListXPos + xgoal, m_ListYPos, DataBase::Instance()->GetPresetData()->GetList(), "Preset", &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (ptr->GetBase()->GetID() == ListsSel(2 - 1)); });
			});
			if ((ListsSel(1) != InvalidID) && (ListsSel(2) != InvalidID)) {
				xgoal -= xs_add * 3;
			}
			ExecuteLists(isChild, xgoal);
		}
		//List
		{
			if ((ListsSel(2) != InvalidID) && (ListsSel(2) != this->m_SelectPreset)) {
				m_ChildData.clear();
			}
			m_SelectPreset = ListsSel(2);
		}
		{
			if (ListsSel(1) != this->m_SelectBuffer) {
				m_BaseWeapon = (ListsSel(1) != InvalidID) ? DataBase::Instance()->GetItemData()->FindPtr(ListsSel(1)) : nullptr;
				if (m_BaseWeapon == nullptr) {
					m_ChildData.clear();
				}
			}
			m_SelectBuffer = ListsSel(1);
		}
	}
	void CustomBG::DrawFront_Sub(int posx, int posy, float) noexcept {
		auto* DrawParts = DXDraw::Instance();
		//
		{
			int xp = y_r(10);
			int yp = LineHeight + y_r(10);
			if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
				if (!BackLists()) {
					TurnOnGoNextBG();
				}
			}
		}
		//下から上に
		if (m_BaseWeapon) {
			bool PrevSight = this->m_EnableSight;
			bool PrevMount = this->m_EnableMount;
			bool PrevMag = this->m_EnableMag;


			int xp = LineHeight;
			int yp = y_r(1080) - LineHeight;
			//
			yp -= y_r(80);
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "エルゴノミクス");
			int Erg = (int)m_Ergonomics, OldE = Erg;
			//WindowSystem::UpDownBar(xp, y_r(640), yp + LineHeight + y_r(5), &Erg, 0, 100);
			if (m_BaseWeapon) {
				int xmin = xp;
				int xmax = y_r(640);
				int ypos = yp + LineHeight + y_r(5);
				int valueMin = 0;
				int valueMax = 100;

				float ErgonomicsPer2 = (m_Ergonomics - this->m_BaseWeapon->GetWeaponErgonomics());
				float ErgMin = (m_BaseWeapon->GetWeaponErgonomics() + ErgonomicsPer2 + this->m_ErgAddMin);
				float ErgMax = (m_BaseWeapon->GetWeaponErgonomics() + ErgonomicsPer2 + this->m_ErgAddMax);

				int xp_t = 0;
				{
					int xpmin = xmin + LineHeight + 1;
					int xpmax = xmax - 1;
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin), ypos + LineHeight, DarkGreen);
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Green);
					if (m_SpecChange) {
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp((int)ErgMin - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Red);
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp((int)ErgMax - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Blue);
					}

					DrawControl::Instance()->SetDrawLine(DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Black, 3);
					DrawControl::Instance()->SetDrawLine(DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Gray15, 1);
				}
				/*
				xp_t = xmax;
				if (WindowSystem::ClickCheckBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, true, true, Gray25, "△")) {
					Erg = std::min(Erg + 1, valueMax);
				}
				//*/
				xp_t = (xmin + (xmax - xmin) / 2);
				if (m_SpecChange) {
					if ((int)ErgMin == (int)m_Ergonomics) {
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, STRX_MID, White, Black, "               <%5.2f", ErgMax);
					}
					else if ((int)ErgMax == (int)m_Ergonomics) {
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<               ", ErgMin);
					}
					else {
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<            <%5.2f", ErgMin, ErgMax);
					}
				}
				WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight, STRX_MID, White, Black, "%5.2f", this->m_Ergonomics);
				/*
				xp_t = xmin;
				if (WindowSystem::ClickCheckBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, true, true, Gray25, "▽")) {
					Erg = std::max(Erg - 1, valueMin);
				}
				//*/
			}

			if (OldE != Erg) {
				m_Ergonomics = (float)Erg;
			}
			//
			yp -= y_r(80);
			WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "縦リコイル");
			int Rec = (int)m_Recoil, OldR = Rec;
			//WindowSystem::UpDownBar(xp, y_r(640), yp + LineHeight + y_r(5), &Rec, 10, 200);
			if (m_BaseWeapon) {
				int xmin = xp;
				int xmax = y_r(640);
				int ypos = yp + LineHeight + y_r(5);
				int valueMin = 0;
				int valueMax = 100;

				float RecoilPer2 = this->m_Recoil * 100.f / (float)(this->m_BaseWeapon->GetRecoilVertical()) - 100.f;
				float RecMin = ((float)(m_BaseWeapon->GetRecoilVertical())*(100.f + RecoilPer2 + this->m_RecAddMin) / 100.f);
				float RecMax = ((float)(m_BaseWeapon->GetRecoilVertical())*(100.f + RecoilPer2 + this->m_RecAddMax) / 100.f);

				int xp_t = 0;
				{
					int xpmin = xmin + LineHeight + 1;
					int xpmax = xmax - 1;
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin), ypos + LineHeight, DarkGreen);
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Green);
					if (m_SpecChange) {
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp((int)RecMin - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Blue);
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp((int)RecMax - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Red);
					}

					DrawControl::Instance()->SetDrawLine(DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Black, 3);
					DrawControl::Instance()->SetDrawLine(DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Gray15, 1);
				}
				/*
				xp_t = xmax;
				if (WindowSystem::ClickCheckBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, true, true, Gray25, "△")) {
					Rec = std::min(Rec + 1, valueMax);
				}
				//*/
				xp_t = (xmin + (xmax - xmin) / 2);
				if (m_SpecChange) {
					if ((int)RecMin == (int)m_Recoil) {
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, STRX_MID, White, Black, "               <%5.2f", RecMax);
					}
					else if ((int)RecMax == (int)m_Recoil) {
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<               ", RecMin);
					}
					else {
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<            <%5.2f", RecMin, RecMax);
					}
				}
				WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight, STRX_MID, White, Black, "%5.2f", this->m_Recoil);
				/*
				xp_t = xmin;
				if (WindowSystem::ClickCheckBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, true, true, Gray25, "▽")) {
					Rec = std::max(Rec - 1, valueMin);
				}
				//*/
			}
			if (OldR != Rec) { this->m_Recoil = (float)Rec; }
			//
			yp -= y_r(50);
			WindowSystem::CheckBox(xp, yp, &m_EnableSight);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "サイトを含む");
			//
			yp -= y_r(50);
			WindowSystem::CheckBox(xp, yp, &m_EnableMount);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "マウントを含む");
			//
			yp -= y_r(50);
			WindowSystem::CheckBox(xp, yp, &m_EnableMag);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "マガジンを含む");
			//
			if (m_EnableSight) {
				m_EnableMount = true;
			}
			if (!m_EnableMount) {
				m_EnableSight = false;
			}
			if ((PrevSight != this->m_EnableSight) || (PrevMount != this->m_EnableMount) || (PrevMag != this->m_EnableMag)) {
				m_PartsChange = true;
			}

		}
		//場所ガイド
		if (m_BaseWeapon) {
			if (m_BaseWeapon->GetIcon().GetGraph()) {
				int xp = y_r(1440);
				int yp = y_r(820);

				int xs = y_r(320);
				int ys = y_r(180);

				int x_p1 = std::max(posx * xs / DrawParts->m_DispXSize, -xs / 2);
				int y_p1 = std::max(posy * ys / DrawParts->m_DispYSize, -ys / 2);
				int x_p2 = std::min(this->m_posxMaxBuffer * xs / DrawParts->m_DispXSize, xs + xs / 2);
				int y_p2 = std::min(this->m_posyMaxBuffer * ys / DrawParts->m_DispYSize, ys + ys / 2);

				DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 64);
				DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Black, TRUE);
				DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
				DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Green, FALSE);
				DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xp, yp, xp + xs, yp + ys, Red, FALSE);
			}
		}
		//
	}
};
