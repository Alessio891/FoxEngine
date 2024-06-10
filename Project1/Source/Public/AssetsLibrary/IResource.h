#pragma once

#include "Core.h"
#include <Editor/AssetsLibrary.h>

class IResource {
public:
	virtual void DrawResourceThumbnail() {
		
		/*
		ImGui::BeginGroup();
		auto resource = FAssetsLibrary::GetImage(entry.path().string())->GetResource(Viewport->ViewportContext);
		ImGui::Image((void*)(intptr_t)resource->GetTextureID(), ImVec2(64, 64));
		ImGui::Text(entry.path().filename().string().c_str());
		ImGui::EndGroup();


		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::Image((void*)(intptr_t)FAssetsLibrary::GetImage(entry.path().string())->GetResource(Viewport->ViewportContext)->GetTextureID(), ImVec2(32, 32));
			ImGui::Text(entry.path().filename().string().c_str());
			std::string path = resource->GetTexturePath();
			ImGui::SetDragDropPayload("ASSET_DRAG", path.c_str(), path.size() + 1);
			ImGui::EndDragDropSource();
		}*/
	};
};