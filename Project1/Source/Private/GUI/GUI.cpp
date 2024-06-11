#include "GUI/GUI.h"
#include "Graphics/Texture.h"
#include "Application.h"

Vector3F FGUI::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos, 0.1f);
	ImGui::Separator();
	return Vector3F(pos[0], pos[1], pos[2]);
}

void FGUI::Color(BString label, Vector3F& Color)
{
	float c[3] = { Color.x, Color.y, Color.z };
	
	if (ImGui::ColorEdit3(label.c_str(), c, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel)) {
		Color.x = c[0];
		Color.y = c[1];
		Color.z = c[2];
	}
	ImGui::Separator();
}

void FGUI::Material(String label, SharedPtr<FBaseMaterial>& outMat)
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->ChannelsSplit(2);
	drawList->ChannelsSetCurrent(1);
	ImGui::BeginGroup();

	ImGui::Text(label);
	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.3f);
	if (Texture == nullptr) {
		ImGui::Dummy(ImVec2(60, 60));
		ImGui::Text("No texture");
	}
	else {
		GLFWwindow* editorCtx = FApplication::Get()->EditorGUIViewport->ViewportContext;
		ImGui::Image((void*)(intptr_t)FAssetsLibrary::GetImage("Resources/Images/GUI/mat_icon.png").get()->GetTextureID(editorCtx), ImVec2(60, 60));
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
				auto assetPath = static_cast<const char*>(payload->Data);
				auto other = FAssetsLibrary::GetResourceAs<FBaseMaterial>(assetPath);
				outMat = other;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::NextColumn();
		ImGui::Text(outMat->GetOnlyFileName().c_str());
		ImGui::NextColumn();
		ImGui::Columns(1);
	}
	ImGui::EndGroup();

	drawList->ChannelsSetCurrent(0);
	ImVec2 p_min = ImGui::GetItemRectMin();
	ImVec2 p_max = ImGui::GetItemRectMax();
	p_max.x = ImGui::GetWindowWidth();
	//drawList->AddRect(p_min, p_max, IM_COL32(100, 150, 150, 255));
	drawList->ChannelsMerge();
	ImGui::Separator();

}

void FGUI::Texture(String label, FTextureResource& Texture)
{
	ImGui::BeginGroup();

	ImGui::Text(label);
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
	ImGui::Separator();
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
	ImGui::Separator();
}
