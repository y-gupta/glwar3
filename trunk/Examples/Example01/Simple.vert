attribute vec4 Position;
attribute vec2 TexCoord;

varying vec2 DestTexCoord;

void main(void)
{
	DestTexCoord = TexCoord;
	gl_Position = Position;
}