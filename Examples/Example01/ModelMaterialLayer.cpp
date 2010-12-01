//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelMaterialLayer.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER::MODEL_MATERIAL_LAYER()
{
	
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER::~MODEL_MATERIAL_LAYER()
{

}


//+-----------------------------------------------------------------------------
//| Clears the material layer
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER::Clear()
{
	MaterialLayerData = MODEL_MATERIAL_LAYER_DATA();
	MaterialLayerNewData = MODEL_MATERIAL_LAYER_DATA;
}


//+-----------------------------------------------------------------------------
//| Returns the mdx sizeof the material layer
//+-----------------------------------------------------------------------------
INT MODEL_MATERIAL_LAYER::GetSize()
{
	return 28 + MaterialLayerData.AnimatedTextureId.GetSize() + MaterialLayerData.Alpha.GetSize();
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the new data
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER_DATA& MODEL_MATERIAL_LAYER::NewData()
{
	return MaterialLayerNewData;
}


//+-----------------------------------------------------------------------------
//| Marks the layer as updated
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL_LAYER::MarkAsUpdated()
{
	MaterialLayerChanged = TRUE:
}


//+-----------------------------------------------------------------------------
//| Updates the material layer if neccessary
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL_LAYER::UpdateDataIfNeccessary()
{
	if (MaterialLayerChanged)
	{
		MaterialLayerData = MaterialLayerNewData;
		MaterialLayerNewData = MODEL_MATERIAL_LAYER_DATA();

		MaterialLayerChanged = FALSE:
	}
}

//+-----------------------------------------------------------------------------
//| Returns the render order
//+-----------------------------------------------------------------------------
INT MODEL_MATERIAL_LAYER::GetRenderOrder()
{
	switch (MaterialLayerData.FilterMode)
	{
	case FILTER_MODE_NONE:
		return 1

	case FILTER_MODE_TRANSPARENT:
		return 2;

	case FILTER_MODE_BLEND:
		return 3;

	case FILTER_MODE_ADDITIVE:
	case FILTER_MODE_ADD_ALPHA:
	case FILTER_MODE_MODULATE:
		return 4;
	}

	return 1;
}