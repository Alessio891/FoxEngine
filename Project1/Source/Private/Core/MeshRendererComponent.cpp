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
	Material->SetFloat("_Unlit", 0.0f);
	Material->SetVec3("_LightData.Directional.Direction", -FApplication::Get()->GetCurrentScene()->LightTransform.GetForwardVector());
	Material->SetVec3("_LightData.Directional.Color", FApplication::Get()->GetCurrentScene()->LightColor);
	Material->SetFloat("_LightData.Directional.Intensity", FApplication::Get()->GetCurrentScene()->Intensity);
	Material->SetVec3("_LightData.AmbientColor", FApplication::Get()->GetCurrentScene()->AmbientColor);
	Material->SetFloat("_LightData.AmbientIntensity", 1.0f);

	if (Material->Texture != nullptr) {
		glBindTexture(GL_TEXTURE_2D, Material->Texture->GetTextureID());
	}

	Material->UploadParameters();
	glBindVertexArray(MeshData->VAO);
	
	if (MeshData->IndexArray.size() > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshData->EBO);
		glDrawElements(MeshData->DrawType, MeshData->IndexArray.size(), GL_UNSIGNED_INT, 0);
	}
	else 
		glDrawArrays(MeshData->DrawType, 0, MeshData->VertexArray.size() / 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	// Second Pass
	// Render the object scaled up, masked with the stencil buffer
	// Where Stencil != ObjectID
	if (Owner->Outlined) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glm::mat4 M = glm::scale(Owner->Transform.GetTransformMatrix(), glm::vec3(1.05f, 1.05f, 1.05f));
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
		Material->SetFloat("_Unlit", 1.0f);
		Material->UploadParameters();
		glStencilFunc(GL_NOTEQUAL, Owner->ObjectID, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		if (MeshData->IndexArray.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshData->EBO);
			glDrawElements(MeshData->DrawType, MeshData->IndexArray.size(), GL_UNSIGNED_INT, 0);
		}
		else
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
