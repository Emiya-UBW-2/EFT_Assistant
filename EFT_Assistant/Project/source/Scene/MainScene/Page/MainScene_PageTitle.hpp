#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class TitleBG :public BGParent {
	private:
		BGSelect m_Select{ (BGSelect)0 };
		std::string strResult;
		int ttt = -1;
	private:
		void DrawOnce(int xpos1, int ypos1, int xpos2, int ypos2, const char* Name, BGSelect SelWindow) noexcept {
			if (WindowSystem::ClickCheckBox(xpos1, ypos1, xpos2, ypos2, false, true, Gray25, Name)) {
				m_Select = SelWindow;
				TurnOnGoNextBG();
			}
		}
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
			m_Select = (BGSelect)0;
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {}
		void Draw_Back_Sub(int, int, float) noexcept override {
			int xpos = y_r(960);
			int ypos = y_r(540);
			int xsize = y_r(420);
			int ysize = y_r(52);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "タスク", BGSelect::Task);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "ハイドアウト", BGSelect::HideOut);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "アイテム", BGSelect::Item);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "マップ", BGSelect::Map);
			ypos += y_r(100);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "カスタマイズ", BGSelect::Custom);
			ypos += y_r(100);


			xpos = y_r(1440);
			ypos = y_r(540);
			DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "アイテム収集リスト", BGSelect::ItemList);
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
				int count = 0;
				//ItemData::Instance()->InitDatabyJson();
				while (true) {
					if (ItemDataRequest(20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						ItemData::Instance()->GetJsonData(data);
						ItemData::Instance()->SaveDatabyJson();
						if (data["data"]["items"].size() != 20) {
							break;
						}
					}
					count++;
				}
				ItemData::Instance()->CheckThroughJson();
				TimeCard();
			}
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "タスク更新")) {
				int count = 0;
				TaskData::Instance()->InitDatabyJson();
				while (true) {
					if (TaskDataRequest(20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						TaskData::Instance()->GetJsonData(data);
						TaskData::Instance()->SaveDatabyJson();
						if (data["data"]["tasks"].size() != 20) {
							break;
						}
					}
					count++;
				}
				TaskData::Instance()->CheckThroughJson();
				TimeCard();
			}
			YPos += Ysize + y_r(5);
			if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "ハイドアウト更新")) {
				int count = 0;
				//HideoutData::Instance()->InitDatabyJson();
				while (true) {
					if (HideoutDataRequest(20 * count, 20, strResult)) {
						ProcessMessage();
						auto data = nlohmann::json::parse(strResult);
						HideoutData::Instance()->GetJsonData(data);
						HideoutData::Instance()->SaveDatabyJson();
						if (data["data"]["hideoutStations"].size() != 20) {
							break;
						}
					}
					count++;
				}
				HideoutData::Instance()->CheckThroughJson();
				TimeCard();
			}
			YPos += Ysize + y_r(5);
			if (!(ttt == -1)) {
				WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "更新可能まで %2d:%02d", (TotalTime - NowTime) / 1000 / 60, ((TotalTime - NowTime) / 1000)%60);
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	public:
		const auto& GetNextSelect() const noexcept { return m_Select; }
	};
};