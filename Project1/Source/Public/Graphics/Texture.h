#pragma once
#include "Core.h"
#include "AssetsLibrary/ISerializedAsset.h"
#include "AssetsLibrary/AssetResource.h"
class FTexture : public ISerializedAsset, public FAssetResource {

public:
	FTexture(BString path) : TexturePath(path), ISerializedAsset(), FAssetResource(EAssetResourceType::Image, path) {
	}
	virtual void Load(GLFWwindow* ctx, BString Path);

	virtual bool IsLoaded() const { return TextureID > 0; }
	virtual GLuint GetTextureID(GLFWwindow* ctx) { 
		if (TextureByContext.count(ctx) <= 0)
		{
			Load(ctx, TexturePath);
		}
		return TextureByContext[ctx]; 
	}
	virtual BString GetTexturePath() const { return TexturePath; }

	int GetWidth() const { return Width; }
	int GetHeight() const { return Height; }

	virtual void OnDeleted() override;

protected:
	Map<GLFWwindow*, GLuint> TextureByContext;
	GLuint TextureID = -1;
	BString TexturePath;

	int Width;
	int Height;

	virtual ImTextureID GetThumbnailIcon() override;
};
