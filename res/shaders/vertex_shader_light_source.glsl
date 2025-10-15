#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 u_projectionTransform;
uniform mat4 u_viewTransform;
uniform mat4 u_modelTransform;

void main() {
    gl_Position = u_projectionTransform * u_viewTransform * u_modelTransform * vec4(aPos, 1.0);
}
