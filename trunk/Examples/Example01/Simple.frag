precision mediump float;

uniform sampler2D Texture;

varying vec2 DestTexturePosition;

void main(void)
{
	gl_FragColor = texture2D(Texture, DestTexturePosition);
}
