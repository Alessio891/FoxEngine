#pragma once
#include "Core.h"
#include "ApplicationModule.h"
#include "Scene.h"
#include <Graphics/Fonts.h>

class FDebugModule : public FApplicationModule {
public:
	virtual void OnStartup() override;
	virtual void OnTick(float Delta) override;

	SharedPtr<FScene> Scene;

	SharedPtr<FSceneObject> Cube;
	SharedPtr<FSceneObject> Triangle;
	SharedPtr<FFont> font;
	SharedPtr<FBaseMaterial> FontMat;
};