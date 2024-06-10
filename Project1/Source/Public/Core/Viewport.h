#pragma once

#include "Core.h"
#include <functional>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

typedef std::function<void()> OnViewportRenderDelegate;
typedef std::function<void(int, int, int)> OnMouseButtonDelegate;
typedef std::function<void(double, double)> OnMouseMotionDelegate;
typedef std::function<void(int, int, int, int)> OnKeyboardButtonDelegate;

class FViewport {
public:
	FViewport(int x, int y, int w, int h) : X(x), Y(y), TargetWidth(w), TargetHeight(h), Width(w), Height(h) {}
	
	const int GetWidth() { return Width; }
	const int GetHeight() { return Height; }

	void SetWidth(int w) { Width = w; }
	void SetHeight(int h) { Height = h; }

	void SetViewportLocation(int x, int y, int width, int height);

	const int GetX() { return X; }
	const int GetY() { return Y; }

	void SetX(int x) { X = x; }
	void SetY(int y) { Y = y; }

	virtual void InitializeViewport(GLFWwindow* ParentWindow);
	virtual void UpdateViewport();
	virtual void RenderViewport();

	void RegisterRenderCallback(OnViewportRenderDelegate callback);
	void RegisterMouseButtonCallback(OnMouseButtonDelegate callback);
	void RegisterMouseMotionCallback(OnMouseMotionDelegate callback);
	void RegisterKeyboardButtonCallback(OnKeyboardButtonDelegate callback);

	virtual void HandleMouseButton(int Button, int Action, int Mods);
	virtual void HandleMouseMotion(double x, double y);
	virtual void HandleKeyboardButton(int Key, int scanCode, int action, int mods);

	SharedPtr<ImGuiContext> GetGUIContext();
	bool ValidGUIContext() { return GuiContext != nullptr; }

	glm::mat4 GetProjectionMatrix() {
		float aspect = float(Width) / float(Height);

		glm::mat4 ProjectionMatrix = glm::perspective<float>(
			glm::radians(45.0f),
			aspect,
			0.1f,
			100.0f
		);
		return ProjectionMatrix;
	}

	GLFWwindow* ViewportContext;
protected:
	List<OnViewportRenderDelegate> OnRenderCallbacks;
	List<OnMouseButtonDelegate> OnMouseButtonCallbacks;
	List<OnMouseMotionDelegate> OnMouseMotionCallbacks;
	List<OnKeyboardButtonDelegate> OnKeyboardCallbacks;

	int X;
	int Y;
	int Width;
	int Height;

	int TargetWidth;
	int TargetHeight;
	SharedPtr<ImGuiContext> GuiContext;
};