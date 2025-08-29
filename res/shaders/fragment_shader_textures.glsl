#version 330 core

in vec2 TexCoord;

uniform sampler2D u_elmoTexture;
uniform sampler2D u_obamaTexture;

uniform vec3 u_lightColor;

out vec4 FragColor;

void main() {

    vec4 textureMix = mix(
        texture(u_elmoTexture, TexCoord),
        texture(u_obamaTexture, TexCoord),
        0.5
    );

    FragColor = vec4(u_lightColor, 1.0f) * textureMix;
}
