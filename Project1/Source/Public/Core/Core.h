#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <list>
#include <map>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "windows.h"
#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <memory>

template <class T>
using List = std::list<T>;
template <class Key, class T> using Map = std::map<Key, T>;

typedef	glm::vec3 Vector3F;
typedef	glm::vec2 Vector2F;

typedef const char* String;

template <class T>
using SharedPtr = std::shared_ptr<T>;