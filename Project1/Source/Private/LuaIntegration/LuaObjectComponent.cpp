#include "LuaIntegration/LuaObjectComponent.h"
#include "sol/sol.hpp"
#include "Application.h"
#include "LuaIntegration/LuaContext.h"
#include "SceneObject.h"
#include <AssetsLibrary/ScriptAsset.h>
#include "LuaIntegration/LuaObjects.h"
#include "GUI/GUI.h"
#include "PropsDrawers/PropDrawer.h"

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
		if (ImGui::BeginPopupContextItem("Actions")) {
			if (ImGui::Selectable("Remove Component")) {
				Owner->RemoveComponent(this);
			}
			ImGui::EndPopup();
		}

		//auto ctx = FApplication::Get()->GetLuaContext().lock();
		auto script = LuaComponent->env;
		sol::table state = script.get_or<sol::table, BString, sol::table>("Data", sol::nil);
		if (state != sol::nil) {
			for (const auto& kvp : state) {
				sol::table v = kvp.second;
				BString key = kvp.first.as<BString>();
				BString type = v.get_or<BString, BString, BString>("Type", "none");
				BString name = v.get_or<BString, BString, BString>("Name", key.c_str());

				FPropDrawer::DrawProp(name, v);
				script[key] = v["Value"];
				/*
				LuaProxyValue value = v["Value"];

				auto propType = value.get_type();
				if (type == "string") {
					char* buf = new char[100];
					strcpy(buf, value.get<BString>().c_str());
					ImGui::InputText(name.c_str(), buf, 100);
					v["Value"] = std::string(buf);
					free(buf);
				}
				else if (type == "int") {

					int val = value.get<int>();
					ImGui::InputInt(name.c_str(), &val);
					v["Value"] = val;
				}
				else if (type == "float") {
					float val = value.get<float>();
					auto speed = v.get_or<float, BString, float>("FloatSpeed", 0.5f);
					auto min = v.get_or<float, BString, float>("Min", 0.0f);
					auto max = v.get_or<float, BString, float>("Max", 0.0f);
					ImGui::DragFloat(name.c_str(), &val, speed, min, max);
					v["Value"] = val;

					script[kvp.first.as<BString>()] = val;
				}
				else if (type == "Object") {
					SharedPtr<FSceneObject> val = v.get_or<SharedPtr<FSceneObject>, BString, SharedPtr<FSceneObject>>("Value", SharedPtr<FSceneObject>());
					FGUI::ObjectReference(name.c_str(), val);
					v["Value"] = val;
				}*/
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
