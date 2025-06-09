#version 330 core

out vec4 FragColor;
in vec2 pos;

void main() {
    FragColor = vec4(pos.x / 2, cos(pos.y - 1), 0.5f, 1.0f);
//    FragColor = vec4(1.);
}
