attribute vec4 Position;
attribute vec2 TexturePosition;

uniform mat4 WVPMatrix;

varying vec2 DestTexturePosition;

void main(void)
{
	DestTexturePosition = TexturePosition;
	gl_Position =  WVPMatrix * Position;
}