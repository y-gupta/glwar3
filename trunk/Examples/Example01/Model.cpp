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
//| Renders the model
//+-----------------------------------------------------------------------------
VOID MODEL::Render(INT TimeDifference)
{
	std::list<INT> RenderOrderList1;
	std::list<INT> RenderOrderList2;
	std::list<INT> RenderOrderList3;
	std::list<INT> RenderOrderList4;
	std::list<INT>::iterator j;

	for (INT i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if (ModelData.GeosetContainer.ValidIndex(i))
		{
			switch (ModelData.GeosetContainer[i]->GetRenderOrder())
			{
			case 1:
				{
					RenderOrderList1.push_back(i);
					break;
				}

			case 2:
				{
					RenderOrderList2.push_back(i);
					break;
				}

			case 3:
				{
					RenderOrderList3.push_back(i);
					break;
				}

			case 4:
				{
					RenderOrderList4.push_back(i);
					break;
				}
			}
		}
	}

	/*
	GLuint programObject;
	GLuint positionSlot;


	FLOAT vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	programObject = esLoadProgramFromFile("Simple.vert", "Simple.frag");
	glUseProgram(programObject);

	positionSlot = glGetAttribLocation(programObject, "a_position");

	const GLshort indices1[] = {0, 1, 2};
	glEnableVertexAttribArray(positionSlot);
	glVertexAttribPointer(positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(FLOAT) * 3, vertices);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices1);
	glDisableVertexAttribArray(positionSlot);

	return;
	*/

	j = RenderOrderList1.begin();
	while (j != RenderOrderList1.end())
	{
		ModelData.GeosetContainer[*j]->Render(SEQUENCE_TIME(), FALSE);
		j++;
	}
}


//+-----------------------------------------------------------------------------
//| Adds a geoset
//+-----------------------------------------------------------------------------
BOOL MODEL::AddGeoset(MODEL_GEOSET *Geoset, BOOL Imported)
{
	std::stringstream Stream;

	if(!ModelData.GeosetContainer.Add(Geoset))
	{
		Error.SetMessage("Unable to add a new geoset!");
		return FALSE;
	}

	return TRUE;

}