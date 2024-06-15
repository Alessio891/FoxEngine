#include "Core.h"
#include "Viewport.h"
#include "Application.h"

void FViewport::SetViewportLocation(int x, int y, int width, int height)
{
	Width = width;
	Height = height;
	X = x;
	Y = y;
	HandeViewportResize(x,y,width,height);
}
void FViewport::InitializeViewport(GLFWwindow* ParentWindow)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	ViewportContext = glfwCreateWindow(Width, Height, "FoxEngine", NULL, NULL);

	X = 200;
	Y = 200;
	glfwSetWindowPos(ViewportContext, X, Y);
	glfwSetInputMode(ViewportContext, GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwMakeContextCurrent(ViewportContext);

	glewInit();
	glfwInit();
	glfwSwapInterval(1);
	glfwSetMouseButtonCallback(ViewportContext, [](GLFWwindow* w, int b, int a, int m) {
		SharedPtr<FViewport> vp = FApplication::Get()->GetViewportWithContext(w);
		vp->HandleMouseButton(b, a, m);

		});
	glfwSetCursorPosCallback(ViewportContext, [](GLFWwindow* w, double x, double y) {
		SharedPtr<FViewport> vp = FApplication::Get()->GetViewportWithContext(w);
		if (vp != nullptr)
			vp->HandleMouseMotion(x, y);
		});
}

void FViewport::UpdateViewport()
{
}

void FViewport::RenderViewport()
{
	int consoleHeight = Height * 0.27f;
	int hierarchyWidth = Width * 0.15;
	int inspectorWidth = Width * 0.15;
	int sceneWidth = Width - inspectorWidth - hierarchyWidth;

	glfwMakeContextCurrent(ViewportContext);
	//glfwGetFramebufferSize(ViewportContext, &Width, &Height);
	glClearColor(0.01f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(inspectorWidth, consoleHeight, sceneWidth, Height-consoleHeight);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	auto ctx = GetGUIContext().get();
	ImGui::SetCurrentContext(ctx);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.Ctx = ctx;
	ImGui::NewFrame();
	

	for (OnViewportRenderDelegate onRender : OnRenderCallbacks) {
		onRender();
	}

	if (GuiContext != nullptr) {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	//glFlush();
	glfwSwapBuffers(ViewportContext);
}

void FViewport::RegisterRenderCallback(OnViewportRenderDelegate callback)
{
	OnRenderCallbacks.push_back(callback);
}

void FViewport::RegisterMouseButtonCallback(OnMouseButtonDelegate callback)
{
	OnMouseButtonCallbacks.push_back(callback);
}

void FViewport::RegisterMouseMotionCallback(OnMouseMotionDelegate callback)
{
	OnMouseMotionCallbacks.push_back(callback);
}

void FViewport::RegisterKeyboardButtonCallback(OnKeyboardButtonDelegate callback)
{
	OnKeyboardCallbacks.push_back(callback);
}

void FViewport::RegisterViewportResizeCallback(OnViewportResizeDelegate callback)
{
	OnViewportResizeCallbacks.push_back(callback);
}

void FViewport::HandleMouseButton(int Button, int Action, int Mods)
{
	for (auto cb : OnMouseButtonCallbacks) {
		cb(Button, Action, Mods);
	}
}

void FViewport::HandleMouseMotion(double x, double y)
{
	for (auto cb : OnMouseMotionCallbacks) {
		cb(x, y);
	}
}

void FViewport::HandleKeyboardButton(int Key, int scanCode, int action, int mods)
{
	// We need this or text inputs won't register backspace, return, delete and so on
	if (GuiContext) {
		auto ctx = GuiContext.get();
		ImGui::SetCurrentContext(ctx);
		ImGui_ImplGlfw_KeyCallback(ViewportContext, Key, scanCode, action, mods);
	}
	for (auto cb : OnKeyboardCallbacks) {
		cb(Key, scanCode, action, mods);
	}
}

void FViewport::HandeViewportResize(int x, int y, int w, int h)
{
	for (auto cb : OnViewportResizeCallbacks) {
		cb(x,y,w,h);
	}
}

SharedPtr<ImGuiContext> FViewport::GetGUIContext()
{
	if (GuiContext == nullptr) {
		glfwMakeContextCurrent(ViewportContext);
		GuiContext = SharedPtr<ImGuiContext>(ImGui::CreateContext());
		ImGui::SetCurrentContext(GuiContext.get());

		ImGui_ImplGlfw_InitForOpenGL(ViewportContext, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	}
	return GuiContext;
}
