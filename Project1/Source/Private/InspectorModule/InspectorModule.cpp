#include "InspectorModule/InspectorModule.h"
#include <Application.h>
#include "InputSystem.h"
#include <MeshRendererComponent.h>
#include <Graphics/MaterialLibrary.h>
#include "Graphics/Primitives.h"
void FInspectorModule::OnStartup()
{
	FEditorGUIModule::OnStartup();
	
	Instance = SharedPtr<FInspectorModule>(this);

	Viewport->RegisterViewportResizeCallback([this](int x, int y, int w, int h) {
		int consoleHeight = h * FApplication::Get()->ConsoleHeight;
		int inspectorWidth = w * FApplication::Get()->InspectorWidth;

		Size.x = inspectorWidth;
		Size.y = h-consoleHeight;
	});
}

void FInspectorModule::OnTick(float Delta)
{
	FEditorGUIModule::OnTick(Delta);

}

void FInspectorModule::OnGUIRender()
{
	MAKE_WINDOW("Inspector");

	if (CurrentDisplayedObject != nullptr) {
		CurrentDisplayedObject->DrawInspector();
	}
	END_WINDOW;

}

SharedPtr<FInspectorModule> FInspectorModule::Instance;

Vector3F FImGui::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos, 0.1f);

	return Vector3F(pos[0], pos[1], pos[2]);
}
