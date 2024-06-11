#pragma once
#include "Core.h"
#include "EditorGUIModule.h"

class FSceneHierarchyModule : public FEditorGUIModule {
public:
	virtual void OnStartup() override;
	FSceneHierarchyModule(SharedPtr<FViewport> vp) : FEditorGUIModule(vp) {
		Instance = SharedPtr<FSceneHierarchyModule>(this);
	}

	virtual void OnGUIRender() override;
	static SharedPtr<FSceneHierarchyModule> Get() { return Instance; }

	void SetCurrentSelectedObject(SharedPtr<FSceneObject> Obj);
	SharedPtr<FSceneObject> GetCurrentSelectedObject() { return CurrentSelectedObject; }
protected:
	static SharedPtr<FSceneHierarchyModule> Instance;
	SharedPtr<FSceneObject> CurrentSelectedObject;

	void DrawSceneHierarchy();
	void DrawLightSettings();

	int CurrentTab = 0;
};