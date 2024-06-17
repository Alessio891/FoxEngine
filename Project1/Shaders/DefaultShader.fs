#version 330 core

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

in LightData Lights;

in vec3 Color;
in vec3 Normal;
in vec3 FragPos;
in vec2 UVs;
in float Unlit;
in int UseTexture;
uniform sampler2D MainTexture;
out vec4 color;

void main() {
	if (Unlit <= 0.0) {
		vec4 textColor = texture(MainTexture, UVs);
		if (UseTexture != 1) {
			textColor = vec4(1.0,1.0,1.0,1.0);
		}
		textColor = vec4(1.0,1.0,1.0,1.0);
		vec3 norm = normalize(Normal);
		vec3 lightd = Lights.Directional.Direction;
		float diff = clamp(dot(lightd, norm), 0., 1.);
		vec3 diffuse = diff * Lights.Directional.Color * Lights.Directional.Intensity;
		vec3 result = (Lights.AmbientColor * Lights.AmbientIntensity + diffuse) * Color;
		color = vec4(result, 1.0) * textColor;
	} else {
		color = vec4(Color, 1.0);
	}
}