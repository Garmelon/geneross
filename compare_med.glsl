// Compute the pixelwise diff between two textures.
// Computes the diff for each color chanel separately (r, g, b).
// Assumes both images are not transparent.

// try isampler2D, ivec4 etc.? Probably not
uniform vec2 size;
uniform vec2 offs;
uniform sampler2D base;
uniform sampler2D curr;

void main(void)
{
	vec2 pos = (gl_FragCoord.xy-offs)/size;
	pos.y = 1.0 - pos.y;
	vec4 color = texture2D(curr, pos);
	vec4 basecolor = texture2D(base, pos);
	color.xyz = color.xyz - basecolor.xyz;
	color.xyz = (vec3(1.0, 1.0, 1.0) + color.xyz)/2.0;
	gl_FragColor = color;
}
