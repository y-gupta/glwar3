#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"



//+-----------------------------------------------------------------------------
//| Mesh class
//+-----------------------------------------------------------------------------
class MESH
{
public:
	CONSTRUCTOR MESH();
	DESTRUCTOR ~MESH();

	BOOL Create(INT NrOfVertices, INT NrOfIndices);
	VOID Clear();

protected:
	FLOAT* vertices;
	INT* indices;
};