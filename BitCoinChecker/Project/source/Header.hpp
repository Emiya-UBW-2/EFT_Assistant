#pragma once

#define NOMINMAX

#ifdef _DEBUG

#define DEBUG

#endif // _DEBUG

//#define DEBUG

#include "DXLib_ref/DXLib_ref.h"

using namespace DXLib_ref;

#include <chrono>

#include "Enums.hpp"
#include "HttpReq.hpp"
//Parts
#include "Parts/DrawSystem.hpp"
#include "Parts/InputParts.hpp"
#include "Parts/WindowParts.hpp"
//Main
#include "Scene/MainScene.hpp"
