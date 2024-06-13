#include "Core.h"
#include "InputSystem.h"

#include "InspectorModule\InspectorModule.h"
#include "ConsoleModule\ConsoleModule.h"
#include "Editor\EditorSceneModule.h"
#include "Editor\SceneHierarchyModule.h"

#include "GLFW/glfw3.h"
#include "windows.h"
#include "commctrl.h"

using namespace std::chrono;

const int WIDTH = 1920;
const int HEIGHT = 1080;

int64_t fLastFrameTime = 0;
FApplication* MainApplication;

int64_t getCurrentTime() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void mainIdleFunc() {

	int64_t currentTime = getCurrentTime();
	float delta = (currentTime - fLastFrameTime) / 100.0f;
	fLastFrameTime = currentTime;
	float target = 1.0f / 120.0f;
	if (delta < target) {
		float remaining = target - delta;
		delta = target;
		Sleep(remaining);
	}
	if (MainApplication != nullptr) {
		MainApplication->MainIdleLoop(delta);
	}

}

void OnGLFWError(int error, const char* description) {
	char buffer[100];
	sprintf(buffer, "Error %s\n", description);
	FLogger::LogError(buffer);
}

static LRESULT CALLBACK msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM
	lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (message) {
	case WM_DROPFILES: {
		HDROP hDrop = reinterpret_cast<HDROP>(wParam);

		// extract files here
		std::list<std::string> files;
		char filename[MAX_PATH];

		UINT count = DragQueryFileA(hDrop, -1, NULL, 0);
		for (UINT i = 0; i < count; ++i)
		{
			if (DragQueryFileA(hDrop, i, filename, MAX_PATH))
			{
				files.push_back(std::string(filename));
			}
		}

		DragFinish(hDrop);
		MainApplication->HandleFilesDropped(files);
		return 0;
	} break;
	}
	return DefSubclassProc(hWnd, message, wParam, lParam);
}

void SetupModules() {
	// Register Application Modules
	int consoleHeight = HEIGHT * MainApplication->ConsoleHeight;
	int inspectorWidth = WIDTH * MainApplication->InspectorWidth;
	int sceneWidth = WIDTH - inspectorWidth - inspectorWidth;

	// Bottom Side Widget Module
	// TODO: Rename this from FConsoleModule to a more suited name
	REGISTER_GUI_MODULE(FConsoleModule, MainApplication->EditorGUIViewport);
	FConsoleModule_Instance->SetPositionAndSize(ImVec2(0, HEIGHT - consoleHeight), ImVec2(WIDTH, consoleHeight));

	// Editor Scene Viewport Module
	REGISTER_GUI_MODULE(FEditorSceneModule, MainApplication->EditorGUIViewport);
	FEditorSceneModule_Instance->SetPositionAndSize(ImVec2(inspectorWidth, 0), ImVec2(sceneWidth, HEIGHT - consoleHeight));

	// Inspector Module
	REGISTER_GUI_MODULE(FInspectorModule, MainApplication->EditorGUIViewport);
	FInspectorModule_Instance->SetPositionAndSize(ImVec2(0, 0), ImVec2(inspectorWidth, HEIGHT - consoleHeight));

	// Scene Settings Module
	// TODO: Rename FSceneHierarchyModule to a more suited name
	REGISTER_GUI_MODULE(FSceneHierarchyModule, MainApplication->EditorGUIViewport);
	FSceneHierarchyModule_Instance->SetPositionAndSize(ImVec2(inspectorWidth + sceneWidth, 0), ImVec2(inspectorWidth, HEIGHT - consoleHeight));
}

int main(int argc, char* argv[]) {
	fLastFrameTime = getCurrentTime();

	GLFWwindow* MainWindow;

	// Setup Error Callback
	glfwSetErrorCallback(OnGLFWError);

	// Init Glew
	glewInit();

	// Init GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// Create Main Window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// Create Main Application
	MainWindow = NULL;
	MainApplication = new FApplication();
	MainApplication->Init(argc, argv, WIDTH, HEIGHT, MainWindow);
	
	// TODO: Maybe we should let the FApplication setup the core modules
	SetupModules();
	FLogger::LogInfo("Core Modules Loaded");

	MainApplication->Start(argc, argv, WIDTH, HEIGHT, MainWindow);
	
	// Setup Callback for Drag&Drop Files
	HWND hwNative = glfwGetWin32Window(MainApplication->EditorGUIViewport->ViewportContext);
	SetWindowSubclass(hwNative, &msgProc, 0, 0);

	// Main Loop
	while (!glfwWindowShouldClose(MainApplication->EditorGUIViewport->ViewportContext)) {
		// Game Update
		mainIdleFunc();

		// Render Loop
		MainApplication->MainDisplayLoop();

		// Update Input System State
		FInputSystem::LateUpdate(0.0f);
	}

	// Exit, cleanup
	glfwDestroyWindow(MainApplication->EditorGUIViewport->ViewportContext);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
