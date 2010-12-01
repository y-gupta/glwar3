#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model texture data structure
//+-----------------------------------------------------------------------------
struct MODEL_TEXTURE_DATA
{
	MODEL_TEXTURE_DATA()
	{

	}

	INT ReplaceableId;

	BOOL WarpWidth;
	BOOL WarpHeight;

	std::string FileName;

	INT InternalTextureId;
};


//+-----------------------------------------------------------------------------
//| Model texture class
//+-----------------------------------------------------------------------------
class MODEL_TEXTURE
{
public:
	CONSTRUCTOR MODEL_TEXTURE();
	DESTRUCTOR ~MODEL_TEXTURE();

	VOID Clear();
	INT GetSize();

	MODEL_TEXTURE_DATA& Data();

protected:
	MODEL_TEXTURE_DATA TextureData;

public:

};