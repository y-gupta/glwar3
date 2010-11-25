#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
//#include "Mpq.h"
#include "Buffer.h"

//+-----------------------------------------------------------------------------
//| File loader class
//+-----------------------------------------------------------------------------
class FILE_LOADER
{
public:
	CONSTRUCTOR FILE_LOADER();
	DESTRUCTOR ~FILE_LOADER();

	BOOL Load(CONST std::string& FileName, BUFFER& Buffer);

	BOOL LoadFromFile(CONST std::string& FileName, BUFFER& Buffer);
	
	BOOL SaveToFile(CONST std::string& FileName, BUFFER& Buffer);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern FILE_LOADER FileLoader;
