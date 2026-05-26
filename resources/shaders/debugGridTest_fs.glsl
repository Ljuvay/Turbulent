#version 330 core

uniform vec3 camPos;

in vec3 nearPoint;
in vec3 farPoint;

out vec4 FragColor;

void main()
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    if(t < 0.0) discard;

    vec3 worldPos = nearPoint + t * (farPoint - nearPoint);

    vec2 grid = abs(fract(worldPos.xz / 10.0) - 0.5);
    vec2 fw = fwidth(worldPos.xz / 20.0);
    vec2 smoothGrid = smoothstep(fw, 2.0 * fw, grid);
    float gridLine = 1.0 - min(smoothGrid.x, smoothGrid.y);

    float dist = length(camPos - worldPos);
    float fade = 1.0 - smoothstep(50.0, 250.0, dist);

    if(gridLine * fade < 0.01) discard;

    FragColor = vec4(1.0, 1.0, 1.0, gridLine * fade);
}
