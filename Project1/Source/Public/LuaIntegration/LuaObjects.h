#pragma once

#include "Core.h"
#include "Logger.h"
#include "SceneObject.h"
struct LLuaSceneObjectComponent {

	BString Type = "Test";
	SharedPtr<FSceneObject> Object;
	std::function<void(int, const double&)> Tick = [](int o, const double& D) { };
	std::function<void(void)> Begin = []() {};
	std::function<void(void)> End = []() {};
	sol::table Data = sol::table();



};