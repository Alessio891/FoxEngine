#include "Editor/AssetsLibrary.h"
#include "Core.h"
#include <Logger.h>
#include "Graphics/MaterialLibrary.h"
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

void FAssetsLibrary::Initialize()
{
	BString path = "Resources/";
	for (const auto& entry : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(entry.path())) {
			BString filePath = entry.path().string();
			std::replace(filePath.begin(), filePath.end(), '\\', '/');
			BString extension = entry.path().extension().string();
			FLogger::LogInfo("Loading Resource " + filePath);
			
			bool isImage = (std::find(IMAGE_FILE_EXTENSIONS.begin(), IMAGE_FILE_EXTENSIONS.end(), extension) != IMAGE_FILE_EXTENSIONS.end());
			if (isImage) {
				
				ImageResources[filePath] = SharedPtr<FAssetResource<FTexture>>(new FAssetResource<FTexture>(EAssetResourceType::Image, filePath));
			}
			bool isMat = (std::find(MAT_FILE_EXTENSIONS.begin(), MAT_FILE_EXTENSIONS.end(), extension) != MAT_FILE_EXTENSIONS.end());
			if (isMat) {
				std::ifstream file(filePath.c_str());
				auto matJson = json::parse( file );
				SharedPtr<FBaseMaterial> Material(new FBaseMaterial(matJson));

				FMaterialLibrary::RegisterMaterial(Material);
				//MaterialResources[filePath] = SharedPtr<FAssetResource<FBaseMaterial>>(new FAssetResource<FBaseMaterial>(EAssetResourceType::Material, filePath));

			}
		}
	}
}

Map<BString, SharedPtr<FAssetResource<FTexture>>>  FAssetsLibrary::ImageResources;
//Map<BString, SharedPtr<FAssetResource<FBaseMaterial>>>  FAssetsLibrary::MaterialResources;