#pragma once

#include "DXLib_ref/DXLib_ref.h"

using namespace DXLib_ref;

#define DIV_STR "/|"

#define NOMINMAX
#include <chrono>
#include <WinUser.h>

//FPS(‹¤’Ê)
inline float FPS{ 60.f };


//Parts
#include "Enums.hpp"
#include "HttpReq.hpp"
//
#include "Scene/MainSceneLoader.hpp"//Load
#include "Scene/MainScene.hpp"//Main
