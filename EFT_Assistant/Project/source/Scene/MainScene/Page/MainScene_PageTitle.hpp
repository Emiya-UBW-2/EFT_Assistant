#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TitleBG :public BGParent {
	private:
		BGSelect	m_Select{ (BGSelect)0 };
		std::string	strResult;
		std::string	InfoStr;
		int			ttt{ -1 };
	private:
		void DrawOnce(int xpos1, int ypos1, int xpos2, int ypos2, const char* Name, const char* Info, BGSelect SelWindow) noexcept {
			auto* Input = InputControl::Instance();
			if (in2_(Input->GetMouseX(), Input->GetMouseY(), xpos1, ypos1, xpos2, ypos2)) {
				InfoStr = Info;
			}
			if (WindowSystem::ClickCheckBox(xpos1, ypos1, xpos2, ypos2, false, true, Gray25, Name)) {
				m_Select = SelWindow;
				TurnOnGoNextBG();
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Select = (BGSelect)0;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}
		void Draw_Back_Sub(int, int, float) noexcept override {
			InfoStr = "";

			int xpos = y_r(960);
			int ypos = y_r(440);
			int xsize = y_r(420);
			int ysize = y_r(52);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "タスク", "タスク情報と達成状況の更新を行うページです", BGSelect::Task);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "ハイドアウト", "ハイドアウト情報と開放状況の更新を行うページです", BGSelect::HideOut);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "アイテム", "アイテム使用情報と売却価格を確認できるページです", BGSelect::Item);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "マップ", "ミニマップの閲覧及びタスクで向かう場所を記したページです", BGSelect::Map);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "カスタマイズ", "銃器のカスタム値を確認できるページです", BGSelect::Custom);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "トレーダー", "トレーダーのLLなどの設定ができるページです", BGSelect::Trader);
			ypos += y_r(100);


			xpos = y_r(960 + 480);
			ypos = y_r(540);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "アイテム収集リスト", "他ページで設定した状況で収集する必要のあるアイテムリストのページです", BGSelect::ItemList);

			xpos = y_r(960 - 480);
			ypos = y_r(540);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "プレイヤー情報", "目標やタスク情報などを管理するページです", BGSelect::PlayerInfo);


			xpos = y_r(960);
			ypos = y_r(1080) - LineHeight;
			WindowSystem::SetMsg(xpos, ypos, xpos, ypos, LineHeight, STRX_MID, White, Black, InfoStr);
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			int Xsize = y_r(400);
			int Ysize = LineHeight;
			int XPos = y_r(1920) - Xsize - y_r(10);
			int YPos = LineHeight + y_r(10);

			auto NowTime = (GetNowCount() - ttt);
			auto TotalTime = 1000 * 60 * 5;

			if (ttt != -1) {
				if (NowTime > TotalTime) {
					ttt = -1;
				}
			}
			auto TimeCard = [&]() {
				time_t t = time(NULL);				// 現在日時を取得する
				tm local;							// 日時情報を格納する変数を用意する
				localtime_s(&local, &t);			// ローカル日時を変数に格納する
				char buffer[256];
				strftime(buffer, sizeof(buffer), "%Y %m/%d %H:%M", &local);
				PlayerData::Instance()->SetLastDataReceive(buffer);
				ttt = GetNowCount();
			};
			WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "データをAPIから更新");
			YPos += Ysize + y_r(5);

			auto Color = (ttt == -1) ? Gray25 : Gray50;

			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "アイテム更新")) {
				auto BaseTime = GetNowHiPerformanceCount();

				int count = 0;
				DataBase::Instance()->GetItemData()->InitDatabyJson();
				while (true) {
					if (CommonDataRequest("data/query/barter_query.txt", 20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						DataBase::Instance()->GetItemData()->GetJsonData(data);
						DataBase::Instance()->GetItemData()->UpdateData(20 * count, 20);
						DataBase::Instance()->GetItemData()->SaveAsNewData2("data/item/Maked/Maked/");

						if (data["data"]["items"].size() != 20) {
							break;
						}
					}
					count++;
				}
				DataBase::Instance()->GetItemData()->CheckThroughJson();
				TimeCard();
				{
					std::string ErrMes = "Load:" + std::to_string((GetNowHiPerformanceCount() - BaseTime) / 1000);
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "タスク更新")) {
				auto BaseTime = GetNowHiPerformanceCount();

				int count = 0;
				DataBase::Instance()->GetTaskData()->InitDatabyJson();
				while (true) {
					if (CommonDataRequest("data/query/task_query.txt", 20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						DataBase::Instance()->GetTaskData()->GetDataJson(data["data"]["tasks"]);
						DataBase::Instance()->GetTaskData()->UpdateData(20 * count, 20);
						DataBase::Instance()->GetTaskData()->SaveAsNewData2("data/task/Maked/");
						if (data["data"]["tasks"].size() != 20) {
							break;
						}
					}
					count++;
				}
				DataBase::Instance()->GetTaskData()->CheckThroughJson();
				TimeCard();
				{
					std::string ErrMes = "Load:" + std::to_string((GetNowHiPerformanceCount() - BaseTime) / 1000);
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "ハイドアウト更新")) {
				auto BaseTime = GetNowHiPerformanceCount();

				int count = 0;
				DataBase::Instance()->GetHideoutData()->InitDatabyJson();
				while (true) {
					if (CommonDataRequest("data/query/hideout_query.txt", 20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						DataBase::Instance()->GetHideoutData()->GetDataJson(data["data"]["hideoutStations"]);
						DataBase::Instance()->GetHideoutData()->UpdateData(20 * count, 20);
						DataBase::Instance()->GetHideoutData()->SaveAsNewData("data/Hideout/Maked/Maked");
						if (data["data"]["hideoutStations"].size() != 20) {
							break;
						}
					}
					count++;
				}
				DataBase::Instance()->GetHideoutData()->CheckThroughJson();
				TimeCard();
				{
					std::string ErrMes = "Load:" + std::to_string((GetNowHiPerformanceCount() - BaseTime) / 1000);
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "トレーダー更新")) {
				auto BaseTime = GetNowHiPerformanceCount();

				int count = 0;
				DataBase::Instance()->GetTraderData()->InitDatabyJson();
				while (true) {
					if (CommonDataRequest("data/query/trader_query.txt", 2 * count, 2, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						DataBase::Instance()->GetTraderData()->GetDataJson(data["data"]["traders"]);
						DataBase::Instance()->GetTraderData()->UpdateData(2 * count, 2);
						DataBase::Instance()->GetTraderData()->SaveAsNewData("data/trader/Maked");
						if (data["data"]["traders"].size() != 2) {
							break;
						}
					}
					count++;
				}
				DataBase::Instance()->GetTraderData()->CheckThroughJson();
				TimeCard();
				{
					std::string ErrMes = "Load:" + std::to_string((GetNowHiPerformanceCount() - BaseTime) / 1000);
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
			YPos += Ysize + y_r(5);
			if (!(ttt == -1)) {
				WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "更新可能まで %2d:%02d", (TotalTime - NowTime) / 1000 / 60, ((TotalTime - NowTime) / 1000) % 60);
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		const auto& GetNextSelect() const noexcept { return this->m_Select; }
	};
};
