#version 330 core

layout (location = 0)

in vec3 aPos;
out vec2 pos;

void main() {
    pos = aPos.xy;
    gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
}
