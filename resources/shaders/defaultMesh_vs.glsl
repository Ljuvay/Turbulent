#version 330 core

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

layout(location = 0) in vec3 aPos;    
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 vPos;
flat out vec3 vColor;

void main()
{
    vPos = aPos;
    vColor = vec3(1.0, 0.5, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
