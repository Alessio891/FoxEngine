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

	CurrentScene->LightTransform.Position = FImGui::Vec3("LightPos", CurrentScene->LightTransform.Position);
	CurrentScene->LightTransform.Rotation = FImGui::Vec3("LightRot", CurrentScene->LightTransform.Rotation);
	//CurrentScene->AmbientColor = FImGui::Vec3("LightRot", CurrentScene->AmbientColor);
	{
	float c[3] = { CurrentScene->AmbientColor.x, CurrentScene->AmbientColor.y, CurrentScene->AmbientColor.z };
	ImGui::ColorEdit3("Ambient Color", c, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel);
	CurrentScene->AmbientColor.x = c[0];
	CurrentScene->AmbientColor.y = c[1];
	CurrentScene->AmbientColor.z = c[2];
	}
	{
	float c[3] = { CurrentScene->LightColor.x, CurrentScene->LightColor.y, CurrentScene->LightColor.z };
	ImGui::ColorEdit3("Light Color", c, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel);
	CurrentScene->LightColor.x = c[0];
	CurrentScene->LightColor.y = c[1];
	CurrentScene->LightColor.z = c[2];
	}
	ImGui::DragFloat("Intensity", &CurrentScene->Intensity, 0.05f, 0.0f, 1.0f);
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