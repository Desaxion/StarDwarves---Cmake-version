#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//out vec3 ourColor;
//out vec2 TexCoord;

//out float displacement;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 rotation;


out vec3 FragPosition;
out vec3 Normal;

vec4 tempNormal =  rotation*vec4(aNormal, 1.0);





vec3 random3(vec3 st)
{
    st =vec3(dot(st, vec3(127.1, 311.7, 543.21)),
        dot(st, vec3(269.5, 183.3, 355.23)),
        dot(st, vec3(846.34, 364.45, 123.65))); // Haphazard additional numbers by IR
    return vec3( -1.0 + 2.0 * fract(sin(st.x) * 43758.5453123), -1.0 + 2.0 * fract(sin(st.y) * 43758.5453123), -1.0 + 2.0 * fract(sin(st.z) * 43758.5453123));
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
// Trivially extended to 3D by Ingemar
float gradientNoise(vec3 st)
{
    vec3 i = floor(st);
    vec3 f = fract(st);

    vec3 u = vec3(f.x * f.x * (3.0 - 2.0 * f.x), f.y * f.y * (3.0 - 2.0 * f.y), f.z * f.z * (3.0 - 2.0 * f.z));

    return mix(
           mix(mix(dot(random3(i + vec3(0.0, 0.0, 0.0)), f - vec3(0.0, 0.0, 0.0)),
           dot(random3(i + vec3(1.0, 0.0, 0.0)), f - vec3(1.0, 0.0, 0.0)), u.x),
           mix(dot(random3(i + vec3(0.0, 1.0, 0.0)), f - vec3(0.0, 1.0, 0.0)),
           dot(random3(i + vec3(1.0, 1.0, 0.0)), f - vec3(1.0, 1.0, 0.0)), u.x), u.y),

           mix(mix(dot(random3(i + vec3(0.0, 0.0, 1.0)), f - vec3(0.0, 0.0, 1.0)),
           dot(random3(i +vec3(1.0, 0.0, 1.0)), f - vec3(1.0, 0.0, 1.0)), u.x),
           mix(dot(random3(i +vec3(0.0, 1.0, 1.0)), f -vec3(0.0, 1.0, 1.0)),
           dot(random3(i + vec3(1.0, 1.0, 1.0)), f - vec3(1.0, 1.0, 1.0)), u.x), u.y), u.z

    );
}



float displacementScaling = 1;

int octaves = 8;


void main()
{   
    

    float displacement = 0.0f;

    //for (int k = 1; k < octaves + 1; k++) {
    //	displacement += gradientNoise(aPos) *displacementScaling;
    //}

    //Normal = rotation*vec4(aNormal);
    //tempNormal = rotation*tempNormal;
    Normal = tempNormal.xyz;
    vec4 tempPosition = projection*view*model*vec4(aPos, 1.0);
    FragPosition = tempPosition.xyz;

    //FragPosition = tempPosition.xyz +  displacement * Normal * displacementScaling; //The displacement is added in the direction of the normal 
    
    //displacement = 0.1;

    gl_Position = tempPosition;//vec4(FragPosition, 1.0);
   // ourColor = aColor;
   // TexCoord = aTexCoord;
}



