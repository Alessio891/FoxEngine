#include "Graphics\MaterialLibrary.h"
#include <fstream>

void FMaterialLibrary::Initialize()
{
	
}

GLuint FMaterialLibrary::GetShader(BString ShaderPath, GLenum ShaderType)
{
	if (CachedShaders.count(ShaderPath)) {
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
			glGetShaderInfoLog(shader, infoLen, nullptr, buf);
			printf("Error compiling shader: %d \n %s \n", ShaderType, buf);
			free(buf);
		}

		glDeleteShader(shader);
		shader = 0;
	}
	else {
		printf("Shader %s compiled with id %d\n", ShaderPath.c_str(), shader);
	}
	CachedShaders[ShaderPath] = shader;
	
	return shader;
}

std::string FMaterialLibrary::LoadShaderSource(BString pathToFile)
{
	std::string content;
	std::ifstream fileStream(pathToFile, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Couldn't load shader file %s\n", pathToFile.c_str());
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

Map<BString, SharedPtr<FBaseMaterial>> FMaterialLibrary::CachedMaterials;
Map<BString, GLuint> FMaterialLibrary::CachedShaders;