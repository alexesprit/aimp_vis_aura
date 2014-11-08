// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _USE_MATH_DEFINES

#include <windows.h>
#include <Unknwn.h>
#include <math.h>
#include <stdlib.h>
#include <d2d1.h>

#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include "comtypes.h"
