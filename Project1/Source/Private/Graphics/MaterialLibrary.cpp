#include "Graphics\MaterialLibrary.h"
#include <fstream>

GLuint FMaterialLibrary::GetShader(String ShaderPath, GLenum ShaderType)
{
	auto it = CachedShaders.find(ShaderPath);
	if (it != CachedShaders.end()) {
		return CachedShaders[ShaderPath];
	}
	std::string shaderSource = LoadShaderSource(ShaderPath);

	GLuint shader = glCreateShader(ShaderType);
	GLchar* c = (GLchar*)shaderSource.c_str();
	glShaderSource(shader, 1, &c, NULL);
	glCompileShader(shader);
	
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen) {
			char* buf = (char*)malloc(infoLen);
			printf("Error compiling shader: %d \n %s \n", ShaderType, buf);
			free(buf);
		}

		glDeleteShader(shader);
		shader = 0;
	}
	else {
		printf("Shader %s compiled\n", ShaderPath);
	}
	CachedShaders[ShaderPath] = shader;
	
	return shader;
}

std::string FMaterialLibrary::LoadShaderSource(String pathToFile)
{
	std::string content;
	std::ifstream fileStream(pathToFile, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Couldn't load shader file %s\n", pathToFile);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	return content;
}

Map<String, SharedPtr<FBaseMaterial>> FMaterialLibrary::CachedMaterials;
Map<String, GLuint> FMaterialLibrary::CachedShaders;