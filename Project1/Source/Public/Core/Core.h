#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <list>
#include <vector>
#include <map>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "windows.h"
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <GL/glew.h>
//#include <GL/glut.h>
#include <GL/gl.h>
#include "GLFW/glfw3.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <memory>
#include <string>
template <class T>
using List = std::list<T>;
template <class Key, class T> using Map = std::map<Key, T>;

typedef	glm::vec3 Vector3F;
typedef	glm::vec2 Vector2F;
typedef glm::quat Quaternion;

typedef const char* String;

// Why B String?
// Because it's a [B]etter String :D
typedef std::string BString;

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using UniquePtr = std::unique_ptr<T>;

template <class T>
using WeakPtr = std::weak_ptr<T>;

static void to_json(json& j, const Vector3F& v) {
	j = json{ { "X", v.x }, { "Y", v.y }, { "Z", v.z } };
}
static void from_json(const json& j, Vector3F& v) {
	j.at("X").get_to(v.x);
	j.at("Y").get_to(v.y);
	j.at("Z").get_to(v.z);
}