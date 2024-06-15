#pragma once
#include "Core.h"
#include "SceneObject.h"

class FBoundingBox {
public:
	Vector3F Min, Max;
};

class FGraphics {
	
public:
	static bool Raycast(
		Vector3F Origin,
		Vector3F Direction,
		FSceneObject* Target,
		float& Distance
	);
};