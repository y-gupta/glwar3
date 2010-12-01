//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL Model;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL::MODEL()
{

}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL::~MODEL()
{

}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_DATA& MODEL::Data()
{
	return ModelData;
}


//+-----------------------------------------------------------------------------
//| Clears the model
//+-----------------------------------------------------------------------------
VOID MODEL::Clear()
{
	
}


//+-----------------------------------------------------------------------------
//| Adds a sequence
//+-----------------------------------------------------------------------------
BOOL MODEL::AddSequence(MODEL_SEQUENCE* Sequence)
{
	if (!ModelData.SequenceContainer.Add(Sequence))
	{
		Error.SetMessage("Unable to add a new sequence!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a global sequence
//+-----------------------------------------------------------------------------
BOOL MODEL::AddGlobalSequence(MODEL_GLOBAL_SEQUENCE* GlobalSequence)
{
	std::stringstream Stream;

	if (!ModelData.GlobalSequenceContainer.Add(GlobalSequence))
	{
		Error.SetMessage("Unable to add a new global sequence!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a texture
//+-----------------------------------------------------------------------------
BOOL MODEL::AddTexture(MODEL_TEXTURE* Texture)
{
	std::stringstream Stream;

	if (!ModelData.TextureContainer.Add(Texture))
	{
		Error.SetMessage("Unable to add a new texture!");
		return FALSE;
	}

	if(Texture->Data().FileName == "")
	{
		Stream << "Replaceable ID " << Texture->Data().ReplaceableId;
	}
	else
	{
		Stream << Texture->Data().FileName;
	}

	return TRUE;
}