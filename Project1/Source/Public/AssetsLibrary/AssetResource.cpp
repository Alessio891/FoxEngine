#include "AssetsLibrary/AssetResource.h"
#include "Editor/AssetsLibrary.h"
#include <InspectorModule/InspectorModule.h>

void FAssetResource::DrawResourceThumbnail()
{
	ImVec2 size = ImGui::CalcTextSize(GetOnlyFileName().c_str());
	//size.x = 50;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->ChannelsSplit(2);
	drawList->ChannelsSetCurrent(1);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	ImGui::BeginGroup();
	ImGui::PopStyleColor();
	ImGui::Dummy(ImVec2(0, 3));
	ImGui::Dummy(ImVec2(size.x / 2 - 24, 0));
	ImGui::SameLine();
	ImGui::Image(GetThumbnailIcon(), ImVec2(48, 48));
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(size.x / 2 - 24, 0));
	ImGui::NewLine();

	ImGui::Spacing();
	ImGui::SameLine();
	ImGui::Text(GetOnlyFileName().c_str());
	ImGui::Dummy(ImVec2(0,3));
	ImGui::EndGroup();

	SharedPtr<FInspectorModule> inspector = FInspectorModule::Get();

	if (inspector != nullptr && inspector->GetDisplayedObject().get() == this)
	{
		drawList->ChannelsSetCurrent(0);
		ImVec2 p_min = ImGui::GetItemRectMin();
		ImVec2 p_max = ImGui::GetItemRectMax();
		ImU32 col = IM_COL32(66, 135, 245, 255);

		ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, col);
	} else if (ImGui::IsItemHovered()) {
		drawList->ChannelsSetCurrent(0);
		ImVec2 p_min = ImGui::GetItemRectMin();
		ImVec2 p_max = ImGui::GetItemRectMax();
		ImU32 col = IM_COL32(54, 103, 181, 255);

		ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, col);
	}
	drawList->ChannelsMerge();


	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
		ImGui::Image(GetThumbnailIcon(), ImVec2(32, 32));
		ImGui::Text(GetOnlyFileName().c_str());
		std::string path = FilePath;
		ImGui::SetDragDropPayload("ASSET_DRAG", path.c_str(), path.size() + 1);
		ImGui::EndDragDropSource();
	}
}

void FAssetResource::DrawInspector()
{

	ImGui::Text("Unknown Asset Type");
	ImGui::Text(FilePath.c_str());
}

ImTextureID FAssetResource::GetThumbnailIcon()
{
	return (void*)(intptr_t)FAssetsLibrary::GetImage("Resources/Images/GUI/unknown.png")->GetTextureID(FApplication::Get()->EditorGUIViewport->ViewportContext);
}
