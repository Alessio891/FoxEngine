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
#include <Graphics/MaterialLibrary.h>

FSceneObject::FSceneObject() : Name("[Object]"), IInspectable()
{
	FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
	meshRenderer->Material.Set(FMaterialLibrary::GetMaterial("DefaultLit"));
	AddComponent(meshRenderer);
	SetupRenderer(meshRenderer);
}

FSceneObject::FSceneObject(String name) : Name(name), IInspectable()
{
	FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
	meshRenderer->Material.Set(FMaterialLibrary::GetMaterial("DefaultLit"));
	AddComponent(meshRenderer);
	SetupRenderer(meshRenderer);
}

void FSceneObject::Tick(float DeltaTime)
{
	//Scale -= Vector3F(0.1f, 0.1f, 0.1f) * DeltaTime;
	//Rotation.y += 0.1f * DeltaTime;
	//Rotation.x += 0.2f * DeltaTime;
	TickComponents(DeltaTime);

	auto element = Components.begin();
	for (int i = 0; i < Components.size(); i++) {
		if ((*element)->IsMarkedForRemoval()) {
			Components.remove(*element);
		}
		else {
			std::advance(element, 1);
		}
	}
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

void FSceneObject::OnDrawGUI(float Delta)
{
	for (FObjectComponent* comp : Components) {
		comp->OnDrawGUI(Delta);
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
	Component->MarkForRemoval();
}

void FSceneObject::SetupRenderer(FMeshRendererComponent* Renderer)
{
	this->Renderer = SharedPtr<FMeshRendererComponent>(Renderer);
}

void FSceneObject::DrawInspector()
{
	ImGui::PushID(Name.c_str());
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
		auto scripts = FAssetsLibrary::GetAllResourcesOfType(EAssetResourceType::Script);
		for (BString script : scripts) {
			SharedPtr<FLuaScriptAsset> s = FAssetsLibrary::GetResourceAs<FLuaScriptAsset>(script);
			if (ImGui::Selectable(s->GetOnlyFileName(false).c_str())) {
				FLuaObjectComponent* comp = new FLuaObjectComponent();
				comp->ScriptAsset.Set(s);
				AddComponent(comp);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}
	float spacing = ImGui::GetWindowWidth() - 250;
	ImGui::Separator();
	ImGui::Dummy(ImVec2(spacing / 2, 0));
	ImGui::SameLine();
	if (ImGui::Button("Add Component", ImVec2(200, 30))) {
	}
	ImGui::OpenPopupOnItemClick("#components", ImGuiPopupFlags_MouseButtonLeft);
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(spacing / 2, 0));
	ImGui::PopID();
}

void FSceneObject::TickComponents(float Delta)
{
	for (FObjectComponent* Component : Components) {
		Component->Tick(Delta);
	}
}

void FTransform::LookAt(Vector3F Target)
{
	Vector3F direction = Target - Position;
	glm::vec3 euler;
	glm::vec3 forward = glm::normalize(direction);

	// Calculate yaw (rotation around Y-axis)
	euler.y = atan2(forward.x, forward.z);

	// Calculate pitch (rotation around X-axis)
	euler.x = -asin(forward.y);

	// Roll (rotation around Z-axis) is typically zero for a simple look-at
	euler.z = 0.0f;

	Rotation = euler;
}
