#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class GRAPHICS;

//+-----------------------------------------------------------------------------
//| Texture class
//+-----------------------------------------------------------------------------
class TEXTURE
{
public:
	CONSTRUCTOR TEXTURE();
	DESTRUCTOR ~TEXTURE();

	VOID Clear();

	INT GetTextureId() CONST;
	std::string GetFileName() CONST;

	INT GetWidth() CONST;
	INT GetHeight() CONST;
	INT GetRealWidth() CONST;
	INT GetRealHeight() CONST;

	BOOL CreateEmpty(INT NewWidth, INT NewHeight);

protected:
	VOID Attach();

	INT TextureId;
	std::string FileName;

	INT	Width;
	INT Height;
	INT RealWidth;
	INT RealHeight;

	REFERENCE<TEXTURE*, GRAPHICS*> TextureReference;

	friend class TEXTURE_LOADER_BASIC;
	friend class TEXTURE_LOADER_BLP;
	friend class TEXTURE_LOADER_PCX;
};

//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"
