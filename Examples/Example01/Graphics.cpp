//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
GRAPHICS Graphics;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
GRAPHICS::GRAPHICS()
{

}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
GRAPHICS::~GRAPHICS()
{

}


//+-----------------------------------------------------------------------------
//| Sets a new world matrix
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetWorldMatrix(CONST MATRIX4& NewWorldMatrix)
{
	WorldMatrix = NewWorldMatrix;
	//Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
}


//+-----------------------------------------------------------------------------
//|
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::SetShader()
{
	ProgramObject = esLoadProgramFromFile("Simple.vert", "Simple.frag");
	PositionSlot = glGetAttribLocation(ProgramObject, "Position");
	TexturePositionSlot = glGetAttribLocation(ProgramObject, "TexturePosition");
	TextureSlot = glGetUniformLocation(ProgramObject, "Texture");
	WVPSlot = glGetUniformLocation(ProgramObject, "WVPMatrix");
	return TRUE;
}

//+-----------------------------------------------------------------------------
//| 
//+-----------------------------------------------------------------------------
GLuint GRAPHICS::Program() CONST
{
	return ProgramObject;
}


//+-----------------------------------------------------------------------------
//| 
//+-----------------------------------------------------------------------------
GLuint GRAPHICS::Position() CONST
{
	return PositionSlot;
}


//+-----------------------------------------------------------------------------
//| 
//+-----------------------------------------------------------------------------
GLuint GRAPHICS::TexturePosition() CONST
{
	return TexturePositionSlot;
}


//+-----------------------------------------------------------------------------
//| 
//+-----------------------------------------------------------------------------
GLuint GRAPHICS::Texture() CONST
{
	return TextureSlot;
}


//+-----------------------------------------------------------------------------
//| 
//+-----------------------------------------------------------------------------
GLuint GRAPHICS::WVPMatrix() CONST
{
	return WVPSlot;
}