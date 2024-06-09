#include "Editor/AssetsLibrary.h"
#include "Core.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

static List<String> IMAGE_FILE_EXTENSIONS = {
	".png",
	".jpg",
	".bmb"
};

void FAssetsLibrary::Initialize()
{
	BString path = "Resources/";
	for (const auto& entry : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(entry.path())) {
			BString filePath = entry.path().string();
			std::replace(filePath.begin(), filePath.end(), '\\', '/');
			BString extension = entry.path().extension().string();
			printf("File %s\n", filePath.c_str());
			bool isImage = (std::find(IMAGE_FILE_EXTENSIONS.begin(), IMAGE_FILE_EXTENSIONS.end(), extension) != IMAGE_FILE_EXTENSIONS.end());
			if (isImage) {
				printf("Found an image\n");
				ImageResources[filePath] = SharedPtr<FAssetResource<FTexture>>(new FAssetResource<FTexture>(EAssetResourceType::Image, filePath));
			}

		}
	}
}

Map<BString, SharedPtr<FAssetResource<FTexture>>>  FAssetsLibrary::ImageResources;