#pragma once
#include "MainScene_PageParents.hpp"

namespace FPS_n2 {
	class PageManager : public SingletonBase<PageManager> {
	private:
		friend class SingletonBase<PageManager>;
	private:
		std::shared_ptr<PageParent>									m_NowOpenPagePtr;
		std::array<std::shared_ptr<PageParent>,(int)BGSelect::Max>	m_PagePtr;
		int															m_posx{ 0 };
		int															m_posy{ 0 };
		float														m_Scale{ 0.6f };
		bool														m_IsGoNextPage{ false };

		bool					m_ScaleActive{ true };

	private:
		PageManager(void)noexcept;
		~PageManager(void)noexcept;
	public:
		auto& GetPage(BGSelect value) noexcept { return this->m_PagePtr.at((int)value); }
		void TurnOnGoNextPage() noexcept { this->m_IsGoNextPage = true; }
		void ChangePage(BGSelect Select) noexcept;

		const auto&		GetScaleActive(void) const noexcept { return this->m_ScaleActive; }
		void			SetScaleActive(bool value) noexcept { this->m_ScaleActive = value; }
	public:
		void Init() noexcept;
		void FirstExecute() noexcept;
		void LateExecute() noexcept;
		void Draw_Back() noexcept;
		void DrawFront() noexcept;
		void Dispose(void) noexcept;
	};
};
