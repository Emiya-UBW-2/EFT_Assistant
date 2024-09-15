#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"
#include "../Data/MainScene_Item.hpp"
#include "../Data/MainScene_Preset.hpp"

namespace FPS_n2 {
	class PartsBaseData {
	public:
		struct PartsID
		{
			std::vector<ItemID>			m_ConflictPartsID{};
			ItemID						MyID{ INVALID_ID };
			std::string					MyName{};
			ItemID						ParentID{ INVALID_ID };
			int							SlotNum = 0;
			PartsID(ItemID my, const std::string& name, ItemID parent,int slot, const std::vector<ItemID>& conflict) {
				MyID = my;
				MyName = name;
				m_ConflictPartsID = conflict;
				ParentID = parent;
				SlotNum = slot;
			}
			bool IsSameItem(const PartsID& o) const noexcept {
				return (this->ParentID == o.ParentID) && (this->SlotNum == o.SlotNum) && (this->MyID == o.MyID);
			}

			bool IsSameSlot(const PartsID& o) const noexcept {
				return (this->ParentID == o.ParentID) && (this->SlotNum == o.SlotNum);
			}
		};
	public:
		std::vector<PartsID>					m_PartsIDList;
	};

	class CustomParts {
	public:
		class ChildData {
			const ItemList* m_ParentPtr{ nullptr };
			int				m_ParentSlot{ 0 };
			int				ChildSel{ 0 };
			int				watchCounter{ 0 };
			int				m_PartsOn{ InvalidID };
		public:
			const ItemList::ItemProperties::ChildItemSettings& GetMySlotData() const noexcept {
				return this->m_ParentPtr->GetChildParts().at(this->m_ParentSlot);
			}
			const bool			GetPtrIsParentSlot(const ItemList* parentptr, int parentslot) const noexcept {
				return (this->m_ParentPtr == parentptr) && (this->m_ParentSlot == parentslot);
			}
			const bool			GetIsSelected(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					return (this->ChildSel < (int)(GetMySlotData().GetData().size()));
				}
				return false;
			}
			void				OnOffSelect() noexcept;
			void				AddSelect() noexcept;
			void				SubSelect() noexcept;
			const ItemList* GetChildPtr(int parentslot = -1) const noexcept;
			const bool			ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter, bool AxModFilter) const noexcept;
		public:
			const auto			IsWatch() const noexcept { return this->watchCounter > 0; }
			void				ResetWatchCounter() noexcept { this->watchCounter = 0; }
			void				AddWatchCounter() noexcept { ++this->watchCounter; }
		public:
			void				Set(const ItemList* parentptr, int parentslot, int Select) noexcept;
		};
	private:
		std::vector<ChildData>					m_ChildData;
		ItemList* m_BaseWeapon{ nullptr };

		int										m_posxMinBuffer{ 0 };
		int										m_posyMinBuffer{ 0 };
		int										m_posxMaxBuffer{ 0 };
		int										m_posyMaxBuffer{ 0 };

		float									m_Recoil{ 50 };
		float									m_Ergonomics{ 50 };

		float									m_RecAddMin{ 0 };
		float									m_RecAddMax{ 0 };
		float									m_ErgAddMin{ 0 };
		float									m_ErgAddMax{ 0 };
		bool									m_SpecChange{ false };
	private:
		void				AddSelectToCanSelect(ChildData* cID) noexcept;
		void				SubSelectToCanSelect(ChildData* cID) noexcept;
	public:
		const auto& IsSpecChange(void) const noexcept { return m_SpecChange; }

		const auto& GetCustomDrawXMinPosition(void) const noexcept { return m_posxMinBuffer; }
		const auto& GetCustomDrawYMinPosition(void) const noexcept { return m_posyMinBuffer; }
		const auto& GetCustomDrawXMaxPosition(void) const noexcept { return m_posxMaxBuffer; }
		const auto& GetCustomDrawYMaxPosition(void) const noexcept { return m_posyMaxBuffer; }

		void SetBaseWeapon(ItemList* pBaseWeapon) noexcept { m_BaseWeapon = pBaseWeapon; }
		const auto* GetBaseWeapon(void) const noexcept { return m_BaseWeapon; }

		const auto GetPartsCount(void) const noexcept {
			return m_ChildData.size();
		}

		const auto GetErgonomics(void) const noexcept {
			return m_Ergonomics;
		}
		const auto GetErgonomicsMax(void) const noexcept {
			return (m_Ergonomics + this->m_ErgAddMax);
		}
		const auto GetErgonomicsMin(void) const noexcept {
			return (m_Ergonomics + this->m_ErgAddMin);
		}
		const auto GetRecoil(void) const noexcept {
			return m_Recoil;
		}
		const auto GetRecoilMax(void) const noexcept {
			int BaseRecoil = m_BaseWeapon->GetRecoilVertical();
			if (BaseRecoil != 0) {
				float RecoilPer2 = 100.f * (this->m_Recoil / (float)(BaseRecoil));
				return ((float)(BaseRecoil) * (RecoilPer2 + this->m_RecAddMax) / 100.f);
			}
			else {
				return 0.0f;
			}
		}
		const auto GetRecoilMin(void) const noexcept {
			int BaseRecoil = m_BaseWeapon->GetRecoilVertical();
			if (BaseRecoil != 0) {
				float RecoilPer2 = 100.f * (this->m_Recoil / (float)(BaseRecoil));
				return ((float)(BaseRecoil) * (RecoilPer2 + this->m_RecAddMin) / 100.f);
			}
			else {
				return 0.0f;
			}
		}
	public:
		//プリセットを適応
		void AttachPreset(const PresetList& Preset, const ItemList* Ptr = nullptr) noexcept;
		//設定
		bool CheckConflict(const ItemList* MyPtr, const ItemList* Ptr = nullptr) noexcept;
		//
		void CalcChild(bool MagFilter, bool MountFilter, bool SightFilter, const ItemList* Ptr = nullptr) noexcept;
		//
		static const bool			ItemPtrChecktoBeFiltered(ItemTypeID TypeID, bool MagFilter, bool MountFilter, bool SightFilter, bool AxModFilter) noexcept;
		//
		void CalcChildBranch(std::vector<PartsBaseData>* Data, const ItemList* Ptr = nullptr, ItemID ParentDataID = INVALID_ID, int ParentDataIndex = INVALID_ID, int slot = 0) noexcept;
		void CalcChildErgRec(std::vector<PartsBaseData>* Data) noexcept;
		//描画
		bool DrawChild(int XLeftPosition, int YMiddlePosition, float Scale, int parentXpos = 0, int parentYpos = 0, int* Lane = nullptr, const ItemList* Ptr = nullptr) noexcept;
	public:
		//描画
		void DrawChildOnce(ChildData* cID, int XLeftPosition, int YMiddlePosition, int Xsize, int Ysize, float Scale, int* Lane = nullptr) noexcept;
	public:
		void Init() noexcept {
			m_ChildData.clear();
			m_BaseWeapon = nullptr;

			m_Recoil = 50.f;
			m_Ergonomics = 50.f;
		}
		void Dispose() noexcept {
			m_ChildData.clear();
		}
	};

	class CustomBG :public PageParent {
	private:
		ItemID									m_SelectWeaponID{ InvalidID };
		PresetID								m_SelectPreset{ InvalidID };

		bool									m_EnableMag{ false };
		bool									m_EnableMount{ false };
		bool									m_EnableSight{ false };

		bool									m_PartsChange{ true };
		std::unique_ptr<CustomParts>			m_CustomParts{ nullptr };
	public:
		void SetWeaponParam(int WeaponID) noexcept;
		void SetSubparam(int WeaponID, int PresetID) noexcept;
		void InitList() noexcept;
	private:
		void Init_Sub(int* posx, int* posy, float* Scale) noexcept override;
		void LateExecute_Sub(int*, int*, float*) noexcept override;
		void Draw_Back_Sub(int xpos, int ypos, float scale) noexcept override;
		void DrawFront_Sub(int, int, float) noexcept override;
		void Dispose_Sub(void) noexcept override {
			m_CustomParts->Dispose();
			m_CustomParts.reset();
		}
	};
};
