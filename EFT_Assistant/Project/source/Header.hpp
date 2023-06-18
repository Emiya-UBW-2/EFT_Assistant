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


//Parts
#include "Enums.hpp"
#include "HttpReq.hpp"
#include "Parts/StrControl.hpp"
#include "Parts/DrawSystem.hpp"
#include "Parts/InputParts.hpp"
#include "Parts/WindowParts.hpp"
#include "Parts/PlayerDataParts.hpp"
#include "Parts/InterruptParts.hpp"
//
#include "Scene/MainScene/Data/MainScene_Base.hpp"//Datas
#include "Scene/MainScene/Page/MainScene_PageBase.hpp"//Pages
//
#include "Scene/MainSceneLoader.hpp"//Load
#include "Scene/MainScene.hpp"//Main
