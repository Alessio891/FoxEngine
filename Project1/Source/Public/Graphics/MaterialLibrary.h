#pragma once

#include "Core.h"
#include "BaseMaterial.h"

class FMaterialLibrary {

public:

	static GLuint GetShader(String ShaderPath, GLenum ShaderType);
	
	static std::string LoadShaderSource(String pathToFile);
private:
	static Map<String, SharedPtr<FBaseMaterial>> CachedMaterials;
	static Map<String, GLuint> CachedShaders;

};