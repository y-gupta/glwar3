attribute vec4 Position;
attribute vec2 TexturePosition;

varying vec2 DestTexturePosition;

void main(void)
{
	DestTexturePosition = TexturePosition;
	gl_Position =  Position;
}