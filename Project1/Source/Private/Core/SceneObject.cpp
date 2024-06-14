#include "SceneObject.h"
#include "Core.h"
#include "ObjectComponent.h"
#include "MeshRendererComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "InspectorModule/InspectorModule.h"
#include "imgui_stdlib.h"
#include <LuaIntegration/LuaObjectComponent.h>
#include <AssetsLibrary/ScriptAsset.h>

void FSceneObject::Tick(float DeltaTime)
{
	//Scale -= Vector3F(0.1f, 0.1f, 0.1f) * DeltaTime;
	//Rotation.y += 0.1f * DeltaTime;
	//Rotation.x += 0.2f * DeltaTime;
	TickComponents(DeltaTime);
}

void FSceneObject::Draw(glm::mat4 V, glm::mat4 P)
{
	glm::mat4 ModelMatrix = Transform.GetTransformMatrix();

	if (Renderer != nullptr) {
		if (RenderingQueue == ERenderingQueue::Depth) {
			glStencilFunc(GL_ALWAYS, ObjectID, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
		}
		Renderer->Render(V, P);
	}
}

void FSceneObject::AddComponent(FObjectComponent* Component)
{
	if (Component == nullptr) return;

	std::list<FObjectComponent*>::iterator it = std::find(Components.begin(), Components.end(), Component);
	if (it == Components.end()) {
		Component->Initialize(this);
		Components.push_back(Component);
		Component->Begin();
	}
}

void FSceneObject::RemoveComponent(FObjectComponent* Component)
{
	if (Component == nullptr) return;
	Component->End();
	Components.remove(Component);
}

void FSceneObject::SetupRenderer(FMeshRendererComponent* Renderer)
{
	this->Renderer = SharedPtr<FMeshRendererComponent>(Renderer);
}

void FSceneObject::DrawInspector()
{
	ImGui::Text("Name:");
	ImGui::SameLine();
	ImGui::InputText("_", &Name);

	if (ImGui::CollapsingHeader("Transform")) {
		Transform.Position = FImGui::Vec3("Position", Transform.Position);
		Transform.Rotation = FImGui::Vec3("Rotation", Transform.Rotation);
		Transform.Scale = FImGui::Vec3("Scale", Transform.Scale);
	}

	for (FObjectComponent* Component : Components) {
		Component->DrawInspector();
	}

	if (ImGui::BeginPopup("#components")) {
		if (ImGui::Selectable("Test Script")) {
			FLuaObjectComponent* comp = new FLuaObjectComponent();
			comp->ScriptAsset.Set(FAssetsLibrary::GetResourceAs<FLuaScriptAsset>("Resources/Scripts/MyFirstScript.lua"));
			AddComponent(comp);
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x - 10, 30))) {
	}
	ImGui::OpenPopupOnItemClick("#components", ImGuiPopupFlags_MouseButtonLeft);
}

void FSceneObject::TickComponents(float Delta)
{
	for (FObjectComponent* Component : Components) {
		Component->Tick(Delta);
	}
}
