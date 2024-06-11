#pragma once
#include "Core.h"
#include "ObjectComponent.h"
#include "Graphics\BaseMaterial.h"
#include "Logger.h"

#include "Editor/AssetsLibrary.h"

struct MeshData {

public:
	MeshData() = delete;
	MeshData(
		List<float> vertices,
		List<int> indices = {},
		List<float> normals = {},
		List<float> uvs = {}
	) : VertexArray(vertices), IndexArray(indices), NormalsArray(normals), UVs(uvs) {

		if (normals.size() > 0 && normals.size() != vertices.size()) {
			FLogger::LogError("Couldn't initialize mesh data: Vertex and Normals size mismatch.");
			return;
		}
		int stride = 3;
		//if (normals.size() > 0) {
		stride = 8;
		List<float> actualVertices;
		const int v_size = vertices.size();
		float* v = new float[v_size];
		float* n = new float[v_size];
		float* u = new float[v_size];

		std::copy(vertices.begin(), vertices.end(), v);
		std::copy(normals.begin(), normals.end(), n);
		std::copy(uvs.begin(), uvs.end(), u);

		for (size_t i = 0; i < vertices.size(); i += 3) {
			// Vertices
			actualVertices.push_back(v[i]);
			actualVertices.push_back(v[i + 1]);
			actualVertices.push_back(v[i + 2]);

			// Normals (if available)
			if (i / 3 < normals.size() / 3) {
				actualVertices.push_back(n[i]);
				actualVertices.push_back(n[i + 1]);
				actualVertices.push_back(n[i + 2]);
			}
			else {
				// Default normal
				actualVertices.push_back(0.0f);
				actualVertices.push_back(1.0f);
				actualVertices.push_back(0.0f);
			}

			// UVs (if available)
			if (i / 3 < uvs.size() / 2) {
				actualVertices.push_back(u[i / 3 * 2]);
				actualVertices.push_back(u[i / 3 * 2 + 1]);
			}
			else {
				// Default UV
				actualVertices.push_back(0.0f);
				actualVertices.push_back(0.0f);
			}
		}
		VertexArray = actualVertices;


		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		const int size = VertexArray.size();
		float* arr = new float[size];
		std::copy(VertexArray.begin(), VertexArray.end(), arr);

		glBufferData(GL_ARRAY_BUFFER, sizeof(arr) * VertexArray.size(), arr, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		const int i_size = IndexArray.size();
		int* i_arr = new int[i_size];
		std::copy(IndexArray.begin(), IndexArray.end(), i_arr);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i_arr) * IndexArray.size(), i_arr, GL_STATIC_DRAW);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	GLuint DrawType = GL_TRIANGLES;
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	List<float> VertexArray;
	List<int> IndexArray;
	List<float> NormalsArray;
	List<float> UVs;
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

	virtual void SetTexture(BString path);

	Vector3F Color = Vector3F(1.0f, 1.0f, 1.0f);
	//SharedPtr<FTexture> Texture;
	SharedPtr<FTexture> Texture;
};