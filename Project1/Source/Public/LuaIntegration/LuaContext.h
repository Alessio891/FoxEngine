#pragma once

#include "Core.h"
#include "sol/sol.hpp"
#include "LuaObjects.h"
#include "SceneObject.h"
#include <InputSystem.h>

typedef sol::table_proxy<sol::basic_table_core<false, sol::reference>&, sol::detail::proxy_key_t<const char(&)[6]>> LuaProxyValue;

class FLuaContext {
public:
	FLuaContext() {
		luaState.open_libraries(sol::lib::base, sol::lib::table);
		
		luaState.new_usertype<LLuaSceneObjectComponent>(
			"SceneObjectComponent", sol::constructors<LLuaSceneObjectComponent()>(),
			"Type", &LLuaSceneObjectComponent::Type,
			"Tick", &LLuaSceneObjectComponent::Tick,
			"Begin", &LLuaSceneObjectComponent::Begin,
			"End", &LLuaSceneObjectComponent::End,
			"Object", &LLuaSceneObjectComponent::Object,
			"Data", &LLuaSceneObjectComponent::Data
		);
		luaState.new_usertype<Vector3F>(
			"Vector3F", sol::constructors<Vector3F()>(),
			"x", &Vector3F::x,
			"y", &Vector3F::y,
			"z", &Vector3F::z
		);

		luaState.new_usertype<FTransform>(
			"Transform", sol::constructors<FTransform()>(),
			"Position", &FTransform::Position,
			"Rotation", &FTransform::Rotation,
			"Scale", &FTransform::Scale
		);

		luaState.new_usertype<FSceneObject>(
			"SceneObject",
			"Transform", &FSceneObject::Transform,
			"Name", &FSceneObject::Name
		);

		luaState.set_function("IsKeyDown", &FInputSystem::IsKeyDown);


		BeginLuaContext();
	}

	virtual void BeginLuaContext();

	sol::state& GetState() { return luaState; }
protected:
	sol::state luaState;
};