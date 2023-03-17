#pragma once
#include"../Header.hpp"

namespace FPS_n2 {

	class MAINLOOP : public TEMPSCENE {
	private:
		std::vector<std::pair<std::string, int>>	sellFor;
		bool isFirst = true;
	public:
		void Load_Sub(void) noexcept override {}

		void Set_Sub(void) noexcept override {
			InputControl::Create();
			DrawControl::Create();
			WindowSystem::WindowManager::Create();
		}

		bool Update_Sub(void) noexcept override {
			auto* Input = InputControl::Instance();
			DrawControl::Instance()->ClearList();
			//FirstDoing
			if (GetIsFirstLoop()) {
			}
			Input->Execute();
			//
			{
				if (WindowSystem::ClickCheckBox(240 + 24, 24, 480 - 24, 270 - 24, false, isFirst, Gray15, "")) {
					sellFor.clear();
					std::string strResult;
					if (ItemDataRequest(strResult)) {
						auto data = nlohmann::json::parse(strResult);
						for (const auto& d : data["data"]["items"]) {
							for (const auto& sf : d["sellFor"]) {
								std::string vendor = sf["vendor"]["name"];
								int price = sf["price"];
								sellFor.emplace_back(std::make_pair(vendor, price));
								if (vendor == "Therapist") {
									std::string Mes = std::to_string(price) + "ルーブル";
									SetClipboardText(Mes.c_str());
								}
							}
						}
						isFirst = false;
					}
				}
				WindowSystem::SetMsg(240 + 24, 24, 480 - 24, 270 - 24, 30, FontHandle::FontXCenter::MIDDLE, White, Black, "データ取得");
			}
			int yp = 0;
			for (auto& s : sellFor) {
				DrawControl::Instance()->SetString(DrawLayer::Front,
					FontPool::FontType::Nomal_Edge, 18,
					FontHandle::FontXCenter::LEFT, FontHandle::FontYCenter::TOP, 0, yp, White, Black,
					"%s = %d", s.first.c_str(), s.second);
				yp += 18;
			}
			return true;
		}
		void Dispose_Sub(void) noexcept override {}
	public:
		void BG_Draw_Sub(void) noexcept override {}
		void DrawUI_In_Sub(void) noexcept  override {
			DrawControl::Instance()->Draw();
		}
	};
};
