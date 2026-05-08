#version 330 core

out vec4 FragColor;

in vec3 vPos;

void main()
{
    float xCol = (1.0f);
    float yCol = (0.5f);
    float zCol = (0.0f);

    FragColor = vec4(xCol, yCol, zCol, 1.0f);
}