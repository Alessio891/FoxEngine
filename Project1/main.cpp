#include "Core.h"

#include <stdio.h>
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include "Application.h"
#include "DebugModule.h"
#include <InputSystem.h>
#include "InspectorModule\InspectorModule.h"
#include "ConsoleModule\ConsoleModule.h"
#include "Editor/EditorSceneModule.h"
#include "Editor/SceneHierarchyModule.h"
#include "Graphics\BaseMaterial.h"

#include "GLFW/glfw3.h"
#include "windows.h"
using namespace std::chrono;
int64_t fLastFrameTime = 0;
FApplication* MainApplication;

int64_t getCurrentTime() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void mainIdleFunc() {
	
	int64_t currentTime = getCurrentTime();
	float delta = (currentTime - fLastFrameTime) / 100.0f;
	fLastFrameTime = currentTime;
	
	if (delta < 1.0f / 120.0f) {
		float remaining = (1.0f / 120.0f) - delta;
		delta = 1.0f / 120.0f;
		Sleep(remaining);
	}
	if (MainApplication != nullptr) {
		MainApplication->MainIdleLoop(delta);
	}
	
}
void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

int lastMouseX, lastMouseY;

void OnKeyboardKeyDown(unsigned char Key, int x, int y) {
	FInputSystem::HandleKeyDown(Key);
}
void OnKeyboardKeyUp(unsigned char Key, int x, int y) {
	FInputSystem::HandleKeyUp(Key);
}

void OnMouseMove(int x, int y) {
	if (x != lastMouseX || y != lastMouseY) {
		FInputSystem::OnMouseMove(x, y);
	}
	lastMouseX = x;
	lastMouseY = y;
}
void OnMouseButtonState(int button, int state, int x, int y) {
	FInputSystem::SetMouseButtonState(button, state);
	if (x != lastMouseX || y != lastMouseY) {
		FInputSystem::OnMouseMove(x, y);
	}
	lastMouseX = x;
	lastMouseY = y;
}


void OnGLFWError(int error, const char* description) {
	char buffer[100];
	sprintf(buffer, "Error %s\n", description);
	FLogger::LogError( buffer );
	//MessageBox(NULL, description, "GLFW error", MB_OK);
}
static void OnGLFWKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void OnReshape(int width, int height) {
	MainApplication->OnResize(width, height);
}

void glfwOnMouseBtn(GLFWwindow* win, int btn, int action, int mods) {
	
	//printf("Button %d on window btn %d action %d mods %d\n", &win, btn, action, mods);
	MainApplication->HandleMouseButton(win, btn, action, mods);
	
}
void glfwOnMouseMotion(GLFWwindow* win, double x, double y) {

	//printf("Button %d on window btn %d action %d mods %d\n", &win, btn, action, mods);
	MainApplication->HandleMouseMotion(win, x,y);

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
	MainWindow = glfwCreateWindow(1920, 1080, "FoxEngine Editor", NULL, NULL);
	if (!MainWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Setup Callback on Main Window
	glfwMakeContextCurrent(MainWindow);
	//glfwSetKeyCallback(MainWindow, OnGLFWKeyPress);
	glfwSetMouseButtonCallback(MainWindow, glfwOnMouseBtn);
	glfwSetCursorPosCallback(MainWindow, glfwOnMouseMotion);
	glfwSetWindowSizeCallback(MainWindow, [](GLFWwindow* window, int width, int height) {
		MainApplication->OnResize(width, height);
	});
	//

	// Create Main Application
	MainApplication = new FApplication();
	MainApplication->Start(argc, argv, 1920, 1080, MainWindow);
	// Register Application Modules
	REGISTER_GUI_MODULE(FConsoleModule, MainApplication->ConsoleViewport);
	FLogger::LogInfo("Starting Engine...");
	REGISTER_GUI_MODULE(FInspectorModule, MainApplication->InspectorViewport);
	REGISTER_GUI_MODULE(FSceneHierarchyModule, MainApplication->HierarchyViewport);
	REGISTER_GUI_MODULE(FEditorSceneModule, MainApplication->SceneViewport);
	FLogger::LogInfo("Core Modules Loaded");
	FLogger::LogWarning("This is a warning");
	FLogger::LogError("And this is an error");
	//

	// Main Loop
	while (!glfwWindowShouldClose(MainWindow)) {

		// Game Update
		mainIdleFunc();

		// Render Loop
		MainApplication->MainDisplayLoop();

		// Update Input System State
		FInputSystem::LateUpdate(0.0f);

		glfwSwapBuffers(MainWindow);

	}

	glfwDestroyWindow(MainWindow);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
