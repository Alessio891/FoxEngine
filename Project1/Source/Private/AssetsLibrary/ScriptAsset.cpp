#include "AssetsLibrary/ScriptAsset.h"
#include "LuaIntegration/LuaObjects.h"
#include "LuaIntegration/LuaContext.h"
#include "LuaIntegration/LuaObjectComponent.h"
#include <Editor/AssetsLibrary.h>


FLuaScriptAsset::FLuaScriptAsset(BString path) : FAssetResource(EAssetResourceType::Script, path)
{

	
}

sol::protected_function_result FLuaScriptAsset::HandleCompileErrors(lua_State*, sol::protected_function_result pfr)
{
	sol::error err = pfr;
	FLogger::LogError("Lua Compiler Error: " + std::string(err.what()));
	return pfr;
}

ImTextureID FLuaScriptAsset::GetThumbnailIcon()
{
	auto texture = FAssetsLibrary::GetImage("Resources/Images/GUI/script.png");
	ImTextureID id = (void*)(intptr_t)texture->GetTextureID(FApplication::Get()->EditorGUIViewport->ViewportContext);
	texture.reset();
	return id;
}

SharedPtr<LLuaSceneObjectComponent> FLuaScriptAsset::GetNewInstance()
{
	auto ctx = FApplication::Get()->GetLuaContext().lock();
	SharedPtr<LLuaSceneObjectComponent> comp(new LLuaSceneObjectComponent());
	comp->env = sol::environment(ctx->GetState(), sol::create, ctx->GetState().globals());


	comp->env[GetOnlyFileName(false)] = comp;
	auto result = ctx->GetState().safe_script_file(FilePath, comp->env, &FLuaScriptAsset::HandleCompileErrors);
	envs.push_back(comp);
	if (result.valid()) {
		return comp;
	}
	comp->env.clear();
	return nullptr;

}

void FLuaScriptAsset::DrawInspector()
{
	if (ImGui::Button("Recompile")) {
		Recompile();
	}
}

void FLuaScriptAsset::RegisterOnRecompileCallback(OnRecompileDelegate cb)
{
	OnRecompileDelegates.push_back(cb);
}

void FLuaScriptAsset::Recompile()
{
	try {
		SharedPtr<FLuaContext> ctx = FApplication::Get()->GetLuaContext().lock();
		sol::state& luaCtx = ctx->GetState();

		for (SharedPtr<LLuaSceneObjectComponent> comp : envs) {
			sol::environment& env = comp->env;

			if (!env.valid()) {
				env = sol::environment(ctx->GetState(), sol::create, ctx->GetState().globals());
			}

			BString scriptName = GetOnlyFileName(false);
			bool scriptExists = (env[scriptName] != sol::nil);
			if (!scriptExists) {
				env[scriptName] = comp;
			}
			//scriptExists = (env[scriptName] != sol::nil);
			// Cache old props values
			sol::table oldTable = (scriptExists) ? env[scriptName]["Data"].get<sol::table>() : sol::nil;//.get<sol::table>();
			
			auto result = luaCtx.safe_script_file(FilePath, env, &FLuaScriptAsset::HandleCompileErrors);
			if (result.valid()) {
				if (oldTable != sol::nil) {
					for (const auto& oldData : oldTable) {
						auto tab = env[scriptName]["Data"];
						if (tab != sol::nil)
						{
							BString k = oldData.first.as<BString>();
							auto val = env[scriptName]["Data"][k];
							if (val != sol::nil)
								env[scriptName]["Data"][oldData.first]["Value"] = oldTable[oldData.first]["Value"];
						}
					}
				}
			} else return;

		}
		for (auto cb : OnRecompileDelegates) {
			cb();
		}
		FLogger::LogInfo("Compiled file " + GetOnlyFileName());
	}
	catch (std::exception& e) {
		FLogger::LogError("Error compiling file " + GetOnlyFileName() + "\n" + e.what());
	}
}
