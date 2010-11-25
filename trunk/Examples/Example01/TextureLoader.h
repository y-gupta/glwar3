#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Texture.h"


//+-----------------------------------------------------------------------------
//| Texture loader class
//+-----------------------------------------------------------------------------
class TEXTURE_LOADER
{
public:
	CONSTRUCTOR TEXTURE_LOADER();
	DESTRUCTOR ~TEXTURE_LOADER();

	virtual BOOL Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) = 0;
	virtual BOOL Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) = 0;

protected:
	static std::string CurrentFileName;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "TextureLoaderBlp.h"
