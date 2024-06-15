#include "GUI/GUI.h"
#include "Graphics/Texture.h"
#include "Application.h"
#include <Editor/SceneHierarchyModule.h>

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

void FGUI::Material(String label, FAssetReference<FBaseMaterial>& outMat)
{
	AssetReference<FBaseMaterial>(label, outMat);

}

void FGUI::Texture(String label, FAssetReference<FTexture>& Texture)
{
	AssetReference<FTexture>(label, Texture);
}

void FGUI::ObjectReference(String label, SharedPtr<FSceneObject>& obj, std::function<bool(SharedPtr<FSceneObject>)> acceptObject)
{
	{
		ImGui::BeginGroup();

		ImGui::Text(label);
		ImGui::Columns(2);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->ChannelsSplit(2);
		drawList->ChannelsSetCurrent(1);
		ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.3f);
		if (obj == nullptr) {
			ImGui::Dummy(ImVec2(60, 60));
			if (const auto payload = ImGui::GetDragDropPayload()) {
				if (auto d = *static_cast<SharedPtr<FSceneObject>*>(payload->Data)) {
					drawList->ChannelsSetCurrent(0);
					ImVec2 p_min = ImGui::GetItemRectMin();
					ImVec2 p_max = ImGui::GetItemRectMax();
					ImU32 col = IM_COL32(131, 217, 20, 255);

					drawList->AddRect(p_min, p_max, col);
				}
			}
			drawList->ChannelsMerge();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT_DRAG")) {
					auto droppedObj = *static_cast<SharedPtr<FSceneObject>*>(payload->Data);
					if (droppedObj != nullptr && acceptObject(droppedObj))
					{
						obj = droppedObj;
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::NextColumn();
			ImGui::Text("No Object");
		}
		else {

			GLFWwindow* editorCtx = FApplication::Get()->GameViewport->ViewportContext;
			ImGui::Image(FAssetsLibrary::GetImage("Resources/Images/GUI/object.png")->GetThumbnailIcon(), ImVec2(60, 60));
			
			if (const auto payload = ImGui::GetDragDropPayload()) {
				if (auto d = *static_cast<SharedPtr<FSceneObject>*>(payload->Data)) {
					drawList->ChannelsSetCurrent(0);
					ImVec2 p_min = ImGui::GetItemRectMin();
					ImVec2 p_max = ImGui::GetItemRectMax();
					ImU32 col = IM_COL32(131, 217, 20, 255);

					drawList->AddRect(p_min, p_max, col);
				}
			}
			drawList->ChannelsMerge();
			if (ImGui::IsItemClicked()) {
				FSceneHierarchyModule::Get()->SetCurrentSelectedObject(obj);
			}
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT_DRAG")) {
					auto droppedObj = *static_cast<SharedPtr<FSceneObject>*>(payload->Data);
					if (droppedObj != nullptr && acceptObject(droppedObj))
					{
						obj = SharedPtr<FSceneObject>(droppedObj);
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::NextColumn();
			ImGui::Text(obj->Name.c_str());

		}
		ImGui::NextColumn();
		ImGui::Columns(1);
		

		ImGui::EndGroup();

		ImGui::Separator();
	}
}

void FGUI::EnumPopup(String label, int& value, std::vector<BString>& values)
{
	if (ImGui::BeginCombo(label, values[value].c_str())) {
		for(unsigned int i = 0; i < values.size(); i++)
		{
			if (ImGui::Selectable(values[i].c_str())) {
				value = i;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
}
