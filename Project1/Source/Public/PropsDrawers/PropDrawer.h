#pragma once

#include "Core.h"
#include "sol/sol.hpp"

struct FPropData {
	BString PropName;

};

class FPropDrawer {
	public:
	static void DrawProp(BString PropKey, sol::table& PropTable);

	static void DrawStringProp(BString PropKey, sol::table& StringTable);
	static void DrawFloatProp(BString PropKey, sol::table& FloatTable);
	static void DrawIntProp(BString PropKey, sol::table& IntTable);
	static void DrawVectorProp(BString PropKey, sol::table& VectorTable);
	static void DrawObjectProp(BString PropKey, sol::table& FloatTable);
};