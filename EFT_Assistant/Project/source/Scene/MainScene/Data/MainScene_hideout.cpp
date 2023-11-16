#include "MainScene_hideout.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void HideoutJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		m_LvData.clear();
		for (auto& Ld : data["levels"]) {
			m_LvData.resize(m_LvData.size() + 1);
			auto& L = this->m_LvData.back();
			L.constructionTime = Ld["constructionTime"];
			L.m_Information_Eng = Ld["description"];
			{
				L.m_ItemReq.clear();
				if (Ld.contains("itemRequirements")) {
					if (!Ld["itemRequirements"].is_null()) {
						for (const auto&m : Ld["itemRequirements"]) {
							ItemGetData buf;
							buf.Set(m["item"]["name"], m["count"]);
							L.m_ItemReq.emplace_back(buf);
						}
					}
				}
				for (auto& T : L.m_ItemReq) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
				}
			}
			{
				L.m_Parent.clear();
				if (Ld.contains("stationLevelRequirements")) {
					if (!Ld["stationLevelRequirements"].is_null()) {
						for (const auto&m : Ld["stationLevelRequirements"]) {
							HideoutGetData buf;
							buf.Set(m["station"]["name"], m["level"]);
							L.m_Parent.emplace_back(buf);
						}
					}
				}
				for (auto& T : L.m_Parent) {
					T.CheckID(DataBase::Instance()->GetHideoutData().get());
				}
			}
			{
				L.m_TraderReq.clear();
				if (Ld.contains("traderRequirements")) {
					if (!Ld["traderRequirements"].is_null()) {
						for (const auto&m : Ld["traderRequirements"]) {
							TraderGetData buf;
							buf.Set(m["trader"]["name"], m["level"]);//todo:levelが廃止予定
							L.m_TraderReq.emplace_back(buf);
						}
					}
				}
				for (auto& T : L.m_TraderReq) {
					T.CheckID(DataBase::Instance()->GetTraderData().get());
				}
			}
			//
			L.m_ItemCraft.clear();
			for (auto& Cd : Ld["crafts"]) {
				L.m_ItemCraft.resize(L.m_ItemCraft.size() + 1);
				//
				L.m_ItemCraft.back().durationTime = Cd["duration"];
				//
				{
					L.m_ItemCraft.back().m_ItemReq.clear();
					if (Cd.contains("requiredItems")) {
						if (!Cd["requiredItems"].is_null()) {
							for (const auto&m : Cd["requiredItems"]) {
								ItemGetData buf;
								buf.Set(m["item"]["name"], m["count"]);
								L.m_ItemCraft.back().m_ItemReq.emplace_back(buf);
							}
						}
					}
					for (auto& T : L.m_ItemCraft.back().m_ItemReq) {
						T.CheckID(DataBase::Instance()->GetItemData().get());
					}
				}
				//
				{
					L.m_ItemCraft.back().m_ItemReward.clear();
					if (Cd.contains("rewardItems")) {
						if (!Cd["rewardItems"].is_null()) {
							for (const auto&m : Cd["rewardItems"]) {
								ItemGetData buf;
								buf.Set(m["item"]["name"], m["count"]);
								L.m_ItemCraft.back().m_ItemReward.emplace_back(buf);
							}
						}
					}
					for (auto& T : L.m_ItemCraft.back().m_ItemReward) {
						T.CheckID(DataBase::Instance()->GetItemData().get());
					}
				}
				//
			}
		}
	}
	void HideoutJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		for (const auto&H : DataBase::Instance()->GetHideoutData()->GetList()) {
			if (H.GetName() != m_name) { continue; }
			for (const auto& L : H.GetLvData()) {
				int Lv = (int)(&L - &H.GetLvData().front());
				auto& L2 = this->m_LvData.at(Lv);
				for (auto& C : L.m_ItemCraft) {
					for (auto&C2 : L2.m_ItemCraft) {
						C2.sortbuffer = -1;
						bool IsHit = true;
						for (auto& T : C.m_ItemReward) {
							for (auto& T2 : C2.m_ItemReward) {
								if (T.GetID() != T2.GetID()) {
									IsHit = false;
									break;
								}
								if (T.GetValue() != T2.GetValue()) {
									IsHit = false;
									break;
								}
							}
						}
						if (IsHit) {
							C2.sortbuffer = (int)(&C - &L.m_ItemCraft.front());
						}
					}
					std::sort(L2.m_ItemCraft.begin(), L2.m_ItemCraft.end(),
						[&](const HideoutLvData::CraftData& a, const HideoutLvData::CraftData& b) { return a.sortbuffer < b.sortbuffer; });
				}
			}
			break;
		}
		for (auto& L2 : this->m_LvData) {
			auto LV = "Lv" + std::to_string((&L2 - &this->m_LvData.front()) + 1);

			outputfile << LV + "constructionTime=" + std::to_string(L2.constructionTime) + "\n";
			outputfile << LV + "Information_Eng=" + L2.m_Information_Eng + "\n";
			outputfile << LV + "Information_Jpn=" + L2.m_Information_Jpn + "\n";
			{
				outputfile << LV + "itemReq=[";
				for (auto& m : L2.m_ItemReq) {
					outputfile << m.GetOutputStr();
					if (&m != &L2.m_ItemReq.back()) {
						outputfile << DIV_STR;
					}
				}
				outputfile << "]\n";
			}
			{
				outputfile << LV + "stationLevelReq=[";
				for (auto& m : L2.m_Parent) {
					outputfile << m.GetOutputStr();
					if (&m != &L2.m_Parent.back()) {
						outputfile << DIV_STR;
					}
				}
				outputfile << "]\n";
			}
			{
				outputfile << LV + "traderRequirements=[";
				for (auto& m : L2.m_TraderReq) {
					outputfile << m.GetOutputStr();
					if (&m != &L2.m_TraderReq.back()) {
						outputfile << DIV_STR;
					}
				}
				outputfile << "]\n";
			}
			for (auto& c : L2.m_ItemCraft) {
				outputfile << LV + "craftduration=" + std::to_string(c.durationTime) + "\n";
				{
					outputfile << LV + "craftitemReq=[";
					for (auto& m : c.m_ItemReq) {
						outputfile << m.GetOutputStr();
						if (&m != &c.m_ItemReq.back()) {
							outputfile << DIV_STR;
						}
					}
					outputfile << "]\n";
				}
				{
					outputfile << LV + "craftitemReward=[";
					for (auto& m : c.m_ItemReward) {
						outputfile << m.GetOutputStr();
						if (&m != &c.m_ItemReward.back()) {
							outputfile << DIV_STR;
						}
					}
					outputfile << "]\n";
				}
			}
		}
	}

	void			HideoutList::SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
		std::string LEFTBuf = LEFT.substr(3);
		std::string NumBuf2 = LEFT.substr(2, 1);
		if (std::all_of(NumBuf2.cbegin(), NumBuf2.cend(), isdigit)) {
			size_t ID = (size_t)(std::stoi(NumBuf2) - 1);
			if (m_LvData.size() <= ID) { this->m_LvData.resize(ID + 1); }
			auto& data = this->m_LvData.at(ID);
			//開放データ
			if (LEFTBuf == "constructionTime") { data.constructionTime = std::stoi(Args[0]); }
			if (LEFTBuf == "Information_Eng") { data.m_Information_Eng = Args[0]; }
			if (LEFTBuf == "Information_Jpn") { data.m_Information_Jpn = Args[0]; }
			if (LEFTBuf == "itemReq") {
				for (auto&a : Args) {
					SetGetData<ItemGetData>(&data.m_ItemReq, a, "x", false);
				}
			}
			if (LEFTBuf == "stationLevelReq") {
				for (auto&a : Args) {
					SetGetData<HideoutGetData>(&data.m_Parent, a, "x", false);
				}
			}
			if (LEFTBuf == "traderRequirements") {
				for (auto&a : Args) {
					SetGetData<TraderGetData>(&data.m_TraderReq, a, "x", false);
				}
			}
			//クラフトレシピ
			if (LEFTBuf == "craftduration") {
				data.m_ItemCraft.resize(data.m_ItemCraft.size() + 1);
				data.m_ItemCraft.back().durationTime = std::stoi(Args[0]);
			}
			if (LEFTBuf == "craftitemReq") {
				for (auto&a : Args) {
					SetGetData<ItemGetData>(&data.m_ItemCraft.back().m_ItemReq, a, "x", false);
				}
			}
			if (LEFTBuf == "craftitemReward") {
				for (auto&a : Args) {
					SetGetData<ItemGetData>(&data.m_ItemCraft.back().m_ItemReward, a, "x", false);
				}
			}
		}
	}
	void			HideoutList::Load_Sub() noexcept {
		for (auto& L : this->m_LvData) {
			for (auto& T : L.m_ItemReq) {
				T.CheckID(DataBase::Instance()->GetItemData().get());
			}
			for (auto& T : L.m_Parent) {
				T.CheckID(DataBase::Instance()->GetHideoutData().get());
			}
			for (auto& T : L.m_TraderReq) {
				T.CheckID(DataBase::Instance()->GetTraderData().get());
			}
			for (auto& C : L.m_ItemCraft) {
				for (auto& T : C.m_ItemReq) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
				}
				for (auto& T : C.m_ItemReward) {
					T.CheckID(DataBase::Instance()->GetItemData().get());
				}
			}
		}
		SetOtherPartsID();
	}

	const int		HideoutList::Draw(int xp, int yp, int xsize, int ysize, int Lv, unsigned int defaultcolor, bool Clickactive, int count) noexcept {
		auto& LvData = m_LvData.at(Lv - 1);
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
		std::string Name = this->GetShortName();
		std::string AfterName = "";
		if (Lv > 0) {
			AfterName += " Lv" + std::to_string(Lv);
		}
		if (count > 0) {
			AfterName += " x" + std::to_string(count);
		}
		{
			if (xsize > 0) {
				int Lvbuf = 0;
				while (true) {
					Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName);
					if ((xs - xg) < Xsize) {
						Name = Name.substr(0, (size_t)((int)(Name.size()) * (xs - xg) / Xsize - AfterName.length())) + "…";
					}
					else {
						break;
					}
					Lvbuf++;
					if (Lvbuf > 100) {
						Name = "…";
						Xsize = LineHeight * 9 / 10;
						break;
					}
				}
			}
			else {
				Xsize = WindowSystem::GetMsgLen(LineHeight * 9 / 10, Name + AfterName, Lv);
			}
			xs = std::max(xs, Xsize);
		}

		if (LvData.m_ItemCraft.size() > 0) {
			defaultcolor = Green;//クラフトできるアイテムがあります//ビットコインは除外？
		}


		if (WindowSystem::ClickCheckBox(xp, yp, xp + xs, yp + ysize, false, Clickactive, defaultcolor, "")) {
			auto sizeXBuf = y_r(0);
			auto sizeYBuf = y_r(0);

			if (Input->GetCtrlKey().press()) {
				DrawUnlockWindow(nullptr, 0, Lv, y_r(1920), y_r(1080), &sizeXBuf, &sizeYBuf);//試しにサイズ計測
			}
			else {
				DrawCraftWindow(nullptr, 0, Lv, y_r(1920), y_r(1080), &sizeXBuf, &sizeYBuf, 100);//試しにサイズ計測
			}
			//
			signed long long FreeID = (signed long long)GetID() + 0xFFFF;
			//同じIDの奴いたら消そうぜ
			WindowMngr->DeleteAll();
			bool isHit = false;
			if (Input->GetMouseY() <= LineHeight) {
				isHit = true;
			}

			if (!isHit) {
				//ウィンドウ追加
				m_DrawWindowLv = Lv;

				std::string NameTmp = GetName();
				NameTmp += " Lv" + std::to_string(Lv);
				if (Input->GetCtrlKey().press()) {
					NameTmp += " Unlock";
					WindowMngr->Add()->Set(y_r(960) - sizeXBuf / 2, LineHeight + y_r(10), sizeXBuf, sizeYBuf, 0, NameTmp.c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
						DataBase::Instance()->GetHideoutData()->FindPtr((HideoutID)(win->m_FreeID - 0xFFFF))->DrawUnlockWindow(win, Gray10, this->m_DrawWindowLv, win->GetPosX(), win->GetPosY());
					});
				}
				else {
					NameTmp += " Craft";
					auto Total = (y_r(64) + y_r(5))*(int)(LvData.m_ItemCraft.size());
					WindowMngr->Add()->Set(y_r(960) - sizeXBuf / 2, LineHeight + y_r(10), sizeXBuf, sizeYBuf, Total, NameTmp.c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
						DataBase::Instance()->GetHideoutData()->FindPtr((HideoutID)(win->m_FreeID - 0xFFFF))->DrawCraftWindow(win, Gray10, this->m_DrawWindowLv, win->GetPosX(), win->GetPosY());
					});
				}
			}
		}
		{
			if (Lv > 0) {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, Name + " Lv%1d", Lv);
			}
			else {
				WindowSystem::SetMsg(xp + FirSize, yp, xp + FirSize, yp + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black, Name);
			}
		}
		if (GetIcon().GetGraph()) {
			float Scale = (float)ysize / (float)(std::min(GetIcon().GetXSize(), GetIcon().GetYSize()));
			float rad = (GetIcon().GetXSize() >= GetIcon().GetYSize()) ? deg2rad(0) : deg2rad(90);
			DrawControl::Instance()->SetDrawRotaGraph(DrawLayer::Normal, GetIcon().GetGraph(), xp + FirSize + Xsize + (int)(((float)GetIcon().GetXSize() * std::cos(rad) + (float)GetIcon().GetYSize() * std::sin(rad)) / 2.f * Scale), yp + ysize / 2, Scale, rad, false);
			Xsize += xg;
		}
		Xsize = std::max(Xsize, xs);

		if (IsLocked) {
			DrawControl::Instance()->SetDrawRotaLock(DrawLayer::Front, xp + FirSize / 2, yp + ysize / 2, 1.f, 0.f, true);
		}
		return Xsize;
	}
	void			HideoutList::DrawUnlockWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs, int* ys) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		//auto* InterParts = InterruptParts::Instance();
		int xofs = 0;
		int yofs = LineHeight;
		int yofs2 = yofs;
		{
			yofs += LineHeight;
		}
		if (Lv >= 1) {
			int yofs_OLD = yofs;
			int ysize = (y_r(80));
			auto& LvData = m_LvData.at(Lv - 1);
			if (LvData.m_ItemReq.size() > 0) {
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				{
					for (const auto& I : LvData.m_ItemReq) {
						auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(I.GetID());
						if (ptr) {
							xofs_buf = xofs + y_r(10);
							int xstart = xp + xofs_buf;
							xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs_buf, y_r(300), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true);
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs_buf, xp + xofs_buf, yp + yofs_buf + ysize, Gray75, false);
							yofs_buf += ysize + y_r(5);
						}
					}
				}
				{
					xofs_buf = std::max(xofs_buf, xofs + y_r(300));
				}
				xofs = std::max(xofs, xofs_buf + y_r(10));
				yofs = std::max(yofs, yofs_buf + y_r(10));
			}
			if (LvData.m_Parent.size() > 0) {
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				{
					for (const auto& I : LvData.m_Parent) {
						auto* ptr = DataBase::Instance()->GetHideoutData()->FindPtr(I.GetID());
						if (ptr) {
							xofs_buf = xofs + y_r(10);
							int xstart = xp + xofs_buf;
							xofs_buf += ptr->Draw(xp + xofs_buf, yp + yofs_buf, y_r(300), ysize, I.GetValue(), defaultcolor, !WindowMngr->PosHitCheck(window), 0);
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs_buf, xp + xofs_buf, yp + yofs_buf + ysize, Gray75, false);
							yofs_buf += ysize + y_r(5);
						}
					}
				}
				{
					xofs_buf = std::max(xofs_buf, xofs + y_r(300));
				}
				xofs = std::max(xofs, xofs_buf + y_r(10));
				yofs = std::max(yofs, yofs_buf + y_r(10));
			}
			if (LvData.m_TraderReq.size() > 0) {
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				{
					for (const auto& I : LvData.m_TraderReq) {
						auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(I.GetID());
						if (ptr) {
							xofs_buf = xofs + y_r(10);
							int xstart = xp + xofs_buf;

							if (WindowSystem::ClickCheckBox(
								xp + xofs_buf, yp + yofs_buf,
								xp + xofs_buf + y_r(300), yp + yofs_buf + ysize,
								false, !WindowMngr->PosHitCheck(nullptr), defaultcolor, ptr->GetName() + " Lv" + std::to_string(I.GetValue()))) {
							}
							xofs_buf += y_r(300);
							DrawControl::Instance()->SetDrawBox(DrawLayer::Normal, xstart, yp + yofs_buf, xp + xofs_buf, yp + yofs_buf + ysize, Gray75, false);
							yofs_buf += ysize + y_r(5);
						}
					}
				}
				{
					xofs_buf = std::max(xofs_buf, xofs + y_r(300));
				}
				xofs = std::max(xofs, xofs_buf + y_r(10));
				yofs = std::max(yofs, yofs_buf + y_r(10));
			}
			{
				int xofs_buf = xofs + y_r(10);
				int yofs_buf = yofs_OLD;
				xofs_buf += WindowSystem::SetMsg(xp + xofs_buf, yp + yofs_buf, xp + 0, yp + yofs_buf + ysize, LineHeight * 9 / 10, STRX_LEFT, White, Black,
					"%01d:%02d:%02d", (LvData.constructionTime / 60 / 60), (LvData.constructionTime / 60) % 60, LvData.constructionTime % 60);
				xofs_buf += y_r(30);

				xofs = std::max(xofs, xofs_buf + y_r(10));
			}
		}
		{
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
	void			HideoutList::DrawCraftWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs, int* ys, int size) noexcept {
		int xofs = 0;
		if (xs) {
			*xs = xofs;
		}
		int yofsOLD = LineHeight;
		int yofs = yofsOLD;
		yofs += LineHeight;
		if (Lv >= 1) {
			auto& LvData = m_LvData.at(Lv - 1);
			if (LvData.m_ItemCraft.size() > 0) {
				int ysize = y_r(64);
				int Max = (int)(LvData.m_ItemCraft.size());
				int ofset = (window) ? (int)(window->GetNowScrollPer()*std::max(0, Max - 10 + 1)) : 0;
				for (int loop = 0; loop < std::min(size, Max - ofset); loop++) {
					xofs = std::max(xofs, DrawCraft(window, defaultcolor, xp, yp + yofs + (ysize + y_r(5)) * loop, ysize, Lv, loop + ofset, false, true, 0) + y_r(10));
				}
				yofs += (ysize + y_r(5)) * 10;
			}
		}
		yofs = std::max(yofs, yofsOLD + 64 * 2);
		//
		if (xs) {
			*xs = std::max(*xs, xofs);
		}
		if (ys) {
			*ys = std::max(*ys, yofs - LineHeight);
		}
	}

	void			HideoutList::SetOtherPartsID() noexcept {
		for (auto& D : this->m_LvData) {
			int DLv = (int)(&D - &m_LvData.front()) + 1;
			D.m_Child.clear();
			for (const auto&L : DataBase::Instance()->GetHideoutData()->GetList()) {
				for (const auto& C : L.GetLvData()) {
					int Lv = (int)(&C - &L.GetLvData().front()) + 1;
					for (const auto& P : C.m_Parent) {
						auto ID = P.GetID();
						if ((ID == GetID()) && (DLv == P.GetValue())) {
							//自分が子のパーツの親です
							HideoutGetData buf;
							buf.Set(L.GetName(), Lv);
							D.m_Child.emplace_back(buf);
						}
					}
				}
			}
			for (auto& C : D.m_Child) {
				C.CheckID(DataBase::Instance()->GetHideoutData().get());
			}
		}
		//
	}
	//
	const int		HideoutList::DrawCraft(WindowSystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int ysize, int Lv, int Select, bool isdrawName, bool isdrawAfter, int count) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		const auto& cf = this->m_LvData.at(Lv - 1).m_ItemCraft.at(Select);
		int xofsbuf = y_r(10);
		int xofsbuf2 = y_r(10);
		int yofsbuf = 0;
		int xsize2 = isdrawName ? (ysize / 2) : ysize;
		int ysize2 = isdrawName ? (ysize / 2 - y_r(3)) : ysize;
		if (isdrawName) {
			std::string Name = this->GetName() + " Lv" + std::to_string(Lv);
			if (count > 1) {
				Name += " x" + std::to_string(count);
			}
			xofsbuf2 = WindowSystem::SetMsg(xp + xofsbuf, yp + yofsbuf, xp + xofsbuf, yp + yofsbuf + ysize2, ysize2, STRX_LEFT, White, Black, Name);
			yofsbuf += ysize2 + y_r(5);
		}
		for (const auto& w : cf.m_ItemReq) {
			auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
			if (ptr) {
				xofsbuf += ptr->Draw(xp + xofsbuf, yp + yofsbuf, xsize2, ysize2, w.GetValue()*std::max(1, count), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true) + y_r(5);
			}
		}
		if (isdrawAfter) {
			xofsbuf = std::max(xofsbuf, xsize2 * 10);
			xofsbuf += WindowSystem::SetMsg(xp + xofsbuf, yp, xp + xofsbuf, yp + ysize, (ysize / 2 - y_r(3)), STRX_LEFT, White, Black, ">%01d:%02d:%02d>", (cf.durationTime / 60 / 60), (cf.durationTime / 60) % 60, cf.durationTime % 60) + y_r(30);
			for (const auto& w : cf.m_ItemReward) {
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(w.GetID());
				if (ptr) {
					xofsbuf += ptr->Draw(xp + xofsbuf, yp + yofsbuf, xsize2, ysize2, w.GetValue()*std::max(1, count), defaultcolor, !WindowMngr->PosHitCheck(window), false, false, true) + y_r(5);
				}
			}
		}
		xofsbuf = std::max(xofsbuf, xofsbuf2);
		return xofsbuf;
	}
	//
};