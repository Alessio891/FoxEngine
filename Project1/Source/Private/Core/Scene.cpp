#include "Scene.h"
#include "Core.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <InputSystem.h>
#include <Application.h>
#include "Viewport.h"
#include <AssetsLibrary/TemplateAsset.h>
#include "Logger.h"
#include "MeshRendererComponent.h"
#include <LuaIntegration/LuaObjectComponent.h>
#include "Physics.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
void FScene::RegisterSceneObject(SharedPtr<FSceneObject> SceneObject)
{
	if (SceneObject == nullptr)
		return;

	std::list<SharedPtr<FSceneObject>>::iterator it = std::find(SceneObjects.begin(), SceneObjects.end(), SceneObject);
	if (it == SceneObjects.end())
	{
		LastID++;
		SceneObject->ObjectID = LastID;
		SceneObjects.push_back(SceneObject);
		if (PlayMode)
			SceneObject->Begin();
	}
}

void FScene::UnregisterSceneObject(SharedPtr<FSceneObject> SceneObject)
{
	if (SceneObject == nullptr)
		return;

	SceneObject->End();
	SceneObjects.remove(SceneObject);
}

SharedPtr<FSceneObject> FScene::SpawnObject(BString Name)
{
	SharedPtr<FSceneObject> newObj(new FSceneObject(Name.c_str()));

	RegisterSceneObject(newObj);
	return newObj;
}

SharedPtr<FSceneObject> FScene::SpawnObject(BString Name, BString TemplatePath)
{

	SharedPtr<FTemplateAsset> templ = FAssetsLibrary::GetResourceAs<FTemplateAsset>(TemplatePath);
	if (templ == nullptr) {
		FLogger::LogError("Couldn't find template file " + TemplatePath);
		return nullptr;
	}

	SharedPtr<FSceneObject> newObj(new FSceneObject(Name.c_str()));

	for (auto& comp : templ->Components) {
		if (comp.Type == "MeshRenderer") {
			FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
			meshRenderer->Deserialize(comp.ComponentJson);
			newObj->AddComponent(meshRenderer);
			newObj->SetupRenderer(meshRenderer);
		}
		else if (comp.Type == "LuaComponent") {
			FLuaObjectComponent* luaComp = new FLuaObjectComponent();
			luaComp->Deserialize(comp.ComponentJson);
			newObj->AddComponent(luaComp);
		}
	}
	
	RegisterSceneObject(newObj);
	return newObj;
}

void FScene::TickScene(float Delta)
{
	//if (FInputSystem::MouseDeltaX > 0 || FInputSystem::MouseDeltaY > 0)
		//printf("Delta: %f.2 %f.2\n", FInputSystem::MouseDeltaX, FInputSystem::MouseDeltaY);
	if (PlayMode) {
		FPhysics::StepSimulation(Delta);

		for (SharedPtr<FSceneObject> object : SceneObjects) {
			object->PhysicsTick(Delta);
		}
		for (SharedPtr<FSceneObject> object : SceneObjects) {
			object->Tick(Delta);
		}
	}
	auto obj = SceneObjects.begin();
	while (obj != SceneObjects.end()) {
		if ((*obj)->IsMarkedForDestroy()) {
			(*obj)->End();
			SceneObjects.erase(obj++);
		}
		else {
			++obj;
		}
	}
	
}

void FScene::DrawGUI(float Delta)
{
	for (auto object : SceneObjects) {
		object->OnDrawGUI(Delta);
	}
}

void FScene::Deserialize(json Json)
{
}

void FScene::Serialize(json& outJson)
{
	json rot = LightTransform;
	outJson["LightTransform"] = rot;
}

FScene::FScene(BString Path) : FAssetResource(EAssetResourceType::Scene, Path), ISerializedAsset()
{
	
}

void FScene::SetPlayMode(bool play)
{
	if (PlayMode != play) {
		PlayMode = play;
		if (play) {
			for (auto o : SceneObjects) {
				o->Begin();
			}
		}
		else {
			for (auto o : SceneObjects) {
				o->End();
			}
		}
	}

}

void FScene::OpenScene()
{
	FPhysics::Initialize();
}
