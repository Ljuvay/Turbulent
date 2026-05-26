#version 330 core

out vec4 FragColor;

in vec3 vPos;
in vec3 vColor;

uniform float maxTerrainHeight;



void main()
{
	float hVal = (vPos.y + 13) / 220.0; // Min - Max
	vec3 vColor;

	if(hVal < 0.15){vColor = vec3(0.75, 0.7, 0.5);} //Sand
	else if(hVal <= 0.3){vColor = vec3(0.12, 0.25, 0.0);} //Grass
	else if(hVal <= 0.85){vColor = vec3(0.5);} //Stone
	else{vColor = vec3(1.0);}

	FragColor = vec4(vColor, 1.0f);
}

