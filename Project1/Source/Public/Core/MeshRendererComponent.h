#pragma once
#include "Core.h"
#include "ObjectComponent.h"
#include "Graphics\BaseMaterial.h"

struct MeshData {

public:
	GLuint DrawType = GL_TRIANGLES;
	GLuint VBO;
	GLuint VAO;

	List<float> VertexArray;
};

class FMeshRendererComponent : public FObjectComponent {

public:
	SharedPtr<MeshData> MeshData;
	SharedPtr<FBaseMaterial> Material;
	List<float> VertexColorsArray;


	virtual void Tick(float Delta) override;
	virtual void Initialize(FSceneObject* Owner) override;

	virtual void Render(glm::mat4 V, glm::mat4 P);
	virtual void DrawInspector() override;

	Vector3F Color = Vector3F(1.0f, 1.0f, 1.0f);
};