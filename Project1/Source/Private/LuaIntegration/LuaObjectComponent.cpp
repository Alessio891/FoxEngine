#include "LuaIntegration/LuaObjectComponent.h"
#include "sol/sol.hpp"
#include "Application.h"
#include "LuaIntegration/LuaContext.h"
#include "SceneObject.h"
#include <AssetsLibrary/ScriptAsset.h>
#include "LuaIntegration/LuaObjects.h"
#include "GUI/GUI.h"

void FLuaObjectComponent::SetupLuaScript()
{
	if (LuaComponent == nullptr)
		LuaComponent = ScriptAsset.Get()->GetNewInstance();
	if (LuaComponent == nullptr) return;

	BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
	sol::environment& ctx = LuaComponent->env;
	ctx["Object"] = SharedPtr<FSceneObject>(Owner);
	ctx["Begin"]();
}

void FLuaObjectComponent::Begin()
{
	unique_id = std::rand();
	if (ScriptAsset.IsValid()) {
		ScriptAsset.Get()->RegisterOnRecompileCallback([this]() {
			OnRecompiled();
			});
		LuaComponent = ScriptAsset.Get()->GetNewInstance();
		if (LuaComponent == nullptr) return;

		SetupLuaScript();
	}
}

void FLuaObjectComponent::End()
{
	if (LuaComponent == nullptr) return;
	if (ScriptAsset.IsValid()) {
		BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
		LuaComponent->env["End"]();
	}
}

void FLuaObjectComponent::Tick(float Delta)
{
	if (!CanTick) {
		CanTick = true;
		return;
	}
	if (LuaComponent == nullptr) return;
	if (ScriptAsset.IsValid()) {
		BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
		LuaComponent->env["Tick"](double(Delta));
	}
}

void FLuaObjectComponent::DrawInspector()
{
	if (LuaComponent == nullptr) return;
	ImGui::PushID("#lua_script_props_" + unique_id);
	if (ScriptAsset.IsValid()) {
		ImGui::SeparatorText(ScriptAsset.Get()->GetOnlyFileName(false).c_str());
		//auto ctx = FApplication::Get()->GetLuaContext().lock();
		auto script = LuaComponent->env;
		sol::table state = script["Data"];
		int s = state.size();
		if (state.valid()) {
			for (const auto& kvp : state) {
				sol::table v = kvp.second;
				auto type = v["Type"].get<BString>();
				auto name = v["Name"];
				auto value = v["Value"];

				if (!name.valid()) name = kvp.first;
				BString _name = name.get<BString>();
				auto propType = value.get_type();
				if (type == "string") {
					char* buf = new char[100];
					strcpy(buf, value.get<BString>().c_str());
					ImGui::InputText(_name.c_str(), buf, 100);
					v["Value"] = std::string(buf);
					free(buf);
				}
				else if (type == "int") {

					int val = value.get<int>();
					ImGui::InputInt(_name.c_str(), &val);
					v["Value"] = val;
				}
				else if (type == "float") {
					float val = value.get<float>();
					auto speed = v.get_or<float, BString, float>("FloatSpeed", 0.5f);
					auto min = v.get_or<float, BString, float>("Min", 0.0f);
					auto max = v.get_or<float, BString, float>("Max", 0.0f);
					ImGui::DragFloat(_name.c_str(), &val, speed, min, max);
					v["Value"] = val;

					script[kvp.first.as<BString>()] = val;
				}
				else if (type == "Object") {
					SharedPtr<FSceneObject> val = v.get_or<SharedPtr<FSceneObject>, BString, SharedPtr<FSceneObject>>("Value", SharedPtr<FSceneObject>());
					FGUI::ObjectReference(_name.c_str(), val);
					v["Value"] = val;
				}
				ImGui::Separator();
			}
		}
	}
	ImGui::PopID();
}

void FLuaObjectComponent::OnRecompiled()
{
	BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
	sol::environment& ctx = LuaComponent->env;
	//ctx[scriptName]["Object"] = SharedPtr<FSceneObject>(Owner);
	if (LuaComponent == nullptr) SetupLuaScript();
}

void FLuaObjectComponent::OnDrawGUI(float Delta)
{
	if (LuaComponent == nullptr) return;
	if (ScriptAsset.IsValid()) {
		BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
		try {
			LuaComponent->env["OnDrawGUI"](double(Delta));
		}
		catch (std::exception& e) {
			FLogger::LogError("Error while drawing gui: " + BString(e.what()));
		}
	}
}

void FLuaObjectComponent::Deserialize(json json)
{
	BString script;
	json["Script"].get_to(script);

	SharedPtr<FLuaScriptAsset> asset = FAssetsLibrary::GetResourceAs<FLuaScriptAsset>(script);
	if (asset != nullptr) {
		ScriptAsset.Set(asset);
		SetupLuaScript();
	}
}
