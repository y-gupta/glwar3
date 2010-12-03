//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelGeoset.h"


GLuint programObject;
GLuint positionSlot;

//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET::MODEL_GEOSET()
{
	programObject = esLoadProgramFromFile("Simple.vert", "Simple.frag");
	positionSlot = glGetAttribLocation(programObject, "a_position");
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
	INT pos;

	FLOAT vertices[32 * 3];
	SHORT indices[60];

	pos = 0;
	for (INT i = 0; i < GeosetData.VertexContainer.GetTotalSize(); i++)
	{
		if (GeosetData.VertexContainer.ValidIndex(i))
		{
			vertices[pos] = GeosetData.VertexContainer[i]->Position.x/255.f;
			pos++;

			vertices[pos] = GeosetData.VertexContainer[i]->Position.y/255.f;
			pos++;

			vertices[pos] = GeosetData.VertexContainer[i]->Position.z/255.f;
			pos++;
		}
	}

	pos = 0;
	for(INT i = 0; i < GeosetData.FaceContainer.GetTotalSize(); i++)
	{
		if(GeosetData.FaceContainer.ValidIndex(i))
		{
			indices[pos] = GeosetData.FaceContainer[i]->Index1;
			pos++;

			indices[pos] = GeosetData.FaceContainer[i]->Index2;
			pos++;

			indices[pos] = GeosetData.FaceContainer[i]->Index3;
			pos++;
		}
	}

	
	glUseProgram(programObject);

	glEnableVertexAttribArray(positionSlot);
	glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(FLOAT) * 3, vertices);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, indices);
	glDisableVertexAttribArray(positionSlot);
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
