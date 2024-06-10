#pragma once
#include "Core.h"
#include "Editor/AssetsLibrary.h"

class FGUI {
public:
	static Vector3F Vec3(String label, Vector3F Value);
	static Vector3F Color(String label, Vector3F Value);

	static void Texture(String label, FTextureResource& Texture);

	static void EnumPopup(String label, int& value, std::vector<BString>& values);
};