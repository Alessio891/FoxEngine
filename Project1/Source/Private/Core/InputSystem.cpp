#include "InputSystem.h"
#include <Application.h>
#include "Viewport.h"
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
}

bool FInputSystem::IsKeyDown(unsigned char Key)
{
	return glfwGetKey(FApplication::Get()->MainWindow, Key) == GLFW_PRESS;
	/*std::list<unsigned char>::iterator it = std::find(PressedKeys.begin(), PressedKeys.end(), Key);
	return it != PressedKeys.end();*/
}

bool FInputSystem::IsKeyUp(unsigned char Key)
{
	return glfwGetKey(FApplication::Get()->MainWindow, Key) == GLFW_RELEASE;
	/*std::list<unsigned char>::iterator it = std::find(PressedKeys.begin(), PressedKeys.end(), Key);
	return it == PressedKeys.end();*/
}

void FInputSystem::OnMouseMove(int x, int y)
{
	FViewport viewport = *FApplication::Get()->SceneViewport;
	MouseDeltaX = float(x - LastMouseX) / float(viewport.GetWidth());
	MouseDeltaY = float(y - LastMouseY) / float(viewport.GetHeight());
	
	LastMouseX = x;
	LastMouseY = y;
}

void FInputSystem::SetMouseButtonState(int button, int state)
{
	if (button == 0) 
		LeftButtonDown = state == 0;
	if (button == 1) 
		RightButtonDown = state == 0;
}

List<unsigned char> FInputSystem::PressedKeys;
List<unsigned char> FInputSystem::PressedThisFrame;
float FInputSystem::MouseDeltaX = 0;
float FInputSystem::MouseDeltaY = 0;
int FInputSystem::LastMouseX = 0;
int FInputSystem::LastMouseY = 0;
bool FInputSystem::LeftButtonDown = false;
bool FInputSystem::RightButtonDown = false;