#include "DebugModule.h"
#include <stdio.h>
#include "Core.h"
#include "MeshRendererComponent.h"
#include "GL/glut.h"
#include "Graphics/Primitives.h"
#include <InputSystem.h>
#include "Graphics/MaterialLibrary.h"
#include <Application.h>
#include "InspectorModule/InspectorModule.h"
#include "ConsoleModule/ConsoleModule.h"

void FDebugModule::OnStartup()
{
	printf("Starting up debug module\n");
	
	SharedPtr<FViewport> SceneViewport = FApplication::Get()->SceneViewport;
	glfwMakeContextCurrent(SceneViewport->ViewportContext);

	FFont::InitializeFreetype();
	Scene = SharedPtr<FScene>(new FScene());

	SharedPtr<MeshData> CubeMeshData(new MeshData());
	CubeMeshData->VertexArray = std::list<float>(std::begin(CUBE_MESH_VERTEX_ARRAY), std::end(CUBE_MESH_VERTEX_ARRAY));
	
	SharedPtr<MeshData> TriangleMeshData(new MeshData());
	TriangleMeshData->VertexArray = std::list<float>(std::begin(TRIANGLE_MESH_VERTEX_ARRAY), std::end(TRIANGLE_MESH_VERTEX_ARRAY));

	SharedPtr<FBaseMaterial> Material(new FBaseMaterial(
		FMaterialLibrary::GetShader("Shaders/Grid/GridShader.vs", GL_VERTEX_SHADER), FMaterialLibrary::GetShader("Shaders/Grid/GridShader.fs", GL_FRAGMENT_SHADER), "DefaultMaterial"
	));

	Cube = SharedPtr<FSceneObject>(new FSceneObject());
	
	FMeshRendererComponent* MeshRenderer = new FMeshRendererComponent();
	MeshRenderer->MeshData = CubeMeshData;
	Cube->AddComponent(MeshRenderer);
	Cube->SetupRenderer(MeshRenderer);

	SharedPtr<FBaseMaterial> Material2(new FBaseMaterial(
		FMaterialLibrary::GetShader("Shaders/DefaultShader.vs", GL_VERTEX_SHADER), FMaterialLibrary::GetShader("Shaders/DefaultShader.fs", GL_FRAGMENT_SHADER), "DefaultMaterial"
	));

	Material2->SetVec3("_Color", Vector3F(0.5f, 0.2f, 0.4f));
	MeshRenderer->Color = Vector3F(0.5f, 0.2f, 0.4f);
	MeshRenderer->Material = Material2;

	SharedPtr<FSceneObject> Grid(new FSceneObject());
	FMeshRendererComponent* GridRenderer = new FMeshRendererComponent();
	SharedPtr<MeshData> GridMeshData(new MeshData());
	GridMeshData->VertexArray = std::list<float>(std::begin(PLANE_MESH_VERTEX_ARRAY), std::end(PLANE_MESH_VERTEX_ARRAY));
	GridRenderer->MeshData = GridMeshData;
	GridRenderer->Material = Material;
	Grid->AddComponent(GridRenderer);
	Grid->SetupRenderer(GridRenderer);

	Scene->RegisterSceneObject(Grid);
	Scene->RegisterSceneObject(Cube);

	//Scene->CurrentViewPosition = Vector3F(0,0,-2.0f);
	Scene->CameraTransform.Position = Vector3F(0, 2, -6);
	font = FFont::LoadFont("fonts/Arial.ttf");
	FontMat = SharedPtr<FBaseMaterial>(new FBaseMaterial(
		FMaterialLibrary::GetShader("Shaders/Fonts/FontShader.vs", GL_VERTEX_SHADER), 
		FMaterialLibrary::GetShader("Shaders/Fonts/FontShader.fs", GL_FRAGMENT_SHADER), 
		"FontMaterial"
	));

	FApplication::Get()->SceneViewport->RegisterRenderCallback([this, CubeMeshData]() {
		Scene->RenderScene();
	});
	FInspectorModule::Get()->SetDisplayedObject(Cube);
	
}

void FDebugModule::OnTick(float Delta)
{
	if (FInputSystem::IsKeyDown(GLFW_KEY_W)) {
		Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * 0.5f * Delta;
	} else if (FInputSystem::IsKeyDown(GLFW_KEY_S)) {
		Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * -0.5f * Delta;
	}
	if (FInputSystem::IsKeyDown(GLFW_KEY_A)) {
		Scene->CameraTransform.Position -= Scene->CameraTransform.GetRightVector() * 0.5f * Delta;
	}
	else if (FInputSystem::IsKeyDown(GLFW_KEY_D)) {
		Scene->CameraTransform.Position += Scene->CameraTransform.GetRightVector() * 0.5f * Delta;
	}
	if (FInputSystem::IsKeyDown(32)) {
		Scene->CameraTransform.Rotation = Vector3F(0.0f,0.0f,0.0f);
	}
	if (FInputSystem::LeftButtonDown == true) {
		std::string mouseY = std::to_string(FInputSystem::MouseDeltaY);
		Scene->CameraTransform.Rotation.x -= FInputSystem::MouseDeltaY * Delta * 150.0f;
		Scene->CameraTransform.Rotation.y -= FInputSystem::MouseDeltaX * Delta * 150.0f;
	}
	//Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * -0.4f * Delta;
	//FInspectorModule::Get()->TestTransform = Scene->CameraTransform;
	
	Scene->TickScene(Delta);

	FontMat->SetVec3("_Color", Vector3F(1.0f, 0.0f, 0.0f));
	//FontMat->SetVec3("_Color", Vector3F(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX)));
	std::string state = FInputSystem::LeftButtonDown ? "true" : " false";
	//font->RenderText(FontMat, "LMB: " + state, 10.0f, 10.0f, 1.0f, Vector3F(1.0f, 1.0f, 1.0f));
	
}
