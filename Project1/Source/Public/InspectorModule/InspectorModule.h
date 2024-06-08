#pragma once
#include "Core.h"
#include "Editor/EditorGUIModule.h"
#include <SceneObject.h>

class FImGui {
public:
	static Vector3F Vec3(String label, Vector3F Value);
};

class FInspectorModule : public FEditorGUIModule {
public:
	FInspectorModule(SharedPtr<FViewport> vp) : FEditorGUIModule(vp) {}
	virtual void OnStartup() override;
	virtual void OnTick(float Delta) override;
	virtual void OnGUIRender() override;

	static SharedPtr<FInspectorModule> Get() {
		return Instance;
	}

	void SetDisplayedObject(SharedPtr<FSceneObject> Object) {
		CurrentDisplayedObject = Object;
	}

protected:
	static SharedPtr<FInspectorModule> Instance;
	SharedPtr<FSceneObject> CurrentDisplayedObject;
};