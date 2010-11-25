#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class TEXTURE;
class TEXTURE_LOADER;


//+-----------------------------------------------------------------------------
//| Resource loader class
//+-----------------------------------------------------------------------------
class RESOURCE_LOADER
{
public:
	CONSTRUCTOR RESOURCE_LOADER();
	DESTRUCTOR ~RESOURCE_LOADER();

	VOID RegisterAllLoader();
	VOID ClearAllLoaders();

	BOOL IsTextureExtention(CONST std::string& Extention) CONST;

	BOOL SaveTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST;
	BOOL LoadTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST;

protected:
	VOID RegisterTextureLoader(CONST std::string& Extention, TEXTURE_LOADER* TextureLoader);

	std::map<std::string, TEXTURE_LOADER*> TextureLoaderMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern RESOURCE_LOADER ResourceLoader;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "TextureLoader.h"
