#pragma once
#include "Core.h"
#include "AssetsLibrary/AssetResource.h"
#include <Editor/AssetsLibrary.h>

struct FTemplateComponent {
public:
	FTemplateComponent(json json) : ComponentJson(json) {
		json["Type"].get_to(Type);
	}
	BString Type;
	json ComponentJson;
protected:
};

class FTemplateAsset : public FAssetResource {
public:
	FTemplateAsset(BString Path) : FAssetResource(EAssetResourceType::Template, Path) {
	}
	BString Name;
	List<FTemplateComponent> Components;

	void LoadFromJson(json js) {	
		js["Name"].get_to<BString>(Name);
		for (auto& c : js["Components"]) {
			FTemplateComponent component(c);
			Components.push_back(c);
		}
	}

	virtual void DrawInspector() override {
		ImGui::Text("Name: %s", Name.c_str());
		ImGui::Text("Components: %d", Components.size());
		ImGui::Indent(5.0f);
		for (FTemplateComponent& c : Components) {
			ImGui::Text("Component Type: %s", c.Type.c_str());
		}
		ImGui::Unindent(5.0f);
	};

	virtual ImTextureID GetThumbnailIcon() override {
		auto texture = FAssetsLibrary::GetImage("Resources/Images/GUI/template_icon.png");
		ImTextureID id = (void*)(intptr_t)texture->GetTextureID(FApplication::Get()->EditorGUIViewport->ViewportContext);
		texture.reset();
		return id;
	}
};