#pragma once

#include "Core.h"
#include "Graphics\Texture.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "Graphics/BaseMaterial.h"
#include "AssetsLibrary/AssetResource.h"

using json = nlohmann::json;

class FAssetsLibrary {

public:
	static void Initialize();

	static SharedPtr<FTexture> GetImage(BString resourcePath) {
		BString sanitized = resourcePath;
		std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
		
		if (ImageResources.count(sanitized)) return ImageResources[sanitized];
		return nullptr;
	}

	static SharedPtr<FAssetResource> GetResource(BString resourcePath) {
		BString sanitized = resourcePath;
		std::replace(sanitized.begin(), sanitized.end(), '\\', '/');

		if (Resources.count(sanitized)) return Resources[sanitized];
		return nullptr;
	}

	template<class T>
	static SharedPtr<T> GetResourceAs(BString resourcePath) {
		auto resource = GetResource(resourcePath);
		if (resource) {
			return std::dynamic_pointer_cast<T>(resource);
		}
		return nullptr;
	}

	static List<BString> const GetAllImageResourcesKeys() {
		List<BString> keys;
		for(auto & imap : ImageResources)
			keys.push_back(imap.first);

		return keys;
	}
private:
	static Map<BString, SharedPtr<FAssetResource>> Resources;
	static Map<BString, SharedPtr<FTexture>>  ImageResources;
	//static Map<BString, SharedPtr<FAssetResource<FBaseMaterial>>>  MaterialResources;
};

static List<String> IMAGE_FILE_EXTENSIONS = {
	".png",
	".jpg",
	".bmb"
};
static List<String> MAT_FILE_EXTENSIONS = {
	".mat",
	".mtl"
};
using FTextureResource = SharedPtr<FTexture>;
