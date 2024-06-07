#pragma once

#include "Core.h"
#include <Application.h>
#include "ApplicationModule.h"

class FEditorGUIModule : public FApplicationModule {
public:
	FEditorGUIModule() = delete;
	FEditorGUIModule(SharedPtr<FViewport> viewport) : Viewport(viewport) {
	}

	virtual void OnStartup() override;
	virtual void OnGUIRender();

protected:
	SharedPtr<ImGuiContext> GuiContext;
	SharedPtr<FViewport> Viewport;

	virtual void BeginGUIRender();
	virtual void EndGUIRender();
};