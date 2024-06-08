#pragma once

#include "Core.h"
#include <Application.h>
#include "ApplicationModule.h"
#include "imgui_impl_glfw.h"

#define BASE_GUI_WINDOW_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
#define MAKE_WINDOW(WindowName) \
ImGuiWindowFlags window_flags = BASE_GUI_WINDOW_FLAGS; \
ImGui::SetNextWindowSize(ImVec2(Viewport->GetWidth(), Viewport->GetHeight())); \
ImGui::SetNextWindowPos(ImVec2(0, 0)); \
ImGui::Begin(WindowName, NULL, window_flags);

#define END_WINDOW ImGui::End();
class FEditorGUIModule : public FApplicationModule {
public:
	FEditorGUIModule() = delete;
	FEditorGUIModule(SharedPtr<FViewport> viewport) : Viewport(viewport) {
	}

	virtual void OnStartup() override;
	virtual void OnTick(float Delta) override { FApplicationModule::OnTick(Delta); };
	virtual void OnGUIRender();

	SharedPtr<ImGuiContext> GetGuiContext() { return GuiContext; }

protected:
	SharedPtr<ImGuiContext> GuiContext;
	SharedPtr<FViewport> Viewport;

	virtual void BeginGUIRender();
	virtual void EndGUIRender();
	ImGuiKey KeyToImGuiKey(int key);

	static GLFWwindow* CurrentOveredGui;
};