#pragma once

#include "Core.h"
#include "ApplicationModule.h"
#include "EditorGUIModule.h"
#include <Scene.h>
#include "Editor\SceneGizmo.h"
#include <Graphics/RenderingPipeline.h>
class FEditorSceneModule : public FEditorGUIModule {

public:
	FEditorSceneModule(SharedPtr<FViewport> VP) : FEditorGUIModule(VP) {}
	virtual void OnStartup() override;
	virtual void OnTick(float Delta) override;

	SharedPtr<FScene> GetScene() { return Scene; }

	virtual void OnGUIRender() override;

	static SharedPtr<FEditorSceneModule> Get() { return Instance; }

protected:
	static SharedPtr<FEditorSceneModule> Instance;
	SharedPtr<FScene> Scene;
	SharedPtr<FRenderingPipeline> RenderingPipeline;

private:
	SharedPtr<FSceneGizmo> PositionGizmo;
	void HandleCameraInput(float Delta);

	SharedPtr<FSceneObject> EditorGrid;
};