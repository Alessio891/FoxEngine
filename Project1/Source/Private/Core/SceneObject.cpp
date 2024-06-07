#include "SceneObject.h"
#include "Core.h"
#include "ObjectComponent.h"
#include "MeshRendererComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "InspectorModule/InspectorModule.h"

void FSceneObject::Tick(float DeltaTime)
{
	//Scale -= Vector3F(0.1f, 0.1f, 0.1f) * DeltaTime;
	//Rotation.y += 0.1f * DeltaTime;
	//Rotation.x += 0.2f * DeltaTime;
	TickComponents(DeltaTime);
}

void FSceneObject::Draw(glm::mat4 V, glm::mat4 P)
{
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), Transform.Position);
	ModelMatrix *= glm::eulerAngleXYX(Transform.Rotation.x, Transform.Rotation.y, Transform.Rotation.z);
	ModelMatrix = glm::scale(ModelMatrix, Transform.Scale);

	if (Renderer != nullptr) {
		Renderer->Render(ModelMatrix, V, P);
	}
}

void FSceneObject::AddComponent(FObjectComponent* Component)
{
	if (Component == nullptr) return;

	std::list<FObjectComponent*>::iterator it = std::find(Components.begin(), Components.end(), Component);
	if (it == Components.end()) {
		Component->Initialize(this);
		Components.push_back(Component);
	}
}

void FSceneObject::RemoveComponent(FObjectComponent* Component)
{
	if (Component == nullptr) return;
	Components.remove(Component);
}

void FSceneObject::SetupRenderer(FMeshRendererComponent* Renderer)
{
	if (Renderer == nullptr && this->Renderer != nullptr)
	{
		delete this->Renderer;
	}
	this->Renderer = Renderer;
}

void FSceneObject::DrawInspector()
{
	if (ImGui::CollapsingHeader("Transform")) {
		Transform.Position = FImGui::Vec3("Position", Transform.Position);
		Transform.Rotation = FImGui::Vec3("Rotation", Transform.Rotation);
		Transform.Scale = FImGui::Vec3("Scale", Transform.Scale);
	}

	for (FObjectComponent* Component : Components) {
		Component->DrawInspector();
	}
}

void FSceneObject::TickComponents(float Delta)
{
	for (FObjectComponent* Component : Components) {
		Component->Tick(Delta);
	}
}
