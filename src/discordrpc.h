#pragma once
#include <libloaderapi.h>
#include <string>

#include "stdafx.h"
typedef double DRESULT;
typedef double DINT;
typedef double DBOOL;
typedef const char* DSTRING;
#define DTRUE 1.0
#define DFALSE 0.0

namespace discord
{
	HMODULE DLL = NULL;

	typedef DRESULT(*initialize_DLL_ptr)(DINT);
	typedef DRESULT(*set_Status_ptr)(DSTRING, DSTRING, DSTRING, DSTRING);
	typedef DRESULT(*exit_DLL_ptr)();

	initialize_DLL_ptr initialize_DLL = NULL;
	set_Status_ptr set_Status = NULL;
	exit_DLL_ptr exit_DLL = NULL;

	HRESULT loadDLL(std::string filename);
	
}
