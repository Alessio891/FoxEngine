#include "Editor/AssetsLibrary.h"
#include "Core.h"
#include <Logger.h>
#include "Graphics/MaterialLibrary.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "Graphics/Texture.h"
#include <iostream>>
#include <filesystem>
#include <exception>
#include <AssetsLibrary/TemplateAsset.h>
#include "AssetsLibrary/ScriptAsset.h"

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
			bool isMat = (std::find(MAT_FILE_EXTENSIONS.begin(), MAT_FILE_EXTENSIONS.end(), extension) != MAT_FILE_EXTENSIONS.end());
			bool isTemplate = (std::find(TEMPLATES_FILE_EXTENSIONS.begin(), TEMPLATES_FILE_EXTENSIONS.end(), extension) != TEMPLATES_FILE_EXTENSIONS.end());
			bool isScript = (std::find(SCRIPTS_FILE_EXTENSIONS.begin(), SCRIPTS_FILE_EXTENSIONS.end(), extension) != SCRIPTS_FILE_EXTENSIONS.end());
			if (isImage) {
				Resources[filePath] = SharedPtr<FTexture>(new FTexture(filePath));
			} else if (isMat) {
				std::ifstream file(filePath.c_str());
				auto matJson = json::parse( file );
				SharedPtr<FBaseMaterial> Material(new FBaseMaterial(filePath, matJson));

				FMaterialLibrary::RegisterMaterial(Material);
				Resources[filePath] = Material;
				//MaterialResources[filePath] = SharedPtr<FAssetResource<FBaseMaterial>>(new FAssetResource<FBaseMaterial>(EAssetResourceType::Material, filePath));
			}
			else if (isTemplate) {
				std::ifstream file(filePath.c_str());
				auto tmpltJson = json::parse(file);
				SharedPtr<FTemplateAsset> Template(new FTemplateAsset(filePath));
				Template->LoadFromJson(tmpltJson);
				Resources[filePath] = Template;
			}
			else if (isScript) {
				SharedPtr<FLuaScriptAsset> resource(new FLuaScriptAsset(filePath));
				Resources[filePath] = resource;
			}
			else {
				Resources[filePath] = SharedPtr<FAssetResource>(new FAssetResource(EAssetResourceType::Misc, filePath));
			}

		}
	}
}

void FAssetsLibrary::HandleDroppedFiles(List<BString> droppedFiles, BString CurrentFolder)
{
	for (BString file : droppedFiles) {
		ImportFile(file, CurrentFolder);
	}
	//Resources.clear();

	// FixMe: We don't want to reinitialize all the assets library every time we import some files.
	// we should make a Reload method that will somehow check if the actual source file has changed
	//Initialize();
}

void FAssetsLibrary::ImportFile(BString FilePath, BString TargetFolder)
{
	BString sanitized = FilePath;
	std::replace(sanitized.begin(), sanitized.end(), '\\', '/');
	BString fileName(sanitized.substr(sanitized.rfind("/") + 1));
	BString dest = TargetFolder + "/" + fileName;

	try {
		fs::copy_file(FilePath, dest, fs::copy_options::overwrite_existing);
		std::replace(dest.begin(), dest.end(), '\\', '/');
		std::string extension(dest.substr(dest.rfind(".") + 1));
		bool isImage = (std::find(IMAGE_FILE_EXTENSIONS.begin(), IMAGE_FILE_EXTENSIONS.end(), extension) != IMAGE_FILE_EXTENSIONS.end());
		bool isMat = (std::find(MAT_FILE_EXTENSIONS.begin(), MAT_FILE_EXTENSIONS.end(), extension) != MAT_FILE_EXTENSIONS.end());
		if (isImage) {
			Resources[dest] = SharedPtr<FTexture>(new FTexture(dest));
		}
		else if (isMat) {
			std::ifstream file(dest.c_str());
			auto matJson = json::parse(file);
			SharedPtr<FBaseMaterial> Material(new FBaseMaterial(dest, matJson));

			FMaterialLibrary::RegisterMaterial(Material);
			Resources[dest] = Material;
			//MaterialResources[filePath] = SharedPtr<FAssetResource<FBaseMaterial>>(new FAssetResource<FBaseMaterial>(EAssetResourceType::Material, filePath));
		}
		else {
			Resources[dest] = SharedPtr<FAssetResource>(new FAssetResource(EAssetResourceType::Misc, dest));
		}
	}
	catch (std::exception& e) {
		FLogger::LogError("Error importing file " + FilePath);
	}
}

void FAssetsLibrary::DeleteAsset(BString Path)
{
	try {
		fs::remove(Path);
		if (Resources.count(Path) > 0) {
			Resources[Path]->OnDeleted();
			Resources[Path].reset();
			Resources.erase(Path);
		}

		/*ImageResources.clear();
		Resources.clear();

		// FixMe: We don't want to reinitialize all the assets library every time we import some files.
		// we should make a Reload method that will somehow check if the actual source file has changed
		Initialize();*/
		
	}
	catch (std::exception& e) {
		FLogger::LogError("Error deleting file " + Path);
	}
	
}

Map<BString, SharedPtr<FAssetResource>>  FAssetsLibrary::Resources;
//Map<BString, SharedPtr<FAssetResource<FBaseMaterial>>>  FAssetsLibrary::MaterialResources;