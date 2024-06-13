#include "LuaIntegration/LuaObjectComponent.h"
#include "sol/sol.hpp"
#include "Application.h"
#include "LuaIntegration/LuaContext.h"
void FLuaObjectComponent::Begin()
{
	
	if (ScriptAsset.IsValid()) {
		BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
		sol::state& ctx = FApplication::Get()->GetLuaContext().lock()->GetState();
		ctx[scriptName]["Object"] = SharedPtr<FSceneObject>(Owner);
		ScriptAsset.Get()->GetLuaObject().Begin();
	}
}

void FLuaObjectComponent::End()
{
	if (ScriptAsset.IsValid()) {
		ScriptAsset.Get()->GetLuaObject().End();
	}
}

void FLuaObjectComponent::Tick(float Delta)
{
	if (ScriptAsset.IsValid()) {
		/*BString scriptName = ScriptAsset.Get()->GetOnlyFileName(false);
		sol::state& ctx = FApplication::Get()->GetLuaContext().lock()->GetState();
		if (!ctx[scriptName]["Object"].valid())
		{
			ctx[scriptName]["Object"] = SharedPtr<FSceneObject>(Owner);
		}
		*/
		auto o = ScriptAsset.Get()->GetLuaObject();
		o.Tick(12, double(Delta));
	}
}

void FLuaObjectComponent::DrawInspector()
{
	ImGui::Text("LuaScript");
	if (ScriptAsset.IsValid()) {
		auto ctx = FApplication::Get()->GetLuaContext().lock();
		auto script = ctx->GetState()[ScriptAsset.Get()->GetOnlyFileName(false)];
		sol::table state = script["Data"];
		int s = state.size();
		if (state.valid()) {
			for (const auto& kvp : state) {
				sol::table v = kvp.second;
				auto type = v["Type"];
				auto name = v["Name"];
				auto value = v["Value"];
				
				if (!name.valid()) name = kvp.first;
				BString _name = name.get<BString>();
				auto propType = value.get_type();
				if (propType == sol::type::nil) {
					ImGui::Text("Error with property %d", name.get<BString>().c_str());
				}
				else if (propType == sol::type::string) {
					char* buf = new char[100];
					strcpy(buf, value.get<BString>().c_str());
					ImGui::InputText(_name.c_str(), buf, 100);
					v["Value"] = std::string(buf);
					free(buf);
				}
				else if (propType == sol::type::number) {
					if (value.is<int>()) {
						int val = value.get<int>();
						ImGui::InputInt(_name.c_str(), &val);
						v["Value"] = val;
					}
					else {
						float val = value.get<float>();
						auto speed = v.get_or<float, BString, float>("FloatSpeed", 0.5f);
						ImGui::DragFloat(_name.c_str(), &val, speed);
						v["Value"] = val;
					}
				}
			}
		}
	}
}
