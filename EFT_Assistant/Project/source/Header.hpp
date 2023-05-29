#pragma once

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG
//#define DEBUG

#include "DXLib_ref/DXLib_ref.h"

using namespace DXLib_ref;

#define DIV_STR "/|"

#define NOMINMAX
#include <chrono>
#include <WinUser.h>

#include "Enums.hpp"
#include "HttpReq.hpp"
//Load
#include "Scene/MainSceneLoader.hpp"
//Parts
#include "Parts/StrControl.hpp"
#include "Parts/DrawSystem.hpp"
#include "Parts/InputParts.hpp"
#include "Parts/WindowParts.hpp"
#include "Parts/PlayerDataParts.hpp"
#include "Parts/InterruptParts.hpp"
//Datas
#include "Scene/MainScene/Data/MainScene_Base.hpp"

//Pages
#include "Scene/MainScene/Page/MainScene_PageParents.hpp"
#include "Scene/MainScene/Page/MainScene_PageTitle.hpp"
#include "Scene/MainScene/Page/MainScene_PageTask.hpp"
#include "Scene/MainScene/Page/MainScene_PageHideout.hpp"
#include "Scene/MainScene/Page/MainScene_PageItem.hpp"
#include "Scene/MainScene/Page/MainScene_PageMap.hpp"
#include "Scene/MainScene/Page/MainScene_PageCustom.hpp"
#include "Scene/MainScene/Page/MainScene_PageTrader.hpp"
#include "Scene/MainScene/Page/MainScene_PageItemList.hpp"
#include "Scene/MainScene/Page/MainScene_PagePlayerInfo.hpp"
//Main
#include "Scene/MainScene.hpp"
