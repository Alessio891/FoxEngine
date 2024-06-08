#pragma once
#include "Core.h"
#include "Editor/EditorGUIModule.h"
#include <string>
#include "Logger.h"

class FConsoleModule : public FEditorGUIModule {
public:
	FConsoleModule(SharedPtr<FViewport> Viewport) : FEditorGUIModule(Viewport) {}
	virtual void OnStartup() override;

	virtual void OnGUIRender() override;

	void LogInfo(String Message);

	static SharedPtr<FConsoleModule> Get() {
		return Instance;
	}
protected:
	List<FLogMessage> LogMessages;
	static SharedPtr<FConsoleModule> Instance;
};