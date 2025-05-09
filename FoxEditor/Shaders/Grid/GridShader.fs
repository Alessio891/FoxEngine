#version 330 core
out vec4 color;
in vec3 nearPoint;
in vec3 farPoint;
in mat4 View;
in mat4 Proj;

vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 gridColor = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    if (fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        gridColor.z = 1.0;
    if (fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        gridColor.x = 1.0;
    return gridColor;
}

float computeLinearDepth(vec3 pos) {
    float near = 0.1;
    float far = 100.0;
    vec4 clip_space_pos = Proj * View * vec4(pos.xyz, 1.0);
    if (clip_space_pos.z >= far) return 1.0f;
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    linearDepth = clamp(linearDepth, near, far); // Clamp to valid range
    float retVal = clamp(linearDepth / far, 0.0, 1.0);
    
    return retVal;
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeLinearDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0.0, 0.5 - linearDepth); // Ensure fading is within valid range

    vec4 grid1 = grid(fragPos3D, 10.0);
    vec4 grid2 = grid(fragPos3D, 1.0);
    color = (grid1 + grid2) * float(t > 0.0);
    color.a *= fading;
}
