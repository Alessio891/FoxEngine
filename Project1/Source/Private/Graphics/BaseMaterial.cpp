#include "Graphics\BaseMaterial.h"
#include "Graphics\MaterialLibrary.h"
#include <fstream>>

#include <glm/gtc/type_ptr.hpp>

FBaseMaterial::FBaseMaterial(GLuint VertexShader, GLuint FragmentShader, const String MaterialName)
{
	this->VertexShader = VertexShader;
	this->FragmentShader = FragmentShader;
	ProgramIndex = glCreateProgram();
	glAttachShader(ProgramIndex, this->VertexShader);
	glAttachShader(ProgramIndex, this->FragmentShader);
	glLinkProgram(ProgramIndex);
}

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
