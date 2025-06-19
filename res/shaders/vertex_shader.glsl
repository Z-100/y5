#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform float u_positionMultiplier;

out vec3 color;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos.x + (u_positionMultiplier/2), aPos.y + (u_positionMultiplier/2), aPos.z, 1.0);
    color       = aColor;
    TexCoord    = aTexCoords;
}
