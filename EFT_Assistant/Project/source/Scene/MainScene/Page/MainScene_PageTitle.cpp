#include "MainScene_PageTitle.hpp"
#include "../Data/MainScene_Base.hpp"

namespace FPS_n2 {
	void TitleBG::Draw_Back_Sub(int, int, float) noexcept {
		m_InfoStr = "";

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
		WindowSystem::SetMsg(xpos, ypos, xpos, ypos, LineHeight, STRX_MID, White, Black, m_InfoStr);
	}
	void TitleBG::DrawFront_Sub(int, int, float) noexcept {
		int Xsize = y_r(400);
		int Ysize = LineHeight;
		int XPos = y_r(1920) - Xsize - y_r(10);
		int YPos = LineHeight + y_r(10);
		//
		WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "�f�[�^��API����X�V");
		YPos += Ysize + y_r(5);
		//
		if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, CanUpdateData(), CanUpdateData() ? Gray25 : Gray50, "�Q�[�������X�V")) {
			DataBase::Instance()->DataUpdate(EnumDataType::ITEMDATA, "data/query/barter_query.txt", "data/item/Maked/Maked/", "items", 60);
			DataBase::Instance()->DataUpdate(EnumDataType::ITEMDATAJP, "data/query/barter_ja_query.txt", "data/item/Maked/Maked/", "items", 120);
			DataBase::Instance()->DataUpdate(EnumDataType::TASKDATA, "data/query/task_query.txt", "data/task/Maked/", "tasks", 60);
			DataBase::Instance()->DataUpdate(EnumDataType::HIDEOUTDATA, "data/query/hideout_query.txt", "data/Hideout/Maked/Maked", "hideoutStations", 40);
			DataBase::Instance()->DataUpdate(EnumDataType::TRADERDATA, "data/query/trader_query.txt", "data/trader/Maked", "traders", 1);
			PlayerData::Instance()->SetLastDataReceive();
			m_CoolTimeCount = GetNowCount();
		}
		YPos += Ysize + y_r(5);
		//
		if (!CanUpdateData()) {
			auto LastTimeSec = 60 * 5 - (GetNowCount() - m_CoolTimeCount) / 1000;
			if (LastTimeSec < 0) {
				m_CoolTimeCount = -1;
			}
			WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "�X�V�\�܂� %2d:%02d", LastTimeSec / 60, LastTimeSec % 60);
		}
		YPos += Ysize + y_r(5);
		//
		YPos += Ysize + y_r(5);
		//
		WindowSystem::SetMsg(XPos, YPos, XPos + Xsize, YPos + Ysize, Ysize, STRX_MID, White, Black, "Wiki�y�[�W����");
		YPos += Ysize + y_r(5);
		//
		if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, true, Gray25, "���y�[�W����")) {
			DataBase::Instance()->WikiDataUpdate(EnumWikiDataType::ITEMDATA_KEY_WIKI_HTML, nullptr, "WikiData/input/item/key_html/", "WikiData/output/item/key/");
			DataBase::Instance()->WikiDataUpdate(EnumWikiDataType::ITEMDATA_KEY, "WikiData/input/item/key.txt", "WikiData/input/item/key_html/", "WikiData/output/item/key/");
		}
		YPos += Ysize + y_r(5);
		if (WindowSystem::ClickCheckBox(XPos, YPos, XPos + Xsize, YPos + Ysize, false, true, Gray25, "���[�g�y�[�W����")) {
			DataBase::Instance()->WikiDataUpdate(EnumWikiDataType::LOOT_WIKI_HTML, nullptr, "WikiData/input/item/loot_html/", "WikiData/output/item/loot/");
		}
		YPos += Ysize + y_r(5);
	}
};
