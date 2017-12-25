#include "SceneUtil.h"
#include "SceneScore.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace ScoreClass;

namespace ScoreData
{
	const float fSpacing = 64.0f;
	const float fBPM = 120.0f;

	float fBaseSPB;
	float fScale;
	float fActSpacing;

	int iTitle;
	int iDifficulty;

	bool bKeyUp[6];
	float fKeyUp[6];

	Background	*pBackground;
	Box			*pBoxTitle;
	Box			*pBoxScore;
	Arrow		*pArrow;

	Sound		*pBGM;
	Sound		*pSE;

	ID3DXFont	*pFont;
	ID3DXFont	*pSFont;
	ID3DXFont	*pLFont;
}

using namespace ScoreData;

//
//Arrow
//

Arrow::Arrow()
{
	SetRange(-64, -72, 64, 72);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM4), 128, 128);
	Init();
	AddTexRect(0, 0, 32, 36);				 // 0  -- 1-1
	AddTexRect(32, 0, 64, 36);				 // 1  -- 1-2
	AddState(0, SINGLE, 0.0f, 0);
	AddState(1, SINGLE, 0.0f, 1);
	SetState(0);
	SetPos(256.0f, 256.0f);

	m_fOffset = 0.0f;
}

Arrow::~Arrow()
{
	DXObject::~DXObject();
}

BOOL Arrow::Update(double fTime, float fElapsedTime)
{
	m_fOffset += 0.04f * fElapsedTime;
	if (m_fOffset >= 0.04f * fBaseSPB)
		m_fOffset -= 0.04f * fBaseSPB;

	return TRUE;
}

BOOL Arrow::Render(double fTime, float fElapsedTime)
{
	SetState(0);
	SetPos(12 * fSpacing - m_fOffset, 2 * fSpacing);
	DXObject::Render(fTime, fElapsedTime);
	SetState(1);
	SetPos(18 * fSpacing + m_fOffset, 2 * fSpacing);
	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

//
//Scene
//

SceneScore::SceneScore()
{
	pBackground = NULL;
	pBoxTitle = NULL;
	pBoxScore = NULL;
	pArrow = NULL;
	pBGM = NULL;
	pSE = NULL;
	pFont = NULL;
	pSFont = NULL;
	pLFont = NULL;
}

SceneScore::~SceneScore()
{
	End();
}

void SceneScore::Init()
{
	fBaseSPB = 60000.0f / fBPM;
	fScale = GetSettings().m_fScale;
	fActSpacing = fSpacing * fScale;

	iTitle = 0;
	iDifficulty = 0;

	for (int i = 0; i < 6; i++)
	{
		bKeyUp[i] = false;
		bKeyUp[i] = 2 * fBaseSPB;
	}

	pBackground = new Background(true);

	pBoxTitle = new Box();
	pBoxTitle->Set(960, 128, 512, 0, 1.0f);

	pBoxScore = new Box();
	pBoxScore->Set(960, 608, 512, 320, 1.0f);

	pArrow = new Arrow();

	pBGM = new Sound();
	pBGM->Register(ID_BGM_GALLERY, ID_SUBMIX_BACKGROUND);
	pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_GALLERY, L"MP3", 255);
	pBGM->Play(0);

	pSE = new Sound();
	pSE->Register(ID_SE_GALLERY, ID_SUBMIX_BACKGROUND);
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CURSOR, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SELECT, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CANCEL, L"WAVE");

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(36.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(48.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
}

void SceneScore::End()
{
	SAFE_DELETE(pBackground);

	SAFE_DELETE(pBoxTitle);
	SAFE_DELETE(pBoxScore);

	SAFE_DELETE(pArrow);

	SAFE_DELETE(pBGM);
	SAFE_DELETE(pSE);

	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);
}

void SceneScore::OnResetDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(36.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(48.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
}

void SceneScore::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);
}

