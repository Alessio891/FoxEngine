#include "Core.h"
#include "LuaIntegration/LuaContext.h"
#include "LuaIntegration/LuaObjects.h"
#include "SceneObject.h"
#include <InputSystem.h>
#include "MeshRendererComponent.h"
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
		"Vector3F", sol::constructors<Vector3F()>(),
		"x", &Vector3F::x,
		"y", &Vector3F::y,
		"z", &Vector3F::z
	);

	luaState.new_usertype<FTransform>(
		"Transform", sol::constructors<FTransform()>(),
		"Position", &FTransform::Position,
		"Rotation", &FTransform::Rotation,
		"Scale", &FTransform::Scale
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



	/*luaState.new_usertype<FAssetsLibrary>(
		"AssetsLibrary",
		"GetImage",
	)*/

	luaState.new_enum("ScriptType",
		"ObjectComponent", ELuaScriptType::ObjectComponent,
		"ApplicationModule", ELuaScriptType::ApplicationModule);

	luaState.set_function("IsKeyDown", &FInputSystem::IsKeyDown);
	luaState.set_function("Lerp", [](float a, float b, float f) {
		return a + f * (b-a);
	});
	
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
