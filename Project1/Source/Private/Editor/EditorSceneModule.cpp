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
#include <Graphics/Graphics.h>
#include "Physics.h"
#include <PhysicsComponent.h>

void FEditorSceneModule::OnStartup()
{
	Instance = SharedPtr<FEditorSceneModule>(this);
	SharedPtr<FViewport> SceneViewport = FApplication::Get()->GameViewport;
	glfwMakeContextCurrent(SceneViewport->ViewportContext);

	Scene = SharedPtr<FScene>(new FScene("Resources/Scenes/Scene.sc"));
	Scene->OpenScene();
	EditorGrid = SharedPtr<FSceneObject>(new FSceneObject());
	EditorGrid->HideInHierarchy = true;

	FMeshRendererComponent* GridRenderer = new FMeshRendererComponent();
	SharedPtr<MeshData> GridMeshData(new MeshData(std::list<float>(std::begin(PLANE_MESH_VERTEX_ARRAY), std::end(PLANE_MESH_VERTEX_ARRAY))));

	EditorGrid->Renderer->MeshDataRef = GridMeshData;
	EditorGrid->Renderer->Material.Set(FMaterialLibrary::GetMaterial("GridShader"));
	EditorGrid->Renderer->UseMeshAsset = false;
	
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

	StartCube = SharedPtr<FSceneObject>(new FSceneObject("A Cube"));
	SharedPtr<FModelAsset> model = FAssetsLibrary::GetResourceAs<FModelAsset>("Resources/Models/cube.obj");
	StartCube->Renderer->MeshAsset.Set(model);
	FPhysicsComponent* phys = new FPhysicsComponent();
	phys->Shape = SharedPtr<FCollisionShape>(new FSphereShape());
	//phys->IsKinematic = false;
	StartCube->AddComponent(phys);
	phys->Rigidbody->setFriction(1.0f);
	phys->Rigidbody->setRestitution(1.0f);
	phys->Rigidbody->setRollingFriction(1.0f);
	{
		auto c = SharedPtr<FSceneObject>(new FSceneObject("A Cube2"));
		c->Renderer->MeshAsset.Set(model);
		FPhysicsComponent* phys2 = new FPhysicsComponent();
		//phys2->IsKinematic = true;
		c->Transform.Scale = Vector3F(10.0f, 0.5f, 10.0f);
		phys2->Mass = 0.0f;
		c->AddComponent(phys2);
		phys2->Rigidbody->setRestitution(1.0f);
		Scene->RegisterSceneObject(c);
	}
	{
		auto c = SharedPtr<FSceneObject>(new FSceneObject("A Cube3"));
		c->Transform.Position = Vector3F(5, 10, 0);
		c->Renderer->MeshAsset.Set(model);
		FPhysicsComponent* phys2 = new FPhysicsComponent();
		//phys2->IsKinematic = false;
		c->AddComponent(phys2);
		Scene->RegisterSceneObject(c);
	}
	StartCube->Transform.Position = Vector3F(0, 15, 0);
	
	PositionGizmo = SharedPtr<FSceneGizmo>(new FSceneGizmo());
	PositionGizmo->HideInHierarchy = true;
	EditorGrid->RenderingQueue = ERenderingQueue::PreRender;
	Scene->RegisterSceneObject(EditorGrid);
	Scene->RegisterSceneObject(StartCube);
	//Scene->RegisterSceneObject(PositionGizmo);

	RenderingPipeline = SharedPtr<FRenderingPipeline>(new FRenderingPipeline(SceneViewport));

	Scene->CameraTransform.Rotation = Vector3F(0.0f,0.0f,0.0f);

	drawer = new GLDebugDrawer();
	drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);// | btIDebugDraw::DBG_DrawContactPoints);
	FPhysics::btWorld->setDebugDrawer(drawer);
	
	SceneViewport->RegisterRenderCallback([this]() {
		glLineWidth(3.0f);

		RenderingPipeline->PreRender(Scene->CameraTransform, Scene);
		if (RenderWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		RenderingPipeline->Render(Scene->CameraTransform, Scene);
		if (RenderWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		

		RenderingPipeline->PostRender(Scene->CameraTransform, Scene);

		if (FInputSystem::IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			GLfloat _z = 0;
			unsigned char stencil;
			float zNear = 0.1f;
			float zFar = 100.0f;
			//= ys - 1.0 - y;
			int y = FApplication::Get()->GameViewport->GetHeight() - 1 - FInputSystem::LastMouseY;
			glReadPixels(FInputSystem::LastMouseX, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &stencil);
			float depth = _z;                                               // logarithmic
			depth = (2.0 * depth) - 1.0;                                  // logarithmic NDC
			depth = (2.0 * zNear) / (zFar + zNear - (depth * (zFar - zNear)));    // linear <0,1>
			depth = zNear + depth * (zFar - zNear);
			//printf("At %d , %d the value is %d\n", FInputSystem::LastMouseX, y, stencil);
			if (stencil > 0) {
				auto objects = FApplication::Get()->GetCurrentScene()->GetSceneObjects();
				List<SharedPtr<FSceneObject>>::const_iterator o = objects.begin();
				SharedPtr<FSceneObject> picked;
				while (o != objects.end()) {
					if (*o != nullptr && (*o)->ObjectID == stencil) {
						picked = (*o);
						break;
					}
					o++;
				}
				
				if (FSceneHierarchyModule::Get() != nullptr && picked != nullptr) {
					FSceneHierarchyModule::Get()->SetCurrentSelectedObject(picked);
				}
				if (FInspectorModule::Get() != nullptr && picked != nullptr) {
					FInspectorModule::Get()->SetDisplayedObject(picked);
				}
				FLogger::LogInfo("Selected object " + std::string(o->get()->Name));
			}
		}
		for (auto obj : Scene->GetSceneObjects()) {
			for (auto c : obj->GetComponents()) {
				if (auto p = dynamic_cast<FPhysicsComponent*>(c)) {
					/*btTransform t;
					auto rot = btQuaternion();
					rot.setEulerZYX(obj->Transform.Rotation.y, obj->Transform.Rotation.x, obj->Transform.Rotation.z);
					t.setRotation(rot);
					t.setOrigin(btVector3(obj->Transform.Position.x, obj->Transform.Position.y, obj->Transform.Position.z));
					FPhysics::btWorld->debugDrawObject(t, p->Rigidbody->getCollisionShape(), btVector3(0.0f, 1.0f, 0.0f));*/
				}
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
/*	ImGui::SetNextWindowPos(ImVec2(Position.x, Position.y));
	ImGui::SetNextWindowSize(ImVec2(Size.x, Size.y));
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("_", NULL, BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
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
	*/
	Scene->DrawGUI(0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);

	ImGui::SetNextWindowPos(ImVec2(Position.x + 10, Position.y + 10));
	ImGui::SetNextWindowSize(ImVec2(50, 200));
	ImGui::Begin("TestWin", NULL, BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar);
	ImGui::PopStyleVar();
	static float targetAlpha = 0.0f;
	static bool showMenu = false;
	static float alpha = 0.0f;

	auto primitiveIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/primitive.png");
	auto wireframeIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/wireframe.png");
	auto playIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/play.png");
	auto stopIcon = FAssetsLibrary::GetImage("Resources/Images/GUI/stop.png");
	auto playStop = Scene->IsPlaying() ? stopIcon : playIcon;
	if (ImGui::ImageButton("objects",(void*)(intptr_t)primitiveIcon->GetTextureID(Viewport->ViewportContext), ImVec2(25, 25))) {
		FLogger::LogInfo("Clicked " + std::to_string(targetAlpha));
		showMenu = !showMenu;
		targetAlpha = showMenu ? 1.0f : 0.0f;
	}
	if (ImGui::ImageButton("wireframe", (void*)(intptr_t)wireframeIcon->GetTextureID(Viewport->ViewportContext), ImVec2(25, 25))) {
		RenderWireFrame = !RenderWireFrame;
	}
	if (ImGui::ImageButton("play", (void*)(intptr_t)playStop->GetTextureID(Viewport->ViewportContext), ImVec2(25, 25))) {
		if (Scene->IsPlaying()) {
			Scene->SetPlayMode(false);
		}
		else {
			Scene->SetPlayMode(true);
		}
	}
	primitiveIcon.reset();

	if (showMenu) {
		if (abs(targetAlpha - alpha) > 0.001f) {
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
			SharedPtr<FModelAsset> model = FAssetsLibrary::GetResourceAs<FModelAsset>("Resources/Models/cube.obj");
			SharedPtr<FSceneObject> o = Scene->SpawnObject("Cube");
			o->Renderer->MeshAsset.Set(model);
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)pyramidIcon->GetTextureID(Viewport->ViewportContext), ImVec2(20, 20))) {
			SharedPtr<FModelAsset> model = FAssetsLibrary::GetResourceAs<FModelAsset>("Resources/Models/cone.obj");
			SharedPtr<FSceneObject> o = Scene->SpawnObject("Cone");
			o->Renderer->MeshAsset.Set(model);
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((void*)(intptr_t)sphereIcon->GetTextureID(Viewport->ViewportContext), ImVec2(20, 20))) {
			SharedPtr<FModelAsset> model = FAssetsLibrary::GetResourceAs<FModelAsset>("Resources/Models/sphere.obj");
			SharedPtr<FSceneObject> o = Scene->SpawnObject("Sphere");
			o->Renderer->MeshAsset.Set(model);
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
	float camSpeed = 1.5f;
	if (FInputSystem::IsMouseButtonHeld(GLFW_MOUSE_BUTTON_LEFT)) {
		if (FInputSystem::IsKeyHeld(GLFW_KEY_W)) {
			Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * camSpeed * Delta;
		}
		else if (FInputSystem::IsKeyHeld(GLFW_KEY_S)) {
			Scene->CameraTransform.Position += Scene->CameraTransform.GetForwardVector() * -camSpeed * Delta;
		}
		if (FInputSystem::IsKeyHeld(GLFW_KEY_A)) {
			Scene->CameraTransform.Position -= Scene->CameraTransform.GetRightVector() * camSpeed * Delta;
		}
		else if (FInputSystem::IsKeyHeld(GLFW_KEY_D)) {
			Scene->CameraTransform.Position += Scene->CameraTransform.GetRightVector() * camSpeed * Delta;
		}
		
		std::string mouseY = std::to_string(FInputSystem::MouseDeltaY);
		Scene->CameraTransform.AddRotation( Vector3F(FInputSystem::MouseDeltaY *  190.0f, -FInputSystem::MouseDeltaX * 190.0f, 0.0f ));
		//Scene->CameraTransform.Rotation.x += FInputSystem::MouseDeltaY * Delta * 290.0f;
		//Scene->CameraTransform.Rotation.y -= FInputSystem::MouseDeltaX * Delta * 290.0f;
	}
	if (FInputSystem::IsKeyHeld(GLFW_KEY_SPACE)) {
		for (auto a : StartCube->GetComponents()) {
			if (auto c = dynamic_cast<FPhysicsComponent*>(a)) {
				c->Rigidbody->applyForce(btVector3(0, 200, 0), btVector3(StartCube->Transform.Position.x, StartCube->Transform.Position.y, StartCube->Transform.Position.z));
			}
		} 
	}
}

SharedPtr<FEditorSceneModule> FEditorSceneModule::Instance;