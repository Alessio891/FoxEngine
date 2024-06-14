#pragma once

#include "Core.h"
#include "Graphics\Texture.h"
#include <string>
#include <iostream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "Graphics/BaseMaterial.h"
#include "AssetsLibrary/AssetResource.h"
#include "watcher.hpp"
#include <mutex>

using namespace wtr;

using json = nlohmann::json;

class FAssetsLibrary {

protected:
	static void HandleFileChanged(event e);
	static SharedPtr<watcher::watch> AssetWatcher;
	static std::thread WatcherThread;
	static BString LastModifiedFile;
	static long long LastModifiedTime;
	static std::mutex AssetsMutex;

public:
	static void Initialize();
	static void Shutdown();
	static void Update();
	static void HandleDroppedFiles(List<BString> droppedFiles, BString CurrentFolder);

	static void ImportFile(BString FilePath, BString TargetFolder);
	static void ProcessAsset(BString FilePath);
	static void DeleteAsset(BString Path);
	static void RefreshAssetsState();

	static bool AssetExists(BString path) {
		return Resources.count(path) > 0;
	}

	static SharedPtr<FTexture> GetImage(BString resourcePath) {
		BString sanitized = resourcePath;
		std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
		
		if (Resources.count(sanitized)) return GetResourceAs<FTexture>(sanitized);
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
		for(auto & imap : Resources)
		{
			if (imap.second->ResourceType == EAssetResourceType::Image) {
				keys.push_back(imap.first);
			}
		}

		return keys;
	}

	static List<BString> const GetAllResourcesOfType(EAssetResourceType type) {
		List<BString> keys;
		for (auto& imap : Resources)
		{
			if (imap.second->ResourceType == type) {
				keys.push_back(imap.first);
			}
		}

		return keys;
	}
private:
	static Map<BString, SharedPtr<FAssetResource>> Resources;
	//static Map<BString, SharedPtr<FAssetResource<FBaseMaterial>>>  MaterialResources;
};

static List<String> IMAGE_FILE_EXTENSIONS = {
	".png", "png",
	".jpg", "jpg",
	".bmp", "bmp"
};
static List<String> MAT_FILE_EXTENSIONS = {
	".mat",
	".mtl"
};
static List<String> TEMPLATES_FILE_EXTENSIONS = {
	".tmpl"
};
static List<String> SCRIPTS_FILE_EXTENSIONS = {
	".lua"
};
using FTextureResource = SharedPtr<FTexture>;
