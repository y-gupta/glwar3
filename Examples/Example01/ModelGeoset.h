#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelGeosetVertex.h"
#include "ModelGeosetFace.h"
#include "ModelGeosetGroup.h"
#include "ModelBase.h"
//#include "Mesh.h"


//+-----------------------------------------------------------------------------
//| Model geoset data struct
//+-----------------------------------------------------------------------------
struct MODEL_GEOSET_DATA
{
	MODEL_GEOSET_DATA()
	{
		MaterialId = INVALID_INDEX;
		SelectionGroup = 0;

		Unselectable = FALSE;

		InternalGeosetId = INVALID_INDEX;
	}

	INT MaterialId;
	INT SelectionGroup;

	BOOL Unselectable;

	SIMPLE_CONTAINER<MODEL_GEOSET_VERTEX*> VertexContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET_FACE*> FaceContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET_GROUP*> GroupContainer;
	SIMPLE_CONTAINER<EXTENT*> ExtentContainer;

	EXTENT Extent;

	INT InternalGeosetId;
};


//+-----------------------------------------------------------------------------
//| Model geoset class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET
{
public:
	CONSTRUCTOR MODEL_GEOSET();
	DESTRUCTOR ~MODEL_GEOSET();

	VOID Clear();
	INT GetSize();

	MODEL_GEOSET_DATA& Data();

	INT GetRenderOrder();

	VOID Render(CONST SEQUENCE_TIME& time, BOOL Animated);

	BOOL BuildMesh();

	BOOL AddVertex(MODEL_GEOSET_VERTEX* Vertex);
	BOOL AddFace(MODEL_GEOSET_FACE* Face);
	BOOL AddGroup(MODEL_GEOSET_GROUP* Group);
	BOOL AddExtent(EXTENT* Extent);

protected:
	MODEL_GEOSET_DATA GeosetData;

	BOOL MeshBuildt;
	FLOAT* vertices;
	INT* indices;

public:

};