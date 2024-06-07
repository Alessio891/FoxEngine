#pragma once
#include "ConsoleModule/ConsoleModule.h"
#include <Application.h>
#include <chrono>
#include <format>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
void FConsoleModule::OnStartup()
{
	FEditorGUIModule::OnStartup();
	Instance = SharedPtr<FConsoleModule>(this);
}

void FConsoleModule::OnGUIRender()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	static bool open = true;
	ImGui::SetNextWindowSize(ImVec2(FApplication::Get()->ConsoleViewport->GetWidth(), FApplication::Get()->ConsoleViewport->GetHeight()));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Console", &open, window_flags);
	
	for (FConsoleLogMessage m : LogMessages) {
		auto color = IM_COL32(255,255,255,255);
		if (m.Severity == EConsoleLogSeverity::Warning) color = IM_COL32(150, 150, 0, 255);
		if (m.Severity == EConsoleLogSeverity::Error) color = IM_COL32(160, 10, 10, 255);
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		char buffer[80];
		strftime(buffer, 80, "%I:%M", localtime(&m.Time));
		ImGui::Text("[ %s ] %s", buffer, m.Message.c_str());
		ImGui::PopStyleColor();
		ImGui::Separator();
		
	}
	ImGui::SetScrollHereY();
	ImGui::End();

}

void FConsoleModule::LogInfo(String Message)
{
	auto start = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	FConsoleLogMessage log;
	log.Message = std::string(Message);
	log.Time = start;
	log.Severity = EConsoleLogSeverity::Warning;
	LogMessages.push_back(log);
}

SharedPtr<FConsoleModule> FConsoleModule::Instance;

#undef _CRT_SECURE_NO_WARNINGS