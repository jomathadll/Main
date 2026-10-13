// stdafx.h : include file for standard system include files,
#pragma once

//warining
#pragma warning( disable : 4067 ) 
#pragma warning( disable : 4786 ) 
#pragma warning( disable : 4800 ) 
#pragma warning( disable : 4996 ) 
#pragma warning( disable : 4244 )
#pragma warning( disable : 4237 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 4503 ) 
#pragma warning( disable : 4267 ) 
#pragma warning( disable : 4091 ) 
#pragma warning( disable : 4819 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4702 )
#pragma warning( disable : 4838 )
#pragma warning( disable : 5208 )
#pragma warning( disable : 4477 )
#pragma warning( disable : 4789 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4715 )
//#pragma warning( disable : 4482 )
//#pragma warning( disable : 4700 )
//#pragma warning( disable : 4748 )
//#pragma warning( disable : 4786 )
#pragma warning( disable : 28159 )
#pragma warning( disable : 26812 )

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN	
	
#ifndef _USE_32BIT_TIME_T
	//#define _USE_32BIT_TIME_T
#endif //_USE_32BIT_TIME_T

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#pragma warning( push, 3 )

#ifndef CLIENT_VERSION
#define CLIENT_VERSION 502
#endif

#if CLIENT_VERSION == 502
#define LUA_LOAD_PATH "Data\\Custom\\Lua"
#define FILE_EXTENSION "ogocx"
#endif

#if CLIENT_VERSION == 1001
#define LUA_LOAD_PATH "Data\\Custom\\Lua"
#define FILE_EXTENSION "vox"
#define OZJ_EXT "OZJ"
#define OZT_EXT "OZT"
#define BMD_EXT "bmd"
//#define SELECT_SERVER_S3
//#define SELECT_CHARACTER_S3
#endif

#ifndef LUA_LOAD_PATH
#define LUA_LOAD_PATH "Data\\Custom\\Lua"
#endif

#ifndef FILE_EXTENSION
#define FILE_EXTENSION "ogocx"
#endif

#ifndef OZJ_EXT
#define OZJ_EXT "OZJ"
#endif

#ifndef OZT_EXT
#define OZT_EXT "OZT"
#endif

#ifndef BMD_EXT
#define BMD_EXT "bmd"
#endif

#include <windows.h>

//windows
#include <WinSock2.h>
#include <mmsystem.h>
#include <shellapi.h>

//c runtime
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <mbstring.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <conio.h>
#include <array>

#include <string>
#include <list>
#include <map>
#include <deque>
#include <algorithm>
#include <vector>
#include <queue>

#pragma warning( pop )

//opengl
#include <gl/glew.h>
#include <gl/gl.h>

//patch
//winmain
//#include <VMProtectSDK.h>
#include "Winmain.h"
#include "Defined_Global.h"

//client
#include "_define.h"
#include "_enum.h"
#include "_types.h"
#include "_struct.h"	
#include "w_WindowMessageHandler.h"
#include "_GlobalFunctions.h"
#include "_TextureIndex.h"
#include "UIDefaultBase.h"
#include "NewUICommon.h"
#include "./Math/ZzzMathLib.h"
#include "ZzzOpenglUtil.h"
#include "Protect.h"
#include "ItemManager.h"
#include "CreateFont.h"
#include <unordered_map>

//#define ENCSTR(x) (char*)VMProtectDecryptStringA(x)