void SceneScore::Update(double fTime, float fElapsedTime)
{
	//Control

	GetInput();

	for (int i = 0; i < 6; i++)
	{
		if (fKeyUp[i] <= 0.0f)
			fKeyUp[i] = 0.0f;
		else
			fKeyUp[i] -= fElapsedTime;
	}

	if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
	{
		if (bKeyUp[0] || fKeyUp[0] == 0.0f)
		{
			pSE->Play(0);

			if (iTitle == 0)
				iTitle = 3;
			else
				iTitle--;

			fKeyUp[0] = fBaseSPB / 4;
			if (bKeyUp[0])
				fKeyUp[0] = fBaseSPB;
		}
		bKeyUp[0] = false;
	}
	else
		bKeyUp[0] = true;

	if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
	{
		if (bKeyUp[1] || fKeyUp[1] == 0.0f)
		{
			pSE->Play(0);

			if (iTitle == 3)
				iTitle = 0;
			else
				iTitle++;

			fKeyUp[1] = fBaseSPB / 4;
			if (bKeyUp[1])
				fKeyUp[1] = fBaseSPB;
		}
		bKeyUp[1] = false;
	}
	else
		bKeyUp[1] = true;

	if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
	{
		if (bKeyUp[2] || fKeyUp[2] == 0.0f)
		{
			pSE->Play(1);

			if (iDifficulty == 3)
				iDifficulty = 0;
			else
				iDifficulty++;

			fKeyUp[2] = fBaseSPB / 4;
			if (bKeyUp[2])
				fKeyUp[2] = fBaseSPB;
		}
		bKeyUp[2] = false;
	}
	else
		bKeyUp[2] = true;

	if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
	{
		if (bKeyUp[3] || fKeyUp[3] == 0.0f)
		{
			pSE->Play(1);

			if (iDifficulty == 3)
				iDifficulty = 0;
			else
				iDifficulty++;

			fKeyUp[3] = fBaseSPB / 4;
			if (bKeyUp[3])
				fKeyUp[3] = fBaseSPB;
		}
		bKeyUp[3] = false;
	}
	else
		bKeyUp[3] = true;

	if (!IsCurtainOn())
	{
		if (IsKeyDown(DIK_X) || IsKeyDown(DIK_ESCAPE))
		{
			if (bKeyUp[4])
			{
				pSE->Play(2);
				SetCurtain(0, 255, 500.0f, OpenMainMenu);
			}
			bKeyUp[4] = false;
		}
		else
			bKeyUp[4] = true;
	}

	pBackground->Update(fTime, fElapsedTime);
	pArrow->Update(fTime, fElapsedTime);
}

