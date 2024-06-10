#pragma once

#include "Core.h"
#include <Application.h>
#include "ApplicationModule.h"
#include "imgui_impl_glfw.h"

#define BASE_GUI_WINDOW_FLAGS ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
#define MAKE_WINDOW(WindowName) \
ImGuiWindowFlags window_flags = BASE_GUI_WINDOW_FLAGS; \
ImGui::SetNextWindowSize(ImVec2(Size.x, Size.y)); \
ImGui::SetNextWindowPos(ImVec2(Position.x, Position.y)); \
ImGui::Begin(WindowName, NULL, window_flags);

#define MAKE_WINDOW_F(WindowName, Flags) \
ImGuiWindowFlags window_flags = Flags##; \
ImGui::SetNextWindowSize(ImVec2(Size.x, Size.y)); \
ImGui::SetNextWindowPos(ImVec2(Position.x, Position.y)); \
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
	virtual void SetPositionAndSize(ImVec2 Pos, ImVec2 Size) {
		this->Position = Pos;
		this->Size = Size;
	};
	ImVec2 GetPosition() const { return Position; }
	ImVec2 GetSize() const { return Size; }
	//SharedPtr<ImGuiContext> GetGuiContext() { return GuiContext; }

	static GLFWwindow* CurrentOveredGui;
protected:
	//SharedPtr<ImGuiContext> GuiContext;
	SharedPtr<FViewport> Viewport;

	virtual void BeginGUIRender();
	virtual void EndGUIRender();
	ImGuiKey KeyToImGuiKey(int key);


	ImVec2 Position;
	ImVec2 Size;
};