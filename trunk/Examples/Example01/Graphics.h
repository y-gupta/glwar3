#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Texture.h"

//+-----------------------------------------------------------------------------
//| Graphics class
//+-----------------------------------------------------------------------------
class GRAPHICS
{
public:
	CONSTRUCTOR GRAPHICS();
	DESTRUCTOR ~GRAPHICS();


protected:
	REFERENCE_OBJECT<TEXTURE*, GRAPHICS*> TextureReferenceObject;

	friend class GRAPHICS_WINDOW;
	friend class TEXTURE;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern GRAPHICS Graphics;