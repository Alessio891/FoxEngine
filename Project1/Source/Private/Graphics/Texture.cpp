#include "Graphics\Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GLFW/glfw3.h"
#include <AssetsLibrary/AssetsLibrary.h>

void FTexture::Load(GLFWwindow* ctx, BString Path)
{
	GLFWwindow* oldW = glfwGetCurrentContext();
	glfwMakeContextCurrent(ctx);
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nrChannels;
	unsigned char* data = stbi_load(Path.c_str(), &Width, &Height, NULL, 4);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
	TexturePath = Path;
	TextureByContext[ctx] = textureId;

	glfwMakeContextCurrent(oldW);

}

void FTexture::OnDeleted()
{
	FAssetResource::OnDeleted();
	GLFWwindow* oldW = glfwGetCurrentContext();
	for (auto it : TextureByContext) {
		glfwMakeContextCurrent(it.first);
		glDeleteBuffers(1, &it.second);

	}
	glfwMakeContextCurrent(oldW);
}

ImTextureID FTexture::GetThumbnailIcon()
{
	auto texture = FAssetsLibrary::GetImage(TexturePath);
	ImTextureID id = (void*)(intptr_t)texture->GetTextureID(FApplication::Get()->GameViewport->ViewportContext);
	texture.reset();
	return id;
}
