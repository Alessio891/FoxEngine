#include "MeshRendererComponent.h"

#include "Core.h"
#include <InspectorModule/InspectorModule.h>
#include <glm/ext/matrix_transform.hpp>

void FMeshRendererComponent::Tick(float Delta)
{
}

void FMeshRendererComponent::Initialize(FSceneObject* Owner)
{
	FObjectComponent::Initialize(Owner);
	
	if (MeshData->VBO <= 0) {
		
		glGenVertexArrays(1, &MeshData->VAO);
		glGenBuffers(1, &MeshData->VBO);

		glBindVertexArray(MeshData->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, MeshData->VBO);
		
		const int size = MeshData->VertexArray.size();
		float* arr = new float[size];
		std::copy(MeshData->VertexArray.begin(), MeshData->VertexArray.end(), arr);

		glBufferData(GL_ARRAY_BUFFER, sizeof(arr) * MeshData->VertexArray.size(), arr, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}

void FMeshRendererComponent::Render(glm::mat4 V, glm::mat4 P)
{
	
	glUseProgram(Material->GetProgram());
	
	// Normal Pass
	// In this pass the Object ID is written to the stencil buffer
	glm::mat4 MVP = P * V * Owner->Transform.GetTransformMatrix();
	GLuint MatrixID = glGetUniformLocation(Material->GetProgram(), "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 m = Owner->Transform.GetTransformMatrix();
	GLuint M_MatrixID = glGetUniformLocation(Material->GetProgram(), "_M");
	glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &m[0][0]);
	GLuint V_MatrixID = glGetUniformLocation(Material->GetProgram(), "_V");
	glUniformMatrix4fv(V_MatrixID, 1, GL_FALSE, &V[0][0]);
	GLuint P_MatrixID = glGetUniformLocation(Material->GetProgram(), "_P");
	glUniformMatrix4fv(P_MatrixID, 1, GL_FALSE, &P[0][0]);

	Material->SetVec3("_Color", Color);
	Material->UploadParameters();
	glBindVertexArray(MeshData->VAO);
	glDrawArrays(MeshData->DrawType, 0, MeshData->VertexArray.size() / 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Second Pass
	// Render the object scaled up, masked with the stencil buffer
	// Where Stencil != ObjectID
	if (Owner->Outlined) {
		glm::mat4 M = glm::scale(Owner->Transform.GetTransformMatrix(), glm::vec3(1.01f, 1.01f, 1.01f));
		MVP = P * V * M;
		MatrixID = glGetUniformLocation(Material->GetProgram(), "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		M_MatrixID = glGetUniformLocation(Material->GetProgram(), "_M");
		glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &M[0][0]);
		V_MatrixID = glGetUniformLocation(Material->GetProgram(), "_V");
		glUniformMatrix4fv(V_MatrixID, 1, GL_FALSE, &V[0][0]);
		P_MatrixID = glGetUniformLocation(Material->GetProgram(), "_P");
		glUniformMatrix4fv(P_MatrixID, 1, GL_FALSE, &P[0][0]);

		Material->SetVec3("_Color", Vector3F(0.02f, 0.373f, 0.95f));
		Material->UploadParameters();
		glStencilFunc(GL_NOTEQUAL, Owner->ObjectID, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		glDrawArrays(MeshData->DrawType, 0, MeshData->VertexArray.size() / 3);

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
		if (Material != nullptr) {
			float c[3] = { Color.x, Color.y, Color.z };
			ImGui::ColorEdit3("Color", c, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel);
			Color.x = c[0];
			Color.y = c[1];
			Color.z = c[2];

			for (auto f : Material->FloatParams) {
				ImGui::DragFloat(f.first, &f.second);
			}
			for (auto v : Material->VectorParams) {
				if (v.first == "_Color") continue;
				Material->VectorParams[v.first] = FImGui::Vec3(v.first, v.second);
			}
		}
	}

	if (ImGui::CollapsingHeader("Mesh Data")) { 
		
	}
}
