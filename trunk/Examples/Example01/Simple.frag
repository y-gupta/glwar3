precision mediump float;

uniform sampler2D Texture;

varying vec2 DestTexturePosition;

void main(void)
{
	vec4 baseColor =  texture2D(Texture, DestTexturePosition);
	
	if( baseColor.a < 0.25 )
	{
		discard;
	}
	else
	{
		gl_FragColor = baseColor;
	}
}
