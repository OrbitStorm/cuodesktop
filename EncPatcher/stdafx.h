// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning(disable: 4018)
#pragma warning(disable: 4309)
#pragma warning(disable: 4311)
#pragma warning(disable: 4312)
#pragma warning(disable: 4800)
#pragma warning(disable: 4530)
#pragma pack(1)
#pragma comment(lib, "ws2_32.lib")

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <winsock2.h>
#include <time.h>
#include <zmouse.h>

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
using std::vector;
using std::map;
using std::string;
using std::ifstream;

// TODO: reference additional headers your program requires here
