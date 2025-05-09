
#include "Core.h"
#include "Application.h"
#include "LuaIntegration/LuaContext.h"
#include "LuaIntegration/LuaObjects.h"
#include "SceneObject.h"
#include <InputSystem.h>
#include "MeshRendererComponent.h"
#include "GUI/GUI.h"
#include "Scene.h"

FLuaContext::FLuaContext()
{
	luaState.open_libraries(sol::lib::base, sol::lib::table);

	luaState.new_usertype<LLuaSceneObjectComponent>(
		"SceneObjectComponent", sol::meta_function::construct,
		sol::factories( []() { return SharedPtr<LLuaSceneObjectComponent>(new LLuaSceneObjectComponent()); } ),
		sol::call_constructor,
		sol::factories([]() { return std::make_shared<LLuaSceneObjectComponent>(); }),
		"new", sol::factories([]() { return LLuaSceneObjectComponent(); }),
		"Type", &LLuaSceneObjectComponent::Type,
		"Tick", &LLuaSceneObjectComponent::Tick,
		"Begin", &LLuaSceneObjectComponent::Begin,
		"End", &LLuaSceneObjectComponent::End,
		"Object", &LLuaSceneObjectComponent::Object,
		"Data", &LLuaSceneObjectComponent::Data,
		"ScriptType", &LLuaSceneObjectComponent::ScriptType
	);
	luaState.new_usertype<Vector3F>(
		"Vector3F", sol::constructors<Vector3F(), Vector3F(float, float, float)>(),
		"x", &Vector3F::x,
		"y", &Vector3F::y,
		"z", &Vector3F::z,
		sol::meta_function::subtraction, [] (const Vector3F& v1, const Vector3F& v2) {
			return v1 - v2;
		},
		sol::meta_function::addition, [](const Vector3F& v1, const Vector3F& v2) { 
			return v1 + v2;
		},
		sol::meta_function::multiplication, [](const Vector3F& v1, const float& scalar) { 
			return v1 * scalar;
		}
	);
	

	luaState.new_usertype<FTransform>(
		"Transform", sol::constructors<FTransform()>(),
		"Position", &FTransform::Position,
		"Rotation", &FTransform::Rotation,
		"Scale", &FTransform::Scale,
		"GetRightVector", &FTransform::GetRightVector,
		"GetUpVector", &FTransform::GetUpVector,
		"GetForwardVector", &FTransform::GetForwardVector,
		"LookAt", &FTransform::LookAt
	);

	luaState.new_usertype<FMeshRendererComponent>(
		"MeshRendererComponent",
		"Color", &FMeshRendererComponent::Color,
		"SetColor", &FMeshRendererComponent::SetColor,
		"SetTexture", &FMeshRendererComponent::SetTexture
	);

	luaState.new_usertype<FSceneObject>(
		"SceneObject",
		"Transform", &FSceneObject::Transform,
		"Name", &FSceneObject::Name,
		"Renderer", &FSceneObject::Renderer
	);
	luaState.set_function("GetRotationFromDirection", [](Vector3F direction, Vector3F up) { 
		float      directionLength = glm::length(direction);

		// Check if the direction is valid; Also deals with NaN
		if (!(directionLength > 0.0001))
			return glm::eulerAngles(glm::quat(1, 0, 0, 0)); // Just return identity

		// Normalize direction
		direction /= directionLength;

		// Is the normal up (nearly) parallel to direction?
		if (glm::abs(glm::dot(direction, up)) > .9999f) {
			// Use alternative up
			Vector3F alt(0.0f, 1.0f, 0.0f);
			return glm::eulerAngles(glm::quatLookAt(direction, alt));
		}
		else {
			return glm::eulerAngles(glm::quatLookAt(direction, up));
		}
		//return glm::eulerAngles(glm::quatLookAt(dir, up));
	});
	luaState.set_function("Normalize", [](Vector3F& v) {
		return glm::normalize(v);
	});
	luaState.set_function("GetCameraTransform", []() {
		if (FApplication::Get()->GetCurrentScene() != nullptr) {
			return FApplication::Get()->GetCurrentScene()->CameraTransform;
		}
	});
	luaState.set_function("SetCameraTransform", [](FTransform& t) {
		if (FApplication::Get()->GetCurrentScene() != nullptr) {
			FApplication::Get()->GetCurrentScene()->CameraTransform = t;
		}
		});
	luaState.set_function("SpawnObject", [](BString name, BString templatePath) {
		return FApplication::Get()->GetCurrentScene()->SpawnObject(name, templatePath);
	});

	
	luaState.set_function("GuiLabel", &ImGui::Text);
	luaState.set_function("GuiSetWindowPosition", [](double x, double y) {
		ImGui::SetNextWindowPos(ImVec2(x, y));
	});

	luaState.set_function("GuiSetWindowSize", [](double x, double y) {
		ImGui::SetNextWindowSize(ImVec2(x, y));
		});
	luaState.set_function("GuiBeginWindow", [](BString title) {
		return ImGui::Begin(title.c_str());
	});
	luaState.set_function("GuiEndWindow", &ImGui::End);
	luaState.set_function("GuiColor", [](BString label, Vector3F color) {
		Vector3F c = color;
		FGUI::Color(label.c_str(), c);
		return c;
	});

	luaState.set_function("Raycast", [](SharedPtr<FSceneObject> obj) {
		auto scene = FApplication::Get()->GetCurrentScene();
		float dist = 0.0f;
		FSceneObject* ref = obj.get();
		return FGraphics::Raycast( scene->CameraTransform.Position, scene->CameraTransform.GetForwardVector(), ref, dist);
	});

	/*luaState.new_usertype<FAssetsLibrary>(
		"AssetsLibrary",
		"GetImage",
	)*/

	luaState.new_enum("ScriptType",
		"ObjectComponent", ELuaScriptType::ObjectComponent,
		"ApplicationModule", ELuaScriptType::ApplicationModule);
	luaState.set_function("IsKeyDown", &FInputSystem::IsKeyDown);
	luaState.set_function("IsKeyHeld", &FInputSystem::IsKeyHeld);
	luaState.set_function("Lerp", [](float a, float b, float f) {
		return a + f * (b-a);
	});
	
	luaState.script(
	"function Set(variable, value)\n"
	 "  Data[variable].Value = value\n"
	 "end\n"
	 "function Get(variable)\n"
	 "  return Data[variable].Value\n"
	 "end\n"
	);

	SetupKeysEnum();
	

	BeginLuaContext();
}

