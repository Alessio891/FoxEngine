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
#include <LuaIntegration/LuaObjectComponent.h>
#include "AssetsLibrary/ScriptAsset.h"

void FEditorSceneModule::OnStartup()
{
	Instance = SharedPtr<FEditorSceneModule>(this);
	SharedPtr<FViewport> SceneViewport = FApplication::Get()->EditorGUIViewport;
	glfwMakeContextCurrent(SceneViewport->ViewportContext);

	Scene = SharedPtr<FScene>(new FScene());

	EditorGrid = SharedPtr<FSceneObject>(new FSceneObject());
	EditorGrid->HideInHierarchy = true;

	FMeshRendererComponent* GridRenderer = new FMeshRendererComponent();
	SharedPtr<MeshData> GridMeshData(new MeshData(std::list<float>(std::begin(PLANE_MESH_VERTEX_ARRAY), std::end(PLANE_MESH_VERTEX_ARRAY))));

	GridRenderer->MeshDataRef = GridMeshData;
	GridRenderer->Material.Set(FMaterialLibrary::GetMaterial("GridShader"));
	EditorGrid->AddComponent(GridRenderer);
	EditorGrid->SetupRenderer(GridRenderer);

	Scene->CameraTransform.Position = Vector3F(0, 3, -3);

	SharedPtr<MeshData> mData = SharedPtr<MeshData>(new MeshData(CUBE_MESH_VERTICES, CUBE_MESH_INDICES, CUBE_MESH_NORMALS, CUBE_MESH_UVS));
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
	meshRenderer->MeshDataRef = mData;//CubePrimitive;
	meshRenderer->Material.Set(FMaterialLibrary::GetMaterial("DefaultLit"));
	meshRenderer->Texture = FAssetReference<FTexture>(FAssetsLibrary::GetImage("Resources/Images/test.png"), "Resources/Images/test.png");
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
			int y = FApplication::Get()->EditorGUIViewport->GetHeight() - 1 - FInputSystem::LastMouseY;
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
	LastDelta = Delta;
}

void FEditorSceneModule::OnGUIRender()
{
	ImGui::SetNextWindowPos(ImVec2(Position.x, Position.y));
	ImGui::SetNextWindowSize(ImVec2(Size.x, Size.y));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("_", NULL, BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	ImGui::Dummy(ImVec2(Size.x, Size.y));
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
			auto assetPath = static_cast<const char*>(payload->Data);
			SharedPtr<FAssetResource> asset = FAssetsLibrary::GetResource(assetPath);
			if (asset->ResourceType == EAssetResourceType::Template) {
				SharedPtr<FTemplateAsset> templ = std::static_pointer_cast<FTemplateAsset>(asset);
				NewObject(templ);
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
	
	ImGui::SetNextWindowPos(ImVec2(Position.x + 10, Position.y + 10));
	ImGui::SetNextWindowSize(ImVec2(50, 200));
	ImGui::Begin("TestWin", NULL, BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar);
	ImGui::PopStyleVar();
	static float targetAlpha = 0.0f;
	static bool showMenu = false;
	static float alpha = 0.0f;
	
	auto primitiveIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/primitive.png");
	
	if (ImGui::ImageButton((void*)(intptr_t)primitiveIcon->GetTextureID(Viewport->ViewportContext), ImVec2(25, 25))) {
		FLogger::LogInfo("Clicked " + std::to_string(targetAlpha));
		showMenu = !showMenu;
		targetAlpha = showMenu ? 1.0f : 0.0f;
	}

	primitiveIcon.reset();

	if (showMenu) {
		if (abs(targetAlpha-alpha) > 0.001f) {
			float delta = targetAlpha - alpha;
			float step = delta >= 0 ? 0.8f : -0.8f;
			alpha += step * LastDelta;
		}
		auto cubeIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/cube.png");
		auto pyramidIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/pyramid.png");
		auto sphereIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/sphere.png");


		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		ImGui::SetNextWindowSize(ImVec2(120, 42));
		ImGui::SetNextWindowPos(ImVec2(Position.x + 60, Position.y + 10));
		ImGui::Begin("#primitives", NULL, BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
		ImGui::PopStyleVar();
		if (ImGui::ImageButton((void*)(intptr_t)cubeIcon->GetTextureID(Viewport->ViewportContext), ImVec2(20, 20))) {
			NewObject(SharedPtr<MeshData>(new MeshData(CUBE_MESH_VERTICES, CUBE_MESH_INDICES, CUBE_MESH_NORMALS, CUBE_MESH_UVS)), "New Cube");
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)pyramidIcon->GetTextureID(Viewport->ViewportContext), ImVec2(20, 20))) {
			NewObject(SharedPtr<MeshData>(new MeshData(PYRAMID_MESH_VERTICES, PYRAMID_MESH_INDICES, PYRAMID_MESH_NORMALS, PYRAMID_MESH_UVS)), "New Pyramid");
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)sphereIcon->GetTextureID(Viewport->ViewportContext), ImVec2(20, 20))) {
			NewObject(generateSphereMesh(0.5f, 30), "New Sphere");
		}

		ImGui::End();

		cubeIcon.reset();
		pyramidIcon.reset();
		sphereIcon.reset();
	}
	ImGui::SameLine();
	ImGui::Spacing();

	ImGui::End();

	
}

