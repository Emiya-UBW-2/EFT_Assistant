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
			this->ChildSel = this->m_PartsOn;
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
	const ItemList* CustomParts::ChildData::GetChildPtr(int parentslot) const noexcept {
		if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
			if (GetIsSelected(parentslot)) {
				return DataBase::Instance()->GetItemData()->FindPtr(this->GetMySlotData().GetData().at(this->ChildSel).GetID());
			}
		}
		return nullptr;
	}
	const bool			CustomParts::ChildData::ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter, bool AxModFilter) const noexcept {
		return CustomParts::ItemPtrChecktoBeFiltered(GetChildPtr(parentslot)->GetTypeID(), MagFilter, MountFilter, SightFilter, AxModFilter);
	}
	void				CustomParts::ChildData::Set(const ItemList* parentptr, int parentslot, int Select) noexcept {
		this->m_ParentPtr = parentptr;
		this->m_ParentSlot = parentslot;
		this->ChildSel = Select;
		this->watchCounter = 1;
	}
	//
	void CustomParts::AddSelectToCanSelect(ChildData* cID) noexcept {
		while (true) {
			if (cID->GetIsSelected() && CheckConflict(cID->GetChildPtr())) {
				cID->AddSelect();
				continue;
			}
			break;
		}
	}
	void CustomParts::SubSelectToCanSelect(ChildData* cID) noexcept {
		while (true) {
			if (cID->GetIsSelected() && CheckConflict(cID->GetChildPtr())) {
				cID->SubSelect();
				continue;
			}
			break;
		}
	}
	//
	void CustomParts::UpdateItemList() noexcept {
		if (m_ItemListUpdate) {
			m_ItemListUpdate = false;
			m_BlackList.clear();
			m_ItemLockList.clear();
			for (auto& item : DataBase::Instance()->GetItemData()->GetList()) {
				if (PlayerData::Instance()->GetItemBlackList(item.GetIDstr().c_str())) {
					m_BlackList.emplace_back(&item);
				}
				else if (PlayerData::Instance()->GetItemLock(item.GetIDstr().c_str())) {
					m_ItemLockList.emplace_back(&item);
				}
			}
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
				auto& cID = this->m_ChildData.back();
				cID.OnOffSelect();
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
					if (cID.GetIsSelected() && cID.ChecktoBeFiltered(-1, MagFilter, MountFilter, SightFilter, false)) {
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
				m_Recoil = ((float)(m_BaseWeapon->GetRecoilVertical()) * (100.f + RecoilPer) / 100.f);
				m_Ergonomics = (m_BaseWeapon->GetWeaponErgonomics() + ErgonomicsPer);
			}
		}
	}


	const bool CustomParts::ItemPtrChecktoBeFiltered(ItemTypeID TypeID, bool MagFilter, bool MountFilter, bool SightFilter, bool AxModFilter) noexcept {
		bool IsHit = false;
		if (MagFilter) {
			if (
				TypeID == DataBase::Instance()->GetItemTypeData()->FindID("magazine")
				) {
				IsHit = true;
			}
		}
		if (MountFilter) {
			if (
				TypeID == DataBase::Instance()->GetItemTypeData()->FindID("mount")
				) {
				IsHit = true;
			}
		}
		if (SightFilter) {
			if (
				TypeID == DataBase::Instance()->GetItemTypeData()->FindID("reflex-sight")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("compact-reflex-sight")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("assault-scope")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("scope")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("special-scope")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("ironsight")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("night-vision")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("flashlight")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("comb-tact-device")
				) {
				IsHit = true;
			}
		}
		if (AxModFilter) {
			if (
				TypeID == DataBase::Instance()->GetItemTypeData()->FindID("auxiliary-mod")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("bipod")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("foregrip")
				|| TypeID == DataBase::Instance()->GetItemTypeData()->FindID("charging-handle")
				) {
				IsHit = true;
			}
		}
		return IsHit;
	}

	template<class T>
	bool CheckSameElements(const std::vector<T>& A, const std::vector<T>& B) {
		if (A.size() != B.size()) { return false; }
		bool IsHit2 = true;
		for (const auto& cc2 : A) {
			for (const auto& cc3 : B) {
				IsHit2 = false;
				if (cc2.GetID() == cc3.GetID()) {
					IsHit2 = true;
					break;
				}
			}
			if (!IsHit2) {
				break;
			}
		}
		return IsHit2;
	}

	// 枝を網羅(組み合わせ網羅ではない)
	void CustomParts::CalcChildBranch(std::vector<PartsBaseData>* Data, bool MagFilter, bool MountFilter, bool SightFilter, bool AxModFilter, const ItemList* Ptr, ItemID ParentDataID, int ParentDataIndex, int slot) noexcept
	{
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
		if (!IsParent) {
			Data->back() = Data->at(ParentDataIndex);
		}
		//
		{
			std::vector<ItemID> Conflict;
			Conflict.clear();
			for (auto& conf : Ptr_Buf->GetConflictParts()) {
				Conflict.emplace_back(conf.GetID());
			}
			if (!IsParent) {
				Data->back().m_PartsIDList.emplace_back(Ptr_Buf->GetID(), Ptr_Buf->GetName_Jpn(), ParentDataID, slot, Conflict, Ptr_Buf->GetRecoil(), Ptr_Buf->GetErgonomics());
			}
			else {
				Data->back().m_PartsIDList.emplace_back(Ptr_Buf->GetID(), Ptr_Buf->GetName_Jpn(), ParentDataID, slot, Conflict, 0.f, 0.f);
			}
			ParentDataIndex = (int)Data->size() - 1;
		}
		//子パーツ
		std::vector<ItemList*> ItemHitList;
		bool Required = false;
		for (int index = 0; const auto & c : Ptr_Buf->GetChildParts()) {
			Required |= c.m_Required;
			//if (!c.m_Required) { continue; }

			//アイテム固定に入っているアイテムがスロット候補にあるなら
			ItemID CheckSlotLock = INVALID_ID;
			for (auto& L : m_ItemLockList) {
				for (auto& cptr : c.GetData()) {
					if (cptr.GetID() == L->GetID()) {
						CheckSlotLock = cptr.GetID();
						break;
					}
				}
				if (CheckSlotLock != INVALID_ID) {
					break;
				}
			}

			ItemHitList.clear();
			for (auto& cptr : c.GetData()) {
				//ロックアイテムと同じスロットでロックアイテム出ないなら
				if (CheckSlotLock != INVALID_ID && CheckSlotLock != cptr.GetID()) {
					continue;
				}
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID());
				//任意パーツで特定のアイテム以外
				if (!c.m_Required) {
					if (CustomParts::ItemPtrChecktoBeFiltered(ptr->GetTypeID(), MagFilter, MountFilter, SightFilter, AxModFilter)) {
						continue;
					}
				}
				// 既に登録したアイテムの中でエルゴ、反動、スロット、干渉がすべて同じものはスルーする
				{
					bool IsSameSpec = false;
					for (auto& l : ItemHitList) {
						//リコイル、エルゴが同じでない
						if (l->GetErgonomics() != ptr->GetErgonomics()) { continue; }
						if (l->GetRecoil() != ptr->GetRecoil()) { continue; }
						//干渉リストに同じものが入っているかどうか
						if (!CheckSameElements(ptr->GetConflictParts(), l->GetConflictParts())) { continue; }
						//こどもリストが同じ
						{
							if (ptr->GetChildParts().size() != l->GetChildParts().size()) { continue; }
							bool IsHit = true;
							for (const auto& c2 : ptr->GetChildParts()) {
								for (const auto& c3 : l->GetChildParts()) {
									IsHit = false;
									if (CheckSameElements(c2.GetData(), c3.GetData())) {
										IsHit = true;
										break;
									}
								}
								if (!IsHit) {
									break;
								}
							}
							if (!IsHit) {
								continue;
							}
						}
						IsSameSpec = true;
						break;
					}
					if (IsSameSpec) {
						continue;
					}
				}
				//ブラックリストに入っているなら
				{
					bool OnBlackList = false;
					for (auto& L : m_BlackList) {
						if (cptr.GetID() == L->GetID()) {
							OnBlackList = true;
							break;
						}
					}
					if (OnBlackList) {
						continue;
					}
				}
				//
				ItemHitList.emplace_back(ptr);//
				CalcChildBranch(Data, MagFilter, MountFilter, SightFilter, AxModFilter, ptr, Ptr_Buf->GetID(), ParentDataIndex, index);
			}
			index++;
		}
		//必要なパーツが付けられていないのでつけない場合を消す
		if (Required) {
			std::swap(Data->back(), Data->at(ParentDataIndex));
			Data->pop_back();
		}
		if (IsParent) {
			for (auto& Parts2Base : *Data) {
				std::sort(Parts2Base.m_PartsIDList.begin() + 1, Parts2Base.m_PartsIDList.end(),
					[&](const PartsBaseData::PartsID& Parts1, const PartsBaseData::PartsID& Parts2) { return Parts1.MyID > Parts2.MyID; });
			}
		}
	}
	void CustomParts::CalcBlackList() noexcept {
		for (auto& item : DataBase::Instance()->GetItemData()->GetList()) {
			auto* TypePtr = DataBase::Instance()->GetItemTypeData()->FindPtr(item.GetTypeID());
			auto* catPtr = DataBase::Instance()->GetItemCategoryData()->FindPtr(TypePtr->GetCategoryID());
			if (
				//item.GetIsWeapon()
				catPtr->GetName() == "WeaponMods"
				) {
				bool IsHit = false;
				bool HasParts = false;
				for (auto& L : DataBase::Instance()->GetTraderData()->SetList()) {
					for (const auto& Ld : L.GetLvData()) {
						for (const auto& cf : Ld.m_ItemBarters) {
							for (const auto& I : cf.m_ItemReward) {
								if (I.GetID() == item.GetID()) {
									HasParts = true;
									IsHit = (cf.m_TaskReq.size() == 0);
									if (IsHit) {
										break;
									}
								}
							}
							if (IsHit) {
								break;
							}
						}
						if (IsHit) {
							break;
						}
					}
					if (IsHit) {
						break;
					}
				}
				if (HasParts && !IsHit) {
					//トレーダー交換がLLアップのみでできない場合ブラックリスト入り
					PlayerData::Instance()->SetItemBlackList(item.GetIDstr().c_str(), true);
					PlayerData::Instance()->SetItemLock(item.GetIDstr().c_str(), false);
					UpdateItemListFlag();
				}
			}
		}
		PlayerData::Instance()->Save();
	}
	//
	void CustomParts::CalcChildErgRec(std::vector<PartsBaseData>* Data, const std::vector<PartsBaseData>& BranchDataBase) noexcept {
		(*Data) = BranchDataBase;
		// 枝をもとに組み合わせ網羅
		{
			int BaseMin = 0;
			int BaseMax = 0;
			for (int loop = 0; loop < 10; loop++) {
				BaseMin = BaseMax;
				BaseMax = (int)Data->size();
				if (BaseMin == BaseMax) { break; }
				for (auto & Parts1Base : BranchDataBase) {
					auto CheckSameSlot = [&](const PartsBaseData::PartsID& Parts2) {
						bool IsThrough = false;
						for (auto& Parts1 : Parts1Base.m_PartsIDList) {
							if (Parts1.IsSameSlot(Parts2)) {
								IsThrough = true;
								break;
							}
						}
						return IsThrough;
						};
					auto CheckConflict = [&](const PartsBaseData::PartsID& Parts2) {
						bool IsConflict = false;
						for (auto& parts : Parts1Base.m_PartsIDList) {
							for (auto& conf : parts.m_ConflictPartsID) {
								if (conf == Parts2.MyID) {
									IsConflict = true;
									break;
								}
							}
							if (IsConflict) {
								break;
							}
						}
						return IsConflict;
						};
					auto CheckParent = [&](const PartsBaseData::PartsID& Parts2) {
						bool HasParent = false;
						for (auto& parts : Parts1Base.m_PartsIDList) {
							if (parts.MyID == Parts2.ParentID) {
								HasParent = true;
								break;
							}
						}
						return HasParent;
						};
					for (int Branch2 = BaseMin; Branch2 < BaseMax; Branch2++) {
						bool IsActive = false;
						for (int loop2 = 0, max2 = (int)Data->at(Branch2).m_PartsIDList.size(); loop2 < max2; loop2++) {
							auto& Parts2 = Data->at(Branch2).m_PartsIDList.at(loop2);
							// Branch1の方にあるデータと同じスロットに被っている場合はスルー
							if (CheckSameSlot(Parts2)) { continue; }
							// 干渉チェック
							if (CheckConflict(Parts2)) { continue; }
							// 親になるアイテムがないなら
							if (!CheckParent(Parts2)) { continue; }
							//適用
							if (!IsActive) {
								IsActive = true;
								Data->resize(Data->size() + 1);
								Data->back() = Parts1Base;
							}
							Data->back().m_PartsIDList.emplace_back(Parts2);
						}
						if (IsActive) {
							std::sort(Data->back().m_PartsIDList.begin() + 1, Data->back().m_PartsIDList.end(),
								[&](const PartsBaseData::PartsID& Parts1, const PartsBaseData::PartsID& Parts2) { return Parts1.MyID > Parts2.MyID; });
							//被っているものなら削除
							{
								auto& data = Data->back();
								bool IsSame = false;
								for (auto& data2 : *Data) {
									if (&data == &data2) { continue; }
									if (data.m_PartsIDList.size() != data2.m_PartsIDList.size()) { continue; }
									int max = (int)data.m_PartsIDList.size();
									bool isHit = false;
									for (int loop3 = 0; loop3 < max; loop3++) {
										isHit = true;
										//要素がかぶらない場合は違うよ
										if (!data.m_PartsIDList.at(loop3).IsSameItem(data2.m_PartsIDList.at(loop3))) {
											isHit = false;
											break;
										}
									}
									if (isHit) {
										IsSame = true;
										break;
									}
								}
								if (IsSame) {
									Data->pop_back();
								}
							}
						}
					}
				}
			}
		}
		while (true) {
			//break;
			//必須パーツが埋まっていない場合省く
			{
				for (int index = 0, Max = (int)Data->size(); index < Max; index++) {
					auto& data = Data->at(index);

					auto CheckFillItemOnSlot = [&](const std::vector<IDParents<ItemID>>& Datas, ItemID ParentID) {
						bool IsHit = false;
						for (auto& parts2 : data.m_PartsIDList) {
							if (parts2.ParentID == ParentID) {
								IsHit = true;
								//子オブジェの中でスロット対象のオブジェクトがあったら
								for (auto& cptr : Datas) {
									if (cptr.GetID() == parts2.MyID) {
										return true;
									}
								}
							}
						}
						return !IsHit;
					};

					bool SlotAllOk = true;
					for (auto& parts : data.m_PartsIDList) {
						auto* Ptr_t = DataBase::Instance()->GetItemData()->FindPtr(parts.MyID);
						if (Ptr_t) {
							for (const auto& c : Ptr_t->GetChildParts()) {
								if (c.m_Required) {//埋める必要のあるスロットがあって
									//スロットが空なら
									if (!CheckFillItemOnSlot(c.GetData(), parts.MyID)) {
										SlotAllOk = false;
										break;
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
			break;
		}
		//リコイル、エルゴのデータを計算
		for (auto& data2 : *Data) {
			data2.m_Recoil = 0.0f;
			data2.m_Ergonomics = 0.0f;
			for (auto& Parts1 : data2.m_PartsIDList) {
				data2.m_Recoil += Parts1.m_RecoilAdd;
				data2.m_Ergonomics += Parts1.m_ErgonomicsAdd;
			}
			data2.m_Recoil = 
				((float)(m_BaseWeapon->GetRecoilVertical()) * (100.f + data2.m_Recoil) / 100.f) + 
				((float)(m_BaseWeapon->GetRecoilHorizontal()) * (100.f + data2.m_Recoil) / 100.f)
				;
			data2.m_Ergonomics = (m_BaseWeapon->GetWeaponErgonomics() + data2.m_Ergonomics);
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
			const_cast<ItemList*>(cID->GetChildPtr())->Draw(Xleft, YUp, (Xright - Xleft), (YBottom - YUp), 0, Gray10, false, false, false, false);
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
				if (Pad->GetAtoZKey('B').trigger()) {
					PlayerData::Instance()->OnOffItemBlackList(cID->GetChildPtr()->GetIDstr().c_str());
					if (PlayerData::Instance()->GetItemBlackList(cID->GetChildPtr()->GetIDstr().c_str())) {
						PlayerData::Instance()->SetItemLock(cID->GetChildPtr()->GetIDstr().c_str(), false);
					}
					UpdateItemListFlag();
				}
				else if (Pad->GetAtoZKey('L').trigger()) {
					//ロックをかける
					for (const auto& cID2 : cID->GetMySlotData().GetData()) {
						if (cID->GetChildPtr() != DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())) {
							PlayerData::Instance()->SetItemLock(DataBase::Instance()->GetItemData()->FindPtr(cID2.GetID())->GetIDstr().c_str(), false);
						}
					}
					PlayerData::Instance()->OnOffItemLock(cID->GetChildPtr()->GetIDstr().c_str());
					if (PlayerData::Instance()->GetItemLock(cID->GetChildPtr()->GetIDstr().c_str())) {
						PlayerData::Instance()->SetItemBlackList(cID->GetChildPtr()->GetIDstr().c_str(), false);
					}
					UpdateItemListFlag();
				}
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
					"Bキーでブラックリストに設定"
				);
				WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
					FontPool::FontType::MS_Gothic, LineHeight,
					STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y() + LineHeight, RedPop, Black,
					"Lキーでアイテム固定に設定"
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
	void CustomBG::Init_Sub(int* posx, int* posy, float* Scale) noexcept {
		//
		*posx = DXDraw::Instance()->GetUIY(100);
		*posy = LineHeight + DXDraw::Instance()->GetUIY(100);
		*Scale = 0.2f;
		//
		m_EnableMag = false;
		m_EnableMount = false;
		m_EnableSight = false;
		m_EnableAxParts = true;
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
		m_CustomParts->UpdateItemListFlag();
	}
	//
	void CustomBG::LateExecute_Sub(int* xpos, int* ypos, float* scale) noexcept {
		auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		//設定
		if (m_CustomParts->GetBaseWeapon()) {
			//プリセットを適応
			if (m_CustomParts->GetPartsCount() == 0) {
				if (this->m_SelectPreset != InvalidID) {
					m_CustomParts->AttachPreset(*DataBase::Instance()->GetPresetData()->FindPtr(this->m_SelectPreset));
				}
			}
			m_CustomParts->CalcChild(!m_EnableMag, !m_EnableMount, !m_EnableSight);
		}
		//背景とブランチ描画
		if (m_CustomParts->GetBaseWeapon()) {
			float Scale = ((float)DXDraw::Instance()->GetUIY(1080) / 128) * *scale;
			if (m_CustomParts->GetBaseWeapon()->GetIcon().GetGraph()) {
				WindowSystem::DrawControl::Instance()->SetDrawRotaGraph(WindowSystem::DrawLayer::Normal, this->m_CustomParts->GetBaseWeapon()->GetIcon().GetGraph(), *xpos + (int)((float)DXDraw::Instance()->GetUIY(960) * *scale / 0.2f), *ypos + (int)((float)DXDraw::Instance()->GetUIY(540) * *scale / 0.2f), Scale, 0.f, false);
			}
			m_CustomParts->DrawChild(*xpos, *ypos, Scale);
		}
		//List
		{
			int xgoal = 0;
			int xs_add = m_ListXSize + DXDraw::Instance()->GetUIY(50);
			bool isChild = false;
			isChild |= MakeLists(0, true, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemTypeData()->DrawList(XP + xgoal, YP, XS, "ItemType", &IDs->first, !IDs->second, false, false, [&](const auto* ptr) { return (ptr->GetCategoryID() == DataBase::Instance()->GetItemCategoryData()->FindID("Weapons")); });
				});
			isChild |= MakeLists(1, true, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetItemData()->DrawList(XP + xgoal, YP, XS, "Item", &IDs->first, !IDs->second, false, false, [&](const auto* ptr) { return (!ptr->GetIsPreset()) && (ptr->GetTypeID() == ListsSel(1 - 1)); });
				});
			isChild |= MakeLists(2, false, [&](std::pair<int, bool>* IDs, bool IsChild, int XP, int YP, int XS) {
				if (IsChild) { xgoal -= xs_add; }
				DataBase::Instance()->GetPresetData()->DrawList(XP + xgoal, YP, XS, "Preset", &IDs->first, !IDs->second, false, false, [&](const auto* ptr) { return (ptr->GetBase()->GetID() == ListsSel(2 - 1)); });
				});
			if ((ListsSel(1) != InvalidID) && (ListsSel(2) != InvalidID)) {
				xgoal -= xs_add * 3;
			}
			ExecuteLists(isChild, xgoal);
			//
			SetWeaponParam(ListsSel(1));
			//
			if ((ListsSel(2) != InvalidID) && (ListsSel(2) != this->m_SelectPreset)) {
				m_CustomParts->Dispose();
			}
			this->m_SelectPreset = ListsSel(2);
		}
		//場所ガイド
		if (m_CustomParts->GetBaseWeapon()) {
			SetPositionGuide(
				m_CustomParts->GetCustomDrawXMinPosition(), m_CustomParts->GetCustomDrawYMinPosition(),
				m_CustomParts->GetCustomDrawXMaxPosition(), m_CustomParts->GetCustomDrawYMaxPosition());
		}
		//下から上に
		if (m_CustomParts->GetBaseWeapon()) {
			int xp = LineHeight;
			int yp = DXDraw::Instance()->GetUIY(1080) - LineHeight / 2;
			//
			yp -= DXDraw::Instance()->GetUIY(80);
			WindowSystem::SetMsg(xp, yp, LineHeight, STRX_LEFT, White, Black, "エルゴノミクス");
			{
				int xmin = xp;
				int xmax = DXDraw::Instance()->GetUIY(480);
				int xmid = (xmin + (xmax - xmin) / 2);
				int ymin = yp + LineHeight / 2 + DXDraw::Instance()->GetUIY(5);
				int ymax = ymin + LineHeight;
				int ymid = (ymin + (ymax - ymin) / 2);

				int valueMin = 0;
				int valueMax = 100;
				int valueLen = valueMax - valueMin;
				int Erg = std::clamp((int)m_CustomParts->GetErgonomics() - valueMin, 0, valueLen);
				int ErgMin = std::clamp((int)m_CustomParts->GetErgonomicsMin() - valueMin, 0, valueLen);
				int ErgMax = std::clamp((int)m_CustomParts->GetErgonomicsMax() - valueMin, 0, valueLen);

				{
					int xpmin = xmin + LineHeight + 1;
					int xpmax = xmax - 1;
					int xplen = (xpmax - xpmin);
					WindowSystem::SetBox(xpmin, ymin, xpmin + xplen, ymax, DarkGreen);
					WindowSystem::SetBox(xpmin, ymin, xpmin + xplen * Erg / valueLen, ymax, Green);
					if (m_CustomParts->IsSpecChange()) {
						WindowSystem::SetBox(xpmin + xplen * ErgMin / valueLen, ymin, xpmin + xplen * Erg / valueLen, ymax, Red);
						WindowSystem::SetBox(xpmin + xplen * Erg / valueLen, ymin, xpmin + xplen * ErgMax / valueLen, ymax, Blue);
					}

					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + xplen * Erg / valueLen, ymin, xpmin + xplen * Erg / valueLen, ymax, Black, 3);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + xplen * Erg / valueLen, ymin, xpmin + xplen * Erg / valueLen, ymax, Gray15, 1);
				}
				if (m_CustomParts->IsSpecChange()) {
					WindowSystem::SetMsg(xmid, ymid, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<            <%5.2f", m_CustomParts->GetErgonomicsMin(), m_CustomParts->GetErgonomicsMax());
				}
				WindowSystem::SetMsg(xmid, ymid, LineHeight, STRX_MID, White, Black, "%5.2f", this->m_CustomParts->GetErgonomics());
			}
			//
			yp -= DXDraw::Instance()->GetUIY(80);
			WindowSystem::SetMsg(xp, yp, LineHeight, STRX_LEFT, White, Black, "縦リコイル");
			{
				int xmin = xp;
				int xmax = DXDraw::Instance()->GetUIY(480);
				int xmid = (xmin + (xmax - xmin) / 2);
				int ymin = yp + LineHeight / 2 + DXDraw::Instance()->GetUIY(5);
				int ymax = ymin + LineHeight;
				int ymid = (ymin + (ymax - ymin) / 2);

				int valueMin = 0;
				int valueMax = 100;
				int valueLen = valueMax - valueMin;
				int Rec = std::clamp((int)m_CustomParts->GetRecoil() - valueMin, 0, valueLen);
				int RecMin = std::clamp((int)m_CustomParts->GetRecoilMin() - valueMin, 0, valueLen);
				int RecMax = std::clamp((int)m_CustomParts->GetRecoilMax() - valueMin, 0, valueLen);

				{
					int xpmin = xmin + LineHeight + 1;
					int xpmax = xmax - 1;
					int xplen = (xpmax - xpmin);
					WindowSystem::SetBox(xpmin, ymin, xpmin + xplen, ymax, DarkGreen);
					WindowSystem::SetBox(xpmin, ymin, xpmin + xplen * Rec / valueLen, ymax, Green);
					if (m_CustomParts->IsSpecChange()) {
						WindowSystem::SetBox(xpmin + xplen * RecMin / valueLen, ymin, xpmin + xplen * Rec / valueLen, ymax, Blue);
						WindowSystem::SetBox(xpmin + xplen * Rec / valueLen, ymin, xpmin + xplen * RecMax / valueLen, ymax, Red);
					}

					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + xplen * Rec / valueLen, ymin, xpmin + xplen * Rec / valueLen, ymax, Black, 3);
					WindowSystem::DrawControl::Instance()->SetDrawLine(WindowSystem::DrawLayer::Normal,
						xpmin + xplen * Rec / valueLen, ymin, xpmin + xplen * Rec / valueLen, ymax, Gray15, 1);
				}
				if (m_CustomParts->IsSpecChange()) {
					WindowSystem::SetMsg(xmid, ymid, LineHeight * 7 / 10, STRX_MID, White, Black, "%5.2f<            <%5.2f", m_CustomParts->GetRecoilMin(), m_CustomParts->GetRecoilMax());
				}
				WindowSystem::SetMsg(xmid, ymid, LineHeight, STRX_MID, White, Black, "%5.2f", this->m_CustomParts->GetRecoil());
			}
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableSight = WindowSystem::CheckBox(xp, (yp - LineHeight / 2), m_EnableSight);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, LineHeight, STRX_LEFT, White, Black, "サイトを含む");
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableMount = WindowSystem::CheckBox(xp, (yp - LineHeight / 2), m_EnableMount);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, LineHeight, STRX_LEFT, White, Black, "マウントを含む");
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableMag = WindowSystem::CheckBox(xp, (yp - LineHeight / 2), m_EnableMag);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, LineHeight, STRX_LEFT, White, Black, "マガジンを含む");
			//
			yp -= DXDraw::Instance()->GetUIY(50);
			m_EnableAxParts = WindowSystem::CheckBox(xp, (yp - LineHeight / 2), m_EnableAxParts);
			WindowSystem::SetMsg(xp + LineHeight * 3, yp, LineHeight, STRX_LEFT, White, Black, "その他必須ではないパーツを含む");
			//
			if (m_EnableSight) {
				m_EnableMount = true;
			}
			if (!m_EnableMount) {
				m_EnableSight = false;
			}
			//パターンを検索
			yp -= DXDraw::Instance()->GetUIY(50);
			if (WindowSystem::SetMsgClickBox(
				xp, yp, xp + DXDraw::Instance()->GetUIY(480), yp + LineHeight, LineHeight,
				Gray25, false, !WindowMngr->PosHitCheck(nullptr), "パターン検索")) {
				m_PartsDatas.clear();

				std::vector<PartsBaseData>	BranchDataBase;
				m_CustomParts->CalcChildBranch(&BranchDataBase, !m_EnableSight, !m_EnableMount, !m_EnableMag, !m_EnableAxParts);
				clsDx();
				int totalsize = (int)BranchDataBase.size();
				printfDx("パーツパターン : %d\n", totalsize);
				if (totalsize >= 50) {
					printfDx("パターンが多すぎます、50未満まで抑えてください\n");
					printfDx("最悪の場合の総数 : [2^%d - 1]\n", totalsize);
				}
				else {
					m_CustomParts->CalcChildErgRec(&m_PartsDatas, BranchDataBase);

					std::sort(m_PartsDatas.begin(), m_PartsDatas.end(),
						[&](const PartsBaseData& Parts1, const PartsBaseData& Parts2) {
							if (Parts1.m_Recoil == Parts2.m_Recoil) {
								return Parts1.m_Ergonomics < Parts2.m_Ergonomics;
							}
							return Parts1.m_Recoil < Parts2.m_Recoil;
						});

					//保存
					{
						std::string Path = "Save/BuildList/";
						Path += m_CustomParts->GetBaseWeapon()->GetName();
						SubStrs(&Path, ":");
						SubStrs(&Path, "*");
						SubStrs(&Path, "?");
						SubStrs(&Path, "\"");
						SubStrs(&Path, ">");
						SubStrs(&Path, "<");
						SubStrs(&Path, "|");
						Path += ".txt";

						std::ofstream outputfile(Path);
						for (auto& data : m_PartsDatas) {
							outputfile << std::to_string(data.m_Recoil) + "\t,";
							outputfile << std::to_string(data.m_Ergonomics) + "\t,";
							for (auto& parts : data.m_PartsIDList) {
								outputfile << parts.MyName + "\t,";
							}
							outputfile << "\n";
						}
						outputfile.close();
						printfDx("OK : %d\n", m_PartsDatas.size());
					}
				}
			}
		}
		// ブラックリスト
		{
			auto* Pad = PadControl::Instance();
			//
			m_CustomParts->UpdateItemList();
			//表示
			int xp = LineHeight + DXDraw::Instance()->GetUIY(480);
			int yp = LineHeight * 3 + LineHeight / 2;

			int Max = (int)m_CustomParts->GetBlackList().size();
			{
				int xofs = 0;
				int yofs = 0;
				WindowSystem::SetMsg(xp, yp + LineHeight / 2 + yofs, LineHeight, STRX_LEFT, White, Black, "ブラックリスト(%d)", Max);
				xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "ブラックリスト(%d)", Max) + DXDraw::Instance()->GetUIY(30)); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

				int ysize = DXDraw::Instance()->GetUIY(36);
				int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);

				int ofset = (int)(this->m_Scroll.GetNowScrollYPer() * (std::max(0, Max - 4 + 1) * ysizeAdd));
				int yofs_t = yofs;
				yofs_t += LineHeight + DXDraw::Instance()->GetUIY(5);
				int ypMin = yp + yofs_t;
				int ypMax = yp + yofs_t + ysizeAdd * 4;
				int yp1 = yp + yofs_t - ofset;
				for (auto* item : m_CustomParts->GetBlackList()) {
					if (ypMin - ysizeAdd < yp1 && yp1 < ypMax) {
						if (ypMin < yp1 && yp1 < ypMax - ysizeAdd) {
							WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
						}
						else {
							if (yp1 <= ypMin) {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypMin - yp1) / ysizeAdd, 0, 255));
							}
							else {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp1 - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
							}
						}
						ItemList* pitem = (ItemList*)item;
						int x_1 = xp + DXDraw::Instance()->GetUIY(30);
						int y_1 = yp1;
						int x_2 = x_1 + DXDraw::Instance()->GetUIY(800);
						int y_2 = y_1 + ysize;
						pitem->Draw(x_1, y_1, x_2 - x_1, y_2 - y_1, 0, Gray25, (!WindowMngr->PosHitCheck(nullptr) && !(xp == 0 && yp == 0)), false, false, false);
						if (IntoMouse(x_1, y_1, x_2, y_2)) {
							WindowSystem::DrawControl::Instance()->SetDrawBox(WindowSystem::DrawLayer::Normal, x_1, y_1, x_2, y_2, RedPop, false);
							if (Pad->GetAtoZKey('B').trigger()) {
								PlayerData::Instance()->OnOffItemBlackList(item->GetIDstr().c_str());
								if (PlayerData::Instance()->GetItemBlackList(item->GetIDstr().c_str())) {
									PlayerData::Instance()->SetItemLock(item->GetIDstr().c_str(), false);
								}
								m_CustomParts->UpdateItemListFlag();
							}
							else if (Pad->GetAtoZKey('L').trigger()) {
								PlayerData::Instance()->OnOffItemLock(item->GetIDstr().c_str());
								if (PlayerData::Instance()->GetItemLock(item->GetIDstr().c_str())) {
									PlayerData::Instance()->SetItemBlackList(item->GetIDstr().c_str(), false);
								}
								m_CustomParts->UpdateItemListFlag();
							}
							WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
								FontPool::FontType::MS_Gothic, LineHeight,
								STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
								"Bキーでブラックリストに設定"
							);
							WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
								FontPool::FontType::MS_Gothic, LineHeight,
								STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y() + LineHeight, RedPop, Black,
								"Lキーでアイテム固定に設定"
							);
						}
					}
					yofs_t += ysizeAdd;
					yp1 += ysizeAdd;
				}
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
				//スクロールバー
				{
					float Total = (float)(yofs_t - yofs) / (ypMax - ypMin);
					if (Total > 1.f) {
						this->m_Scroll.SetScrollBoxParam(
							xp + DXDraw::Instance()->GetUIY(30), ypMin,
							xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30), ypMax,
							Total, !WindowMngr->PosHitCheck(nullptr));
						this->m_Scroll.ScrollBox();
					}
				}
				yofs = ypMax - yp;
				//
				yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

				if (WindowSystem::SetMsgClickBox(
					xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30) - DXDraw::Instance()->GetUIY(480), yp + 0,
					xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30), yp + 0 + LineHeight, LineHeight,
					Gray25, false, !WindowMngr->PosHitCheck(nullptr), "ブラックリストにタスク開放品をセット")) {
					m_CustomParts->CalcBlackList();
					m_CustomParts->UpdateItemListFlag();
				}
			}
		}
		// アイテム固定リスト
		{
			//GetItemLockList
			auto* Pad = PadControl::Instance();
			//
			m_CustomParts->UpdateItemList();
			//表示
			int xp = LineHeight + DXDraw::Instance()->GetUIY(480);
			int yp = LineHeight * 3 + LineHeight / 2 + DXDraw::Instance()->GetUIY(480);

			int Max = (int)m_CustomParts->GetItemLockList().size();
			{
				int xofs = 0;
				int yofs = 0;
				WindowSystem::SetMsg(xp, yp + LineHeight / 2 + yofs, LineHeight, STRX_LEFT, White, Black, "アイテム固定(%d)", Max);
				xofs = std::max(xofs, WindowSystem::GetMsgLen(LineHeight, "アイテム固定(%d)", Max) + DXDraw::Instance()->GetUIY(30)); yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

				int ysize = DXDraw::Instance()->GetUIY(36);
				int ysizeAdd = ysize + DXDraw::Instance()->GetUIY(5);

				int ofset = (int)(this->m_Scroll.GetNowScrollYPer() * (std::max(0, Max - 4 + 1) * ysizeAdd));
				int yofs_t = yofs;
				yofs_t += LineHeight + DXDraw::Instance()->GetUIY(5);
				int ypMin = yp + yofs_t;
				int ypMax = yp + yofs_t + ysizeAdd * 4;
				int yp1 = yp + yofs_t - ofset;
				for (auto* item : m_CustomParts->GetItemLockList()) {
					if (ypMin - ysizeAdd < yp1 && yp1 < ypMax) {
						if (ypMin < yp1 && yp1 < ypMax - ysizeAdd) {
							WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
						}
						else {
							if (yp1 <= ypMin) {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (ypMin - yp1) / ysizeAdd, 0, 255));
							}
							else {
								WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255 - std::clamp(255 * (yp1 - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
							}
						}
						ItemList* pitem = (ItemList*)item;
						int x_1 = xp + DXDraw::Instance()->GetUIY(30);
						int y_1 = yp1;
						int x_2 = x_1 + DXDraw::Instance()->GetUIY(800);
						int y_2 = y_1 + ysize;
						pitem->Draw(x_1, y_1, x_2 - x_1, y_2 - y_1, 0, Gray25, (!WindowMngr->PosHitCheck(nullptr) && !(xp == 0 && yp == 0)), false, false, false);
						if (IntoMouse(x_1, y_1, x_2, y_2)) {
							WindowSystem::DrawControl::Instance()->SetDrawBox(WindowSystem::DrawLayer::Normal, x_1, y_1, x_2, y_2, RedPop, false);
							if (Pad->GetAtoZKey('B').trigger()) {
								PlayerData::Instance()->OnOffItemBlackList(item->GetIDstr().c_str());
								if (PlayerData::Instance()->GetItemBlackList(item->GetIDstr().c_str())) {
									PlayerData::Instance()->SetItemLock(item->GetIDstr().c_str(), false);
								}
								m_CustomParts->UpdateItemListFlag();
							}
							else if (Pad->GetAtoZKey('L').trigger()) {
								PlayerData::Instance()->OnOffItemLock(item->GetIDstr().c_str());
								if (PlayerData::Instance()->GetItemLock(item->GetIDstr().c_str())) {
									PlayerData::Instance()->SetItemBlackList(item->GetIDstr().c_str(), false);
								}
								m_CustomParts->UpdateItemListFlag();
							}
							WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
								FontPool::FontType::MS_Gothic, LineHeight,
								STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y(), RedPop, Black,
								"Bキーでブラックリストに設定"
							);
							WindowSystem::DrawControl::Instance()->SetString(WindowSystem::DrawLayer::Front,
								FontPool::FontType::MS_Gothic, LineHeight,
								STRX_RIGHT, STRY_BOTTOM, Pad->GetMS_X(), Pad->GetMS_Y() + LineHeight, RedPop, Black,
								"Lキーでアイテム固定に設定"
							);
						}
					}
					yofs_t += ysizeAdd;
					yp1 += ysizeAdd;
				}
				WindowSystem::DrawControl::Instance()->SetAlpha(WindowSystem::DrawLayer::Normal, 255);
				//スクロールバー
				{
					float Total = (float)(yofs_t - yofs) / (ypMax - ypMin);
					if (Total > 1.f) {
						this->m_Scroll.SetScrollBoxParam(
							xp + DXDraw::Instance()->GetUIY(30), ypMin,
							xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30), ypMax,
							Total, !WindowMngr->PosHitCheck(nullptr));
						this->m_Scroll.ScrollBox();
					}
				}
				yofs = ypMax - yp;
				//
				yofs += LineHeight + DXDraw::Instance()->GetUIY(5);

				if (WindowSystem::SetMsgClickBox(
					xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30) - DXDraw::Instance()->GetUIY(480), yp + 0,
					xp + DXDraw::Instance()->GetUIY(30) + DXDraw::Instance()->GetUIY(800) + DXDraw::Instance()->GetUIY(30), yp + 0 + LineHeight, LineHeight,
					Gray25, false, !WindowMngr->PosHitCheck(nullptr), "アイテム固定にタスク開放品をセット")) {
					//m_CustomParts->CalcBlackList();
					m_CustomParts->UpdateItemListFlag();
				}
			}
		}
	}
};