void FLuaContext::BeginLuaContext()
{
	luaState.set_function("FoxLog", [](BString msg) {
		FLogger::LogInfo("[LuaLog] " + msg);
	});
	luaState.script("FoxLog('Lua Context Started')");
}

void FLuaContext::SetupKeysEnum()
{
	luaState.new_enum("Key",
		"Space", GLFW_KEY_SPACE,
		"Apostrophe", GLFW_KEY_APOSTROPHE,
		"Comma", GLFW_KEY_COMMA,
		"Minus", GLFW_KEY_MINUS,
		"Period", GLFW_KEY_PERIOD,
		"Slash", GLFW_KEY_SLASH,
		"Num0", GLFW_KEY_0,
		"Num1", GLFW_KEY_1,
		"Num2", GLFW_KEY_2,
		"Num3", GLFW_KEY_3,
		"Num4", GLFW_KEY_4,
		"Num5", GLFW_KEY_5,
		"Num6", GLFW_KEY_6,
		"Num7", GLFW_KEY_7,
		"Num8", GLFW_KEY_8,
		"Num9", GLFW_KEY_9,
		"A", GLFW_KEY_A,
		"B", GLFW_KEY_B,
		"C", GLFW_KEY_C,
		"D", GLFW_KEY_D,
		"E", GLFW_KEY_E,
		"F", GLFW_KEY_F,
		"G", GLFW_KEY_G,
		"H", GLFW_KEY_H,
		"I", GLFW_KEY_I,
		"J", GLFW_KEY_J,
		"K", GLFW_KEY_K,
		"L", GLFW_KEY_L,
		"M", GLFW_KEY_M,
		"N", GLFW_KEY_N,
		"O", GLFW_KEY_O,
		"P", GLFW_KEY_P,
		"Q", GLFW_KEY_Q,
		"R", GLFW_KEY_R,
		"S", GLFW_KEY_S,
		"T", GLFW_KEY_T,
		"U", GLFW_KEY_U,
		"V", GLFW_KEY_V,
		"W", GLFW_KEY_W,
		"X", GLFW_KEY_X,
		"Y", GLFW_KEY_Y,
		"Z", GLFW_KEY_Z,
		"Backslash", GLFW_KEY_BACKSLASH,
		"Escape", GLFW_KEY_ESCAPE,
		"Enter", GLFW_KEY_ENTER,
		"Tab", GLFW_KEY_TAB,
		"Backspace", GLFW_KEY_BACKSPACE,
		"Insert", GLFW_KEY_INSERT,
		"Delete", GLFW_KEY_DELETE,
		"Right", GLFW_KEY_RIGHT,
		"Left", GLFW_KEY_LEFT,
		"Down", GLFW_KEY_DOWN,
		"Up", GLFW_KEY_UP,
		"PageUp", GLFW_KEY_PAGE_UP,
		"PageDown", GLFW_KEY_PAGE_DOWN,
		"Home", GLFW_KEY_HOME,
		"End", GLFW_KEY_END,
		"CapsLock", GLFW_KEY_CAPS_LOCK,
		"ScrollLock", GLFW_KEY_SCROLL_LOCK,
		"NumLock", GLFW_KEY_NUM_LOCK,
		"PrintScreen", GLFW_KEY_PRINT_SCREEN,
		"Pause", GLFW_KEY_PAUSE,
		"F1", GLFW_KEY_F1,
		"F2", GLFW_KEY_F2,
		"F3", GLFW_KEY_F3,
		"F4", GLFW_KEY_F4,
		"F5", GLFW_KEY_F5,
		"F6", GLFW_KEY_F6,
		"F7", GLFW_KEY_F7,
		"F8", GLFW_KEY_F8,
		"F9", GLFW_KEY_F9,
		"F10", GLFW_KEY_F10,
		"F11", GLFW_KEY_F11,
		"F12", GLFW_KEY_F12,
		"Decimal", GLFW_KEY_KP_DECIMAL,
		"Divide", GLFW_KEY_KP_DIVIDE,
		"Multiply", GLFW_KEY_KP_MULTIPLY,
		"Subtract", GLFW_KEY_KP_SUBTRACT,
		"Add", GLFW_KEY_KP_ADD,
		"Return", GLFW_KEY_KP_ENTER,
		"KpEqual", GLFW_KEY_KP_EQUAL,
		"LeftShift", GLFW_KEY_LEFT_SHIFT,
		"LeftCtrl", GLFW_KEY_LEFT_CONTROL,
		"LeftAlt", GLFW_KEY_LEFT_ALT,
		"RightShift", GLFW_KEY_RIGHT_SHIFT,
		"RightCtrl", GLFW_KEY_RIGHT_CONTROL,
		"RightAlt", GLFW_KEY_RIGHT_ALT,
		"Menu", GLFW_KEY_MENU
	);
}
