#pragma once

#ifndef MISCDATA_H
#define MISCDATA_H

#define IDR_LOGO0		100
#define IDR_LOGO1		101
#define IDR_LOGO2		102
#define IDR_LOGO4		103
#define IDR_MENU0		200
#define IDR_MENU1		201
#define IDR_MENU2		202
#define IDR_MENU4		203
#define IDR_MAP			300
#define IDR_ITEM0		400
#define IDR_ITEM1		401
#define IDR_ITEM2		402
#define IDR_ITEM3		403
#define IDR_ITEM4		404
#define IDR_SHADOW		900

#include <Windows.h>

struct MiscDLL
{
public:
	MiscDLL();
	~MiscDLL();

	void Load();
	void Free();

public:
	HINSTANCE hLibMisc;
};

void LoadMiscDLL();
void FreeMiscDLL();

HINSTANCE GetMiscDLLHandle();

#endif // !MISCDATA_H
