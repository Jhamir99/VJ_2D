#version 330

uniform vec4 color;
uniform sampler2D tex;

in vec2 texCoordFrag;
out vec4 outColor;

in vec4 pos;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.a < 0.5f)
		discard;
	
	if(pos[1] > 0)
		outColor = color * texColor;
	else
		outColor = color * texColor * vec4(0.8, 0.8, 1.2, 1.0);
}

