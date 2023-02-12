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
			const auto& GetMySlotData() const noexcept { return this->m_ParentPtr->GetChildParts().at(this->m_ParentSlot); }
			const auto GetPtrIsParentSlot(const ItemList* parentptr, int parentslot) const noexcept { return (this->m_ParentPtr == parentptr) && (this->m_ParentSlot == parentslot); }
			const auto GetIsSelected(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					return (this->ChildSel < (int)(GetMySlotData().Data.size()));
				}
				return false;
			}
			void	OnOffSelect() noexcept {
				if (this->m_PartsOn == InvalidID) {
					if (GetIsSelected()) {
						this->m_PartsOn = this->ChildSel;
						this->ChildSel = (int)(GetMySlotData().Data.size());
					}
					else {
						this->ChildSel = 0;
					}
				}
				else {
					bool IsHit = false;
					int  Now = 0;
					for (const auto& cID2 : GetMySlotData().Data) {
						if (PlayerData::Instance()->GetItemLock(cID2.first->GetName().c_str())) {
							IsHit = true;
							break;
						}
						Now++;
					}
					this->ChildSel = IsHit ? Now : this->m_PartsOn;
					this->m_PartsOn = InvalidID;
				}
			}
			void	AddSelect() noexcept {
				this->m_PartsOn = InvalidID;
				++this->ChildSel %= (GetMySlotData().Data.size() + 1);
			}
			void	SubSelect() noexcept {
				this->m_PartsOn = InvalidID;
				--this->ChildSel;
				if (this->ChildSel < 0) {
					this->ChildSel = (int)(GetMySlotData().Data.size());
				}
			}
			const ItemList* GetChildPtr(int parentslot = -1) const noexcept {
				if ((parentslot == -1) || ((parentslot != -1) && GetPtrIsParentSlot(this->m_ParentPtr, parentslot))) {
					if (GetIsSelected(parentslot)) {
						return this->GetMySlotData().Data.at(this->ChildSel).first;
					}
				}
				return nullptr;
			}
			static const auto ItemPtrChecktoBeFiltered(const ItemList* ptr, bool MagFilter, bool MountFilter, bool SightFilter) noexcept {
				bool IsHit = false;
				if (ptr) {
					if (MagFilter) {
						if (
							ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Magazine")
							) {
							IsHit = true;
						}
					}
					if (MountFilter) {
						if (
							ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Mount")
							) {
							IsHit = true;
						}
					}
					if (SightFilter) {
						if (
							ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Reflex sight")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Compact reflex sight")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Assault scope")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Scope")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Special scope")
							|| ptr->GetTypeID() == ItemTypeData::Instance()->FindID("Iron sight")
							) {
							IsHit = true;
						}
					}
				}
				return IsHit;
			}
			const auto ChecktoBeFiltered(int parentslot, bool MagFilter, bool MountFilter, bool SightFilter) const noexcept {
				return ItemPtrChecktoBeFiltered(GetChildPtr(parentslot), MagFilter, MountFilter, SightFilter);
			}
		public:
		public:
			const auto IsWatch() const noexcept { return this->watchCounter > 0; }
			void	ResetWatchCounter() noexcept { this->watchCounter = 0; }
			void	AddWatchCounter() noexcept { ++this->watchCounter; }
		public:
			void	Set(const ItemList* parentptr, int parentslot, int Select) noexcept {
				this->m_ParentPtr = parentptr;
				this->m_ParentSlot = parentslot;
				this->ChildSel = Select;
				this->watchCounter = 1;
			}
		};
	private://アイテムのやつ
		std::vector<std::pair<int, bool>>	m_ItemIDs;
		float							m_XChild{ 0.f };
		void MakeLists(int Layer, bool AndNext, const std::function<void(std::pair<int, bool>*)>& ListSet) noexcept {
			auto& NowSel = m_ItemIDs.at(Layer);
			NowSel.second = ((NowSel.first != InvalidID) && AndNext);
			if (Layer == 0 || (Layer >= 1 && m_ItemIDs.at(Layer - 1).second)) {
				ListSet(&NowSel);
			}
			else {
				NowSel.first = InvalidID;
			}
		}
	private:
		ItemID	m_SelectBuffer{ InvalidID };
		ItemList* m_BaseWeapon{ nullptr };

		bool m_EnableMag = false;
		bool m_EnableMount = false;
		bool m_EnableSight = false;
		float m_Recoil = 50;
		float m_Ergonomics = 50;

		float m_RecAddMin{ 0 };
		float m_RecAddMax{ 0 };
		float m_ErgAddMin{ 0 };
		float m_ErgAddMax{ 0 };
		bool m_SpecChange = false;


		std::vector<ChildData>			m_ChildData;

		int															m_posxMaxBuffer{ 0 };
		int															m_posyMaxBuffer{ 0 };


		class PartsBaseData {
		public:
			std::vector<std::string>	m_PartsID;
			float				m_RecoilPer{ 0.f };
			float				m_ErgonomicsPer{ 0.f };
		};
		class ErgRecData {
			class PartsBufferData {
			public:
				std::string		m_PartsID;
				float		m_RecoilPer{ 0.f };
				float		m_ErgonomicsPer{ 0.f };
			};
		public:
			std::vector<PartsBufferData>	m_Parts;
		};
		std::vector<std::vector<PartsBaseData>>	m_PartsBaseData;
		std::vector<PartsBaseData>				m_PartsResultData;
		std::vector<int>						m_PartsSeek;
		bool									m_PartsChange{ true };
		void AddPartsSeek(int i) noexcept {
			if (i >= 0) {
				m_PartsSeek[i]++;
				if (m_PartsSeek[i] >= m_PartsBaseData[i].size()) {
					m_PartsSeek[i] = 0;
					AddPartsSeek(i - 1);
				}
			}
		};
	private:
		//プリセットを適応
		void AttachPreset(const PresetList& Preset, const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				m_ChildData.clear();
				Ptr_Buf = m_BaseWeapon;
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				bool IsHit = false;
				for (auto& cID : m_ChildData) {
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
						for (auto& cptr : c.Data) {
							if (P == cptr.first) {
								IsHit2 = true;
								ChildSel = (int)(&cptr - &c.Data.front());
								break;
							}
						}
						if (IsHit2) { break; }
					}
					if (ChildSel != -1) {
						//設定vectorに追加
						m_ChildData.resize(m_ChildData.size() + 1);
						m_ChildData.back().Set(Ptr_Buf, Index, ChildSel);
						AttachPreset(Preset, m_ChildData.back().GetChildPtr());
					}
				}
			}
		}
		//設定
		bool CheckConflict(const ItemList* MyPtr, const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				Ptr_Buf = m_BaseWeapon;
			}
			for (const auto& cP : Ptr_Buf->GetConflictParts()) {
				if (cP.first == MyPtr) {
					return true;
				}
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				for (const auto& cID : m_ChildData) {
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
		void CalcChild(const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				for (auto& cID : m_ChildData) {
					cID.ResetWatchCounter();
				}
				Ptr_Buf = m_BaseWeapon;
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				bool IsHit = false;
				for (auto& cID : m_ChildData) {
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
					auto& cID = m_ChildData.back();
					for (const auto& cID2 : m_ChildData.back().GetMySlotData().Data) {
						if (PlayerData::Instance()->GetItemLock(cID2.first->GetName().c_str())) {
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
				for (int count = 0; count < m_ChildData.size(); count++) {
					if (!m_ChildData[count].IsWatch()) {
						m_ChildData.erase(m_ChildData.begin() + count);
						count--;
					}
				}
				//フィルターに引っかかっていたら大丈夫なものにする
				for (auto& cID : m_ChildData) {
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
					for (const auto& cID : m_ChildData) {
						if (cID.GetIsSelected()) {
							auto* ChildPtr = cID.GetChildPtr();
							RecoilPer += ChildPtr->GetRecoil();
							ErgonomicsPer += ChildPtr->GetErgonomics();
						}
					}
					m_Recoil = (m_BaseWeapon->GetRecoil()*(100.f + RecoilPer) / 100.f);
					m_Ergonomics = (m_BaseWeapon->GetErgonomics() + ErgonomicsPer);
				}
			}
		}

		void CalcChildErgRec(std::vector<std::vector<ErgRecData>>* Data, int BaseID = 0, int BaseNest = 0, const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				Data->clear();
				Ptr_Buf = m_BaseWeapon;
				for (const auto& c : Ptr_Buf->GetChildParts()) {
					Data->resize(Data->size() + 1);//こどもの分岐
					for (auto& cptr : c.Data) {
						//フィルターに引っかかってなければOK
						if (!ChildData::ItemPtrChecktoBeFiltered(cptr.first, !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
							Data->back().resize(Data->back().size() + 1);
							CalcChildErgRec(Data, (int)Data->back().size() - 1, (int)Data->back().back().m_Parts.size(), cptr.first);
						}
					}
					for (int i = 0; i < Data->back().size(); i++) {
						if (Data->back()[i].m_Parts.size() == 0) {
							std::swap(Data->back()[i], Data->back().back());
							Data->back().pop_back();
							i--;
						}
					}
				}
			}
			if (Ptr != nullptr) {
				Data->back().back().m_Parts.resize(Data->back().back().m_Parts.size() + 1);
				Data->back().back().m_Parts.back().m_PartsID = Ptr_Buf->GetName();
				Data->back().back().m_Parts.back().m_RecoilPer = Data->back().at(BaseID).m_Parts.back().m_RecoilPer + Ptr_Buf->GetRecoil();
				Data->back().back().m_Parts.back().m_ErgonomicsPer = Data->back().at(BaseID).m_Parts.back().m_ErgonomicsPer + Ptr_Buf->GetErgonomics();

				bool IsChild = false;
				for (const auto& c : Ptr_Buf->GetChildParts()) {
					for (auto& cptr : c.Data) {
						//フィルターに引っかかってなければOK
						if (!ChildData::ItemPtrChecktoBeFiltered(cptr.first, !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
							IsChild = true;
							CalcChildErgRec(Data, (int)Data->back().size() - 1, (int)Data->back().back().m_Parts.size(), cptr.first);
						}
					}
				}
				if (!IsChild) {//次のパターンへ
					Data->back().resize(Data->back().size() + 1);
					for (int i = 0; i < BaseNest; i++) {
						Data->back().back().m_Parts.emplace_back(Data->back().at(BaseID).m_Parts[i]);
					}
				}
			}
		}

		void CalcChildErgRec(std::vector<PartsBaseData>* AnsData, const ItemList* Ptr = nullptr) {
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				Ptr_Buf = m_BaseWeapon;
				AnsData->resize(AnsData->size() + 1);
			}
			int Now = 0;
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				for (auto& cptr : c.Data) {
					//フィルターに引っかかってなければOK
					if (!ChildData::ItemPtrChecktoBeFiltered(cptr.first, !m_EnableMag, !m_EnableMount, !m_EnableSight)) {
						CalcChildErgRec(AnsData, cptr.first);
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
		bool DrawChild(int xposbase, int yposbase, int xpos, int ypos, float Scale, int* Lane, int Nest = 0, const ItemList* Ptr = nullptr) noexcept {
			auto* Input = InputControl::Instance();

			bool HaveChild = false;
			auto* Ptr_Buf = Ptr;
			if (Ptr == nullptr) {
				for (auto& cID : m_ChildData) {
					cID.ResetWatchCounter();
				}
				Ptr_Buf = m_BaseWeapon;
				m_posxMaxBuffer = 0;
				m_posyMaxBuffer = 0;
			}
			for (const auto& c : Ptr_Buf->GetChildParts()) {
				auto Index = (int)(&c - &Ptr_Buf->GetChildParts().front());
				for (auto& cID : m_ChildData) {
					if (cID.GetPtrIsParentSlot(Ptr_Buf, Index)) {
						HaveChild = true;

						int xsize = (int)((float)(600)*Scale);// y_r(400);
						int xsizeMin = (int)((float)(30)*Scale);// y_r(400);
						int ysize = (int)((float)(64) * Scale);// y_r(92);
						int xbase = xposbase + Nest * (xsize + (int)((float)(y_r(30))*Scale / 0.2f));
						int yp2 = yposbase + (*Lane) * (ysize + (int)((float)(y_r(10))*Scale / 0.2f));
						int ybase = yp2 - ysize / 2;
						if (Ptr != nullptr) {
							DrawControl::Instance()->SetDrawLine(false, xpos, ypos, xbase, yp2, Red, y_r(3));
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
							cID.GetChildPtr()->Draw(xbase + xsizeMin, ybase, xsize - xsizeMin * 2, ysize, 0, Gray10, false, false);

							if (DrawChild(xposbase, yposbase, xbase + xsize, yp2, Scale, Lane, Nest + 1, cID.GetChildPtr())) {
								(*Lane)--;
							}

							if (in2_(Input->GetMouseX(), Input->GetMouseY(), xbase, ybase, xbase + xsize, ybase + ysize)) {
								DrawControl::Instance()->SetDrawBox(false, xbase, ybase, xbase + xsize, ybase + ysize, RedPop, false);
								if (Input->GetKey('L').trigger()) {
									//ロックをかける
									for (const auto& cID2 : cID.GetMySlotData().Data) {
										PlayerData::Instance()->SetItemLock(cID2.first->GetName().c_str(), false);
									}
									PlayerData::Instance()->OnOffItemLock(cID.GetChildPtr()->GetName().c_str());
								}
								DrawControl::Instance()->SetString(true,
									FontPool::FontType::Nomal_Edge, LineHeight,
									FontHandle::FontXCenter::RIGHT, FontHandle::FontYCenter::BOTTOM, Input->GetMouseX(), Input->GetMouseY(), RedPop, Black,
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
							for (const auto& cID2 : cID.GetMySlotData().Data) {
								if (!CheckConflict(cID2.first)) {
									if (m_RecAddMin > cID2.first->GetRecoil()) {
										m_RecAddMin = cID2.first->GetRecoil();
									}
									if (m_RecAddMax < cID2.first->GetRecoil()) {
										m_RecAddMax = cID2.first->GetRecoil();
									}
									if (m_ErgAddMin > cID2.first->GetErgonomics()) {
										m_ErgAddMin = cID2.first->GetErgonomics();
									}
									if (m_ErgAddMax < cID2.first->GetErgonomics()) {
										m_ErgAddMax = cID2.first->GetErgonomics();
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
						for (const auto& t : cID.GetMySlotData().TypeID) {
							int xOfsetAdd = WindowSystem::GetMsgLen(Hight, "[%s]", ItemTypeData::Instance()->FindPtr(t)->GetName().c_str());
							if (xOfset + xOfsetAdd > (xsize - xsizeMin * 2)) {
								break;
							}
							WindowSystem::SetMsg(xbase + xOfset, ybase, xbase + xOfset, ybase + Hight, Hight, FontHandle::FontXCenter::LEFT, White, Black,
								"[%s]", ItemTypeData::Instance()->FindPtr(t)->GetName().c_str());
							xOfset += xOfsetAdd;
						}

						(*Lane)++;
						break;
					}
				}
			}
			return HaveChild;
		}
	private:
		void Init_Sub(int *posx, int *posy, float* Scale) noexcept override {
			*posx = y_r(100);
			*posy = LineHeight + y_r(100);
			*Scale = 0.2f;
			m_SelectBuffer = InvalidID;
			m_BaseWeapon = nullptr;

			m_EnableMag = true;
			m_EnableMount = false;
			m_EnableSight = false;
			m_Recoil = 50.f;
			m_Ergonomics = 50.f;

			m_ChildData.clear();

			m_XChild = 0.f;
			m_ItemIDs.clear();
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
			m_ItemIDs.emplace_back(std::make_pair<int, bool>((int)InvalidID, false));
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
			if (m_BaseWeapon) {
				//プリセットを適応
				if (m_ChildData.size() == 0) {
					for (const auto& L : PresetData::Instance()->GetList()) {
						if (m_BaseWeapon == L.GetBase()) {
							AttachPreset(L);
							break;
						}
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
					//float RecoilPer2 = m_Recoil * 100.f / m_BaseWeapon->GetRecoil() - 100.f;
					//float ErgonomicsPer2 = (m_Ergonomics - m_BaseWeapon->GetErgonomics());

					std::vector<std::vector<ErgRecData>>	m_PartsDatas;
					CalcChildErgRec(&m_PartsDatas);

					std::vector<PartsBaseData> Tmp;
					CalcChildErgRec(&Tmp);

					m_PartsBaseData.clear();
					for (auto& parents : m_PartsDatas) {
						m_PartsBaseData.resize(m_PartsBaseData.size() + 1);
						auto& back = m_PartsBaseData.back();
						for (auto& Base : parents) {
							back.resize(back.size() + 1);
							back.back().m_RecoilPer = 0.f;
							back.back().m_ErgonomicsPer = 0.f;
							for (auto& p : Base.m_Parts) {
								back.back().m_PartsID.emplace_back(p.m_PartsID);
								back.back().m_RecoilPer += p.m_RecoilPer;
								back.back().m_ErgonomicsPer += p.m_ErgonomicsPer;
							}
						}
					}
					//
					m_PartsResultData.clear();
					m_PartsResultData.reserve(5000000);
					m_PartsSeek.resize(m_PartsBaseData.size());
					for (auto& seeks : m_PartsSeek) { seeks = 0; }
					while (true) {
						m_PartsResultData.resize(m_PartsResultData.size() + 1);
						m_PartsResultData.back().m_PartsID.clear();
						m_PartsResultData.back().m_RecoilPer = 0.f;
						m_PartsResultData.back().m_ErgonomicsPer = 0.f;

						for (auto& parents : m_PartsBaseData) {
							auto& Base = parents[m_PartsSeek[(int)(&parents - &m_PartsBaseData.front())]];
							for (auto& p : Base.m_PartsID) {
								m_PartsResultData.back().m_PartsID.emplace_back(p);
							}
							m_PartsResultData.back().m_RecoilPer += Base.m_RecoilPer;
							m_PartsResultData.back().m_ErgonomicsPer += Base.m_ErgonomicsPer;
						}
						AddPartsSeek((int)(m_PartsSeek.size()) - 1);
						int count = 0;
						for (auto& seeks : m_PartsSeek) { count += seeks; }
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

		void Draw_Back_Sub(int xpos, int ypos, float scale) noexcept override {
			if (m_BaseWeapon) {
				if (m_BaseWeapon->GetIcon().GetGraph()) {
					float Scale = ((float)y_r(1920) / m_BaseWeapon->GetIcon().GetXSize()) * scale;
					DrawControl::Instance()->SetDrawRotaGraph(false, m_BaseWeapon->GetIcon().GetGraph(), xpos + (int)((float)y_r(960)*scale / 0.2f), ypos + (int)((float)y_r(540)*scale / 0.2f), Scale, 0.f, false);
					int Lane = 0;
					DrawChild(xpos, ypos, 0, 0, Scale, &Lane);
				}
			}
		}
		void DrawFront_Sub(int posx, int posy, float) noexcept override {
			auto* DrawParts = DXDraw::Instance();
			//
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					if (m_ItemIDs.back().first != InvalidID) {
						m_ItemIDs.back().first = InvalidID;
					}
					else {
						TurnOnGoNextBG();
					}
				}
			}
			//
			{
				int xgoal = 0;
				int xsize = y_r(400);
				int xs_add = -(xsize + y_r(50));
				int xp = y_r(1920 - 10) - xsize - (int)m_XChild;
				int yp = LineHeight + y_r(10);
				bool isChild = false;
				int Layer = 0;
				//
				{
					Layer = 0;
					MakeLists(Layer, true, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemTypeList>(xp + xgoal, yp, ItemTypeData::Instance()->GetList(), &IDs->first, !IDs->second, false, true, [&](const auto *ptr) { return (ptr->GetCategoryID() == ItemCategoryData::Instance()->FindID("Weapons")); });
					});
				}
				//
				{
					Layer = 1;
					MakeLists(Layer, false, [&](std::pair<int, bool>* IDs) {
						isChild |= (Layer >= 1);
						if (isChild) {
							xgoal += xs_add;
						}
						MakeList<ItemList>(xp + xgoal, yp, ItemData::Instance()->GetList(), &IDs->first, !IDs->second, false, false, [&](const auto *ptr) { return (ptr->GetTypeID() == m_ItemIDs.at(Layer - 1).first); });
					});
				}
				//決定
				{
					auto prev = m_SelectBuffer;
					m_SelectBuffer = m_ItemIDs.back().first;
					if (m_SelectBuffer != prev) {
						m_BaseWeapon = (m_SelectBuffer != InvalidID) ? ItemData::Instance()->FindPtr(m_SelectBuffer) : nullptr;
						m_PartsChange = true;
					}
					if (m_BaseWeapon == nullptr) {
						m_ChildData.clear();
					}
				}
				//
				if (isChild) {
					xgoal -= xs_add / 2;
				}
				if (m_SelectBuffer != InvalidID) {
					xgoal += xs_add * 2;
				}
				Easing(&m_XChild, (float)xgoal, 0.8f, EasingType::OutExpo);
			}
			//下から上に
			if (m_BaseWeapon) {
				bool PrevSight = m_EnableSight;
				bool PrevMount = m_EnableMount;
				bool PrevMag = m_EnableMag;


				int xp = LineHeight;
				int yp = y_r(1080) - LineHeight;
				//
				yp -= y_r(80);
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "エルゴノミクス");
				int Erg = (int)m_Ergonomics, OldE = Erg;
				//WindowSystem::UpDownBar(xp, y_r(640), yp + LineHeight + y_r(5), &Erg, 0, 100);
				if (m_BaseWeapon) {
					int xmin = xp;
					int xmax = y_r(640);
					int ypos = yp + LineHeight + y_r(5);
					int valueMin = 0;
					int valueMax = 100;

					float RecoilPer2 = m_Recoil * 100.f / m_BaseWeapon->GetRecoil() - 100.f;

					//(m_BaseWeapon->GetRecoil()*(100.f + RecoilPer2) / 100.f);

					float ErgonomicsPer2 = (m_Ergonomics - m_BaseWeapon->GetErgonomics());
					int ErgMin = (int)(m_BaseWeapon->GetErgonomics() + ErgonomicsPer2 + m_ErgAddMin);
					int ErgMax = (int)(m_BaseWeapon->GetErgonomics() + ErgonomicsPer2 + m_ErgAddMax);

					int xp_t = 0;
					{
						int xpmin = xmin + LineHeight + 1;
						int xpmax = xmax - 1;
						WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin), ypos + LineHeight, DarkGreen);
						WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Green);
						if (m_SpecChange) {
							WindowSystem::SetBox(
								xpmin + (xpmax - xpmin)*std::clamp(ErgMin - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
								xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Red);
							WindowSystem::SetBox(
								xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
								xpmin + (xpmax - xpmin)*std::clamp(ErgMax - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Blue);
						}

						DrawControl::Instance()->SetDrawLine(false,
							xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp(Erg - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Black, 3);
						DrawControl::Instance()->SetDrawLine(false,
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
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, FontHandle::FontXCenter::MIDDLE, White, Black, "%d<      <%d", ErgMin, ErgMax);
					}
					WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight, FontHandle::FontXCenter::MIDDLE, White, Black, "%d", Erg);
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
				WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "縦リコイル");
				int Rec = (int)m_Recoil, OldR = Rec;
				//WindowSystem::UpDownBar(xp, y_r(640), yp + LineHeight + y_r(5), &Rec, 10, 200);
				if (m_BaseWeapon) {
					int xmin = xp;
					int xmax = y_r(640);
					int ypos = yp + LineHeight + y_r(5);
					int valueMin = 0;
					int valueMax = 100;

					float RecoilPer2 = m_Recoil * 100.f / m_BaseWeapon->GetRecoil() - 100.f;
					int RecMin = (int)(m_BaseWeapon->GetRecoil()*(100.f + RecoilPer2 + m_RecAddMin) / 100.f);
					int RecMax = (int)(m_BaseWeapon->GetRecoil()*(100.f + RecoilPer2 + m_RecAddMax) / 100.f);

					int xp_t = 0;
					{
						int xpmin = xmin + LineHeight + 1;
						int xpmax = xmax - 1;
						WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin), ypos + LineHeight, DarkGreen);
						WindowSystem::SetBox(xpmin, ypos, xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Green);
						if (m_SpecChange) {
							WindowSystem::SetBox(
								xpmin + (xpmax - xpmin)*std::clamp(RecMin - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
								xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Blue);
							WindowSystem::SetBox(
								xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
								xpmin + (xpmax - xpmin)*std::clamp(RecMax - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Red);
						}

						DrawControl::Instance()->SetDrawLine(false,
							xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos,
							xpmin + (xpmax - xpmin)*std::clamp(Rec - valueMin, 0, valueMax - valueMin) / (valueMax - valueMin), ypos + LineHeight, Black, 3);
						DrawControl::Instance()->SetDrawLine(false,
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
						WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight * 7 / 10, FontHandle::FontXCenter::MIDDLE, White, Black, "%d<      <%d", RecMin, RecMax);
					}
					WindowSystem::SetMsg(xp_t, ypos, xp_t, ypos + LineHeight, LineHeight, FontHandle::FontXCenter::MIDDLE, White, Black, "%d", Rec);
					/*
					xp_t = xmin;
					if (WindowSystem::ClickCheckBox(xp_t, ypos, xp_t + LineHeight, ypos + LineHeight, true, true, Gray25, "▽")) {
						Rec = std::max(Rec - 1, valueMin);
					}
					//*/
				}
				if (OldR != Rec) { m_Recoil = (float)Rec; }
				//
				yp -= y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableSight);
				WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "サイトを含む");
				//
				yp -= y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableMount);
				WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "マウントを含む");
				//
				yp -= y_r(50);
				WindowSystem::CheckBox(xp, yp, &m_EnableMag);
				WindowSystem::SetMsg(xp + LineHeight * 3, yp, xp + LineHeight * 3, yp + LineHeight, LineHeight, FontHandle::FontXCenter::LEFT, White, Black, "マガジンを含む");
				//
				if (m_EnableSight) {
					m_EnableMount = true;
				}
				if (!m_EnableMount) {
					m_EnableSight = false;
				}
				if ((PrevSight != m_EnableSight) || (PrevMount != m_EnableMount) || (PrevMag != m_EnableMag)) {
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

					DrawControl::Instance()->SetAlpha(false, 64);
					DrawControl::Instance()->SetDrawBox(false, xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Black, TRUE);
					DrawControl::Instance()->SetAlpha(false, 255);
					DrawControl::Instance()->SetDrawBox(false, xp + x_p1, yp + y_p1, xp + x_p2, yp + y_p2, Green, FALSE);
					DrawControl::Instance()->SetDrawBox(false, xp, yp, xp + xs, yp + ys, Red, FALSE);
				}
			}
			//
		}
		void Dispose_Sub(void) noexcept override {
			m_ChildData.clear();
		}
	};
};
