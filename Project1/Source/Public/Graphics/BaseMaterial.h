#pragma once

#include "Core/Core.h"
#include <string>
#include "Texture.h"
#include <fstream>
#include "AssetsLibrary/ISerializedAsset.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class FBaseMaterial : public ISerializedAsset, public FAssetResource {

public:

	FBaseMaterial(BString filePath, json serialized) : ISerializedAsset(), FAssetResource(EAssetResourceType::Material, filePath) {
		Deserialize(serialized);
	}

	const GLuint GetProgram() { return ProgramIndex; }

	void SetFloat(String ParamName, float Value);
	void SetMatrix(String ParamName, glm::mat4 Value);
	void SetVec3(String ParamName, Vector3F Value);

	void UploadParameters();
	BString GetName() const { return Name; }

	Map<String, float> FloatParams;
	Map<String, glm::mat4> MatParams;
	Map<String, Vector3F> VectorParams;

	virtual void Deserialize(json Json) override;
	virtual void Serialize(json& outJson) override;

	virtual void DrawInspector() override;
protected:
	GLuint ProgramIndex;
	GLuint VertexShader;
	GLuint FragmentShader;

	BString VertexShaderPath;
	BString FragmentShaderPath;

	BString Name;

	virtual ImTextureID GetThumbnailIcon() override;

};