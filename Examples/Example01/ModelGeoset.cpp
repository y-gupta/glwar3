//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelGeoset.h"



//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET::MODEL_GEOSET()
{
	vertices = NULL;
	indices = NULL;
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
//| Builds a mesh from the vertex and face information
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::BuildMesh()
{
	INT NrOfVertices;
	INT NrOfIndices;
	INT Pos;

	/*
	static FLOAT ve[] = {
		-0.5f, -0.866f, 0.0f,	
		0.5f, -0.866f, 0.0f,	
		0, 1, 0.0f, 0.0f,
	};

	static INT in[] = {0, 1, 2};

	if (MeshBuildt) return TRUE;

	SAFE_ARRAY_DELETE(vertices);
	SAFE_ARRAY_DELETE(indices);

	vertices =  new FLOAT[9];
	indices = new INT[3];

	for (int i=0; i<9; i++)
	{
		vertices[i] = ve[i];
	}

	for (int i=0; i<3; i++)
	{
		indices[i] = in[i];
	}

	MeshBuildt = true;

	return TRUE;
	*/

	if (MeshBuildt) return TRUE;

	NrOfVertices = GeosetData.VertexContainer.GetSize();
	if(NrOfVertices <= 0) return FALSE;

	NrOfIndices = GeosetData.FaceContainer.GetSize() * 3;
	if(NrOfIndices <= 0) return FALSE;

	//SAFE_ARRAY_DELETE(vertices);
	//SAFE_ARRAY_DELETE(indices);

	//vertices =  new FLOAT[NrOfVertices];
	//indices = new INT[NrOfIndices];

	static FLOAT ve[2048];
	static FLOAT te[2048];
	static INT in[2048];

	Pos = 0;
	for (INT i = 0; i < GeosetData.VertexContainer.GetTotalSize(); i++)
	{
		if (GeosetData.VertexContainer.ValidIndex(i))
		{
			ve[Pos] = GeosetData.VertexContainer[i]->Position.x/255.f;
			Pos++;

			ve[Pos] = GeosetData.VertexContainer[i]->Position.y/255.f;
			Pos++;

			ve[Pos] = GeosetData.VertexContainer[i]->Position.z/255.f;
			Pos++;
		}
	}

	Pos = 0;
	for (INT i = 0; i < GeosetData.VertexContainer.GetTotalSize(); i++)
	{
		if (GeosetData.VertexContainer.ValidIndex(i))
		{
			te[Pos] = GeosetData.VertexContainer[i]->TexturePosition.x;
			Pos++;

			te[Pos] = GeosetData.VertexContainer[i]->TexturePosition.y;
			Pos++;
		}
	}

	Pos = 0;
	for(INT i = 0; i < GeosetData.FaceContainer.GetTotalSize(); i++)
	{
		if(GeosetData.FaceContainer.ValidIndex(i))
		{
			in[Pos] = GeosetData.FaceContainer[i]->Index1;
			Pos++;

			in[Pos] = GeosetData.FaceContainer[i]->Index2;
			Pos++;

			in[Pos] = GeosetData.FaceContainer[i]->Index3;
			Pos++;
		}
	}

	vertices = ve;
	TexturePositions = te;
	indices = in;
	MeshBuildt = TRUE;

	return TRUE;
}

//+-----------------------------------------------------------------------------
//| Renders the geoset
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::Render(CONST SEQUENCE_TIME& time, BOOL Animated)
{
	BuildMesh();
	glUseProgram(Graphics.Program());

	ESMatrix modelviewMatrix;
	ESMatrix perspectiveMatrix;
	ESMatrix mvpMatrix;

	int aspect;

	aspect = (GLfloat) 600 / (GLfloat) 800;
	esMatrixLoadIdentity(&perspectiveMatrix);
	esPerspective(&perspectiveMatrix, 60.0f, aspect, 1.0f, 20.0f);

	esMatrixLoadIdentity(&modelviewMatrix);
	esScale(&modelviewMatrix, 1.0f, 1.0f, 1.0f);
	esTranslate(&modelviewMatrix, 0.0f, 0.0f, 0.0f);

	static float angle = 0.0f;
	esRotate(&modelviewMatrix,angle, 1.0f, 0.0f, 0.0f);
	angle += 0.2f;

	esMatrixLoadIdentity(&mvpMatrix);
	esMatrixMultiply(&mvpMatrix, &modelviewMatrix, &perspectiveMatrix);
	glUniformMatrix4fv(Graphics.WVPMatrix(), 1, GL_FALSE, (GLfloat*) mvpMatrix.m);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE0);

	glActiveTexture(GL_TEXTURE0);
	glEnableVertexAttribArray(Graphics.Position());
	glEnableVertexAttribArray(Graphics.TexturePosition());
	
	glUniform1i(Graphics.Texture(), 0);

	glVertexAttribPointer(Graphics.Position(), 3, GL_FLOAT, GL_FALSE, sizeof(FLOAT) * 3, vertices);
	glVertexAttribPointer(Graphics.TexturePosition(), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(FLOAT), TexturePositions);
	glDrawElements(GL_TRIANGLES, GeosetData.FaceContainer.GetTotalSize() * 3, GL_UNSIGNED_INT, indices);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE0);
	glDisableVertexAttribArray(Graphics.TexturePosition());
	glDisableVertexAttribArray(Graphics.Texture());
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
