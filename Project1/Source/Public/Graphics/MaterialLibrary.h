#pragma once

#include "Core.h"
#include "BaseMaterial.h"

class FMaterialLibrary {

public:

	static void Initialize();

	static GLuint GetShader(String ShaderPath, GLenum ShaderType);
	
	static std::string LoadShaderSource(String pathToFile);

	static SharedPtr<FBaseMaterial> GetMaterial(String Name) {
		if (CachedMaterials.count(Name)) 
			return CachedMaterials[Name];
		return nullptr;
	}
	static void RegisterMaterial(SharedPtr<FBaseMaterial> Material) {
		CachedMaterials[Material->GetName()] = Material;
	}
private:
	static Map<String, SharedPtr<FBaseMaterial>> CachedMaterials;
	static Map<String, GLuint> CachedShaders;

};