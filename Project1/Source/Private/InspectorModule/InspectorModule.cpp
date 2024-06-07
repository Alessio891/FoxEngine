#include "InspectorModule/InspectorModule.h"
#include <Application.h>
#include "InputSystem.h"
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
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	static bool open = true;
	ImGui::SetNextWindowSize(ImVec2(FApplication::Get()->InspectorViewport->GetWidth(), FApplication::Get()->InspectorViewport->GetHeight()));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Inspector", &open, window_flags);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImVec2 p = ImGui::GetMousePos();
	ImGui::Text("MousePos: %.2f %.2f", p.x, p.y);
	ImGui::SeparatorText("Scene Object");
	if (CurrentDisplayedObject != nullptr) {
		CurrentDisplayedObject->DrawInspector();
	}
	ImGui::End();

}

SharedPtr<FInspectorModule> FInspectorModule::Instance;

Vector3F FImGui::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos);

	return Vector3F(pos[0], pos[1], pos[2]);
}
