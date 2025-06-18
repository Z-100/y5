#version 330 core

layout (location = 0) in vec3 aPos;

uniform float u_positionMultiplier;

out vec2 pos;

void main() {
    pos = aPos.xy;
    gl_Position = vec4(aPos.x + u_positionMultiplier, -aPos.y, aPos.z, 1.0);
}
