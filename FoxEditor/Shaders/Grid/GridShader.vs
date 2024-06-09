#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 MVP;

uniform mat4 _V;
uniform mat4 _P;

uniform vec3 pos;

out vec3 nearPoint;
out vec3 farPoint;
out mat4 View;
out mat4 Proj;


vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * (projInv * vec4(x, y, z, 1.0));
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = aPos;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, _V, _P).xyz;
    farPoint = UnprojectPoint(p.x, p.y, 1.0, _V, _P).xyz;

	gl_Position = vec4(p, 1.0);
    View = _V;
    Proj = _P;
}

