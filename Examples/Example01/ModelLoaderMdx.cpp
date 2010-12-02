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
	INT CurrentSize;
	INT GeosetSize;
	MODEL_GEOSET* Geoset;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		GeosetSize = DataStream.ReadDWord();
		CurrentSize += GeosetSize;

		Geoset = new MODEL_GEOSET();
		if(Geoset == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadGeoset((*Geoset), DataStream, GeosetSize))
		{
			delete Geoset;
			return FALSE;
		}

		/*
		if(!Model.AddGeoset(Geoset))
		{
			delete Geoset;
			return FALSE;
		}
		*/
	}
}


//+-----------------------------------------------------------------------------
//| Loads a model geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeoset(MODEL_GEOSET &Geoset, DATA_IN_STREAM& DataStream, INT Size)
{
	INT j;
	INT IndexSize;
	INT FaceType;
	DWORD NrOfVertices;
	DWORD NrOfNormals;
	DWORD NrOfVertexGroups;
	DWORD NrOfTexturePositions;
	DWORD NrOfIndexes;
	DWORD NrOfFaces;
	DWORD NrOfMTGC;
	DWORD NrOfMatrices;
	DWORD NrOfAnimations;
	DWORD NrOfFaceGroups;
	DWORD NrOfFaceTypeGroups;
	DWORD NrOfTextureVertexGroups;
	VECTOR3 TempVector3;
	VECTOR2 TempVector2;
	std::stringstream Stream;
	std::vector<VECTOR3> PositionList;
	std::vector<VECTOR3> NormalList;
	std::vector<VECTOR2> TexturePositionList;
	std::vector<INT> GroupList;
	std::vector<DWORD> GroupIndexList;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	MODEL_GEOSET_GROUP_NODE* GroupNode;
	EXTENT* Extent;

	if(!ExpectTag(DataStream, 'VRTX')) return FALSE;

	NrOfVertices = DataStream.ReadDWord();

	PositionList.reserve(NrOfVertices);
	NormalList.reserve(NrOfVertices);
	GroupList.reserve(NrOfVertices);
	TexturePositionList.reserve(NrOfVertices);
	
	for (INT i = 0; i < static_cast<INT>(NrOfVertices); i++)
	{
		TempVector3.x = DataStream.ReadFloat();
		TempVector3.y = DataStream.ReadFloat();
		TempVector3.z = DataStream.ReadFloat();

		PositionList.push_back(TempVector3);
	}

	if(!ExpectTag(DataStream, 'NRMS')) return FALSE;

	NrOfNormals = DataStream.ReadDWord();
	if(NrOfNormals != NrOfVertices)
	{
		Stream << "Normal count mismatch in \"" << CurrentFileName << "\", " << NrOfNormals << " normals for " << NrOfVertices << " vertices)!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	for(INT i = 0; i < static_cast<INT>(NrOfNormals); i++)
	{
		TempVector3.x = DataStream.ReadFloat();
		TempVector3.y = DataStream.ReadFloat();
		TempVector3.z = DataStream.ReadFloat();

		NormalList.push_back(TempVector3);
	}

	if(!ExpectTag(DataStream, 'PTYP')) return FALSE;
	NrOfFaceTypeGroups = DataStream.ReadDWord();

	for (INT i = 0; i < static_cast<INT>(NrOfFaceTypeGroups); i++)
	{
		FaceType = DataStream.ReadDWord();
		if(FaceType != 4)
		{
			Stream << "Can only load triangle lists! Expected type 4, got type " << FaceType << "!";
			Error.SetMessage(Stream.str());
			return FALSE;
		}
	}

	if(!ExpectTag(DataStream, 'PCNT')) return FALSE;
	NrOfFaceGroups = DataStream.ReadDWord();

	for(INT i = 0; i < static_cast<INT>(NrOfFaceGroups); i++)
	{
		NrOfIndexes = DataStream.ReadDWord();
	}

	if(!ExpectTag(DataStream, 'PVTX')) return FALSE;

	NrOfIndexes = DataStream.ReadDWord();
	NrOfFaces = NrOfIndexes / 3;
	if (NrOfIndexes != (NrOfFaces * 3))
	{
		Error.SetMessage("The number of indexes in \"" + CurrentFileName + "\" is not a multiple of 3!");
		return FALSE;
	}

	for(INT i = 0; i < static_cast<INT>(NrOfFaces); i++)
	{
		Face = new MODEL_GEOSET_FACE();
		if(Face == NULL)
		{
			Error.SetMessage("Unable to create a new face!");
			return FALSE;
		}

		Face->Index1 = DataStream.ReadWord();
		Face->Index2 = DataStream.ReadWord();
		Face->Index3 = DataStream.ReadWord();

		if(!Geoset.AddFace(Face)) return FALSE;
	}

	if(!ExpectTag(DataStream, 'GNDX')) return FALSE;

	NrOfVertexGroups = DataStream.ReadDWord();
	if(NrOfVertexGroups != NrOfVertices)
	{
		Stream << "Vertex group count mismatch in \"" << CurrentFileName << "\", " << NrOfVertexGroups << " vertex groups for " << NrOfVertices << " vertices)!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	for(INT i = 0; i < static_cast<INT>(NrOfVertexGroups); i++)
	{
		GroupList.push_back(DataStream.ReadChar());
	}

	if(!ExpectTag(DataStream, 'MTGC')) return FALSE;

	NrOfMTGC = DataStream.ReadDWord();
	for(INT i = 0; i < static_cast<INT>(NrOfMTGC); i++)
	{
		Group = new MODEL_GEOSET_GROUP();
		if(Group == NULL)
		{
			Error.SetMessage("Unable to create a new matrix group!");
			return FALSE;
		}

		Group->MatrixListSize = static_cast<INT>(DataStream.ReadDWord());

		if(!Geoset.AddGroup(Group)) return FALSE;
	}

	if(!ExpectTag(DataStream, 'MATS')) return FALSE;

	j = -1;
	IndexSize = 0;
	NrOfMatrices = DataStream.ReadDWord();
	for(INT i = 0; i < static_cast<INT>(NrOfMatrices); i++)
	{
		if(IndexSize <= 0)
		{
			j++;
			IndexSize = Geoset.Data().GroupContainer[j]->MatrixListSize;
		}

		GroupNode = new MODEL_GEOSET_GROUP_NODE();
		if(GroupNode == NULL)
		{
			Error.SetMessage("Unable to create a new matrix group node!");
			return FALSE;
		}

		GroupNode->NodeId = DataStream.ReadDWord();
		if(!Geoset.Data().GroupContainer[j]->MatrixList.Add(GroupNode)) return FALSE;

		IndexSize--;
	}

	Geoset.Data().MaterialId = static_cast<INT>(DataStream.ReadDWord());
	Geoset.Data().SelectionGroup = DataStream.ReadDWord();
	Geoset.Data().Unselectable = (DataStream.ReadDWord() == 4);

	Geoset.Data().Extent.Radius = DataStream.ReadFloat();
	Geoset.Data().Extent.Min.x = DataStream.ReadFloat();
	Geoset.Data().Extent.Min.y = DataStream.ReadFloat();
	Geoset.Data().Extent.Min.z = DataStream.ReadFloat();
	Geoset.Data().Extent.Max.x = DataStream.ReadFloat();
	Geoset.Data().Extent.Max.y = DataStream.ReadFloat();
	Geoset.Data().Extent.Max.z = DataStream.ReadFloat();

	NrOfAnimations = DataStream.ReadDWord();
	for(INT i = 0; i < static_cast<INT>(NrOfAnimations); i++)
	{
		Extent = new EXTENT();
		if(Extent == NULL)
		{
			Error.SetMessage("Unable to create a new extent!");
			return FALSE;
		}

		Extent->Radius = DataStream.ReadFloat();
		Extent->Min.x = DataStream.ReadFloat();
		Extent->Min.y = DataStream.ReadFloat();
		Extent->Min.z = DataStream.ReadFloat();
		Extent->Max.x = DataStream.ReadFloat();
		Extent->Max.y = DataStream.ReadFloat();
		Extent->Max.z = DataStream.ReadFloat();

		if(!Geoset.AddExtent(Extent)) return FALSE;
	}

	if(!ExpectTag(DataStream, 'UVAS')) return FALSE;

	NrOfTextureVertexGroups = DataStream.ReadDWord();

	if(!ExpectTag(DataStream, 'UVBS')) return FALSE;

	NrOfTexturePositions = DataStream.ReadDWord();
	if(NrOfTexturePositions != NrOfVertices)
	{
		Stream << "Texture position count mismatch in \"" << CurrentFileName << "\", " << NrOfTexturePositions << " texture positions for " << NrOfVertices << " vertices)!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	for(INT i = 0; i < static_cast<INT>(NrOfTexturePositions); i++)
	{
		TempVector2.x = DataStream.ReadFloat();
		TempVector2.y = DataStream.ReadFloat();

		TexturePositionList.push_back(TempVector2);
	}

	for(INT i = 0; i < static_cast<INT>(NrOfVertices); i++)
	{
		Vertex = new MODEL_GEOSET_VERTEX();
		if(Vertex == NULL)
		{
			Error.SetMessage("Unable to create a new vertex!");
			return FALSE;
		}

		Vertex->Position = PositionList[i];
		Vertex->Normal = NormalList[i];
		Vertex->TexturePosition = TexturePositionList[i];
		Vertex->VertexGroup = GroupList[i];

		if(!Geoset.AddVertex(Vertex)) return FALSE;
	}

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


//+-----------------------------------------------------------------------------
//| Expects a certain tag
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::ExpectTag(DATA_IN_STREAM& DataStream, DWORD Tag)
{
	DWORD ReceivedTag;

	ReceivedTag = ReverseDWord(DataStream.ReadDWord());
	if(ReceivedTag == Tag) return TRUE;

	Error.SetMessage("Expected a \"" + GroupToString(Tag) + "\" tag, got a \"" + GroupToString(ReceivedTag) + "\" tag!");
	return FALSE;
}