SharedPtr<FSceneObject> FEditorSceneModule::NewObject(SharedPtr<MeshData> meshData, BString ObjName)
{
	SharedPtr<FSceneObject> newObj(new FSceneObject(ObjName.c_str()));
	FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
	meshRenderer->MeshDataRef = meshData;
	meshRenderer->Material.Set(FMaterialLibrary::GetMaterial("DefaultLit"));
	meshRenderer->Texture = FAssetReference<FTexture>(FAssetsLibrary::GetImage("Resources/Images/test.png"), "Resources/Images/test.png");
	newObj->AddComponent(meshRenderer);
	newObj->SetupRenderer(meshRenderer);
	newObj->Transform.Position = Scene->CameraTransform.Position + Scene->CameraTransform.GetForwardVector() * 8.0f;

	FLuaObjectComponent* luaComp = new FLuaObjectComponent();
	if (ObjName == "New Pyramid")
		luaComp->ScriptAsset.Set(FAssetsLibrary::GetResourceAs<FLuaScriptAsset>("Resources/Scripts/MyFirstScript.lua"));
	else
		luaComp->ScriptAsset.Set(FAssetsLibrary::GetResourceAs<FLuaScriptAsset>("Resources/Scripts/Script2.lua"));
	newObj->AddComponent(luaComp);

	Scene->RegisterSceneObject(newObj);
	return newObj;
}

SharedPtr<FSceneObject> FEditorSceneModule::NewObject(SharedPtr<FTemplateAsset> templ)
{
	SharedPtr<FSceneObject> newObj(new FSceneObject(templ->Name.c_str()));

	for (auto& comp : templ->Components) {
		if (comp.Type == "MeshRenderer") {
			FMeshRendererComponent* meshRenderer = new FMeshRendererComponent();
			meshRenderer->Deserialize(comp.ComponentJson);
			newObj->AddComponent(meshRenderer);
			newObj->SetupRenderer(meshRenderer);
		}
	}
	newObj->Transform.Position = Scene->CameraTransform.Position + Scene->CameraTransform.GetForwardVector() * 8.0f;
	Scene->RegisterSceneObject(newObj);
	return newObj;
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
	if (FInputSystem::IsMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {
		std::string mouseY = std::to_string(FInputSystem::MouseDeltaY);
		Scene->CameraTransform.Rotation.x -= FInputSystem::MouseDeltaY * Delta * 50.0f;
		Scene->CameraTransform.Rotation.y -= FInputSystem::MouseDeltaX * Delta * 50.0f;
	}
}

SharedPtr<FEditorSceneModule> FEditorSceneModule::Instance;