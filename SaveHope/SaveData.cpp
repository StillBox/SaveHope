#include "SaveData.h"
#include "CharaData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include <shlobj.h>
#include <shlwapi.h>
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "shlwapi.lib")

std::vector<int> vecCharaID = 
{ 
	200,100,300,
	201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
	301,998,999 
};

SaveData *g_pSaveData = new SaveData();
bool g_bSaveDataUpdated;

SaveData &GetSaveData()
{
	assert(g_pSaveData != NULL);
	return *g_pSaveData;
}

void SaveDataUpdated()
{
	g_bSaveDataUpdated = true;
}

bool IsSaveDataUpdated()
{
	return g_bSaveDataUpdated;
}

BOOL InitSaveData()
{
	g_pSaveData->m_iStory = 0;
	g_pSaveData->m_iDifficulty = 0;

	for (int i = 0; i < 4; i++)
	{
		g_pSaveData->m_iClear[i] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		g_pSaveData->m_iUnlock[i] = 0;
	}

	g_pSaveData->m_iHopeFragment = 0;
	g_pSaveData->m_iGoldFragment = 0;
	g_pSaveData->m_iCharaID = 200;
	for (int i = 0; i < CHARACOUNT; i++)
	{
		CharaData newChara;
		newChara.ID = vecCharaID[i];
		newChara.bAvailable = (vecCharaID[i] % 100 == 0) ? 1 : 0;
		for (int j = 0; j < SKILLCOUNT; j++)
		{
			newChara.iSkillLevel[j] = 0;
			if (IDC_HINATASUPER == newChara.ID || IDC_KAMUKURA == newChara.ID)
				newChara.iSkillLevel[j] = 18;
		}
		g_pSaveData->m_vecChara.push_back(newChara);
	}
	g_bSaveDataUpdated = false;

	ReadSaveData(0);

	return TRUE;
}

BOOL NewSaveData()
{
	g_pSaveData->m_iStory = 0;
	g_pSaveData->m_iDifficulty = 0;

	for (int i = 0; i < 4; i++)
	{
		g_pSaveData->m_iClear[i] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		g_pSaveData->m_iUnlock[i] = 0;
	}

	g_pSaveData->m_iHopeFragment = 0;
	g_pSaveData->m_iGoldFragment = 0;
	g_pSaveData->m_iCharaID = 200;
	for (int i = 0; i < CHARACOUNT; i++)
	{
		CharaData newChara;
		newChara.ID = vecCharaID[i];
		newChara.bAvailable = (vecCharaID[i] % 100 == 0) ? 1 : 0;
		for (int j = 0; j < SKILLCOUNT; j++)
		{
			newChara.iSkillLevel[j] = 0;
			if (IDC_HINATASUPER == newChara.ID || IDC_KAMUKURA == newChara.ID)
				newChara.iSkillLevel[j] = 18;
		}
		g_pSaveData->m_vecChara.push_back(newChara);
	}
	g_bSaveDataUpdated = false;

	return TRUE;
}

