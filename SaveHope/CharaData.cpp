#include "CharaData.h"
#include <assert.h>

CharaDLL::CharaDLL()
{
	hLibChara		= NULL;
	GetCharaID		= NULL;
	GetCharaHeight	= NULL;
	GetCharaWeight	= NULL;
	GetCharaBust	= NULL;
	GetCharaName	= NULL;
	GetCharaKata	= NULL;
	GetCharaTitle	= NULL;
	GetCharaBirth	= NULL;
	GetCharaBlood	= NULL;
	GetCharaStage	= NULL;
	GetCharaDegree	= NULL;
}

CharaDLL::~CharaDLL()
{
	Free();
	hLibChara		= NULL;
	GetCharaID		= NULL;
	GetCharaHeight	= NULL;
	GetCharaWeight	= NULL;
	GetCharaBust	= NULL;
	GetCharaName	= NULL;
	GetCharaKata	= NULL;
	GetCharaTitle	= NULL;
	GetCharaBirth	= NULL;
	GetCharaBlood	= NULL;
	GetCharaStage	= NULL;
	GetCharaDegree	= NULL;
}

void CharaDLL::Load()
{
	hLibChara		= LoadLibrary(L"CharaData.dll");
	GetCharaID		= (GETCHARADATA)GetProcAddress(hLibChara, "GetCharaID");
	GetCharaHeight	= (GETCHARADATA)GetProcAddress(hLibChara, "GetCharaHeight");
	GetCharaWeight	= (GETCHARADATA)GetProcAddress(hLibChara, "GetCharaWeight");
	GetCharaBust	= (GETCHARADATA)GetProcAddress(hLibChara, "GetCharaBust");
	GetCharaName	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaName");
	GetCharaKata	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaKata");
	GetCharaTitle	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaTitle");
	GetCharaBirth	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaBirth");
	GetCharaBlood	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaBlood");
	GetCharaStage	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaStage");
	GetCharaDegree	= (GETCHARAINFO)GetProcAddress(hLibChara, "GetCharaDegree");
}

void CharaDLL::Free()
{
	FreeLibrary(hLibChara);
}

CharaDLL *g_pCharaDll = new CharaDLL();

CharaDLL &GetCharaDLL()
{
	assert(g_pCharaDll != NULL);
	return *g_pCharaDll;
}

void LoadCharaDLL()
{
	g_pCharaDll->Load();
}

void FreeCharaDLL()
{
	g_pCharaDll->Free();
}

HINSTANCE GetCharaDLLHandle()
{
	return g_pCharaDll->hLibChara;
}

int GetCharaID(int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaID(iChara, iGroup);
}

int GetCharaHeight(int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaHeight(iChara, iGroup);
}

int GetCharaWeight(int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaWeight(iChara, iGroup);
}

int GetCharaBust(int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaBust(iChara, iGroup);
}

bool GetCharaName(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaName(szDest, iChara, iGroup);
}

bool GetCharaKata(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaKata(szDest, iChara, iGroup);
}

bool GetCharaTitle(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaTitle(szDest, iChara, iGroup);
}

bool GetCharaBirth(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaBirth(szDest, iChara, iGroup);
}

bool GetCharaBlood(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaBlood(szDest, iChara, iGroup);
}

bool GetCharaStage(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaStage(szDest, iChara, iGroup);
}

bool GetCharaDegree(LPWSTR szDest, int iChara, int iGroup)
{
	assert(g_pCharaDll != NULL);
	return g_pCharaDll->GetCharaDegree(szDest, iChara, iGroup);
}
