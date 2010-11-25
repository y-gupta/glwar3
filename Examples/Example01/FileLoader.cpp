//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "FileLoader.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
FILE_LOADER FileLoader;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
FILE_LOADER::FILE_LOADER()
{

}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
FILE_LOADER::~FILE_LOADER()
{

}


//+-----------------------------------------------------------------------------
//| Loads a file
//+-----------------------------------------------------------------------------
BOOL FILE_LOADER::Load(CONST std::string &FileName, BUFFER &Buffer)
{
	if (LoadFromFile(FileName, Buffer)) return TRUE;

	Error.SetMessage("Unable to load \"" + FileName + "\", file does not exist!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a file from a physical file
//+-----------------------------------------------------------------------------
BOOL FILE_LOADER::LoadFromFile(CONST std::string& FileName, BUFFER& Buffer)
{
	INT Size;
	std::ifstream File;

	File.open(FileName.c_str(), std::ios::in | std::ios::binary);
	if (File.fail()) return FALSE;

	File.seekg(0, std::ios::end);
	Size = File.tellg();
	File.seekg(0, std::ios::beg);

	if (!Buffer.Resize(Size)) return FALSE;

	File.read(Buffer.GetData(), Buffer.GetSize());

	return TRUE;
}
