#include <Application.h>
#include "Editor/EditorSceneModule.h"
#include "MeshRendererComponent.h"
#include "Graphics\Primitives.h"

#include <Graphics/MaterialLibrary.h>
#include <InputSystem.h>
#include <iostream>
#include <vector>
#include <Editor/SceneHierarchyModule.h>
#include <InspectorModule/InspectorModule.h>
#include <Logger.h>

void FEditorSceneModule::OnStartup()
{
	SharedPtr<FViewport> SceneViewport = FApplication::Get()->SceneViewport;
	glfwMakeContextCurrent(SceneViewport->ViewportContext);

	Scene = SharedPtr<FScene>(new FScene());

	EditorGrid = SharedPtr<FSceneObject>(new FSceneObject());
	EditorGrid->HideInHierarchy = true;

	FMeshRendererComponent* GridRenderer = new FMeshRendererComponent();
	SharedPtr<MeshData> GridMeshData(new MeshData(std::list<float>(std::begin(PLANE_MESH_VERTEX_ARRAY), std::end(PLANE_MESH_VERTEX_ARRAY))));

	GridRenderer->MeshData = GridMeshData;
	GridRenderer->Material = FMaterialLibrary::GetMaterial("GridShader");
	EditorGrid->AddComponent(GridRenderer);
	EditorGrid->SetupRenderer(GridRenderer);

	Scene->CameraTransform.Position = Vector3F(0, 3, -3);

	SharedPtr<MeshData> mData(new MeshData(CUBE_MESH_VERTICES, CUBE_MESH_INDICES, CUBE_MESH_NORMALS, CUBE_MESH_UVS));
	std::vector<float> vertices = {
			 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f
	};

	SharedPtr<MeshData> gizmosMesh(new MeshData({
			 0.0f, 0.0f, 0.0f,
			 0.0f, 1.0f, 0.0f
		}));

	gizmosMesh->DrawType = GL_LINES;

	SharedPtr<FSceneObject> newObj(new FSceneObject("A Cube"));
	FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
	meshRenderer->MeshData = mData;//CubePrimitive;
	meshRenderer->Material = FMaterialLibrary::GetMaterial("DefaultLit");
	meshRenderer->Texture = FAssetsLibrary::GetImage("Resources/Images/test.png");
	newObj->AddComponent(meshRenderer);
	newObj->SetupRenderer(meshRenderer);


	PositionGizmo = SharedPtr<FSceneGizmo>(new FSceneGizmo());
	PositionGizmo->HideInHierarchy = true;
	EditorGrid->RenderingQueue = ERenderingQueue::PreRender;
	Scene->RegisterSceneObject(EditorGrid);
	FApplication::Get()->GetCurrentScene()->RegisterSceneObject(newObj);
	FApplication::Get()->GetCurrentScene()->RegisterSceneObject(PositionGizmo);

	RenderingPipeline = SharedPtr<FRenderingPipeline>(new FRenderingPipeline(SceneViewport));


	SceneViewport->RegisterRenderCallback([this]() {
		glLineWidth(3.0f);

		RenderingPipeline->PreRender(Scene->CameraTransform, Scene);

		RenderingPipeline->Render(Scene->CameraTransform, Scene);

		RenderingPipeline->PostRender(Scene->CameraTransform, Scene);

		if (FInputSystem::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			GLfloat _z = 0;
			unsigned char stencil;
			float zNear = 0.1f;
			float zFar = 100.0f;
			//= ys - 1.0 - y;
			int y = FApplication::Get()->SceneViewport->GetHeight() - 1 - FInputSystem::LastMouseY;
			glReadPixels(FInputSystem::LastMouseX, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &stencil);
			float depth = _z;                                               // logarithmic
			depth = (2.0 * depth) - 1.0;                                  // logarithmic NDC
			depth = (2.0 * zNear) / (zFar + zNear - (depth * (zFar - zNear)));    // linear <0,1>
			depth = zNear + depth * (zFar - zNear);
			//printf("At %d , %d the value is %d\n", FInputSystem::LastMouseX, y, stencil);
			if (stencil > 0) {
				auto objects = FApplication::Get()->GetCurrentScene()->GetSceneObjects();
				List<SharedPtr<FSceneObject>>::const_iterator o = objects.begin();
				std::advance(o, stencil);
				if (FSceneHierarchyModule::Get() != nullptr) {
					FSceneHierarchyModule::Get()->SetCurrentSelectedObject(*o);
				}
				if (FInspectorModule::Get() != nullptr) {
					FInspectorModule::Get()->SetDisplayedObject(*o);
				}
				FLogger::LogInfo("Selected object " + std::string(o->get()->Name));
			}
		}

		//Scene->PostRender();
		});
	FEditorGUIModule::OnStartup();

}

void FEditorSceneModule::OnTick(float Delta)
{
	//FEditorGUIModule::OnTick(Delta);

	HandleCameraInput(Delta);

	Scene->TickScene(Delta);
	if (FSceneHierarchyModule::Get() != nullptr) {
		auto selected = FSceneHierarchyModule::Get()->GetCurrentSelectedObject();
		if (selected != nullptr) {
			PositionGizmo->Transform.Position = selected->Transform.Position;
			PositionGizmo->Transform.Rotation = selected->Transform.Rotation;
		}

	}
}

void FEditorSceneModule::OnGUIRender()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
	ImGui::Begin("TestWin", NULL, BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar);
	ImGui::PopStyleVar();
	ImGui::SetWindowPos(ImVec2(10, 10));
	ImGui::SetWindowSize(ImVec2(50, 200));
	ImGui::Text("%.1f", ImGui::GetIO().Framerate);
	ImGui::End();
}

void FEditorSceneModule::HandleCameraInput(float Delta)
{
	if (FInputSystem::IsKeyDown(GLFW_KEY_W)) {
		Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * 0.2f * Delta;
	}
	else if (FInputSystem::IsKeyDown(GLFW_KEY_S)) {
		Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * -0.2f * Delta;
	}
	if (FInputSystem::IsKeyDown(GLFW_KEY_A)) {
		Scene->CameraTransform.Position -= Scene->CameraTransform.GetRightVector() * 0.2f * Delta;
	}
	else if (FInputSystem::IsKeyDown(GLFW_KEY_D)) {
		Scene->CameraTransform.Position += Scene->CameraTransform.GetRightVector() * 0.2f * Delta;
	}
	if (FInputSystem::IsKeyDown(GLFW_KEY_G)) {
		Scene->CameraTransform.Rotation = Vector3F(0.0f, 0.0f, 0.0f);
	}
	if (FInputSystem::LeftButtonDown == true) {
		std::string mouseY = std::to_string(FInputSystem::MouseDeltaY);
		Scene->CameraTransform.Rotation.x -= FInputSystem::MouseDeltaY * Delta * 50.0f;
		Scene->CameraTransform.Rotation.y -= FInputSystem::MouseDeltaX * Delta * 50.0f;
	}
}
