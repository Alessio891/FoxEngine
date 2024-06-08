#pragma once
#include "Core.h"

class FTexture {

public:
	
	virtual void Load(BString Path);

	virtual bool IsLoaded() const { return TextureID > 0; }
	virtual GLuint GetTextureID() const { return TextureID; }
	virtual BString GetTexturePath() const { return TexturePath; }


	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }
protected:
	GLuint TextureID = -1;
	BString TexturePath;

	int Width;
	int Height;
};
