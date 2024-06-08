#pragma once

#include "Core.h"
#include "ApplicationModule.h"
#include <Scene.h>
#include "Editor\SceneGizmo.h"
class FEditorSceneModule : public FApplicationModule {

public:
	virtual void OnStartup() override;
	virtual void OnTick(float Delta) override;

	SharedPtr<FScene> GetScene() { return Scene; }

protected:
	
	SharedPtr<FScene> Scene;

private:
	SharedPtr<FSceneGizmo> PositionGizmo;
	void HandleCameraInput(float Delta);

	SharedPtr<FSceneObject> EditorGrid;
};