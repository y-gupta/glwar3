#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class TEXTURE;

//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT NR_OF_REPLACEABLE_TEXTURES = 13;


//+-----------------------------------------------------------------------------
//| Texture manager class
//+-----------------------------------------------------------------------------
class TEXTURE_MANAGER
{
public:
	CONSTRUCTOR TEXTURE_MANAGER();
	DESTRUCTOR ~TEXTURE_MANAGER();

	VOID Clear();

	BOOL LoadAllReplaceableTextures();
	VOID UnLoadAllReplaceableTextures();

	BOOL Load(CONST std::string& FileName);
	BOOL UnLoad(CONST std::string& FileName);

	TEXTURE* GetTexture(CONST std::string& FileName);

protected:
	TEXTURE* InternalLoad(CONST std::string& FileName);

	TEXTURE* InvalidTexture;

	CONTAINER<TEXTURE*> TextureContainer;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_MANAGER TextureManager;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "ResourceLoader.h"