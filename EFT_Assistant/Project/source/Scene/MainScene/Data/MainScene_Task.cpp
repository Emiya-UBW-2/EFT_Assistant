#include"../../../Header.hpp"

namespace FPS_n2 {
	void		TaskList::TaskNeedData::SetAfter() noexcept {
		m_Trader.CheckID(DataBase::Instance()->GetTraderData().get());
		for (auto& T : this->m_Item) {
			T.CheckID(DataBase::Instance()->GetItemData().get());
		}
	}
	void		TaskList::TaskNeedData::SetNeedTasktoID() noexcept {
		for (auto& t : this->m_Parenttask) {
			t.CheckID(DataBase::Instance()->GetTaskData().get());
		}
	}
	void		TaskList::TaskWorkData::Set(const std::string& LEFT, const std::vector<std::string>& Args) noexcept {
		if (LEFT == "Task_Map") {//ロケーション追加
			for (auto&a : Args) {
				m_MapArgs.emplace_back(a);
			}
		}
		else if (LEFT == "factionName") {

		}
		else if (LEFT == "Task_Kill") {
			for (auto&a : Args) {
				m_EnemyKillArgs.emplace_back(a);
			}
		}
		else if (LEFT == "Task_FiR_HandOver") {
			for (auto&a : Args) {
				SetGetData<ItemGetData>(&this->m_FiR_Item, a, "x");
			}
		}
		else if (LEFT == "Task_NotFiR_HandOver") {
			for (auto&a : Args) {
				SetGetData<ItemGetData>(&this->m_NotFiR_Item, a, "x");
			}
		}
		else if (LEFT == "Task_WeaponPreset_HandOver") {
			for (auto&a : Args) {
				m_PresetArgs.emplace_back(a);
			}
		}
		else if (LEFT == "Task_Else") {//特殊　メッセージ
			this->m_ElseMsg.emplace_back(Args[0]);
		}
		else if (LEFT == "Task_PinPoint") {//特殊　メッセージ
			this->m_Pin.resize(this->m_Pin.size() + 1);
			this->m_Pin.back().m_Point.Set(std::stof(Args[0]), std::stof(Args[1]), std::stof(Args[2]));
		}
		else if (LEFT == "Task_PinMap") {//特殊　メッセージ
			this->m_Pin.back().m_MapArg = Args[0];
			this->m_Pin.back().m_MapSel = std::atoi(Args[1].c_str());
		}
	}
	void		TaskList::TaskWorkData::SetAfter() noexcept {
		auto SetKill = [&](const std::string& mes) {
			auto L = mes.rfind("x");
			if (L != std::string::npos) {
				EnemyKill tmp;
				auto MP = mes.rfind("-");
				if (MP != std::string::npos) {
					std::string Name = mes.substr(MP + 1, L - (MP + 1));
					auto kakko = Name.find("{");
					if (kakko != std::string::npos) {
						Name = Name.substr(0, kakko);
					}

					tmp.Set(
						DataBase::Instance()->GetMapData()->FindID(mes.substr(0, MP).c_str()),
						DataBase::Instance()->GetEnemyData()->FindID(Name.c_str()),
						std::stoi(mes.substr(L + 1)));
				}
				else {
					std::string Name = mes.substr(0, L);
					auto kakko = Name.find("{");
					if (kakko != std::string::npos) {
						Name = Name.substr(0, kakko);
					}

					tmp.Set(
						InvalidID,
						DataBase::Instance()->GetEnemyData()->FindID(Name.c_str()),
						std::stoi(mes.substr(L + 1)));
				}
				this->m_Kill.emplace_back(tmp);
			}
			else {
				//int a = 0;
			}
		};
		for (auto& a : this->m_EnemyKillArgs) {
			SetKill(a);
		}

		for (auto& T : this->m_FiR_Item) {
			T.CheckID(DataBase::Instance()->GetItemData().get());
		}
		for (auto& T : this->m_NotFiR_Item) {
			T.CheckID(DataBase::Instance()->GetItemData().get());
		}


		for (auto& a : this->m_PresetArgs) {
			m_PresetID.emplace_back(DataBase::Instance()->GetPresetData()->FindID(a));
		}
		for (auto& a : this->m_Pin) {
			a.m_MapID = DataBase::Instance()->GetMapData()->FindID(a.m_MapArg);
		}
		for (auto& a : this->m_MapArgs) {
			this->m_Map.emplace_back(DataBase::Instance()->GetMapData()->FindID(a));
		}
	}
	void		TaskList::TaskRewardData::SetAfter() noexcept {
		for (auto& L : this->m_LLAdd) {
			L.CheckID(DataBase::Instance()->GetTraderData().get());
		}
		for (auto& T : this->m_Item) {
			T.CheckID(DataBase::Instance()->GetItemData().get());
		}
	}
	//
	const int	TaskList::Draw(int xp, int yp, int xsize, int ysize, int count, bool Clickactive) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* Input = InputControl::Instance();

