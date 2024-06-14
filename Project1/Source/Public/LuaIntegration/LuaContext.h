#pragma once

#include "Core.h"
#include "sol/sol.hpp"

typedef sol::table_proxy<sol::basic_table_core<false, sol::reference>&, sol::detail::proxy_key_t<const char(&)[6]>> LuaProxyValue;

enum ELuaScriptType {
	ObjectComponent = 0,
	ApplicationModule = 1
};

class FLuaContext {
public:
	FLuaContext();
	virtual void BeginLuaContext();

	void SetupKeysEnum();

	sol::state& GetState() { return luaState; }
protected:
	sol::state luaState;
};