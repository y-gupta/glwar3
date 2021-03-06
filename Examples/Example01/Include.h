#pragma once

//+-----------------------------------------------------------------------------
//| Prevents stupid warning messages
//+-----------------------------------------------------------------------------
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4005)

//+-----------------------------------------------------------------------------
//| Windows included files
//+-----------------------------------------------------------------------------
#include <windows.h>
#include <commctrl.h>
#include <cctype>
#include <cmath>
#include <ctime>


//+-----------------------------------------------------------------------------
//| OpenGL ES included files
//+-----------------------------------------------------------------------------
#include "esUtil.h"


//+-----------------------------------------------------------------------------
//| STL included files
//+-----------------------------------------------------------------------------
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <list>
#include <map>
#include <set>


//+-----------------------------------------------------------------------------
//| Removes stupid macros
//+-----------------------------------------------------------------------------
#undef min
#undef max