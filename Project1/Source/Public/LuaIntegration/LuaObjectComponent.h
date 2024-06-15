#pragma once
#include "Core.h"
#include "ObjectComponent.h"
#include "AssetsLibrary/ScriptAsset.h"
#include "AssetsLibrary/AssetReference.h"
class FLuaObjectComponent : public FObjectComponent {
protected:
	SharedPtr<class LLuaSceneObjectComponent> LuaComponent;
	void SetupLuaScript();

	int unique_id = 0;
public:
	FLuaObjectComponent() : FObjectComponent() {}
	FAssetReference<FLuaScriptAsset> ScriptAsset;

	virtual void Begin() override;
	virtual void End() override;

	virtual void Tick(float Delta) override;

	virtual void DrawInspector() override;

	virtual void OnRecompiled();

	virtual void OnDrawGUI(float Delta) override;

	virtual void Deserialize(json json);
};
