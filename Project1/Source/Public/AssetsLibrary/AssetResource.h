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
	Text,
	Material,
	Misc
};

class FAssetResource : public IInspectable {

public:
	FAssetResource(EAssetResourceType type, BString p) : ResourceType(type), FilePath(p), IInspectable() {}

	BString FilePath;
	EAssetResourceType ResourceType;

	virtual bool IsLoaded(GLFWwindow* context) {
		return true;
	}

	virtual void ReloadResource(BString FilePath) {
		this->FilePath = FilePath;
	}
	virtual void Load(GLFWwindow* context, EAssetResourceType Type, BString FilePath) {
	}
	virtual void Unload() {

	}

	virtual void DrawResourceThumbnail();
	virtual void DrawInspector();

	std::string GetOnlyFileName() {
		BString sanitized = FilePath;
		std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
		std::string last_element(sanitized.substr(sanitized.rfind("/") + 1));

		return last_element;
	}


protected:
	virtual ImTextureID GetThumbnailIcon();
};