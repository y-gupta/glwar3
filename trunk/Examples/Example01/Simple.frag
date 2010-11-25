precision mediump float;

varying vec2 DestTexCoord;

uniform sampler2D Sampler;

void main(void)
{
	gl_FragColor = texture2D(Sampler, DestTexCoord);
}
