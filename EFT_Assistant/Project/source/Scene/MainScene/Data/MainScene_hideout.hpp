#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageManager.hpp"

namespace FPS_n2 {
	//
	struct HideoutLvData {
		struct CraftData {
			int											durationTime{ 0 };
			std::vector<ItemGetData>					m_ItemReq;
			std::vector<ItemGetData>					m_ItemReward;
			//ソート用のバッファー
			int sortbuffer{ 0 };
		};
		//開放データ
		int											constructionTime{ 0 };
		std::string									m_Information_Eng;
		std::string									m_Information_Jpn;
		std::vector<ItemGetData>					m_ItemReq;
		std::vector<TraderGetData>					m_TraderReq;
		std::vector<HideoutGetData>					m_Parent;
		std::vector<HideoutGetData>					m_Child;
		//クラフトデータ
		std::vector<CraftData>						m_ItemCraft;
	};
	//
	class HideoutList : public ListParent<HideoutID> {
		std::vector<HideoutLvData>	m_LvData;
		int							m_DrawWindowLv{ 1 };
	private:
		//追加設定
		void	SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override;
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto& GetLvData() const noexcept { return this->m_LvData; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int Lv, unsigned int defaultcolor, bool Clickactive, int count) noexcept;
		void			DrawUnlockWindow(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int* xs = nullptr, int* ys = nullptr) const noexcept;
		void			DrawCraftWindow(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int* xs = nullptr, int* ys = nullptr, int size = 10) noexcept;
		void			SetOtherPartsID() noexcept;

		const int		DrawCraft(WindowMySystem::WindowControl* window, unsigned int defaultcolor, int xp, int yp, int ysize, int Lv, int Select, bool isdrawName, bool isdrawAfter, int count) const noexcept;
	};
	//
	class HideoutJsonData :public JsonDataParent {
	public:
		std::vector<HideoutLvData>	m_LvData;
	public:
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};
	//
	class HideoutData : public DataParent<HideoutList>, public JsonListParent<HideoutJsonData> {
	public:
		HideoutData() noexcept {
			std::string Path = "data/Hideout/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				std::string Path2 = Path + RetPath2 + "/";
				GetDirList(Path2.c_str(), [&](const char* RetPath3) {
					SetDirList((Path2 + RetPath3 + "/").c_str());
					});
				});
		}
		~HideoutData() noexcept {}
	public:
	};
	//
};