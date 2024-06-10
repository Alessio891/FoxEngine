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
		auto mat = FMaterialLibrary::GetMaterial("DefaultLit");
		json j;
		mat->Serialize(j);
		printf("%s\n", j.dump().c_str());
	/*
		glfwMakeContextCurrent(FApplication::Get()->SceneViewport->ViewportContext);
		
		SharedPtr<FSceneObject> newObj(new FSceneObject("Another Cube"));
		FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
		SharedPtr<MeshData> mData(new MeshData( CUBE_MESH_VERTICES, CUBE_MESH_INDICES, CUBE_MESH_NORMALS, CUBE_MESH_UVS ) );
		meshRenderer->MeshData = mData;
		meshRenderer->SetTexture("Resources/Images/test.png");
		meshRenderer->Material = FMaterialLibrary::GetMaterial("DefaultLit");
		newObj->AddComponent(meshRenderer);
		newObj->SetupRenderer(meshRenderer);
		
		FApplication::Get()->GetCurrentScene()->RegisterSceneObject(newObj);

		glfwMakeContextCurrent(FApplication::Get()->EditorGUIViewport->ViewportContext);*/
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
