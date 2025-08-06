#version 330 core

in vec2 TexCoord;

uniform vec3 u_lightSourceColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(u_lightSourceColor, 1.0f);
}
