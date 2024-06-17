#include "PropsDrawers/PropDrawer.h"
#include <SceneObject.h>
#include <GUI/GUI.h>

void FPropDrawer::DrawProp(BString PropKey, sol::table& PropTable)
{
	BString type = PropTable.get_or<BString, BString, BString>("Type", "none");
	std::transform(type.begin(), type.end(), type.begin(),
		[](unsigned char c) { return std::tolower(c); });
	if (type == "string") {
		DrawStringProp(PropKey, PropTable);
	}
	else if (type == "float" || type == "double") {
		DrawFloatProp(PropKey, PropTable);
	}
	else if (type == "integer" || type == "int") {
		DrawIntProp(PropKey, PropTable);
	}
	else if (type == "vector" || type == "rotation") {
		DrawVectorProp(PropKey, PropTable);
	}
	else if (type == "object" || type == "reference") {
		DrawObjectProp(PropKey, PropTable);
	}
}

void FPropDrawer::DrawStringProp(BString PropKey, sol::table& StringTable)
{
	BString value = StringTable.get_or<BString, BString, BString>("Value", "-error-");
	char* buf = new char[100];
	strcpy(buf, value.c_str());
	ImGui::InputText(PropKey.c_str(), buf, 100);
	StringTable["Value"] = std::string(buf);

}

void FPropDrawer::DrawFloatProp(BString PropKey, sol::table& FloatTable)
{
	float val = FloatTable.get_or<float, BString, float>("Value", 0.0f);

	auto speed = FloatTable.get_or<float, BString, float>("Speed", 0.5f);
	auto min = FloatTable.get_or<float, BString, float>("Min", 0.0f);
	auto max = FloatTable.get_or<float, BString, float>("Max", 0.0f);
	bool isSlider = FloatTable.get_or<bool, BString, bool>("IsSlider", false);
	if (isSlider) {
		ImGui::SliderFloat(PropKey.c_str(), &val, min, max);
	} else {
		ImGui::DragFloat(PropKey.c_str(), &val, speed, min, max);
	}
	FloatTable["Value"] = val;
}

void FPropDrawer::DrawIntProp(BString PropKey, sol::table& IntTable)
{
	int val = IntTable.get_or<int, BString, int>("Value", 0);

	auto speed = IntTable.get_or<float, BString, float>("Speed", 0.5f);
	int min = IntTable.get_or<int, BString, int>("Min", 0);
	int max = IntTable.get_or<int, BString, int>("Max", 0);
	bool isSlider = IntTable.get_or<bool, BString, bool>("IsSlider", false);
	if (isSlider) {
		ImGui::SliderInt(PropKey.c_str(), &val, min, max);
	}
	else {
		ImGui::DragInt(PropKey.c_str(), &val, speed, min, max);
	}
	IntTable["Value"] = val;
}

void FPropDrawer::DrawVectorProp(BString PropKey, sol::table& VectorTable)
{
	Vector3F val = VectorTable.get_or<Vector3F, BString, Vector3F>("Value", Vector3F(0,0,0));
	float vals[3];
	vals[0] = val.x; vals[1] = val.y; vals[2] = val.z;
	ImGui::DragFloat3(PropKey.c_str(), vals);
	val.x = vals[0];
	val.y = vals[1];
	val.z = vals[2];
	VectorTable["Value"] = val;
}

void FPropDrawer::DrawObjectProp(BString PropKey, sol::table& ObjTable)
{
	SharedPtr<FSceneObject> val = ObjTable.get_or<SharedPtr<FSceneObject>, BString, SharedPtr<FSceneObject>>("Value", SharedPtr<FSceneObject>());
	FGUI::ObjectReference(PropKey.c_str(), val);
	ObjTable["Value"] = val;
}
