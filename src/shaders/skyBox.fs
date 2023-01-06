#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

vec2 screenLocation = gl_FragCoord.st;

uniform float time;
//uniform sampler2D ourTexture;


vec2 random2(vec2 st)
{
	st = vec2(dot(st, vec2(127.1, 311.7)),
		dot(st,vec2(269.5, 183.3)));
	return vec2(-1.0 + 2.0 *fract(sin(st.s) * 43758.5453123), -1.0 + 2.0 * fract(sin(st.t) * 43758.5453123));
}

float noise(vec2 st)
{
	vec2 i = floor(st);
	vec2 f = fract(st);

	vec2 u = vec2(f.s * f.s * (3.0 - 2.0 * f.s), f.t * f.t * (3.0 - 2.0 * f.t));

	return mix(mix(dot(random2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)),
			dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), u.x),
			mix(dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)),
			dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), u.x), u.y);
}

float noiseScale = 0.12;


void main()
{
	//vec2 rand = vec2(sin(random2(gl_FragCoord.st).s*noiseScale),cos(random2(gl_FragCoord.st).t*noiseScale));

	float noisy = round(noise(screenLocation*noiseScale) + 0.1);
	//float noisy = noise(screenLocation*noiseScale);
	vec3 noises = vec3(noisy);

	//vec2 rand = vec2(truncatedTrig(screenLocation));//sin(screenLocation.s*noiseScale),cos(screenLocation.t*noiseScale))*random;


	vec4 color =  vec4(noises,1.0);

    //FragColor = vec4(0.5,0.5,0.5,1.0);
    FragColor = color;//vec4(TexCoord, 0.0,1.0);//texture(ourTexture, TexCoord);
}