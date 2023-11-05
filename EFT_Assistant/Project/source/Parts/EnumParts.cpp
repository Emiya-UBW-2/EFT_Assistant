#include "EnumParts.hpp"
#include "../PartsHeader.hpp"

namespace FPS_n2 {
	//
	extern EnumTaskObjective GetStringToEnumTaskObjective(std::string_view buf) {
		for (int i = 0; i < sizeof(TypesStr) / sizeof(TypesStr[0]); i++) {
			if (buf == TypesStr[i]) {
				return (EnumTaskObjective)i;
			}
		}
		return EnumTaskObjective::Max;
	}
	//
	extern EnumCompareMethod GetStringToEnumCompareMethod(std::string_view buf) {
		for (int i = 0; i < sizeof(CompareMethodStr) / sizeof(CompareMethodStr[0]); i++) {
			if (buf == CompareMethodStr[i]) {
				return (EnumCompareMethod)i;
			}
		}
		return EnumCompareMethod::Max;
	}
	extern std::string GetEnumCompareMethodToString(EnumCompareMethod buf) {
		return CompareMethodStr[(int)buf];
	}
	//
	extern EnumItemProperties GetStringToEnumItemProperties(std::string_view buf) {
		for (int i = 0; i < (int)EnumItemProperties::Max; i++) {
			if (buf == ItemPropertiesStr[i]) {
				return (EnumItemProperties)i;
			}
		}
		return EnumItemProperties::Max;
	}
	//
};
