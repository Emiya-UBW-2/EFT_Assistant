#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	void TaskJsonData::GetJsonSub(const nlohmann::json& data) noexcept {
		{
			std::string buf = data["trader"]["name"];
			traderID = TraderData::Instance()->FindID(buf.c_str());
		}
		if (data.contains("map")) {
			if (!data["map"].is_null()) {
				std::string buf = data["map"]["name"];
				MapID = MapData::Instance()->FindID(buf.c_str());
			}
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
	void TaskJsonData::OutputDataSub(std::ofstream& outputfile) noexcept {
		auto SetTaskObjectiveCommon = [&](std::ofstream& outputfile, const TaskJsonData::TaskObjective& obj) {
			outputfile << "[\n";
			outputfile << "\tTaskType=" + obj.type + "\n";
			outputfile << "\tTaskText=" + obj.description + "\n";
			outputfile << "\tTaskOptional=" + (std::string)(obj.optional ? "true" : "false") + "\n";
			outputfile << "]\n";
		};
		auto SetTaskObjective = [&](std::ofstream& outputfile, const TaskJsonData::TaskObjective& obj, EnumTaskObjective /*prev*/) {
			switch ((EnumTaskObjective)obj.TaskObjectiveType) {
			case FPS_n2::EnumTaskObjective::TaskObjectiveBasic:
			{
				outputfile << "Task_Else=TaskType:" + obj.type + "\n";
				outputfile << "Task_Else=" + obj.description + "\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveBuildItem:
			{
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				if (obj.Items != InvalidID) {
					outputfile << "NeedItem=" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x1\n";
				}
				for (auto& m : obj.containsAll) {
					outputfile << "ContainsAll=" + m + "\n";
				}
				for (auto& m : obj.containsCategory) {
					outputfile << "ContainsCategory=" + m + "\n";
				}
				for (auto& m : obj.attributes) {
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
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				for (auto& m : obj.healthEffect) {
					if (m.bodyParts.size() > 0) {
						outputfile << "Task_Else=Ž©•ª‚Ìó‘ÔˆÙí‰ÓŠ:[";
						for (auto& m2 : m.bodyParts) {
							outputfile << m2;
							if (&m2 != &m.bodyParts.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (m.effects.size() > 0) {
						outputfile << "Task_Else=Ž©•ª‚Ìó‘ÔˆÙí“à—e:[";
						for (auto& m2 : m.effects) {
							outputfile << m2;
							if (&m2 != &m.effects.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (m.m_time.IsActive()) {
						outputfile << "Task_Else=‘Ï‹vŽžŠÔ: " + (std::string)(CompareMethodStr[(int)m.m_time.compareMethod]) + " " + std::to_string(m.m_time.value) + "\n";
					}
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveExtract:
			{
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				if (obj.exitStatus.size() > 0) {
					outputfile << "Task_Else=’EoƒXƒe[ƒ^ƒX:[";
					for (auto& m : obj.exitStatus) {
						outputfile << m;
						if (&m != &obj.exitStatus.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
				if (obj.exitName != "") {
					outputfile << "Task_Else=’Eo’n“_:" + obj.exitName + "\n";
				}
				if (obj.zoneNames.size() > 0) {
					outputfile << "Task_Else=’Eoƒ][ƒ“:[";
					for (auto& m : obj.zoneNames) {
						outputfile << m;
						if (&m != &obj.zoneNames.back()) {
							outputfile << ",";
						}
					}
					outputfile << "]\n";
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveItem:
			{
				if (obj.type == "findItem") {
					if (obj.Items != InvalidID) {
						outputfile << "NeedItem=" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x" + std::to_string(obj.count) + "\n";
					}
				}
				else {
					if (obj.Items != InvalidID) {
						if (obj.foundInRaid) {
							outputfile << "Task_FiR_HandOver=[" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x" + std::to_string(obj.count) + "]\n";
						}
						else {
							outputfile << "Task_NotFiR_HandOver=[" + ItemData::Instance()->FindPtr(obj.Items)->GetName() + "x" + std::to_string(obj.count) + "]\n";
						}
					}
					outputfile << "DogTagLv=" + std::to_string(obj.dogTagLevel) + "\n";
					//outputfile << "Max=" + std::to_string(obj.maxDurability) + "\n";
					//outputfile << "Min=" + std::to_string(obj.minDurability) + "\n";
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveMark:
			{
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				outputfile << "NeedItem=" + obj.markerItem + "x1\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectivePlayerLevel:
			{
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				outputfile << "Task_Else=ƒŒƒxƒ‹:" + std::to_string(obj.playerLevel) + "\n";
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveQuestItem:
				for (auto& m : obj.Maps) {
					outputfile << "Task_Else=E‚Á‚Ä”[•i:" + MapData::Instance()->FindPtr(m)->GetName() + "-" + obj.questItem + "x" + std::to_string(obj.count) + "\n";
				}
				break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveShoot:
			{
				outputfile << "Task_Kill=[\n";
				if (obj.Maps.size() == 0) {
					outputfile << "\t" + EnemyData::Instance()->FindPtr(obj.target)->GetName();
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
					for (auto& m : obj.Maps) {
						outputfile << "\t" + MapData::Instance()->FindPtr(m)->GetName() + "-" + EnemyData::Instance()->FindPtr(obj.target)->GetName();
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
						if (&m != &obj.Maps.back()) {
							outputfile << DIV_STR;
						}
						outputfile << "\n";
					}
				}
				outputfile << "]\n";

				//outputfile << "shotType=" + obj.shotType + "\n";
				if (obj.zoneNames.size() > 0) {
					outputfile << "Task_Else=ƒ][ƒ“:[";
					for (auto& m : obj.zoneNames) {
						outputfile << m;
						if (&m != &obj.zoneNames.back()) {
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
					outputfile << "Task_Else=‘•”õ‚ª•K—v:{\n";
					for (auto& m : obj.wearing) {
						outputfile << "Task_Else=  " + m + "\n";
					}
					outputfile << "Task_Else=}\n";
				}
				if (obj.notWearing.size() > 0) {
					outputfile << "Task_Else=‘•”õ‚µ‚Ä‚Í‚¢‚¯‚È‚¢:{\n";
					for (auto& m : obj.notWearing) {
						outputfile << "Task_Else=  " + m + "\n";
					}
					outputfile << "Task_Else=}\n";
				}
				if (obj.distance.IsActive()) {
					outputfile << "Task_Else=‹——£: " + (std::string)(CompareMethodStr[(int)obj.distance.compareMethod]) + " " + std::to_string(obj.distance.value) + "m\n";
				}
				{
					if (obj.playerHealthEffect.bodyParts.size() > 0) {
						outputfile << "Task_Else=Ž©•ª‚ÌŠY“–‰ÓŠ‚ÉˆÙíó‘Ô:[";
						for (auto& m2 : obj.playerHealthEffect.bodyParts) {
							outputfile << m2;
							if (&m2 != &obj.playerHealthEffect.bodyParts.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.playerHealthEffect.effects.size() > 0) {
						outputfile << "Task_Else=Ž©•ª‚ÌˆÙíó‘Ô:[";
						for (auto& m2 : obj.playerHealthEffect.effects) {
							outputfile << m2;
							if (&m2 != &obj.playerHealthEffect.effects.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.playerHealthEffect.m_time.IsActive()) {
						outputfile << "Task_Else=è‡’l: " + (std::string)(CompareMethodStr[(int)obj.playerHealthEffect.m_time.compareMethod]) + " " + std::to_string(obj.playerHealthEffect.m_time.value) + "\n";
					}
				}
				{
					if (obj.enemyHealthEffect.bodyParts.size() > 0) {
						outputfile << "Task_Else=“G‚ÌŠY“–‰ÓŠ‚ÉˆÙíó‘Ô:[";
						for (auto& m2 : obj.enemyHealthEffect.bodyParts) {
							outputfile << m2;
							if (&m2 != &obj.enemyHealthEffect.bodyParts.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.enemyHealthEffect.effects.size() > 0) {
						outputfile << "Task_Else=“G‚ÌˆÙíó‘Ô:[";
						for (auto& m2 : obj.enemyHealthEffect.effects) {
							outputfile << m2;
							if (&m2 != &obj.enemyHealthEffect.effects.back()) {
								outputfile << ",";
							}
						}
						outputfile << "]\n";
					}
					if (obj.enemyHealthEffect.m_time.IsActive()) {
						outputfile << "Task_Else=è‡’l: " + (std::string)(CompareMethodStr[(int)obj.enemyHealthEffect.m_time.compareMethod]) + " " + std::to_string(obj.enemyHealthEffect.m_time.value) + "\n";
					}
				}
			}
			break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveSkill:
				for (auto& m : obj.skillLevel) {
					outputfile << "Task_Else=Skill‚ðã‚°‚é:" + m.first + "+" + std::to_string(m.second) + "\n";
				}
				break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveTaskStatus:
			{
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				outputfile << "Task_Else=ŠY“–ƒ^ƒXƒN" + obj.task + "\n";
				if (obj.status.size() > 0) {
					outputfile << "Task_Else=ŠY“–ƒ^ƒXƒNó‘Ô:";
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
					outputfile << "Task_Else=TraderStanding‚Ìè‡’l: " + (std::string)(CompareMethodStr[(int)obj.Compares.compareMethod]) + " " + std::to_string(obj.Compares.value) + "\n";
				}
				break;
			case FPS_n2::EnumTaskObjective::TaskObjectiveUseItem:
			{
				for (auto& m : obj.Maps) {
					outputfile << "Task_Map=" + MapData::Instance()->FindPtr(m)->GetName() + "\n";
				}
				if (obj.Compares.IsActive()) {
					outputfile << "Task_Else=è‡’l: " + (std::string)(CompareMethodStr[(int)obj.Compares.compareMethod]) + " " + std::to_string(obj.Compares.value) + "\n";
				}
				if (obj.zoneNames.size() > 0) {
					outputfile << "Task_Else=ƒ][ƒ“:[";
					for (auto& m : obj.zoneNames) {
						outputfile << m;
						if (&m != &obj.zoneNames.back()) {
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
				outputfile << "Reward_Rep=" + m.trader + ((m.standing >= 0) ? "+" : "") + std::to_string(m.standing) + "\n";
			}
			if (obj.Items.size() > 0) {
				outputfile << "Reward_Item=[\n";
				for (auto& m : obj.Items) {
					if (m.ID != InvalidID) {
						outputfile << "\t" + ItemData::Instance()->FindPtr(m.ID)->GetName() + "x" + std::to_string(m.count);
					}
					else {
						outputfile << "\t" + m.Name + "x" + std::to_string(m.count);
					}
					if (&m != &obj.Items.back()) {
						outputfile << DIV_STR;
					}
					outputfile << "\n";
				}
				outputfile << "]\n";
			}
			for (auto& m : obj.m_skillLevelReward) {
				outputfile << "Reward_SkillLevelUp=" + m.name + ((m.level >= 0) ? "+" : "") + std::to_string(m.level) + "\n";
			}
			for (auto& m : obj.traderUnlock) {
				outputfile << "Reward_TraderUnlock=" + m + "\n";
			}
		};
		auto WriteText = [&](std::ofstream& outputfile) {
			outputfile << "Trader=" + TraderData::Instance()->FindPtr(this->traderID)->GetName() + "\n";
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
			if (this->MapID != InvalidID) {
				outputfile << "Task_Map=" + MapData::Instance()->FindPtr(this->MapID)->GetName() + "\n";
			}
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
					Prev = ob.TaskObjectiveType;
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
					Prev = ob.TaskObjectiveType;
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
};