BOOL ReadSaveData(int iData)
{
	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	wcscat_s(szPath, L"\\SaveHope");

	WCHAR szFile[MAX_PATH] = L"";
	swprintf_s(szFile, TEXT("%s\\save%d.dat"),szPath, iData);

	std::ifstream infile;
	infile.open(szFile, std::ios::_Nocreate);

	if (!infile.is_open())
	{
		infile.close();
		return FALSE;
	}
	else
	{
		if (iData == 0)
		{
			std::string szIn, szData;
			infile >> szIn;

			int iSeed, iMul, iData;
			std::istringstream inStr;

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iSeed;
			inStr.clear();

			//NoMissRec

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 4; j++)
				{
					g_pSaveData->m_iNoMiss[i][j] = 0;
					
					szData = szIn.substr(0, 2);
					szIn = szIn.substr(2);
					inStr.str(szData);
					inStr >> iMul;
					inStr.clear();

					szData = szIn.substr(0, 4);
					szIn = szIn.substr(4);
					inStr.str(szData);
					inStr >> iData;
					inStr.clear();

					g_pSaveData->m_iNoMiss[i][j] = (10000 + iData - iSeed * iMul) % 10000;
				}

			//Score

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 10; j++)
				{
					g_pSaveData->m_STScore[i][j] = { 0,0,0,0,0,0 };
					infile >> g_pSaveData->m_STScore[i][j].iScore;
					infile >> g_pSaveData->m_STScore[i][j].iMaxCombo;
					infile >> g_pSaveData->m_STScore[i][j].iPerfect;
					infile >> g_pSaveData->m_STScore[i][j].iGood;
					infile >> g_pSaveData->m_STScore[i][j].iBad;
					infile >> g_pSaveData->m_STScore[i][j].iMiss;
				}

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 10; j++)
				{
					g_pSaveData->m_SKScore[i][j] = { 0,0,0,0,0 };
					infile >> g_pSaveData->m_SKScore[i][j].iScore;
					infile >> g_pSaveData->m_SKScore[i][j].iMin;
					infile >> g_pSaveData->m_SKScore[i][j].iSec;
					infile >> g_pSaveData->m_SKScore[i][j].iDamage;
					infile >> g_pSaveData->m_SKScore[i][j].iFragment;
				}

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 10; j++)
				{
					g_pSaveData->m_USScore[i][j] = { 0,0,0,0,0 };
					infile >> g_pSaveData->m_USScore[i][j].iScore;
					infile >> g_pSaveData->m_USScore[i][j].iMin;
					infile >> g_pSaveData->m_USScore[i][j].iSec;
					infile >> g_pSaveData->m_USScore[i][j].iDamage;
					infile >> g_pSaveData->m_USScore[i][j].iFragment;
				}

			for (int i = 0; i < 10; i++)
			{
				g_pSaveData->m_InfUSScore[i] = { 0,0,0,0,0 };
				infile >> g_pSaveData->m_InfUSScore[i].iFloor;
				infile >> g_pSaveData->m_InfUSScore[i].iMin;
				infile >> g_pSaveData->m_InfUSScore[i].iSec;
				infile >> g_pSaveData->m_InfUSScore[i].iDamage;
				infile >> g_pSaveData->m_InfUSScore[i].iFragment;
			}
		}
		else
		{
			std::string szIn, szData;
			infile >> szIn;

			if (szIn.size() < 20)
			{
				infile.close();
				return FALSE;
			}

			int iSeed, iMul, iData;
			std::istringstream inStr;

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iSeed;
			inStr.clear();

			//Story

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->m_iStory = (10000 + iData - iSeed * iMul) % 10000;

			//Difficulty

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->m_iDifficulty = (10000 + iData - iSeed * iMul) % 10000;

			//Story Clear

			for (int i = 0; i < 4; i++)
			{
				szData = szIn.substr(0, 2);
				szIn = szIn.substr(2);
				inStr.str(szData);
				inStr >> iMul;
				inStr.clear();

				szData = szIn.substr(0, 4);
				szIn = szIn.substr(4);
				inStr.str(szData);
				inStr >> iData;
				inStr.clear();

				g_pSaveData->m_iClear[i] = (10000 + iData - iSeed * iMul) % 10000;
			}

			//Unlock

			for (int i = 0; i < 4; i++)
			{
				szData = szIn.substr(0, 2);
				szIn = szIn.substr(2);
				inStr.str(szData);
				inStr >> iMul;
				inStr.clear();

				szData = szIn.substr(0, 4);
				szIn = szIn.substr(4);
				inStr.str(szData);
				inStr >> iData;
				inStr.clear();

				g_pSaveData->m_iUnlock[i] = (10000 + iData - iSeed * iMul) % 10000;
			}

			//HopeFragment

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->m_iHopeFragment = (10000 + iData - iSeed * iMul) % 10000;

			//GoldFragment

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->m_iGoldFragment = (10000 + iData - iSeed * iMul) % 10000;

			//Chara

			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			g_pSaveData->m_iCharaID = (10000 + iData - iSeed * iMul) % 10000;

			for (int i = 0; i < CHARACOUNT; i++)
			{
				if (szIn.size() < 12 + 6 * SKILLCOUNT)
					break;

				CharaData readChara;

				szData = szIn.substr(0, 2);
				szIn = szIn.substr(2);
				inStr.str(szData);
				inStr >> iMul;
				inStr.clear();

				szData = szIn.substr(0, 4);
				szIn = szIn.substr(4);
				inStr.str(szData);
				inStr >> iData;
				inStr.clear();

				readChara.ID = (10000 + iData - iSeed * iMul) % 10000;

				szData = szIn.substr(0, 2);
				szIn = szIn.substr(2);
				inStr.str(szData);
				inStr >> iMul;
				inStr.clear();

				szData = szIn.substr(0, 4);
				szIn = szIn.substr(4);
				inStr.str(szData);
				inStr >> iData;
				inStr.clear();

				readChara.bAvailable = (10000 + iData - iSeed * iMul) % 10000;

				for (int j = 0; j < SKILLCOUNT; j++)
				{
					szData = szIn.substr(0, 2);
					szIn = szIn.substr(2);
					inStr.str(szData);
					inStr >> iMul;
					inStr.clear();

					szData = szIn.substr(0, 4);
					szIn = szIn.substr(4);
					inStr.str(szData);
					inStr >> iData;
					inStr.clear();

					readChara.iSkillLevel[j] = (10000 + iData - iSeed * iMul) % 10000;
				}

				for (auto &chara : g_pSaveData->m_vecChara)
				{
					if (chara.ID == readChara.ID)
					{
						chara.bAvailable = readChara.bAvailable;
						for (int j = 0; j < SKILLCOUNT; j++)
						{
							chara.iSkillLevel[j] = readChara.iSkillLevel[j];
						}
					}

				}
			}
		}
	}
	infile.close();
	return TRUE;
}

