#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageBase.hpp"
#include "../Data/MainScene_Item.hpp"
#include "../Data/MainScene_Preset.hpp"

namespace FPS_n2 {
	class CustomBG :public BGParent {
		class ChildData {
			const ItemList*	m_ParentPtr{ nullptr };
			int				m_ParentSlot{ 0 };
			int				ChildSel{ 0 };
			int				watchCounter{ 0 };
			int				m_PartsOn{ InvalidID };
		public:
			const ItemList::ItemProperties::ChildItemSettings&			GetMySlotData() const noexcept;
			const bool			GetPtrIsParentSlot(const ItemList* parentptr, int parentslot) const noexcept;
			const bool			GetIsSelected(int parentslot = -1) const noexcept;
			void				OnOffSelect() noexcept;
			void				AddSelect() noexcept;
			void				SubSelect() noexcept;
			const ItemList*		GetChildPtr(int parentslot = -1) const noexcept;
			static const bool	ItemPtrChecktoBeFiltered(const ItemList* ptr, bool MagFilter, bool MountFilter, bool SightFilter) noexcept;
			const bool			ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter) const noexcept;
		public:
			const auto			IsWatch() const noexcept { return this->watchCounter > 0; }
			void				ResetWatchCounter() noexcept { this->watchCounter = 0; }
			void				AddWatchCounter() noexcept { ++this->watchCounter; }
		public:
			void				Set(const ItemList* parentptr, int parentslot, int Select) noexcept;
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
		void AddPartsSeek(int i) noexcept;
		//プリセットを適応
		void AttachPreset(const PresetList& Preset, const ItemList* Ptr = nullptr) noexcept;
		//設定
		bool CheckConflict(const ItemList* MyPtr, const ItemList* Ptr = nullptr) noexcept;
		//
		void CalcChild(const ItemList* Ptr = nullptr) noexcept;
		//
		void CalcChildErgRec(std::vector<std::vector<std::vector<PartsBaseData>>>* Data, int BaseID = 0, int BaseNest = 0, const ItemList* Ptr = nullptr) noexcept;
		//
		void CalcChildErgRec(std::vector<PartsBaseData>* AnsData, const ItemList* Ptr = nullptr) noexcept;
		//描画
		bool DrawChild(int xposbase, int yposbase, int xpos, int ypos, float Scale, int* Lane, int Nest = 0, const ItemList* Ptr = nullptr) noexcept;
	public:
		void SetSubparam(int WeaponID, int PresetID) noexcept;
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override;
		void LateExecute_Sub(int*, int*, float*) noexcept override;
		void Draw_Back_Sub(int xpos, int ypos, float scale) noexcept override;
		void DrawFront_Sub(int posx, int posy, float) noexcept override;
		void Dispose_Sub(void) noexcept override;
	};
};
