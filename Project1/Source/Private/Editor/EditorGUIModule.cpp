#include "Editor/EditorGUIModule.h"
#include "Logger.h"
#include "Core.h"
#include "imgui.h"
//#include <imgui_impl_glfw.cpp>

void FEditorGUIModule::OnStartup()
{
	FApplicationModule::OnStartup();
	if (Viewport == nullptr) return;

	glfwMakeContextCurrent(Viewport->ViewportContext);
	ImGui::SetCurrentContext(Viewport->GetGUIContext().get());
	IMGUI_CHECKVERSION();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags = ImGuiConfigFlags_NavNoCaptureKeyboard;
	Viewport->RegisterRenderCallback([this]() {
		//BeginGUIRender();
		OnGUIRender();
		//EndGUIRender();
		});
		
	Viewport->RegisterMouseButtonCallback([this](int btn, int action, int mods) {
		
	});

	Viewport->RegisterMouseMotionCallback([this](double x, double y) {
		
		});
    glfwSetCursorEnterCallback(Viewport->ViewportContext, [](GLFWwindow* w, int enter) {
		if (enter) {
			CurrentOveredGui = w;
		}
    });
	Viewport->RegisterKeyboardButtonCallback([this](int Key, int Scancode, int Action, int Mods) {
		// FIXME: ImGui doesn't like to work with multiple windows. While working fine for mouse events,
		// keyboard events don't work as well. For now, we check the last overed window and will send the
		// event to that window. This should be fixed so that inputs are correctly dispatched
		if (Viewport->ViewportContext == CurrentOveredGui)
		{
		}
		//glfwMakeContextCurrent(Viewport->ViewportContext);
		
	}
	);
}

void FEditorGUIModule::OnGUIRender()
{

}

void FEditorGUIModule::BeginGUIRender()
{
	auto ctx = Viewport->GetGUIContext().get();

	ImGui::SetCurrentContext(ctx);
    
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.Ctx = Viewport->GetGUIContext().get();
	ImGui::NewFrame();
}

void FEditorGUIModule::EndGUIRender()
{
	ImGui::Render();
	glViewport(Position.x, Position.y, Size.x, Size.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* FEditorGUIModule::CurrentOveredGui;