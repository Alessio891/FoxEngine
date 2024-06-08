#pragma once

#include "Core/Core.h"
#include <string>
#include "Texture.h"
class FBaseMaterial {

public:

	FBaseMaterial(
		GLuint VertexShader,
		GLuint FragmentShader,
		const String MaterialName
	);

	const GLuint GetProgram() { return ProgramIndex; }

	void SetFloat(String ParamName, float Value);
	void SetMatrix(String ParamName, glm::mat4 Value);
	void SetVec3(String ParamName, Vector3F Value);

	void UploadParameters();
	
	Map<String, float> FloatParams;
	Map<String, glm::mat4> MatParams;
	Map<String, Vector3F> VectorParams;

	SharedPtr<FTexture> Texture;
protected:
	GLuint ProgramIndex;
	GLuint VertexShader;
	GLuint FragmentShader;


};