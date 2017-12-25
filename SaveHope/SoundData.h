#pragma once

#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#define IDR_ACCELERATOR1                101
#define IDR_BOX16                       102
#define IDR_DISTRUST                    103
#define IDR_NEWWORLD                    104
#define IDR_UNDERGROUND                 105
#define IDR_GALLERY                     106
#define IDR_EKOROSIA                    107
#define IDR_CLEAR                       108
#define IDR_MONOMI                      109
#define IDR_GET                         110
#define IDR_FAIL                        111
#define IDR_PREPARE                     112
#define IDR_MIRAI                       113
#define IDR_KIBOU                       114
#define IDR_ALTEREGO                    115
#define IDR_BEAT1                       201
#define IDR_BEAT01                      202
#define IDR_BEAT02                      203
#define IDR_BEAT03                      204
#define IDR_CANCEL                      205
#define IDR_CURSOR                      206
#define IDR_DAMAGE                      207
#define IDR_DASH                        208
#define IDR_DECIDE                      209
#define IDR_ITEM                        210
#define IDR_PRICK                       211
#define IDR_SCORE                       212
#define IDR_SELECT                      213
#define IDR_STEP                        214
#define IDR_TEXT                        215

#include <Windows.h>

struct SoundDLL
{
public:
	SoundDLL();
	~SoundDLL();

	void Load();
	void Free();

public:
	HINSTANCE hLibSound;
};

void LoadSoundDLL();
void FreeSoundDLL();

HINSTANCE GetSoundDLLHandle();

#endif // !SOUNDDATA_H