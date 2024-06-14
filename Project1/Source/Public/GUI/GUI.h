#pragma once
#include "Core.h"
#include "AssetsLibrary/AssetsLibrary.h"
#include "AssetsLibrary/AssetReference.h"

class FGUI {
public:
	static Vector3F Vec3(String label, Vector3F Value);
	static void Color(BString label, Vector3F& Value);

	static void Material(String label, FAssetReference<FBaseMaterial>& outMat );
	static void Texture(String label, FAssetReference<FTexture>& Texture);
	template<class T>
	static void AssetReference(String label, FAssetReference<T>& asset, std::function<bool(FAssetResource&)> acceptAsset = [](FAssetResource& newRef) {return true;}) {
		ImGui::BeginGroup();

		ImGui::Text(label);
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.3f);
		if (!asset.IsValid()) {
			ImGui::Dummy(ImVec2(60, 60));
			ImGui::Text("No Asset");
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
					auto assetPath = static_cast<const char*>(payload->Data);
					SharedPtr<FAssetResource> other = std::static_pointer_cast<FAssetResource>(FAssetsLibrary::GetResourceAs<T>(assetPath));
					if (other != nullptr && acceptAsset(*other))
						asset.Set(FAssetsLibrary::GetResourceAs<T>(assetPath));
				}
				ImGui::EndDragDropTarget();
			}
		}
		else {

			GLFWwindow* editorCtx = FApplication::Get()->EditorGUIViewport->ViewportContext;
			SharedPtr<FAssetResource> assetRef = std::static_pointer_cast<FAssetResource>(asset.Get());
			ImGui::Image((void*)(intptr_t)assetRef->GetThumbnailIcon(), ImVec2(60, 60));
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
					auto assetPath = static_cast<const char*>(payload->Data);
					SharedPtr<FAssetResource> other = std::static_pointer_cast<FAssetResource>(FAssetsLibrary::GetResourceAs<T>(assetPath));
					if (other != nullptr && acceptAsset(*other))
						asset.Set(FAssetsLibrary::GetResourceAs<T>(assetPath));
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::NextColumn();
			std::string last_element(assetRef->FilePath.substr(assetRef->FilePath.rfind("/") + 1));
			ImGui::Text(last_element.c_str());

			ImGui::NextColumn();
			ImGui::Columns(1);
		}

		ImGui::EndGroup();
		ImGui::Separator();
	}

	static void EnumPopup(String label, int& value, std::vector<BString>& values);
};