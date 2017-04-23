#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float smove(float value)
{
	return (value + 1.0)/2.0;
}

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(void)
{
	vec2 pos = gl_FragCoord.xy;  // rename for more meaty code
	mouse.y = -mouse.y;
// 	vec2 uv = pos/resolution.xy;
// 	vec2 muv = mouse/resolution.xy;
	
	float scale = 100;
	float modifier = sin(pos.x/scale + time)*sin(pos.y/scale - time);
	modifier = smove(modifier);
	
	float dist = distance(gl_FragCoord.xy, mouse);
	float radius = smove(sin(time))*50 + 100;
	
	float value = max(1 - dist/radius, 0);
	value = value + modifier*0.3;
	
	value = value + (rand(pos.x*pos.y)-0.5)*0.03;
	
	gl_FragColor = vec4(value);
}
