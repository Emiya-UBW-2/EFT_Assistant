#pragma once
#include"../Header.hpp"

namespace FPS_n2 {



	class MAINLOOP : public TEMPSCENE {
	private:
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		float														m_Scale{ 0.6f };
		bool														m_IsPullDown{ false };
		float														m_PullDown{ 1.f };

		std::shared_ptr<BGParent>									m_BGPtr;
		std::shared_ptr<TitleBG>									m_TitleBG;
		std::shared_ptr<TaskBG>										m_TaskBG;
		std::shared_ptr<HideOutBG>									m_HideOutBG;
		std::shared_ptr<ItemBG>										m_ItemBG;
		std::shared_ptr<MapBG>										m_MapBG;
		std::shared_ptr<CustomBG>									m_CustomBG;

		bool														m_Loading{ false };

		bool														m_WindowMove{ false };

		float														m_NoneActiveTimes{ 0.f };

		int ttt = -1;
	public:
		void Load_Sub(void) noexcept override {}

		std::string strResult;

		void Set_Sub(void) noexcept override {
			//
			PlayerData::Create();
			InputControl::Create();
			DataErrorLog::Create();
			DrawControl::Create();
			InterruptParts::Create();
			WindowSystem::WindowManager::Create();
			//
			ItemData::Create();
			PresetData::Create();
			EnemyData::Create();
			TraderData::Create();
			MapData::Create();
			TaskData::Create();
			//
			SetUseASyncLoadFlag(TRUE);
			PresetData::Instance()->LoadList();
			ItemData::Instance()->LoadList();
			EnemyData::Instance()->LoadList();
			TraderData::Instance()->LoadList();
			MapData::Instance()->LoadList();
			TaskData::Instance()->LoadList();
			SetUseASyncLoadFlag(FALSE);
			m_Loading = true;
			//
			m_TitleBG = std::make_shared<TitleBG>();
			m_TaskBG = std::make_shared<TaskBG>();
			m_HideOutBG = std::make_shared<HideOutBG>();
			m_ItemBG = std::make_shared<ItemBG>();
			m_MapBG = std::make_shared<MapBG>();
			m_CustomBG = std::make_shared<CustomBG>();
			//
			m_BGPtr = m_TitleBG;
			//
			m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
		}

