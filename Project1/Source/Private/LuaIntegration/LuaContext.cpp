#include "LuaIntegration/LuaContext.h"
#include "Logger.h"

void FLuaContext::BeginLuaContext()
{
	luaState.set_function("FoxLog", [](BString msg) {
		FLogger::LogInfo("[LuaLog] " + msg);
	});
	luaState.script("FoxLog('Lua Context Started')");
}
