#version 330 core

in vec3 color;
in vec2 TexCoord;

uniform sampler2D u_elmoTexture;

out vec4 FragColor;

void main() {
//    vec2 flippedTexCoord = vec2(TexCoord.x, -TexCoord.y);
//    FragColor = texture(u_elmoTexture, flippedTexCoord);
    FragColor = texture(u_elmoTexture, TexCoord);
}
