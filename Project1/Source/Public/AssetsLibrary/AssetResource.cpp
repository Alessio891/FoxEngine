#include "AssetsLibrary/AssetResource.h"
#include "AssetsLibrary/AssetsLibrary.h"
#include <InspectorModule/InspectorModule.h>
#include <fstream>
bool FAssetResource::FileStillExists()
{
	std::ifstream f(FilePath.c_str());
	return f.good();
}

void FAssetResource::DrawResourceThumbnail()
{
	BString name = GetOnlyFileName(false);
	if (name.size() > 7) {
		name = name.substr(0, 5) + "..";
	}
	//static BString placeholder = "56789123456789";
	ImVec2 size(80,15);// = ImGui::CalcTextSize(placeholder.c_str());
	ImVec2 spacing((size.x-32)/2,0);
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->ChannelsSplit(2);
	drawList->ChannelsSetCurrent(1);
	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(0, 3));

	ImGui::Dummy(spacing);
	ImGui::SameLine();
	
	ImGui::Image(GetThumbnailIcon(), ImVec2(32, 32));
	
	ImGui::SameLine();
	ImGui::Dummy(spacing);
	ImGui::NewLine();
	
	auto windowWidth = size.x + 12;
	auto textWidth = ImGui::CalcTextSize(name.c_str()).x;
	
	
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((windowWidth - textWidth)*0.5f ));
	ImGui::Text(name.c_str());

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
	if (ImGui::IsItemHovered()) {
		if (ImGui::BeginTooltip()) {
			ImGui::Text(FilePath.c_str());
			ImGui::EndTooltip();
		}
	}
}

void FAssetResource::DrawInspector()
{

	ImGui::Text("Unknown Asset Type");
	ImGui::Text(FilePath.c_str());
}

ImTextureID FAssetResource::GetThumbnailIcon()
{
	return (void*)(intptr_t)FAssetsLibrary::GetImage("Resources/Images/GUI/unknown.png")->GetTextureID(FApplication::Get()->GameViewport->ViewportContext);
}
