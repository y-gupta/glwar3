//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ResourceLoader.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
RESOURCE_LOADER ResourceLoader;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
RESOURCE_LOADER::RESOURCE_LOADER()
{

};


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
RESOURCE_LOADER::~RESOURCE_LOADER()
{
	ClearAllLoaders();
};


//+-----------------------------------------------------------------------------
//| Register all loaders
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::RegisterAllLoaders()
{
	ClearAllLoaders();

	RegisterTextureLoader("blp", &TextureLoaderBlp);
}


//+-----------------------------------------------------------------------------
//| Clears all loaders
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::ClearAllLoaders()
{
	TextureLoaderMap.clear();
}


//+-----------------------------------------------------------------------------
//| Checks if an extention is a texture extention
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::IsTextureExtention(CONST std::string &Extention) CONST
{
	std::map<std::string, TEXTURE_LOADER*>::const_iterator i;

	i = TextureLoaderMap.find(Common.LowerCase(Extention));
	if (i == TextureLoaderMap.end()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a texture
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::SaveTexture(TEXTURE &Texture, CONST std::string &FileName, BUFFER &Buffer) CONST
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a texture
//+-----------------------------------------------------------------------------
BOOL RESOURCE_LOADER::LoadTexture(TEXTURE& Texture, CONST std::string& FileName, BUFFER& Buffer) CONST
{
	std::string Extention;
	std::map<std::string, TEXTURE_LOADER*>::const_iterator i;

	Texture.Clear();

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	i = TextureLoaderMap.find(Extention);
	if (i == TextureLoaderMap.end())
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unknown extention \"" + Extention + "\"!");
		return FALSE;
	}

	return i->second->Load(Texture, FileName, Buffer);
}


//+-----------------------------------------------------------------------------
//| Registers a texture loader
//+-----------------------------------------------------------------------------
VOID RESOURCE_LOADER::RegisterTextureLoader(const std::string &Extention, TEXTURE_LOADER *TextureLoader)
{
	TextureLoaderMap.insert(std::make_pair(Common.LowerCase(Extention), TextureLoader));
}