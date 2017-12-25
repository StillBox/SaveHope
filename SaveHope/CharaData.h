#pragma once

#ifndef CHARADATA_H
#define CHARADATA_H

#define TOTALCHARACOUNT		27
#define CHARACOUNT			21

#define IDC_HINATA			200
#define IDC_NAEGI			100
#define IDC_SAIHARA			300
#define IDC_ENOSHIMA		199
#define IDC_NANAMI			201
#define IDC_KOMAEDA			202
#define IDC_SAGISHI			203
#define IDC_TANAKA			204
#define IDC_SOUDA			205
#define IDC_HANAMURA		206
#define IDC_NIDAI			207
#define IDC_KUZURYU			208
#define IDC_OWARI			209
#define IDC_SONIA			210
#define IDC_KOIZUMI			211
#define IDC_TSUMIKI			212
#define IDC_MIODA			213
#define IDC_SAIONJI			214
#define IDC_PEKOYAMA		215
#define IDC_OMA				301
#define IDC_MONOKUMA		900
#define IDC_USAMI			901
#define IDC_MONOMI			902
#define IDC_ALTEREGO		903
#define IDC_HAKO			942
#define IDC_HINATASUPER		998
#define IDC_KAMUKURA		999

#include <Windows.h>

typedef int(*GETCHARADATA)(int iChara, int iGroup);
typedef bool(*GETCHARAINFO)(LPWSTR szDest, int iChara, int iGroup);

struct CharaDLL
{
public:
	CharaDLL();
	~CharaDLL();

	void Load();
	void Free();

public:
	HINSTANCE hLibChara;
	GETCHARADATA GetCharaID;
	GETCHARADATA GetCharaHeight;
	GETCHARADATA GetCharaWeight;
	GETCHARADATA GetCharaBust;
	GETCHARAINFO GetCharaName;
	GETCHARAINFO GetCharaKata;
	GETCHARAINFO GetCharaTitle;
	GETCHARAINFO GetCharaBirth;
	GETCHARAINFO GetCharaBlood;
	GETCHARAINFO GetCharaStage;
	GETCHARAINFO GetCharaDegree;
};

void LoadCharaDLL();
void FreeCharaDLL();

HINSTANCE GetCharaDLLHandle();
int GetCharaID(int iChara, int iGroup);
int GetCharaHeight(int iChara, int iGroup);
int GetCharaWeight(int iChara, int iGroup);
int GetCharaBust(int iChara, int iGroup);
bool GetCharaName(LPWSTR szDest, int iChara, int iGroup);
bool GetCharaKata(LPWSTR szDest, int iChara, int iGroup);
bool GetCharaTitle(LPWSTR szDest, int iChara, int iGroup);
bool GetCharaBirth(LPWSTR szDest, int iChara, int iGroup);
bool GetCharaBlood(LPWSTR szDest, int iChara, int iGroup);
bool GetCharaStage(LPWSTR szDest, int iChara, int iGroup);
bool GetCharaDegree(LPWSTR szDest, int iChara, int iGroup);

#endif // !CHARADATA_H