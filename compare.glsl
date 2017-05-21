// Compute the pixelwise diff between two textures, one row at a time.
// Assumes both images are not transparent.
// Warning: Only works on rows <= 65793
//          Formula for max pixels per row: (256^3)/255

uniform vec2 size;
uniform bool horizontal;
uniform sampler2D base;
uniform sampler2D curr;

void main(void)
{
	// calculate total diff of row in either direction
	float total = 0.0;
	
	if (horizontal) {
		float posy = gl_FragCoord.y/size.y;
		for (float x=0.0; x<=1.0; x+=(1.0/size.x)) {
			vec4 color     = texture2D(curr, vec2(x, posy));
			vec4 basecolor = texture2D(base, vec2(x, posy));
			color.rgb = abs(color.rgb - basecolor.rgb);
			total = total + color.r + color.g + color.b;
		}
	} else {
		float posx = gl_FragCoord.x/size.x;
		for (float y=0.0; y<=1.0; y+=(1.0/size.y)) {
			vec4 color     = texture2D(curr, vec2(posx, y));
			vec4 basecolor = texture2D(base, vec2(posx, y));
			color.rgb = abs(color.rgb - basecolor.rgb);
			total = total + color.r + color.g + color.b;
		}
	}
	
	// encode to rgba channels in current pixel
	vec4 col = vec4(0.0, 0.0, 0.0, 1.0);
	
	col.r = floor(mod(total*255.0,         256.0))/255.0;
	col.g = floor(mod(total*255.0/256.0,   256.0))/255.0;
	col.b = floor(mod(total*255.0/65536.0, 256.0))/255.0;
	
	gl_FragColor = col;
}
