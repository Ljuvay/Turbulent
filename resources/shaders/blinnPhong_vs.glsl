#version 330 core

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

layout(location = 0) in vec3 position;    
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 vPos;
out vec3 vNorm;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
    vPos = position;
    TexCoord = uv;
    vNorm = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
}
