#pragma once
#include "Core.h"

class FInputSystem {

public:
	static void HandleKeyDown(unsigned char Key);
	static void HandleKeyUp(unsigned char Key);

	static void Update(float Delta);
	static void LateUpdate(float Delta);

	static bool IsKeyDown(unsigned char Key);
	static bool IsKeyUp(unsigned char Key);
	static bool IsMouseButtonDown(int button);
	static bool IsMouseButtonHeld(int button);

	static void OnMouseMove(int x, int y);
	static void SetMouseButtonState(int button, int state);

	static float MouseDeltaX;
	static float MouseDeltaY;
	static int LastMouseX;
	static int LastMouseY;


protected:
	static bool LeftButtonDown;
	static bool RightButtonDown;
	static List<unsigned char> PressedKeys;
	static List<unsigned char> PressedThisFrame;

	static bool LeftButtonLastFrame;
	static bool RightButtonLastFrame;
};
