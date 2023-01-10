#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPosition;
in vec4 posTest;
in vec2 TexCoord;


uniform vec4 metColor; //this could be proceduraly generated as well.
uniform float deltaTime;

uniform vec3 shipDirection;
uniform vec3 meteorPosition;
uniform vec3 lightDirection;
uniform vec3 shipPosition;
uniform bool bFog;

vec3 color = vec3(1,1,1);
vec3 ambient = vec3(0.1,0.1,0.1);

//Written by Ingemar Ragnemalm
vec2 random2(vec2 st)
{
    st = vec2(dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)));
			
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

vec2 rot2(vec2 v, float r)
{
    vec2 res;
    res.x = cos(r)*v.x + sin(r)*v.y;
    res.y = -sin(r)*v.x + cos(r)*v.y;
    return res;
}

// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
// This is a 2D gradient noise. Input your texture coordinates as argument, scaled properly.
// Conversion to flow noise by Ingemar Ragnemalm
float noise(vec2 st, float r)
{
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

	
    return mix( mix( dot( rot2( random2(i + vec2(0.0,0.0) ),r), f - vec2(0.0,0.0) ),
                     dot( rot2( random2(i + vec2(1.0,0.0) ),r), f - vec2(1.0,0.0) ), u.x),
                mix( dot( rot2( random2(i + vec2(0.0,1.0) ),r), f - vec2(0.0,1.0) ),
                     dot( rot2( random2(i + vec2(1.0,1.0) ),r), f - vec2(1.0,1.0) ), u.x), u.y);
}
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

void main()
{    
    float r = 0.56;
 
    vec4 resColor = vec4(1.0);
    vec4 colNoise = vec4(1.0);
    vec4 grayNoise = vec4(1.0);

    vec2 st = TexCoord;
    st *= 100.0f;
    st.x += 10f;
    st.y -= 8*deltaTime;
    
    //colNoise *= vec4(noise(st, deltaTime), noise(vec2(st.x + 0.5, st.y - 0.32*sin(deltaTime)), deltaTime * 2), noise(st, r), 1.0);
    
    for(int i = 0; i < 4; i++) {
        colNoise += vec4(noise(st, deltaTime*random2(st).y) * random2(st).y, 0, 0, 1) + vec4(0, noise(st, deltaTime)*2, 0, 1);
        grayNoise += vec4(noise(st, random2(st).x), noise(st, random2(st).x * deltaTime), noise(st, random2(st).x * deltaTime), 1.0);
    }
     
    resColor = vec4(1.0, colNoise.y/3, 0.0, 1.0);

    if(colNoise.y/3 < 0.01) {
        resColor =  vec4(0.5, 0.01, 0.0, 1.0) * vec4(grayNoise.x/3, 1.0, 0.0, 1.0);
    }

    if(colNoise.y/3 > 0.99998) {
        resColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    
    if(bFog) {
       float depth = logisticDepth(gl_FragCoord.z);
       FragColor = FragColor = resColor * (1.0 - depth) + vec4(depth * vec3(0.8, 0.67, 0.54), 1.0f);
    }
    else {
        FragColor = resColor;
    }
   

    
    
    
    
    /*vec4 color = vec4(1.0,0.4,0.0,1.0);
    vec4 resCol = vec4(1.0,0.4,0.0,1.0);
    vec4 testCol = vec4(1.0,1.0,1.0,1.0);
     
    vec2 coord = TexCoord;
    coord *= 1000.0f;
    coord.x += 10f;

    coord.y -= 8*deltaTime; 

    testCol *= vec4(noise(coord, deltaTime), noise(vec2(coord.x + 0.5, coord.y - 0.32*sin(deltaTime)), deltaTime * 2), noise(coord, r), 1.0);
    
    
    coord.x += 0.01*cos(deltaTime*10);
   


    for(int i = 0; i < 4; i++)
    {
        resCol += vec4(noise(coord, r*random2(coord).y) * random2(coord).y, 0, 0, 1) + vec4(0, noise(coord, r)*2, 0, 1);
    }

    if(resCol.r < 0.2 && resCol.g > 0.2) 
    {
        resCol.r = 0.8;
        resCol.g = 0.5;
    }

    if(testCol.r < 0.5)
    {
        testCol.r = 1.0;    
    } else if(testCol.g < 0.5) {
        testCol.g = 1.0;
    }
    
   
    FragColor = vec4(testCol.y, testCol.y, testCol.y, 1.0) * vec4(testCol.x, testCol.x, testCol.x, 1.0) * resCol;*/
    //FragColor = (testCol.x *  resCol) - vec4(0.0,0.0,0.5,0.0);

}