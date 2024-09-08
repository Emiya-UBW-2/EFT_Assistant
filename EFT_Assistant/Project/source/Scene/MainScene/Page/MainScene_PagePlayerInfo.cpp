#include "MainScene_PagePlayerInfo.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void PlayerInfoBG::DrawFront_Sub(int, int, float) noexcept {
		//auto* WindowMngr = WindowMySystem::WindowManager::Instance();
		//auto* Pad = PadControl::Instance();

		int xpos = DXDraw::Instance()->GetUIY(50);
		int ypos = LineHeight + DXDraw::Instance()->GetUIY(50);
		//今期の目標---------------------------------
		{
			WindowSystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "今期の目標");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//ライトキーパーに必要か
		{
			auto tmp = PlayerData::Instance()->GetIsNeedLightKeeper();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "ライトキーパー開放まで必須なタスクのみ対象とする");
			if (tmp) {
				PlayerData::Instance()->SetIsNeedKappa(false);
			}
			PlayerData::Instance()->SetIsNeedLightKeeper(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//カッパに必要か
		{
			auto tmp = PlayerData::Instance()->GetIsNeedKappa();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "カッパーコンテナ開放までに必須なタスクのみ対象とする");
			if (tmp) {
				PlayerData::Instance()->SetIsNeedLightKeeper(false);
			}
			PlayerData::Instance()->SetIsNeedKappa(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//レベル操作
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
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(12) + DXDraw::Instance()->GetUIY(36) / 2, DXDraw::Instance()->GetUIY(36), STRX_LEFT, White, Black, "目標レベル");
			xp += DXDraw::Instance()->GetUIY(300);
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(48) / 2, DXDraw::Instance()->GetUIY(48), STRX_RIGHT, White, Black, "%d", PlayerData::Instance()->GetMaxLevel());
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//フリマ開放したか
		{
			auto tmp = PlayerData::Instance()->GetIsOpenFreaMarket();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "フリーマーケットを開放しているか");
			PlayerData::Instance()->SetIsOpenFreaMarket(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//プレイヤーデータ---------------------------
		{
			WindowSystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "プレイヤーデータ");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//エディション操作
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
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(12) + DXDraw::Instance()->GetUIY(36) / 2, DXDraw::Instance()->GetUIY(36), STRX_LEFT, White, Black, "エディション");
			xp += DXDraw::Instance()->GetUIY(300);
			WindowSystem::SetMsg(xp, ypos + DXDraw::Instance()->GetUIY(48) / 2, DXDraw::Instance()->GetUIY(48), STRX_LEFT, GetColor(col, col, col), GetColor(256 - col, 256 - col, 256 - col), "%s", EnumEditionStr[(int)PlayerData::Instance()->GetEdition()]);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//陣営
		{
			auto tmp = PlayerData::Instance()->GetIsUSEC();
			int xp = xpos + DXDraw::Instance()->GetUIY(24);
			tmp = WindowSystem::CheckBox(xp, ypos, tmp);
			WindowSystem::SetMsg(xp + DXDraw::Instance()->GetUIY(210), ypos + DXDraw::Instance()->GetUIY(48) / 2, LineHeight, STRX_LEFT, White, Black, "プレイヤー:%s", tmp ? "USEC" : "BEAR");
			PlayerData::Instance()->SetIsUSEC(tmp);
			ypos += DXDraw::Instance()->GetUIY(48);
		}
		//プレイヤーデータ---------------------------
		{
			WindowSystem::SetMsg(xpos, ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "データのリセット");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//タスクの進行状況をリセット
		{
			if (WindowSystem::SetMsgClickBox(xpos + DXDraw::Instance()->GetUIY(24), ypos, xpos + DXDraw::Instance()->GetUIY(200), ypos + LineHeight, LineHeight, Red, true, true, "Reset")) {
				PlayerData::Instance()->ResetTaskClear();
			}
			WindowSystem::SetMsg(xpos + DXDraw::Instance()->GetUIY(210), ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "タスクの進行状況をリセット");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//ハイドアウトの開放状況をリセット
		{
			if (WindowSystem::SetMsgClickBox(xpos + DXDraw::Instance()->GetUIY(24), ypos, xpos + DXDraw::Instance()->GetUIY(200), ypos + LineHeight, LineHeight, Red, true, true, "Reset")) {
				PlayerData::Instance()->ResetHideoutClear();
			}
			WindowSystem::SetMsg(xpos + DXDraw::Instance()->GetUIY(210), ypos + LineHeight / 2, LineHeight, STRX_LEFT, White, Black, "ハイドアウトの開放状況をリセット");
			ypos += LineHeight + DXDraw::Instance()->GetUIY(15);
		}
		//-------------------------------------------
	}
};
