#version 330 core
out vec4 FragColor;
  

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float ambientScaling;

vec3 color = lightColor;




void main()
{
    //FragColor = vec4(0.5,0.5,0.5,1.0);
    vec4 ambient = vec4(lightColor*ambientScaling,1.0) * texture(ourTexture, TexCoord);
    FragColor = dot(lightDirection,Normal)*texture(ourTexture, TexCoord) + ambient;
}