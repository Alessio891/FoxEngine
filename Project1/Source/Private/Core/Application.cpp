#include "Application.h"
#include "windows.h"
#include "GL\glew.h"
#include "GL\glut.h"
#include "GL\gl.h"
#include <stdio.h>
#include "commctrl.h"

#include "ApplicationModule.h"
#include <InputSystem.h>
#include <functional>
#include <Editor/EditorSceneModule.h>
#include "Logger.h"
#include <format>
#include <iostream>
#include "Graphics/Primitives.h"
#include "Graphics/MaterialLibrary.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <LuaIntegration/LuaContext.h>

void FApplication::Init(int argc, char** argv, int width, int height, GLFWwindow* MainWindow)
{
	FLogger::RegisterLogCallback([this](const FLogMessage& msg) {
		char buffer[80];
		strftime(buffer, 80, "%I:%M", localtime(&msg.Time));
		printf("[%s][%d] %s\n", buffer, msg.Severity, msg.Message.c_str());
		});
	SetupMainViewport(MainWindow, width, height);

	glfwMakeContextCurrent(MainWindow);
	LuaCtx = SharedPtr<FLuaContext>(new FLuaContext());

	FAssetsLibrary::Initialize();
	FMaterialLibrary::Initialize();
}

void FApplication::Start(int argc, char** argv, int width, int height, GLFWwindow* MainWindow)
{
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
	HWND hwNative = glfwGetWin32Window(EditorGUIViewport->ViewportContext);
	DragAcceptFiles(hwNative, TRUE);
	glfwPollEvents();
	for (SharedPtr<FViewport> viewport : Viewports) {
		viewport->RenderViewport();
	}

	DragAcceptFiles(hwNative, FALSE);
	//glfwMakeContextCurrent(MainWindow);
}

void FApplication::OnResize(int width, int height)
{
	int consoleHeight = height * ConsoleHeight;
	int hierarchyWidth = width * InspectorWidth;
	int inspectorWidth = width * InspectorWidth;
	int sceneWidth = width - inspectorWidth - hierarchyWidth;

	EditorGUIViewport->SetViewportLocation(0, 0, width, height);
	
	/*InspectorViewport->SetViewportLocation(
		0, 0, inspectorWidth, height - consoleHeight
	);
	
	ConsoleViewport->SetViewportLocation(
		0, height - consoleHeight, width, consoleHeight
	);

	HierarchyViewport->SetViewportLocation(
		inspectorWidth + sceneWidth, 0, hierarchyWidth, height - consoleHeight
	);*/
}

void FApplication::HandleKeyboardInput(unsigned char Key, int x, int y)
{
}

void FApplication::HandleMouseButton(GLFWwindow* Window, int Button, int Action, int Mods)
{
	/*if (Window == EditorGUIViewport->ViewportContext) {
		if (Button == 0)
			FInputSystem::LeftButtonDown = Action == 1;
		else if (Button == 1)
			FInputSystem::RightButtonDown = Action == 1;
	}
	else if (Window == EditorGUIViewport->ViewportContext) {
		EditorGUIViewport->HandleMouseButton(Button, Action, Mods);
	}*/
}

void FApplication::HandleMouseMotion(GLFWwindow* Window, double x, double y)
{
	for (auto viewport : Viewports) {
		if (viewport->ViewportContext == Window) {
			viewport->HandleMouseMotion(x,y);
		}
	}
}

void FApplication::HandleFilesDropped(List<BString> droppedFiles)
{
	for (auto droppedFilesCb : OnFilesDroppedCallbacks) {
		droppedFilesCb(droppedFiles);
	}
	//FAssetsLibrary::HandleDroppedFiles(droppedFiles);
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

WeakPtr<class FLuaContext> FApplication::GetLuaContext()
{
	return LuaCtx;
}

void FApplication::SetupMainViewport(GLFWwindow* MainWindow, int width, int height)
{
	int consoleHeight = height * 0.27f;
	int hierarchyWidth = width * 0.15;
	int inspectorWidth = width * 0.15;
	int sceneWidth = width - inspectorWidth - hierarchyWidth;

	EditorGUIViewport = SharedPtr<FViewport>(new FViewport(0, 0, width, height));
	EditorGUIViewport->InitializeViewport(MainWindow);
	Viewports.push_back(EditorGUIViewport);

	EditorGUIViewport->RegisterMouseButtonCallback([this](int btn, int act, int mods) {
		FInputSystem::SetMouseButtonState(btn, act);

		});
	EditorGUIViewport->RegisterMouseMotionCallback([this](double x, double y) {
		FInputSystem::OnMouseMove(x, y);
		});
	EditorGUIViewport->RegisterKeyboardButtonCallback([this](int Key, int ScanCode, int action, int mods) {
		//if (action == 0) FInputSystem::HandleKeyUp(Key);
		//else if (action == 1) FInputSystem::HandleKeyDown(Key);
		});

	glfwMakeContextCurrent(EditorGUIViewport->ViewportContext);
	ImGui::SetCurrentContext(EditorGUIViewport->GetGUIContext().get());
	glfwSetKeyCallback(EditorGUIViewport->ViewportContext, [](GLFWwindow* w, int btn, int scan, int act, int mods) {
		ImGui::SetCurrentContext(FApplication::Get()->EditorGUIViewport->GetGUIContext().get());
		FApplication::Get()->EditorGUIViewport->HandleKeyboardButton(btn, scan, act, mods);
		});
	glfwFocusWindow(EditorGUIViewport->ViewportContext);
	glfwSetWindowSizeCallback(EditorGUIViewport->ViewportContext, [](GLFWwindow* window, int width, int height) {
		FApplication::Get()->OnResize(width, height);
		});
}

FApplication* FApplication::Instance = nullptr;