		bool IsClearTask = PlayerData::Instance()->GetTaskClear(this->GetName().c_str());
		auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(this->GetTrader());
		auto color = ptr->GetColors(0);
		//
		if (IsClearTask) {
			color = ptr->GetColors(-150);
		}
		std::string Name = this->GetName();
		if (count > 0) {
			Name += " x" + std::to_string(count);
		}
		if (WindowSystem::ClickCheckBox(xp, yp, xp + xsize, yp + ysize, false, Clickactive, color, Name)) {
			if (Input->GetSpaceKey().press()) {
				PlayerData::Instance()->OnOffTaskClear(this->GetName().c_str());
			}
			else {
				auto sizeXBuf = y_r(800);
				auto sizeYBuf = y_r(0);
				this->DrawWindow(nullptr, y_r(1920), y_r(1080), &sizeXBuf, &sizeYBuf);//試しにサイズ計測
				signed long long FreeID = this->GetID();
				WindowMngr->Add()->Set(xp + xsize / 2 - sizeXBuf / 2, yp, sizeXBuf, sizeYBuf, 0, this->GetName().c_str(), false, true, FreeID, [&](WindowSystem::WindowControl* win) {
					DataBase::Instance()->GetTaskData()->FindPtr((TaskID)win->m_FreeID)->DrawWindow(win, win->GetPosX(), win->GetPosY());
				});
			}
		}
		if (IsClearTask) {
			DrawControl::Instance()->SetDrawRotaFiR(DrawLayer::Normal, xp + xsize / 2, yp + ysize / 2, (float)(yp) / (float)(LineHeight), 0.f, true);
		}


