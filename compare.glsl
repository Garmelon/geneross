// Compute the pixelwise diff between two textures.
// Computes the diff for each color chanel separately (r, g, b).
// Assumes both images are not transparent.

// try isampler2D, ivec4 etc.? Probably not
uniform vec2 size;
uniform sampler2D base;
uniform sampler2D curr;

void main(void)
{
	vec2 pos = gl_FragCoord.xy/size;
// 	vec4 color = texture(curr, pos);  // fails for some reason!?
// 	vec4 basecolor = texture(base, pos);  // fails too...
// 	color.xyz = abs(color.xyz - basecolor.xyz);
// 	gl_FragColor = color;
// 	gl_FragColor = basecolor;
	gl_FragColor = vec4(1.0);
}
