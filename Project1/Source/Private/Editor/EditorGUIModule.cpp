#include "Editor/EditorGUIModule.h"

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
	Viewport->RegisterRenderCallback([this]() {
		BeginGUIRender();
		OnGUIRender();
		EndGUIRender();
		});
	Viewport->RegisterMouseButtonCallback([this](int btn, int action, int mods) {
		ImGui::SetCurrentContext(GuiContext.get());
		ImGui_ImplGlfw_MouseButtonCallback(FApplication::Get()->InspectorViewport->ViewportContext, btn, action, mods);
		});

	Viewport->RegisterMouseMotionCallback([this](double x, double y) {
		ImGui::SetCurrentContext(GuiContext.get());
		ImGui_ImplGlfw_CursorPosCallback(FApplication::Get()->InspectorViewport->ViewportContext, x, y);
		});

	Viewport->RegisterKeyboardButtonCallback([this](int Key, int Scancode, int Action, int Mods) {
			ImGui::SetCurrentContext(GuiContext.get());
			ImGui_ImplGlfw_KeyCallback(FApplication::Get()->InspectorViewport->ViewportContext, Key, Scancode, Action, Mods);
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
	glClearColor(0.4f, 0.0f, 0.6f, 1.0f);
	glViewport(0, 0, FApplication::Get()->ConsoleViewport->GetWidth(), FApplication::Get()->ConsoleViewport->GetHeight());
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
