#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureManager.h"

//+-----------------------------------------------------------------------------
//| Graphics class
//+-----------------------------------------------------------------------------
class GRAPHICS
{
public:
	CONSTRUCTOR GRAPHICS();
	DESTRUCTOR ~GRAPHICS();

	VOID SetWorldMatrix(CONST MATRIX4& NewWorldMatrix);

protected:
	MATRIX4 ProjectionMatirx;
	MATRIX4 ViewMatrix;
	MATRIX4 WorldMatrix;
	MATRIX4 WorldViewProjectionMatrix;

	REFERENCE_OBJECT<TEXTURE*, GRAPHICS*> TextureReferenceObject;

	friend class GRAPHICS_WINDOW;
	friend class TEXTURE;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern GRAPHICS Graphics;