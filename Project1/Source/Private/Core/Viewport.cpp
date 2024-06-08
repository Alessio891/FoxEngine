#include "Core.h"
#include "Viewport.h"
#include "Application.h"
void FViewport::SetViewportLocation(int x, int y, int width, int height)
{
	Width = width;
	Height = height;
	X = x;
	Y = y;

	glfwSetWindowSize(ViewportContext, Width, Height);
	glfwSetWindowPos(ViewportContext, X, Y);
}
void FViewport::InitializeViewport(GLFWwindow* ParentWindow)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	ViewportContext = glfwCreateWindow(Width, Height, "FoxEngine Scene", NULL, NULL);

	HWND hwNative = glfwGetWin32Window(ViewportContext);
	HWND hwParent = glfwGetWin32Window(ParentWindow);
	SetParent(hwNative, hwParent);
	long style = GetWindowLong(hwNative, GWL_STYLE);
	style &= ~WS_POPUP; // remove popup style
	style |= WS_CHILDWINDOW; // add childwindow style
	SetWindowLong(hwNative, GWL_STYLE, style);

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
		vp->HandleMouseMotion(x, y);
	});
	
	glfwSetKeyCallback(ViewportContext, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		printf("Pressed");
		/*SharedPtr<FViewport> vp = FApplication::Get()->GetViewportWithContext(window);
		if (vp != nullptr)
			vp->HandleKeyboardButton(key, scancode, action, mods);*/
	});

	/*unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);*/
	ShowWindow(hwNative, SW_SHOW);
}

void FViewport::UpdateViewport()
{
	glfwMakeContextCurrent(ViewportContext);
	
	/*	int deltaw = TargetWidth - Width;
	if (abs(deltaw) > 2) {
		int step = 10 * (deltaw > 0 ? 1 : -1);
		Width += step;
	}
		int deltah = TargetHeight - Height;
	if (abs(deltah) > 2) {
		int step = 10 * (deltah > 0 ? 1 : -1);
		Height += step;
	}
	
	glfwSetWindowSize(ViewportContext, Width, Height);*/
	//
}

void FViewport::RenderViewport()
{
	glfwMakeContextCurrent(ViewportContext);


	glfwGetFramebufferSize(ViewportContext, &Width, &Height);
	glClearColor(0.01f,0.1f,0.1f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, Width, Height);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (OnViewportRenderDelegate onRender : OnRenderCallbacks) {
		onRender();
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

void FViewport::HandleMouseButton(int Button, int Action, int Mods)
{
	for (auto cb : OnMouseButtonCallbacks) {
		cb(Button, Action, Mods);
	}
}

void FViewport::HandleMouseMotion(double x, double y)
{
	for (auto cb : OnMouseMotionCallbacks) {
		cb(x,y);
	}
}

void FViewport::HandleKeyboardButton(int Key, int scanCode, int action, int mods)
{
	for (auto cb : OnKeyboardCallbacks) {
		cb(Key, scanCode, action, mods);
	}
}
