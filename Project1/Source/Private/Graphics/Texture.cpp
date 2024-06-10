#include "Graphics\Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GLFW/glfw3.h"
#include <Editor/AssetsLibrary.h>

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

ImTextureID FTexture::GetThumbnailIcon()
{
	return (void*)(intptr_t)FAssetsLibrary::GetImage(TexturePath)->GetTextureID(FApplication::Get()->EditorGUIViewport->ViewportContext);
}
