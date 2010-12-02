//+-----------------------------------------------------------------------------
//| Include files
//+-----------------------------------------------------------------------------
#include "ModelLoaderMdx.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDX ModelLoaderMdx;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDX::MODEL_LOADER_MDX()
{
	RegisterMdxLoader('VERS', LoadVersion);
	RegisterMdxLoader('MODL', LoadModel);
	RegisterMdxLoader('SEQS', LoadSequences);
	RegisterMdxLoader('GLBS', LoadGlobalSequences);
	RegisterMdxLoader('MTLS', LoadMaterials);
	RegisterMdxLoader('TEXS', LoadTextures);
	RegisterMdxLoader('TXAN', LoadTextureAnimations);
	RegisterMdxLoader('GEOS', LoadGeosets);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDX::~MODEL_LOADER_MDX()
{
	MdxLoaderMap.clear();
}


//+-----------------------------------------------------------------------------
//| Saves a model to a buffer
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model from a buffer 
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer)
{
	INT Size;
	DWORD Group;
	DATA_IN_STREAM DataStream;
	std::map<DWORD, MDX_LOADER>::iterator i;

	CurrentFileName = FileName;
	DataStream.SetFileName(FileName);

	if (!DataStream.Load(Buffer)) return FALSE;

	if (ReverseDWord(DataStream.ReadDWord()) != 'MDLX')
	{
		Error.SetMessage("The file is not an MDX model!");
		return FALSE;
	}

	while (!DataStream.Eof())
	{
		Group = ReverseDWord(DataStream.ReadDWord());
		Size = static_cast<INT>(DataStream.ReadDWord());

		std::string StrGroup =  GroupToString(Group);
		i = MdxLoaderMap.find(Group);
		if (i == MdxLoaderMap.end())
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", unknown group \"" + GroupToString(Group) + "\"!");
			return FALSE;
		}

		if (!(i->second(Model, DataStream, Size))) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model version
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadVersion(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	Model.Data().Info.Version = DataStream.ReadDWord();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model info
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadModel(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	CHAR Buffer[MODEL_NAME_SIZE + 1];
	
	Buffer[MODEL_NAME_SIZE] = '\0';

	if (!DataStream.Read(Buffer, MODEL_NAME_SIZE)) return FALSE;
	Model.Data().Info.Name = Buffer;

	DataStream.ReadDWord();

	Model.Data().Info.Extent.Radius = DataStream.ReadFloat();
	Model.Data().Info.Extent.Min.x = DataStream.ReadFloat();
	Model.Data().Info.Extent.Min.y = DataStream.ReadFloat();
	Model.Data().Info.Extent.Min.z = DataStream.ReadFloat();
	Model.Data().Info.Extent.Max.x = DataStream.ReadFloat();
	Model.Data().Info.Extent.Max.y = DataStream.ReadFloat();
	Model.Data().Info.Extent.Max.z = DataStream.ReadFloat();

	Model.Data().Info.BlendTime = static_cast<INT>(DataStream.ReadDWord());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT SequenceSize;

	CurrentSize = 0;
	while (CurrentSize < Size)
	{
		SequenceSize = 132;
		CurrentSize += SequenceSize;

		DataStream.SetPosition(DataStream.GetPosition() + SequenceSize);
	}

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Loads the global model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGlobalSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT GlobalSequenceSize;

	CurrentSize = 0;
	while (CurrentSize < Size)
	{
		GlobalSequenceSize = 4;
		CurrentSize += GlobalSequenceSize;

		DataStream.SetPosition(DataStream.GetPosition() + GlobalSequenceSize);
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model materials
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadMaterials(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT MaterialSize;

	CurrentSize = 0;
	std::string strs = "";
	while(CurrentSize < Size)
	{
		MaterialSize = DataStream.ReadDWord();
		CurrentSize += MaterialSize;

		DataStream.SetPosition(DataStream.GetPosition() + MaterialSize - 4);
	}

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Loads the model textures
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTextures(MODEL &Model, DATA_IN_STREAM &DataStream, INT Size)
{
	INT CurrentSize;
	INT TextureSize;
	MODEL_TEXTURE* Texture;

	TextureSize = 268;

	CurrentSize = 0;
	while (CurrentSize < Size)
	{
		CurrentSize += TextureSize;

		Texture = new MODEL_TEXTURE();
		if (Texture == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if (!LoadTexture((*Texture), DataStream, TextureSize))
		{
			delete Texture;
			return FALSE;
		}

		/*
		if (!Model.AddTexture(Texture))
		{
			delete Texture;
			return FALSE;
		}
		*/

		if (Texture->Data().FileName != "")
		{
			if (!TextureManager.Load(Texture->Data().FileName)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model texture
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTexture(MODEL_TEXTURE &Texture, DATA_IN_STREAM &DataStream, INT Size)
{
	DWORD Flags;
	CHAR Buffer[MODEL_NAME_TEXTURE_SIZE + 1];

	Buffer[MODEL_NAME_TEXTURE_SIZE] = '\0';

	Texture.Data().ReplaceableId = static_cast<INT>(DataStream.ReadDWord());

	if (!DataStream.Read(Buffer, MODEL_NAME_TEXTURE_SIZE)) return FALSE;
	Texture.Data().FileName = Buffer;

	DataStream.ReadDWord();

	Flags = DataStream.ReadDWord();
	Texture.Data().WarpWidth = (Flags & 1);
	Texture.Data().WarpHeight = (Flags & 2);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model texture animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTextureAnimations(MODEL& Model ,DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT TextureAnimationSize;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		TextureAnimationSize = DataStream.ReadDWord();
		CurrentSize += TextureAnimationSize;

		DataStream.SetPosition(DataStream.GetPosition() + TextureAnimationSize);
	}

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Loads the model geosets
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeosets(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeoset(MODEL_GEOSET &Geoset, DATA_IN_STREAM& DataStream, INT Size)
{
	return TRUE;
}














//+-----------------------------------------------------------------------------
//| Registers an mdx loader
//+-----------------------------------------------------------------------------
VOID MODEL_LOADER_MDX::RegisterMdxLoader(DWORD Group, MDX_LOADER Loader)
{
	MdxLoaderMap.insert(std::make_pair(Group, Loader));
}


//+-----------------------------------------------------------------------------
//| Reverses a DWORD
//+-----------------------------------------------------------------------------
DWORD MODEL_LOADER_MDX::ReverseDWord(DWORD DWord)
{
	DWORD NewDWord;
	CHAR* Source;
	CHAR* Target;

	Source = reinterpret_cast<CHAR*>(&DWord);
	Target = reinterpret_cast<CHAR*>(&NewDWord);

	Target[0] = Source[3];
	Target[1] = Source[2];
	Target[2] = Source[1];
	Target[3] = Source[0];

	return NewDWord;
}


//+-----------------------------------------------------------------------------
//| Converts a group to a string
//+-----------------------------------------------------------------------------
std::string MODEL_LOADER_MDX::GroupToString(DWORD Group)
{
	CHAR Buffer[5];
	CHAR* Pointer;

	Pointer = reinterpret_cast<CHAR*>(&Group);

	Buffer[0] = Pointer[3];
	Buffer[1] = Pointer[2];
	Buffer[2] = Pointer[1];
	Buffer[3] = Pointer[0];
	Buffer[4] = '\0';

	return Buffer;
}
