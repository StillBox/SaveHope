#include "SoundData.h"
#include <assert.h>

SoundDLL::SoundDLL()
{
	hLibSound = NULL;
}

SoundDLL::~SoundDLL()
{
	Free();
	hLibSound = NULL;
}

void SoundDLL::Load()
{
	hLibSound = LoadLibrary(L"SoundData.dll");
}

void SoundDLL::Free()
{
	FreeLibrary(hLibSound);
}

SoundDLL *g_pSoundDLL = new SoundDLL();

SoundDLL &GetMiscDLL()
{
	assert(g_pSoundDLL != NULL);
	return *g_pSoundDLL;
}

void LoadSoundDLL()
{
	g_pSoundDLL->Load();
}

void FreeSoundDLL()
{
	g_pSoundDLL->Free();
}

HINSTANCE GetSoundDLLHandle()
{
	return g_pSoundDLL->hLibSound;
}