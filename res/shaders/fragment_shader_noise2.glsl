#version 330 core

in vec2 TexCoord;

uniform sampler2D u_elmoTexture;
uniform sampler2D u_obamaTexture;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(u_elmoTexture, TexCoord), texture(u_obamaTexture, vec2(TexCoord)), 0.5);
}
