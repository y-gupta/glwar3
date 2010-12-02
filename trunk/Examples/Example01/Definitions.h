#pragma once

//+-----------------------------------------------------------------------------
//| Memory macros
//+-----------------------------------------------------------------------------
#define SAFE_DELETE(P)			if(P) { delete (P);			(P) = NULL; }
#define SAFE_ARRAY_DELETE(P)	if(P) { delete[] (P);		(P) = NULL; }
#define SAFE_RELEASE(P)			if(P) { (P)->Release();		(P) = NULL; }
#define SAFE_DESTROY(P)			if(P) { ::DestroyWindow(P);	(P) = NULL; }


//+-----------------------------------------------------------------------------
//| Container clearing macro
//+-----------------------------------------------------------------------------
#define SAFE_CLEAR(C) \
	for(INT _i = 0; _i < (C).GetTotalSize(); _i++) \
{ \
	if((C).ValidIndex(_i)) \
	{ \
	delete (C)[_i]; \
	} \
} \
(C).Clear();


//+-----------------------------------------------------------------------------
//| Class macros
//+-----------------------------------------------------------------------------
#define CONSTRUCTOR
#define DESTRUCTOR virtual