#include "InputSystem.h"
#include <Application.h>
#include "Viewport.h"
#include "Logger.h"
void FInputSystem::HandleKeyDown(unsigned char Key)
{
	std::list<unsigned char>::iterator it = std::find(PressedKeys.begin(), PressedKeys.end(), Key);
	if (it == PressedKeys.end()) {
		PressedKeys.push_back(Key);
	}
}

void FInputSystem::HandleKeyUp(unsigned char Key)
{
	std::list<unsigned char>::iterator it = std::find(PressedKeys.begin(), PressedKeys.end(), Key);
	if (it != PressedKeys.end()) {
		PressedKeys.remove(Key);
	}
}

void FInputSystem::Update(float Delta)
{
	MouseDeltaX = 0;
	MouseDeltaY = 0;

	LeftButtonDown = glfwGetMouseButton(FApplication::Get()->SceneViewport->ViewportContext, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void FInputSystem::LateUpdate(float Delta)
{
	LeftButtonLastFrame = LeftButtonDown;
}

bool FInputSystem::IsKeyDown(unsigned char Key)
{
	return glfwGetKey(FApplication::Get()->MainWindow, Key) == GLFW_PRESS;
}

bool FInputSystem::IsKeyUp(unsigned char Key)
{

	return glfwGetKey(FApplication::Get()->MainWindow, Key) == GLFW_RELEASE;
}

bool FInputSystem::IsMouseButtonDown(int button)
{
	bool pressed = glfwGetMouseButton(FApplication::Get()->SceneViewport->ViewportContext, button) == GLFW_PRESS;
	
	return  pressed && !LeftButtonLastFrame;
}

void FInputSystem::OnMouseMove(int x, int y)
{
	SharedPtr<FViewport> viewport = FApplication::Get()->SceneViewport;
	if (viewport != nullptr) {
		MouseDeltaX = float(x - LastMouseX) / float(viewport->GetWidth());
		MouseDeltaY = float(y - LastMouseY) / float(viewport->GetHeight());
	
		LastMouseX = x;
		LastMouseY = y;
	}
}

void FInputSystem::SetMouseButtonState(int button, int state)
{
	/*if (button == 0)
		LeftButtonDown = state == 1;
	if (button == 1) 
		RightButtonDown = state == 1;

	if (LeftButtonDown) {
		LeftButtonReleased = false;
	}
	else {
		LeftButtonReleased = true;
	}
	RightButtonReleased = !RightButtonDown;*/
}

List<unsigned char> FInputSystem::PressedKeys;
List<unsigned char> FInputSystem::PressedThisFrame;
float FInputSystem::MouseDeltaX = 0;
float FInputSystem::MouseDeltaY = 0;
int FInputSystem::LastMouseX = 0;
int FInputSystem::LastMouseY = 0;
bool FInputSystem::LeftButtonDown = false;
bool FInputSystem::RightButtonDown = false;
bool FInputSystem::RightButtonLastFrame = false;
bool FInputSystem::LeftButtonLastFrame = false;