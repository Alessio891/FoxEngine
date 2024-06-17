#include "MeshRendererComponent.h"

#include "Core.h"
#include <InspectorModule/InspectorModule.h>
#include <glm/ext/matrix_transform.hpp>
#include "Graphics\BaseMaterial.h"
#include "GUI/GUI.h"
#include "Graphics\Texture.h"
#include "Graphics\Primitives.h"
#include <Graphics/MaterialLibrary.h>

void FMeshRendererComponent::SetColor(Vector3F color)
{
	Color = color;
}

void FMeshRendererComponent::Tick(float Delta)
{
}

void FMeshRendererComponent::Initialize(FSceneObject* Owner)
{
	FObjectComponent::Initialize(Owner);
	
	if (MeshDataRef->VBO <= 0) {
		
		
	}

}

void FMeshRendererComponent::Render(glm::mat4 V, glm::mat4 P)
{
	if (!Material.IsValid()) return;
	if (UseMeshAsset && !MeshAsset.IsValid()) return;

	glUseProgram(Material.Get()->GetProgram());
	
	// Normal Pass
	// In this pass the Object ID is written to the stencil buffer
	glm::mat4 MVP = P * V * Owner->Transform.GetTransformMatrix();
	GLuint MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 m = Owner->Transform.GetTransformMatrix();
	GLuint M_MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "_M");
	glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &m[0][0]);
	GLuint V_MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "_V");
	glUniformMatrix4fv(V_MatrixID, 1, GL_FALSE, &V[0][0]);
	GLuint P_MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "_P");
	glUniformMatrix4fv(P_MatrixID, 1, GL_FALSE, &P[0][0]);

	Material.Get()->SetVec3("_Color", Color);
	Material.Get()->SetFloat("_Unlit", 0.0f);
	Material.Get()->SetVec3("_LightData.Directional.Direction", -FApplication::Get()->GetCurrentScene()->LightTransform.GetForwardVector());
	Material.Get()->SetVec3("_LightData.Directional.Color", FApplication::Get()->GetCurrentScene()->LightColor);
	Material.Get()->SetFloat("_LightData.Directional.Intensity", FApplication::Get()->GetCurrentScene()->Intensity);
	Material.Get()->SetVec3("_LightData.AmbientColor", FApplication::Get()->GetCurrentScene()->AmbientColor);
	Material.Get()->SetFloat("_LightData.AmbientIntensity", 1.0f);

	Material.Get()->SetVec3("_CameraPos", FApplication::Get()->GetCurrentScene()->CameraTransform.Position);

	Material.Get()->UploadParameters();
	GLuint loc = glGetUniformLocation(Material.Get()->GetProgram(), "_UseTexture");
	if (Texture.IsValid()) {
		
		auto textureId = Texture.Get()->GetTextureID(FApplication::Get()->GameViewport->ViewportContext);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(loc, 1);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(loc, 0);
	}

	if (UseMeshAsset) {
		for (int i = 0; i < MeshAsset.Get()->GetMeshes().size(); i++) {
			auto meshData = MeshAsset.Get()->GetMeshes()[i];
			glBindVertexArray(meshData->VAO);
	
			if (meshData->IndexArray.size() > 0) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->EBO);
				glDrawElements(meshData->DrawType, meshData->IndexArray.size(), GL_UNSIGNED_INT, 0);
			}
			else 
				glDrawArrays(meshData->DrawType, 0, meshData->VertexArray.size() / 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		}
	}
	else {
		auto meshData =MeshDataRef;
		glBindVertexArray(meshData->VAO);

		if (MeshDataRef->IndexArray.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->EBO);
			glDrawElements(meshData->DrawType, meshData->IndexArray.size(), GL_UNSIGNED_INT, 0);
		}
		else
			glDrawArrays(meshData->DrawType, 0, meshData->VertexArray.size() / 3);
	}
	
	// Second Pass
	// Render the object scaled up, masked with the stencil buffer
	// Where Stencil != ObjectID
	if (Owner->Outlined) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glm::mat4 M = glm::scale(Owner->Transform.GetTransformMatrix(), glm::vec3(1.05f, 1.05f, 1.05f));
		MVP = P * V * M;
		MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		M_MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "_M");
		glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &M[0][0]);
		V_MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "_V");
		glUniformMatrix4fv(V_MatrixID, 1, GL_FALSE, &V[0][0]);
		P_MatrixID = glGetUniformLocation(Material.Get()->GetProgram(), "_P");
		glUniformMatrix4fv(P_MatrixID, 1, GL_FALSE, &P[0][0]);

		Material.Get()->SetVec3("_Color", Vector3F(0.02f, 0.373f, 0.95f));
		Material.Get()->SetFloat("_Unlit", 1.0f);
		Material.Get()->UploadParameters();
		glStencilFunc(GL_NOTEQUAL, Owner->ObjectID, 0xFF);
		glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);

		if (UseMeshAsset) {
			for (int i = 0; i < MeshAsset.Get()->GetMeshes().size(); i++) {
				auto meshData = MeshAsset.Get()->GetMeshes()[i];
				glBindVertexArray(meshData->VAO);

				if (meshData->IndexArray.size() > 0) {
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->EBO);
					glDrawElements(meshData->DrawType, meshData->IndexArray.size(), GL_UNSIGNED_INT, 0);
				}
				else
					glDrawArrays(meshData->DrawType, 0, meshData->VertexArray.size() / 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
			}
		}
		else {
			auto meshData = MeshDataRef;
			glBindVertexArray(meshData->VAO);

			if (MeshDataRef->IndexArray.size() > 0) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->EBO);
				glDrawElements(meshData->DrawType, meshData->IndexArray.size(), GL_UNSIGNED_INT, 0);
			}
			else
				glDrawArrays(meshData->DrawType, 0, meshData->VertexArray.size() / 3);
		}

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}
	
}

