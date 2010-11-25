//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TextureManager.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER TextureManager;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER::TEXTURE_MANAGER()
{
	InvalidTexture = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER::~TEXTURE_MANAGER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears all textures
//+-----------------------------------------------------------------------------
VOID TEXTURE_MANAGER::Clear()
{
	for (INT i=0; i<TextureContainer.GetTotalSize(); i++)
	{
		if (TextureContainer.ValidIndex(i))
		{
			delete TextureContainer[i];
		}
	}

	TextureContainer.Clear();
}


//+-----------------------------------------------------------------------------
//| Clears all textures including replaceable textures
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::LoadAllReplaceableTextures()
{
	InvalidTexture = InternalLoad("Textures\\BTNtempW.blp");
	if (InvalidTexture == NULL) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Clears all textures except for for the replacealbe textures
//+-----------------------------------------------------------------------------
VOID TEXTURE_MANAGER::UnLoadAllReplaceableTextures()
{
	SAFE_DELETE(InvalidTexture);
}


//+-----------------------------------------------------------------------------
//| Loads a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER::Load(const std::string &FileName)
{
	INT Index;
	TEXTURE* Texture;
	
	Index = TextureContainer.GetIndex(FileName);
	if (Index != INVALID_INDEX) return TRUE;

	Texture = InternalLoad(FileName);
	if (Texture == NULL) return FALSE;

	if (!TextureContainer.Add(FileName, Texture))
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", unable to add texture!");
		delete Texture;
		return FALSE;
	}

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Returns a texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::GetTexture(CONST std::string &FileName)
{
	INT Index;

	Index = TextureContainer.GetIndex(FileName);
	if (Index == INVALID_INDEX) return InvalidTexture;

	return TextureContainer[Index];
}

//+-----------------------------------------------------------------------------
//| Loads a texture
//+-----------------------------------------------------------------------------
TEXTURE* TEXTURE_MANAGER::InternalLoad(const std::string &FileName)
{
	BUFFER Buffer;
	TEXTURE* Texture;

	Texture = new TEXTURE();
	if (Texture == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", memory allocation failed!");
		return NULL;
	}

	if (!FileLoader.Load(FileName, Buffer))
	{
		delete Texture;
		return NULL;
	}

	if (!ResourceLoader.LoadTexture((*Texture), FileName, Buffer))
	{
		delete Texture;
		return NULL;
	}

	return Texture;
}