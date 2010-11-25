#pragma once

//+-----------------------------------------------------------------------------
//| Memory macros
//+-----------------------------------------------------------------------------
#define SAFE_DELETE(P)			if(P) { delete (P);			(P) = NULL; }
#define SAFE_ARRAY_DELETE(P)	if(P) { delete[] (P);		(P) = NULL; }
#define SAFE_RELEASE(P)			if(P) { (P)->Release();		(P) = NULL; }
#define SAFE_DESTROY(P)			if(P) { ::DestroyWindow(P);	(P) = NULL; }


//+-----------------------------------------------------------------------------
//| Class macros
//+-----------------------------------------------------------------------------
#define CONSTRUCTOR
#define DESTRUCTOR virtual