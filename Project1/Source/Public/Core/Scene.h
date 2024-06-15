#pragma once
#include "Core.h"
#include "SceneObject.h"

class FScene {

public:
	virtual void RegisterSceneObject(SharedPtr<FSceneObject> SceneObject);
	virtual void UnregisterSceneObject(SharedPtr<FSceneObject> SceneObject);

	virtual SharedPtr<FSceneObject> SpawnObject(BString Name, BString TemplatePath);

	List<SharedPtr<FSceneObject>> GetSceneObjects() { return SceneObjects; }

	virtual void TickScene(float Delta);
	virtual void DrawGUI(float Delta);

	FScene() {}

	// TODO: Wrap this in a way so that the camera transform is based on an object and not on a fixed FTransform in the Scene
	FTransform CameraTransform;
	FTransform LightTransform;
	Vector3F LightColor = Vector3F(1.0f, 1.0f, 1.0f);
	float Intensity = 1.0f;
	Vector3F AmbientColor = Vector3F(0.2f, 0.2f, 0.2f);
protected:
private:
	std::list<SharedPtr<FSceneObject>> SceneObjects;
};