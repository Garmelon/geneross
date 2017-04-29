// Compute the pixelwise diff between two textures, one row at a time.
// Assumes both images are not transparent.
// Warning: Only works on rows <= ((2**8)**4)/255 = 16843009

// try isampler2D, ivec4 etc.? Probably not
uniform vec2 size;
uniform bool horizontal;
// uniform vec2 offs;
uniform sampler2D base;
uniform sampler2D curr;

void main(void)
{
	// calculate total diff of row in either direction
	float total = 0.0;
	
	if (horizontal) {
		float posy = gl_FragCoord.y/size.y;
		for (float x=0.0; x<=1.0; x+=(1.0/size.x)) {
			vec4 color     = texture2D(curr, vec2(x,     posy));
			vec4 basecolor = texture2D(base, vec2(x, 1.0-posy));
			color.rgb = abs(color.rgb - basecolor.rgb);
			total = total + color.r + color.g + color.b;
		}
	} else {
		float posx = gl_FragCoord.x/size.x;
		for (float y=0.0; y<=1.0; y+=(1.0/size.y)) {
			vec4 color     = texture2D(curr, vec2(posx,     y));
			vec4 basecolor = texture2D(base, vec2(posx, 1.0-y));
			color.rgb = abs(color.rgb - basecolor.rgb);
			total = total + color.r + color.g + color.b;
		}
	}
	
	// encode to rgba channels in current pixel
	vec4 col = vec4(0.0, 0.0, 0.0, 1.0);
// 	total = total/200.0;
	col.r = mod(total,         256.0/255.0);
	col.g = mod(total/256.0,   256.0/255.0);
	col.b = mod(total/65536.0, 256.0/255.0);
// 	color.g = 0.0;
// 	color.b = 0.0;
// 	color.g = mod(total, 256.0/255.0);
// 	color.r = float(mod(total, 256))/255.0;
// 	total = total/256;
// 	color.r = 1.0/255.0;
// 	color.r = mod(total, 256.0/255.0);
// 	total = (total - color.r) / (256.0/255.0);
// 	color.g = mod(total, 256.0/255.0); total = (total - color.g) / (256.0/255.0);
// 	color.b = mod(total, 256.0/255.0);
// 	color.a = 1.0;
	
// 	color.r = 0;
	
	gl_FragColor = col;
// 	color.a = mod(total, 256.0/255.0);
	
// 	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}

/*
void main(void)
{
	vec2 pos = gl_FragCoord.xy/size;
	vec4 color = texture2D(curr, pos);
	pos.y = 1.0 - pos.y;
	vec4 basecolor = texture2D(base, pos);
	color.rgb = abs(color.rgb - basecolor.rgb);
	gl_FragColor = color;
}
*/
