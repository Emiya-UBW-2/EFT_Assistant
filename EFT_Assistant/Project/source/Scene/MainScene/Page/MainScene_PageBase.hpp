#pragma once
#include"../../../Header.hpp"
//*
#include "MainScene_PageParents.hpp"
#include "MainScene_PageTitle.hpp"
#include "MainScene_PageTask.hpp"
#include "MainScene_PageHideout.hpp"
#include "MainScene_PageMap.hpp"
#include "MainScene_PageTrader.hpp"
#include "MainScene_PageItemList.hpp"
#include "MainScene_PagePlayerInfo.hpp"
//*/
namespace FPS_n2 {
	class BGBase : public SingletonBase<BGBase> {
	private:
		friend class SingletonBase<BGBase>;
	private:
		std::shared_ptr<BGParent>									m_BGPtr;
		std::shared_ptr<BGParent>									m_TitleBG;
		std::shared_ptr<BGParent>										m_TaskBG;
		std::shared_ptr<BGParent>									m_HideOutBG;
		std::shared_ptr<BGParent>										m_ItemBG;
		std::shared_ptr<BGParent>										m_MapBG;
		std::shared_ptr<BGParent>									m_CustomBG;
		std::shared_ptr<BGParent>									m_TraderBG;
		std::shared_ptr<BGParent>									m_ItemListBG;
		std::shared_ptr<BGParent>								m_PlayerInfoBG;
	private:
		BGBase(void)noexcept;
		~BGBase(void)noexcept;
	public:
		void SetNext(BGSelect value) noexcept;
	public:
		void Init(int *posx, int *posy, float* Scale) noexcept { this->m_BGPtr->Init(posx, posy, Scale); }
		void LateExecute(int *posx, int *posy, float* Scale) noexcept { this->m_BGPtr->LateExecute(posx, posy, Scale); }
		void Draw_Back(int posx, int posy, float Scale) noexcept { this->m_BGPtr->Draw_Back(posx, posy, Scale); }
		void DrawFront(int posx, int posy, float Scale) noexcept { this->m_BGPtr->DrawFront(posx, posy, Scale); }
		void Dispose(void) noexcept { this->m_BGPtr->Dispose(); }
	public:
		auto& GetNowBG(void) noexcept { return this->m_BGPtr; }
		auto& GetTitleBG(void) noexcept { return this->m_TitleBG; }
		auto& GetCustomBG(void) noexcept { return this->m_CustomBG; }
	};
};
