#include "InputSystem.h"
#include <Application.h>
#include "Viewport.h"
#include "Logger.h"

#include "Editor/EditorSceneModule.h"

void FInputSystem::HandleKeyDown(unsigned char Key)
{
	std::list<unsigned char>::iterator it = std::find(PressedKeys.begin(), PressedKeys.end(), Key);
	if (it == PressedKeys.end()) {
		PressedKeys.push_back(Key);
	}
	std::list<unsigned char>::iterator it2 = std::find(PressedThisFrame.begin(), PressedThisFrame.end(), Key);
	if (it2 == PressedThisFrame.end()) {
		PressedThisFrame.push_back(Key);
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
	PressedThisFrame.clear();
	LeftButtonDown = glfwGetMouseButton(FApplication::Get()->GameViewport->ViewportContext, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void FInputSystem::LateUpdate(float Delta)
{
	LeftButtonLastFrame = LeftButtonDown;
}

bool FInputSystem::IsKeyHeld(unsigned char Key)
{
	return glfwGetKey(FApplication::Get()->GameViewport->ViewportContext, Key) == GLFW_PRESS;
}

bool FInputSystem::IsKeyDown(unsigned char Key)
{
	ImGui::SetCurrentContext(FApplication::Get()->GameViewport->GetGUIContext().get());
	if (ImGui::GetIO().WantTextInput) return false;
	std::list<unsigned char>::iterator it = std::find(PressedThisFrame.begin(), PressedThisFrame.end(), Key);
	return it != PressedThisFrame.end();
}

bool FInputSystem::IsKeyUp(unsigned char Key)
{
	if (glfwGetKey(FApplication::Get()->GameViewport->ViewportContext, Key) == GLFW_PRESS) return false;

	std::list<unsigned char>::iterator it = std::find(PressedKeys.begin(), PressedKeys.end(), Key);


	return glfwGetKey(FApplication::Get()->GameViewport->ViewportContext, Key) == GLFW_RELEASE;
}

bool FInputSystem::IsMouseButtonDown(int button)
{
	double x, y;
	glfwGetCursorPos(FApplication::Get()->GameViewport->ViewportContext, &x, &y);
	ImVec2 size = FEditorSceneModule::Get()->GetSize();
	ImVec2 pos = FEditorSceneModule::Get()->GetPosition();
	if (int(x) < pos.x || int(y) < pos.y || int(x) > pos.x + size.x || int(y) > pos.y + size.y) return false;
	if (ImGui::GetIO().WantCaptureMouse) return false;
	bool pressed = glfwGetMouseButton(FApplication::Get()->GameViewport->ViewportContext, button) == GLFW_PRESS;

	return  pressed && !LeftButtonLastFrame;
}

bool FInputSystem::IsMouseButtonHeld(int button)
{
	double x, y;
	glfwGetCursorPos(FApplication::Get()->GameViewport->ViewportContext, &x, &y);
	ImVec2 size = FEditorSceneModule::Get()->GetSize();
	ImVec2 pos = FEditorSceneModule::Get()->GetPosition();
	if (int(x) < pos.x || int(y) < pos.y || int(x) > pos.x + size.x || int(y) > pos.y + size.y) return false;
	if (ImGui::GetIO().WantCaptureMouse) return false;
	if (ImGui::GetDragDropPayload()) return false;

	bool pressed = glfwGetMouseButton(FApplication::Get()->GameViewport->ViewportContext, button) != GLFW_RELEASE;
	return pressed;
}


void FInputSystem::OnMouseMove(int x, int y)
{
	SharedPtr<FViewport> viewport = FApplication::Get()->GameViewport;
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