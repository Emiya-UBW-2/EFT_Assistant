#include "MainScene_PageTitle.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void TitleBG::Draw_Back_Sub(int, int, float) noexcept {
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
	void TitleBG::DrawFront_Sub(int, int, float) noexcept {
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
		if (DataBase::Instance()->DataUpdate(
			EnumDataType::ITEMDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "アイテム更新",
			"data/query/barter_query.txt",
			"data/item/Maked/Maked/",
			"items"
		)) {

			if (DataBase::Instance()->DataUpdate(
				EnumDataType::ITEMDATAJP,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "アイテム日本語更新",
				"data/query/barter_ja_query.txt",
				"data/item/Maked/Maked/",
				"items",
				20, true
			)) {
			}

			TimeCard();
		}
		YPos += Ysize + y_r(5);

		if (DataBase::Instance()->DataUpdate(
			EnumDataType::TASKDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "タスク更新",
			"data/query/task_query.txt",
			"data/task/Maked/",
			"tasks"
		)) {
			TimeCard();
		}
		YPos += Ysize + y_r(5);

		if (DataBase::Instance()->DataUpdate(
			EnumDataType::HIDEOUTDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "ハイドアウト更新",
			"data/query/hideout_query.txt",
			"data/Hideout/Maked/Maked",
			"hideoutStations"
		)) {
			TimeCard();
		}
		YPos += Ysize + y_r(5);

		if (DataBase::Instance()->DataUpdate(
			EnumDataType::TRADERDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "トレーダー更新",
			"data/query/trader_query.txt",
			"data/trader/Maked",
			"traders",
			1
		)) {
			TimeCard();
		}
		YPos += Ysize + y_r(5);
		if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "全て更新")) {
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::ITEMDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "アイテム更新",
				"data/query/barter_query.txt",
				"data/item/Maked/Maked/",
				"items",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::ITEMDATAJP,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "アイテム日本語更新",
				"data/query/barter_ja_query.txt",
				"data/item/Maked/Maked/",
				"items",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::TASKDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "タスク更新",
				"data/query/task_query.txt",
				"data/task/Maked/",
				"tasks",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::HIDEOUTDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "ハイドアウト更新",
				"data/query/hideout_query.txt",
				"data/Hideout/Maked/Maked",
				"hideoutStations",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::TRADERDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "トレーダー更新",
				"data/query/trader_query.txt",
				"data/trader/Maked",
				"traders",
				1, true
			)) {
			}
			TimeCard();
		}
		YPos += Ysize + y_r(5);
		if (!(ttt == -1)) {
			WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "更新可能まで %2d:%02d", (TotalTime - NowTime) / 1000 / 60, ((TotalTime - NowTime) / 1000) % 60);
		}
		//
		Xsize = y_r(500);
		XPos = y_r(1920) - Xsize - y_r(10);
		YPos = y_r(340);
		WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "Wikiページ生成");
		YPos += Ysize + y_r(5);
		if (DataBase::Instance()->WikiDataUpdate(
			EnumWikiDataType::ITEMDATA_KEY_WIKI_HTML,
			XPos, YPos, Xsize, Ysize, true, Color, "キーのhtmlをWikiから取得",
			"WikiData/input/item/key.txt",
			"WikiData/input/item/key_html/",
			"WikiData/output/item/key/"
		)) {
		}
		YPos += Ysize + y_r(5);
		if (DataBase::Instance()->WikiDataUpdate(
			EnumWikiDataType::ITEMDATA_KEY,
			XPos, YPos, Xsize, Ysize, true, Color, "キー情報更新",
			"WikiData/input/item/key.txt",
			"WikiData/input/item/key_html/",
			"WikiData/output/item/key/"
		)) {
		}
		YPos += Ysize + y_r(5);
	}
};
