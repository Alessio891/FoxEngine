#pragma once
#include "Core.h"
#include <string>
#include <functional>

enum ELogSeverity {
	Log = 0,
	Warning = 1,
	Error = 2
};


struct FLogMessage {
public:
	ELogSeverity Severity;
	time_t Time;
	std::string Message;
};

typedef std::function<void(const FLogMessage& Message)> OnMessageLoggedDelegate;

class FLogger {

public:
	static void LogInfo(std::string Message);
	static void LogWarning(std::string Message);
	static void LogError(std::string Message);
	static void LogMessage(std::string Message, ELogSeverity LogSeverity);

	static void RegisterLogCallback(OnMessageLoggedDelegate Callback);

private:
	static List<OnMessageLoggedDelegate> OnMessageLoggedCallbacks;
};
