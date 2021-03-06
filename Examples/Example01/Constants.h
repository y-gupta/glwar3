#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Include.h"
#include "Definitions.h"
#include "Resource.h"


//+-----------------------------------------------------------------------------
//| Common constants
//+-----------------------------------------------------------------------------
CONST INT INVALID_INDEX			= -1;


//+-----------------------------------------------------------------------------
//| Model constants
//+-----------------------------------------------------------------------------
CONST DWORD MODEL_DEFAULT_VERSION			= 800;
CONST INT MODEL_NAME_SIZE					= 336;
CONST INT MODEL_NAME_SEQUENCE_SIZE			= 80;
CONST INT MODEL_NAME_TEXTURE_SIZE			= 256;

//+-----------------------------------------------------------------------------
//| Math constants
//+-----------------------------------------------------------------------------
CONST FLOAT PI		= 3.141592654f;


//+-----------------------------------------------------------------------------
//| Filter mode enumeration
//+-----------------------------------------------------------------------------
enum FILTER_MODE
{
	FILTER_MODE_NONE,
	FILTER_MODE_TRANSPARENT,
	FILTER_MODE_BLEND,
	FILTER_MODE_ADDITIVE,
	FILTER_MODE_ADD_ALPHA,
	FILTER_MODE_MODULATE,
};