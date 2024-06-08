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
	IMGUI_CHECKVERSION();
	GuiContext = SharedPtr<ImGuiContext>(ImGui::CreateContext());
	ImGui::SetCurrentContext(GuiContext.get());

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Viewport->ViewportContext, false);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags = ImGuiConfigFlags_NavNoCaptureKeyboard;
	Viewport->RegisterRenderCallback([this]() {
		BeginGUIRender();
		OnGUIRender();
		EndGUIRender();
		});
	Viewport->RegisterMouseButtonCallback([this](int btn, int action, int mods) {
		ImGui::SetCurrentContext(GuiContext.get());
		ImGui_ImplGlfw_MouseButtonCallback(Viewport->ViewportContext, btn, action, mods);
		});

	Viewport->RegisterMouseMotionCallback([this](double x, double y) {
		ImGui::SetCurrentContext(GuiContext.get());
		ImGui_ImplGlfw_CursorPosCallback(Viewport->ViewportContext, x, y);
		});
    glfwSetCursorEnterCallback(Viewport->ViewportContext, [](GLFWwindow* w, int enter) {
		if (enter) CurrentOveredGui = w;
    });
	Viewport->RegisterKeyboardButtonCallback([this](int Key, int Scancode, int Action, int Mods) {
		// FIXME: ImGui doesn't like to work with multiple windows. While working fine for mouse events,
		// keyboard events don't work as well. For now, we check the last overed window and will send the
		// event to that window. This should be fixed so that inputs are correctly dispatched
		if (Viewport->ViewportContext == CurrentOveredGui)
		{
			glfwMakeContextCurrent(Viewport->ViewportContext);
			ImGui::SetCurrentContext(GuiContext.get());
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigInputTextEnterKeepActive = true;
			io.Ctx = GuiContext.get();
			ImGui_ImplGlfw_KeyCallback(Viewport->ViewportContext, Key, Scancode, Action, Mods);
		}
	}
	);
}

void FEditorGUIModule::OnGUIRender()
{

}

void FEditorGUIModule::BeginGUIRender()
{
	ImGui::SetCurrentContext(GuiContext.get());
    
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.Ctx = GuiContext.get();
	ImGui::NewFrame();
}

void FEditorGUIModule::EndGUIRender()
{
	ImGui::Render();
	glViewport(0, 0, Viewport->GetWidth(), Viewport->GetHeight());
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLFWwindow* FEditorGUIModule::CurrentOveredGui;