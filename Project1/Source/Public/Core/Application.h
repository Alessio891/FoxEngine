#pragma once
#include <list>
#include "ApplicationModule.h"
#include "Viewport.h"

class FScene;

#define REGISTER_MODULE(ModuleClass) \
ModuleClass##* ModuleClass##_Instance = new ModuleClass##(); \
FApplication::Get()->RegisterModule( ModuleClass##_Instance );

#define REGISTER_GUI_MODULE(ModuleClass, Viewport) \
ModuleClass##* ModuleClass##_Instance = new ModuleClass##(Viewport); \
FApplication::Get()->RegisterModule( ModuleClass##_Instance );

typedef std::function<void(List<BString>)> OnFilesDroppedDelegate;

class FApplication {

public:
	FApplication() {
		Instance = this;
	};
	~FApplication() {};

	void Init(int argc, char** argv, int width, int height, GLFWwindow* MainWindow);
	void Start(int argc, char** argv, int width, int height, GLFWwindow* MainWindow);
	void Shutdown();
	virtual void MainIdleLoop(float DeltaTime);
	virtual void MainDisplayLoop();

	virtual void OnResize(int width, int height);

	virtual void HandleKeyboardInput(unsigned char Key, int x, int y);
	virtual void HandleMouseButton(GLFWwindow* Window, int Button, int Action, int Mods);
	virtual void HandleMouseMotion(GLFWwindow* Window, double x, double y);

	virtual void HandleFilesDropped(List<BString> droppedFiles);
	void RegisterFilesDroppedCallback(OnFilesDroppedDelegate callback) {
		OnFilesDroppedCallbacks.push_back(callback);
	}

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

	SharedPtr<FViewport> GameViewport;

	float InspectorWidth = 0.15f;
	float ConsoleHeight = 0.27f;

	bool ShouldProcessSceneInputs() const {
		
		return true;
	}
	WeakPtr<class FLuaContext> GetLuaContext();
protected:
	
	virtual void SetupMainViewport(GLFWwindow* MainWindow, int width, int height);

	static FApplication* Instance;

	List<SharedPtr<FViewport>> Viewports;
	
	List<FApplicationModule*> ApplicationModules;

	List<OnFilesDroppedDelegate> OnFilesDroppedCallbacks;

	SharedPtr<class FLuaContext> LuaCtx;
private:
};