#version 330 core

out vec4 FragColor;
in vec2 pos;

void main() {
    FragColor = vec4(pos.x * 2, sin(pos.y + 1), 0.2f, 1.0f);
}
