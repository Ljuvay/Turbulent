#version 330 core

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vPos;

void main()
{
    vPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}