#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int ItemID;


	class ItemList : public ListParent<ItemID> {
		class ChildItemSettings {
		public:
			std::vector<std::pair<const ItemList*, std::string>>	Data;
			std::vector<ItemTypeID>									TypeID;
		};
		typedef std::vector<std::pair<const ItemList*, std::string>> ItemSettings;

		ItemTypeID												m_TypeID{ InvalidID };
		std::vector<MapID>										m_MapID;
		std::vector<ChildItemSettings>							m_ChildPartsID;
		std::vector<const ItemList*>							m_ParentPartsID;
		std::vector<std::pair<const ItemList*, std::string>>	m_ConflictPartsID;
		float													m_Recoil{ 0.f };
		float													m_Ergonomics{ 0.f };
		bool													m_isWeaponMod{ false };

		int											m_SightRange{ -100 };

		int											m_basePrice{ 0 };
		int											m_width{ 0 };
		int											m_height{ 0 };
		std::vector<std::string>					m_types;
		int											m_avg24hPrice{ 0 };
		int											m_low24hPrice{ 0 };
		int											m_lastOfferCount{ 0 };
		std::vector<std::pair<TraderID, int>>		m_sellFor;
		float										m_weight{ 0.f };
		int											m_fleaMarketFee{ 0 };
		bool										m_IsPreset{ false };
		std::vector<int>							m_UseTaskID;
	private:
		//追加設定
		void	Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override {
			if (LEFT == "Itemtype") {
				m_TypeID = ItemTypeData::Instance()->FindID(RIGHT.c_str());
			}
			if (LEFT == "Map") {
				m_MapID.emplace_back(MapData::Instance()->FindID(RIGHT.c_str()));
			}
			if (LEFT == "ChildParts") {
				m_ChildPartsID.resize(m_ChildPartsID.size() + 1);
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
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
				}
				else {
					bool isHit = false;
					for (auto& d : m_ChildPartsID.back().Data) {
						if (d.second == RIGHT) {
							isHit = true;
							break;
						}
					}
					if (!isHit) {
						m_ChildPartsID.back().Data.resize(m_ChildPartsID.back().Data.size() + 1);
						m_ChildPartsID.back().Data.back().second = RIGHT;
					}
				}
			}
			if (LEFT == "Conflict") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
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
				}
				else {
					bool isHit = false;
					for (auto& d : m_ConflictPartsID) {
						if (d.second == RIGHT) {
							isHit = true;
							break;
						}
					}
					if (!isHit) {
						m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
						m_ConflictPartsID.back().second = RIGHT;
					}
				}
			}

			if (LEFT == "Recoil") {
				if (RIGHT.find("+") != std::string::npos) {
					m_Recoil = std::stof(RIGHT.substr(RIGHT.find("+") + 1));
				}
				else {
					if (RIGHT == "") {
						//int a = 0;
					}
					m_Recoil = std::stof(RIGHT);
				}
				m_isWeaponMod = true;
			}
			if (LEFT == "Ergonomics") {
				if (RIGHT.find("+") != std::string::npos) {
					m_Ergonomics = std::stof(RIGHT.substr(RIGHT.find("+") + 1));
				}
				else {
					if (RIGHT == "") {
						//int a = 0;
					}
					m_Ergonomics = std::stof(RIGHT);
				}
				m_isWeaponMod = true;
			}

			if (LEFT == "SightRange") { m_SightRange = std::stoi(RIGHT); }

			if (LEFT == "basePrice") { m_basePrice = std::stoi(RIGHT); }
			if (LEFT == "width") { m_width = std::stoi(RIGHT); }
			if (LEFT == "height") { m_height = std::stoi(RIGHT); }
			if (LEFT == "avg24hPrice") { m_avg24hPrice = std::stoi(RIGHT); }
			if (LEFT == "low24hPrice") { m_low24hPrice = std::stoi(RIGHT); }
			if (LEFT == "lastOfferCount") { m_lastOfferCount = std::stoi(RIGHT); }
			for (auto& sf : TraderData::Instance()->GetList()) {
				if (LEFT == "Sell_" + sf.GetName()) {
					m_sellFor.emplace_back(std::make_pair(sf.GetID(), std::stoi(RIGHT)));
				}
			}
			if (LEFT == "Sell_Flea Market") {
				m_sellFor.emplace_back(std::make_pair(InvalidID, std::stoi(RIGHT)));
			}

			if (LEFT == "weight") { m_weight = std::stof(RIGHT); }
			if (LEFT == "fleaMarketFee") { m_fleaMarketFee = std::stoi(RIGHT); }
			if (LEFT == "propertiestype") { m_IsPreset = (RIGHT == "ItemPropertiesPreset"); }
		}
		void	Load_Sub() noexcept override {
			if (m_TypeID == InvalidID) {
				std::string ErrMes = "Error : Not Setting ItemType in Item ";
				ErrMes += GetShortName();
				DataErrorLog::Instance()->AddLog(ErrMes.c_str());
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		int										m_CheckJson{ 0 };
	public:
		const auto&	GetTypeID() const noexcept { return m_TypeID; }
		const auto&	GetMapID() const noexcept { return m_MapID; }
		const auto&	GetChildParts() const noexcept { return m_ChildPartsID; }
		const auto&	GetConflictParts() const noexcept { return m_ConflictPartsID; }

		const auto&	GetRecoil() const noexcept { return m_Recoil; }
		const auto&	GetErgonomics() const noexcept { return m_Ergonomics; }
		const auto&	GetSightRange() const noexcept { return m_SightRange; }



		const auto&	GetbasePrice() const noexcept { return m_basePrice; }
		const auto&	Getwidth() const noexcept { return m_width; }
		const auto&	Getheight() const noexcept { return m_height; }
		const auto&	Gettypes() const noexcept { return m_types; }
		const auto&	Getavg24hPrice() const noexcept { return m_avg24hPrice; }
		const auto&	Getlow24hPrice() const noexcept { return m_low24hPrice; }
		const auto&	GetlastOfferCount() const noexcept { return m_lastOfferCount; }
		const auto&	GetsellFor() const noexcept { return m_sellFor; }
		const auto&	Getweight() const noexcept { return m_weight; }
		const auto&	GetfleaMarketFee() const noexcept { return m_fleaMarketFee; }
		const auto&	GetIsPreset() const noexcept { return m_IsPreset; }
		const auto&	GetUseTaskID() const noexcept { return m_UseTaskID; }
	public:
		const auto GetSellValue(TraderID* ID, int* pValue) const noexcept {
			*ID = InvalidID;
			*pValue = -1;
			for (const auto& sf : GetsellFor()) {
				auto basev = sf.second;
				if (sf.first == InvalidID) {
					basev -= GetfleaMarketFee();
				}
				if (*pValue < basev) {
					*pValue = basev;
					*ID = sf.first;
				}
			}
			return (*pValue != -1);
		}
		void		ResetTaskUseID(void) noexcept { this->m_UseTaskID.clear(); }
		void		AddTaskUseID(int ID) noexcept { this->m_UseTaskID.emplace_back(ID); }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* InterParts = InterruptParts::Instance();
			int xofs = 0;
			int yofs = LineHeight;
			int yofs2 = yofs;
			{
				yofs += LineHeight;

				auto* typePtr = ItemTypeData::Instance()->FindPtr(GetTypeID());
				auto* catPtr = ItemCategoryData::Instance()->FindPtr(typePtr->GetCategoryID());
				if (catPtr->GetName() == "Weapons") {
					if (WindowSystem::ClickCheckBox(xp, yp + yofs, xp + WindowSystem::GetMsgLen(LineHeight, "GotoPreset"), yp + LineHeight + yofs, false, true, Gray10, "GotoPreset")) {
						InterParts->GotoNext(BGSelect::Custom);
						InterParts->SetInitParam(0, GetID());//武器ID
						InterParts->SetInitParam(1, InvalidID);//プリセットID
					}
				}
				else {
					if (m_isWeaponMod) {
						xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, (m_Recoil < 0.f) ? Green : Red, Black,
							"Recoil:%s%3.1f %%", (m_Recoil > 0.f) ? "+" : "", m_Recoil) + y_r(30)); yofs += LineHeight;
						xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, (m_Ergonomics >= 0.f) ? Green : Red, Black,
							"Ergonomics:%s%3.1f", (m_Ergonomics > 0.f) ? "+" : "", m_Ergonomics) + y_r(30)); yofs += LineHeight;
					}
					if (m_ChildPartsID.size() > 0 || m_ParentPartsID.size() > 0) {
						xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + LineHeight + yofs, LineHeight, STR_LEFT, White, Black, "Mods:") + y_r(30)); yofs += LineHeight;
					}
					int ysize = (int)((float)y_r(80));
					for (const auto& cp : m_ChildPartsID) {
						for (const auto& c : cp.Data) {
							auto* ptr = c.first;
							xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true) + y_r(30)); yofs += ysize;
						}
					}
					for (const auto& c : m_ParentPartsID) {
						auto* ptr = c;
						xofs = std::max<int>(xofs, ptr->Draw(xp + y_r(30), yp + yofs, y_r(800), ysize, 0, defaultcolor, (!WindowMngr->PosHitCheck(window) && !(xp == 0 && yp == 0)), false, true) + y_r(30)); yofs += ysize;
					}
				}
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
		void			SetOtherPartsID(const std::vector<ItemList>& itemList) noexcept {
			for (auto& cp : m_ChildPartsID) {
				for (auto& c : cp.Data) {
					for (const auto& t : itemList) {
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
			for (const auto& t : itemList) {
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
				for (const auto& t : itemList) {
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
		}
		void			SetOtherPartsID_After(const std::vector<ItemList>& itemList) noexcept {
			//自分を干渉相手にしている奴を探してそいつもリストに入れる　相思相愛
			for (const auto& t : itemList) {
				for (auto& cp : t.GetConflictParts()) {
					if (cp.first == this) {
						m_ConflictPartsID.resize(m_ConflictPartsID.size() + 1);
						m_ConflictPartsID.back().first = &t;
						m_ConflictPartsID.back().second = t.GetName();
					}
				}
			}
		}
	};

	struct properties {
		std::string			typeName;
	public:
		void GetJsonData(const nlohmann::json& data) {
			if (data.contains("__typename")) {
				if (!data["__typename"].is_null()) {
					typeName = data["__typename"];
				}
			}
		}
	};

	class ItemJsonData {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
		std::string									shortName;
		std::string									description;
		int											basePrice{ 0 };
		int											width{ 0 };
		int											height{ 0 };
		std::vector<std::string>					types;
		int											avg24hPrice{ 0 };
		int											low24hPrice{ 0 };
		int											lastOfferCount{ 0 };
		std::vector<std::pair<std::string, int>>	sellFor;
		std::vector<std::pair<std::string, int>>	buyFor;
		std::string									categorytypes;
		float										weight{ 0.f };
		float										recoilModifier{ -1000.f };
		float										ergonomicsModifier{ -1000.f };

		std::vector<std::string>					conflictingItems;

		std::vector<std::string>					usedInTasks;
		std::vector<std::string>					receivedFromTasks;
		std::vector<std::string>					bartersFor;
		std::vector<std::string>					bartersUsing;
		std::vector<std::string>					craftsFor;
		std::vector<std::string>					craftsUsing;
		int											fleaMarketFee{ 0 };

		properties									m_properties;
	public:
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];
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
			if (data.contains("avg24hPrice")) {
				avg24hPrice = data["avg24hPrice"];
			}

			if (data.contains("low24hPrice")) {
				if (!data["low24hPrice"].is_null()) {
					low24hPrice = data["low24hPrice"];
				}
			}

			if (data.contains("recoilModifier")) {
				if (!data["recoilModifier"].is_null()) {
					recoilModifier = data["recoilModifier"];
				}
			}
			if (data.contains("ergonomicsModifier")) {
				if (!data["ergonomicsModifier"].is_null()) {
					ergonomicsModifier = data["ergonomicsModifier"];
				}
			}

			if (data.contains("lastOfferCount")) {
				if (!data["lastOfferCount"].is_null()) {
					lastOfferCount = data["lastOfferCount"];
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
	};

	class ItemData : public SingletonBase<ItemData>, public DataParent<ItemID, ItemList> {
	private:
		friend class SingletonBase<ItemData>;
	private:
		ItemData() noexcept {
			std::string Path = "data/item/";
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					std::string Path2 = Path + d.cFileName + "/";
					auto data_t2 = GetFileNamesInDirectory(Path2.c_str());
					for (auto& d2 : data_t2) {
						if (d2.cFileName[0] != '.') {
							SetList((Path2 + d2.cFileName + "/").c_str());
						}
					}
				}
			}
			for (auto& t : m_List) {
				t.SetOtherPartsID(m_List);
			}
			for (auto& t : m_List) {
				t.SetOtherPartsID_After(m_List);
			}
			for (auto& t : m_List) {
				t.m_CheckJson = 0;
			}
		}
		~ItemData() noexcept {}
	private:
		std::vector<ItemJsonData> m_ItemJsonData;
	public:
		void GetJsonData(nlohmann::json& data) {
			m_ItemJsonData.clear();
			for (const auto& d : data["data"]["items"]) {
				m_ItemJsonData.resize(m_ItemJsonData.size() + 1);
				m_ItemJsonData.back().GetJsonData(d);
				m_ItemJsonData.back().m_IsFileOpened = false;
			}
		}
		void SaveDatabyJson() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : m_ItemJsonData) {
					if (L.GetIDstr() == jd.id) {
						L.m_CheckJson++;

						jd.m_IsFileOpened = true;

						std::ofstream outputfile(L.GetFilePath());
						outputfile << "IDstr=" + jd.id + "\n";
						outputfile << "Name=" + jd.name + "\n";
						outputfile << "ShortName=" + jd.shortName + "\n";
						outputfile << "Itemtype=" + jd.categorytypes + "\n";

						//既存のものを保持しておく1
						for (auto& m : L.GetMapID()) {
							auto* ptr = MapData::Instance()->FindPtr(m);
							if (ptr) {
								outputfile << "Map=" + ptr->GetName() + "\n";
							}
						}
						if (L.GetSightRange() >= 0) {
							outputfile << "SightRange=" + std::to_string(L.GetSightRange()) + "\n";
						}

						for (auto& m : L.GetChildParts()) {
							outputfile << "ChildParts=[\n";
							std::vector<std::string> Names;
							for (auto& d : m.Data) {
								if (d.first) {
									auto NmBuf = d.first->GetName();
									if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
										outputfile << "\t" + NmBuf + ((&d != &m.Data.back()) ? "," : "") + "\n";
										Names.emplace_back(NmBuf);
									}
								}
								else {
									//int a = 0;
								}
							}
							outputfile << "]\n";
						}
						if (jd.conflictingItems.size() > 0) {
							outputfile << "Conflict=[\n";
							std::vector<std::string> Names;
							for (auto& m : jd.conflictingItems) {
								auto NmBuf = m;
								if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
									outputfile << "\t" + NmBuf + ((&m != &jd.conflictingItems.back()) ? "," : "") + "\n";
									Names.emplace_back(NmBuf);
								}
							}
							outputfile << "]\n";
						}
						else {
							if (L.GetConflictParts().size() > 0) {
								outputfile << "Conflict=[\n";
								std::vector<std::string> Names;
								for (auto& m : L.GetConflictParts()) {
									if (m.first) {
										auto NmBuf = m.first->GetName();
										if (std::find_if(Names.begin(), Names.end(), [&](std::string& tgt) { return tgt == NmBuf; }) == Names.end()) {
											outputfile << "\t" + NmBuf + ((&m != &L.GetConflictParts().back()) ? "," : "") + "\n";
											Names.emplace_back(NmBuf);
										}
									}
									else {
										//int a = 0;
									}
								}
								outputfile << "]\n";
							}
						}

						if (jd.recoilModifier > -500) {
							outputfile << "Recoil=" + std::to_string(jd.recoilModifier) + "\n";
						}
						else {
							outputfile << "Recoil=" + std::to_string(L.GetRecoil()) + "\n";
						}
						if (jd.ergonomicsModifier > -500) {
							outputfile << "Ergonomics=" + std::to_string(jd.ergonomicsModifier) + "\n";
						}
						else {
							outputfile << "Ergonomics=" + std::to_string(L.GetErgonomics()) + "\n";
						}


						outputfile << "Information_Eng=" + jd.description + "\n";
						outputfile << "basePrice=" + std::to_string(jd.basePrice) + "\n";
						outputfile << "width=" + std::to_string(jd.width) + "\n";
						outputfile << "height=" + std::to_string(jd.height) + "\n";
						outputfile << "avg24hPrice=" + std::to_string(jd.avg24hPrice) + "\n";
						outputfile << "low24hPrice=" + std::to_string(jd.low24hPrice) + "\n";
						outputfile << "lastOfferCount=" + std::to_string(jd.lastOfferCount) + "\n";
						for (auto& sf : jd.sellFor) {
							outputfile << "Sell_" + sf.first + "=" + std::to_string(sf.second) + "\n";
						}
						//for (auto& bf : jd.buyFor) { outputfile << "Buy_" + bf.first + "=" + std::to_string(bf.second) + "\n"; }
						outputfile << "weight=" + std::to_string(jd.weight) + "\n";
						//std::vector<std::string>					usedInTasks;
						//std::vector<std::string>					receivedFromTasks;
						//std::vector<std::string>					bartersFor;
						//std::vector<std::string>					bartersUsing;
						//std::vector<std::string>					craftsFor;
						//std::vector<std::string>					craftsUsing;
						outputfile << "fleaMarketFee=" + std::to_string(jd.fleaMarketFee) + "\n";
						outputfile << "propertiestype=" + (jd.m_properties.typeName) + "\n";
						outputfile.close();
						break;
					}
				}
			}
			for (auto& jd : m_ItemJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string FolderPath = jd.categorytypes;

					std::string ParentPath = "data/item/Maked/Maked/" + jd.categorytypes;

					CreateDirectory(ParentPath.c_str(), NULL);

					std::string ItemName = jd.name;
					auto SubStrs = [&](const char* str) {
						while (true) {
							auto now = ItemName.find(str);
							if (now != std::string::npos) {
								ItemName = ItemName.substr(0, now) + ItemName.substr(now + 1);
							}
							else {
								break;
							}
						}
					};
					SubStrs(".");

					SubStrs("\\");
					SubStrs("/");
					SubStrs(":");
					SubStrs("*");
					SubStrs("?");
					SubStrs("\"");
					SubStrs(">");
					SubStrs("<");
					SubStrs("|");

					std::string Name = ParentPath +"/" + ItemName + ".txt";
					std::ofstream outputfile(Name);
					outputfile << "IDstr=" + jd.id + "\n";
					outputfile << "Name=" + jd.name + "\n";
					outputfile << "ShortName=" + jd.shortName + "\n";
					outputfile << "Itemtype=" + jd.categorytypes + "\n";

					//既存のものを保持しておく1
					/*
					for (auto& m : L.GetMapID()) {
						auto* ptr = MapData::Instance()->FindPtr(m);
						if (ptr) {
							outputfile << "Map=" + ptr->GetName() + "\n";
						}
					}
					if (L.GetSightRange() >= 0) {
						outputfile << "SightRange=" + std::to_string(L.GetSightRange()) + "\n";
					}
					for (auto& m : L.GetChildParts()) {
						outputfile << "ChildParts=[\n";
						for (auto& d : m.Data) {
							outputfile << "\t" + d.first->GetName() + ((&d != &m.Data.back()) ? "," : "") + "\n";
						}
						outputfile << "]\n";
					}
					if (L.GetConflictParts().size() > 0) {
						outputfile << "Conflict=[\n";
						for (auto& m : L.GetConflictParts()) {
							outputfile << "\t" + m.first->GetName() + ((&m != &L.GetConflictParts().back()) ? "," : "") + "\n";
						}
						outputfile << "]\n";
					}
					//*/

					if (jd.recoilModifier > -500) {
						outputfile << "Recoil=" + std::to_string(jd.recoilModifier) + "\n";
					}
					else {
						//outputfile << "Recoil=" + std::to_string(0) + "\n";
					}
					if (jd.ergonomicsModifier > -500) {
						outputfile << "Ergonomics=" + std::to_string(jd.ergonomicsModifier) + "\n";
					}
					else {
						//outputfile << "Ergonomics=" + std::to_string(0) + "\n";
					}


					outputfile << "Information_Eng=" + jd.description + "\n";
					outputfile << "basePrice=" + std::to_string(jd.basePrice) + "\n";
					outputfile << "width=" + std::to_string(jd.width) + "\n";
					outputfile << "height=" + std::to_string(jd.height) + "\n";
					outputfile << "avg24hPrice=" + std::to_string(jd.avg24hPrice) + "\n";
					outputfile << "low24hPrice=" + std::to_string(jd.low24hPrice) + "\n";
					outputfile << "lastOfferCount=" + std::to_string(jd.lastOfferCount) + "\n";
					for (auto& sf : jd.sellFor) {
						outputfile << "Sell_" + sf.first + "=" + std::to_string(sf.second) + "\n";
					}
					//for (auto& bf : jd.buyFor) { outputfile << "Buy_" + bf.first + "=" + std::to_string(bf.second) + "\n"; }
					outputfile << "weight=" + std::to_string(jd.weight) + "\n";
					//std::vector<std::string>					usedInTasks;
					//std::vector<std::string>					receivedFromTasks;
					//std::vector<std::string>					bartersFor;
					//std::vector<std::string>					bartersUsing;
					//std::vector<std::string>					craftsFor;
					//std::vector<std::string>					craftsUsing;
					outputfile << "fleaMarketFee=" + std::to_string(jd.fleaMarketFee) + "\n";
					outputfile << "propertiestype=" + (jd.m_properties.typeName) + "\n";
					outputfile.close();

					//RemoveDirectory(Path.c_str());
				}
			}
		}
		void CheckThroughJson(void) noexcept {
			for (auto& t : m_List) {
				if (t.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : ";
					ErrMes += t.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			for (auto& t : m_List) {
				if (t.m_CheckJson >= 2) {
					std::string ErrMes = "Error : Be repeated ";
					ErrMes += std::to_string(t.m_CheckJson);
					ErrMes += " : ";
					ErrMes += t.GetName();

					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
	};

	const int		ItemList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive, bool IsFir, bool IsDrawBuy) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* Input = InputControl::Instance();
		int xs = xsize;
		int  Xsize = 0;

		int xg = 0;
		if (GetIcon().GetGraph()) {
			xg = (GetIcon().GetXSize() >= GetIcon().GetYSize())
				? (ysize * GetIcon().GetXSize() / GetIcon().GetYSize())
				: (ysize * GetIcon().GetYSize() / GetIcon().GetXSize());
		}
		bool IsLocked = PlayerData::Instance()->GetItemLock(this->GetIDstr().c_str());
		int FirSize = (IsFir || IsLocked) ? 36 : 0;
		xg += FirSize;
		auto Name = this->GetShortName();
		{
			if (xsize > 0) {
				int countbuf = 0;
				while (true) {
					if (count > 0) {
						Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s x%2d", Name.c_str(), count);
					}
					else {
						Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
					}
					if ((xs - xg) < Xsize) {
						Name = Name.substr(0, Name.size() * (xs - xg) / Xsize - 2) + "…";
					}
					else {
						break;
					}
					countbuf++;
					if (countbuf > 100) {
						Name = "…";
						Xsize = LineHeight * 9 / 10;
						break;
					}
				}
			}
			else {
				if (count > 0) {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s x%2d", Name.c_str(), count);
				}
				else {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, "%s", Name.c_str());
				}
			}
			xs = std::max(xs, Xsize);
		}

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
		if (count > 0) {
			WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STR_LEFT, White, Black, "%s x%2d", Name.c_str(), count);
		}
		else {
			WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STR_LEFT, White, Black, "%s", Name.c_str());
		}
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(), xp + FirSize + Xsize + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			Xsize += xg;
		}

		if (IsLocked) {
			DrawControl::Instance()->SetDrawRotaLock(DrawLayer::Front, xp + FirSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}
		if (IsFir) {
			DrawControl::Instance()->SetDrawRotaFiR(DrawLayer::Normal, xp + FirSize / 2, yp + ysize / 2, 1.f, 0.f, true);
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
						FontHandle::FontXCenter::RIGHT, FontHandle::FontYCenter::BOTTOM, Input->GetMouseX(), Input->GetMouseY(), Color, Black,
						"最高値:%s = %d", TraderName.c_str(), Value
					);
					DrawControl::Instance()->SetString(DrawLayer::Front,
						FontPool::FontType::Nomal_Edge, LineHeight,
						FontHandle::FontXCenter::RIGHT, FontHandle::FontYCenter::TOP, Input->GetMouseX(), Input->GetMouseY(), Color, Black,
						"マス単価: %d", Value / (Getwidth()*Getheight())
					);
				}
			}
		}
		return Xsize;
	}

	class ItemGetData {
		ItemID				m_ID{ InvalidID };
		int					m_Count{ 0 };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetCount() const noexcept { return m_Count; }
		void			Set(ItemID id, int count) noexcept {
			m_ID = id;
			m_Count = count;
		}
	};
	void			SetItem(std::vector<ItemGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto ID = ItemData::Instance()->FindID(mes.substr(0, L).c_str());
			if (std::find_if(Data->begin(), Data->end(), [&](const ItemGetData& obj) {return obj.GetID() == ID; }) == Data->end()) {
				ItemGetData tmp;
				tmp.Set(ID, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};
};