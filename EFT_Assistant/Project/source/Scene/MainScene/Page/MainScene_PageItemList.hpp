#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemListBG :public BGParent {
	private:
		bool							m_IsCheckCraft{ true };//�^�X�N�ɕK�v�ȃA�C�e�����Ȃ����ۂ�
		bool							m_IsCheckBarter{ true };//�^�X�N�ɕK�v�ȃA�C�e�����Ȃ����ۂ�
		bool							m_IsNeedItem{ true };//�^�X�N�ɕK�v�ȃA�C�e�����Ȃ����ۂ�
		bool							m_IsCheckOpenCraft{ true };
		bool							m_IsCheckTask{ true };
		bool							m_DrawCanClearTask{ false };
		bool							m_DrawCanClearHideout{ false };
		WindowSystem::ScrollBoxClass	m_Scroll;
		float							m_YNow{ 0.f };
	private:
		void Init_Sub(int*, int*, float*) noexcept override {
		}
		void LateExecute_Sub(int*, int*, float*) noexcept override {}

		void Draw_Back_Sub(int, int, float) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* DrawParts = DXDraw::Instance();
			//auto* Input = InputControl::Instance();

			int xpos = y_r(50);
			int ypos = LineHeight + y_r(10) + LineHeight + y_r(10);
			{
				struct counts
				{
					ItemID m_ID{ InvalidID };
					int count;
					bool isFir{ true };
					bool isNeed{ false };
				};
				std::vector<std::vector<counts>> Counter;
				Counter.resize(ItemTypeData::Instance()->GetList().size());
				//�^�X�N
				if (m_IsCheckTask) {
					for (const auto& tasks : TaskData::Instance()->GetList()) {
						bool IsChecktask = true;
						if (IsChecktask && PlayerData::Instance()->GetIsNeedKappa()) {//�͓��K�v�^�X�N��������
							if (!tasks.GetTaskNeedData().GetKappaRequired()) {
								IsChecktask = false;
							}
							if (IsChecktask) {
								if (tasks.GetName() == "Collector") {
									//IsChecktask = false;
								}
							}
						}
						if (IsChecktask && PlayerData::Instance()->GetIsNeedLightKeeper()) {//���C�g�L�[�p�[
							if (!tasks.GetTaskNeedData().GetLightKeeperRequired()) {
								IsChecktask = false;
							}
						}
						if (IsChecktask && PlayerData::Instance()->GetMaxLevel() < tasks.GetTaskNeedData().GetLevel()) {//�ő僌�x��
							IsChecktask = false;
						}
						if (IsChecktask && PlayerData::Instance()->GetTaskClear(tasks.GetName().c_str())) {
							IsChecktask = false;
						}
						if (IsChecktask && m_DrawCanClearTask) {//�N���A�ł���^�X�N�����\��
							if (tasks.GetTaskNeedData().GetParenttaskID().size() > 0) {
								for (const auto& p : tasks.GetTaskNeedData().GetParenttaskID()) {
									if (!PlayerData::Instance()->GetTaskClear(TaskData::Instance()->FindPtr(p.GetParenttaskID())->GetName().c_str())) {
										IsChecktask = false;
										break;
									}
								}
							}
						}
						if (!IsChecktask) { continue; }
						for (const auto& w : tasks.GetTaskWorkData().GetFiR_Item()) {
							auto ID = w.GetID();
							auto* ptr = ItemData::Instance()->FindPtr(ID);
							if (ptr) {
								auto& Types = Counter.at(ptr->GetTypeID());
								auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == true); });
								if (Find != Types.end()) {
									Find->count += w.GetValue();
								}
								else {
									counts tmp;
									tmp.m_ID = ID;
									tmp.count = w.GetValue();
									tmp.isFir = true;
									tmp.isNeed = false;
									Types.emplace_back(tmp);
								}
							}
						}
						for (const auto& w : tasks.GetTaskWorkData().GetNotFiR_Item()) {
							auto ID = w.GetID();
							auto* ptr = ItemData::Instance()->FindPtr(ID);
							if (ptr) {
								auto& Types = Counter.at(ptr->GetTypeID());
								auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == false); });
								if (Find != Types.end()) {
									Find->count += w.GetValue();
								}
								else {
									counts tmp;
									tmp.m_ID = ID;
									tmp.count = w.GetValue();
									tmp.isFir = false;
									tmp.isNeed = false;
									Types.emplace_back(tmp);
								}
							}
						}
						if (m_IsNeedItem) {
							for (const auto& w : tasks.GetTaskNeedData().GetItem()) {
								auto ID = w.GetID();
								auto* ptr = ItemData::Instance()->FindPtr(ID);
								if (ptr) {
									auto& Types = Counter.at(ptr->GetTypeID());
									auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isNeed == true); });
									if (Find != Types.end()) {
										Find->count += w.GetValue();
									}
									else {
										auto Find2 = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isNeed == false); });
										if (Find2 == Types.end()) {
											counts tmp;
											tmp.m_ID = ID;
											tmp.count = w.GetValue();
											tmp.isFir = false;
											tmp.isNeed = true;
											Types.emplace_back(tmp);
										}
									}
								}
							}
						}
					}
				}
				//�n�C�h�A�E�g�J��
				if (m_IsCheckOpenCraft) {
					for (const auto&L : HideoutData::Instance()->GetList()) {
						for (const auto& Ld : L.GetLvData()) {
							bool IsChecktask = true;
							if (IsChecktask && PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
								IsChecktask = false;
							}
							if(IsChecktask && m_DrawCanClearHideout){
								for (const auto& w : Ld.m_Parent) {
									if (!PlayerData::Instance()->GetHideoutClear(HideoutData::Instance()->FindPtr(w.GetID())->GetName().c_str(), w.GetValue())) {
										IsChecktask = false;
										break;
									}
								}
							}
							if (!IsChecktask) { continue; }
							for (const auto& w : Ld.m_ItemReq) {
								auto ID = w.GetID();
								auto* ptr = ItemData::Instance()->FindPtr(ID);
								if (ptr) {
									auto& Types = Counter.at(ptr->GetTypeID());
									auto Find = std::find_if(Types.begin(), Types.end(), [&](const counts& obj) {return (obj.m_ID == ID) && (obj.isFir == false) && (obj.isNeed == false); });
									if (Find != Types.end()) {
										Find->count += w.GetValue();
									}
									else {
										counts tmp;
										tmp.m_ID = ID;
										tmp.count = w.GetValue();
										tmp.isFir = false;
										tmp.isNeed = true;
										Types.emplace_back(tmp);
									}
								}
							}
						}
					}
				}
				//�`��
				{
					int xpBase = xpos;
					int ypBase = ypos - (int)m_YNow;
					int xp = xpBase;
					int yp = ypBase;
					int ypMax = (y_r(1080) - y_r(240));
					int xsize = (int)((float)y_r(600));
					int ysize = (int)((float)y_r(64));
					int xsizeAdd = xsize + y_r(30);
					int ysizeAdd = ysize + y_r(5);
					for (auto& Cat : ItemCategoryData::Instance()->GetList()) {
						bool IsHit = false;
						for (auto& Type : ItemTypeData::Instance()->GetList()) {
							if (Type.GetCategoryID() == Cat.GetID()) {
								auto& Types = Counter.at(Type.GetID());
								for (auto& c : Types) {
									auto* ptr = ItemData::Instance()->FindPtr(c.m_ID);
									if (ptr) {
										if (ypos - ysizeAdd < yp && yp < ypMax) {
											if (ypos - 1 < yp && yp < ypMax - ysizeAdd + 1) {
												DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);
												ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), c.isFir, false, false);
												//�T�C�h�`��
												int xp2 = xp + xsizeAdd + y_r(10);
												int yp2 = yp;
												int xsize2 = ysize / 2;
												int ysize2 = ysize / 2 - y_r(3);
												//�n�C�h�A�E�g�N���t�g
												if (m_IsCheckCraft) {
													for (const auto&L : HideoutData::Instance()->GetList()) {
														for (const auto& Ld : L.GetLvData()) {
															int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;
															/*
															bool IsChecktask = true;
															if (PlayerData::Instance()->GetHideoutClear(L.GetName().c_str(), (int)(&Ld - &L.GetLvData().front()) + 1)) {
																IsChecktask = false;
															}
															if (!IsChecktask) { continue; }
															//*/
															for (const auto& C : Ld.m_ItemCraft) {
																for (const auto& R : C.m_ItemReward) {
																	int craftcount = std::max(1, c.count / R.GetValue());
																	if (R.GetID() == c.m_ID) {
																		yp2 = yp;
																		std::string NameTmp = L.GetName();
																		NameTmp += " Lv" + std::to_string(Lv);
																		NameTmp += "x" + std::to_string(craftcount);
																		auto xl = xp2 + WindowSystem::SetMsg(xp2, yp2, xp2, yp2 + ysize2, ysize2, STRX_LEFT, White, Black, NameTmp);
																		yp2 += ysize2 + y_r(5);

																		for (const auto& w : C.m_ItemReq) {
																			auto ID = w.GetID();
																			auto* ptr2 = ItemData::Instance()->FindPtr(ID);
																			if (ptr2) {
																				int count = w.GetValue()*craftcount;
																				xp2 += ptr2->Draw(xp2, yp2, xsize2, ysize2, (count >= 2) ? count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, true) + y_r(5);
																			}
																		}
																		xp2 = std::max(xl, xp2);
																		xp2 += y_r(10);
																		break;
																	}
																}
															}
														}
													}
												}
												//����
												if (m_IsCheckBarter && !c.isFir) {
													for (auto&L : TraderData::Instance()->SetList()) {
														for (const auto& Ld : L.GetLvData()) {
															int Lv = (int)(&Ld - &L.GetLvData().front()) + 1;

															for (const auto& cf : Ld.m_ItemBarters) {
																for (const auto& I : cf.m_ItemReward) {
																	int craftcount = std::max(1, c.count / I.GetValue());
																	if (I.GetID() == c.m_ID) {
																		yp2 = yp;
																		std::string NameTmp = L.GetName();
																		NameTmp += " Lv" + std::to_string(Lv);
																		NameTmp += "x" + std::to_string(craftcount);
																		auto xl = xp2 + WindowSystem::SetMsg(xp2, yp2, xp2, yp2 + ysize2, ysize2, STRX_LEFT, White, Black, NameTmp);
																		yp2 += ysize2 + y_r(5);

																		for (const auto& r : cf.m_ItemReq) {
																			auto* ptr2 = ItemData::Instance()->FindPtr(r.GetID());
																			if (ptr2) {
																				int count = r.GetValue()*craftcount;
																				xp2 += ptr2->Draw(xp2, yp2, xsize2, ysize2, (count >= 2) ? count : 0, Gray15, !WindowMngr->PosHitCheck(nullptr), false, false, true) + y_r(5);
																			}
																		}
																		xp2 = std::max(xl, xp2);
																		xp2 += y_r(10);
																		break;
																	}
																}
															}
															//�N���t�g�x�[�X
														}
													}
												}
											}
											else {
												if (yp <= ypos) {
													DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (ypos - yp) / ysizeAdd, 0, 255));
												}
												else {
													DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255 - std::clamp(255 * (yp - (ypMax - ysizeAdd)) / ysizeAdd, 0, 255));
												}
												ptr->Draw(xp, yp, xsize, ysize, (c.count >= 2) ? c.count : 0, Gray15, false, c.isFir, false, false);
											}
										}
										yp += ysizeAdd;
										IsHit = true;
									}
								}
							}
						}
						if (IsHit) {
							yp += y_r(10);
						}
					}
					DrawControl::Instance()->SetAlpha(DrawLayer::Normal, 255);

					int ScrPosX = xpos + xsizeAdd;
					int ScrSizY = ypMax - ypos;
					m_Scroll.ScrollBox(xpos, ypos, ScrPosX, ypos + ScrSizY, (float)std::max(yp- ypBase, ScrSizY) / (float)ScrSizY, true);

					m_YNow = std::max(0.f, m_Scroll.GetNowScrollYPer()*(float)((yp - ypBase) - ScrSizY));
				}
			}
		}
		void DrawFront_Sub(int, int, float) noexcept override {
			//auto* WindowMngr = WindowSystem::WindowManager::Instance();
			//auto* Input = InputControl::Instance();

			//�n�C�h�A�E�g
			{
				//�n�C�h�A�E�g�݂̂��m�F
				{
					int xp = y_r(1920) - y_r(48) - y_r(200);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 2 - y_r(5) - y_r(40) * 2;
					WindowSystem::CheckBox(xp, yp, &m_IsCheckOpenCraft);
					auto size = WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "�n�C�h�A�E�g�J���A�C�e����\��:");
					DrawControl::Instance()->SetDrawLine(DrawLayer::Normal, xp - size, yp + LineHeight, xp + y_r(64), yp + LineHeight, Gray25, y_r(4));
				}
				//���J���\�ȃn�C�h�A�E�g�Ώۂ̂ݕ\��
				{
					int xp = y_r(1920) - y_r(48) - y_r(200);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 2 - y_r(5) - y_r(40) * 1;
					WindowSystem::CheckBox(xp, yp, &m_DrawCanClearHideout);
					WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, m_IsCheckOpenCraft ? White : Gray50, Black, "�J���\�Ώۂ̂�");
				}
			}
			//�^�X�N
			{
				//�^�X�N�݂̂��m�F
				{
					int xp = y_r(1920) - y_r(48) - y_r(200);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 2;
					WindowSystem::CheckBox(xp, yp, &m_IsCheckTask);
					auto size = WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, White, Black, "�^�X�N�[�i/�K�v�A�C�e����\��:");
					DrawControl::Instance()->SetDrawLine(DrawLayer::Normal, xp - size, yp + LineHeight, xp + y_r(64), yp + LineHeight, Gray25, y_r(4));
				}
				//���N���A�\�ȃ^�X�N�Ώۂ̂ݕ\��
				{
					int xp = y_r(1920) - y_r(48) - y_r(200);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 1;
					WindowSystem::CheckBox(xp, yp, &m_DrawCanClearTask);
					WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, m_IsCheckTask ? White : Gray50, Black, "�N���A�\�Ώۂ̂�");
				}
				//�^�X�N�ɕK�v�ȃA�C�e��(�T�v���b�T�[�Ȃ�)��ǉ�
				{
					int xp = y_r(1920) - y_r(48) - y_r(200);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 0;
					WindowSystem::CheckBox(xp, yp, &m_IsNeedItem);
					WindowSystem::SetMsg(xp, yp, xp, yp + LineHeight, LineHeight, STRX_RIGHT, m_IsCheckTask ? White : Gray50, Black, "�K�v�i(���Ȃ�)��ǉ�");
				}
			}
			//�T�C�h�\��
			{
				//�n�C�h�A�E�g�N���t�g���m�F
				{
					int xp = y_r(48);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 4;
					WindowSystem::CheckBox(xp, yp, &m_IsCheckCraft);
					WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "�N���t�g���T�C�h�\��");
				}
				//�g���[�_�[�������m�F
				{
					int xp = y_r(48);
					int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 3;
					WindowSystem::CheckBox(xp, yp, &m_IsCheckBarter);
					WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "�g���[�_�[�������T�C�h�\��");
				}
			}

			//���C�g�L�[�p�[�ɕK�v��
			{
				auto tmp = PlayerData::Instance()->GetIsNeedLightKeeper();
				int xp = y_r(48);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 2;
				WindowSystem::CheckBox(xp, yp, &tmp);
				WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "���C�g�L�[�p�[�J���܂łɍi��");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedKappa(false);
				}
				PlayerData::Instance()->SetIsNeedLightKeeper(tmp);
			}
			//�J�b�p�ɕK�v��
			{
				auto tmp = PlayerData::Instance()->GetIsNeedKappa();
				int xp = y_r(48);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 1;
				WindowSystem::CheckBox(xp, yp, &tmp);
				WindowSystem::SetMsg(xp + y_r(64), yp, xp + y_r(64), yp + LineHeight, LineHeight, STRX_LEFT, White, Black, "�J�b�p�[�J���܂łɍi��");
				if (tmp) {
					PlayerData::Instance()->SetIsNeedLightKeeper(false);
				}
				PlayerData::Instance()->SetIsNeedKappa(tmp);
			}
			//���x������
			{
				int xp = y_r(5);
				int yp = y_r(1080) - y_r(48) - y_r(5) - y_r(40) * 0;
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(100), yp + y_r(48), true, true, Red, "DOWN")) {
					PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() - 1);
				}
				xp += y_r(105);
				if (WindowSystem::ClickCheckBox(xp, yp, xp + y_r(100), yp + y_r(48), true, true, Green, "UP")) {
					PlayerData::Instance()->SetMaxLevel(PlayerData::Instance()->GetMaxLevel() + 1);
				}
				PlayerData::Instance()->SetMaxLevel(std::clamp(PlayerData::Instance()->GetMaxLevel(), 1, 71));
				xp += y_r(105);
				WindowSystem::SetMsg(xp, yp + y_r(12), xp, yp + y_r(12) + y_r(36), y_r(36), STRX_LEFT, White, Black, "MaxLevel");
				xp += y_r(250);
				WindowSystem::SetMsg(xp, yp, xp, yp + y_r(48), y_r(48), STRX_RIGHT, White, Black, "%d", PlayerData::Instance()->GetMaxLevel());
			}

			//
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