#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int TraderID;
	class TraderList : public ListParent<TraderID> {
		std::array<int, 4>		m_Lv{ 0,0,0,0 };
		std::array<float, 4>	m_Rep{ 0,0,0,0 };
	public:
		//í«â¡ê›íË
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			for (int i = 0; i < 4; i++) {
				if (i == 0) {
					m_Lv[i] = 1;
					m_Rep[i] = 0.f;
				}
				else {
					if (LEFT == "LL" + std::to_string(i + 1) + "Rep") {
						m_Rep[i] = std::stof(RIGHT);
						break;
					}
					else if (LEFT == "LL" + std::to_string(i + 1) + "Lv") {
						m_Lv[i] = std::stoi(RIGHT);
						break;
					}
				}
			}
		}
		void	Load_Sub() noexcept override {
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	};
	class TraderData : public SingletonBase<TraderData>, public DataParent<TraderID, TraderList> {
	private:
		friend class SingletonBase<TraderData>;
	private:
		TraderData() noexcept {
			SetList("data/trader/");
		}
		~TraderData() noexcept {
		}
	public:
	};

	class TraderGetData {
		TraderID			m_ID;
		int					m_Lv{ 0 };
	public:
		const auto&		GetID() const noexcept { return m_ID; }
		const auto&		GetLv() const noexcept { return m_Lv; }
		void			Set(const std::string& name, int lv) noexcept {
			m_ID = TraderData::Instance()->FindID(name.c_str());
			m_Lv = lv;
		}
	};
	void			SetTraderLv(std::vector<TraderGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto ID = mes.substr(0, L);
			if (std::find_if(Data->begin(), Data->end(), [&](const TraderGetData& obj) {return obj.GetID() == TraderData::Instance()->FindID(ID.c_str()); }) == Data->end()) {
				TraderGetData tmp;
				tmp.Set(ID, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};

};