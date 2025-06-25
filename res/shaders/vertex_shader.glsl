#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform float u_positionMultiplier;

uniform mat4 u_modelTransform;
uniform mat4 u_viewTransform;
uniform mat4 u_projectionTransform;

out vec3 color;
out vec2 TexCoord;

void main() {
    gl_Position = u_projectionTransform * u_viewTransform * u_modelTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoord    = aTexCoords;
}
