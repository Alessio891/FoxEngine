#pragma once

#include "Core.h"
#include "AssetResource.h"
#include <Editor/AssetsLibrary.h>
#include "sol/sol.hpp"
#include "LuaIntegration/LuaObjects.h"
#include "LuaIntegration/LuaContext.h"

class FLuaScriptAsset : public FAssetResource {

public:
	FLuaScriptAsset(BString path) : FAssetResource(EAssetResourceType::Script, path) {
		auto ctx = FApplication::Get()->GetLuaContext().lock();
		ctx->GetState()[GetOnlyFileName(false)] = LuaObject;
		auto result = ctx->GetState().safe_script_file(path, [this](lua_State*, sol::protected_function_result pfr) {
			sol::error err = pfr;
			FLogger::LogError("Lua Compiler Error: " + std::string(err.what()));
			return pfr;
		});
	}

	virtual ImTextureID GetThumbnailIcon() override {
		auto texture = FAssetsLibrary::GetImage("Resources/Images/GUI/script.png");
		ImTextureID id = (void*)(intptr_t)texture->GetTextureID(FApplication::Get()->EditorGUIViewport->ViewportContext);
		texture.reset();
		return id;
	};
	LLuaSceneObjectComponent GetLuaObject() {
		auto ctx = FApplication::Get()->GetLuaContext().lock();
		return ctx->GetState().get<LLuaSceneObjectComponent>(GetOnlyFileName(false));
	};

	virtual void DrawInspector() override {
		if (ImGui::Button("Recompile")) {
			auto ctx = FApplication::Get()->GetLuaContext().lock();
			auto luaObj = ctx->GetState()[GetOnlyFileName(false)];
			auto o = luaObj["Object"];

			SharedPtr<FSceneObject> object;
			if (o.valid())
				object = o.get<SharedPtr<FSceneObject>>();
			

			LuaObject = LLuaSceneObjectComponent();
			ctx->GetState()[GetOnlyFileName(false)] = LuaObject;
			auto result = ctx->GetState().safe_script_file(FilePath, [this, ctx, object](lua_State*, sol::protected_function_result pfr) {
				sol::error err = pfr;
				FLogger::LogError("Lua Compiler Error: " + std::string(err.what()));
				if (object != nullptr)
					ctx->GetState()[GetOnlyFileName(false)]["Object"] = object;
				return pfr;
			});
			if (result.valid()) {
				FLogger::LogInfo("Compiled file " + GetOnlyFileName());
			}
		}
	};

	LLuaSceneObjectComponent LuaObject;

};