		bool Update_Sub(void) noexcept override {
			auto* WindowMngr = WindowSystem::WindowManager::Instance();
			auto* DrawParts = DXDraw::Instance();
			auto* Input = InputControl::Instance();
			auto* InterParts = InterruptParts::Instance();

			DrawControl::Instance()->ClearList();

			if (m_Loading) {
				PresetData::Instance()->WhenAfterLoadListCommon();
				ItemData::Instance()->WhenAfterLoadListCommon();
				EnemyData::Instance()->WhenAfterLoadListCommon();
				TraderData::Instance()->WhenAfterLoadListCommon();
				MapData::Instance()->WhenAfterLoadListCommon();
				TaskData::Instance()->WhenAfterLoadListCommon();
				if (GetASyncLoadNum() == 0) {
					PresetData::Instance()->WhenAfterLoadList();
					ItemData::Instance()->WhenAfterLoadList();
					EnemyData::Instance()->WhenAfterLoadList();
					TraderData::Instance()->WhenAfterLoadList();
					MapData::Instance()->WhenAfterLoadList();
					TaskData::Instance()->WhenAfterLoadList();
					m_Loading = false;
				}
			}
			//FirstDoing
			if (GetIsFirstLoop()) {
				//SetWindowPosition(-960, 0);//0,0
				SetWindowPosition(0, 0);//0,0
			}
			auto mouse_moveX = Input->GetMouseX();							//�h���b�O�O�̃}�E�X���W�i�[
			auto mouse_moveY = Input->GetMouseY();
			Input->Execute();
			mouse_moveX = Input->GetMouseX() - mouse_moveX;
			mouse_moveY = Input->GetMouseY() - mouse_moveY;
			//�h���b�O�J�n���̏���
			bool inMouse = in2_(Input->GetMouseX(), Input->GetMouseY(), 0, 0, y_r(1920), LineHeight);
			if (Input->GetMiddleClick().press()) {
				if (Input->GetMiddleClick().trigger()) {
					m_WindowMove = false;
					if (m_PullDown >= 1.f) {
						if (inMouse) {
							m_WindowMove = true;
						}
					}
				}
				if (m_WindowMove) {
					int start_windowX = 0, start_windowY = 0;
					GetWindowPosition(&start_windowX, &start_windowY);			//�E�B���h�E�̈ʒu���i�[
					start_windowX += mouse_moveX;
					start_windowY += mouse_moveY;
					SetWindowPosition(start_windowX, start_windowY);			//�}�E�X�ʒu�̍����Z�o���A�E�B���h�E�𓮂���
					Input->SetMouse();
					HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
					SetCursor(hCursor);
				}
			}
			//
			if (Input->GetWheelAdd() != 0) {
				auto PrevScale = this->m_Scale;
				this->m_Scale = std::clamp(this->m_Scale + (float)Input->GetWheelAdd() / 10.f, 0.1f, 2.f);
				auto ScaleChange = (this->m_Scale - PrevScale);
				if (ScaleChange != 0.f) {
					this->m_posx -= (int)((float)(Input->GetMouseX() - this->m_posx) * ScaleChange / this->m_Scale);
					this->m_posy -= (int)((float)(Input->GetMouseY() - this->m_posy) * ScaleChange / this->m_Scale);
				}
			}
			if (Input->GetRightClick().press()) {
				this->m_posx += mouse_moveX;
				this->m_posy += mouse_moveY;
				HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
				SetCursor(hCursor);
			}
			//
			WindowMngr->Execute();
			m_BGPtr->LateExecute(&this->m_posx, &this->m_posy, &this->m_Scale);
			//
			if (m_BGPtr->IsGoNextBG()) {
				WindowMngr->DeleteAll();
				m_BGPtr->Dispose();
				if (m_BGPtr == m_TitleBG) {
					switch (m_TitleBG->GetNextSelect()) {
					case BGSelect::Task:
						m_BGPtr = m_TaskBG;
						break;
					case BGSelect::HideOut:
						m_BGPtr = m_HideOutBG;
						break;
					case BGSelect::Item:
						m_BGPtr = m_ItemBG;
						break;
					case BGSelect::Map:
						m_BGPtr = m_MapBG;
						break;
					case BGSelect::Custom:
						m_BGPtr = m_CustomBG;
						break;
					default:
						m_BGPtr = m_TaskBG;
						break;
					}
				}
				else {
					m_BGPtr = m_TitleBG;
				}
				m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
			}
			//
			if (InterParts->IsActive()) {
				WindowMngr->DeleteAll();
				m_BGPtr->Dispose();
				switch ((BGSelect)InterParts->GetNextScene()) {
				case BGSelect::Task:
					m_BGPtr = m_TaskBG;
					break;
				case BGSelect::HideOut:
					m_BGPtr = m_HideOutBG;
					break;
				case BGSelect::Item:
					m_BGPtr = m_ItemBG;
					break;
				case BGSelect::Map:
					m_BGPtr = m_MapBG;
					break;
				case BGSelect::Custom:
					m_BGPtr = m_CustomBG;
					break;
				default:
					break;
				}
				m_BGPtr->Init(&this->m_posx, &this->m_posy, &this->m_Scale);
				switch ((BGSelect)InterParts->GetNextScene()) {
				case BGSelect::Task:
					break;
				case BGSelect::HideOut:
					break;
				case BGSelect::Item:
					break;
				case BGSelect::Map:
					break;
				case BGSelect::Custom:
					m_CustomBG->SetSubparam(
						InterParts->GetInitParam(0),//�A�C�e����
						InterParts->GetInitParam(1)//�v���Z�b�g��
					);
					break;
				default:
					break;
				}
				InterParts->Complete();
			}
			if (!GetWindowActiveFlag()) {
				if (m_NoneActiveTimes > 0.f) {
					m_NoneActiveTimes -= 1.f / FPS;
				}
				else {
					m_NoneActiveTimes = 0.f;
					m_IsPullDown = true;
				}
			}
			else {
				m_NoneActiveTimes = 5.f;
			}
			//SetDraw
			{
				int Xsize = DrawParts->m_DispXSize;
				int Ysize = DrawParts->m_DispYSize;

				int Xmin = y_r(320);
				int Ymin = LineHeight;

				DrawControl::Instance()->SetDrawBox(DrawLayer::BackGround, 0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), (int)(Lerp((float)Ymin, (float)Ysize, m_PullDown)), Gray75, TRUE);
				if (m_PullDown >= 1.f) {
					//Back
					m_BGPtr->Draw_Back(this->m_posx, this->m_posy, this->m_Scale);
					//�E�B���h�E
					WindowMngr->Draw();
				}
				//�^�C�g���o�b�N
				int DieCol = std::clamp((int)(Lerp(1.f, 128.f, m_NoneActiveTimes / 5.f)), 0, 255);
				WindowSystem::SetBox(0, 0, (int)(Lerp((float)Xmin, (float)Xsize, m_PullDown)), LineHeight, GetColor(DieCol, DieCol, DieCol));
				//�^�C�g��
				if (m_PullDown >= 1.f) {
					if (WindowSystem::ClickCheckBox(Xmin + y_r(10), 0, Xmin + y_r(10 + 220), Ymin, false, true, Gray25, "�S�������")) {
						WindowMngr->DeleteAll();
					}

					if (ttt != -1) {
						if ((GetNowCount() - ttt) > 1000 * 60 * 5) {
							ttt = -1;
						}
					}

					if (WindowSystem::ClickCheckBox(Xmin + y_r(10 + 230), 0, Xmin + y_r(10 + 450), Ymin, false, (ttt == -1), Gray25, "�A�C�e���X�V")) {
						const char* Names[] = {
							//"Weapon",
							"AssaultCarbine",
							"AssaultRifle",
							"GrenadeLauncher",
							"Handgun",
							"Machinegun",
							"MarksmanRifle",
							"Revolver",
							"SMG",
							"Shotgun",
							"SniperRifle",
							"ThrowableWeapon",
							"Knife",

							//"WeaponMod",
							//"EssentialMod",
							//"FunctionalMod",
							//"GearMod",
							"Barrel",
							"GasBlock",
							"MuzzleDevice",
							//"Flashhider",
							//"CombMuzzleDevice",
							//"Silencer",
							"Sights",
							//"Ironsight",
							//"AssaultScope",
							//"Scope",
							//"ReflexSight",
							//"CompactReflexSight",
							//"SpecialScope",
							//"NightVision",
							//"TermalVision",
							"Magazine",
							//"CylinderMagazine",
							//"SpringDrivenCylinder",
							"Handguard",
							"Flashlight",
							"CombTactDevice",
							"Foregrip",
							"AuxiliaryMod",
							"Bipod",
							"ChargingHandle",
							"Mount",
							"PistolGrip",
							"Receiver",
							"Stock",
							"UBGL",
							//���[�g�i
							//"Item",
							//"CompoundItem",
							//"StackableItem",
							//"SearchableItem",
							//"RepairKits",
							//"Money",
							//"PortableRangeFinder",
							//"RadioTransmitter",
							//"BarterItem",
							//"Fuel",
							//"Compass",
							"BuildingMaterial",
							"Battery",
							"Electronics",
							"HouseholdGoods",
							"Info",
							"Jewelry",
							"Lubricant",
							"Map",
							"SpecialItem",
							"Other",
							"Tool",
							"MedicalSupplies",
							//��
							"Equipment",
							//"ArmBand",
							//"Armor",
							//"Headwear",
							//"Headphones",
							//"ArmoredEquipment",
							//"FaceCover",
							//"VisObservDevice",
							"Backpack",
							"ChestRig",
							"CommonContainer",
							//�H�ו�
							"FoodAndDrink",
							//"Food",
							//"Drink",
							//�e
							"Ammo",
							"AmmoContainer",
							//
							"Key",
							//"MechanicalKey",
							//"Keycard",
							//
							"Meds",
							//"MedicalItem",
							//"Drug",
							//"Medikit",
							//"Stimulant",
							//
							"PortContainer",
							"RandomLootContainer",
							"LockingContainer",
						};
						printfDx("�ʐM�J�n...\n");
						ScreenFlip();
						int SIZE = sizeof(Names) / sizeof(Names[0]);
						for (int i = 0; i < SIZE; i++) {
							if (ItemDataRequest(Names[i], strResult)) {
								ProcessMessage();
								auto data = nlohmann::json::parse(strResult);
								ItemData::Instance()->GetJsonData(data);
								ItemData::Instance()->SaveDatabyJson(Names[i]);
								if ((i % 5) == (5 - 1)) {
									//printfDx("Comp[%s/%s/%s/%s/%s]\n", Names[i - 4], Names[i - 3], Names[i - 2], Names[i - 1], Names[i]);
									printfDx("[%d %%]\n", 100 * (i + 1) / SIZE);
								}
								ScreenFlip();
								DxLib::WaitTimer(100);
							}
							//��t�H���_�폜
							{
								std::string Path = "data/item/Maked/";
								Path += Names[i];
								RemoveDirectory(Path.c_str());
							}
						}
						ItemData::Instance()->CheckThroughJson();

						time_t t = time(NULL);				// ���ݓ������擾����
						tm local;							// ���������i�[����ϐ���p�ӂ���
						localtime_s(&local, &t);			// ���[�J��������ϐ��Ɋi�[����
						char buffer[256];
						strftime(buffer, sizeof(buffer), "%Y %m/%d %H:%M", &local);
						PlayerData::Instance()->SetLastDataReceive(buffer);
						ttt = GetNowCount();
					}

					WindowSystem::SetMsg(0, 0, y_r(1920), LineHeight, LineHeight, STR_MID, White, Black, "EFT Assistant");
					WindowSystem::SetMsg(y_r(1280), LineHeight * 1 / 10, y_r(1280), LineHeight, LineHeight * 8 / 10, STR_LEFT, White, Black, "ver %d.%d.%d", 0, 1, 7);

					WindowSystem::SetMsg(y_r(960), LineHeight + LineHeight * 1 / 10, y_r(960), LineHeight + LineHeight, LineHeight * 8 / 10, STR_MID, White, Black, "�ŏI�X�V:%s", PlayerData::Instance()->GetLastDataReceive().c_str());

					if (WindowSystem::CloseButton(y_r(1920) - LineHeight, 0)) { SetisEnd(true); }
				}
				//�W�J
				if (WindowSystem::ClickCheckBox(0, 0, Xmin, Ymin, false, true, Gray25, !m_IsPullDown ? "�܂肽����" : "�W�J")) { m_IsPullDown ^= 1; }
				Easing(&m_PullDown, !m_IsPullDown ? 1.f : 0.f, 0.8f, EasingType::OutExpo);
				if (m_PullDown >= 0.95f) { m_PullDown = 1.f; }
				if (m_PullDown <= 0.05f) { m_PullDown = 0.f; }
				//
				if (m_PullDown >= 1.f) {
					//Front
					m_BGPtr->DrawFront(this->m_posx, this->m_posy, this->m_Scale);
					//�����ʒu����̂��߂̏��~
					DrawControl::Instance()->SetDrawCircle(DrawLayer::Front, Xsize, Ysize, y_r(100), TransColor, TRUE);
				}
				DataErrorLog::Instance()->Draw();
				if (GetASyncLoadNum() > 0) {
					WindowSystem::SetMsg(0, y_r(1080) - LineHeight, y_r(0), y_r(1080), LineHeight, STR_LEFT, White, Black, "Loading...");
				}
			}
			//SetIsUpdateDraw(false);
			return true;
		}
		void Dispose_Sub(void) noexcept override {
			m_BGPtr.reset();
			m_TaskBG.reset();
			m_HideOutBG.reset();
			m_ItemBG.reset();
			m_MapBG.reset();
			m_CustomBG.reset();

			PlayerData::Instance()->Save();
			DataErrorLog::Instance()->Save();
		}
	public:
		void BG_Draw_Sub(void) noexcept override {}

		//UI�\��
		void DrawUI_In_Sub(void) noexcept  override {
			DrawControl::Instance()->Draw();
		}
	};
};
