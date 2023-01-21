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
		std::vector<float>		m_TraderRep;
	private:
		TraderData() noexcept {
			SetList("data/trader/");
			m_TraderRep.resize(m_List.size());
		}
		~TraderData() noexcept {
		}
	public:
		void			ResetRep() noexcept {
			if (true) {
				for (auto&t : this->m_TraderRep) {
					t = 0.f;
				}
			}
			else {
				for (auto&t : this->m_TraderRep) {
					t = 0.2f;
				}
			}
		}
		float*			FindTraderRep(TraderID ID) noexcept {
			for (const auto&t : m_List) {
				if (t.GetID() == ID) {
					return &m_TraderRep[&t - &m_List.front()];
				}
			}
			return nullptr;
		}
	};
};