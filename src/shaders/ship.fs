#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;


uniform sampler2D ourTexture;

void main()
{
//FragColor = vec4(0.5,0.5,0.5,1.0);
    FragColor = texture(ourTexture, TexCoord);
}