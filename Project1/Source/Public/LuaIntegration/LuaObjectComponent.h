#pragma once
#include "Core.h"
#include "ObjectComponent.h"
#include "AssetsLibrary/AssetReference.h"
#include <AssetsLibrary/ScriptAsset.h>

class FLuaObjectComponent : public FObjectComponent {

public:

	FAssetReference<FLuaScriptAsset> ScriptAsset;

	virtual void Begin() override;
	virtual void End() override;

	virtual void Tick(float Delta) override;

	virtual void DrawInspector() override;
};
