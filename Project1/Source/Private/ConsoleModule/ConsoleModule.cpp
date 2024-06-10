#pragma once
#include "Core.h"
#include "ConsoleModule/ConsoleModule.h"
#include <Application.h>
#include <chrono>
#include <format>
#include "Viewport.h"
#include <Editor/AssetsLibrary.h>
#include <string>
#include <iostream>
#include <filesystem>
#include "InspectorModule/InspectorModule.h"

namespace fs = std::filesystem;
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

	if (ImGui::BeginTabBar("#tabs")) {
		if (ImGui::BeginTabItem("Assets Library")) {
			TabIndex = 0;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Logs")) {
			TabIndex = 1;
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	if (TabIndex == 1) {
		DrawLogs();
	}
	else if (TabIndex == 0) {
		DrawAssets();
	}


	END_WINDOW

}

void FConsoleModule::DrawLogs()
{
	ImGui::BeginChild("Console");
	for (FLogMessage m : LogMessages) {
		auto color = IM_COL32(255, 255, 255, 255);
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
	ImGui::EndChild();
}

void FConsoleModule::DrawAssets()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f,0.2f,0.2f,1.0f));
	ImGui::BeginChild("Asset Folders", ImVec2(250, Size.y-40));
	ImGui::PopStyleColor();
	
	BString path = "Resources";
	BString LastNode = "Resources";
	DrawPath(path);
	ImVec2 foldersPos = ImGui::GetWindowPos();
	ImGui::EndChild();
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(260, foldersPos.y));
	ImGui::BeginChild("Current Folder", ImVec2(Size.x - 265, Size.y-40));
	ImGui::PopStyleColor();
	ImGui::Spacing();
	int file_index = 0;
	for (const auto& entry : fs::directory_iterator(CurrentSelectedPath)) {
		if (entry.is_regular_file()) {
			SharedPtr<FAssetResource> asset = FAssetsLibrary::GetResource(entry.path().string());
			asset->DrawResourceThumbnail();
			
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(0)) {
				FInspectorModule::Get()->SetDisplayedObject(asset);
			}
			ImGui::SameLine();
		}
	}
	ImGui::EndChild();
	
}

void FConsoleModule::DrawPath(BString path)
{
	List<BString> subFolders;
	List<BString> files;
	
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.64f, 0.0f, 1.0f));
	ImGuiTreeNodeFlags flags = 0;
	if (path == CurrentSelectedPath) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	bool nodeResult = ImGui::TreeNodeEx(path.substr(path.rfind("\\") + 1).c_str(), flags);

	if (ImGui::IsItemClicked()) {
		//flags |= ImGuiTreeNodeFlags_Selected;
		//printf("Clicked %s\n", path.c_str());
		CurrentSelectedPath = path;
	}
	if (nodeResult) {
	
		for (const auto& entry : fs::directory_iterator(path)) {
			if (!fs::is_regular_file(entry.path())) {
				BString filePath = entry.path().filename().string();
				std::replace(filePath.begin(), filePath.end(), '\\', '/');
				subFolders.push_back(entry.path().string());
			}
			else {
				files.push_back(entry.path().filename().string());
			}
		}

		for (BString folder : subFolders) {
			DrawPath(folder);
		}

		ImGui::TreePop();
	}
	ImGui::PopStyleColor();
}

SharedPtr<FConsoleModule> FConsoleModule::Instance;

#undef _CRT_SECURE_NO_WARNINGS