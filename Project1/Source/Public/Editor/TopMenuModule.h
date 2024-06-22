#pragma once

#include "Core.h"
#include "EditorGUIModule.h"

class FTopMenuModule : public FEditorGUIModule {
public:
	FTopMenuModule(SharedPtr<FViewport> VP) : FEditorGUIModule(VP) {}
	virtual void OnStartup() override;
	virtual void OnTick(float Delta) override;
	virtual void OnGUIRender() override;
};