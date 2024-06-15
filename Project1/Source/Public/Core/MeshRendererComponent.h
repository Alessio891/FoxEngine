#pragma once
#include "Core.h"
#include "ObjectComponent.h"
#include "Logger.h"

#include "AssetsLibrary/AssetReference.h"
#include <Graphics/Graphics.h>

struct MeshData {

public:
	MeshData() = delete;
	MeshData(
		List<float> vertices,
		List<int> indices = {},
		List<float> normals = {},
		List<float> uvs = {}
	);

	GLuint DrawType = GL_TRIANGLES;
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	List<float> VertexArray;
	List<int> IndexArray;
	List<float> NormalsArray;
	List<float> UVs;
	FBoundingBox BoundingBox;
};

class FMeshRendererComponent : public FObjectComponent {

public:
	SharedPtr<MeshData> MeshDataRef;
	FAssetReference<class FBaseMaterial> Material;
	List<float> VertexColorsArray;

	void SetColor(Vector3F color);
	
	virtual void Tick(float Delta) override;
	virtual void Initialize(FSceneObject* Owner) override;

	virtual void Render(glm::mat4 V, glm::mat4 P);
	virtual void DrawInspector() override;

	void SetTexture(BString path);

	virtual void Deserialize(json json) override;

	Vector3F Color = Vector3F(1.0f, 1.0f, 1.0f);
	//SharedPtr<FTexture> Texture;
	FAssetReference<class FTexture> Texture;
};