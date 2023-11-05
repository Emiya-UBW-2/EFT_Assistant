#include "MainScene_PageTitle.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void TitleBG::Draw_Back_Sub(int, int, float) noexcept {
		InfoStr = "";

		int xpos = y_r(960);
		int ypos = y_r(440);
		int xsize = y_r(420);
		int ysize = y_r(52);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�^�X�N", "�^�X�N���ƒB���󋵂̍X�V���s���y�[�W�ł�", BGSelect::Task);
		ypos += y_r(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�n�C�h�A�E�g", "�n�C�h�A�E�g���ƊJ���󋵂̍X�V���s���y�[�W�ł�", BGSelect::HideOut);
		ypos += y_r(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�A�C�e��", "�A�C�e���g�p���Ɣ��p���i���m�F�ł���y�[�W�ł�", BGSelect::Item);
		ypos += y_r(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�}�b�v", "�~�j�}�b�v�̉{���y�у^�X�N�Ō������ꏊ���L�����y�[�W�ł�", BGSelect::Map);
		ypos += y_r(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�J�X�^�}�C�Y", "�e��̃J�X�^���l���m�F�ł���y�[�W�ł�", BGSelect::Custom);
		ypos += y_r(100);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�g���[�_�[", "�g���[�_�[��LL�Ȃǂ̐ݒ肪�ł���y�[�W�ł�", BGSelect::Trader);
		ypos += y_r(100);


		xpos = y_r(960 + 480);
		ypos = y_r(540);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�A�C�e�����W���X�g", "���y�[�W�Őݒ肵���󋵂Ŏ��W����K�v�̂���A�C�e�����X�g�̃y�[�W�ł�", BGSelect::ItemList);

		xpos = y_r(960 - 480);
		ypos = y_r(540);
		DrawOnce(xpos - xsize / 2, ypos - ysize / 2, xpos + xsize / 2, ypos + ysize / 2, "�v���C���[���", "�ڕW��^�X�N���Ȃǂ��Ǘ�����y�[�W�ł�", BGSelect::PlayerInfo);


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
			time_t t = time(NULL);				// ���ݓ������擾����
			tm local;							// ���������i�[����ϐ���p�ӂ���
			localtime_s(&local, &t);			// ���[�J��������ϐ��Ɋi�[����
			char buffer[256];
			strftime(buffer, sizeof(buffer), "%Y %m/%d %H:%M", &local);
			PlayerData::Instance()->SetLastDataReceive(buffer);
			ttt = GetNowCount();
		};
		WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "�f�[�^��API����X�V");
		YPos += Ysize + y_r(5);

		auto Color = (ttt == -1) ? Gray25 : Gray50;
		if (DataBase::Instance()->DataUpdate(
			EnumDataType::ITEMDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�A�C�e���X�V",
			"data/query/barter_query.txt",
			"data/item/Maked/Maked/",
			"items"
		)) {

			if (DataBase::Instance()->DataUpdate(
				EnumDataType::ITEMDATAJP,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�A�C�e�����{��X�V",
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
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�^�X�N�X�V",
			"data/query/task_query.txt",
			"data/task/Maked/",
			"tasks"
		)) {
			TimeCard();
		}
		YPos += Ysize + y_r(5);

		if (DataBase::Instance()->DataUpdate(
			EnumDataType::HIDEOUTDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�n�C�h�A�E�g�X�V",
			"data/query/hideout_query.txt",
			"data/Hideout/Maked/Maked",
			"hideoutStations"
		)) {
			TimeCard();
		}
		YPos += Ysize + y_r(5);

		if (DataBase::Instance()->DataUpdate(
			EnumDataType::TRADERDATA,
			XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�g���[�_�[�X�V",
			"data/query/trader_query.txt",
			"data/trader/Maked",
			"traders",
			1
		)) {
			TimeCard();
		}
		YPos += Ysize + y_r(5);
		if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, (ttt == -1), Color, "�S�čX�V")) {
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::ITEMDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�A�C�e���X�V",
				"data/query/barter_query.txt",
				"data/item/Maked/Maked/",
				"items",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::ITEMDATAJP,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�A�C�e�����{��X�V",
				"data/query/barter_ja_query.txt",
				"data/item/Maked/Maked/",
				"items",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::TASKDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�^�X�N�X�V",
				"data/query/task_query.txt",
				"data/task/Maked/",
				"tasks",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::HIDEOUTDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�n�C�h�A�E�g�X�V",
				"data/query/hideout_query.txt",
				"data/Hideout/Maked/Maked",
				"hideoutStations",
				20, true
			)) {
			}
			if (DataBase::Instance()->DataUpdate(
				EnumDataType::TRADERDATA,
				XPos, YPos, Xsize, Ysize, (ttt == -1), Color, "�g���[�_�[�X�V",
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
			WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "�X�V�\�܂� %2d:%02d", (TotalTime - NowTime) / 1000 / 60, ((TotalTime - NowTime) / 1000) % 60);
		}
		//
		Xsize = y_r(500);
		XPos = y_r(1920) - Xsize - y_r(10);
		YPos = y_r(340);
		WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "Wiki�y�[�W����");
		YPos += Ysize + y_r(5);
		if (DataBase::Instance()->WikiDataUpdate(
			EnumWikiDataType::ITEMDATA_KEY_WIKI_HTML,
			XPos, YPos, Xsize, Ysize, true, Color, "�L�[��html��Wiki����擾",
			"WikiData/input/item/key.txt",
			"WikiData/input/item/key_html/",
			"WikiData/output/item/key/"
		)) {
		}
		YPos += Ysize + y_r(5);
		if (DataBase::Instance()->WikiDataUpdate(
			EnumWikiDataType::ITEMDATA_KEY,
			XPos, YPos, Xsize, Ysize, true, Color, "�L�[���X�V",
			"WikiData/input/item/key.txt",
			"WikiData/input/item/key_html/",
			"WikiData/output/item/key/"
		)) {
		}
		YPos += Ysize + y_r(5);
	}
};
