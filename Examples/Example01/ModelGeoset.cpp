//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelGeoset.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET::MODEL_GEOSET()
{

}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET::~MODEL_GEOSET()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the geoset
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::Clear()
{

}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_GEOSET_DATA& MODEL_GEOSET::Data()
{
	return GeosetData;
}


//+-----------------------------------------------------------------------------
//| Returns the render order
//+-----------------------------------------------------------------------------
INT MODEL_GEOSET::GetRenderOrder()
{
	return 1;
}

//+-----------------------------------------------------------------------------
//| Renders the geoset
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::Render(CONST SEQUENCE_TIME& time, BOOL Animated)
{
	MATRIX4 Matrix;

	Matrix.Identity();
	Graphics.SetWorldMatrix(Matrix);
}

//+-----------------------------------------------------------------------------
//| Adds a new vertex
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddVertex(MODEL_GEOSET_VERTEX* Vertex)
{
	if(!GeosetData.VertexContainer.Add(Vertex))
	{
		Error.SetMessage("Unable to add a new vertex!");
		return FALSE;
	}

	MeshBuildt = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new face
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddFace(MODEL_GEOSET_FACE* Face)
{
	if (!GeosetData.FaceContainer.Add(Face))
	{
		Error.SetMessage("Unable to add a new face!");
		return FALSE;
	}

	MeshBuildt = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new group
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddGroup(MODEL_GEOSET_GROUP* Group)
{
	if(!GeosetData.GroupContainer.Add(Group))
	{
		Error.SetMessage("Unable to add a new group!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new extent
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddExtent(EXTENT* Extent)
{
	if(!GeosetData.ExtentContainer.Add(Extent))
	{
		Error.SetMessage("Unable to add a new extent!");
		return FALSE;
	}

	return TRUE;
}
