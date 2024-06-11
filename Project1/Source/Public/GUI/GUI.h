#pragma once
#include "Core.h"
#include "Editor/AssetsLibrary.h"

class FGUI {
public:
	static Vector3F Vec3(String label, Vector3F Value);
	static void Color(BString label, Vector3F& Value);

	static void Material(String label, SharedPtr<FBaseMaterial>& outMat );
	static void Texture(String label, FTextureResource& Texture);

	static void EnumPopup(String label, int& value, std::vector<BString>& values);
};