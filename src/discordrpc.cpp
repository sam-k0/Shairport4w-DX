#include "discordrpc.h"

HRESULT discord::loadDLL(std::string filename)
{
	if (discord::DLL)return TRUE;

	discord::DLL = LoadLibraryA(filename.c_str()); // load lib

	if (!discord::DLL) // exit
	{
		ATLTRACE(_T("Could not load Discord RPC"));
		return FALSE;
	}

	// try allocate ptrs
	discord::initialize_DLL = (discord::initialize_DLL_ptr)GetProcAddress(discord::DLL, "gmrpc_init");

	discord::set_Status = (discord::set_Status_ptr)GetProcAddress(discord::DLL, "gmrpc_setPresence");

	discord::exit_DLL = (discord::exit_DLL_ptr)GetProcAddress(discord::DLL, "gmrpc_exit");

	// TODO: Check for errors after loading, aka null
	using namespace discord;
	if (initialize_DLL && set_Status && exit_DLL)
	{
		return TRUE;
	}

	ATLTRACE(_T("Discord RPC Dll was loaded, but one or more functions could not be referenced."));
	return FALSE;
}