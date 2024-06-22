#include "Editor/TopMenuModule.h"
#include "Scene.h"

void FTopMenuModule::OnStartup()
{
	FEditorGUIModule::OnStartup();
}

void FTopMenuModule::OnTick(float Delta)
{
}

void FTopMenuModule::OnGUIRender()
{
	//MAKE_WINDOW_F("Menu", BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoDecoration);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 9));
	if (ImGui::BeginMainMenuBar()) {
		ImGui::PopStyleVar();
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New")) {
				json j;
				FApplication::Get()->GetCurrentScene()->Serialize(j);
				printf(j.dump().c_str());
			}
			if (ImGui::MenuItem("Open")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About")) {
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open")) {}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	//END_WINDOW
}
