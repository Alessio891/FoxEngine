#pragma once

#include "Core.h"
#include "Graphics\Texture.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <filesystem>
enum EAssetResourceType {
	Image,
	Model,
	Sound,
	Text,
	Misc
};

template <class T>
class FAssetResource {
public:
	FAssetResource(EAssetResourceType type, BString p) : ResourceType(type), FilePath(p) {}

	BString FilePath;
	EAssetResourceType ResourceType;

	SharedPtr<T> GetResource(GLFWwindow* context) { 
		if (!IsLoaded(context)) Load(context, ResourceType, FilePath);
		return Resources[context];
	}

	virtual bool IsLoaded(GLFWwindow* context) { 
		return Resources.count(context) > 0;
	}
	virtual void ReloadResource(BString FilePath) {
		this->FilePath = FilePath;
		Resources.clear();
	}
	virtual void Load(GLFWwindow* context, EAssetResourceType Type, BString FilePath) {
		
		glfwMakeContextCurrent(context);

		ResourceType = Type;
		this->FilePath = FilePath;
		switch(Type) {
			case EAssetResourceType::Image:
				SharedPtr<FTexture> t(new FTexture());
				t->Load(FilePath);
				Resources[context] = t;
			break;
		}
	}
	virtual void Unload() {

	}

protected:
	Map<GLFWwindow*, SharedPtr<T>> Resources;
};

class FAssetsLibrary {

public:
	static void Initialize();

	static SharedPtr<FAssetResource<FTexture>> GetImage(BString resourcePath) {
		BString sanitized = resourcePath;
		std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
		if (ImageResources.count(sanitized)) return ImageResources[sanitized];
		return nullptr;
	}

	static List<BString> const GetAllImageResourcesKeys() {
		List<BString> keys;
		for(auto & imap : ImageResources)
			keys.push_back(imap.first);

		return keys;
	}
private:
	static Map<BString, SharedPtr<FAssetResource<FTexture>>>  ImageResources;
};

static List<String> IMAGE_FILE_EXTENSIONS = {
	".png",
	".jpg",
	".bmb"
};