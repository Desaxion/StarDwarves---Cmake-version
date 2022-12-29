#version 330 core
out vec4 FragColor;
  
//in vec3 ourColor;
//in vec2 TexCoord;
uniform vec4 metColor;

//uniform sampler2D ourTexture;

void main()
{
    //FragColor = vec4(1.0,0.5,0.5,1.0);
    FragColor = metColor;
    //FragColor = texture(ourTexture, TexCoord);
}