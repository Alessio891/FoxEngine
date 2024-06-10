#include "GUI/GUI.h"
#include "Graphics/Texture.h"
#include "Application.h"

Vector3F FGUI::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos, 0.1f);

	return Vector3F(pos[0], pos[1], pos[2]);
}

Vector3F FGUI::Color(String label, Vector3F Color)
{
	float c[3] = { Color.x, Color.y, Color.z };
	ImGui::Text("Material Color");
	ImGui::ColorEdit3("Color", c, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel);
	Color.x = c[0];
	Color.y = c[1];
	Color.z = c[2];

	return Vector3F(c[0], c[1], c[2]);
}

void FGUI::Texture(String label, FTextureResource& Texture)
{
	ImGui::BeginGroup();

	ImGui::Text("Main Texture");
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.3f);
	if (Texture == nullptr) {
		ImGui::Dummy(ImVec2(60, 60));
		ImGui::Text("No texture");
	}
	else {
		GLFWwindow* editorCtx = FApplication::Get()->EditorGUIViewport->ViewportContext;
		ImGui::Image((void*)(intptr_t)Texture.get()->GetTextureID(editorCtx), ImVec2(60, 60));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
				auto assetPath = static_cast<const char*>(payload->Data);
				auto other = FAssetsLibrary::GetImage(assetPath);
				Texture = other;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::NextColumn();
		std::string last_element(Texture.get()->FilePath.substr(Texture.get()->FilePath.rfind("/") + 1));
		ImGui::Text(last_element.c_str());
		auto resource = Texture.get();
		ImGui::Text("%dx%d", resource->GetWidth(), resource->GetHeight());
		ImGui::NextColumn();

		ImGui::Columns(1);
	}

	ImGui::EndGroup();
}

void FGUI::EnumPopup(String label, int& value, std::vector<BString>& values)
{
	if (ImGui::BeginCombo(label, values[value].c_str())) {
		for(int i = 0; i < values.size(); i++)
		{
			if (ImGui::Selectable(values[i].c_str())) {
				value = i;
			}
		}
		ImGui::EndCombo();
	}
}
