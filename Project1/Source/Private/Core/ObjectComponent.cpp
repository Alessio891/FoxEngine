#include "Core.h"
#include "ObjectComponent.h"

void FObjectComponent::DrawInspector()
{
	ImGui::SeparatorText("Component");
}

FObjectComponent* FObjectComponent::Clone()
{
	return nullptr;
}
