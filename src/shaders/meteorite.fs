#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPosition;
//in float displacement;
//in vec2 TexCoord;?
uniform vec4 metColor; //this could be proceduraly generated as well.


//vec3 LightEnd =  vec3(-1.0, -1.0, 0.0);
//uniform sampler2D ourTexture;


//vec3 globalLightDirection = normalize(LightEnd - LightPosition);

uniform vec3 shipDirection;
uniform vec3 meteorPosition;
uniform vec3 lightDirection;
uniform vec3 shipPosition;

//vec3 LightPosition = meteorPosition + vec3(2, 1,1);
//vec3 localLightDirection =  normalize(vec3(2,1,1));

vec3 color = vec3(1,1,1);

vec3 ambient = vec3(0.1,0.1,0.1);

void main()
{  
    float distance = sqrt((shipPosition.x - meteorPosition.x)*(shipPosition.x - meteorPosition.x) + (shipPosition.y - meteorPosition.y)*(shipPosition.y - meteorPosition.y) + (shipPosition.z - meteorPosition.z)*(shipPosition.z - meteorPosition.z) );
   
     
     vec3 diffuse =  color * max(dot(lightDirection, Normal), 0.0) ;// vec4(displacement, displacement, displacement,1.0);
     diffuse = diffuse + vec3(0.7,0.65,0.5)*max(dot(normalize(-shipDirection), Normal), 0.0)/(distance*distance)*(1 - (meteorPosition.z/abs(meteorPosition.z)))*0.7;

     //diffuse = shipDirection;

   vec3 finalColor =  diffuse;
   //FragColor = vec4(meteorPosition,1.0);
    
    //FragColor = vec4(1.0,0.5,0.5,1.0);
    //FragColor = metColor;
    //FragColor = vec4(Normal, 1.0);
    //FragColor = texture(ourTexture, TexCoord);
    FragColor  = vec4(finalColor,1.0);
}