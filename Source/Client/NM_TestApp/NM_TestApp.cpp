#include "stdafx.h"
#include "Utils.h"

#include <lazy_importer.hpp>
#include <xorstr.hpp>

static std::string	s_szModuleName	= xorstr("NoMercy.dll").crypt_get();;
static std::string	s_szLogFile		= xorstr("NoMercyApp.log").crypt_get();;

typedef bool(__cdecl* TInitializeFunction)(const char* c_szLicenseCode);

// -------------------

bool InitNoMercy(const char* c_szLicenseCode)
{
	NNoMercyUtils::FileLog(s_szLogFile, xorstr("Initilization started!").crypt_get());

	LI_FIND(CreateDirectoryA)(xorstr("NoMercy").crypt_get(), NULL);

	if (NNoMercyUtils::IsFileExist(s_szModuleName.c_str()) == false)
	{
		NNoMercyUtils::FileLog(s_szLogFile, xorstr("Error! DLL file not found!").crypt_get());
		return false;
	}
	NNoMercyUtils::FileLog(s_szLogFile, xorstr("DLL module succesfully found!").crypt_get());

	auto hModule = LI_FIND(LoadLibraryA)(s_szModuleName.c_str());
	if (!hModule)
	{
		NNoMercyUtils::FileLogf(s_szLogFile, xorstr("Error! DLL file can not load! Error code: %u").crypt_get(), LI_FIND(GetLastError)());
		return false;
	}
	NNoMercyUtils::FileLogf(s_szLogFile, xorstr("DLL file succesfully loaded!").crypt_get());

	auto InitializeFunction = reinterpret_cast<TInitializeFunction>(LI_FIND(GetProcAddress)(hModule, xorstr("Initialize").crypt_get()));
	if (!InitializeFunction)
	{
		NNoMercyUtils::FileLogf(s_szLogFile, xorstr("Error! Initialize function not found! Error code: %u").crypt_get(), LI_FIND(GetLastError)());
		return false;
	}
	NNoMercyUtils::FileLog(s_szLogFile, xorstr("Initialize function found!").crypt_get());

	if (!InitializeFunction(c_szLicenseCode))
	{
		NNoMercyUtils::FileLogf(s_szLogFile, xorstr("Error! Initilization call fail! Error code: %u").crypt_get(), LI_FIND(GetLastError)());
		return false;
	}
	NNoMercyUtils::FileLog(s_szLogFile, xorstr("Initializion completed!").crypt_get());
	return true;
}

// ---

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AllocConsole();
	freopen("CONOUT$", "a", stdout);
	freopen("CONIN$", "r", stdin);

//	if (InitNoMercy(xorstr("ABCDEF123490").crypt_get()) == false) // test license
	if (InitNoMercy(xorstr("123").crypt_get()) == false) // undefined-normal license
	{
#ifdef _DEBUG
		printf(xorstr("NoMercy can not loaded!\n").crypt_get());
#endif
		NNoMercyUtils::FileLog(s_szLogFile, xorstr("NoMercy can not loaded").crypt_get());
		return EXIT_FAILURE;
	}

#ifdef _DEBUG
	printf(" # COMPLETED! # \n");
#endif

	while (1) LI_FIND(Sleep)(1000);

	return EXIT_SUCCESS;
}

