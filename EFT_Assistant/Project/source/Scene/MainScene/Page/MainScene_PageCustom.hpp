#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class CustomBG :public BGParent {
		class ChildData {
			const ItemList*	m_ParentPtr{ nullptr };
			int				m_ParentSlot{ 0 };
			int				ChildSel{ 0 };
			int				watchCounter{ 0 };
			int				m_PartsOn{ InvalidID };
		public:
			const auto&			GetMySlotData() const noexcept { return this->m_ParentPtr->GetChildParts().at(this->m_ParentSlot); }
			const auto			GetPtrIsParentSlot(const ItemList* parentptr, int parentslot) const noexcept { return (this->m_ParentPtr == parentptr) && (this->m_ParentSlot == parentslot); }
			const auto			GetIsSelected(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					return (this->ChildSel < (int)(GetMySlotData().GetData().size()));
				}
				return false;
			}
			void				OnOffSelect() noexcept {
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
			void				AddSelect() noexcept {
				this->m_PartsOn = InvalidID;
				++this->ChildSel %= (GetMySlotData().GetData().size() + 1);
			}
			void				SubSelect() noexcept {
				this->m_PartsOn = InvalidID;
				--this->ChildSel;
				if (this->ChildSel < 0) {
					this->ChildSel = (int)(GetMySlotData().GetData().size());
				}
			}
			const ItemList*		GetChildPtr(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					if (GetIsSelected(parentslot)) {
						return DataBase::Instance()->GetItemData()->FindPtr(this->GetMySlotData().GetData().at(this->ChildSel).GetID());
					}
				}
				return nullptr;
			}
			static const auto	ItemPtrChecktoBeFiltered(const ItemList* ptr, bool MagFilter, bool MountFilter, bool SightFilter) noexcept {
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
			const auto			ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter) const noexcept {
				return ItemPtrChecktoBeFiltered(GetChildPtr(parentslot), MagFilter, MountFilter, SightFilter);
			}
		public:
			const auto			IsWatch() const noexcept { return this->watchCounter > 0; }
			void				ResetWatchCounter() noexcept { this->watchCounter = 0; }
			void				AddWatchCounter() noexcept { ++this->watchCounter; }
		public:
			void				Set(const ItemList* parentptr, int parentslot, int Select) noexcept {
				this->m_ParentPtr = parentptr;
				this->m_ParentSlot = parentslot;
				this->ChildSel = Select;
				this->watchCounter = 1;
			}
		};
		class PartsBaseData {
		public:
			std::vector<std::string>	m_PartsID;
			float						m_RecoilPer{ 0.f };
			float						m_ErgonomicsPer{ 0.f };
		};
	private:
		PresetID								m_SelectPreset{ InvalidID };
		ItemID									m_SelectBuffer{ InvalidID };
		ItemList*								m_BaseWeapon{ nullptr };

		bool									m_EnableMag{ false };
		bool									m_EnableMount{ false };
		bool									m_EnableSight{ false };
		float									m_Recoil{ 50 };
		float									m_Ergonomics{ 50 };

		float									m_RecAddMin{ 0 };
		float									m_RecAddMax{ 0 };
		float									m_ErgAddMin{ 0 };
		float									m_ErgAddMax{ 0 };
		bool									m_SpecChange{ false };

		std::vector<ChildData>					m_ChildData;

		int										m_posxMaxBuffer{ 0 };
		int										m_posyMaxBuffer{ 0 };

		std::vector<std::vector<PartsBaseData>>	m_PartsBaseData;
		std::vector<PartsBaseData>				m_PartsResultData;
		std::vector<int>						m_PartsSeek;
		bool									m_PartsChange{ true };
	private:
		//
		void AddPartsSeek(int i) noexcept {
			if (i >= 0) {
				m_PartsSeek[i]++;
				if (m_PartsSeek[i] >= this->m_PartsBaseData[i].size()) {
					m_PartsSeek[i] = 0;
					AddPartsSeek(i - 1);
				}
			}
		}
		//プリセットを適応
		void AttachPreset(const PresetList& Preset, const ItemList* Ptr = nullptr) noexcept {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				m_ChildData.clear();
				Ptr_Buf = this->m_BaseWeapon;
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				bool IsHit = false;
				for (auto& cID : this->m_ChildData) {
					if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
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
						for (auto& cptr : c.GetData()) {
							if (P == DataBase::Instance()->GetItemData()->FindPtr(cptr.GetID())) {
								IsHit2 = true;
								ChildSel = (int)(&cptr - &c.GetData().front());
								break;
							}
						}
						if (IsHit2) { break; }
					}
					if (ChildSel != -1) {
						//設定vectorに追加
						m_ChildData.resize(m_ChildData.size() + 1);
						m_ChildData.back().Set(Ptr_Buf, Index, ChildSel);
						AttachPreset(Preset, this->m_ChildData.back().GetChildPtr());
					}
				}
			}
		}
		//設定
		bool CheckConflict(const ItemList* MyPtr, const ItemList* Ptr = nullptr) noexcept {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				Ptr_Buf = this->m_BaseWeapon;
			}
			for (const auto& cP : Ptr_Buf->GetConflictParts()) {
				if (cP.GetID() == MyPtr->GetID()) {
					return true;
				}
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				for (const auto& cID : this->m_ChildData) {
					if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
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
		void CalcChild(const ItemList* Ptr = nullptr) noexcept;
		//
		void CalcChildErgRec(std::vector<std::vector<std::vector<PartsBaseData>>>* Data, int BaseID = 0, int BaseNest = 0, const ItemList* Ptr = nullptr) noexcept;
		//
		void CalcChildErgRec(std::vector<PartsBaseData>* AnsData, const ItemList* Ptr = nullptr) noexcept;
		//描画
		bool DrawChild(int xposbase, int yposbase, int xpos, int ypos, float Scale, int* Lane, int Nest = 0, const ItemList* Ptr = nullptr) noexcept;
	public:
		void SetSubparam(int WeaponID, int PresetID) noexcept {
			m_SelectBuffer = WeaponID;
			m_SelectPreset = PresetID;
			m_BaseWeapon = DataBase::Instance()->GetItemData()->FindPtr(m_SelectBuffer);
			if (m_BaseWeapon) {
				m_ItemIDs.at(0).first = this->m_BaseWeapon->GetTypeID();
				m_ItemIDs.at(0).second = true;

				m_ItemIDs.at(1).first = this->m_SelectBuffer;
				m_ItemIDs.at(1).second = true;

				m_ItemIDs.at(2).first = this->m_SelectPreset;
				m_ItemIDs.at(2).second = true;
			}
		}
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override;
		void LateExecute_Sub(int*, int*, float*) noexcept override;
		void Draw_Back_Sub(int xpos, int ypos, float scale) noexcept override;
		void DrawFront_Sub(int posx, int posy, float) noexcept override;
		void Dispose_Sub(void) noexcept override {
			m_ChildData.clear();
		}
	};
};
