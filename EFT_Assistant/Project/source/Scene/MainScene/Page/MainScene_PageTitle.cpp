#include "MainScene_PageTitle.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void TitleBG::Draw_Back_Sub(int, int, float) noexcept {
		m_InfoStr = "";

		int xpos = DXDraw::Instance()->GetUIY(960);
		int ypos = DXDraw::Instance()->GetUIY(440);
		int xsize = DXDraw::Instance()->GetUIY(420);
		int ysize = DXDraw::Instance()->GetUIY(52);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "タスク", "タスク情報と達成状況の更新を行うページです", BGSelect::Task);
		ypos += DXDraw::Instance()->GetUIY(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "ハイドアウト", "ハイドアウト情報と開放状況の更新を行うページです", BGSelect::HideOut);
		ypos += DXDraw::Instance()->GetUIY(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "アイテム", "アイテム使用情報と売却価格を確認できるページです", BGSelect::Item);
		ypos += DXDraw::Instance()->GetUIY(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "マップ", "ミニマップの閲覧及びタスクで向かう場所を記したページです", BGSelect::Map);
		ypos += DXDraw::Instance()->GetUIY(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "カスタマイズ", "銃器のカスタム値を確認できるページです", BGSelect::Custom);
		ypos += DXDraw::Instance()->GetUIY(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "トレーダー", "トレーダーのLLなどの設定ができるページです", BGSelect::Trader);
		ypos += DXDraw::Instance()->GetUIY(100);


		xpos = DXDraw::Instance()->GetUIY(960 + 480);
		ypos = DXDraw::Instance()->GetUIY(540);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "アイテム収集リスト", "他ページで設定した状況で収集する必要のあるアイテムリストのページです", BGSelect::ItemList);

		xpos = DXDraw::Instance()->GetUIY(960 - 480);
		ypos = DXDraw::Instance()->GetUIY(540);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "プレイヤー情報", "目標やタスク情報などを管理するページです", BGSelect::PlayerInfo);


		xpos = DXDraw::Instance()->GetUIY(960);
		ypos = DXDraw::Instance()->GetUIY(1080) - LineHeight;
		WindowSystem::SetMsg(xpos, ypos, LineHeight, STRX_MID, White, Black, m_InfoStr);
	}
	void TitleBG::DrawFront_Sub(int, int, float) noexcept {
		int Xsize = DXDraw::Instance()->GetUIY(400);
		int Ysize = LineHeight;
		int XPos = DXDraw::Instance()->GetUIY(1920) - Xsize - DXDraw::Instance()->GetUIY(10);
		int YPos = LineHeight + DXDraw::Instance()->GetUIY(10);
		//
		WindowSystem::SetMsg(XPos + Xsize / 2, YPos + Ysize / 2, Ysize, STRX_MID, White, Black, "データをAPIから更新");
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
		//
		if (WindowSystem::SetMsgClickBox(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, CanUpdateData() ? Gray25 : Gray50, false, CanUpdateData(), "ゲーム内情報更新")) {
			DataBase::Instance()->DataUpdate(EnumDataType::ITEMDATA, "data/query/barter_query.txt", "data/item/Maked/", "items", 50);
			DataBase::Instance()->DataUpdate(EnumDataType::ITEMDATAJP, "data/query/barter_ja_query.txt", "data/item/Maked/MakedJP/", "items", 120);
			DataBase::Instance()->DataUpdate(EnumDataType::TASKDATA, "data/query/task_query.txt", "data/task/Maked/", "tasks", 60);
			DataBase::Instance()->DataUpdate(EnumDataType::HIDEOUTDATA, "data/query/hideout_query.txt", "data/Hideout/Maked/Maked", "hideoutStations", 40);
			DataBase::Instance()->DataUpdate(EnumDataType::TRADERDATA, "data/query/trader_query.txt", "data/trader/Maked", "traders", 2);
			DataBase::Instance()->DataUpdate(EnumDataType::TRADERBARTERS, "data/query/barters_query.txt", "data/trader/MakedBarters", "barters", 100);
			PlayerData::Instance()->SetLastDataReceive();
			m_CoolTimeCount = GetNowCount();
		}
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
		//
		if (!CanUpdateData()) {
			auto LastTimeSec = 60 * 5 - (GetNowCount() - m_CoolTimeCount) / 1000;
			if (LastTimeSec < 0) {
				m_CoolTimeCount = -1;
			}
			WindowSystem::SetMsg(XPos + Xsize / 2, YPos + Ysize / 2, Ysize, STRX_MID, White, Black, "更新可能まで %2d:%02d", LastTimeSec / 60, LastTimeSec % 60);
		}
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
		//
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
		//
		WindowSystem::SetMsg(XPos + Xsize / 2, YPos + Ysize / 2, Ysize, STRX_MID, White, Black, "Wikiページ生成");
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
		//
		if (WindowSystem::SetMsgClickBox(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, Gray25, false, true, "鍵ページ生成")) {
			DataBase::Instance()->WikiDataUpdate(EnumWikiDataType::ITEMDATA_KEY_WIKI_HTML, nullptr, "WikiData/input/item/key_html/", "WikiData/output/item/key/");
			DataBase::Instance()->WikiDataUpdate(EnumWikiDataType::ITEMDATA_KEY, "WikiData/input/item/key.txt", "WikiData/input/item/key_html/", "WikiData/output/item/key/");
		}
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
		if (WindowSystem::SetMsgClickBox(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, Gray25, false, true, "ルートページ生成")) {
			DataBase::Instance()->WikiDataUpdate(EnumWikiDataType::LOOT_WIKI_HTML, nullptr, "WikiData/input/item/loot_html/", "WikiData/output/item/loot/");
		}
		YPos += Ysize + DXDraw::Instance()->GetUIY(5);
	}
};
