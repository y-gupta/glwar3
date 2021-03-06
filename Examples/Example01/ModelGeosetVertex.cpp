//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelGeosetVertex.h"

//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET_VERTEX::MODEL_GEOSET_VERTEX()
{
	Position = VECTOR3(0.0f, 0.0f, 0.0f);
	Normal = VECTOR3(0.0f, 0.0f, 0.0f);
	TexturePosition = VECTOR2(0.0f, 0.0f);

	VertexGroup = INVALID_INDEX;

	FaceNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET_VERTEX::~MODEL_GEOSET_VERTEX()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the geoset vertex
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET_VERTEX::Clear()
{
	//Empty
}
