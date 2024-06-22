#pragma once
#include "Core.h"
#include "AssetsLibrary/AssetsLibrary.h"
#include "AssetsLibrary/AssetReference.h"
#include "SceneObject.h"
class FGUI {
public:
	static Vector3F Vec3(String label, Vector3F Value);
	static void Color(BString label, Vector3F& Value);

	static void Material(String label, FAssetReference<FBaseMaterial>& outMat );
	static void Texture(String label, FAssetReference<FTexture>& Texture);
	static void ObjectReference(String label, SharedPtr<FSceneObject>& obj, std::function<bool(SharedPtr<FSceneObject>)> acceptObject = [](SharedPtr<FSceneObject> newRef) {return true;});
	template<class T>
	static void AssetReference(String label, FAssetReference<T>& asset, std::function<bool(FAssetResource&)> acceptAsset = [](FAssetResource& newRef) {return true;}) {
		ImGui::PushID(label);
		ImGui::BeginGroup();

		ImGui::Text(label);
		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.3f);
		if (!asset.IsValid()) {
			ImGui::Dummy(ImVec2(60, 60));
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
			ImGui::Text("No Asset");
		}
		else {

			GLFWwindow* editorCtx = FApplication::Get()->GameViewport->ViewportContext;
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

		}
		ImGui::NextColumn();
		ImGui::Columns(1);
		if (ImGui::Button("Clear")) {
		printf("clear\n");
			asset.Set(nullptr);
		}
		ImGui::EndGroup();
		ImGui::Separator();

		ImGui::PopID();
	}

	static void EnumPopup(String label, int& value, std::vector<BString>& values);
};