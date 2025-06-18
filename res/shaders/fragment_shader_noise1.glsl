#version 330 core

in vec2 pos;

uniform float u_colorMultiplier;

//uniform vec4 u_vec4Stuff;
uniform vec3 u_vec3Stuff;

out vec4 FragColor;

void main() {
//    FragColor = vec4(u_colorMultiplier, cos(pos.x), pos.y, 1.0f);
//    FragColor = vec4(u_vec4Stuff);
    FragColor = vec4(u_colorMultiplier, u_vec3Stuff);
}
