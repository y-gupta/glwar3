#pragma once


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelMaterialLayer.h"


//+-----------------------------------------------------------------------------
//| Model material data structure
//+-----------------------------------------------------------------------------
struct MODEL_MATERIAL_DATA
{
	MODEL_MATERIAL_DATA()
	{
		PriorityPlane = 0;

		ConstantColor = FALSE;
		SortPrimitivesFarZ = FALSE;
		FullResolution = FALSE;

		InternalMaterialId = INVALID_INDEX;
	}

	INT PriorityPlane;

	BOOL ConstantColor;
	BOOL SortPrimitivesFarZ;
	BOOL FullResolution;

	SIMPLE_CONTAINER<MODEL_MATERIAL_LAYER*> LayerContainer;

	INT InternalMaterialId;
};


//+-----------------------------------------------------------------------------
//| Model material class
//+-----------------------------------------------------------------------------
class MODEL_MATERIAL
{
public:
	CONSTRUCTOR MODEL_MATERIAL();
	DESTRUCTOR ~MODEL_MATERIAL();

	VOID Clear();
	INT GetSize();

	MODEL_MATERIAL_DATA& Data();

	INT GetRenderOrder();

	BOOL AddLayer(MODEL_MATERIAL_LAYER* Layer);

protected:
	MODEL_MATERIAL_DATA MaterialData;
};