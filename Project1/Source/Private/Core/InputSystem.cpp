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

	LeftButtonDown = glfwGetMouseButton(FApplication::Get()->EditorGUIViewport->ViewportContext, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void FInputSystem::LateUpdate(float Delta)
{
	LeftButtonLastFrame = LeftButtonDown;
}

bool FInputSystem::IsKeyDown(unsigned char Key)
{
	ImGui::SetCurrentContext(FApplication::Get()->EditorGUIViewport->GetGUIContext().get());
	if (ImGui::GetIO().WantTextInput) return false;
	return glfwGetKey(FApplication::Get()->EditorGUIViewport->ViewportContext, Key) == GLFW_PRESS;
}

bool FInputSystem::IsKeyUp(unsigned char Key)
{

	return glfwGetKey(FApplication::Get()->EditorGUIViewport->ViewportContext, Key) == GLFW_RELEASE;
}

bool FInputSystem::IsMouseButtonDown(int button)
{
	double x, y;
	glfwGetCursorPos(FApplication::Get()->EditorGUIViewport->ViewportContext, &x, &y);
	ImVec2 size = FEditorSceneModule::Get()->GetSize();
	ImVec2 pos = FEditorSceneModule::Get()->GetPosition();
	if (int(x) < pos.x || int(y) < pos.y || int(x) > pos.x + size.x || int(y) > pos.y + size.y) return false;
	bool pressed = glfwGetMouseButton(FApplication::Get()->EditorGUIViewport->ViewportContext, button) == GLFW_PRESS;

	return  pressed && !LeftButtonLastFrame;
}

bool FInputSystem::IsMouseButtonHeld(int button)
{
	double x, y;
	glfwGetCursorPos(FApplication::Get()->EditorGUIViewport->ViewportContext, &x, &y);
	ImVec2 size = FEditorSceneModule::Get()->GetSize();
	ImVec2 pos = FEditorSceneModule::Get()->GetPosition();
	if (int(x) < pos.x || int(y) < pos.y || int(x) > pos.x + size.x || int(y) > pos.y + size.y) return false;

	if (ImGui::GetDragDropPayload()) return false;

	bool pressed = glfwGetMouseButton(FApplication::Get()->EditorGUIViewport->ViewportContext, button) != GLFW_RELEASE;
	return pressed;
}


void FInputSystem::OnMouseMove(int x, int y)
{
	SharedPtr<FViewport> viewport = FApplication::Get()->EditorGUIViewport;
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