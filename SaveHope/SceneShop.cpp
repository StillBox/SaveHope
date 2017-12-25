#include "SceneUtil.h"
#include "SceneShop.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace ShopClass;

namespace ShopData
{
	const float fSpacing = 64.0f;
	const float fBPM = 128.01f;
	const int iCost[18] = { 1,2,3,4,5,6,8,12,18,24,30,36,44,55,66,77,88,99 };
	const LPWSTR szSkillInfo[SKILLCOUNT][2] =
	{
		{ L"力量",L"精准射击：\n降低带负面效果瓶子的出现率\n\n\
					节奏闪避：\n减少每次冲压机的出现数量\n\n\
					极限攀登：\n提高转身能力的恢复速度" },
		{ L"敏捷",L"精准射击：\n略微提高PERFECT的判定范围\n\n\
					节奏闪避：\n略微降低冲压机的伤害判定高度\n\n\
					极限攀登：\n延长冲刺的最长持续时间" },
		{ L"耐力",L"精准射击：\n缩短狛枝从中毒状态恢复的时间\n\n\
					节奏闪避：\n提高角色的最高连接同步值\n\n\
					极限攀登：\n提高角色的最高连接同步值" },
		{ L"智力",L"精准射击：\n缩短负面效果的持续时间\n\n\
					节奏闪避：\n扩大角色的观测场范围\n\n\
					极限攀登：\n每6级获得或增加二段跳次数" },
		{ L"幸运",L"提高各种好事的发生率~" }
	};

	float fBaseSPB;
	float fScale;
	float fActSpacing;

	int iFocus; // 0 - CharaSelect
				// 1 - CharaData
	
	int iSkill;	// 0 - Str
				// 1 - Agi
				// 2 - End
				// 3 - Int
				// 4 - Luc

	int iHopeFragment;
	int iGoldFragment;
	int iCostFragment;
	int iChara;
	int iAvailable;
	int iSkillLevel[SKILLCOUNT];
	int iSkillUp[SKILLCOUNT];

	bool bKeyUp[6];
	float fKeyUp[6];

	Background	*pBackground;
	Box			*pBoxChara;
	Box			*pBoxData;
	Box			*pBoxInfo;
	Box			*pBoxFrag;
	Arrow		*pArrow;
	Fragment	*pFragment;
	SkillBar	*pSkillBar;

	Chara		*pChara;
	Chara		*pPreChara;

	Sound		*pBGM;
	Sound		*pSE;

	ID3DXFont	*pFont;
	ID3DXFont	*pSFont;
	ID3DXFont	*pLFont;
}

using namespace ShopData;

void LoadCharaData()
{
	iAvailable = GetSaveData().m_vecChara[iChara].bAvailable;
	for (int i = 0; i < SKILLCOUNT; i++)
	{
		iSkillLevel[i] = GetSaveData().m_vecChara[iChara].iSkillLevel[i];
		iSkillUp[i] = 0;
	}
}

void SaveCharaData()
{
	GetSaveData().m_vecChara[iChara].bAvailable = iAvailable;
	for (int i = 0; i < SKILLCOUNT; i++)
	{
		GetSaveData().m_vecChara[iChara].iSkillLevel[i] = iSkillLevel[i] + iSkillUp[i];
	}
	pChara->SetState(HAPPY);

	iHopeFragment -= iCostFragment;
	while (iHopeFragment < 0)
	{
		iHopeFragment += 10;
		iGoldFragment--;
	}
	iCostFragment = 0;
	GetSaveData().m_iHopeFragment = iHopeFragment;
	GetSaveData().m_iGoldFragment = iGoldFragment;
}

bool IsSkillChanged()
{
	for (int i = 0; i < SKILLCOUNT; i++)
	{
		if (iSkillUp[i] != 0)
		{
			return true;
		}
	}
	return false;
}

//
//Chara
//

