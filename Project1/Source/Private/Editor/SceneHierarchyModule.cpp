#include "Editor/SceneHierarchyModule.h"
#include <InspectorModule/InspectorModule.h>
#include <string>

void FSceneHierarchyModule::OnGUIRender()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	static bool open = true;
	ImGui::SetNextWindowSize(ImVec2(FApplication::Get()->InspectorViewport->GetWidth(), FApplication::Get()->InspectorViewport->GetHeight()));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Inspector", &open, window_flags);
	
	SharedPtr<FScene> CurrentScene = FApplication::Get()->GetCurrentScene();

	if (CurrentScene != nullptr) {
		for (auto sceneObj : CurrentScene->GetSceneObjects()) {
			if (sceneObj->HideInHierarchy) continue;
			bool selected = sceneObj == CurrentSelectedObject;
			std::string st = "["+ std::to_string(sceneObj->ObjectID) + "] " + sceneObj->Name; 
			if (ImGui::Selectable(st.c_str(), selected)) {
				SetCurrentSelectedObject(sceneObj);
			}
		}
	}

	ImGui::End();
}

void FSceneHierarchyModule::SetCurrentSelectedObject(SharedPtr<FSceneObject> Obj)
{
	if (CurrentSelectedObject != nullptr) CurrentSelectedObject->Outlined = false;
	CurrentSelectedObject = Obj;
	if (CurrentSelectedObject != nullptr) CurrentSelectedObject->Outlined = true;
	FInspectorModule::Get()->SetDisplayedObject(CurrentSelectedObject);
}

SharedPtr<FSceneHierarchyModule> FSceneHierarchyModule::Instance;