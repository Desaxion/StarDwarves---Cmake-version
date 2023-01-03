#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPosition;
//in float displacement;
//in vec2 TexCoord;
uniform vec4 metColor; //this could be proceduraly generated as well.

vec3 LightPosition = vec3(1.0, 5.0, 0.0);
vec3 LightEnd =  vec3(-1.0, -1.0, 0.0);
//uniform sampler2D ourTexture;

vec3 globalLightDirection = normalize(LightEnd - LightPosition);

uniform vec3 shipDirection;


void main()
{  
    //float distance = sqrt((FragPosition.x - LightPosition.x)*(FragPosition.x - LightPosition.x) + (FragPosition.y - LightPosition.y)*(FragPosition.y - LightPosition.y) + (FragPosition.z - LightPosition.z)*(FragPosition.z - LightPosition.z) );
   
    
     vec4 finalColor =  vec4(0.7,0.7,0.7,1.0) * max(dot(LightPosition, Normal),0.0) ;// vec4(displacement, displacement, displacement,1.0);
   // vec4 finalColor =  vec4(0.7,0.7,0.7,1.0) * max(dot(shipDirection, Normal), 0.0);
    
    
    //FragColor = vec4(1.0,0.5,0.5,1.0);
    //FragColor = metColor;
    //FragColor = vec4(Normal, 1.0);
    //FragColor = texture(ourTexture, TexCoord);
    FragColor  = finalColor;
}