BOOL WriteSaveData(int iData)
{
	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);
	wcscat_s(szPath, L"\\SaveHope");
	if (!PathFileExists(szPath))
		CreateDirectory(szPath, NULL);

	WCHAR szFile[MAX_PATH] = L"";
	swprintf_s(szFile, L"%s\\save%d.dat",szPath, iData);

	std::ofstream outfile(szFile);

	int iSeed = 11 + rand() % 89;
	int iMul, iOut;

	outfile << iSeed;

	if (iData == 0)
	{
		//MoMissRecord

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
			{
				iMul = 11 + rand() % 89;
				outfile << iMul;
				iOut = (iSeed * iMul + g_pSaveData->m_iNoMiss[i][j]) % 10000;
				outfile << std::setw(4) << std::setfill('0') << iOut;
			}
		outfile << std::endl;

		//Score

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 10; j++)
			{
				outfile << g_pSaveData->m_STScore[i][j].iScore << std::endl;
				outfile << g_pSaveData->m_STScore[i][j].iMaxCombo << std::endl;
				outfile << g_pSaveData->m_STScore[i][j].iPerfect << std::endl;
				outfile << g_pSaveData->m_STScore[i][j].iGood << std::endl;
				outfile << g_pSaveData->m_STScore[i][j].iBad << std::endl;
				outfile << g_pSaveData->m_STScore[i][j].iMiss << std::endl;
			}

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 10; j++)
			{
				outfile << g_pSaveData->m_SKScore[i][j].iScore << std::endl;
				outfile << g_pSaveData->m_SKScore[i][j].iMin << std::endl;
				outfile << g_pSaveData->m_SKScore[i][j].iSec << std::endl;
				outfile << g_pSaveData->m_SKScore[i][j].iDamage << std::endl;
				outfile << g_pSaveData->m_SKScore[i][j].iFragment << std::endl;
			}

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 10; j++)
			{
				outfile << g_pSaveData->m_USScore[i][j].iScore << std::endl;
				outfile << g_pSaveData->m_USScore[i][j].iMin << std::endl;
				outfile << g_pSaveData->m_USScore[i][j].iSec << std::endl;
				outfile << g_pSaveData->m_USScore[i][j].iDamage << std::endl;
				outfile << g_pSaveData->m_USScore[i][j].iFragment << std::endl;
			}

		for (int i = 0; i < 10; i++)
		{
			outfile << g_pSaveData->m_InfUSScore[i].iFloor << std::endl;
			outfile << g_pSaveData->m_InfUSScore[i].iMin << std::endl;
			outfile << g_pSaveData->m_InfUSScore[i].iSec << std::endl;
			outfile << g_pSaveData->m_InfUSScore[i].iDamage << std::endl;
			outfile << g_pSaveData->m_InfUSScore[i].iFragment << std::endl;
		}
	}
	else
	{
		//Story

		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->m_iStory) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;

		//Difficulty

		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->m_iDifficulty) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;

		//Story Clear

		for (int i = 0; i < 4; i++)
		{
			iMul = 11 + rand() % 89;
			outfile << iMul;
			iOut = (iSeed * iMul + g_pSaveData->m_iClear[i]) % 10000;
			outfile << std::setw(4) << std::setfill('0') << iOut;
		}

		//Unlock

		for (int i = 0; i < 4; i++)
		{
			iMul = 11 + rand() % 89;
			outfile << iMul;
			iOut = (iSeed * iMul + g_pSaveData->m_iUnlock[i]) % 10000;
			outfile << std::setw(4) << std::setfill('0') << iOut;
		}

		//HopeFragment

		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->m_iHopeFragment) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;

		//GoldFragment

		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->m_iGoldFragment) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;

		//Chara

		iMul = 11 + rand() % 89;
		outfile << iMul;
		iOut = (iSeed * iMul + g_pSaveData->m_iCharaID) % 10000;
		outfile << std::setw(4) << std::setfill('0') << iOut;

		for (int i = 0; i < CHARACOUNT; i++)
		{
			iMul = 11 + rand() % 89;
			outfile << iMul;
			iOut = (iSeed * iMul + g_pSaveData->m_vecChara[i].ID) % 10000;
			outfile << std::setw(4) << std::setfill('0') << iOut;

			iMul = 11 + rand() % 89;
			outfile << iMul;
			iOut = (iSeed * iMul + g_pSaveData->m_vecChara[i].bAvailable) % 10000;
			outfile << std::setw(4) << std::setfill('0') << iOut;

			for (int j = 0; j < SKILLCOUNT; j++)
			{
				iMul = 11 + rand() % 89;
				outfile << iMul;
				iOut = (iSeed * iMul + g_pSaveData->m_vecChara[i].iSkillLevel[j]) % 10000;
				outfile << std::setw(4) << std::setfill('0') << iOut;
			}
		}

		outfile.close();

		g_bSaveDataUpdated = false;
	}

	return TRUE;
}