void FMeshRendererComponent::DrawInspector()
{
	ImGui::SeparatorText("Mesh Renderer");
	static bool matShown = true;
	if (ImGui::CollapsingHeader("Material", matShown)) {

		FGUI::Material("Material", Material);

		//FGUI::Material("Material", Material);

		if (Material.IsValid()) {
			
			FGUI::Color("Material Color", Color);
			
			for (auto f : Material.Get()->FloatParams) {
				if (f.first[0] == '_') continue;
				ImGui::DragFloat(f.first, &f.second);
			}
			for (auto v : Material.Get()->VectorParams) {
				if (v.first[0] == '_') continue;
				Material.Get()->VectorParams[v.first] = FImGui::Vec3(v.first, v.second);
			}
			FGUI::Texture("Main Texture", Texture);
		}
	}
	
	FGUI::AssetReference<FModelAsset>("Mesh", MeshAsset);

	if (MeshDataRef != nullptr) {
		if (ImGui::CollapsingHeader("Mesh Data")) { 
			std::vector<BString> options = { "Points", "Lines", "Line Loop", "Line Strip", "Triangles", "Triangle Strips", "Triangle Fan", "Quads", "Quad Strip", "Polygon"};
			FGUI::EnumPopup("DrawType", (int&)MeshDataRef->DrawType, options);
		
		}
	}
}

void FMeshRendererComponent::SetTexture(BString path)
{
	Texture.Set(FAssetsLibrary::GetImage(path));
}

void FMeshRendererComponent::Deserialize(json json)
{
	BString meshDataType;
	json["MeshData"].get_to(meshDataType);

	SharedPtr<MeshData> meshData;
	if (meshDataType == "Cube") {
		meshData = SharedPtr<MeshData>(new MeshData(CUBE_MESH_VERTICES, CUBE_MESH_INDICES, CUBE_MESH_NORMALS, CUBE_MESH_UVS));
	}
	else if (meshDataType == "Pyramid") {
		meshData = SharedPtr<MeshData>(new MeshData(PYRAMID_MESH_VERTICES, PYRAMID_MESH_INDICES, PYRAMID_MESH_NORMALS, PYRAMID_MESH_UVS));
	}
	else if (meshDataType == "Sphere") {
		meshData = generateSphereMesh(1.0f, 30);
	}

	this->MeshDataRef = meshData;

	BString mat;
	json["Material"].get_to(mat);
	Material.Set(FMaterialLibrary::GetMaterial(mat.c_str()));

	BString texture;
	json["Texture"].get_to(texture);
	Texture.Set(FAssetsLibrary::GetImage(texture));
	
}

MeshData::MeshData(List<float> vertices, List<int> indices, List<float> normals, List<float> uvs) : VertexArray(vertices), IndexArray(indices), NormalsArray(normals), UVs(uvs) {

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
	
	BoundingBox.Min = Vector3F(0,0,0);
	BoundingBox.Max = Vector3F(0,0,0);
	
	for (size_t i = 0; i < vertices.size(); i += 3) {
		float x = v[i];
		float y = v[i+1];
		float z = v[i+2];

		if (x < BoundingBox.Min.x) BoundingBox.Min.x = x;
		if (x > BoundingBox.Max.x) BoundingBox.Max.x = x;
		if (y < BoundingBox.Min.y) BoundingBox.Min.y = y;
		if (y > BoundingBox.Max.y) BoundingBox.Max.y = y;
		if (z < BoundingBox.Min.z) BoundingBox.Min.z = z;
		if (z > BoundingBox.Max.z) BoundingBox.Max.z = z;

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
