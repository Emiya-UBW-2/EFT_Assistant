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
			//今期の目標---------------------------------
			{
				WindowSystem::SetMsg(xpos, ypos, xpos, ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "今期の目標");
				ypos += LineHeight + y_r(15);
			}
			//ライトキーパーに必要か
			{
				auto tmp = PlayerData::Instance()->GetIsNeedLightKeeper();
				int xp = xpos + y_r(24);
				WindowSystem::CheckBox(xp, ypos, &tmp);
				WindowSystem::SetMsg(xp + y_r(210), ypos, xp + y_r(210), ypos + y_r(48), LineHeight, STRX_LEFT, White, Black, "ライトキーパー開放まで必須なタスクのみ対象とする");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedKappa(false);
				}
				PlayerData::Instance()->SetIsNeedLightKeeper(tmp);
				ypos += y_r(48);
			}
			//カッパに必要か
			{
				auto tmp = PlayerData::Instance()->GetIsNeedKappa();
				int xp = xpos + y_r(24);
				WindowSystem::CheckBox(xp, ypos, &tmp);
				WindowSystem::SetMsg(xp + y_r(210), ypos, xp + y_r(210), ypos + y_r(48), LineHeight, STRX_LEFT, White, Black, "カッパーコンテナ開放までに必須なタスクのみ対象とする");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedLightKeeper(false);
				}
				PlayerData::Instance()->SetIsNeedKappa(tmp);
				ypos += y_r(48);
			}
			//レベル操作
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
				WindowSystem::SetMsg(xp, ypos + y_r(12), xp, ypos + y_r(12) + y_r(36), y_r(36), STRX_LEFT, White, Black, "目標レベル");
				xp += y_r(300);
				WindowSystem::SetMsg(xp, ypos, xp, ypos + y_r(48), y_r(48), STRX_RIGHT, White, Black, "%d", PlayerData::Instance()->GetMaxLevel());
				ypos += y_r(48);
			}
			//プレイヤーデータ---------------------------
			{
				WindowSystem::SetMsg(xpos, ypos, xpos, ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "プレイヤーデータ");
				ypos += LineHeight + y_r(15);
			}
			//エディション操作
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
				WindowSystem::SetMsg(xp, ypos + y_r(12), xp, ypos + y_r(12) + y_r(36), y_r(36), STRX_LEFT, White, Black, "エディション");
				xp += y_r(300);
				WindowSystem::SetMsg(xp, ypos, xp, ypos + y_r(48), y_r(48), STRX_LEFT, GetColor(col, col, col), GetColor(256 - col, 256 - col, 256 - col), "%s", EnumEditionStr[(int)PlayerData::Instance()->GetEdition()]);
				ypos += y_r(48);
			}
			//陣営
			{
				auto tmp = PlayerData::Instance()->GetIsUSEC();
				int xp = xpos + y_r(24);
				WindowSystem::CheckBox(xp, ypos, &tmp);
				WindowSystem::SetMsg(xp + y_r(210), ypos, xp + y_r(210), ypos + y_r(48), LineHeight, STRX_LEFT, White, Black, "プレイヤー:%s", tmp ? "USEC" : "BEAR");
				PlayerData::Instance()->SetIsUSEC(tmp);
				ypos += y_r(48);
			}
			//プレイヤーデータ---------------------------
			{
				WindowSystem::SetMsg(xpos, ypos, xpos, ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "データのリセット");
				ypos += LineHeight + y_r(15);
			}
			//タスクの進行状況をリセット
			{
				if (WindowSystem::ClickCheckBox(xpos + y_r(24), ypos, xpos + y_r(200), ypos + LineHeight, true, true, Red, "Reset")) {
					PlayerData::Instance()->ResetTaskClear();
				}
				WindowSystem::SetMsg(xpos + y_r(210), ypos, xpos + y_r(210), ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "タスクの進行状況をリセット");
				ypos += LineHeight + y_r(15);
			}
			//ハイドアウトの開放状況をリセット
			{
				if (WindowSystem::ClickCheckBox(xpos + y_r(24), ypos, xpos + y_r(200), ypos + LineHeight, true, true, Red, "Reset")) {
					PlayerData::Instance()->ResetHideoutClear();
				}
				WindowSystem::SetMsg(xpos + y_r(210), ypos, xpos + y_r(210), ypos + LineHeight, LineHeight, STRX_LEFT, White, Black, "ハイドアウトの開放状況をリセット");
				ypos += LineHeight + y_r(15);
			}
			//-------------------------------------------
			//戻る
			{
				int xp = y_r(10);
				int yp = LineHeight + y_r(10);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(200), yp + LineHeight, false, true, Gray25, "戻る")) {
					TurnOnGoNextBG();
				}
			}
		}
		void Dispose_Sub(void) noexcept override {
		}
	};
};