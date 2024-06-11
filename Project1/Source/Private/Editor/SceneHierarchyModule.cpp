#include "Editor/SceneHierarchyModule.h"
#include <InspectorModule/InspectorModule.h>
#include <string>
#include <GUI/GUI.h>

void FSceneHierarchyModule::OnStartup()
{
	FEditorGUIModule::OnStartup();

	Viewport->RegisterViewportResizeCallback([this](int x, int y, int w, int h){
		int consoleHeight = h * FApplication::Get()->ConsoleHeight;
		int hierarchyWidth = w * FApplication::Get()->InspectorWidth;
		int inspectorWidth = w * FApplication::Get()->InspectorWidth;
		int sceneWidth = w - inspectorWidth - hierarchyWidth;

		Position.x = inspectorWidth+sceneWidth;
		Size.x = hierarchyWidth;
		Size.y = h - consoleHeight;
	});
}

void FSceneHierarchyModule::OnGUIRender()
{
	MAKE_WINDOW("Scene");
	
	ImGui::BeginTabBar("#scene_tabs");

	if (ImGui::BeginTabItem("Scene Hierarchy")) {
		CurrentTab = 0;
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Scene Settings")) {
		CurrentTab = 1;
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	if (CurrentTab == 0) DrawSceneHierarchy();
	if (CurrentTab == 1) DrawLightSettings();

	END_WINDOW
}

void FSceneHierarchyModule::SetCurrentSelectedObject(SharedPtr<FSceneObject> Obj)
{
	if (CurrentSelectedObject != nullptr) CurrentSelectedObject->Outlined = false;
	CurrentSelectedObject = Obj;
	if (CurrentSelectedObject != nullptr) CurrentSelectedObject->Outlined = true;
	FInspectorModule::Get()->SetDisplayedObject(CurrentSelectedObject);
}

void FSceneHierarchyModule::DrawSceneHierarchy()
{
	SharedPtr<FScene> CurrentScene = FApplication::Get()->GetCurrentScene();
	if (CurrentScene == nullptr) return;
	if (CurrentScene != nullptr) {
		for (auto sceneObj : CurrentScene->GetSceneObjects()) {
			if (sceneObj->HideInHierarchy) continue;
			bool selected = sceneObj == CurrentSelectedObject;
			std::string st = "[" + std::to_string(sceneObj->ObjectID) + "] " + sceneObj->Name;
			if (ImGui::Selectable(st.c_str(), selected)) {
				SetCurrentSelectedObject(sceneObj);
			}
		}
	}

	
}

void FSceneHierarchyModule::DrawLightSettings()
{
	SharedPtr<FScene> CurrentScene = FApplication::Get()->GetCurrentScene();
	CurrentScene->LightTransform.Position = FGUI::Vec3("LightPos", CurrentScene->LightTransform.Position);
	CurrentScene->LightTransform.Rotation = FGUI::Vec3("LightRot", CurrentScene->LightTransform.Rotation);
	//CurrentScene->AmbientColor = FImGui::Vec3("LightRot", CurrentScene->AmbientColor);
	
	Vector3F ambient = CurrentScene->AmbientColor;
	FGUI::Color("Ambient Color", ambient);
	CurrentScene->AmbientColor = ambient;

	Vector3F lightCol = CurrentScene->LightColor;
	FGUI::Color("Light Color", lightCol);
	CurrentScene->LightColor = lightCol;
	
	ImGui::DragFloat("Intensity", &CurrentScene->Intensity, 0.05f, 0.0f, 1.0f);
}

SharedPtr<FSceneHierarchyModule> FSceneHierarchyModule::Instance;