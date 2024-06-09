#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUVs;

uniform mat4 MVP;
uniform mat4 _M;
uniform vec3 _Color;

out vec3 Color;
out vec3 Normal;
out vec3 FragPos;
out vec2 UVs;

void main() {
	gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	Color = _Color;
	Normal =  mat3(transpose(inverse(_M))) * aNormal;;
	FragPos = vec3(_M * vec4(aPos, 1.0));
	UVs = aUVs;
}