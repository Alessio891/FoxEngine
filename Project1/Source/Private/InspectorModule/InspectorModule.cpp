#include "InspectorModule/InspectorModule.h"
#include <Application.h>
#include "InputSystem.h"
#include <MeshRendererComponent.h>
#include <Graphics/MaterialLibrary.h>
#include "Graphics/Primitives.h"
void FInspectorModule::OnStartup()
{
	FEditorGUIModule::OnStartup();
	
	Instance = SharedPtr<FInspectorModule>(this);
}

void FInspectorModule::OnTick(float Delta)
{
	FEditorGUIModule::OnTick(Delta);

}

void FInspectorModule::OnGUIRender()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	static bool open = true;
	ImGui::SetNextWindowSize(ImVec2(FApplication::Get()->InspectorViewport->GetWidth(), FApplication::Get()->InspectorViewport->GetHeight()));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Inspector", &open, window_flags);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImVec2 p = ImGui::GetMousePos();
	ImGui::Text("MousePos: %.2f %.2f", p.x, p.y);
	ImGui::SeparatorText("Scene Object");
	if (CurrentDisplayedObject != nullptr) {
		CurrentDisplayedObject->DrawInspector();
	}

	if (ImGui::Button("Spawn")) {
		glfwMakeContextCurrent(FApplication::Get()->SceneViewport->ViewportContext);
		SharedPtr<FBaseMaterial> Material(new FBaseMaterial(
			FMaterialLibrary::GetShader("Shaders/DefaultShader.vs", GL_VERTEX_SHADER), FMaterialLibrary::GetShader("Shaders/DefaultShader.fs", GL_FRAGMENT_SHADER), "DefaultMaterial2"
		));

		SharedPtr<FSceneObject> newObj(new FSceneObject("Another Cube"));
		FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
		SharedPtr<MeshData> mData(new MeshData());
		mData->VertexArray = std::list<float>(std::begin(CUBE_MESH_VERTEX_ARRAY), std::end(CUBE_MESH_VERTEX_ARRAY));
		meshRenderer->MeshData = mData;
		meshRenderer->Material = Material;
		meshRenderer->Color = Vector3F(0.5f, 0.2f, 0.4f);
		newObj->AddComponent(meshRenderer);
		newObj->SetupRenderer(meshRenderer);
		
		FApplication::Get()->GetCurrentScene()->RegisterSceneObject(newObj);

		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//newObj->Transform.Position = Vector3F(r * 5.0f, g * 10.0f, b * 8.0f);
		glfwMakeContextCurrent(FApplication::Get()->InspectorViewport->ViewportContext);
	}

	ImGui::End();

}

SharedPtr<FInspectorModule> FInspectorModule::Instance;

Vector3F FImGui::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos, 0.1f);

	return Vector3F(pos[0], pos[1], pos[2]);
}
