#include "Graphics\BaseMaterial.h"
#include "Graphics\MaterialLibrary.h"
#include <fstream>>
#include <glm/gtc/type_ptr.hpp>
#include "Core.h"
#include <Application.h>
#include "imgui_stdlib.h"
#include <Editor/AssetsLibrary.h>

void FBaseMaterial::SetFloat(String ParamName, float Value)
{
	FloatParams[ParamName] = Value;
}

void FBaseMaterial::SetMatrix(String ParamName, glm::mat4 Value)
{
	MatParams[ParamName] = Value;
}

void FBaseMaterial::SetVec3(String ParamName, Vector3F Value)
{
	VectorParams[ParamName] = Value;
}

void FBaseMaterial::UploadParameters()
{
	for (auto floatParam = FloatParams.begin(); floatParam != FloatParams.end(); ++floatParam) {
		GLuint paramID = glGetUniformLocation(ProgramIndex, floatParam->first);
		glUniform1f(paramID, floatParam->second);
	}
	for (auto vecParam = VectorParams.begin(); vecParam != VectorParams.end(); ++vecParam) {
		GLuint paramID = glGetUniformLocation(ProgramIndex, vecParam->first);
		glUniform3fv(paramID, 1, glm::value_ptr( vecParam->second ));
	}
	for (auto matParam = MatParams.begin(); matParam != MatParams.end(); ++matParam) {
		GLuint paramID = glGetUniformLocation(ProgramIndex, matParam->first);
		glUniformMatrix4fv(paramID, 1, GL_FALSE, &matParam->second[0][0]);
	}
}

void FBaseMaterial::Deserialize(json Json)
{
	glfwMakeContextCurrent(FApplication::Get()->EditorGUIViewport->ViewportContext);
	Json["Name"].get_to(Name);	
	Json["VertexShaderPath"].get_to(VertexShaderPath);
	Json["FragmentShaderPath"].get_to(FragmentShaderPath);
	this->VertexShader = FMaterialLibrary::GetShader(VertexShaderPath.c_str(), GL_VERTEX_SHADER);
	this->FragmentShader = FMaterialLibrary::GetShader(FragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);
	ProgramIndex = glCreateProgram();
	glAttachShader(ProgramIndex, this->VertexShader);
	glAttachShader(ProgramIndex, this->FragmentShader);
	glLinkProgram(ProgramIndex);
}

void FBaseMaterial::Serialize(json& outJson)
{
	outJson["Name"] = Name;
	//retVal["Vertex"] = VertexShader
	for (auto f : FloatParams) {
		outJson[f.first] = f.second;
	}
	for (auto f : VectorParams) {
		std::string s = std::to_string(f.second.x) + "," + std::to_string(f.second.y) + "," + std::to_string(f.second.z);
		outJson[f.first] = s;
	}
	/*for (auto f : FloatParams) {
		retVal[f.first] = f.second;
	}*/
}

void FBaseMaterial::DrawInspector()
{
	ImGui::InputText("VertexShader", &VertexShaderPath);
	ImGui::InputText("FragmentShader", &FragmentShaderPath);

	ImGui::Text("Mat ID: %d", ProgramIndex);
	ImGui::Text("VertexShader ID: %d", VertexShader);
	ImGui::Text("FragmentShader ID: %d", FragmentShader);

	if (ImGui::Button("Recompile")) {
		auto oldCtx = glfwGetCurrentContext();
		glfwMakeContextCurrent( FApplication::Get()->EditorGUIViewport->ViewportContext );
		
		glDeleteProgram(ProgramIndex);

		this->VertexShader = FMaterialLibrary::GetShader(VertexShaderPath, GL_VERTEX_SHADER);
		this->FragmentShader = FMaterialLibrary::GetShader(FragmentShaderPath, GL_FRAGMENT_SHADER);
		
		this->VertexShader = FMaterialLibrary::GetShader(VertexShaderPath.c_str(), GL_VERTEX_SHADER);
		this->FragmentShader = FMaterialLibrary::GetShader(FragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);
		ProgramIndex = glCreateProgram();
		glAttachShader(ProgramIndex, this->VertexShader);
		glAttachShader(ProgramIndex, this->FragmentShader);
		glLinkProgram(ProgramIndex);
		glfwMakeContextCurrent(oldCtx);
	}
}

ImTextureID FBaseMaterial::GetThumbnailIcon()
{
	return (void*)(intptr_t)FAssetsLibrary::GetImage("Resources/Images/GUI/mat_icon.png")->GetTextureID(FApplication::Get()->EditorGUIViewport->ViewportContext);
}
