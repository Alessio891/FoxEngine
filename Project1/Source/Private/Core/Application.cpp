#include "Application.h"
#include "windows.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include "GL\gl.h"
#include <stdio.h>

#include "ApplicationModule.h"
#include <InputSystem.h>
#include <functional>
#include <Editor/EditorSceneModule.h>
#include "Logger.h"
#include <format>
#include <iostream>
#include "Graphics/Primitives.h"
void FApplication::Start(int argc, char** argv, int width, int height, GLFWwindow* MainWindow)
{
	FLogger::RegisterLogCallback([this](const FLogMessage& msg) {
		char buffer[80];
		strftime(buffer, 80, "%I:%M", localtime(&msg.Time));
		printf("[%s][%d] %s\n", buffer, msg.Severity, msg.Message.c_str());
	});

	this->MainWindow = MainWindow;
	glfwSetKeyCallback(MainWindow, [](GLFWwindow* w, int btn, int scan, int act, int mods) {
		for (SharedPtr<FViewport> viewport : FApplication::Get()->Viewports) {
			viewport->HandleKeyboardButton(btn, scan, act, mods);
		}
	});


	int consoleHeight = 200;
	int hierarchyWidth = width * 0.15;
	int inspectorWidth = width * 0.15;
	int sceneWidth = width - inspectorWidth - hierarchyWidth;

	MainImGuiContext = SharedPtr<ImGuiContext>(ImGui::CreateContext());
	ImGui_ImplGlfw_InitForOpenGL(MainWindow, true);
	ImGui_ImplOpenGL3_Init();

	SceneViewport = SharedPtr<FViewport>(new FViewport(inspectorWidth, 0, sceneWidth, height - consoleHeight));
	SceneViewport->InitializeViewport(MainWindow);

	SceneViewport->RegisterMouseButtonCallback([this](int btn, int act, int mods) {
		FInputSystem::SetMouseButtonState(btn, act);
		
	});
	SceneViewport->RegisterMouseMotionCallback([this](double x, double y) {
		FInputSystem::OnMouseMove(x, y);
	});
	SceneViewport->RegisterKeyboardButtonCallback([this](int Key, int ScanCode, int action, int mods) {
		if (action == 0) FInputSystem::HandleKeyUp(Key);
		else if (action == 1) FInputSystem::HandleKeyDown(Key);
	});

	Viewports.push_back(SceneViewport);
	
	ConsoleViewport = SharedPtr<FViewport>(new FViewport(0, height - consoleHeight, width, consoleHeight));
	ConsoleViewport->InitializeViewport(MainWindow);
	Viewports.push_back(ConsoleViewport);

	InspectorViewport = SharedPtr<FViewport>(new FViewport(0,0, inspectorWidth, height- consoleHeight));
	InspectorViewport->InitializeViewport(MainWindow);
	Viewports.push_back(InspectorViewport);

	HierarchyViewport = SharedPtr<FViewport>(new FViewport(inspectorWidth + sceneWidth, 0, hierarchyWidth, height - consoleHeight));
	HierarchyViewport->InitializeViewport(MainWindow);
	Viewports.push_back(HierarchyViewport);

	ImGui::SetCurrentContext(MainImGuiContext.get());
	//glfwSetInputMode(MainWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	
}

void FApplication::MainIdleLoop(float DeltaTime)
{
	/*glfwMakeContextCurrent(MainWindow);
	ImGui::SetCurrentContext(MainImGuiContext.get());

	for (SharedPtr<FViewport> viewport : Viewports) {
		glfwMakeContextCurrent(viewport->ViewportContext);
	//	glfwPollEvents();
	}*/
	for (FApplicationModule* module : ApplicationModules) {
		module->OnTick(DeltaTime);
	}

	for (SharedPtr<FViewport> viewport : Viewports) {
		viewport->UpdateViewport();
	}

	FInputSystem::Update(DeltaTime);

}

void FApplication::MainDisplayLoop()
{
	glfwMakeContextCurrent(MainWindow);
	glfwPollEvents();

	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (SharedPtr<FViewport> viewport : Viewports) {
		viewport->RenderViewport();
	}
	glfwMakeContextCurrent(MainWindow);
}

void FApplication::OnResize(int width, int height)
{
	int consoleHeight = 200;
	int hierarchyWidth = width * 0.15;
	int inspectorWidth = width * 0.15;
	int sceneWidth = width - inspectorWidth - hierarchyWidth;

	SceneViewport->SetViewportLocation(
		inspectorWidth, 0, sceneWidth, height - consoleHeight
	);
	
	InspectorViewport->SetViewportLocation(
		0, 0, inspectorWidth, height - consoleHeight
	);
	
	ConsoleViewport->SetViewportLocation(
		0, height - consoleHeight, width, consoleHeight
	);

	HierarchyViewport->SetViewportLocation(
		inspectorWidth + sceneWidth, 0, hierarchyWidth, height - consoleHeight
	);
}

void FApplication::HandleKeyboardInput(unsigned char Key, int x, int y)
{
}

void FApplication::HandleMouseButton(GLFWwindow* Window, int Button, int Action, int Mods)
{
	if (Window == SceneViewport->ViewportContext) {
		if (Button == 0)
			FInputSystem::LeftButtonDown = Action == 1;
		else if (Button == 1)
			FInputSystem::RightButtonDown = Action == 1;
	}
	else if (Window == InspectorViewport->ViewportContext) {
		InspectorViewport->HandleMouseButton(Button, Action, Mods);
	}
	else if (Window == ConsoleViewport->ViewportContext) {
		ConsoleViewport->HandleMouseButton(Button, Action, Mods);
	}
}

void FApplication::HandleMouseMotion(GLFWwindow* Window, double x, double y)
{
	for (auto viewport : Viewports) {
		if (viewport->ViewportContext == Window) {
			viewport->HandleMouseMotion(x,y);
		}
	}
}

SharedPtr<FScene> FApplication::GetCurrentScene()
{
	for (auto module : ApplicationModules) {
		FEditorSceneModule* m = dynamic_cast<FEditorSceneModule*>(module);
		if (m != nullptr) {
			return m->GetScene();
		}
	}
	return nullptr;
}

FApplication* FApplication::Get()
{
	return Instance;
}

void FApplication::RegisterModule(FApplicationModule* Module)
{
	ApplicationModules.push_back(Module);
	Module->OnStartup();
}

void FApplication::UnregisterModule(FApplicationModule* Module)
{
	ApplicationModules.remove(Module);
	Module->OnShutdown();
	delete Module;
}

FApplication* FApplication::Instance = nullptr;