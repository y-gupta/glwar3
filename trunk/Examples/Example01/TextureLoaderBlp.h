#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureLoader.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT MAX_NR_OF_BLP_MIP_MAPS = 16;


//+-----------------------------------------------------------------------------
//| Blp header structure
//+-----------------------------------------------------------------------------
struct BLP_HEADER
{

};


//+-----------------------------------------------------------------------------
//| Blp RGBA structure
//+-----------------------------------------------------------------------------
struct BLP_RGBA
{

};


//+-----------------------------------------------------------------------------
//| Blp pixel structure
//+-----------------------------------------------------------------------------
struct BLP_PIXEL
{

};


//+-----------------------------------------------------------------------------
//| Texture loader blp class
//+-----------------------------------------------------------------------------
class TEXTURE_LOADER_BLP : public TEXTURE_LOADER
{
public:
	CONSTRUCTOR TEXTURE_LOADER_BLP();
	DESTRUCTOR ~TEXTURE_LOADER_BLP();

	virtual BOOL Save(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);
	virtual BOOL Load(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer);

protected:
	static BOOL LoadCompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer);
	static BOOL LoadUncompressed(TEXTURE& Texture, BLP_HEADER& Header, BUFFER& Buffer);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEXTURE_LOADER_BLP TextureLoaderBlp;
