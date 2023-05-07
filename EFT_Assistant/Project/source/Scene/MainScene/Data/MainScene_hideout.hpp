#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int HideoutID;

	class HideoutGetData {
		HideoutID			m_ID{ InvalidID };
		int					m_Lv{ 0 };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetLv() const noexcept { return m_Lv; }
		void			Set(ItemID id, int lv) noexcept {
			m_ID = id;
			m_Lv = lv;
		}
	};
	void			SetHideoutLv(std::vector<HideoutGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto ID = ItemData::Instance()->FindID(mes.substr(0, L).c_str());
			if (std::find_if(Data->begin(), Data->end(), [&](const HideoutGetData& obj) {return obj.GetID() == ID; }) == Data->end()) {
				HideoutGetData tmp;
				tmp.Set(ID, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};


	struct CraftData {
		int											durationTime{ 0 };
		std::vector<ItemGetData>					m_ItemReq;
		std::vector<ItemGetData>					m_ItemReward;
	};

	struct LvData {
		//開放データ
		int											constructionTime{ 0 };
		std::string									Information_Eng;
		std::vector<ItemGetData>					m_ItemReq;
		std::vector<HideoutGetData>					m_HideoutReq;
		//クラフトデータ
		std::vector<CraftData>						m_ItemCraft;
	};

	class HideoutList : public ListParent<HideoutID> {
		std::array<LvData, 3> m_LvData;
	private:
		//追加設定
		void	Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>& Args) noexcept override {
			std::string LEFTBuf = LEFT.substr(3);
			std::string NumBuf2 = LEFT.substr(2, 1);
			int ID = 0;
			if (std::all_of(NumBuf2.cbegin(), NumBuf2.cend(), isdigit)){
				ID = std::stoi(NumBuf2) - 1;
			}
			//開放データ
			if (LEFTBuf == "constructionTime") { m_LvData.at(ID).constructionTime = std::stoi(RIGHT); }
			if (LEFTBuf == "Information_Eng") { m_LvData.at(ID).Information_Eng = RIGHT; }
			if (LEFTBuf == "itemReq") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							SetItem(&m_LvData.at(ID).m_ItemReq, a);
						}
					}
				}
				else {
					SetItem(&m_LvData.at(ID).m_ItemReq, RIGHT);
				}
			}
			if (LEFTBuf == "hideoutReq") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							SetHideoutLv(&m_LvData.at(ID).m_HideoutReq, a);
						}
					}
				}
				else {
					SetHideoutLv(&m_LvData.at(ID).m_HideoutReq, RIGHT);
				}
			}
			//クラフトレシピ
			if (LEFTBuf == "craftduration") {
				m_LvData.at(ID).m_ItemCraft.resize(m_LvData.at(ID).m_ItemCraft.size() + 1);
			}
			if (LEFTBuf == "craftitemReq") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReq, a);
						}
					}
				}
				else {
					SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReq, RIGHT);
				}
			}
			if (LEFTBuf == "craftitemReward") {
				if (Args.size() > 0) {
					for (auto&a : Args) {
						if (a == "or") {

						}
						else {
							SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReward, a);
						}
					}
				}
				else {
					SetItem(&m_LvData.at(ID).m_ItemCraft.back().m_ItemReward, RIGHT);
				}
			}
			//
		}
		void	Load_Sub() noexcept override {}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		int										m_CheckJson{ 0 };
	public:
		const auto&	GetLvData() const noexcept { return m_LvData; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) const noexcept;
		void			DrawWindow(WindowSystem::WindowControl* /*window*/, unsigned int /*defaultcolor*/, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept {
			//auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* InterParts = InterruptParts::Instance();
			int xofs = 0;
			int yofs = LineHeight;
			int yofs2 = yofs;
			{
				yofs += LineHeight;
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
	};

	class HideoutJsonData {
	public:
		bool										m_IsFileOpened{ false };
	public:
		std::string									id;
		std::string									name;
		std::array<LvData, 3>						m_LvData;
	public:
		void GetJsonData(const nlohmann::json& data) {
			id = data["id"];
			name = data["name"];

			for (auto& L : m_LvData) {
				auto LV = "Lv" + std::to_string((&L - &m_LvData.front()) + 1);
				/*
				outputfile << LV + "constructionTime=" + std::to_string(L.constructionTime) + "\n";
				outputfile << LV + "Information_Eng=" + L.Information_Eng + "\n";
				{
					outputfile << LV + "itemReq=[";
					for (auto& m : L.m_ItemReq) {
						outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
						outputfile << "x" + std::to_string(m.GetCount());
						if (&m != &L.m_ItemReq.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
				{
					outputfile << LV + "stationLevelReq=[";
					for (auto& m : L.m_HideoutReq) {
						outputfile << HideoutData::Instance()->FindPtr(m.GetID())->GetName();
						outputfile << "x" + std::to_string(m.GetLv());
						if (&m != &L.m_HideoutReq.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
				for (auto& c : L.m_ItemCraft) {
					outputfile << LV + "craftduration=" + std::to_string(c.durationTime) + "\n";
					{
						outputfile << LV + "craftitemReq=[";
						for (auto& m : c.m_ItemReq) {
							outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
							outputfile << "x" + std::to_string(m.GetCount());
							if (&m != &c.m_ItemReq.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					{
						outputfile << LV + "craftitemReward=[";
						for (auto& m : c.m_ItemReward) {
							outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
							outputfile << "x" + std::to_string(m.GetCount());
							if (&m != &c.m_ItemReward.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
				}
				//*/
			}

		}
	};

	class HideoutData : public SingletonBase<HideoutData>, public DataParent<HideoutID, HideoutList> {
	private:
		friend class SingletonBase<HideoutData>;
	private:
		HideoutData() noexcept {
			std::string Path = "data/Hideout/";
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
				t.m_CheckJson = 0;
			}
		}
		~HideoutData() noexcept {}
	private:
		std::vector<HideoutJsonData> m_HideoutJsonData;
	public:
		void GetJsonData(nlohmann::json& data) {
			m_HideoutJsonData.clear();
			for (const auto& d : data["data"]["Hideouts"]) {
				m_HideoutJsonData.resize(m_HideoutJsonData.size() + 1);
				m_HideoutJsonData.back().GetJsonData(d);
				m_HideoutJsonData.back().m_IsFileOpened = false;
			}
		}
		void SaveDatabyJson() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : m_HideoutJsonData) {
					if (L.GetIDstr() == jd.id) {
						L.m_CheckJson++;

						jd.m_IsFileOpened = true;

						std::ofstream outputfile(L.GetFilePath());
						outputfile << "IDstr=" + jd.id + "\n";
						outputfile << "Name=" + jd.name + "\n";

						for (auto& L2 : jd.m_LvData) {
							auto LV = "Lv" + std::to_string((&L2 - &jd.m_LvData.front()) + 1);

							outputfile << LV + "constructionTime=" + std::to_string(L2.constructionTime) + "\n";
							outputfile << LV + "Information_Eng=" + L2.Information_Eng + "\n";
							{
								outputfile << LV + "itemReq=[";
								for (auto& m : L2.m_ItemReq) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetCount());
									if (&m != &L2.m_ItemReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "stationLevelReq=[";
								for (auto& m : L2.m_HideoutReq) {
									outputfile << HideoutData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetLv());
									if (&m != &L2.m_HideoutReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							for (auto& c : L2.m_ItemCraft) {
								outputfile << LV + "craftduration=" + std::to_string(c.durationTime) + "\n";
								{
									outputfile << LV + "craftitemReq=[";
									for (auto& m : c.m_ItemReq) {
										outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
										outputfile << "x" + std::to_string(m.GetCount());
										if (&m != &c.m_ItemReq.back()) {
											outputfile << ",";
										}
									}
									outputfile << "]\n";
								}
								{
									outputfile << LV + "craftitemReward=[";
									for (auto& m : c.m_ItemReward) {
										outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
										outputfile << "x" + std::to_string(m.GetCount());
										if (&m != &c.m_ItemReward.back()) {
											outputfile << ",";
										}
									}
									outputfile << "]\n";
								}
							}
						}

						outputfile.close();
						break;
					}
				}
			}
			for (auto& jd : m_HideoutJsonData) {
				if (!jd.m_IsFileOpened) {
					std::string ParentPath = "data/Hideout/Maked/Maked/";
					CreateDirectory(ParentPath.c_str(), NULL);
					std::string HideoutName = jd.name;
					auto SubStrs = [&](const char* str) {
						while (true) {
							auto now = HideoutName.find(str);
							if (now != std::string::npos) {
								HideoutName = HideoutName.substr(0, now) + HideoutName.substr(now + 1);
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

					std::string Name = ParentPath + "/" + HideoutName + ".txt";
					std::ofstream outputfile(Name);
					outputfile << "IDstr=" + jd.id + "\n";
					outputfile << "Name=" + jd.name + "\n";

					for (auto& L : jd.m_LvData) {
						auto LV = "Lv" + std::to_string((&L - &jd.m_LvData.front()) + 1);

						outputfile << LV + "constructionTime=" + std::to_string(L.constructionTime) + "\n";
						outputfile << LV + "Information_Eng=" + L.Information_Eng + "\n";
						{
							outputfile << LV + "itemReq=[";
							for (auto& m : L.m_ItemReq) {
								outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
								outputfile << "x" + std::to_string(m.GetCount());
								if (&m != &L.m_ItemReq.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						{
							outputfile << LV + "stationLevelReq=[";
							for (auto& m : L.m_HideoutReq) {
								outputfile << HideoutData::Instance()->FindPtr(m.GetID())->GetName();
								outputfile << "x" + std::to_string(m.GetLv());
								if (&m != &L.m_HideoutReq.back()) {
									outputfile << ",";
								}
							}
							outputfile << "]\n";
						}
						for (auto& c : L.m_ItemCraft) {
							outputfile << LV + "craftduration=" + std::to_string(c.durationTime) + "\n";
							{
								outputfile << LV + "craftitemReq=[";
								for (auto& m : c.m_ItemReq) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetCount());
									if (&m != &c.m_ItemReq.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
							{
								outputfile << LV + "craftitemReward=[";
								for (auto& m : c.m_ItemReward) {
									outputfile << ItemData::Instance()->FindPtr(m.GetID())->GetName();
									outputfile << "x" + std::to_string(m.GetCount());
									if (&m != &c.m_ItemReward.back()) {
										outputfile << ",";
									}
								}
								outputfile << "]\n";
							}
						}
					}
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

	const int		HideoutList::Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) const noexcept {
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
		bool IsLocked = false;// PlayerData::Instance()->GetHideoutLock(this->GetIDstr().c_str());
		int FirSize = (IsLocked) ? 36 : 0;
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
					HideoutData::Instance()->FindPtr((HideoutID)(win->m_FreeID - 0xFFFF))->DrawWindow(win, Gray10, win->GetPosX(), win->GetPosY());
				});
			}
		}
		{
			if (count > 0) {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s x%2d", Name.c_str(), count);
			}
			else {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, "%s", Name.c_str());
			}
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
		return Xsize;
	}
};