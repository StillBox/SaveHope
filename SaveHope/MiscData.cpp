#include "MiscData.h"
#include <assert.h>

MiscDLL::MiscDLL()
{
	hLibMisc = NULL;
}

MiscDLL::~MiscDLL()
{
	Free();
	hLibMisc = NULL;
}

void MiscDLL::Load()
{
	hLibMisc = LoadLibrary(L"MiscData.dll");
}

void MiscDLL::Free()
{
	FreeLibrary(hLibMisc);
}

MiscDLL *g_pMiscDLL = new MiscDLL();

MiscDLL &GetMiscDLL()
{
	assert(g_pMiscDLL != NULL);
	return *g_pMiscDLL;
}

void LoadMiscDLL()
{
	g_pMiscDLL->Load();
}

void FreeMiscDLL()
{
	g_pMiscDLL->Free();
}

HINSTANCE GetMiscDLLHandle()
{
	return g_pMiscDLL->hLibMisc;
}