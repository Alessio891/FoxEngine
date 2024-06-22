#pragma once
#include "Core.h"
#include "SceneObject.h"
#include "AssetsLibrary/AssetResource.h"
#include "AssetsLibrary/ISerializedAsset.h"

class FScene : public FAssetResource, public ISerializedAsset {

public:
	FScene(BString Path);

	virtual void RegisterSceneObject(SharedPtr<FSceneObject> SceneObject);
	virtual void UnregisterSceneObject(SharedPtr<FSceneObject> SceneObject);
	virtual SharedPtr<FSceneObject> SpawnObject(BString Name);
	virtual SharedPtr<FSceneObject> SpawnObject(BString Name, BString TemplatePath);
	virtual void TickScene(float Delta);
	virtual void DrawGUI(float Delta);

	virtual void Deserialize(json Json) override;
	virtual void Serialize(json& outJson) override;

	List<SharedPtr<FSceneObject>> GetSceneObjects() { return SceneObjects; }
	void SetPlayMode(bool play);
	bool IsPlaying() { return PlayMode; }

	void OpenScene();

	// TODO: Wrap this in a way so that the camera transform is based on an object and not on a fixed FTransform in the Scene
	FTransform CameraTransform;
	FTransform LightTransform;
	Vector3F LightColor = Vector3F(1.0f, 1.0f, 1.0f);
	float Intensity = 1.0f;
	Vector3F AmbientColor = Vector3F(0.2f, 0.2f, 0.2f);
protected:
	bool PlayMode = false;
private:
	List<SharedPtr<FSceneObject>> SceneObjects;
	int LastID = -1;
};