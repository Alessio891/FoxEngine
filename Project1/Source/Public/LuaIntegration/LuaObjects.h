#pragma once

#include "Core.h"
#include "Logger.h"

class LLuaSceneObjectComponent {

public:
	~LLuaSceneObjectComponent() {}

	BString Type = "Test";
	int ScriptType = 0;
	SharedPtr<class FSceneObject> Object;
	std::function<void(int, const double&)> Tick = [](int o, const double& D) { };
	std::function<void(void)> Begin = []() {};
	std::function<void(void)> End = []() {};
	sol::table Data = sol::table();

	sol::environment env;


};