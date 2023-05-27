#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class PlayerInfoBG :public BGParent {
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {
		}
		void Draw_Back_Sub(int, int, float) noexcept override {
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* Input = InputControl::Instance();

			int xpos = y_r(50);
			int ypos = LineHeight + y_r(50);
			//�����̖ڕW---------------------------------
			{
				WindowSystem::SetMsg(xpos, ypos, xpos, ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "�����̖ڕW");
				ypos += LineHeight + y_r(15);
			}
			//���C�g�L�[�p�[�ɕK�v��
			{
				auto tmp = PlayerData::Instance()->GetIsNeedLightKeeper();
				int xp = xpos + y_r(24);
				WindowSystem::CheckBox(xp, ypos, &tmp);
				WindowSystem::SetMsg(xp + y_r(210), ypos, xp + y_r(210), ypos + y_r(48), LineHeight, STRX_LEFT, White, Black, "���C�g�L�[�p�[�J���܂ŕK�{�ȃ^�X�N�̂ݑΏۂƂ���");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedKappa(false);
				}
				PlayerData::Instance()->SetIsNeedLightKeeper(tmp);
				ypos += y_r(48);
			}
			//�J�b�p�ɕK�v��
			{
				auto tmp = PlayerData::Instance()->GetIsNeedKappa();
				int xp = xpos + y_r(24);
				WindowSystem::CheckBox(xp, ypos, &tmp);
				WindowSystem::SetMsg(xp + y_r(210), ypos, xp + y_r(210), ypos + y_r(48), LineHeight, STRX_LEFT, White, Black, "�J�b�p�[�R���e�i�J���܂łɕK�{�ȃ^�X�N�̂ݑΏۂƂ���");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedLightKeeper(false);
				}
				PlayerData::Instance()->SetIsNeedKappa(tmp);
				ypos += y_r(48);
			}
			//���x������
			{
				int xp = xpos + y_r(24);
				if (WindowSystem::ClickCheckBox(xp, ypos, xp + y_r(100), ypos + y_r(48), true, true, Red, "DOWN")) {
					PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() - 1);
				}
				xp += y_r(105);
				if (WindowSystem::ClickCheckBox(xp, ypos, xp + y_r(100), ypos + y_r(48), true, true, Green, "UP")) {
					PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() + 1);
				}
				PlayerData::Instance()->SetMaxLevel(std::clamp(PlayerData::Instance()->GetMaxLevel(), 1, 71));
				xp += y_r(105);
				WindowSystem::SetMsg(xp, ypos + y_r(12), xp, ypos + y_r(12) + y_r(36), y_r(36), STRX_LEFT, White, Black, "�ڕW���x��");
				xp += y_r(300);
				WindowSystem::SetMsg(xp, ypos, xp, ypos + y_r(48), y_r(48), STRX_RIGHT, White, Black, "%d", PlayerData::Instance()->GetMaxLevel());
				ypos += y_r(48);
			}
			//�v���C���[�f�[�^---------------------------
			{
				WindowSystem::SetMsg(xpos, ypos, xpos, ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "�v���C���[�f�[�^");
				ypos += LineHeight + y_r(15);
			}
			//�G�f�B�V��������
			{
				int col = 1;
				auto Base = (int)PlayerData::Instance()->GetEdition();
				int xp = xpos + y_r(24);
				if (WindowSystem::ClickCheckBox(xp, ypos, xp + y_r(100), ypos + y_r(48), true, true, Red, "DOWN")) {
					Base--;
				}
				xp += y_r(105);
				if (WindowSystem::ClickCheckBox(xp, ypos, xp + y_r(100), ypos + y_r(48), true, true, Green, "UP")) {
					Base++;
				}
				Base = std::clamp(Base, (int)EnumEdition::STANDARD, (int)EnumEdition::EDGE_OF_DARKNESS);
				col = std::clamp(255 - 255 * Base / ((int)EnumEdition::EDGE_OF_DARKNESS), 1, 255);
				PlayerData::Instance()->SetEdition((EnumEdition)Base);
				xp += y_r(105);
				WindowSystem::SetMsg(xp, ypos + y_r(12), xp, ypos + y_r(12) + y_r(36), y_r(36), STRX_LEFT, White, Black, "�G�f�B�V����");
				xp += y_r(300);
				WindowSystem::SetMsg(xp, ypos, xp, ypos + y_r(48), y_r(48), STRX_LEFT, GetColor(col, col, col), GetColor(256 - col, 256 - col, 256 - col), "%s", EnumEditionStr[(int)PlayerData::Instance()->GetEdition()]);
				ypos += y_r(48);
			}
			//�w�c
			{
				auto tmp = PlayerData::Instance()->GetIsUSEC();
				int xp = xpos + y_r(24);
				WindowSystem::CheckBox(xp, ypos, &tmp);
				WindowSystem::SetMsg(xp + y_r(210), ypos, xp + y_r(210), ypos + y_r(48), LineHeight, STRX_LEFT, White, Black, "�v���C���[:%s", tmp ? "USEC" : "BEAR");
				PlayerData::Instance()->SetIsUSEC(tmp);
				ypos += y_r(48);
			}
			//�v���C���[�f�[�^---------------------------
			{
				WindowSystem::SetMsg(xpos, ypos, xpos, ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "�f�[�^�̃��Z�b�g");
				ypos += LineHeight + y_r(15);
			}
			//�^�X�N�̐i�s�󋵂����Z�b�g
			{
				if (WindowSystem::ClickCheckBox(xpos + y_r(24), ypos, xpos + y_r(200), ypos + LineHeight, true, true, Red, "Reset")) {
					PlayerData::Instance()->ResetTaskClear();
				}
				WindowSystem::SetMsg(xpos + y_r(210), ypos, xpos + y_r(210), ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "�^�X�N�̐i�s�󋵂����Z�b�g");
				ypos += LineHeight + y_r(15);
			}
			//�n�C�h�A�E�g�̊J���󋵂����Z�b�g
			{
				if (WindowSystem::ClickCheckBox(xpos + y_r(24), ypos, xpos + y_r(200), ypos + LineHeight, true, true, Red, "Reset")) {
					PlayerData::Instance()->ResetHideoutClear();
				}
				WindowSystem::SetMsg(xpos + y_r(210), ypos, xpos + y_r(210), ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "�n�C�h�A�E�g�̊J���󋵂����Z�b�g");
				ypos += LineHeight + y_r(15);
			}
			//-------------------------------------------
			//�߂�
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "�߂�")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};