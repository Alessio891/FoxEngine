#pragma once

#include "Core.h"
#include "AssetResource.h"
#include "MeshRendererComponent.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
class FModelAsset : public FAssetResource {
public:
	FModelAsset(BString Path);

	virtual ImTextureID GetThumbnailIcon() override;
	virtual void DrawInspector() override;

	std::vector<SharedPtr<struct MeshData>> GetMeshes();

protected:
	std::vector<SharedPtr<struct MeshData>> meshes;
	void ProcessNode(aiNode* node, const aiScene* scene);
	SharedPtr<struct MeshData> ProcessMesh(aiMesh* mesh, const aiScene* scene);
};