BOOL GetSaveInfo(int iData, SaveInfo *pInfo)
{
	WCHAR szPath[MAX_PATH] = L"";
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, L"\\StillBox");
	wcscat_s(szPath, L"\\SaveHope");

	WCHAR szFile[MAX_PATH] = L"";
	swprintf_s(szFile, TEXT("%s\\save%d.dat"), szPath, iData);

	HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL,
							  OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	FILETIME ftModify;
	SYSTEMTIME stLocal;

	if (INVALID_HANDLE_VALUE == hFile)
		return FALSE;

	if (!GetFileTime(hFile, NULL, NULL, &ftModify))
		return FALSE;

	CloseHandle(hFile);

	ZeroMemory(&stLocal, sizeof(SYSTEMTIME));
	FileTimeToLocalFileTime(&ftModify, &ftModify);
	FileTimeToSystemTime(&ftModify, &stLocal);

	pInfo->iYear = stLocal.wYear;
	pInfo->iMonth = stLocal.wMonth;
	pInfo->iDay = stLocal.wDay;
	pInfo->iHour = stLocal.wHour;
	pInfo->iMinute = stLocal.wMinute;

	std::ifstream infile;
	infile.open(szFile, std::ios::_Nocreate);

	if (!infile.is_open())
	{
		return FALSE;
	}
	else
	{
		std::string szIn, szData;
		infile >> szIn;

		if (szIn.size() < 20)
			return FALSE;

		int iSeed, iMul, iData;
		std::istringstream inStr;

		szData = szIn.substr(0, 2);
		szIn = szIn.substr(2);
		inStr.str(szData);
		inStr >> iSeed;
		inStr.clear();

		//Story

		szData = szIn.substr(0, 2);
		szIn = szIn.substr(2);
		inStr.str(szData);
		inStr >> iMul;
		inStr.clear();

		szData = szIn.substr(0, 4);
		szIn = szIn.substr(4);
		inStr.str(szData);
		inStr >> iData;
		inStr.clear();

		pInfo->iStory = (10000 + iData - iSeed * iMul) % 10000;

		//Difficulty

		szData = szIn.substr(0, 2);
		szIn = szIn.substr(2);
		inStr.str(szData);
		inStr >> iMul;
		inStr.clear();

		szData = szIn.substr(0, 4);
		szIn = szIn.substr(4);
		inStr.str(szData);
		inStr >> iData;
		inStr.clear();

		pInfo->iDifficulty = (10000 + iData - iSeed * iMul) % 10000;

		//StoryClear

		for (int i = 0; i < 4; i++)
		{
			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			pInfo->iClear[i] = (10000 + iData - iSeed * iMul) % 10000;
		}

		//Unlock

		for (int i = 0; i < 4; i++)
		{
			szData = szIn.substr(0, 2);
			szIn = szIn.substr(2);
			inStr.str(szData);
			inStr >> iMul;
			inStr.clear();

			szData = szIn.substr(0, 4);
			szIn = szIn.substr(4);
			inStr.str(szData);
			inStr >> iData;
			inStr.clear();

			pInfo->iUnlock[i] = (10000 + iData - iSeed * iMul) % 10000;
		}

		//HopeFragment

		szData = szIn.substr(0, 2);
		szIn = szIn.substr(2);
		inStr.str(szData);
		inStr >> iMul;
		inStr.clear();

		szData = szIn.substr(0, 4);
		szIn = szIn.substr(4);
		inStr.str(szData);
		inStr >> iData;
		inStr.clear();

		pInfo->iHopeFragment = (10000 + iData - iSeed * iMul) % 10000;

		//GoldFragment

		szData = szIn.substr(0, 2);
		szIn = szIn.substr(2);
		inStr.str(szData);
		inStr >> iMul;
		inStr.clear();

		szData = szIn.substr(0, 4);
		szIn = szIn.substr(4);
		inStr.str(szData);
		inStr >> iData;
		inStr.clear();

		pInfo->iGoldFragment = (10000 + iData - iSeed * iMul) % 10000;
	}

	infile.close();

	pInfo->bExist = true;
	return TRUE;
}

