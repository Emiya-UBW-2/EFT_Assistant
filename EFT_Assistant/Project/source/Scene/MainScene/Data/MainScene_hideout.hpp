#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	//
	struct HideoutLvData {
		struct CraftData {
			int											durationTime{ 0 };
			std::vector<ItemGetData>					m_ItemReq;
			std::vector<ItemGetData>					m_ItemReward;
		};
		//開放データ
		int											constructionTime{ 0 };
		std::string									Information_Eng;
		std::vector<ItemGetData>					m_ItemReq;
		std::vector<TraderGetData>					m_TraderReq;
		std::vector<HideoutGetData>					m_Parent;
		std::vector<HideoutGetData>					m_Child;
		//クラフトデータ
		std::vector<CraftData>						m_ItemCraft;
	};
	//
	class HideoutList : public ListParent<HideoutID> {
		std::vector<HideoutLvData> m_LvData;
		int m_DrawWindowLv{ 1 };
	private:
		//追加設定
		void	SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override;
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		int										m_CheckJson{ 0 };
	public:
		const auto&	GetLvData() const noexcept { return m_LvData; }
	public:
		const int		Draw(int xp, int yp, int xsize, int ysize, int count, unsigned int defaultcolor, bool Clickactive) noexcept;
		void			DrawUnlockWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs = nullptr, int* ys = nullptr) const noexcept;
		void			DrawCraftWindow(WindowSystem::WindowControl* window, unsigned int defaultcolor, int Lv, int xp, int yp, int *xs = nullptr, int* ys = nullptr, int size = 10) noexcept;
		void			SetOtherPartsID() noexcept;
	};
	//
	class HideoutJsonData :public JsonDataParent {
	public:
		std::vector<HideoutLvData>							m_LvData;
	public:
		void GetJsonSub(const nlohmann::json& data) noexcept override;
		void OutputDataSub(std::ofstream& outputfile) noexcept override;
	};
	//
	class HideoutData : public SingletonBase<HideoutData>, public DataParent<HideoutID, HideoutList>, public JsonListParent<HideoutJsonData> {
	private:
		friend class SingletonBase<HideoutData>;
	private:
		HideoutData() noexcept {
			std::string Path = "data/Hideout/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				std::string Path2 = Path + RetPath2 + "/";
				GetDirList(Path2.c_str(), [&](const char* RetPath3) {
					SetDirList((Path2 + RetPath3 + "/").c_str());
				});
			});
			for (auto& t : m_List) {
				t.m_CheckJson = 0;
			}
		}
		~HideoutData() noexcept {}
	public:
		void UpdateData() noexcept {
			for (auto& L : m_List) {
				for (auto& jd : GetJsonDataList()) {
					if (L.GetIDstr() == jd->m_id) {
						L.m_CheckJson++;

						jd->OutputData(L.GetFilePath());
						break;
					}
				}
			}
		}
		void CheckThroughJson(void) noexcept {
			for (auto& L : m_List) {
				if (L.m_CheckJson == 0) {
					std::string ErrMes = "Error : ThroughJson : " + L.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
				else if (L.m_CheckJson >= 2) {
					std::string ErrMes = "Error : Be repeated " + std::to_string(L.m_CheckJson) + " : " + L.GetName();
					DataErrorLog::Instance()->AddLog(ErrMes.c_str());
				}
			}
		}
	};
	//
};