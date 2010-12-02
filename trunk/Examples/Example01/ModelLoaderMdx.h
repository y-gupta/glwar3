#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelLoader.h"


//+-----------------------------------------------------------------------------
//| Function pointer types
//+-----------------------------------------------------------------------------
typedef BOOL (*MDX_LOADER)(MODEL&, DATA_IN_STREAM&, INT);


//+-----------------------------------------------------------------------------
//| Model loader mdx class
//+-----------------------------------------------------------------------------
class MODEL_LOADER_MDX : public MODEL_LOADER
{
public:
	CONSTRUCTOR MODEL_LOADER_MDX();
	DESTRUCTOR ~MODEL_LOADER_MDX();

	virtual BOOL Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer);
	virtual BOOL Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer);

protected:
	VOID RegisterMdxLoader(DWORD Group, MDX_LOADER Loader);

	static DWORD ReverseDWord(DWORD DWord);
	static std::string GroupToString(DWORD Group);

	static BOOL LoadVersion(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadModel(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadGlobalSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadTextures(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadTexture(MODEL_TEXTURE& Texture, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadMaterials(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadTextureAnimations(MODEL& Model ,DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadGeosets(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
	static BOOL LoadGeoset(MODEL_GEOSET &Geoset, DATA_IN_STREAM& DataStream, INT Size);

	static BOOL ExpectTag(DATA_IN_STREAM& DataStream, DWORD Tag);

	std::map<DWORD, MDX_LOADER> MdxLoaderMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_LOADER_MDX ModelLoaderMdx;
