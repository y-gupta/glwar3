#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model material layer data structure
//+-----------------------------------------------------------------------------
struct MODEL_MATERIAL_LAYER_DATA
{
	MODEL_MATERIAL_LAYER_DATA()
	{
		FilterMode = FILTER_MODE_NONE;

		AnimatedTextureId.SetStaticScalarInt(0, "TextureID");
		Alpha.SetStaticScalar(1.0f, "Alpha");

		TextureId = INVALID_INDEX;
		TextureAnimationId = INVALID_INDEX;

		Unshaded = FALSE:
		Unfogged = FALSE;
		TwoSided = FALSE;
		SphereEnvironmentMap = FALSE;
		NoDepthTest = FALSE;
		NoDepthSet = FALSE;
	}

	FILTER_MODE FilterMode;

	INTERPOLATOR AnimatedTextureId;
	INTERPOLATOR Alpha;

	INT TextureId;
	INT TextureAnimationId;

	BOOL Unshaded;
	BOOL Unfogged;
	BOOL TwoSided;
	BOOL SphereEnvironmentMap;
	BOOL NoDepthTest;
	BOOL NoDepthSet;
};


//+-----------------------------------------------------------------------------
//| Model material layer class
//+-----------------------------------------------------------------------------
class MODEL_MATERIAL_LAYER
{
public:
	CONSTRUCTOR MODEL_MATERIAL_LAYER();
	DESTRUCTOR ~MODEL_MATERIAL_LAYER();

	VOID Clear();
	INT GetSize();

	MODEL_MATERIAL_LAYER_DATA& Data();
	MODEL_MATERIAL_LAYER_DATA& NewData();

	VOID MarkAsUpdated();
	VOID UpdateDataIfNeccessary();

	INT GetRenderOrder();

	VOID UseMaterial(CONST SEQUENCE_TIME& time);

protected:
	MODEL_MATERIAL_LAYER_DATA MaterialLayerData;

	BOOL MaterialLayerChanged;
	MODEL_MATERIAL_LAYER_DATA MaterialLayerNewData;

public:
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"

