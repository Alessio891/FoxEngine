#pragma once
#include "Core.h"

class FInputSystem {

public:
	static void HandleKeyDown(unsigned char Key);
	static void HandleKeyUp(unsigned char Key);

	static void Update(float Delta);

	static bool IsKeyDown(unsigned char Key);
	static bool IsKeyUp(unsigned char Key);

	static void OnMouseMove(int x, int y);
	static void SetMouseButtonState(int button, int state);

	static float MouseDeltaX;
	static float MouseDeltaY;

	static bool LeftButtonDown;
	static bool RightButtonDown;

protected:
	static List<unsigned char> PressedKeys;
	static List<unsigned char> PressedThisFrame;

	static int LastMouseX;
	static int LastMouseY;
};
