#include "GUI/GUI.h"
#include "Graphics/Texture.h"
#include "Application.h"

Vector3F FGUI::Vec3(String label, Vector3F Value)
{
	float pos[3]{ Value.x, Value.y, Value.z };
	ImGui::DragFloat3(label, pos, 0.1f);
	ImGui::Separator();
	return Vector3F(pos[0], pos[1], pos[2]);
}

void FGUI::Color(BString label, Vector3F& Color)
{
	float c[3] = { Color.x, Color.y, Color.z };
	
	if (ImGui::ColorEdit3(label.c_str(), c, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueWheel)) {
		Color.x = c[0];
		Color.y = c[1];
		Color.z = c[2];
	}
	ImGui::Separator();
}

void FGUI::Material(String label, FAssetReference<FBaseMaterial>& outMat)
{
	AssetReference<FBaseMaterial>(label, outMat);

}

void FGUI::Texture(String label, FAssetReference<FTexture>& Texture)
{
	AssetReference<FTexture>(label, Texture);
}

void FGUI::EnumPopup(String label, int& value, std::vector<BString>& values)
{
	if (ImGui::BeginCombo(label, values[value].c_str())) {
		for(unsigned int i = 0; i < values.size(); i++)
		{
			if (ImGui::Selectable(values[i].c_str())) {
				value = i;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Separator();
}
