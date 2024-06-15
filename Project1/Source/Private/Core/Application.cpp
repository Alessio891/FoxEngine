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
#include <iostream>
#include "Graphics/Primitives.h"
#include "Graphics/MaterialLibrary.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <LuaIntegration/LuaContext.h>

// Called at the start, before any module is loaded
void FApplication::Init(int argc, char** argv, int width, int height, GLFWwindow* MainWindow)
{
	// Register Log Callback for printing logs using printf
	FLogger::RegisterLogCallback([this](const FLogMessage& msg) {
		char buffer[80];
		strftime(buffer, 80, "%I:%M", localtime(&msg.Time));
		printf("[%s][%d] %s\n", buffer, msg.Severity, msg.Message.c_str());
	});

	SetupMainViewport(MainWindow, width, height);

	glfwMakeContextCurrent(MainWindow);

	// Create Lua Context
	LuaCtx = SharedPtr<FLuaContext>(new FLuaContext());

	// Initialize Libraries
	FAssetsLibrary::Initialize();
	FMaterialLibrary::Initialize();
}

// Called after core modules have been setup
void FApplication::Start(int argc, char** argv, int width, int height, GLFWwindow* MainWindow)
{
}

// Called when the application is closing
void FApplication::Shutdown()
{
	// Clean Libraries
	FAssetsLibrary::Shutdown();
}

// Main Logic Loop
void FApplication::MainIdleLoop(float DeltaTime)
{
	// Tick Modules
	for (FApplicationModule* module : ApplicationModules) {
		module->OnTick(DeltaTime);
	}

	// Tick Viewports
	for (SharedPtr<FViewport> viewport : Viewports) {
		viewport->UpdateViewport();
	}

	// Update the Input System State
	FInputSystem::Update(DeltaTime);

	// Update the Assets Library. Here assets are re-processed when set dirty
	FAssetsLibrary::Update();
}

// Main Rendering Loop
void FApplication::MainDisplayLoop()
{
	// Start checking for dragged and dropped files
	HWND hwNative = glfwGetWin32Window(GameViewport->ViewportContext);
	DragAcceptFiles(hwNative, TRUE);

	glfwPollEvents();

	// Render Viewports
	for (SharedPtr<FViewport> viewport : Viewports) {
		viewport->RenderViewport();
	}

	// Stop checking for dragged and dropped files
	DragAcceptFiles(hwNative, FALSE);
}

void FApplication::OnResize(int width, int height)
{
	int consoleHeight = height * ConsoleHeight;
	int hierarchyWidth = width * InspectorWidth;
	int inspectorWidth = width * InspectorWidth;
	int sceneWidth = width - inspectorWidth - hierarchyWidth;

	GameViewport->SetViewportLocation(0, 0, width, height);
}

void FApplication::HandleKeyboardInput(unsigned char Key, int x, int y)
{
}

void FApplication::HandleMouseButton(GLFWwindow* Window, int Button, int Action, int Mods)
{
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

	GameViewport = SharedPtr<FViewport>(new FViewport(0, 0, width, height));
	GameViewport->InitializeViewport(MainWindow);
	Viewports.push_back(GameViewport);

	GameViewport->RegisterMouseButtonCallback([this](int btn, int act, int mods) {
		FInputSystem::SetMouseButtonState(btn, act);

		});
	GameViewport->RegisterMouseMotionCallback([this](double x, double y) {
		FInputSystem::OnMouseMove(x, y);
		});
	GameViewport->RegisterKeyboardButtonCallback([this](int Key, int ScanCode, int action, int mods) {
		//if (action == 0) FInputSystem::HandleKeyUp(Key);
		//else if (action == 1) FInputSystem::HandleKeyDown(Key);
		});

	glfwMakeContextCurrent(GameViewport->ViewportContext);
	ImGui::SetCurrentContext(GameViewport->GetGUIContext().get());
	glfwSetKeyCallback(GameViewport->ViewportContext, [](GLFWwindow* w, int btn, int scan, int act, int mods) {
		ImGui::SetCurrentContext(FApplication::Get()->GameViewport->GetGUIContext().get());
		FApplication::Get()->GameViewport->HandleKeyboardButton(btn, scan, act, mods);
		});
	glfwFocusWindow(GameViewport->ViewportContext);
	glfwSetWindowSizeCallback(GameViewport->ViewportContext, [](GLFWwindow* window, int width, int height) {
		FApplication::Get()->OnResize(width, height);
		});
}

FApplication* FApplication::Instance = nullptr;