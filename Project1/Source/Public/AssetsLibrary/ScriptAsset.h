#pragma once

#include "Core.h"
#include "AssetResource.h"
#include "sol/sol.hpp"

typedef std::function<void()> OnRecompileDelegate;

class LLuaSceneObjectComponent;

class FLuaScriptAsset : public FAssetResource {

protected:
	List<SharedPtr<LLuaSceneObjectComponent>> envs;
	List<OnRecompileDelegate> OnRecompileDelegates;

	void PrepareEnvironment(sol::environment& env);
	
public:
	FLuaScriptAsset(BString path);
	static sol::protected_function_result HandleCompileErrors(lua_State*, sol::protected_function_result pfr);

	SharedPtr<LLuaSceneObjectComponent> GetNewInstance();

	virtual ImTextureID GetThumbnailIcon() override;
	virtual void DrawInspector() override;

	void RegisterOnRecompileCallback(OnRecompileDelegate cb);

	void Recompile();

	virtual void ReloadResource(BString FilePath) {
		FAssetResource::ReloadResource(FilePath);
		Recompile();
	}
};