		return xsize;
	}
	void		TaskList::DrawWindow(WindowSystem::WindowControl* window, int xp, int yp, int *xs, int* ys) const noexcept {
		auto* WindowMngr = WindowSystem::WindowManager::Instance();
		auto* InterParts = InterruptParts::Instance();
		int xofs = 0;
		int yofs = LineHeight;
		int sizy = LineHeight * 7 / 10;
		//必要
		{
			auto* ptr = DataBase::Instance()->GetTraderData()->FindPtr(GetTrader());
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "トレーダー:%s Lv %d", ptr->GetName().c_str(), std::max(m_TaskNeedData.GetLL(), 1))); yofs += sizy;
		}
		if (m_TaskNeedData.GetLevel() >= 1) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "必要レベル:%d", this->m_TaskNeedData.GetLevel())); yofs += sizy;
		}
		if (m_TaskNeedData.GetItem().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "必要アイテム:")); yofs += sizy;
			yofs += LineHeight;
			for (const auto& LL : this->m_TaskNeedData.GetItem()) {
				auto ID = LL.GetID();
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
				int total_size = y_r(48);
				xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), false, true, false) + y_r(30));
				yofs += total_size;
			}
		}
		//タスク内容
		if (m_TaskWorkData.GetMap().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "マップ指定:")); yofs += sizy;
			for (auto& LL : this->m_TaskWorkData.GetMap()) {
				auto* ptr = DataBase::Instance()->GetMapData()->FindPtr(LL);
				xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STRX_LEFT, ptr->GetColors(0), Black, "%s", ptr->GetName().c_str()) + y_r(30)); yofs += sizy;
			}
		}
		if (m_TaskWorkData.GetKill().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "敵をキル:")); yofs += sizy;
			for (auto& LL : this->m_TaskWorkData.GetKill()) {
				auto* eny = DataBase::Instance()->GetEnemyData()->FindPtr(LL.GetEnemyID());
				xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STRX_LEFT, eny->GetColors(0), Black, "%s x%2d", eny->GetName().c_str(), LL.GetKillCount()) + y_r(30));
				if (LL.GetMapID() != InvalidID) {
					auto* ptr = DataBase::Instance()->GetMapData()->FindPtr(LL.GetMapID());
					xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(250), yp + yofs, xp + y_r(250), yp + sizy + yofs, LineHeight * 8 / 10, STRX_LEFT, ptr->GetColors(0), Black, " in %s", ptr->GetName().c_str()));
				}
				yofs += sizy;
			}
		}
		if (m_TaskWorkData.GetFiR_Item().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "Firアイテムの納品:")); yofs += sizy;
			yofs += LineHeight;
			for (const auto& LL : this->m_TaskWorkData.GetFiR_Item()) {
				auto ID = LL.GetID();
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
				int total_size = y_r(48);
				xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), true, true, false) + y_r(30));
				yofs += total_size;
			}
		}
		if (m_TaskWorkData.GetNotFiR_Item().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "Firでなくてよいアイテムの納品:")); yofs += sizy;
			yofs += LineHeight;
			for (const auto& LL : this->m_TaskWorkData.GetNotFiR_Item()) {
				auto ID = LL.GetID();
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
				int total_size = y_r(48);
				xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), false, true, false) + y_r(30));
				yofs += total_size;
			}
		}

		if (m_TaskWorkData.GetWeaponPreset().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "カスタム品の納品:")); yofs += sizy;
			yofs += LineHeight;
			//
			for (const auto& LL : this->m_TaskWorkData.GetWeaponPreset()) {
				auto* ptr = DataBase::Instance()->GetPresetData()->FindPtr(LL);
				int total_size = LineHeight;
				int XSize = WindowSystem::GetMsgLen(total_size, ptr->GetName());

				if (WindowSystem::ClickCheckBox(xp + y_r(30), yp + yofs, xp + y_r(30) + XSize, yp + yofs + total_size, false, true, Gray10, ptr->GetName())) {
					InterParts->GotoNext(BGSelect::Custom);
					InterParts->SetInitParam(0, (int)ptr->GetBase()->GetID());//武器ID
					InterParts->SetInitParam(1, (int)ptr->GetID());//プリセットID
				}
				xofs = std::max(xofs, XSize + y_r(30));
				yofs += total_size;
			}
		}
		if (m_TaskWorkData.GetElseMsg().size() > 0) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "メモ:")); yofs += sizy;
			for (auto& m : this->m_TaskWorkData.GetElseMsg()) {
				xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, m.c_str())); yofs += sizy;
			}
		}
		//
		if ((m_TaskRewardData.GetLLAdd().size() > 0) || (m_TaskRewardData.GetItem().size() > 0)) {
			xofs = std::max(xofs, WindowSystem::SetMsg(xp, yp + yofs, xp, yp + sizy + yofs, sizy, STRX_LEFT, White, Black, "報酬アイテム:")); yofs += sizy;
			yofs += LineHeight;
		}
		if (m_TaskRewardData.GetLLAdd().size() > 0) {
			for (auto& LL : this->m_TaskRewardData.GetLLAdd()) {
				auto* trader2 = DataBase::Instance()->GetTraderData()->FindPtr(LL.GetID());
				xofs = std::max(xofs, WindowSystem::SetMsg(xp + y_r(30), yp + yofs, xp + y_r(30), yp + sizy + yofs, sizy, STRX_LEFT, trader2->GetColors(0), Black, "%s %s%4.2f",
					trader2->GetName().c_str(),
					(LL.GetValue() >= 0.f) ? "+" : "-",
					(float)(LL.GetValue()) / 100.f)); yofs += sizy;
			}
		}
		if (m_TaskRewardData.GetItem().size() > 0) {
			for (const auto& LL : this->m_TaskRewardData.GetItem()) {
				auto ID = LL.GetID();
				auto* ptr = DataBase::Instance()->GetItemData()->FindPtr(ID);
				int total_size = y_r(48);
				xofs = std::max(xofs, ptr->Draw(xp + y_r(30), yp + yofs, 0, total_size, LL.GetValue(), Gray10, !WindowMngr->PosHitCheck(window), true, true, false) + y_r(30));
				yofs += total_size;
			}
			//yofs += sizy;
		}
		//
		if (xs) {
			*xs = std::max(*xs, xofs + LineHeight / 10);
		}
		if (ys) {
			*ys = std::max(*ys, yofs + LineHeight / 10);
		}
	}
	//
	void		TaskJsonData::TaskObjective::GetJsonData(const nlohmann::json& data) {
		if (data.contains("__typename") && !data["__typename"].is_null()) {
			std::string buf = data["__typename"];
			for (int i = 0; i < sizeof(TypesStr) / sizeof(TypesStr[0]); i++) {
				if (buf == TypesStr[i]) {
					m_TaskObjectiveType = (EnumTaskObjective)i;
				}

			}
		}
		if (data.contains("type") && !data["type"].is_null()) {
			m_type = data["type"];
		}
		if (data.contains("description") && !data["description"].is_null()) {
			m_description = data["description"];
		}
		if (data.contains("maps") && !data["maps"].is_null()) {
			for (const auto&m : data["maps"]) {
				std::string buf = m["name"];
				m_Maps.emplace_back(DataBase::Instance()->GetMapData()->FindID(buf.c_str()));
			}
		}
		if (data.contains("optional") && !data["optional"].is_null()) {
			m_optional = data["optional"];
		}
		if (data.contains("item") && !data["item"].is_null()) {
			m_Items.Set(data["item"]["name"], (data.contains("count") && !data["count"].is_null()) ? (int)data["count"] : 1);
			m_Items.CheckID(DataBase::Instance()->GetItemData().get());
		}
		if (data.contains("containsAll") && !data["containsAll"].is_null()) {
			for (const auto&m : data["containsAll"]) {
				std::string buf1 = m["name"];
				m_containsAll.emplace_back(buf1);
			}
		}
		if (data.contains("containsCategory") && !data["containsCategory"].is_null()) {
			for (const auto&m : data["containsCategory"]) {
				std::string buf1 = m["name"];
				m_containsCategory.emplace_back(buf1);
			}
		}
		if (data.contains("attributes") && !data["attributes"].is_null()) {
			for (const auto&m : data["attributes"]) {
				std::string buf1 = m["name"];
				Compare buf2; buf2.GetJsonData(m["requirement"]);
				m_attributes.emplace_back(std::make_pair(buf1, buf2));
			}
		}
		if (data.contains("healthEffect")) {
			if (!data["healthEffect"].is_null()) {
				for (const auto&m : data["healthEffect"]) {
					HealthEffect buf1; buf1.GetJsonData(m);
					m_healthEffect.emplace_back(buf1);
				}
			}
		}
		if (data.contains("exitStatus")) {
			if (!data["exitStatus"].is_null()) {
				for (const auto&m : data["exitStatus"]) {
					m_exitStatus.emplace_back(m);
				}
			}
		}
		if (data.contains("exitName")) {
			if (!data["exitName"].is_null()) {
				m_exitName = data["exitName"];
			}
		}
		if (data.contains("zoneNames")) {
			if (!data["zoneNames"].is_null()) {
				for (const auto&m : data["zoneNames"]) {
					std::string buf1 = m;
					m_zoneNames.emplace_back(buf1);
				}
			}
		}
		if (data.contains("count") && !data["count"].is_null()) {
			count = data["count"];
		}
		if (data.contains("foundInRaid")) {
			if (!data["foundInRaid"].is_null()) {
				foundInRaid = data["foundInRaid"];
			}
		}
		if (data.contains("dogTagLevel")) {
			if (!data["dogTagLevel"].is_null()) {
				dogTagLevel = data["dogTagLevel"];
			}
		}
		if (data.contains("maxDurability")) {
			if (!data["maxDurability"].is_null()) {
				maxDurability = data["maxDurability"];
			}
		}
		if (data.contains("minDurability")) {
			if (!data["minDurability"].is_null()) {
				minDurability = data["minDurability"];
			}
		}

		if (data.contains("markerItem")) {
			if (!data["markerItem"].is_null()) {
				markerItem = data["markerItem"]["name"];
			}
		}

		if (data.contains("questItem")) {
			if (!data["questItem"].is_null()) {
				m_QuestItem.Set(data["questItem"]["name"], (data.contains("count") && !data["count"].is_null()) ? (int)data["count"] : 1);
				m_QuestItem.CheckID(DataBase::Instance()->GetItemData().get());
			}
		}

		if (data.contains("target")) {
			if (!data["target"].is_null()) {
				std::string buf = data["target"];
				target = DataBase::Instance()->GetEnemyData()->FindID(buf.c_str());
			}
		}
		if (data.contains("shotType")) {
			if (!data["shotType"].is_null()) {
				shotType = data["shotType"];
			}
		}
		if (data.contains("bodyParts")) {
			if (!data["bodyParts"].is_null()) {
				for (const auto&m : data["bodyParts"]) {
					bodyParts.emplace_back(m);
				}
			}
		}
		if (data.contains("usingWeapon")) {
			if (!data["usingWeapon"].is_null()) {
				for (const auto&m : data["usingWeapon"]) {
					std::string buf1 = m["name"];
					usingWeapon.emplace_back(buf1);
				}
			}
		}
		if (data.contains("usingWeaponMods")) {
			if (!data["usingWeaponMods"].is_null()) {
				for (const auto&m : data["usingWeaponMods"]) {
					for (const auto&m2 : m) {
						std::string buf1 = m2["name"];
						usingWeaponMods.emplace_back(buf1);
					}
				}
			}
		}
		if (data.contains("wearing")) {
			if (!data["wearing"].is_null()) {
				for (const auto&m : data["wearing"]) {
					for (const auto&m2 : m) {
						std::string buf1 = m2["name"];
						wearing.emplace_back(buf1);
					}
				}
			}
		}
		if (data.contains("notWearing")) {
			if (!data["notWearing"].is_null()) {
				for (const auto&m : data["notWearing"]) {
					std::string buf1 = m["name"];
					notWearing.emplace_back(buf1);
				}
			}
		}

		if (data.contains("distance")) {
			if (!data["distance"].is_null()) {
				distance.GetJsonData(data["distance"]);
			}
		}


		if (data.contains("playerHealthEffect")) {
			if (!data["playerHealthEffect"].is_null()) {
				playerHealthEffect.GetJsonData(data["playerHealthEffect"]);
			}
		}
		if (data.contains("enemyHealthEffect")) {
			if (!data["enemyHealthEffect"].is_null()) {
				enemyHealthEffect.GetJsonData(data["enemyHealthEffect"]);
			}
		}

		if (data.contains("skillLevel")) {
			if (!data["skillLevel"].is_null()) {
				std::string buf1 = data["skillLevel"]["name"];
				float buf2 = data["skillLevel"]["level"];
				skillLevel.emplace_back(std::make_pair(buf1, buf2));
			}
		}

		if (data.contains("task")) {
			if (!data["task"].is_null()) {
				task = data["task"]["name"];
			}
		}
		if (data.contains("status")) {
			if (!data["status"].is_null()) {
				for (const auto&m : data["status"]) {
					std::string buf1 = m;
					status.emplace_back(buf1);
				}
			}
		}

		if (data.contains("trader")) {
			if (!data["trader"].is_null()) {
				trader = data["trader"]["name"];
			}
		}
		if (data.contains("level")) {
			if (!data["level"].is_null()) {
				level = data["level"];
			}
		}
		Compares.GetJsonData(data);
	}
	void		TaskJsonData::TaskRewards::GetJsonData(const nlohmann::json& data) {
		if (data.contains("traderStanding")) {
			if (!data["traderStanding"].is_null()) {
				for (const auto&m : data["traderStanding"]) {
					TraderGetData buf;
					buf.Set(m["trader"]["name"], (int)((float)m["standing"] * 100.f));
					m_traderStanding.emplace_back(buf);

					m_traderStanding.back().CheckID(DataBase::Instance()->GetTraderData().get());
				}
			}
		}
		if (data.contains("items")) {
			if (!data["items"].is_null()) {
				for (const auto&m : data["items"]) {
					ItemGetData buf;
					buf.Set(m["item"]["name"], m["count"]);
					m_Items.emplace_back(buf);

					m_Items.back().CheckID(DataBase::Instance()->GetItemData().get());
				}
			}
		}
		if (data.contains("skillLevelReward")) {
			if (!data["skillLevelReward"].is_null()) {
				for (const auto&m : data["skillLevelReward"]) {
					skillLevelReward buf1; buf1.GetJsonData(m);
					m_skillLevelReward.emplace_back(buf1);
				}
			}
		}
		if (data.contains("traderUnlock") && !data["traderUnlock"].is_null()) {
			for (const auto&m : data["traderUnlock"]) {
				std::string buf1 = m["name"];
				m_traderUnlock.emplace_back(buf1);
			}
		}
	}
	void		TaskJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		{
			traderID = DataBase::Instance()->GetTraderData()->FindID(data["trader"]["name"]);
		}
		if (data.contains("map") && !data["map"].is_null()) {
			m_MapID = DataBase::Instance()->GetMapData()->FindID(data["map"]["name"]);
		}
		if (data.contains("factionName") && !data["factionName"].is_null()) {
			m_factionName = data["factionName"];
		}
		if (data.contains("experience")) {
			if (!data["experience"].is_null()) {
				experience = data["experience"];
			}
		}
		if (data.contains("minPlayerLevel")) {
			if (!data["minPlayerLevel"].is_null()) {
				minPlayerLevel = data["minPlayerLevel"];
			}
		}
		if (data.contains("taskRequirements")) {
			for (const auto&m : data["taskRequirements"]) {
				std::string buf1 = m["task"]["name"];
				std::string buf2 = m["status"][0];
				taskRequirements.emplace_back(std::make_pair(buf1, buf2));
			}
		}
		if (data.contains("traderRequirements")) {
			for (const auto&m : data["traderRequirements"]) {
				TraderRequirements TMP;
				TMP.m_name = m["trader"]["name"];
				TMP.m_requirementType = m["requirementType"];
				TMP.m_compareMethod = m["compareMethod"];
				TMP.m_Value = m["value"];
				traderRequirements.emplace_back(TMP);
			}
		}
		if (data.contains("objectives")) {
			if (!data["objectives"].is_null()) {
				for (const auto&m : data["objectives"]) {
					TaskObjective buf1; buf1.GetJsonData(m);
					objectives.emplace_back(buf1);
				}
			}
		}
		startRewards.GetJsonData(data["startRewards"]);
		finishRewards.GetJsonData(data["finishRewards"]);
		if (data.contains("failConditions")) {
			if (!data["failConditions"].is_null()) {
				for (const auto&m : data["failConditions"]) {
					TaskObjective buf1; buf1.GetJsonData(m);
					failConditions.emplace_back(buf1);
				}
			}
		}
		failureOutcome.GetJsonData(data["failureOutcome"]);
		restartable = data["restartable"];
		if (data.contains("neededKeys")) {
			if (!data["neededKeys"].is_null()) {
				for (const auto&m : data["neededKeys"]) {
					for (const auto&k : m["keys"]) {
						neededKeys.emplace_back(k["name"]);
					}
				}
			}
		}
		kappaRequired = data["kappaRequired"];;
		lightkeeperRequired = data["lightkeeperRequired"];
	}
	void		TaskJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		auto SetTaskObjectiveCommon = [&](std::ofstream& outputfile, const TaskJsonData::TaskObjective& obj) {
			outputfile << "[\n";
			outputfile << "\tTaskType=" + obj.m_type + "\n";
			outputfile << "\tTaskText=" + obj.m_description + "\n";
			outputfile << "\tTaskOptional=" + (std::string)(obj.m_optional ? "true" : "false") + "\n";
			outputfile << "]\n";
		};
		auto SetTaskObjective = [&](std::ofstream& outputfile, const TaskJsonData::TaskObjective& obj, EnumTaskObjective /*prev*/) {
			for (auto& m : obj.m_Maps) {
				outputfile << "Task_Map=" + DataBase::Instance()->GetMapData()->FindPtr(m)->GetName() + "\n";
			}

			switch ((EnumTaskObjective)obj.m_TaskObjectiveType) {
			case FPS_n2::EnumTaskObjective::TaskObjectiveBasic:
			{
				outputfile << "Task_Else=TaskType:" + obj.m_type + "\n";
				outputfile << "Task_Else=" + obj.m_description + "\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveBuildItem:
			{
				if (obj.m_Items.GetID() != InvalidID) {
					outputfile << "NeedItem=" + obj.m_Items.GetOutputStr() + "\n";
				}
				for (auto& m : obj.m_containsAll) {
					outputfile << "ContainsAll=" + m + "\n";
				}
				for (auto& m : obj.m_containsCategory) {
					outputfile << "ContainsCategory=" + m + "\n";
				}
				for (auto& m : obj.m_attributes) {
					if (m.second.IsActive()) {
						outputfile << "Task_Else=" + m.first + " " + (std::string)(CompareMethodStr[(int)m.second.compareMethod]) + " " + std::to_string(m.second.value) + "\n";
					}
					else {
						outputfile << "Task_Else=" + m.first + "\n";
					}
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveExperience:
			{
				for (auto& m : obj.m_healthEffect) {
					if (m.bodyParts.size() > 0) {
						outputfile << "Task_Else=自分の状態異常箇所:[";
						for (auto& m2 : m.bodyParts) {
							outputfile << m2;
							if (&m2 != &m.bodyParts.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (m.effects.size() > 0) {
						outputfile << "Task_Else=自分の状態異常内容:[";
						for (auto& m2 : m.effects) {
							outputfile << m2;
							if (&m2 != &m.effects.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (m.m_time.IsActive()) {
						outputfile << "Task_Else=耐久時間: " + (std::string)(CompareMethodStr[(int)m.m_time.compareMethod]) + " " + std::to_string(m.m_time.value) + "\n";
					}
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveExtract:
			{
				if (obj.m_exitStatus.size() > 0) {
					outputfile << "Task_Else=脱出ステータス:[";
					for (auto& m : obj.m_exitStatus) {
						outputfile << m;
						if (&m != &obj.m_exitStatus.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
				if (obj.m_exitName != "") {
					outputfile << "Task_Else=脱出地点:" + obj.m_exitName + "\n";
				}
				if (obj.m_zoneNames.size() > 0) {
					outputfile << "Task_Else=脱出ゾーン:[";
					for (auto& m : obj.m_zoneNames) {
						outputfile << m;
						if (&m != &obj.m_zoneNames.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveItem:
			{
				if (obj.m_Items.GetID() != InvalidID) {
					if (obj.m_type == "findItem") {
						outputfile << "NeedItem=" + obj.m_Items.GetOutputStr() + "\n";
					}
					else {
						if (obj.foundInRaid) {
							outputfile << "Task_FiR_HandOver=[" + obj.m_Items.GetOutputStr() + "]\n";
						}
						else {
							outputfile << "Task_NotFiR_HandOver=[" + obj.m_Items.GetOutputStr() + "]\n";
						}
						outputfile << "DogTagLv=" + std::to_string(obj.dogTagLevel) + "\n";
						//outputfile << "Max=" + std::to_string(obj.maxDurability) + "\n";
						//outputfile << "Min=" + std::to_string(obj.minDurability) + "\n";
					}
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveMark:
			{
				outputfile << "NeedItem=" + obj.markerItem + "x1\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectivePlayerLevel:
			{
				outputfile << "Task_Else=レベル:" + std::to_string(obj.playerLevel) + "\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveQuestItem:
				for (auto& m : obj.m_Maps) {
					outputfile << "Task_Else=拾って納品:" + DataBase::Instance()->GetMapData()->FindPtr(m)->GetName() + "-" + obj.m_QuestItem.GetOutputStr() + "\n";
				}
				break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveShoot:
			{
				outputfile << "Task_Kill=[\n";
				if (obj.m_Maps.size() == 0) {
					outputfile << "\t" + DataBase::Instance()->GetEnemyData()->FindPtr(obj.target)->GetName();
					if (obj.bodyParts.size() > 0) {
						outputfile << "{";
						for (auto& m2 : obj.bodyParts) {
							outputfile << m2;
							if (&m2 != &obj.bodyParts.back()) {
								outputfile << "/";
							}
						}
						outputfile << "}";
					}
					outputfile << "x" + std::to_string(obj.count);
					outputfile << "\n";
				}
				else {
					for (auto& m : obj.m_Maps) {
						outputfile << "\t" + DataBase::Instance()->GetMapData()->FindPtr(m)->GetName() + "-" + DataBase::Instance()->GetEnemyData()->FindPtr(obj.target)->GetName();
						if (obj.bodyParts.size() > 0) {
							outputfile << "{";
							for (auto& m2 : obj.bodyParts) {
								outputfile << m2;
								if (&m2 != &obj.bodyParts.back()) {
									outputfile << "/";
								}
							}
							outputfile << "}";
						}
						outputfile << "x" + std::to_string(obj.count);
						if (&m != &obj.m_Maps.back()) {
							outputfile << DIV_STR;
						}
						outputfile << "\n";
					}
				}
				outputfile << "]\n";

				//outputfile << "shotType=" + obj.shotType + "\n";
				if (obj.m_zoneNames.size() > 0) {
					outputfile << "Task_Else=ゾーン:[";
					for (auto& m : obj.m_zoneNames) {
						outputfile << m;
						if (&m != &obj.m_zoneNames.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
				if (obj.usingWeapon.size() > 0) {
					outputfile << "NeedItem=[\n";
					for (auto& m : obj.usingWeapon) {
						outputfile << "\t" + m + "x0";
						if (&m != &obj.usingWeapon.back()) {
							outputfile << DIV_STR;
						}
						outputfile << "\n";
					}
					outputfile << "]\n";
				}
				if (obj.usingWeaponMods.size() > 0) {
					outputfile << "NeedItem=[\n";
					for (auto& m : obj.usingWeaponMods) {
						outputfile << "\t" + m + "x0";
						if (&m != &obj.usingWeaponMods.back()) {
							outputfile << DIV_STR;
						}
						outputfile << "\n";
					}
					outputfile << "]\n";
				}
				if (obj.wearing.size() > 0) {
					outputfile << "Task_Else=装備が必要:{\n";
					for (auto& m : obj.wearing) {
						outputfile << "Task_Else=  " + m + "\n";
					}
					outputfile << "Task_Else=}\n";
				}
				if (obj.notWearing.size() > 0) {
					outputfile << "Task_Else=装備してはいけない:{\n";
					for (auto& m : obj.notWearing) {
						outputfile << "Task_Else=  " + m + "\n";
					}
					outputfile << "Task_Else=}\n";
				}
				if (obj.distance.IsActive()) {
					outputfile << "Task_Else=距離: " + (std::string)(CompareMethodStr[(int)obj.distance.compareMethod]) + " " + std::to_string(obj.distance.value) + "m\n";
				}
				{
					if (obj.playerHealthEffect.bodyParts.size() > 0) {
						outputfile << "Task_Else=自分の該当箇所に異常状態:[";
						for (auto& m2 : obj.playerHealthEffect.bodyParts) {
							outputfile << m2;
							if (&m2 != &obj.playerHealthEffect.bodyParts.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.playerHealthEffect.effects.size() > 0) {
						outputfile << "Task_Else=自分の異常状態:[";
						for (auto& m2 : obj.playerHealthEffect.effects) {
							outputfile << m2;
							if (&m2 != &obj.playerHealthEffect.effects.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.playerHealthEffect.m_time.IsActive()) {
						outputfile << "Task_Else=閾値: " + (std::string)(CompareMethodStr[(int)obj.playerHealthEffect.m_time.compareMethod]) + " " + std::to_string(obj.playerHealthEffect.m_time.value) + "\n";
					}
				}
				{
					if (obj.enemyHealthEffect.bodyParts.size() > 0) {
						outputfile << "Task_Else=敵の該当箇所に異常状態:[";
						for (auto& m2 : obj.enemyHealthEffect.bodyParts) {
							outputfile << m2;
							if (&m2 != &obj.enemyHealthEffect.bodyParts.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.enemyHealthEffect.effects.size() > 0) {
						outputfile << "Task_Else=敵の異常状態:[";
						for (auto& m2 : obj.enemyHealthEffect.effects) {
							outputfile << m2;
							if (&m2 != &obj.enemyHealthEffect.effects.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.enemyHealthEffect.m_time.IsActive()) {
						outputfile << "Task_Else=閾値: " + (std::string)(CompareMethodStr[(int)obj.enemyHealthEffect.m_time.compareMethod]) + " " + std::to_string(obj.enemyHealthEffect.m_time.value) + "\n";
					}
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveSkill:
				for (auto& m : obj.skillLevel) {
					outputfile << "Task_Else=Skillを上げる:" + m.first + "+" + std::to_string(m.second) + "\n";
				}
				break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveTaskStatus:
			{
				outputfile << "Task_Else=該当タスク" + obj.task + "\n";
				if (obj.status.size() > 0) {
					outputfile << "Task_Else=該当タスク状態:";
					for (auto& m : obj.status) {
						outputfile << m;
						if (&m != &obj.status.back()) {
							outputfile << ",";
						}
					}
					outputfile << "\n";
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveTraderLevel:
			{
				outputfile << "Task_Else=traderLL:" + obj.trader + "LL" + std::to_string(obj.level) + "\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveTraderStanding:
				if (obj.Compares.IsActive()) {
					outputfile << "Task_Else=TraderStandingの閾値: " + (std::string)(CompareMethodStr[(int)obj.Compares.compareMethod]) + " " + std::to_string(obj.Compares.value) + "\n";
				}
				break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveUseItem:
			{
				if (obj.Compares.IsActive()) {
					outputfile << "Task_Else=閾値: " + (std::string)(CompareMethodStr[(int)obj.Compares.compareMethod]) + " " + std::to_string(obj.Compares.value) + "\n";
				}
				if (obj.m_zoneNames.size() > 0) {
					outputfile << "Task_Else=ゾーン:[";
					for (auto& m : obj.m_zoneNames) {
						outputfile << m;
						if (&m != &obj.m_zoneNames.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
			}
			break;
			default:
				break;
			}
		};
		auto SetTaskRewards = [&](std::ofstream& outputfile, const TaskJsonData::TaskRewards& obj) {
			for (auto& m : obj.m_traderStanding) {
				outputfile << "Reward_Rep=" + m.GetName() + ((m.GetValue() >= 0) ? "+" : "") + std::to_string(m.GetValue()) + "\n";
			}
			if (obj.m_Items.size() > 0) {
				outputfile << "Reward_Item=[\n";
				for (auto& m : obj.m_Items) {
					outputfile << "\t" + m.GetOutputStr();
					if (&m != &obj.m_Items.back()) {
						outputfile << DIV_STR;
					}
					outputfile << "\n";
				}
				outputfile << "]\n";
			}
			for (auto& m : obj.m_skillLevelReward) {
				outputfile << "Reward_SkillLevelUp=" + m.name + ((m.level >= 0) ? "+" : "") + std::to_string(m.level) + "\n";
			}
			for (auto& m : obj.m_traderUnlock) {
				outputfile << "Reward_TraderUnlock=" + m + "\n";
			}
		};
		auto WriteText = [&](std::ofstream& outputfile) {
			outputfile << "Trader=" + DataBase::Instance()->GetTraderData()->FindPtr(this->traderID)->GetName() + "\n";
			outputfile << "\n";
			//
			for (auto& tr : this->taskRequirements) {
				outputfile << "NeedTask=" + tr.first + "\n";
			}
			outputfile << "NeedLevel=" + std::to_string(this->minPlayerLevel) + "\n";
			if (this->neededKeys.size() > 0) {
				outputfile << "NeedItem=[";
				for (auto& n : this->neededKeys) {
					outputfile << n + "x1";
					if (&n != &this->neededKeys.back()) {
						outputfile << DIV_STR;
					}
				}
				outputfile << "]\n";
			}
			outputfile << "\n";
			//
			outputfile << "CanRestart=" + (std::string)(this->restartable ? "true" : "false") + "\n";
			outputfile << "NeedKappa=" + (std::string)(this->kappaRequired ? "true" : "false") + "\n";
			outputfile << "NeedLightkeeper=" + (std::string)(this->lightkeeperRequired ? "true" : "false") + "\n";
			outputfile << "\n";
			//
			if (this->m_MapID != InvalidID) {
				outputfile << "Task_Map=" + DataBase::Instance()->GetMapData()->FindPtr(this->m_MapID)->GetName() + "\n";
			}
			outputfile << "factionName=" + m_factionName + "\n";
			for (auto& tr : this->traderRequirements) {
				outputfile << "traderRequirementsName=" + tr.m_name + "\n";
				outputfile << "traderRequirementsType=" + tr.m_requirementType + "\n";
				outputfile << "traderRequirements=" + tr.m_compareMethod + "\n";
				outputfile << "traderRequirementsValue=" + std::to_string(tr.m_Value) + "\n";
			}
			if (this->objectives.size() > 0) {
				EnumTaskObjective Prev = EnumTaskObjective::Max;
				for (auto& ob : this->objectives) {
					SetTaskObjective(outputfile, ob, Prev);
					Prev = ob.m_TaskObjectiveType;
				}
			}
			if (this->objectives.size() > 0) {
				outputfile << "\n";
				for (auto& ob : this->objectives) {
					SetTaskObjectiveCommon(outputfile, ob);
				}
			}
			outputfile << "\n";
			SetTaskRewards(outputfile, this->startRewards);
			SetTaskRewards(outputfile, this->finishRewards);
			if (this->failConditions.size() > 0) {
				outputfile << "\n";
				EnumTaskObjective Prev = EnumTaskObjective::Max;
				for (auto& ob : this->failConditions) {
					SetTaskObjective(outputfile, ob, Prev);
					Prev = ob.m_TaskObjectiveType;
				}
			}
			if (this->failConditions.size() > 0) {
				outputfile << "\n";
				for (auto& ob : this->failConditions) {
					SetTaskObjectiveCommon(outputfile, ob);
				}
			}
			outputfile << "\n";
			SetTaskRewards(outputfile, this->failureOutcome);
			outputfile << "Reward_EXP=" + std::to_string(this->experience) + "\n";
		};

		WriteText(outputfile);
	}
	//
	void		TaskData::AddTaskUseID() noexcept {
		for (auto& i : DataBase::Instance()->GetItemData()->SetList()) {
			i.ResetTaskUseID();
			for (const auto& t : this->m_List) {
				for (const auto& n : t.GetTaskNeedData().GetItem()) {
					if (i.GetID() == n.GetID()) {
						i.AddTaskUseID(t.GetID());
					}
				}
				for (const auto& w : t.GetTaskWorkData().GetFiR_Item()) {
					if (i.GetID() == w.GetID()) {
						i.AddTaskUseID(t.GetID());
					}
				}
				for (const auto& w : t.GetTaskWorkData().GetNotFiR_Item()) {
					if (i.GetID() == w.GetID()) {
						i.AddTaskUseID(t.GetID());
					}
				}
			}
		}
	}
	//
	void		TaskData::InitDatabyJson() noexcept {
		TraderIDs.resize(DataBase::Instance()->GetTraderData()->GetList().size());
		for (auto&i : TraderIDs) { i = 0; }
	}
	void		TaskData::SaveAsNewData2(std::string Path) noexcept {
		bool maked = false;
		std::vector<bool> maked_t;
		maked_t.resize(DataBase::Instance()->GetTraderData()->GetList().size());
		for (auto&&i : maked_t) { i = false; }

		for (auto& jd : GetJsonDataList()) {
			TraderID trID = (dynamic_cast<TaskJsonData*>(jd.get()))->traderID;
			if (!jd->m_IsFileOpened && (trID != InvalidID)) {
				std::string ParentPath = Path;
				if (!maked) {
					CreateDirectory(ParentPath.c_str(), NULL);
					maked = true;
				}
				std::string ChildPath = ParentPath;
				{
					char tID[64];
					sprintfDx(tID, "%02d", trID);
					ChildPath += tID;
					ChildPath += "_" + DataBase::Instance()->GetTraderData()->FindPtr(trID)->GetName() + "/";
					if (!maked_t[trID]) {
						CreateDirectory(ChildPath.c_str(), NULL);
						maked_t[trID] = true;
					}
				}
				std::string FileName;
				{
					char tID[64];
					sprintfDx(tID, "%02d%03d", trID, TraderIDs[trID]);
					TraderIDs[trID]++;
					FileName = tID;
					FileName += "_" + jd->m_name + ".txt";

				}
				SubStrs(&FileName, ".");
				std::string Name = FileName + ".txt";
				jd->OutputData(ChildPath + Name);
			}
		}
	}
	void		TaskData::UpdateAfterbyJson(void) noexcept {
		for (auto& L : this->m_List) {
			for (auto& jd : GetJsonDataList()) {
				if (L.GetIDstr() == jd->m_id) {
					//既存のものを保持しておく
					std::ofstream outputfile(L.GetFilePath(), std::ios::app);
					for (const auto& p : L.GetTaskWorkData().GetPin()) {
						outputfile << "Task_PinPoint=[" + std::to_string(p.m_Point.x()) + DIV_STR + std::to_string(p.m_Point.y()) + DIV_STR + std::to_string(p.m_Point.z()) + "]\n";
						outputfile << "Task_PinMap=[" + DataBase::Instance()->GetMapData()->FindPtr(p.m_MapID)->GetName() + DIV_STR + std::to_string(p.m_MapSel) + "]\n";
					}
					outputfile.close();
					break;
				}
			}
		}
	}
};
