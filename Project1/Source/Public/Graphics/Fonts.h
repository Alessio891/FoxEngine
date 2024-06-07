#pragma once

#include "Core.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Graphics/BaseMaterial.h"

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class FFont {

public:
	void Initialize(String fontPath);
	void RenderText(SharedPtr<FBaseMaterial> Material, std::string Text, float x, float y, float scale, Vector3F Color);

	static void InitializeFreetype();

	static SharedPtr<FFont> LoadFont(String fontPath);

protected:
	Map<char, Character> Characters;

	GLuint VAO, VBO;

	static FT_Library ft;
};
