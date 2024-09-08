#include "MainScene_PagePlayerInfo.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void PlayerInfoBG::DrawFront_Sub(int, int, float) noexcept {
		//auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		//auto* Pad = PadControl::Instance();

		int xpos = DXDraw::Instance()->GetUIY(50);
		int ypos = LineHeight + DXDraw::Instance()->GetUIY(50);
		//�����̖ڕW---------------------------------
		{
			WindowSystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "�����̖ڕW");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//���C�g�L�[�p�[�ɕK�v��
		{
			auto tmp = PlayerData::Instance()->GetIsNeedLightKeeper();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "���C�g�L�[�p�[�J���܂ŕK�{�ȃ^�X�N�̂ݑΏۂƂ���");
			if (tmp) {
				PlayerData::Instance()->SetIsNeedKappa(false);
			}
			PlayerData::Instance()->SetIsNeedLightKeeper(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//�J�b�p�ɕK�v��
		{
			auto tmp = PlayerData::Instance()->GetIsNeedKappa();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "�J�b�p�[�R���e�i�J���܂łɕK�{�ȃ^�X�N�̂ݑΏۂƂ���");
			if (tmp) {
				PlayerData::Instance()->SetIsNeedLightKeeper(false);
			}
			PlayerData::Instance()->SetIsNeedKappa(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//���x������
		{
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			if (WindowSystem::SetMsgClickBox(xp, ypos, xp + DXDraw::Instance()->GetUIY(100), ypos + DXDraw::Instance()->GetUIY(48), LineHeight, Red, true, true, "DOWN")) {
				PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() - 1);
			}
			xp += DXDraw::Instance()->GetUIY(105);
			if (WindowSystem::SetMsgClickBox(xp, ypos, xp + DXDraw::Instance()->GetUIY(100), ypos + DXDraw::Instance()->GetUIY(48), LineHeight, Green, true, true, "UP")) {
				PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() + 1);
			}
			PlayerData::Instance()->SetMaxLevel(std::clamp(PlayerData::Instance()->GetMaxLevel(), 1, 71));
			xp += DXDraw::Instance()->GetUIY(105);
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(12) + DXDraw::Instance()->GetUIY(36) / 2, DXDraw::Instance()->GetUIY(36), STRX_LEFT, White, Black, "�ڕW���x��");
			xp += DXDraw::Instance()->GetUIY(300);
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(48) / 2, DXDraw::Instance()->GetUIY(48), STRX_RIGHT, White, Black, "%d", PlayerData::Instance()->GetMaxLevel());
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//�t���}�J��������
		{
			auto tmp = PlayerData::Instance()->GetIsOpenFreaMarket();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "�t���[�}�[�P�b�g���J�����Ă��邩");
			PlayerData::Instance()->SetIsOpenFreaMarket(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//�v���C���[�f�[�^---------------------------
		{
			WindowSystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "�v���C���[�f�[�^");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//�G�f�B�V��������
		{
			int col = 1;
			auto Base = (int)PlayerData::Instance()->GetEdition();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			if (WindowSystem::SetMsgClickBox(xp, ypos, xp + DXDraw::Instance()->GetUIY(100), ypos + DXDraw::Instance()->GetUIY(48), LineHeight, Red, true, true,  "DOWN")) {
				Base--;
			}
			xp += DXDraw::Instance()->GetUIY(105);
			if (WindowSystem::SetMsgClickBox(xp, ypos, xp + DXDraw::Instance()->GetUIY(100), ypos + DXDraw::Instance()->GetUIY(48), LineHeight, Green, true, true, "UP")) {
				Base++;
			}
			Base = std::clamp(Base, (int)EnumEdition::STANDARD, (int)EnumEdition::EDGE_OF_DARKNESS);
			col = std::clamp(255 - 255 * Base / ((int)EnumEdition::EDGE_OF_DARKNESS), 1, 255);
			PlayerData::Instance()->SetEdition((EnumEdition)Base);
			xp += DXDraw::Instance()->GetUIY(105);
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(12) + DXDraw::Instance()->GetUIY(36) / 2, DXDraw::Instance()->GetUIY(36), STRX_LEFT, White, Black, "�G�f�B�V����");
			xp += DXDraw::Instance()->GetUIY(300);
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(48) / 2, DXDraw::Instance()->GetUIY(48), STRX_LEFT, GetColor(col, col, col), GetColor(256 - col, 256 - col, 256 - col), "%s", EnumEditionStr[(int)PlayerData::Instance()->GetEdition()]);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//�w�c
		{
			auto tmp = PlayerData::Instance()->GetIsUSEC();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "�v���C���[:%s", tmp ? "USEC" : "BEAR");
			PlayerData::Instance()->SetIsUSEC(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//�v���C���[�f�[�^---------------------------
		{
			WindowSystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "�f�[�^�̃��Z�b�g");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//�^�X�N�̐i�s�󋵂����Z�b�g
		{
			if (WindowSystem::SetMsgClickBox(xpos + DXDraw::Instance()->GetUIY(24), ypos, xpos + DXDraw::Instance()->GetUIY(200), ypos + LineHeight, LineHeight, Red, true, true, "Reset")) {
				PlayerData::Instance()->ResetTaskClear();
			}
			WindowSystem::SetMsg(xpos + DXDraw::Instance()->GetUIY(210), ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "�^�X�N�̐i�s�󋵂����Z�b�g");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//�n�C�h�A�E�g�̊J���󋵂����Z�b�g
		{
			if (WindowSystem::SetMsgClickBox(xpos + DXDraw::Instance()->GetUIY(24), ypos, xpos + DXDraw::Instance()->GetUIY(200), ypos + LineHeight, LineHeight, Red, true, true, "Reset")) {
				PlayerData::Instance()->ResetHideoutClear();
			}
			WindowSystem::SetMsg(xpos + DXDraw::Instance()->GetUIY(210), ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "�n�C�h�A�E�g�̊J���󋵂����Z�b�g");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//-------------------------------------------
	}
};
