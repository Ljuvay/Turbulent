#version 330 core

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

layout(location = 0) in vec3 aPos;    
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 aColor;
layout(location = 4) in float aRadius;

out vec3 vPos;
out vec3 vColor;

void main()
{
    vPos = aPos;
    vColor = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
