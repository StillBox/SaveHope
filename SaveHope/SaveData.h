#pragma once
#ifndef SAVEDATA_H
#define SAVEDATA_H

#include <vector>
#include "DXUT.h"

#define SKILLCOUNT	5

struct CharaData
{
	int ID;
	int bAvailable;
	int iSkillLevel[SKILLCOUNT];
};

struct STScore
{
	int iScore = 0;
	int iMaxCombo = 0;
	int iPerfect = 0;
	int iGood = 0;
	int iBad = 0;
	int iMiss = 0;
};

struct SKScore
{
	int iScore = 0;
	int iMin = 0;
	int iSec = 0;
	int iDamage = 0;
	int iFragment = 0;
};

struct USScore
{
	int iScore = 0;
	int iMin = 0;
	int iSec = 0;
	int iDamage = 0;
	int iFragment = 0;
};

struct INFUSScore
{
	int iFloor = 0;
	int iMin = 0;
	int iSec = 0;
	int iDamage = 0;
	int iFragment = 0;
};

struct SaveData 
{
	int m_iStory;
	int m_iDifficulty;
	int m_iClear[4];
	int m_iUnlock[4];
	int m_iHopeFragment;
	int m_iGoldFragment;
	int m_iCharaID;
	std::vector <CharaData> m_vecChara;

	int m_iNoMiss[3][4];
	STScore m_STScore[4][10];
	SKScore m_SKScore[4][10];
	USScore m_USScore[4][10];
	INFUSScore m_InfUSScore[10];
};

struct SaveInfo
{
	bool bExist = false;
	int iYear = 1990;
	int iMonth =1;
	int iDay = 1;
	int iHour =0;
	int iMinute = 0;
	int iStory = 0;
	int iDifficulty = 0;
	int iClear[4] = { 0,0,0,0 };
	int iUnlock[4] = { 0,0,0,0 };
	int iHopeFragment = 0;
	int iGoldFragment = 0;
};

SaveData &GetSaveData();

void SaveDataUpdated();
bool IsSaveDataUpdated();

BOOL InitSaveData();
BOOL NewSaveData();
BOOL ReadSaveData(int iData);
BOOL WriteSaveData(int iData);

BOOL GetSaveInfo(int iData, SaveInfo *pInfo);

//GetSaveData

int GetCharaSkillLevel(int iCharaID, int iSkill);

void CheckSTScore(int iDiff, int iScore, int iMaxCombo, int iPerfect, int iGood, int iBad, int iMiss);
void CheckSKScore(int iDiff, int iScore, int iMin, int iSec, int iDamage, int iFragment);
void CheckUSScore(int iDiff, int iScore, int iMin, int iSec, int iDamage, int iFragment);
void CheckInfUSScore(int iFloor, int iMin, int iSec, int iDamage, int iFragment);

void UnlockChara(int ID);

#endif // !SAVEDATA_H
