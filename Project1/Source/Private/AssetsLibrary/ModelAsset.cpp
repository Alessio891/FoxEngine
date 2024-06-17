#include "AssetsLibrary/ModelAsset.h"
#include <AssetsLibrary/AssetsLibrary.h>

#include "Logger.h"
FModelAsset::FModelAsset(BString Path) : FAssetResource(EAssetResourceType::Model, Path)
{
	
}

ImTextureID FModelAsset::GetThumbnailIcon()
{
	auto texture = FAssetsLibrary::GetImage("Resources/Images/GUI/mesh.png");
	ImTextureID id = (void*)(intptr_t)texture->GetTextureID(FApplication::Get()->GameViewport->ViewportContext);
	texture.reset();
	return id;
}

void FModelAsset::DrawInspector()
{
}

std::vector<SharedPtr<struct MeshData>> FModelAsset::GetMeshes()
{
	if (meshes.empty() && FileStillExists()) {
		Assimp::Importer import;
		const aiScene * scene = import.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			FLogger::LogError("Couldn't load model " + FilePath);
			return meshes;
		}
		ProcessNode(scene->mRootNode, scene);
	}
	return meshes;
}

void FModelAsset::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

SharedPtr<MeshData> FModelAsset::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	List<float> vertices;
	List<float> normals;
	List<float> uvs;
	List<int> indices;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);
		if (mesh->HasNormals()) {
			normals.push_back(mesh->mNormals[i].x);
			normals.push_back(mesh->mNormals[i].y);
			normals.push_back(mesh->mNormals[i].z);
		}
		if (mesh->HasTextureCoords(0)) {
			uvs.push_back(mesh->mTextureCoords[0][i].x);
			uvs.push_back(mesh->mTextureCoords[0][i].y);
		}
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	SharedPtr<MeshData> meshData = SharedPtr<MeshData>(new MeshData(vertices, indices, normals, uvs));

	return meshData;
}
