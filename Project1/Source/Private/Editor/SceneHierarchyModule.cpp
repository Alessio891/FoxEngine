#include "Editor/SceneHierarchyModule.h"
#include <InspectorModule/InspectorModule.h>
#include <string>

void FSceneHierarchyModule::OnGUIRender()
{
	MAKE_WINDOW("Scene Hierarchy");
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
	END_WINDOW
}

void FSceneHierarchyModule::SetCurrentSelectedObject(SharedPtr<FSceneObject> Obj)
{
	if (CurrentSelectedObject != nullptr) CurrentSelectedObject->Outlined = false;
	CurrentSelectedObject = Obj;
	if (CurrentSelectedObject != nullptr) CurrentSelectedObject->Outlined = true;
	FInspectorModule::Get()->SetDisplayedObject(CurrentSelectedObject);
}

SharedPtr<FSceneHierarchyModule> FSceneHierarchyModule::Instance;