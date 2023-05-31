#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	class InterruptParts : public SingletonBase<InterruptParts> {
	private:
		friend class SingletonBase<InterruptParts>;
	private:
		bool				m_IsActive{ false };
		BGSelect			m_NextScene{ 0 };
		std::array<int, 4>	m_InitParamSelect{ 0 };
	private:
		InterruptParts() noexcept {}
		~InterruptParts() noexcept {}
	public:
		const auto& IsActive(void) const noexcept { return this->m_IsActive; }
		const auto& GetNextScene(void) const noexcept { return this->m_NextScene; }
		void		SetInitParam(int Sel, int value) noexcept { this->m_InitParamSelect.at(Sel) = value; }
		const auto& GetInitParam(int Sel) const noexcept { return this->m_InitParamSelect.at(Sel); }
		void		GotoNext(BGSelect SceneSel) noexcept {
			m_IsActive = true;
			m_NextScene = SceneSel;
		}
		void		Complete() noexcept {
			m_IsActive = false;
		}
	};
};