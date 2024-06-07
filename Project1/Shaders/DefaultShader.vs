#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 MVP;
uniform vec3 _Color;

out vec3 Color;

void main() {
	gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	Color = _Color;
}