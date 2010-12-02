#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_GROUP;


//+-----------------------------------------------------------------------------
//| Model geoset vertex class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET_VERTEX
{
public:
	CONSTRUCTOR MODEL_GEOSET_VERTEX();
	DESTRUCTOR ~MODEL_GEOSET_VERTEX();

	VOID Clear();

	VECTOR3 Position;
	VECTOR3 Normal;
	VECTOR2 TexturePosition;

	INT VertexGroup;

	REFERENCE<VOID*, MODEL_GEOSET_GROUP*> GroupNode;
	REFERENCE_OBJECT<VOID*, MODEL_GEOSET_VERTEX*> FaceNodes;
};