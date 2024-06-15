#include "AssetsLibrary/AssetsLibrary.h"
#include "Core.h"
#include <Logger.h>
#include "Graphics/MaterialLibrary.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "Graphics/Texture.h"
#include <filesystem>
#include <exception>
#include <AssetsLibrary/TemplateAsset.h>
#include "AssetsLibrary/ScriptAsset.h"
#include <algorithm>
#include <string>
#include "AssetsLibrary/ScriptAsset.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

void FAssetsLibrary::HandleFileChanged(event e)
{
	BString path = e.path_name.string();
	std::replace(path.begin(), path.end(), '\\', '/');
	BString delimiter = "/";
	size_t pos = 0;
	std::string token;
	while ((pos = path.find(delimiter)) != std::string::npos) {
		token = path.substr(0, pos);
		if (token == "Resources") {
			break;
		}
		else {
			path.erase(0, pos + delimiter.length());
		}
	}
	auto delta = e.effect_time - LastModifiedTime;
	if (path == LastModifiedFile &&  delta < 10000000) {
		LastModifiedFile = path;
		LastModifiedTime = e.effect_time;
		return;
	}
	if (e.effect_type == wtr::event::effect_type::modify) {
		LastModifiedFile = path;
		LastModifiedTime = e.effect_time;
		SharedPtr<FAssetResource> resource = GetResource(path);
		if (resource) {
			//FLogger::LogInfo("Something happened in resources:" + path + "\n" + std::to_string(delta));

			if (resource->ResourceType == EAssetResourceType::Script) {
				SharedPtr<FLuaScriptAsset> script = std::static_pointer_cast<FLuaScriptAsset>(resource);
				//script->Recompile();
				AssetsMutex.lock();
				script->IsDirty = true;
				AssetsMutex.unlock();
			}
		}
		else {
			ProcessAsset(path);
		}
	}
	else if (e.effect_type == event::effect_type::destroy) {
		SharedPtr<FAssetResource> resource = GetResource(path);
		if (resource) {
			resource->OnDeleted();
			Resources.erase(path);
		}
	}
	else if (e.effect_type == event::effect_type::rename) {
		ProcessAsset(path);
		RefreshAssetsState();
	}
	else if (e.effect_type == event::effect_type::create) {
		ProcessAsset(path);
	}
	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void FAssetsLibrary::Initialize()
{
	BString path = "Resources/";

	for (const auto& entry : fs::recursive_directory_iterator(path)) {
		if (fs::is_regular_file(entry.path())) {
			BString filePath = entry.path().string();
			ProcessAsset(filePath);
		}
	}

	WatcherThread = std::thread([path]() {
		AssetWatcher = std::make_shared<wtr::watcher::watch>("Resources/", &FAssetsLibrary::HandleFileChanged);
	});
	
}

void FAssetsLibrary::Shutdown()
{
	AssetWatcher.reset();
	WatcherThread.join();
}

void FAssetsLibrary::Update()
{
	for (auto res : Resources) {
		if (res.second->IsDirty) {
			if (AssetsMutex.try_lock()) {
				res.second->ReloadResource(res.second->FilePath);
				AssetsMutex.unlock();
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
		ProcessAsset(FilePath);
	}
	catch (std::exception& e) {
		FLogger::LogError("Error importing file " + FilePath + "\n" + e.what());
	}
}

void FAssetsLibrary::ProcessAsset(BString FilePath)
{
	std::replace(FilePath.begin(), FilePath.end(), '\\', '/');
	FLogger::LogInfo("Loading Resource " + FilePath);
	std::filesystem::path p(FilePath.c_str());
	BString extension = p.extension().string();

	bool isImage = (std::find(IMAGE_FILE_EXTENSIONS.begin(), IMAGE_FILE_EXTENSIONS.end(), extension) != IMAGE_FILE_EXTENSIONS.end());
	bool isMat = (std::find(MAT_FILE_EXTENSIONS.begin(), MAT_FILE_EXTENSIONS.end(), extension) != MAT_FILE_EXTENSIONS.end());
	bool isTemplate = (std::find(TEMPLATES_FILE_EXTENSIONS.begin(), TEMPLATES_FILE_EXTENSIONS.end(), extension) != TEMPLATES_FILE_EXTENSIONS.end());
	bool isScript = (std::find(SCRIPTS_FILE_EXTENSIONS.begin(), SCRIPTS_FILE_EXTENSIONS.end(), extension) != SCRIPTS_FILE_EXTENSIONS.end());
	if (isImage) {
		Resources[FilePath] = SharedPtr<FTexture>(new FTexture(FilePath));
	}
	else if (isMat) {
		std::ifstream file(FilePath.c_str());
		auto matJson = json::parse(file);
		SharedPtr<FBaseMaterial> Material(new FBaseMaterial(FilePath, matJson));

		FMaterialLibrary::RegisterMaterial(Material);
		Resources[FilePath] = Material;
		//MaterialResources[filePath] = SharedPtr<FAssetResource<FBaseMaterial>>(new FAssetResource<FBaseMaterial>(EAssetResourceType::Material, filePath));
	}
	else if (isTemplate) {
		std::ifstream file(FilePath.c_str());
		auto tmpltJson = json::parse(file);
		SharedPtr<FTemplateAsset> Template(new FTemplateAsset(FilePath));
		Template->LoadFromJson(tmpltJson);
		Resources[FilePath] = Template;
	}
	else if (isScript) {
		SharedPtr<FLuaScriptAsset> resource(new FLuaScriptAsset(FilePath));
		Resources[FilePath] = resource;
	}
	else {
		Resources[FilePath] = SharedPtr<FAssetResource>(new FAssetResource(EAssetResourceType::Misc, FilePath));
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
		FLogger::LogError("Error deleting file " + Path + "\n" + e.what());
	}
	
}
void FAssetsLibrary::RefreshAssetsState()
{
	List<BString> toRemove;
	for (auto kvp : Resources) {
		if (!kvp.second->FileStillExists()) {
			toRemove.push_back(kvp.first);
		}
	}

	for (BString f : toRemove) {
		Resources[f].reset();
		Resources.erase(f);
	}
}
std::mutex FAssetsLibrary::AssetsMutex;
BString FAssetsLibrary::LastModifiedFile;
long long FAssetsLibrary::LastModifiedTime;
std::thread FAssetsLibrary::WatcherThread;
SharedPtr<watcher::watch> FAssetsLibrary::AssetWatcher;
Map<BString, SharedPtr<FAssetResource>>  FAssetsLibrary::Resources;
//Map<BString, SharedPtr<FAssetResource<FBaseMaterial>>>  FAssetsLibrary::MaterialResources;