#include "Scene.h"
#include "Core.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <InputSystem.h>
#include <Application.h>
#include "Viewport.h"
void FScene::RegisterSceneObject(SharedPtr<FSceneObject> SceneObject)
{
	if (SceneObject == nullptr)
		return;

	std::list<SharedPtr<FSceneObject>>::iterator it = std::find(SceneObjects.begin(), SceneObjects.end(), SceneObject);
	if (it == SceneObjects.end())
	{
		SceneObject->ObjectID = SceneObjects.size();
		SceneObjects.push_back(SceneObject);
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

void FScene::TickScene(float Delta)
{
	//if (FInputSystem::MouseDeltaX > 0 || FInputSystem::MouseDeltaY > 0)
		//printf("Delta: %f.2 %f.2\n", FInputSystem::MouseDeltaX, FInputSystem::MouseDeltaY);
	for (SharedPtr<FSceneObject> object : SceneObjects) {
		object->Tick(Delta);
	}
}

void FScene::RenderScene()
{	
	Vector3F right = CameraTransform.GetRightVector();
	Vector3F up = CameraTransform.GetUpVector();

	glm::mat4 CameraMatrix = glm::lookAt(
		CameraTransform.Position,
		CameraTransform.Position + CameraTransform.GetForwardVector(),
		up
	);

	FViewport viewport = *FApplication::Get()->SceneViewport;
	float aspect = float(viewport.GetWidth()) / float(viewport.GetHeight());

	glm::mat4 ProjectionMatrix = glm::perspective<float>(
		glm::radians(45.0f),
		aspect,
		0.1f,
		100.0f
	);
	
	for (SharedPtr<FSceneObject> object : SceneObjects) {
		if (object->RenderingQueue == ERenderingQueue::PreRender)
		{
			object->Draw(CameraMatrix, ProjectionMatrix);
		}
	}
	
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (SharedPtr<FSceneObject> object : SceneObjects) {
		if (object->RenderingQueue == ERenderingQueue::Depth)
		{
			object->Draw(CameraMatrix, ProjectionMatrix);
		}
	}

	
}

void FScene::PostRender()
{
	Vector3F right = CameraTransform.GetRightVector();
	Vector3F up = CameraTransform.GetUpVector();

	glm::mat4 CameraMatrix = glm::lookAt(
		CameraTransform.Position,
		CameraTransform.Position + CameraTransform.GetForwardVector(),
		up
	);

	FViewport viewport = *FApplication::Get()->SceneViewport;
	float aspect = float(viewport.GetWidth()) / float(viewport.GetHeight());

	glm::mat4 ProjectionMatrix = glm::perspective<float>(
		glm::radians(45.0f),
		aspect,
		0.1f,
		100.0f
	);

	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (SharedPtr<FSceneObject> object : SceneObjects) {
		if (object->RenderingQueue == ERenderingQueue::Overlay)
		{
			object->Draw(CameraMatrix, ProjectionMatrix);
		}
	}
}
