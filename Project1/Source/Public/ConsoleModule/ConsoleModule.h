#pragma once
#include "Core.h"
#include "Editor/EditorGUIModule.h"
#include <string>
enum EConsoleLogSeverity {
	Log = 0,
	Warning = 1,
	Error = 2
};

struct FConsoleLogMessage {
public:
	EConsoleLogSeverity Severity;
	time_t Time;
	std::string Message;
};

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
	List<FConsoleLogMessage> LogMessages;
	static SharedPtr<FConsoleModule> Instance;
};