int GetCharaSkillLevel(int iCharaID, int iSkill)
{
	for (auto chara : g_pSaveData->m_vecChara)
	{
		if (chara.ID == iCharaID)
			return chara.iSkillLevel[iSkill];
	}
	return 0;
}

void CheckSTScore(int iDiff, int iScore, int iMaxCombo, int iPerfect, int iGood, int iBad, int iMiss)
{
	for (int i = 9; i >= 0; i--)
	{
		if (g_pSaveData->m_STScore[iDiff][i].iScore > iScore)
		{
			return;
		}
		else
		{
			if (i != 9)
				g_pSaveData->m_STScore[iDiff][i + 1] = g_pSaveData->m_STScore[iDiff][i];

			g_pSaveData->m_STScore[iDiff][i] = { iScore, iMaxCombo, iPerfect, iGood, iBad, iMiss };
		}
	}
	WriteSaveData(0);
}

void CheckSKScore(int iDiff, int iScore, int iMin, int iSec, int iDamage, int iFragment)
{
	for (int i = 9; i >= 0; i--)
	{
		if (g_pSaveData->m_SKScore[iDiff][i].iScore > iScore)
		{
			return;
		}
		else
		{
			if (i != 9)
				g_pSaveData->m_SKScore[iDiff][i + 1] = g_pSaveData->m_SKScore[iDiff][i];

			g_pSaveData->m_SKScore[iDiff][i] = { iScore, iMin, iSec, iDamage, iFragment };
		}
	}
	WriteSaveData(0);
}

void CheckUSScore(int iDiff, int iScore, int iMin, int iSec, int iDamage, int iFragment)
{
	for (int i = 9; i >= 0; i--)
	{
		if (g_pSaveData->m_USScore[iDiff][i].iScore > iScore)
		{
			return;
		}
		else
		{
			if (i != 9)
				g_pSaveData->m_USScore[iDiff][i + 1] = g_pSaveData->m_USScore[iDiff][i];

			g_pSaveData->m_USScore[iDiff][i] = { iScore, iMin, iSec, iDamage, iFragment };
		}
	}
	WriteSaveData(0);
}

void CheckInfUSScore(int iFloor, int iMin, int iSec, int iDamage, int iFragment)
{
	for (int i = 9; i >= 0; i--)
	{
		if (g_pSaveData->m_InfUSScore[i].iFloor > iFloor)
		{
			return;
		}
		else
		{
			if (i != 9)
				g_pSaveData->m_InfUSScore[i + 1] = g_pSaveData->m_InfUSScore[i];

			g_pSaveData->m_InfUSScore[i] = { iFloor, iMin, iSec, iDamage, iFragment };
		}
	}
	WriteSaveData(0);
}

void UnlockChara(int ID)
{
	for (auto &chara : g_pSaveData->m_vecChara)
	{
		if (chara.ID == ID)
		{
			chara.bAvailable = 1;
			return;
		}
	}
}