Chara::Chara()
{
	m_ID = GetCharaID(iChara, 1);
	m_iHeight = GetCharaHeight(iChara, 1);
	m_iWeight = GetCharaWeight(iChara, 1);
	m_iBust = GetCharaBust(iChara, 1);

	GetCharaName(m_szName, iChara, 1);
	GetCharaKata(m_szKata, iChara, 1);
	GetCharaTitle(m_szTitle, iChara, 1);
	GetCharaBirth(m_szBirth, iChara, 1);
	GetCharaBlood(m_szBlood, iChara, 1);
	GetCharaStage(m_szStage, iChara, 1);
	GetCharaDegree(m_szDegree, iChara, 1);

	SetRange(-64, -144, 64, 0);
	Init();
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(m_ID), 256, 256);
	AddTexRect(  0,   0,  32,  36);				 // 0  -- D-1
	AddTexRect( 32,   0,  64,  36);				 // 1  -- D-2
	AddTexRect(224,   0, 256,  36);				 // 2  -- D-8
	AddTexRect( 64,  72,  96, 108);				 // 3  -- L-3
	AddTexRect( 96,  72, 128, 108);				 // 4  -- L-4
	AddTexRect( 64, 108,  96, 144);				 // 5  -- R-3
	AddTexRect( 96, 108, 128, 144);				 // 6  -- R-4
	AddTexRect(128, 144, 160, 180);				 // 7  -- E-5
	AddTexRect(160, 144, 192, 180);				 // 8  -- E-6
	AddState(D_STAY, LOOP, fBaseSPB, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 1);
	AddState(L_MOVE, LOOP, fBaseSPB / 4, 3, 4);
	AddState(R_MOVE, LOOP, fBaseSPB / 4, 5, 6);
	AddState(HAPPY, NOLOOP, fBaseSPB / 2, 7, 8, 7, 8);
	AddStateChain(HAPPY, HAPPY, D_STAY);

	SetPos(7 * fSpacing, 5 * fSpacing); 

	m_fSpeed = 0.6f;
	SetState(D_STAY);
	m_bIn = true;
}

Chara::~Chara()
{
	DXObject::~DXObject();
}

BOOL Chara::Update(double fTime, float fElapsedTime)
{
	switch (m_iState)
	{
	case L_MOVE:
		m_ObjPos.x += m_u * fElapsedTime;
		if (m_bIn)
		{
			if (m_ObjPos.x <= 7 * fSpacing)
			{
				SetPos(7 * fSpacing, 5 * fSpacing);
				SetState(D_STAY);
			}
		}
		else
		{
			if (m_ObjPos.x <= 4 * fSpacing)
			{
				SetPos(4 * fSpacing, 5 * fSpacing);
				SetState(D_STAY);
			}
		}
		break;
	case R_MOVE:
		m_ObjPos.x += m_u * fElapsedTime;
		if (m_bIn)
		{
			if (m_ObjPos.x >= 7 * fSpacing)
			{
				SetPos(7 * fSpacing, 5 * fSpacing);
				SetState(D_STAY);
			}
		}
		else
		{
			if (m_ObjPos.x >= 10 * fSpacing)
			{
				SetPos(10 * fSpacing, 5 * fSpacing);
				SetState(D_STAY);
			}
		}
		break;
	}

	DXObject::Update(fTime, fElapsedTime);
	return TRUE;
}

