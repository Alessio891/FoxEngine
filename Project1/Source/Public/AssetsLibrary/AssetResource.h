#pragma once

#include "Core.h"
#include "Application.h"
#include "IInspectable.h"
#include <string>
#include <algorithm>

enum EAssetResourceType {
	Image,
	Model,
	Sound,
	Script,
	Text,
	Material,
	Template,
	Misc
};

class FAssetResource : public IInspectable {

public:
	FAssetResource(EAssetResourceType type, BString p) : ResourceType(type), FilePath(p), IInspectable() {}

	BString FilePath;
	EAssetResourceType ResourceType;

	virtual void OnImported() {};
	virtual void OnDeleted() {};

	virtual bool IsLoaded(GLFWwindow* context) {
		return true;
	}

	virtual void ReloadResource(BString FilePath) {
		this->FilePath = FilePath;
		IsDirty = false;
	}
	virtual void Load(GLFWwindow* context, EAssetResourceType Type, BString FilePath) {
	}
	virtual void Unload() {

	}

	virtual void DrawResourceThumbnail();
	virtual void DrawInspector();

	std::string GetOnlyFileName(bool includeExtension = true) {
		BString sanitized = FilePath;
		std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
		std::string last_element(sanitized.substr(sanitized.rfind("/") + 1));
		if (includeExtension)
			return last_element;

		last_element = last_element.substr(0, last_element.rfind("."));
		return last_element;
	}


	virtual ImTextureID GetThumbnailIcon();

	bool IsDirty = false;
protected:
};