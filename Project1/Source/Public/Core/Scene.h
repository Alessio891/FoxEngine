#pragma once
#include "Core.h"
#include "SceneObject.h"

class FScene {

public:
	virtual void RegisterSceneObject(SharedPtr<FSceneObject> SceneObject);
	virtual void UnregisterSceneObject(SharedPtr<FSceneObject> SceneObject);

	List<SharedPtr<FSceneObject>> GetSceneObjects() { return SceneObjects; }

	virtual void TickScene(float Delta);

	virtual void RenderScene();
	virtual void PostRender();

	FScene() {}

	//float horizontalAngle;
	//float verticalAngle;
	FTransform CameraTransform;

	//Vector3F CurrentViewPosition;
	//Vector3F CurrentViewDirection = Vector3F(0,0,1);
protected:
private:
	std::list<SharedPtr<FSceneObject>> SceneObjects;
};