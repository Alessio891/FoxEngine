#pragma once

#include "Core.h"
#include "BaseMaterial.h"

class FMaterialLibrary {

public:

	static void Initialize();

	static GLuint GetShader(BString ShaderPath, GLenum ShaderType);
	
	static std::string LoadShaderSource(BString pathToFile);

	static SharedPtr<FBaseMaterial> GetMaterial(String Name) {
		if (CachedMaterials.count(Name)) 
			return CachedMaterials[Name];
		return nullptr;
	}
	static void RegisterMaterial(SharedPtr<FBaseMaterial> Material) {
		CachedMaterials[Material->GetName()] = Material;
	}
private:
	static Map<BString, SharedPtr<FBaseMaterial>> CachedMaterials;
	static Map<BString, GLuint> CachedShaders;

};