void SceneScore::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	SetView();
	SetProj(NULL, GAME_WIDTH, GAME_HEIGHT);
	SetViewPort();

	pBackground->Render(fTime, fElapsedTime);

	pBoxTitle->Render(fElapsedTime);
	pBoxScore->Render(fElapsedTime);

	RECT rcText;
	WCHAR szBuffer[MAX_PATH];

	switch (iTitle)
	{
	case 0:
		swprintf_s(szBuffer, L"精准射击");
		break;
	case 1:
		swprintf_s(szBuffer, L"节奏闪避");
		break;
	case 2:
		swprintf_s(szBuffer, L"极限攀登");
		break;
	case 3:
		swprintf_s(szBuffer, L"无尽攀登");
		break;
	}
	SetGridRect(&rcText, 12.0f, 1.0f, 18.0f, 3.0f, fActSpacing);
	pLFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

	if (iTitle != 3)
	{
		for (int i = 0; i < 4; i++)
		{
			if (i <= 1)
				SetGridRect(&rcText, 7.2f + i * 2.0f, 1.0f, 8.2f + i * 2.0f, 3.0f, fActSpacing);
			else
				SetGridRect(&rcText, 15.8f + i * 2.0f, 1.0f, 16.8f + i * 2.0f, 3.0f, fActSpacing);

			if (GetSaveData().m_iNoMiss[iTitle][i] == 1)
				pLFont->DrawTextW(NULL, L"★", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			else
				pLFont->DrawTextW(NULL, L"☆", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
	}

	if (iTitle == 0)
	{
		SetGridRect(&rcText, 7.0f, 4.5f, 8.0f, 16.0f, fActSpacing);
		switch (iDifficulty)
		{
		case 0:
			swprintf_s(szBuffer, L"简单难度");
			break;
		case 1:
			swprintf_s(szBuffer, L"普通难度");
			break;
		case 2:
			swprintf_s(szBuffer, L"困难难度");
			break;
		case 3:
			swprintf_s(szBuffer, L"欺负人难度");
			break;
		}
		DrawGridText(pLFont, szBuffer, 5, 1, fActSpacing, &rcText, BLACK);
		
		SetGridRect(&rcText, 8.0f, 4.0f, 12.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"总得分", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 11.5f, 4.0f, 14.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"最高连击", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 15.0f, 4.0f, 17.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"PERFECT", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 17.0f, 4.0f, 19.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"GOOD", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 19.0f, 4.0f, 21.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"BAD", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 21.0f, 4.0f, 23.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"MISS", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

		for (int i = 0; i < 10; i++)
		{
			SetGridRect(&rcText, 8.0f, 5.0f + i, 12.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_STScore[iDifficulty][i].iScore);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 12.0f, 5.0f + i, 14.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_STScore[iDifficulty][i].iMaxCombo);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 15.0f, 5.0f + i, 17.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_STScore[iDifficulty][i].iPerfect);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 17.0f, 5.0f + i, 19.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_STScore[iDifficulty][i].iGood);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 19.0f, 5.0f + i, 21.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_STScore[iDifficulty][i].iBad);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 21.0f, 5.0f + i, 23.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_STScore[iDifficulty][i].iMiss);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
	}
	else if (iTitle == 1)
	{
		SetGridRect(&rcText, 7.0f, 4.5f, 8.0f, 16.0f, fActSpacing);
		switch (iDifficulty)
		{
		case 0:
			swprintf_s(szBuffer, L"简单难度");
			break;
		case 1:
			swprintf_s(szBuffer, L"普通难度");
			break;
		case 2:
			swprintf_s(szBuffer, L"困难难度");
			break;
		case 3:
			swprintf_s(szBuffer, L"欺负人难度");
			break;
		}
		DrawGridText(pLFont, szBuffer, 5, 1, fActSpacing, &rcText, BLACK);

		SetGridRect(&rcText, 8.0f, 4.0f, 12.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"总得分", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 12.5f, 4.0f, 15.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"消耗时间", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 16.5f, 4.0f, 19.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"受击次数", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 20.0f, 4.0f, 23.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"收集数量", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

		for (int i = 0; i < 10; i++)
		{
			SetGridRect(&rcText, 8.0f, 5.0f + i, 12.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_SKScore[iDifficulty][i].iScore);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 12.5f, 5.0f + i, 15.5f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d：%d", GetSaveData().m_SKScore[iDifficulty][i].iMin, GetSaveData().m_SKScore[iDifficulty][i].iSec);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 16.5f, 5.0f + i, 19.5f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_SKScore[iDifficulty][i].iDamage);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 20.0f, 5.0f + i, 23.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_SKScore[iDifficulty][i].iFragment);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
	}
	else if (iTitle == 2)
	{
		SetGridRect(&rcText, 7.0f, 4.5f, 8.0f, 16.0f, fActSpacing);
		switch (iDifficulty)
		{
		case 0:
			swprintf_s(szBuffer, L"简单难度");
			break;
		case 1:
			swprintf_s(szBuffer, L"普通难度");
			break;
		case 2:
			swprintf_s(szBuffer, L"困难难度");
			break;
		case 3:
			swprintf_s(szBuffer, L"欺负人难度");
			break;
		}
		DrawGridText(pLFont, szBuffer, 5, 1, fActSpacing, &rcText, BLACK);

		SetGridRect(&rcText, 8.0f, 4.0f, 12.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"总得分", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 12.5f, 4.0f, 15.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"消耗时间", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 16.5f, 4.0f, 19.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"受击次数", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 20.0f, 4.0f, 23.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"收集数量", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

		for (int i = 0; i < 10; i++)
		{
			SetGridRect(&rcText, 8.0f, 5.0f + i, 12.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_USScore[iDifficulty][i].iScore);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 12.5f, 5.0f + i, 15.5f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d：%d", GetSaveData().m_USScore[iDifficulty][i].iMin, GetSaveData().m_USScore[iDifficulty][i].iSec);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 16.5f, 5.0f + i, 19.5f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_USScore[iDifficulty][i].iDamage);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 20.0f, 5.0f + i, 23.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_USScore[iDifficulty][i].iFragment);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
	}
	else if (iTitle == 3)
	{
		SetGridRect(&rcText, 7.0f, 4.5f, 8.0f, 16.0f, fActSpacing);
		swprintf_s(szBuffer, L"无尽模式");
		DrawGridText(pLFont, szBuffer, 5, 1, fActSpacing, &rcText, BLACK);

		SetGridRect(&rcText, 8.0f, 4.0f, 12.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"最高层数", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 12.5f, 4.0f, 15.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"消耗时间", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 16.5f, 4.0f, 19.5f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"受击次数", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 20.0f, 4.0f, 23.0f, 5.0f, fActSpacing);
		pFont->DrawTextW(NULL, L"收集数量", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

		for (int i = 0; i < 10; i++)
		{
			SetGridRect(&rcText, 8.0f, 5.0f + i, 12.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_InfUSScore[i].iFloor);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 12.5f, 5.0f + i, 15.5f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d：%d", GetSaveData().m_InfUSScore[i].iMin, GetSaveData().m_InfUSScore[i].iSec);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 16.5f, 5.0f + i, 19.5f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_InfUSScore[i].iDamage);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			SetGridRect(&rcText, 20.0f, 5.0f + i, 23.0f, 6.0f + i, fActSpacing);
			swprintf_s(szBuffer, L"%d", GetSaveData().m_InfUSScore[i].iFragment);
			pFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
	}

	pArrow->Render(fTime, fElapsedTime);
}
