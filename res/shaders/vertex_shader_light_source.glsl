#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 u_projectionTransform;
uniform mat4 u_viewTransform;
uniform mat4 u_modelTransform;

out vec2 TexCoord;

void main() {
    gl_Position = u_projectionTransform * u_viewTransform * u_modelTransform * vec4(aPos, 1.0);
    TexCoord    = vec2(aTexCoords.x, 1.0 - aTexCoords.y);
}
