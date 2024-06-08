#pragma once
#include <list>
#include "ApplicationModule.h"
#include "Viewport.h"
#include <Scene.h>

#define REGISTER_MODULE(ModuleClass) \
ModuleClass##* ModuleClass##_Instance = new ModuleClass##(); \
FApplication::Get()->RegisterModule( ModuleClass##_Instance );

#define REGISTER_GUI_MODULE(ModuleClass, Viewport) \
ModuleClass##* ModuleClass##_Instance = new ModuleClass##(Viewport); \
FApplication::Get()->RegisterModule( ModuleClass##_Instance );

class FApplication {

public:
	FApplication() {
		Instance = this;
	};
	~FApplication() {};

	void Start(int argc, char** argv, int width, int height, GLFWwindow* MainWindow);
	virtual void MainIdleLoop(float DeltaTime);
	virtual void MainDisplayLoop();

	virtual void OnResize(int width, int height);

	virtual void HandleKeyboardInput(unsigned char Key, int x, int y);
	virtual void HandleMouseButton(GLFWwindow* Window, int Button, int Action, int Mods);
	virtual void HandleMouseMotion(GLFWwindow* Window, double x, double y);

	SharedPtr<FViewport> GetViewportWithContext(GLFWwindow* context) {
		for (auto vp : Viewports) {
			if (vp->ViewportContext == context)
				return vp;
		}
		return nullptr;
	}

	SharedPtr<FScene> GetCurrentScene();

	static FApplication* Get();

	void RegisterModule(FApplicationModule* Module);
	void UnregisterModule(FApplicationModule* Module);

	//const FViewport& GetViewport() { return *Viewport.get(); }

	SharedPtr<FViewport> SceneViewport, ConsoleViewport, InspectorViewport, HierarchyViewport;
	SharedPtr<ImGuiContext> MainImGuiContext;


	GLFWwindow* MainWindow;
protected:
	static FApplication* Instance;

	List<SharedPtr<FViewport>> Viewports;
	//SharedPtr<FViewport> Viewport;

	std::list<FApplicationModule*> ApplicationModules;
private:
};