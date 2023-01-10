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
uniform vec3 lightColor;
uniform bool bFog;

//vec3 LightPosition = meteorPosition + vec3(2, 1,1);
//vec3 localLightDirection =  normalize(vec3(2,1,1));

//Fog implementation by Victor Gordan https://www.youtube.com/watch?v=3xGKu4T4SCU
float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth) {
    return (2.0 * near * far) / (far  + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 1.5f, float offset = 5.0f) {
    float zVal = linearizeDepth(depth);
    return (1 / (1 + exp(-steepness * (zVal - offset))));
}

vec3 color = lightColor;

vec3 ambient = color * 0.1;

void main()
{ 
    vec3 finalColor = vec3(0.0);
    float distance = sqrt((shipPosition.x - meteorPosition.x)*(shipPosition.x - meteorPosition.x) + (shipPosition.y - meteorPosition.y)*(shipPosition.y - meteorPosition.y) + (shipPosition.z - meteorPosition.z)*(shipPosition.z - meteorPosition.z) );
   
     
     vec3 diffuse =  color * max(dot(lightDirection, Normal), 0.0) ;// vec4(displacement, displacement, displacement,1.0);
     diffuse += vec3(0.7,0.65,0.5)*max(dot(normalize(-shipDirection), Normal), 0.0)/(distance*distance)*(1 - (meteorPosition.z/abs(meteorPosition.z)))*0.7;

     //diffuse = shipDirection;
    if(bFog)
    {
        float depth = logisticDepth(gl_FragCoord.z);
        FragColor = vec4(diffuse, 1.0f) * (1.0 - depth) + vec4(depth * vec3(0.8, 0.67, 0.54), 1.0f);
    }
    else
    {
        FragColor  = vec4(diffuse,1.0);
    }       
   //FragColor = vec4(meteorPosition,1.0);
    
    //FragColor = vec4(1.0,0.5,0.5,1.0);
    //FragColor = metColor;
    //FragColor = vec4(Normal, 1.0);
    //FragColor = texture(ourTexture, TexCoord);
    
}