BOOL Chara::Render(double fTime, float fElapsedTime)
{
	DXObject::Render(fTime, fElapsedTime);

	if (IsReady())
	{
		RECT rcText;
		WCHAR szBuffer[MAX_PATH];

		SetGridRect(&rcText, 2.4f, 1.6f, 4.0f, 3.0f, fActSpacing);
		swprintf_s(szBuffer, L"NO.%03d", m_ID);
		pSFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);

		SetGridRect(&rcText, 4.0f, 1.6f, 10.0f, 2.85f, fActSpacing);
		pSFont->DrawText(NULL, m_szKata, -1, &rcText, DT_CENTER | DT_TOP, BLACK);
		pLFont->DrawText(NULL, m_szName, -1, &rcText, DT_CENTER | DT_BOTTOM, BLACK);

		static float fFlash = 0.0f;
		fFlash += fElapsedTime;
		if (fFlash >= 2 * fBaseSPB)
			fFlash -= 4 * fBaseSPB;

		SetGridRect(&rcText, 2.0f, 5.0f, 12.0f, 6.0f, fActSpacing);
		if (iAvailable)
		{
			if (m_ID == GetSaveData().m_iCharaID)
				pFont->DrawText(NULL, L"当前使用角色", -1, &rcText, DT_CENTER | DT_BOTTOM, BLACK);
			else
				if (fFlash >= 0.0f)
					pFont->DrawText(NULL, L"按空格键使用", -1, &rcText, DT_CENTER | DT_BOTTOM, BLACK);
		}
		else
		{
			pFont->DrawText(NULL, L"未解锁角色", -1, &rcText, DT_CENTER | DT_BOTTOM, BLACK);
		}

		if (iFocus == 0)
		{
			if (iAvailable)
			{
				SetGridRect(&rcText, 3.0f, 8.0f, 11.0f, 9.5f, fActSpacing);
				pLFont->DrawText(NULL, m_szTitle, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

				SetGridRect(&rcText, 3.5f, 10.0f, 6.5f, 11.0f, fActSpacing);
				pFont->DrawText(NULL, L"生日", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
				SetGridRect(&rcText, 7.5f, 10.0f, 10.5f, 11.0f, fActSpacing);
				pFont->DrawText(NULL, m_szBirth, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
				
				SetGridRect(&rcText, 3.5f, 11.0f, 6.5f, 12.0f, fActSpacing);
				pFont->DrawText(NULL, L"血型", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
				SetGridRect(&rcText, 7.5f, 11.0f, 10.5f, 12.0f, fActSpacing);
				pFont->DrawText(NULL, m_szBlood, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
				
				swprintf_s(szBuffer, L"%dcm", m_iHeight);
				SetGridRect(&rcText, 3.5f, 12.0f, 6.5f, 13.0f, fActSpacing);
				pFont->DrawText(NULL, L"身高", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
				pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				
				swprintf_s(szBuffer, L"%dkg", m_iWeight);
				SetGridRect(&rcText, 7.5f, 12.0f, 10.5f, 13.0f, fActSpacing);
				pFont->DrawText(NULL, L"体重", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
				pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);

				SetGridRect(&rcText, 3.0f, 13.5f, 11.0f, 15.0f, fActSpacing);
				pFont->DrawText(NULL, m_szDegree, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

			}
			else
			{
				SetGridRect(&rcText, 4.0f, 8.0f, 10.0f, 15.0f, fActSpacing);
				pLFont->DrawText(NULL, L"（未解锁）", -1, &rcText, DT_CENTER | DT_VCENTER, GRAY);
			}
		}
	}
	
	return TRUE;
}

BOOL Chara::SetState(int iState, float u, bool bIn)
{
	if (iState != m_iState)
		DXObject::SetState(iState);

	m_u = u * m_fSpeed;
	m_bIn = bIn;

	return TRUE;
}

int Chara::GetState()
{ 
	return m_iState;
}

bool Chara::IsReady()
{
	if (m_bIn)
		if (D_STAY == m_iState || HAPPY == m_iState)
			return true;
	return false;
}

//
//Fragment
//

Fragment::Fragment()
{
	SetRange(-fSpacing / 2, -fSpacing, fSpacing / 2, fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	Init();
	AddTexRect( 0, 0, 16, 32);
	AddTexRect(16, 0, 32, 32);
	AddTexRect(32, 0, 48, 32);
	AddTexRect(48, 0, 64, 32);
	AddTexRect( 0, 32, 16, 64);
	AddTexRect(16, 32, 32, 64);
	AddTexRect(32, 32, 48, 64);
	AddTexRect(48, 32, 64, 64);

	AddState(0, SINGLE, 0.0f, 0);
	AddState(1, SINGLE, 0.0f, 1);
	AddState(2, SINGLE, 0.0f, 2);
	AddState(3, SINGLE, 0.0f, 3);
	AddState(4, SINGLE, 0.0f, 4);
	AddState(5, SINGLE, 0.0f, 5);
	AddState(6, SINGLE, 0.0f, 6);
	AddState(7, SINGLE, 0.0f, 7);

	m_iTex = 0;
	m_timeTex = 0.0f;
}

Fragment::~Fragment()
{
	DXObject::~DXObject();
}

BOOL Fragment::Update(double fTime, float fElapsedTime)
{
	m_timeTex += fElapsedTime;
	if (m_timeTex >= (float)fBaseSPB / 4)
	{
		m_iTex++;
		if (m_iTex >= 4) m_iTex = 0;
		m_timeTex -= (float)fBaseSPB / 4;
	}

	m_iHope = iHopeFragment - iCostFragment;
	m_iGold = iGoldFragment;

	while (m_iHope < 0)
	{
		m_iGold--;
		m_iHope += 10;
	}

	return TRUE;
}

BOOL Fragment::Render(double fTime, float fElapsedTime)
{
	SetPos(17 * fSpacing, 2.5f * fSpacing);
	SetState(m_iTex + 4);
	DXObject::Render(fTime, fElapsedTime);

	SetPos(22 * fSpacing, 2.5f * fSpacing);
	SetState(m_iTex);
	DXObject::Render(fTime, fElapsedTime);

	RECT rcText;
	WCHAR szBuffer[MAX_PATH];
	SetGridRect(&rcText, 18.0f, 2.0f, 22.0f, 3.0f, fActSpacing);
	swprintf_s(szBuffer, L"X %3d", m_iGold);
	pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
	SetGridRect(&rcText, 23.0f, 2.0f, 27.0f, 3.0f, fActSpacing);
	swprintf_s(szBuffer, L"X %3d", m_iHope);
	pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);

	return TRUE;
}

//
//SkillBar
//

SkillBar::SkillBar()
{
	SetRange(-64, -64, 64, 64);	
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	Init();
	AddTexRect( 0, 64,  32,  96);				 // 0 
	AddTexRect(32, 64,  64,  96);				 // 1 
	AddTexRect(64, 64,  96,  96);				 // 2 
	AddTexRect(96, 64, 128,  96);				 // 3 
	AddTexRect( 0, 96,  32, 128);				 // 4 
	AddTexRect(32, 96,  64, 128);				 // 5 
	AddTexRect(64, 96,  96, 128);				 // 6 
	AddTexRect(96, 96, 128, 128);				 // full 
	AddState(0, SINGLE, 0.0f, 0);
	AddState(1, SINGLE, 0.0f, 1);
	AddState(2, SINGLE, 0.0f, 2);
	AddState(3, SINGLE, 0.0f, 3);
	AddState(4, SINGLE, 0.0f, 4);
	AddState(5, SINGLE, 0.0f, 5);
	AddState(6, SINGLE, 0.0f, 6);
	AddState(7, SINGLE, 0.0f, 7);
	SetState(0);
	SetPos(0.0f, 0.0f);
}

SkillBar::~SkillBar()
{
	DXObject::~DXObject();
}

BOOL SkillBar::Update(double fTime, float fElapsedTime)
{
	for (int i = 0; i < SKILLCOUNT; i++)
	{
		int iFull = (iSkillLevel[i] + iSkillUp[i]) / 6;
		int iLast = (iSkillLevel[i] + iSkillUp[i]) % 6;
		for (int j = 0; j < 3; j++)
		{
			if (iFull == 3)
				m_iLevel[i][j] = 7;
			else
			{
				if (j <= iFull)
					m_iLevel[i][j] = 6;
				else
					m_iLevel[i][j] = 0;
				if (j == iFull)
					m_iLevel[i][j] = iLast;
			}
		}
	}

	return TRUE;
}

BOOL SkillBar::Render(double fTime, float fElapsedTime)
{
	if (!iAvailable)
		return FALSE;

	float x, y;
	for (int i = 0; i < SKILLCOUNT; i++)
	{
		y = (6 + 2 * i) * fSpacing;
		for (int j = 0; j < 3; j++)
		{
			x = (19.2f + 2.4f * j) * fSpacing;
			SetPos(x, y);
			SetState(m_iLevel[i][j]);
			DXObject::Render(fTime, fElapsedTime);
		}
	}

	if (iFocus == 1)
	{
		int iCurSkillLevel = iSkillLevel[iSkill] + iSkillUp[iSkill];
		if (iCurSkillLevel != 18)
		{
			RECT rcText;
			WCHAR szBuffer[MAX_PATH];
			SetGridRect(&rcText, 25.0f, (5.0f + 2.0f * iSkill), 27.0f, (7.0f + 2.0f * iSkill), fActSpacing);
			swprintf_s(szBuffer, L"Next\n%d", iCost[iCurSkillLevel]);
			pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
	}

	return TRUE;
}

//
//Arrow
//

Arrow::Arrow()
{
	SetRange(-64, -72, 64, 72);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM4), 128, 128);
	Init();
	AddTexRect( 0, 0, 32, 36);				 // 0  -- 1-1
	AddTexRect(32, 0, 64, 36);				 // 1  -- 1-2
	AddTexRect(64, 0, 96, 36);				 // 2  -- 1-3
	AddState(0, SINGLE, 0.0f, 0);
	AddState(1, SINGLE, 0.0f, 1);
	AddState(2, SINGLE, 0.0f, 2);
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
	switch (iFocus)
	{
	case 0:
		SetState(2);
		SetPos(4 * fSpacing, 4 * fSpacing);
		DXObject::Render(fTime, fElapsedTime);
		SetPos(10 * fSpacing, 4 * fSpacing);
		DXObject::Render(fTime, fElapsedTime);
		SetState(0);
		SetPos(4 * fSpacing - m_fOffset, 4 * fSpacing);
		DXObject::Render(fTime, fElapsedTime);
		SetState(1);
		SetPos(10 * fSpacing + m_fOffset, 4 * fSpacing);
		DXObject::Render(fTime, fElapsedTime);
		break;
	case 1:
		SetState(1);
		SetPos(14.6f * fSpacing + m_fOffset, (6 + 2 * iSkill) * fSpacing);
		DXObject::Render(fTime, fElapsedTime);
		break;
	case 2:
		SetState(1);
		SetPos(14.6f * fSpacing + 0.03f * fBaseSPB, (6 + 2 * iSkill) * fSpacing);
		DXObject::Render(fTime, fElapsedTime);
		break;
	}

	return TRUE;
}

//
//Scene
//

SceneShop::SceneShop()
{
	pBoxChara = NULL;
	pBoxData = NULL;
	pBoxInfo = NULL;
	pArrow = NULL;
	pBGM = NULL;
	pSE = NULL;
}

SceneShop::~SceneShop()
{
	End();
}

void SceneShop::Init()
{
	fScale = GetSettings().m_fScale;
	fBaseSPB = 60000.0f / fBPM;
	fActSpacing = fSpacing * fScale;
	iChara = 0;
	LoadCharaData();
	iHopeFragment = GetSaveData().m_iHopeFragment;
	iGoldFragment = GetSaveData().m_iGoldFragment;

	//Status

	iFocus = 0;
	iSkill = 0;

	for (int i = 0; i < 6; i++)
	{
		bKeyUp[i] = false;
		bKeyUp[i] = 2 * fBaseSPB;
	}

	//Pointers

	pBackground = new Background();

	pBoxChara = new Box();
	pBoxData = new Box();
	pBoxInfo = new Box();
	pBoxFrag = new Box();
	pBoxChara->Set(448, 256, 256, 106, 1.0f);
	pBoxInfo->Set(448, 740, 256, 190, 1.0f);
	pBoxFrag->Set(1318, 160, 410, 10, 1.0f);
	pBoxData->Set(1318, 642, 410, 288, 1.0f);

	pArrow = new Arrow();
	pFragment = new Fragment();
	pSkillBar = new SkillBar();

	pChara = new Chara();

	pBGM = new Sound();
	pBGM->Register(ID_BGM_GALLERY, ID_SUBMIX_BACKGROUND);
	pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_GALLERY, L"MP3", 255);
	pBGM->Play(0);

	pSE = new Sound();
	pSE->Register(ID_SE_GALLERY, ID_SUBMIX_BACKGROUND);
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CURSOR, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SELECT, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_DECIDE, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CANCEL, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_GET, L"MP3");

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(36.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(40.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
}

void SceneShop::End()
{
	SAFE_DELETE(pBackground);

	SAFE_DELETE(pBoxChara);
	SAFE_DELETE(pBoxData);
	SAFE_DELETE(pBoxInfo);
	SAFE_DELETE(pBoxFrag);

	SAFE_DELETE(pArrow);
	SAFE_DELETE(pFragment);
	SAFE_DELETE(pSkillBar);

	SAFE_DELETE(pBGM);
	SAFE_DELETE(pSE);

	SAFE_DELETE(pChara);
	SAFE_DELETE(pPreChara);

	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);
}

void SceneShop::OnResetDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(36.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(40.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
}

void SceneShop::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);
}

void SceneShop::Update(double fTime, float fElapsedTime)
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
	
	if (iFocus == 0)
	{
		if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
		{
			if (D_STAY == pChara->GetState())
			{
				pSE->Play(1);
				iChara--;
				if (iChara == -1) iChara = CHARACOUNT - 1;

				SAFE_DELETE(pPreChara);
				pPreChara = pChara;
				pChara = new Chara();
				LoadCharaData();

				pPreChara->SetState(R_MOVE, 1.0f, false);
				pChara->SetPos(4 * fSpacing, 5 * fSpacing);
				pChara->SetState(R_MOVE, 1.0f);
			}
		}

		if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
		{
			if (D_STAY == pChara->GetState())
			{
				pSE->Play(1);
				iChara++;
				if (iChara == CHARACOUNT) iChara = 0;

				SAFE_DELETE(pPreChara);
				pPreChara = pChara;
				pChara = new Chara();
				LoadCharaData();

				pPreChara->SetState(L_MOVE, -1.0f, false);
				pChara->SetPos(10 * fSpacing, 5 * fSpacing);
				pChara->SetState(L_MOVE, -1.0f);
			}
		}

		if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
		{
			if (bKeyUp[4])
			{
				if (D_STAY == pChara->GetState())
				{
					if (iAvailable)
					{
						iFocus = 1;
						pSE->Play(2);
					}
					else
					{
						int CharaID = GetSaveData().m_vecChara[iChara].ID;
						if (IDC_HINATASUPER != CharaID && IDC_KAMUKURA != CharaID && iGoldFragment >= 6)
						{
							iAvailable = 1;
							iGoldFragment -= 6;
							SaveCharaData();
							SaveDataUpdated();
							pSE->Play(4);
						}
					}
				}
			}
			bKeyUp[4] = false;
		}
		else
			bKeyUp[4] = true;

		if (!IsCurtainOn())
		{
			if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
			{
				if (bKeyUp[5])
				{
					SetCurtain(0, 255, 500.0f, OpenSelectMenu);
					pSE->Play(3);
				}
				bKeyUp[5] = false;
			}
			else
				bKeyUp[5] = true;
		}
	}

	if (iFocus == 1)
	{
		if (!IsCheckBoxOpen())
		{
			if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
			{
				if (bKeyUp[0] || fKeyUp[0] == 0.0f)
				{
					pSE->Play(0);
					iSkill--;
					if (iSkill == -1) iSkill = 4;

					fKeyUp[0] = fBaseSPB / 4;
					if (bKeyUp[0])
						fKeyUp[0] = fBaseSPB;
				}
				bKeyUp[0] = false;
			}
			else
				bKeyUp[0] = true;

			if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
			{
				if (bKeyUp[1] || fKeyUp[1] == 0.0f)
				{
					pSE->Play(0);
					iSkill++;
					if (iSkill == 5) iSkill = 0;

					fKeyUp[1] = fBaseSPB / 4;
					if (bKeyUp[1])
						fKeyUp[1] = fBaseSPB;
				}
				bKeyUp[1] = false;
			}
			else
				bKeyUp[1] = true;

			if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
			{
				if (bKeyUp[2] || fKeyUp[2] == 0.0f)
				{
					
					if (iSkillUp[iSkill] != 0)
					{
						pSE->Play(1);
						iSkillUp[iSkill]--;
						int iCurSkillLevel = iSkillLevel[iSkill] + iSkillUp[iSkill];
						iCostFragment -= iCost[iCurSkillLevel];
					}

					fKeyUp[2] = fBaseSPB / 8;
					if (bKeyUp[2])
						fKeyUp[2] = fBaseSPB;
				}
				bKeyUp[2] = false;
			}
			else
				bKeyUp[2] = true;

			if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
			{
				if (bKeyUp[3] || fKeyUp[3] == 0.0f)
				{
					pSE->Play(1);
					int iCurSkillLevel = iSkillLevel[iSkill] + iSkillUp[iSkill];
					if (iCostFragment + iCost[iCurSkillLevel] <= 10 * iGoldFragment + iHopeFragment)
					{
						iCostFragment += iCost[iCurSkillLevel];
						iSkillUp[iSkill] = min(18 - iSkillLevel[iSkill], iSkillUp[iSkill] + 1);
					}

					fKeyUp[3] = fBaseSPB / 8;
					if (bKeyUp[3])
						fKeyUp[3] = fBaseSPB;
				}
				bKeyUp[3] = false;
			}
			else
				bKeyUp[3] = true;

			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (bKeyUp[4])
				{
					pSE->Play(2);
					if (IsSkillChanged())
					{
						SaveCharaData();
						LoadCharaData();
						SaveDataUpdated();
					}
				}
				bKeyUp[4] = false;
			}
			else
				bKeyUp[4] = true;

			if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
			{
				if (bKeyUp[5])
				{
					pSE->Play(3);
					if (IsSkillChanged())
					{
						for (int i = 0; i < SKILLCOUNT; i++)
							iSkillUp[i] = 0;
					}
					else
						iFocus = 0;
				}
				bKeyUp[5] = false;
			}
			else
				bKeyUp[5] = true;
		}
	}

	if (IsKeyDown(DIK_SPACE))
	{
		if (D_STAY == pChara->GetState())
		{
			int ID = GetCharaID(iChara, 1);
			if (iAvailable && ID != GetSaveData().m_iCharaID)
			{
				pSE->Play(2);
				pChara->SetState(HAPPY);
				GetSaveData().m_iCharaID = ID;
				SaveDataUpdated();
			}
		}
	}

	//Pointers
	pChara->Update(fTime, fElapsedTime);

	if (NULL != pPreChara)
	{
		pPreChara->Update(fTime, fElapsedTime);
		if (D_STAY == pPreChara->GetState())
		{
			pPreChara->~Chara();
			pPreChara = NULL;
		}
	}

	pArrow->Update(fTime, fElapsedTime);
	pFragment->Update(fTime, fElapsedTime);
	pSkillBar->Update(fTime, fElapsedTime);
	pBackground->Update(fTime, fElapsedTime);

////Test
//	if (true)
//	{
//		if (IsKeyDown(DIK_EQUALS))
//		{
//			iHopeFragment = 99;
//			iGoldFragment = 9;
//		}
//		if (IsKeyDown(DIK_0))
//		{
//			for (auto &chara : GetSaveData().m_vecChara)
//				chara.bAvailable = 1;
//		}
//	}
}

void SceneShop::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	SetView();
	SetProj(NULL, GAME_WIDTH, GAME_HEIGHT);
	SetViewPort();

	pBackground->Render(fTime, fElapsedTime);

	pBoxChara->Render(fElapsedTime);
	pBoxData->Render(fElapsedTime);
	pBoxInfo->Render(fElapsedTime);
	pBoxFrag->Render(fElapsedTime);

	pChara->Render(fTime, fElapsedTime);
	if (NULL != pPreChara)
		pPreChara->Render(fTime, fElapsedTime);

	pArrow->Render(fTime, fElapsedTime);
	pFragment->Render(fTime, fElapsedTime);
	pSkillBar->Render(fTime, fElapsedTime);

	if (iAvailable)
	{
		RECT rcText;

		for (int i = 0; i < SKILLCOUNT; i++)
		{
			SetGridRect(&rcText, 16.0f, (5.0f + 2.0f * i), 20.0f, (7.0f + 2.0f * i), fActSpacing);
			pLFont->DrawText(NULL, szSkillInfo[i][0], -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
		}

		if (iFocus == 1)
		{
			SetGridRect(&rcText, 3.0f, 9.0f, 11.0f, 14.0f, fActSpacing);
			pFont->DrawText(NULL, szSkillInfo[iSkill][1], -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
		}

	}
	else
	{
		RECT rcText;

		if (IDC_HINATASUPER == GetCharaID(iChara, 1) || IDC_KAMUKURA == GetCharaID(iChara, 1))
		{
			SetGridRect(&rcText, 16.0f, 7.0f, 25.0f, 12.0f, fActSpacing);
			pLFont->DrawText(NULL, L"通关剧情欺负人难度解锁", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		}
		else
		{
			if (iGoldFragment < 6)
			{
				SetGridRect(&rcText, 16.0f, 7.0f, 25.0f, 12.0f, fActSpacing);
				pLFont->DrawText(NULL, L"消耗6枚金色碎片解锁\n\n(剩余金色碎片数量不足)", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			}
			else
			{
				SetGridRect(&rcText, 16.0f, 7.0f, 25.0f, 12.0f, fActSpacing);
				pLFont->DrawText(NULL, L"消耗6枚金色碎片解锁", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			}
		}
	}
}
