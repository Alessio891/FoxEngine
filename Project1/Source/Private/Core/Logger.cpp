#include "Logger.h"
#include <chrono>

void FLogger::LogInfo(std::string Message)
{
	LogMessage(Message, ELogSeverity::Log);
}

void FLogger::LogWarning(std::string Message)
{
	LogMessage(Message, ELogSeverity::Warning);
}

void FLogger::LogError(std::string Message)
{
	LogMessage(Message, ELogSeverity::Error);
}

void FLogger::LogMessage(std::string Message, ELogSeverity LogSeverity)
{
	FLogMessage log;
	log.Message = Message;
	log.Time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	log.Severity = LogSeverity;

	for (auto cb : OnMessageLoggedCallbacks) {
		cb(log);
	}
}

void FLogger::RegisterLogCallback(OnMessageLoggedDelegate Callback)
{
	OnMessageLoggedCallbacks.push_back(Callback);
}

List<OnMessageLoggedDelegate> FLogger::OnMessageLoggedCallbacks;