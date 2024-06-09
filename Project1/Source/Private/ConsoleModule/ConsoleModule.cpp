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
	FLogger::RegisterLogCallback([this](const FLogMessage& msg) {
		LogMessages.push_back(msg);
	});
}

void FConsoleModule::OnGUIRender()
{
	MAKE_WINDOW_F("Console", BASE_GUI_WINDOW_FLAGS | ImGuiWindowFlags_NoTitleBar);
	
	ImGui::BeginTabBar("TestTAbBar");
	ImGui::TabItemButton("Hello");
	ImGui::TabItemButton("Hello2");
	ImGui::EndTabBar();

	for (FLogMessage m : LogMessages) {
		auto color = IM_COL32(255,255,255,255);
		if (m.Severity == ELogSeverity::Warning) color = IM_COL32(150, 150, 0, 255);
		if (m.Severity == ELogSeverity::Error) color = IM_COL32(160, 10, 10, 255);
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		char buffer[80];
		strftime(buffer, 80, "%I:%M", localtime(&m.Time));
		ImGui::Text("[ %s ] %s", buffer, m.Message.c_str());
		ImGui::PopStyleColor();
		ImGui::Separator();
		
	}
	ImGui::SetScrollHereY();
	END_WINDOW

}

SharedPtr<FConsoleModule> FConsoleModule::Instance;

#undef _CRT_SECURE_NO_WARNINGS