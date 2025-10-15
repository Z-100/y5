#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_projectionTransform;
uniform mat4 u_viewTransform;
uniform mat4 u_modelTransform;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main() {

    FragPos = vec3(u_modelTransform * vec4(aPos, 1.0));
    TexCoord = vec2(aTexCoords.x, 1.0 - aTexCoords.y);
    Normal = mat3(transpose(inverse(u_modelTransform))) * aNormal;

    gl_Position = u_projectionTransform * u_viewTransform * vec4(FragPos, 1.0);
}
