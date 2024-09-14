#include "MainScene_PageCustom.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void				CustomParts::ChildData::OnOffSelect() noexcept {
		if (this->m_PartsOn == InvalidID) {
			if (GetIsSelected()) {
				this->m_PartsOn = this->ChildSel;
				this->ChildSel = (int)(GetMySlotData().GetData().size());
			}
			else {
				this->ChildSel = 0;
			}
		}
		else {
			bool IsHit = false;
			int  Now = 0;
			for (const auto& cID2 : GetMySlotData().GetData()) {
				if (PlayerData::Instance()->GetItemLock(DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())->GetIDstr().c_str())) {
					IsHit = true;
					break;
				}
				Now++;
			}
			this->ChildSel = IsHit ? Now : this->m_PartsOn;
			this->m_PartsOn = InvalidID;
		}
	}
	void				CustomParts::ChildData::AddSelect() noexcept {
		this->m_PartsOn = InvalidID;
		++this->ChildSel %= (GetMySlotData().GetData().size() + 1);
	}
	void				CustomParts::ChildData::SubSelect() noexcept {
		this->m_PartsOn = InvalidID;
		--this->ChildSel;
		if (this->ChildSel < 0) {
			this->ChildSel = (int)(GetMySlotData().GetData().size());
		}
	}
	//
	const ItemList*		CustomParts::ChildData::GetChildPtr(int parentslot) const noexcept {
		if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
			if (GetIsSelected(parentslot)) {
				return DataBase::Instance()->GetItemData()->FindPtr(this->GetMySlotData().GetData().at(this->ChildSel).GetID());
			}
		}
		return nullptr;
	}
	const bool			CustomParts::ChildData::ItemPtrChecktoBeFiltered(const ItemList* ptr, bool MagFilter, bool MountFilter, bool SightFilter) noexcept {
		bool IsHit = false;
		if (ptr) {
			if (MagFilter) {
				if (
					ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Magazine")
					) {
					IsHit = true;
				}
			}
			if (MountFilter) {
				if (
					ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Mount")
					) {
					IsHit = true;
				}
			}
			if (SightFilter) {
				if (
					ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Reflex sight")
					|| ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Compact reflex sight")
					|| ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Assault scope")
					|| ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Scope")
					|| ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Special scope")
					|| ptr->GetTypeID() == DataBase::Instance()->GetItemTypeData()->FindID("Iron sight")
					) {
					IsHit = true;
				}
			}
		}
		return IsHit;
	}
	const bool			CustomParts::ChildData::ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter) const noexcept {
		return ItemPtrChecktoBeFiltered(GetChildPtr(parentslot), MagFilter, MountFilter, SightFilter);
	}
	void				CustomParts::ChildData::Set(const ItemList* parentptr, int parentslot, int Select) noexcept {
		this->m_ParentPtr = parentptr;
		this->m_ParentSlot = parentslot;
		this->ChildSel = Select;
		this->watchCounter = 1;
	}
	//
	void CustomParts::AddSelectToCanSelect(ChildData* cID) noexcept	{
		while (true) {
			if (cID->GetIsSelected() && CheckConflict(cID->GetChildPtr())) {
				cID->AddSelect();
				continue;
			}
			break;
		}
	}
	void CustomParts::SubSelectToCanSelect(ChildData* cID) noexcept	{
		while (true) {
			if (cID->GetIsSelected() && CheckConflict(cID->GetChildPtr())) {
				cID->SubSelect();
				continue;
			}
			break;
		}
	}
	//プリセットを適応
	void CustomParts::AttachPreset(const PresetList& Preset, const ItemList* Ptr) noexcept {
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			m_ChildData.clear();
			Ptr_Buf = this->m_BaseWeapon;
		}
		for (int index = 0; const auto & c : Ptr_Buf->GetChildParts()) {
			bool IsHit = false;
			for (auto& cID : this->m_ChildData) {
				if (cID.GetPtrIsParentSlot(Ptr_Buf, index)) {
					if (cID.GetIsSelected() && cID.GetChildPtr()) {
						AttachPreset(Preset, cID.GetChildPtr());
					}
					IsHit = true;
					break;
				}
			}
			if (!IsHit) {
				//設定ベクターに追加
				int ChildSel = -1;
				for (const auto& P : Preset.GetParts()) {
					bool IsHit2 = false;
					for (int index2 = 0; auto & cptr : c.GetData()) {
						if (P == DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID())) {
							IsHit2 = true;
							ChildSel = index2;
							break;
						}
						index2++;
					}
					if (IsHit2) { break; }
				}
				if (ChildSel != -1) {
					//設定vectorに追加
					m_ChildData.resize(m_ChildData.size() + 1);
					m_ChildData.back().Set(Ptr_Buf, index, ChildSel);
					AttachPreset(Preset, this->m_ChildData.back().GetChildPtr());
				}
			}
			index++;
		}
	}
	//設定
	bool CustomParts::CheckConflict(const ItemList* MyPtr, const ItemList* Ptr) noexcept {
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			Ptr_Buf = this->m_BaseWeapon;
		}
		for (const auto& cP : Ptr_Buf->GetConflictParts()) {
			if (cP.GetID() == MyPtr->GetID()) {
				return true;
			}
		}
		for (int index = 0, Max = (int)Ptr_Buf->GetChildParts().size(); index < Max; index++) {
			for (const auto& cID : this->m_ChildData) {
				if (cID.GetPtrIsParentSlot(Ptr_Buf, index)) {
					if (cID.GetIsSelected() && CheckConflict(MyPtr, cID.GetChildPtr())) {
						return true;
					}
					break;
				}
			}
		}
		return false;
	}
	//
	void CustomParts::CalcChild(bool MagFilter, bool MountFilter, bool SightFilter, const ItemList* Ptr) noexcept {
		auto* Ptr_Buf = Ptr;
		if (Ptr == nullptr) {
			m_SpecChange = false;
			for (auto& cID : this->m_ChildData) {
				cID.ResetWatchCounter();
			}
			Ptr_Buf = this->m_BaseWeapon;
		}
		for (int index = 0, Max = (int)Ptr_Buf->GetChildParts().size(); index < Max; index++) {
			bool IsHit = false;
			for (auto& cID : this->m_ChildData) {
				if (cID.GetPtrIsParentSlot(Ptr_Buf, index)) {
					cID.AddWatchCounter();
					if (cID.GetIsSelected()) {
						CalcChild(MagFilter, MountFilter, SightFilter, cID.GetChildPtr());
					}
					IsHit = true;
					break;
				}
			}
			if (!IsHit) {
				//設定vectorに追加
				m_ChildData.resize(m_ChildData.size() + 1);
				m_ChildData.back().Set(Ptr_Buf, index, 0);
				//
				auto& cID = this->m_ChildData.back();
				for (const auto& cID2 : this->m_ChildData.back().GetMySlotData().GetData()) {
					if (PlayerData::Instance()->GetItemLock(DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())->GetIDstr().c_str())) {
						break;
					}
					cID.AddSelect();
				}
				//
				AddSelectToCanSelect(&cID);
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
					if (cID.GetIsSelected() && cID.ChecktoBeFiltered(-1, MagFilter, MountFilter, SightFilter)) {
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
	void CustomParts::CalcChildErgRec(
		std::vector<PartsBaseData>* Data, const ItemList* Ptr, ItemID ParentDataID, int ParentDataIndex, int slot) noexcept {
		auto* Ptr_Buf = Ptr;
		bool IsParent = (ParentDataIndex == -1);
		if (IsParent) {
			// 元祖
			Ptr_Buf = this->m_BaseWeapon;
		}
		else {
			//子パーツ
			Ptr_Buf = Ptr;
			if (!Ptr_Buf) { return; }
		}
		//現状のパーツのみ
		Data->resize(Data->size() + 1);

		// 枝を網羅(組み合わせ網羅ではない)
		{
			if (!IsParent) {
				Data->back() = Data->at(ParentDataIndex);
			}
			//何もつけない場合
			Data->back().m_PartsIDList.emplace_back(Ptr_Buf->GetID(), ParentDataID, slot);
			ParentDataIndex = (int)Data->size() - 1;
			//子パーツ
			bool Required = false;
			for (int index = 0; const auto & c : Ptr_Buf->GetChildParts()) {
				Required |= c.m_Required;
				if (!c.m_Required) { continue; }
				for (auto& cptr : c.GetData()) {
					CalcChildErgRec(Data, DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID()), Ptr_Buf->GetID(), ParentDataIndex, index);
				}
				index++;
			}
			//必要なパーツが付けられていないのでつけない場合を消す
			if (Required) {
				std::swap(Data->back(), Data->at(ParentDataIndex));
				Data->pop_back();
			}
		}

		// 枝をもとに組み合わせ網羅
		if (IsParent) {
			int BaseSize = (int)Data->size();
			int BaseMin = 0;
			int BaseMax = 0;
			for (int loop = 0; loop < 4; loop++) {
				BaseMin = BaseMax;
				BaseMax = (int)Data->size();
				for (int Branch1 = 0; Branch1 < BaseSize; Branch1++) {
					for (int Branch2 = BaseMin; Branch2 < BaseMax; Branch2++) {
						bool IsActive = false;
						for (int loop2 = 0, max2 = (int)Data->at(Branch2).m_PartsIDList.size(); loop2 < max2; loop2++) {
							//Branch1の方にあるデータに被っていないものだけ追加する
							auto& Parts2 = Data->at(Branch2).m_PartsIDList.at(loop2);
							if (loop2 < Data->at(Branch1).m_PartsIDList.size()) {
								if (Data->at(Branch1).m_PartsIDList.at(loop2) == Parts2) { continue; }
							}
							//被ってなくても同じスロットに被っている場合はスルー
							{
								bool IsThrough = false;
								for (int loop1 = 0, max1 = (int)Data->at(Branch1).m_PartsIDList.size(); loop1 < max1; loop1++) {
									auto& Parts1 = Data->at(Branch1).m_PartsIDList.at(loop1);
									if (Parts2.SlotNum == Parts1.SlotNum) {
										IsThrough = true;
										break;
									}
								}
								if (IsThrough) { continue; }
							}
							//適用
							if (!IsActive) {
								IsActive = true;
								Data->resize(Data->size() + 1);
								Data->back() = Data->at(Branch1);
							}
							Data->back().m_PartsIDList.emplace_back(Parts2);
						}
					}
				}

				continue;
				//パーツ間に干渉があったら省く
				for (int Branch1 = 0, Max = (int)Data->size(); Branch1 < Max; Branch1++) {
					auto& data = Data->at(Branch1);
					bool IsConflict = false;
					for (auto& parts : data.m_PartsIDList) {
						auto* pItem1 = DataBase::Instance()->GetItemData()->FindPtr(parts.MyID);
						for (auto& conf : pItem1->GetConflictParts()) {
							for (auto& parts2 : data.m_PartsIDList) {
								if (conf.GetID() == parts2.MyID) {
									IsConflict = true;
									break;
								}
							}
							if (IsConflict) {
								break;
							}
						}
						if (IsConflict) {
							break;
						}
					}
					if (IsConflict) {
						std::swap(Data->back(), Data->at(Branch1));
						Data->pop_back();
						Max--;
						Branch1--;
					}
				}

			}
		}

		//被っているものを排除する
		if (IsParent) {
			for (int index = 0, Max = (int)Data->size(); index < Max; index++) {
				auto& data = Data->at(index);
				bool IsSame = false;
				for (auto& data2 : *Data) {
					if (&data == &data2) { continue; }
					if (data.m_PartsIDList.size() != data2.m_PartsIDList.size()) { continue; }
					int max = (int)data.m_PartsIDList.size();
					bool isHit = false;
					for (int loop = 0; loop < max; loop++) {
						isHit = false;
						//要素がかぶらない場合は違うよ
						for (int loop2 = 0; loop2 < max; loop2++) {
							if (data.m_PartsIDList.at(loop) == data2.m_PartsIDList.at(loop2)) {
								isHit = true;
								break;
							}
						}
						if (!isHit) {
							break;
						}
					}
					if (isHit) {
						IsSame = true;
					}
				}
				if (IsSame) {
					std::swap(Data->back(), Data->at(index));
					Data->pop_back();
					Max--;
					index--;
				}
			}
		}



		//必須パーツが埋まっていない場合省く
		if (IsParent) {
			for (int index = 0, Max = (int)Data->size(); index < Max; index++) {
				auto& data = Data->at(index);
				bool SlotAllOk = true;
				for (auto& parts : data.m_PartsIDList) {
					auto* Ptr_t = DataBase::Instance()->GetItemData()->FindPtr(parts.MyID);
					if (Ptr_t) {
						for (const auto& c : Ptr_t->GetChildParts()) {
							if (c.m_Required) {//埋める必要のあるスロットがあって
								bool isHit = false;
								for (auto& parts2 : data.m_PartsIDList) {
									if (parts2.ParentID == parts.MyID) {
										//子オブジェの中でスロット対象のオブジェクトがあったら
										for (auto& cptr : c.GetData()) {
											if (cptr.GetID() == parts2.MyID) {
												isHit = true;
												break;
											}
										}
										if (isHit) {
											break;
										}
									}
									if (isHit) {
										break;
									}
								}
								//スロットが空なら
								if (!isHit) {
									SlotAllOk = false;
								}
							}
						}
						if (!SlotAllOk) {
							break;
						}
					}
				}
				if (!SlotAllOk) {
					std::swap(Data->back(), Data->at(index));
					Data->pop_back();
					Max--;
					index--;
				}
			}
		}

		//保存
		if (IsParent) {
			std::ofstream outputfile("Save/AAAA.txt");
			for (auto& data : *Data) {
				for (auto& parts : data.m_PartsIDList) {
					auto* pItem1 = DataBase::Instance()->GetItemData()->FindPtr(parts.MyID);
					outputfile << pItem1->GetName() + "\t,";
					/*
					if (&parts == &data.m_PartsIDList.back()) {
						auto* pItem1 = DataBase::Instance()->GetItemData()->FindPtr(parts.MyID);
						outputfile << pItem1->GetShortName() + "\t,";
					}
					else {
						outputfile << "\t,";
					}
					//*/
				}
				outputfile << "\n";
			}
			outputfile.close();
		}
	}
	//描画
	bool CustomParts::DrawChild(int XLeftPosition, int YMiddlePosition, float Scale, int parentXpos, int parentYpos, int* Lane, const ItemList* Ptr) noexcept {
		bool IsParent = (Ptr == nullptr);
		int LaneBase = 0;
		bool HaveChild = false;
		auto* Ptr_Buf = Ptr;
		if (IsParent) {
			Lane = &LaneBase;
			for (auto& cID : this->m_ChildData) {
				cID.ResetWatchCounter();
			}
			Ptr_Buf = this->m_BaseWeapon;
			m_posxMinBuffer = XLeftPosition;
			m_posyMinBuffer = YMiddlePosition;
			m_posxMaxBuffer = m_posxMinBuffer;
			m_posyMaxBuffer = m_posyMinBuffer;
		}
		int xsize = DXDraw::Instance()->GetUIY(600);
		int ysize = DXDraw::Instance()->GetUIY(64);

		for (int index = 0, Max = (int)Ptr_Buf->GetChildParts().size(); index < Max; index++) {
			for (auto& cID : this->m_ChildData) {
				if (cID.GetPtrIsParentSlot(Ptr_Buf, index)) {
					YMiddlePosition = m_posyMinBuffer + (*Lane) * (int)((float)(ysize + DXDraw::Instance()->GetUIY(50)) * Scale);
					if (!IsParent) {
						WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal, parentXpos, parentYpos, XLeftPosition, YMiddlePosition, Red, DXDraw::Instance()->GetUIY(3));
					}
					DrawChildOnce(&cID, XLeftPosition, YMiddlePosition, xsize, ysize, Scale, Lane);
					HaveChild = true;
					break;
				}
			}
		}
		return HaveChild;
	}
	//
	void CustomParts::DrawChildOnce(ChildData* cID, int XLeftPosition, int YMiddlePosition, int Xsize, int Ysize, float Scale, int* Lane) noexcept {
		auto* Pad = PadControl::Instance();
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		//
		Xsize = (int)((float)Xsize * Scale);
		Ysize = (int)((float)Ysize * Scale);
		int XRight = XLeftPosition + Xsize;
		int YUp = YMiddlePosition - Ysize / 2;
		int YBottom = YUp + Ysize;
		//最大サイズ更新
		m_posxMaxBuffer = std::max(m_posxMaxBuffer, XRight);
		m_posyMaxBuffer = std::max(m_posyMaxBuffer, YBottom);
		// サイド矢印
		int xsizeMin = (int)((float)DXDraw::Instance()->GetUIY(30) * Scale);
		int Xleft = XLeftPosition + xsizeMin;
		int Xright = XRight - xsizeMin;
		//
		if (WindowSystem::SetMsgClickBox(XLeftPosition, YUp, Xleft, YBottom, LineHeight, Gray25, true, !WindowMngr->PosHitCheck(nullptr), "<")) {
			cID->SubSelect();
			SubSelectToCanSelect(cID);
		}
		if (WindowSystem::SetMsgClickBox(Xright, YUp, XRight, YBottom, LineHeight, Gray25, true, !WindowMngr->PosHitCheck(nullptr), ">")) {
			cID->AddSelect();
			AddSelectToCanSelect(cID);
		}
		if (WindowSystem::SetMsgClickBox(Xleft, YUp, Xright, YBottom, (YBottom - YUp) * 2 / 3, Gray50, true, !WindowMngr->PosHitCheck(nullptr), "None")) {
			cID->OnOffSelect();
			AddSelectToCanSelect(cID);
		}
		//アイテムの描画
		if (cID->GetIsSelected()) {
			const_cast<ItemList*>(cID->GetChildPtr())->Draw(Xleft, YUp, (Xright - Xleft), (YBottom - YUp), 0, Gray10, false, false, true, false);
			if (DrawChild(
				XRight + (int)((float)(DXDraw::Instance()->GetUIY(150)) * Scale),
				YMiddlePosition,
				Scale,
				XRight, YMiddlePosition, Lane, cID->GetChildPtr())) {
				(*Lane)--;
			}
		}
		//アイテムタイプ
		{
			int xOfset = 0;
			int Hight = LineHeight * 3 / 4;
			for (const auto& t : cID->GetMySlotData().GetTypeID()) {
				std::string Name = DataBase::Instance()->GetItemTypeData()->FindPtr(t)->GetName();
				int xOfsetAdd = WindowSystem::GetMsgLen(Hight, "[%s]", Name.c_str());
				if (xOfset + xOfsetAdd > (Xright - Xleft)) {
					break;
				}
				WindowSystem::SetMsg(Xleft + xOfset, YUp + Hight / 2, Hight, STRX_LEFT, White, Black, "[%s]", Name.c_str());
				xOfset += xOfsetAdd;
			}
		}
		//
		if (IntoMouse(XLeftPosition, YUp, XRight, YBottom)) {
			if (cID->GetIsSelected()) {
				WindowSystem::DrawControl::Instance()->SetDrawBox(WindowSystem::DrawLayer::Normal, XLeftPosition, YUp, XRight, YBottom, RedPop, false);
				if (Pad->GetAtoZKey('L').trigger()) {
					//ロックをかける
					for (const auto& cID2 : cID->GetMySlotData().GetData()) {
						if (cID->GetChildPtr() != DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())) {
							PlayerData::Instance()->SetItemLock(DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())->GetIDstr().c_str(), false);
						}
					}
					PlayerData::Instance()->OnOffItemLock(cID->GetChildPtr()->GetIDstr().c_str());
				}
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
					"Lキーで優先パーツに設定"
				);
			}
			m_SpecChange = false;
			m_RecAddMin = 1000.f;
			m_RecAddMax = -1000.f;
			m_ErgAddMin = 1000.f;
			m_ErgAddMax = -1000.f;
			for (const auto& cID2 : cID->GetMySlotData().GetData()) {
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
			if (cID->GetIsSelected()) {
				m_RecAddMin -= cID->GetChildPtr()->GetRecoil();
				m_RecAddMax -= cID->GetChildPtr()->GetRecoil();
				m_ErgAddMin -= cID->GetChildPtr()->GetErgonomics();
				m_ErgAddMax -= cID->GetChildPtr()->GetErgonomics();
			}
		}
		(*Lane)++;
	}


	// ------------------------------------------------------------------------------------------------------------
	void CustomBG::SetWeaponParam(int WeaponID) noexcept {
		if (this->m_SelectWeaponID != WeaponID) {
			this->m_SelectWeaponID = WeaponID;
			m_CustomParts->SetBaseWeapon((this->m_SelectWeaponID != InvalidID) ? DataBase::Instance()->GetItemData()->FindPtr(this->m_SelectWeaponID) : nullptr);
			if (m_CustomParts->GetBaseWeapon()) {
				SetList(0, this->m_CustomParts->GetBaseWeapon()->GetTypeID());
				SetList(1, this->m_SelectWeaponID);
			}
			else {
				m_CustomParts->Dispose();
			}
		}
	}
	//
	void CustomBG::SetSubparam(int WeaponID, int PresetID) noexcept {
		SetWeaponParam(WeaponID);
		if (m_CustomParts->GetBaseWeapon()) {
			this->m_SelectPreset = PresetID;
			SetList(2, this->m_SelectPreset);
		}
	}
	void CustomBG::InitList() noexcept {
		this->m_SelectPreset = InvalidID;
		this->m_SelectWeaponID = InvalidID;
		InitLists(3, DXDraw::Instance()->GetUIY(1920 - 10) - DXDraw::Instance()->GetUIY(400), LineHeight + DXDraw::Instance()->GetUIY(5), DXDraw::Instance()->GetUIY(400));
	}
	//
	void CustomBG::Init_Sub(int *posx, int *posy, float* Scale) noexcept {
		//
		*posx = DXDraw::Instance()->GetUIY(100);
		*posy = LineHeight + DXDraw::Instance()->GetUIY(100);
		*Scale = 0.2f;
		//
		m_EnableMag = true;
		m_EnableMount = true;
		m_EnableSight = true;
		//
		m_CustomParts = std::make_unique<CustomParts>();
		m_CustomParts->Init();
		//
		InitList();
		m_ReturnButtonPress = [&]() {
			auto* PageMngr = PageManager::Instance();
			if (!BackLists()) {
				PageMngr->TurnOnGoNextPage();
			}
		};
	}
	//
	void CustomBG::LateExecute_Sub(int*, int*, float*) noexcept {
		if (m_CustomParts->GetBaseWeapon()) {
			//プリセットを適応
			if (m_CustomParts->GetPartsCount() == 0) {
				if (this->m_SelectPreset != InvalidID) {
					m_CustomParts->AttachPreset(*DataBase::Instance()->GetPresetData()->FindPtr(this->m_SelectPreset));
				}
			}
			//設定
			m_CustomParts->CalcChild(!m_EnableMag, !m_EnableMount, !m_EnableSight);
			//パターンを検索
			m_PartsChange = false;//これで無効化
			if (m_PartsChange) {
				m_PartsChange = false;
				std::vector<PartsBaseData>	PartsDatas;
				m_CustomParts->CalcChildErgRec(&PartsDatas);
				printfDx("%d\n", PartsDatas.size());
			}
		}
		else {
			m_PartsChange = false;
		}
	}
	//
	void CustomBG::Draw_Back_Sub(int xpos, int ypos, float scale) noexcept {
		if (m_CustomParts->GetBaseWeapon()) {
			if (m_CustomParts->GetBaseWeapon()->GetIcon().GetGraph()) {
				float Scale = ((float)DXDraw::Instance()->GetUIY(1080) / 128) * scale;
				WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, this->m_CustomParts->GetBaseWeapon()->GetIcon().GetGraph(), xpos + (int)((float)DXDraw::Instance()->GetUIY(960)*scale / 0.2f), ypos + (int)((float)DXDraw::Instance()->GetUIY(540)*scale / 0.2f), Scale, 0.f, false);
				m_CustomParts->DrawChild(xpos, ypos, Scale);
			}
		}
		//
		{
			int xgoal = 0;
			int xs_add = m_ListXSize + DXDraw::Instance()->GetUIY(50);
			bool isChild = false;
			isChild |= MakeLists(0, true, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemTypeData()->DrawList(XP + xgoal, YP, XS, "ItemType", &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (ptr->GetCategoryID() == DataBase::Instance()->GetItemCategoryData()->FindID("Weapons")); });
			});
			isChild |= MakeLists(1, true, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemData()->DrawList(XP + xgoal, YP, XS, "Item", &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (!ptr->GetIsPreset()) && (ptr->GetTypeID() == ListsSel(1 - 1)); });
			});
			isChild |= MakeLists(2, false, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetPresetData()->DrawList(XP + xgoal, YP, XS, "Preset", &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (ptr->GetBase()->GetID() == ListsSel(2 - 1)); });
			});
			if ((ListsSel(1) != InvalidID) && (ListsSel(2) != InvalidID)) {
				xgoal -= xs_add * 3;
			}
			ExecuteLists(isChild, xgoal);
		}
		//List
		{
			//
			SetWeaponParam(ListsSel(1));
			//
			if ((ListsSel(2) != InvalidID) && (ListsSel(2) != this->m_SelectPreset)) {
				m_CustomParts->Dispose();
			}
			this->m_SelectPreset = ListsSel(2);
		}
	}
	void CustomBG::DrawFront_Sub(int, int, float) noexcept {
		//下から上に
		if (m_CustomParts->GetBaseWeapon()) {
			bool PrevSight = this->m_EnableSight;
			bool PrevMount = this->m_EnableMount;
			bool PrevMag = this->m_EnableMag;


			int xp = LineHeight;
			int yp = DXDraw::Instance()->GetUIY(720) - LineHeight;
			//
			yp -= DXDraw::Instance()->GetUIY(80);
			WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "エルゴノミクス");
			int Erg = (int)m_CustomParts->GetErgonomics();// , OldE = Erg;
			//Erg = WindowSystem::UpDownBar(xp, DXDraw::Instance()->GetUIY(640), yp + LineHeight + DXDraw::Instance()->GetUIY(5), Erg, 0, 100);
			if (m_CustomParts->GetBaseWeapon()) {
				int xmin = xp;
				int xmax = DXDraw::Instance()->GetUIY(640);
				int ypos = yp + LineHeight + DXDraw::Instance()->GetUIY(5);
				int valueMin = 0;
				int valueMax = 100;

				float ErgMin = m_CustomParts->GetErgonomicsMin();
				float ErgMax = m_CustomParts->GetErgonomicsMax();

				int xp_t = 0;
				{
					int xpmin = xmin + LineHeight + 1;
					int xpmax = xmax - 1;
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin), ypos + LineHeight, DarkGreen);
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Green);
					if (m_CustomParts->IsSpecChange()) {
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp((int)ErgMin - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Red);
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp((int)ErgMax - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Blue);
					}

					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Black, 3);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Gray15, 1);
				}
				/*
				xp_t = xmax;
				if (WindowSystem::SetMsgClickBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, LineHeight, Gray25, true, true, "△")) {
					Erg = std::min(Erg + 1, valueMax);
				}
				//*/
				xp_t = (xmin + (xmax - xmin) / 2);
				if (m_CustomParts->IsSpecChange()) {
					if ((int)ErgMin == Erg) {
						WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight * 7 / 10, STRX_MID, White, Black, "               <%5.2f", ErgMax);
					}
					else if ((int)ErgMax == Erg) {
						WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<               ", ErgMin);
					}
					else {
						WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<            <%5.2f", ErgMin, ErgMax);
					}
				}
				WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight, STRX_MID, White, Black, "%5.2f", this->m_CustomParts->GetErgonomics());
				/*
				xp_t = xmin;
				if (WindowSystem::SetMsgClickBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, LineHeight, Gray25, true, true, "▽")) {
					Erg = std::max(Erg - 1, valueMin);
				}
				//*/
			}

			//if (OldE != Erg) { m_Ergonomics = (float)Erg; }
			//
			yp -= DXDraw::Instance()->GetUIY(80);
			WindowSystem::SetMsg(xp, yp + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "縦リコイル");
			int Rec = (int)m_CustomParts->GetRecoil();// , OldR = Rec;
			//Rec = WindowSystem::UpDownBar(xp, DXDraw::Instance()->GetUIY(640), yp + LineHeight + DXDraw::Instance()->GetUIY(5), Rec, 10, 200);
			if (m_CustomParts->GetBaseWeapon()) {
				int xmin = xp;
				int xmax = DXDraw::Instance()->GetUIY(640);
				int ypos = yp + LineHeight + DXDraw::Instance()->GetUIY(5);
				int valueMin = 0;
				int valueMax = 100;

				float RecMin = m_CustomParts->GetRecoilMin();
				float RecMax = m_CustomParts->GetRecoilMax();

				int xp_t = 0;
				{
					int xpmin = xmin + LineHeight + 1;
					int xpmax = xmax - 1;
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin), ypos + LineHeight, DarkGreen);
					WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Green);
					if (m_CustomParts->IsSpecChange()) {
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp((int)RecMin - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Blue);
						WindowSystem::SetBox(
							xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp((int)RecMax - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Red);
					}

					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Black, 3);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
						xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Gray15, 1);
				}
				/*
				xp_t = xmax;
				if (WindowSystem::SetMsgClickBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, LineHeight, Gray25, true, true, "△")) {
					Rec = std::min(Rec + 1, valueMax);
				}
				//*/
				xp_t = (xmin + (xmax - xmin) / 2);
				if (m_CustomParts->IsSpecChange()) {
					if ((int)RecMin == Rec) {
						WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight * 7 / 10, STRX_MID, White, Black, "               <%5.2f", RecMax);
					}
					else if ((int)RecMax == Rec) {
						WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<               ", RecMin);
					}
					else {
						WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<            <%5.2f", RecMin, RecMax);
					}
				}
				WindowSystem::SetMsg(xp_t, ypos + LineHeight / 2, LineHeight, STRX_MID, White, Black, "%5.2f", this->m_CustomParts->GetRecoil());
				/*
				xp_t = xmin;
				if (WindowSystem::SetMsgClickBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, LineHeight, Gray25, true, true, "▽")) {
					Rec = std::max(Rec - 1, valueMin);
				}
				//*/
			}
			//if (OldR != Rec) { this->m_Recoil = (float)Rec; }
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableSight = WindowSystem::CheckBox(xp, yp, m_EnableSight);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "サイトを含む");
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableMount = WindowSystem::CheckBox(xp, yp, m_EnableMount);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "マウントを含む");
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableMag = WindowSystem::CheckBox(xp, yp, m_EnableMag);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "マガジンを含む");
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
		if (m_CustomParts->GetBaseWeapon()) {
			if (m_CustomParts->GetBaseWeapon()->GetIcon().GetGraph()) {
				SetPositionGuide(
					m_CustomParts->GetCustomDrawXMinPosition(), m_CustomParts->GetCustomDrawYMinPosition(),
					m_CustomParts->GetCustomDrawXMaxPosition(), m_CustomParts->GetCustomDrawYMaxPosition());
			}
		}
	}
};
