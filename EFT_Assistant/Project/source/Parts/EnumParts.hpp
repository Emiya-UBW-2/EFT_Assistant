#pragma once
#include "../Header.hpp"

namespace FPS_n2 {
	//
	extern EnumTaskObjective GetStringToEnumTaskObjective(std::string_view buf);
	//
	extern EnumCompareMethod GetStringToEnumCompareMethod(std::string_view buf);
	extern std::string GetEnumCompareMethodToString(EnumCompareMethod buf);
	//
	extern EnumItemProperties GetStringToEnumItemProperties(std::string_view buf);
	//
};