#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUVs;

struct DirectionalLightSettings {
	vec3 Direction;
	vec3 Color;
	float Intensity;
};

struct LightData {
	vec3 AmbientColor;
	float AmbientIntensity;

	DirectionalLightSettings Directional;
};

uniform LightData _LightData;

uniform mat4 MVP;
uniform mat4 _M;
uniform vec3 _Color;
uniform float _Unlit;
uniform int _UseTexture;

out vec3 Color;
out vec3 Normal;
out vec3 FragPos;
out vec2 UVs;
out LightData Lights;
out float Unlit;
out int UseTexture;

void main() {
	gl_Position = MVP * vec4(aPos, 1.0);
	Color = _Color;
	Normal =  mat3(transpose(inverse(_M))) * aNormal;;
	FragPos = vec3(_M * vec4(aPos, 1.0));
	Lights = _LightData;
	UVs = aUVs;
	Unlit = _Unlit;
	UseTexture = _UseTexture;
}