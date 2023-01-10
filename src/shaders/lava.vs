#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 rotation;
uniform float deltaTime;

out vec3 FragPosition;
out vec3 Normal;
out vec4 Color;
out vec4 posTest;
out vec2 TexCoord;


vec4 tempNormal =  projection*rotation*vec4(aNormal, 1.0);

float truncSin(float x, int m) {

	float n = sin(x) * m;
	return n - floor(n);;
}

vec2 random2(vec2 st)
{
    st = vec2(dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)));
			
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}


void main()
{   
	Normal = tempNormal.xyz;
	vec4 pos = vec4(aPos, 1.0);

	pos.y += abs(sin(deltaTime) * random2(aTexCoord).y + cos(deltaTime/20) * random2(aTexCoord).x); //Change the y position of the vertex to simulate a wave
	
	if(pos.y > 0.7) {
		pos.y = 0.7;
	}


	vec4 tempPosition = projection*view*model*pos; //Calculate the position of the vertex on the screen
	
	FragPosition = tempPosition.xyz;
    gl_Position = tempPosition;
	TexCoord = aTexCoord;
}



