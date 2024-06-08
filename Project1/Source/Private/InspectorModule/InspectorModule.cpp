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
	MAKE_WINDOW("Inspector");
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
		SharedPtr<MeshData> mData(new MeshData(std::list<float>(std::begin(CUBE_MESH_VERTEX_ARRAY), std::end(CUBE_MESH_VERTEX_ARRAY))));
		meshRenderer->MeshData = mData;
		meshRenderer->Material = Material;
		meshRenderer->Color = Vector3F(0.5f, 0.2f, 0.4f);
		newObj->AddComponent(meshRenderer);
		newObj->SetupRenderer(meshRenderer);
		
		FApplication::Get()->GetCurrentScene()->RegisterSceneObject(newObj);

		glfwMakeContextCurrent(FApplication::Get()->InspectorViewport->ViewportContext);
	}
	END_WINDOW;

}

SharedPtr<FInspectorModule> FInspectorModule::Instance;

Vector3F FImGui::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos, 0.1f);

	return Vector3F(pos[0], pos[1], pos[2]);
}
