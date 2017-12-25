#include "SceneUtil.h"
#include "SceneOmake.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace OmakeClass;

namespace OmakeData
{
	const float fBPM = 128.01f;
	const float fSpacing = 64.0f;
	const int cMusic = 11;
	const LPWSTR szMusic[cMusic] =
	{
		L"試練Prepare",					
		L"BOX 16",						
		L"エコロシア",					
		L"希望Searching",				
		L"V3 Gallery Music",			
		L"新世界のアルターエゴ",		
		L"ニュー ワールド オーダー",	
		L"開廷アンダーグラウンド",		
		L"モノミ先生の教育実習",		
		L"未来の前の日",				
		L"Re DISTRUST",					
	};
	const int iMusicID[cMusic] =
	{
		IDR_PREPARE,
		IDR_BOX16,
		IDR_EKOROSIA,
		IDR_KIBOU,
		IDR_GALLERY,
		IDR_ALTEREGO,
		IDR_NEWWORLD,
		IDR_UNDERGROUND,
		IDR_MONOMI,
		IDR_MIRAI,
		IDR_DISTRUST
	};

	float fBaseSPB;
	float fScale;
	float fActSpacing;

	//Control

	int iBox;
	
	bool bPlay;
	int iMusicListBeg;
	int iMusicListSel;
	int iPlayMusic;

	bool bDetail;
	int iCharaListBeg;
	int iCharaListSel;

	bool bKeyUp[7];
	float fKeyUp[7];

	//Pointers

	Background	*pBackground;
	Box			*pBoxMusic;
	Box			*pBoxChara;
	LCursor		*pLCursor;
	RCursor		*pRCursor;
	CharaList	*pCharaList;
	Chara		*pChara;
	BoxCurtain	*pBoxCurtain;

	ID3DXFont	*pFont;
	ID3DXFont	*pLFont;
	ID3DXFont	*pLLFont;
	ID3DXFont	*pSFont;
	ID3DXFont	*pSSFont;

	Sound		*pMusic;
	Sound		*pSE;
}

using namespace OmakeData;

//
//Chara
//

Chara::Chara()
{
	int iChara = (iCharaListBeg + iCharaListSel) % TOTALCHARACOUNT;
	m_ID = GetCharaID(iChara, 0);
	m_iHeight = GetCharaHeight(iChara, 0);
	m_iWeight = GetCharaWeight(iChara, 0);
	m_iBust = GetCharaBust(iChara, 0);

	GetCharaName(m_szName, iChara, 0);
	GetCharaKata(m_szKata, iChara, 0);
	GetCharaTitle(m_szTitle, iChara, 0);
	GetCharaBirth(m_szBirth, iChara, 0);
	GetCharaBlood(m_szBlood, iChara, 0);
	GetCharaStage(m_szStage, iChara, 0);
	GetCharaDegree(m_szDegree, iChara, 0);

	SetRange(-128.0f, -288.0f, 128.0f, 0.0f);
	Init();
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(m_ID), 256, 256);
	AddTexRect(  0, 180,  32, 216);				 // 0  -- BW
	AddTexRect(  0,   0,  32,  36);				 // 1  -- D-1
	AddTexRect( 32,   0,  64,  36);				 // 2  -- D-2
	AddTexRect( 64,   0,  96,  36);				 // 3  -- D-3
	AddTexRect( 96,   0, 128,  36);				 // 4  -- D-4
	AddTexRect(128,   0, 160,  36);				 // 5  -- D-5
	AddTexRect(160,   0, 192,  36);				 // 6  -- D-6
	AddTexRect(192,   0, 224,  36);				 // 7  -- D-7
	AddTexRect(224,   0, 256,  36);				 // 8  -- D-8
	AddTexRect(  0,  36,  32,  72);				 // 9  -- U-1
	AddTexRect( 32,  36,  64,  72);				 // 10 -- U-2
	AddTexRect( 64,  36,  96,  72);				 // 11 -- U-3
	AddTexRect( 96,  36, 128,  72);				 // 12 -- U-4
	AddTexRect(128,  36, 160,  72);				 // 13 -- U-5
	AddTexRect(160,  36, 192,  72);				 // 14 -- U-6
	AddTexRect(192,  36, 224,  72);				 // 15 -- U-7
	AddTexRect(224,  36, 256,  72);				 // 16 -- U-8
	AddTexRect(  0,  72,  32, 108);				 // 17 -- L-1
	AddTexRect( 32,  72,  64, 108);				 // 18 -- L-2
	AddTexRect( 64,  72,  96, 108);				 // 19 -- L-3
	AddTexRect( 96,  72, 128, 108);				 // 20 -- L-4
	AddTexRect(128,  72, 160, 108);				 // 21 -- L-5
	AddTexRect(160,  72, 192, 108);				 // 22 -- L-6
	AddTexRect(192,  72, 224, 108);				 // 23 -- L-7
	AddTexRect(224,  72, 256, 108);				 // 24 -- L-8
	AddTexRect(  0, 108,  32, 144);				 // 25 -- R-1
	AddTexRect( 32, 108,  64, 144);				 // 26 -- R-2
	AddTexRect( 64, 108,  96, 144);				 // 27 -- R-3
	AddTexRect( 96, 108, 128, 144);				 // 28 -- R-4
	AddTexRect(128, 108, 160, 144);				 // 29 -- R-5
	AddTexRect(160, 108, 192, 144);				 // 30 -- R-6
	AddTexRect(192, 108, 224, 144);				 // 31 -- R-7
	AddTexRect(224, 108, 256, 144);				 // 32 -- R-8
	AddTexRect(  0, 144,  32, 180);				 // 33 -- E-1
	AddTexRect( 32, 144,  64, 180);				 // 34 -- E-2
	AddTexRect( 64, 144,  96, 180);				 // 35 -- E-3
	AddTexRect( 96, 144, 128, 180);				 // 36 -- E-4
	AddTexRect(128, 144, 160, 180);				 // 37 -- E-5
	AddTexRect(160, 144, 192, 180);				 // 38 -- E-6

	AddState(D_STAY, LOOP, fBaseSPB, 1, 1, 1, 1, 2, 2, 1, 8, 1, 1, 2, 2);
	AddState(U_STAY, LOOP, fBaseSPB, 9, 9, 9, 9, 10, 10, 9, 16, 9, 9, 10, 10);
	AddState(L_STAY, LOOP, fBaseSPB, 17, 17, 17, 17, 18, 18, 17, 24, 17, 17, 18, 18);
	AddState(R_STAY, LOOP, fBaseSPB, 25, 25, 25, 25, 26, 26, 25, 32, 25, 25, 27, 27);
	AddState(D_MOVE, LOOP, fBaseSPB / 4, 3, 1, 4, 1);
	AddState(U_MOVE, LOOP, fBaseSPB / 4, 11, 9, 12, 9);
	AddState(L_MOVE, LOOP, fBaseSPB / 4, 20, 19);
	AddState(R_MOVE, LOOP, fBaseSPB / 4, 28, 27);
	AddState(D_JUMP, SINGLE, 0.0f, 4);
	AddState(U_JUMP, SINGLE, 0.0f, 12);
	AddState(L_JUMP, SINGLE, 0.0f, 20);
	AddState(R_JUMP, SINGLE, 0.0f, 28);
	AddState(SMILE, SINGLE, 0.0f, 33);
	AddState(SHOCK, SINGLE, 0.0f, 34);
	AddState(CRY, LOOP, fBaseSPB, 35, 36);
	AddState(HAPPY, LOOP, fBaseSPB, 37, 38);

	SetState(D_STAY);
	SetPos(16.0f * fSpacing, 13.0f * fSpacing);

	m_pd3dDevice->CreateVertexBuffer(8 * sizeof(COLORVERTEX), 0, COLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertSplit, NULL);

	COLORVERTEX *pVertices;
	m_pVertSplit->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = { 14.0f * fSpacing, 3.5f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[1] = { 27.0f * fSpacing, 3.5f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[2] = { 14.0f * fSpacing, 8.5f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[3] = { 27.0f * fSpacing, 8.5f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[4] = { 22.0f * fSpacing, 4.0f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[5] = { 22.0f * fSpacing, 8.0f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[6] = { 18.5f * fSpacing, 9.0f * fSpacing, 0.1f*m_Layer, BLACK };
	pVertices[7] = { 18.5f * fSpacing, 15.0f * fSpacing, 0.1f*m_Layer, BLACK };
	m_pVertSplit->Unlock();
}

Chara::~Chara()
{
	SAFE_RELEASE(m_pVertSplit);
	DXObject::~DXObject();
}

BOOL Chara::Update(double fTime, float fElapsedTime)
{
	DXObject::Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL Chara::Render(double fTime, float fElapsedTime)
{
	DXObject::Render(fTime, fElapsedTime);

	RECT rcText;
	WCHAR szBuffer[MAX_PATH];

	m_pd3dDevice->SetStreamSource(0, m_pVertSplit, 0, sizeof(COLORVERTEX));
	m_pd3dDevice->SetFVF(COLORTEX_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 4);

	SetGridRect(&rcText, 14.0f, 2.0f, 27.0f, 3.0f, fActSpacing);
	pLLFont->DrawText(NULL, m_szTitle, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

	SetGridRect(&rcText, 14.0f, 4.2f, 22.0f, 5.2f, fActSpacing);
	pSFont->DrawText(NULL, m_szKata, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

	SetGridRect(&rcText, 14.0f, 5.0f, 22.0f, 6.0f, fActSpacing);
	pLLFont->DrawText(NULL, m_szName, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

	SetGridRect(&rcText, 14.0f, 6.5f, 22.0f, 8.0f, fActSpacing);
	pFont->DrawText(NULL, m_szDegree, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
	
	SetGridRect(&rcText, 23.0f, 4.0f, 27.0f, 4.8f, fActSpacing);
	pSFont->DrawText(NULL, L"生日", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
	SetGridRect(&rcText, 25.0f, 4.0f, 27.0f, 4.8f, fActSpacing);
	pSFont->DrawText(NULL, m_szBirth, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);

	m_iHeight == 0 ? swprintf_s(szBuffer, L"???cm") : swprintf_s(szBuffer, L"%dcm", m_iHeight);
	SetGridRect(&rcText, 23.0f, 4.8f, 27.0f, 5.6f, fActSpacing);
	pSFont->DrawText(NULL, L"身高", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
	SetGridRect(&rcText, 25.0f, 4.8f, 27.0f, 5.6f, fActSpacing);
	pSFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);

	m_iWeight == 0 ? swprintf_s(szBuffer, L"???kg") : swprintf_s(szBuffer, L"%dkg", m_iWeight);
	SetGridRect(&rcText, 23.0f, 5.6f, 27.0f, 6.4f, fActSpacing);
	pSFont->DrawText(NULL, L"体重", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
	SetGridRect(&rcText, 25.0f, 5.6f, 27.0f, 6.4f, fActSpacing);
	pSFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);

	m_iBust == 0 ? swprintf_s(szBuffer, L"???cm") : swprintf_s(szBuffer, L"%dcm", m_iBust);
	SetGridRect(&rcText, 23.0f, 6.4f, 27.0f, 7.2f, fActSpacing);
	pSFont->DrawText(NULL, L"胸围", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
	SetGridRect(&rcText, 25.0f, 6.4f, 27.0f, 7.2f, fActSpacing);
	pSFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);

	SetGridRect(&rcText, 23.0f, 7.2f, 27.0f, 8.0f, fActSpacing);
	pSFont->DrawText(NULL, L"血型", -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
	SetGridRect(&rcText, 25.0f, 7.2f, 27.0f, 8.0f, fActSpacing);
	pSFont->DrawText(NULL, m_szBlood, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);

	SetGridRect(&rcText, 19.0f, 8.6f, 27.0f, 15.0f, fActSpacing);
	pSSFont->DrawText(NULL, m_szStage, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

	SetGridRect(&rcText, 14.2f, 13.5f, 18.0f, 15.0f, fActSpacing);
	pSSFont->DrawText(NULL, L"方向键 - 改变方向\nZ/J 键 - 跳跃状态\n空格键 - 表情变换",-1, &rcText, DT_LEFT | DT_TOP, BLACK);

	return TRUE;
}

BOOL Chara::SetState(int iState)
{
	if (iState != m_iState)
	{
		DXObject::SetState(iState);
		return TRUE;
	}
	return FALSE;
}

int Chara::GetState()
{
	return m_iState;
}

//
//CharaList
//

CharaList::CharaList()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pd3dDevice->CreateVertexBuffer(12 * sizeof(TEXVERTEX), 0, TEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	TEXVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	
	pVertices[0] = { -64.0f,  64.0f, 0.0f, 0.0f,   0.140625f };
	pVertices[1] = { -64.0f, -80.0f, 0.0f, 0.0f,   0.0f };
	pVertices[2] = {  64.0f,  64.0f, 0.0f, 0.125f, 0.140625f };
	pVertices[3] = {  64.0f, -80.0f, 0.0f, 0.125f, 0.0f };
	pVertices[4] = { -64.0f,  64.0f, 0.0f, 0.125f, 0.140625f };
	pVertices[5] = { -64.0f, -80.0f, 0.0f, 0.125f, 0.0f };
	pVertices[6] = {  64.0f,  64.0f, 0.0f, 0.25f,  0.140625f };
	pVertices[7] = {  64.0f, -80.0f, 0.0f, 0.25f,  0.0f };
	pVertices[8] = { -64.0f,  64.0f, 0.0f, 0.0f,   0.84375f };
	pVertices[9] = { -64.0f, -80.0f, 0.0f, 0.0f,   0.703125f };
	pVertices[10] = { 64.0f,  64.0f, 0.0f, 0.125f, 0.84375f };
	pVertices[11] = { 64.0f, -80.0f, 0.0f, 0.125f, 0.703125f };

	m_pVertexBuffer->Unlock();

	m_iCharaListBeg = 0;
	for (int i = 0; i < CHARALISTCOUNT; i++)
	{
		m_iCharaID[i] = GetCharaID(i, 0);
		GetCharaName(m_szName[i], i, 0);
		GetCharaTitle(m_szTitle[i], i, 0);
		D3DXCreateTextureFromResource(m_pd3dDevice, GetCharaDLLHandle(), 
			MAKEINTRESOURCE(GetCharaID(i, 0)), &m_pTexture[i]);
	}

	m_timeUpdate = 0.0f;
}

CharaList::~CharaList()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
	SAFE_RELEASE(m_pVertexBuffer);
}

BOOL CharaList::Update(double fTime, float fElapsedTime)
{
	if (m_iCharaListBeg != iCharaListBeg)
	{
		m_iCharaListBeg = iCharaListBeg;
		for (int i = 0; i < CHARALISTCOUNT; i++)
		{
			int iChara = (m_iCharaListBeg + i) % TOTALCHARACOUNT;
			m_iCharaID[i] = GetCharaID(iChara, 0);
			GetCharaName(m_szName[i], iChara, 0);
			GetCharaTitle(m_szTitle[i], iChara, 0);
			SAFE_RELEASE(m_pTexture[i]);
			D3DXCreateTextureFromResource(m_pd3dDevice, GetCharaDLLHandle(),
				MAKEINTRESOURCE(GetCharaID(iChara, 0)), &m_pTexture[i]);
		}
	}

	m_timeUpdate += fElapsedTime;
	if (m_timeUpdate >= 4 * fBaseSPB)
		m_timeUpdate -= 6 * fBaseSPB;

	return TRUE;
}

BOOL CharaList::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
	m_pd3dDevice->SetFVF(TEX_FVF);

	for (int i = 0; i < 4; i++)
	{
		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matWorld, 24.8f * fSpacing, (5.5f + 2.5f * i)*fSpacing, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pd3dDevice->SetTexture(0, m_pTexture[i]);

		if (iBox == 1 && i == iCharaListSel)
		{
			if (m_timeUpdate >= 0.0f)
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			else
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
		}
		else
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

		RECT rcText;
		WCHAR szBuffer[MAX_PATH];
		SetGridRect(&rcText, 16.0f, (4.8f + 2.5f * i), 24.0f, (6.3f + 2.5f * i), fActSpacing);
		pFont->DrawText(NULL, m_szName[i], -1, &rcText, DT_LEFT | DT_TOP, BLACK);
		pFont->DrawText(NULL, m_szTitle[i], -1, &rcText, DT_LEFT | DT_BOTTOM, BLACK);
		swprintf_s(szBuffer, L"NO.%03d", m_iCharaID[i]);
		SetGridRect(&rcText, 22.0f, (4.8f + 2.5f * i), 24.0f, (6.3f + 2.5f * i), fActSpacing);
		pSFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
	}

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	RECT rcChara;
	SetGridRect(&rcChara, 16.0f, 2.0f, 25.0f, 3.0f, fActSpacing);
	pLLFont->DrawText(NULL, L"Character Box", -1, &rcChara, DT_CENTER | DT_VCENTER, BLACK);

	return TRUE;
}

//
//Cursor
//

LCursor::LCursor()
{
	SetRange(-4 * fSpacing, -fSpacing / 2, 4 * fSpacing, fSpacing / 2);
	Init();
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM4), 128, 128);
	AddTexRect( 0, 48, 128,  64);				 // 0
	AddTexRect( 0, 64, 128,  80);				 // 1
	AddTexRect( 0, 80, 128,  96);				 // 2
	AddState(0, LOOP, 500.0f, 1, 2);
	AddState(1, SINGLE, 0.0f, 0);
	AddState(2, SINGLE, 0.0f, 1);
	SetState(0);
}

LCursor::~LCursor()
{
	DXObject::~DXObject();
}

BOOL LCursor::Update(double fTime, float fElapsedTime)
{
	SetPos(7 * fSpacing, (4.5f + 1.5f * iMusicListSel) * fSpacing);
	int iMusic = (iMusicListBeg + iMusicListSel) % cMusic;
	if (iMusic == iPlayMusic)
	{
		if (bPlay)
			SetState(0);
		else
			SetState(1);
	}
	else
		SetState(2);

	DXObject::Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL LCursor::Render(double fTime, float fElapsedTime)
{
	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL LCursor::SetState(int iState)
{
	if (iState != m_iState)
	{
		DXObject::SetState(iState);
		return TRUE;
	}
	else return FALSE;
}

RCursor::RCursor()
{
	SetRange(-fSpacing, -fSpacing / 2, fSpacing, fSpacing / 2);
	Init();
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM4), 128, 128);
	AddTexRect( 0,  96, 32, 112);				 // 0
	AddTexRect( 0, 112, 32, 128);				 // 1
	AddTexRect(32,  96, 64, 112);				 // 2
	AddTexRect(32, 112, 64, 128);				 // 3
	AddTexRect(64,  96, 96, 112);				 // 4
	AddTexRect(64, 112, 96, 128);				 // 5
	AddTexRect(96,   0, 128, 16);				 // 6
	AddTexRect(96,  16, 128, 32);				 // 7
	AddState(0, SINGLE, 0.0f, 0);
	AddState(1, SINGLE, 0.0f, 1);
	AddState(2, SINGLE, 0.0f, 2);
	AddState(3, SINGLE, 0.0f, 3);
	AddState(4, SINGLE, 0.0f, 4);
	AddState(5, SINGLE, 0.0f, 5);
	AddState(6, SINGLE, 0.0f, 6);
	AddState(7, SINGLE, 0.0f, 7);
	SetState(0);

	m_fOffset = 0.0f;
}

RCursor::~RCursor()
{
	DXObject::~DXObject();
}

BOOL RCursor::Update(double fTime, float fElapsedTime)
{
	if (iBox == 1)
	{
		m_fOffset += 0.02f * fElapsedTime;
		if (m_fOffset >= 8.0f)
			m_fOffset -= 8.0f;
	}
	else
		m_fOffset = 0.0f;

	return TRUE;
}

BOOL RCursor::Render(double fTime, float fElapsedTime)
{
	SetPos(16 * fSpacing + m_fOffset, (4.5f + iCharaListSel * 2.5f) * fSpacing + m_fOffset);
	SetState(0);
	DXObject::Render(fTime, fElapsedTime);
	SetPos(16 * fSpacing + m_fOffset, (6.5f + iCharaListSel * 2.5f) * fSpacing - m_fOffset);
	SetState(1);
	DXObject::Render(fTime, fElapsedTime);
	SetPos(20.5f * fSpacing, (4.5f + iCharaListSel * 2.5f) * fSpacing + m_fOffset);
	SetState(2);
	DXObject::Render(fTime, fElapsedTime);
	SetPos(20.5f * fSpacing, (6.5f + iCharaListSel * 2.5f) * fSpacing - m_fOffset);
	SetState(3);
	DXObject::Render(fTime, fElapsedTime);
	SetPos(25 * fSpacing - m_fOffset, (4.5f + iCharaListSel * 2.5f) * fSpacing + m_fOffset);
	SetState(4);
	DXObject::Render(fTime, fElapsedTime);
	SetPos(25 * fSpacing - m_fOffset, (6.5f + iCharaListSel * 2.5f) * fSpacing - m_fOffset);
	SetState(5);
	DXObject::Render(fTime, fElapsedTime);
	if (iBox == 1)
	{
		SetPos(20.5f * fSpacing, 3.8f * fSpacing - m_fOffset);
		SetState(6);
		DXObject::Render(fTime, fElapsedTime);
		SetPos(20.5f * fSpacing, 14.7f * fSpacing + m_fOffset);
		SetState(7);
		DXObject::Render(fTime, fElapsedTime);
	}

	return TRUE;
}

//
//BoxCurtain
//

BoxCurtain::BoxCurtain()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pd3dDevice->CreateVertexBuffer(8 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	RHWCOLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { 136 * fScale, 986 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };
	pVertices[1] = { 136 * fScale,  94 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };
	pVertices[2] = { 760 * fScale, 986 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };
	pVertices[3] = { 760 * fScale,  94 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };

	pVertices[4] = { 852 * fScale,  986 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };
	pVertices[5] = { 852 * fScale,   94 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };
	pVertices[6] = { 1784 * fScale, 986 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };
	pVertices[7] = { 1784 * fScale,  94 * fScale, 0.0f, 1.0f, D3DCOLOR_ARGB(127,0,0,0) };

	m_pVertexBuffer->Unlock();

	m_iOn = 1;
}

BoxCurtain::~BoxCurtain()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

BOOL BoxCurtain::Render()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(RHWCOLORVERTEX));
	m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * m_iOn, 2);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

void BoxCurtain::SetLeft()
{
	m_iOn = 0;
}

void BoxCurtain::SetRight()
{
	m_iOn = 1;
}

//
//Scene
//

SceneOmake::SceneOmake()
{
	pBoxMusic	= NULL;
	pBoxChara	= NULL;
	pBoxCurtain = NULL;
	pLCursor	= NULL;
	pRCursor	= NULL;
	pCharaList	= NULL;
	pChara		= NULL;
	pMusic		= NULL;
	pSE			= NULL;
}

SceneOmake::~SceneOmake()
{
	End();
}

void SceneOmake::Init()
{
	fScale = GetSettings().m_fScale;
	fBaseSPB = 60000.0f / fBPM;
	fActSpacing = fSpacing * fScale;

	iBox = 0;

	bPlay = true;
	iMusicListBeg = 0;
	iMusicListSel = 0;
	iPlayMusic = 4;

	bDetail = false;
	iCharaListBeg = 0;
	iCharaListSel = 0;

	for (int i = 0; i < 6; i++)
	{
		bKeyUp[i] = false;
		bKeyUp[i] = 2 * fBaseSPB;
	}

	//Pointers
	pBackground = new Background();

	pBoxMusic = new Box();
	pBoxChara = new Box();
	pBoxMusic->Set(448, 540, 256, 390, 1.0f);
	pBoxChara->Set(1318, 540, 410, 390, 1.0f);
	
	pCharaList = new CharaList();

	pLCursor = new LCursor();
	pRCursor = new RCursor();

	pBoxCurtain = new BoxCurtain();

	pMusic = new Sound();
	pMusic->Register(ID_MUSICBOX, ID_SUBMIX_BACKGROUND);
	pMusic->AddSoundFromRsrc(GetSoundDLLHandle(), iMusicID[iPlayMusic], L"MP3");
	pMusic->Play(0);

	pSE = new Sound();
	pSE->Register(ID_SE_GALLERY, ID_SUBMIX_BACKGROUND);
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CURSOR, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SELECT, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_DECIDE, L"WAVE");
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CANCEL, L"WAVE");

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(36.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(40.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(48.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLLFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(28.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSSFont);
}

void SceneOmake::End()
{
	SAFE_DELETE(pBoxMusic);
	SAFE_DELETE(pBoxChara);
	SAFE_DELETE(pLCursor);
	SAFE_DELETE(pRCursor);
	SAFE_DELETE(pCharaList);
	SAFE_DELETE(pChara);
	SAFE_DELETE(pBoxCurtain);

	SAFE_DELETE(pSE);
	SAFE_DELETE(pMusic);

	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pLFont);
	SAFE_RELEASE(pLLFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pSSFont);
}

void SceneOmake::OnResetDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pLFont);
	SAFE_RELEASE(pLLFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pSSFont);

	D3DXCreateFont(DXUTGetD3DDevice(), (int)(36.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(40.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(48.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLLFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(28.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSSFont);
}

void SceneOmake::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pLFont);
	SAFE_RELEASE(pLLFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pSSFont);
}

void SceneOmake::Update(double fTime, float fElapsedTime)
{
	GetInput();
	
	for (int i = 0; i < 7; i++)
	{
		if (fKeyUp[i] <= 0.0f)
			fKeyUp[i] = 0.0f;
		else
			fKeyUp[i] -= fElapsedTime;
	}

	if (iBox == 0)
	{
		if (IsKeyDown(DIK_TAB))
		{
			if (bKeyUp[6] || fKeyUp[6] == 0.0f)
			{
				pSE->Play(0);
				pBoxCurtain->SetLeft();
				iBox = 1;

				fKeyUp[6] = fBaseSPB / 4;
				if (bKeyUp[6])
					fKeyUp[6] = fBaseSPB;
			}
			bKeyUp[6] = false;
		}
		else
			bKeyUp[6] = true;

		if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
		{
			if (bKeyUp[3])
			{
				pSE->Play(0);
				pBoxCurtain->SetLeft();
				iBox = 1;
			}
			bKeyUp[3] = false;
		}
		else
			bKeyUp[3] = true;;

		if (!IsCurtainOn())
		{
			if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
			{
				if (bKeyUp[5])
				{
					pSE->Play(3);
					SetCurtain(0, 255, 500.0f, OpenSelectMenu);
				}
				bKeyUp[5] = false;
			}
			else
				bKeyUp[5] = true;
		}

		if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
		{
			if (bKeyUp[0] || fKeyUp[0] == 0.0f)
			{
				pSE->Play(1);
				if (iMusicListSel == 0)
				{
					if (iMusicListBeg == 0)
						iMusicListBeg = cMusic - 1;
					else
						iMusicListBeg--;
				}
				else
					iMusicListSel--;
				
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
				pSE->Play(1);
				if (iMusicListSel == 4)
				{
					if (iMusicListBeg == cMusic - 1)
						iMusicListBeg = 0;
					else
						iMusicListBeg++;
				}
				else
					iMusicListSel++;

				fKeyUp[1] = fBaseSPB / 4;
				if (bKeyUp[1])
					fKeyUp[1] = fBaseSPB;
			}
			bKeyUp[1] = false;
		}
		else
			bKeyUp[1] = true;

		if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
		{
			if (bKeyUp[4])
			{
				pSE->Play(1);
				int iMusic = (iMusicListBeg + iMusicListSel) % cMusic;
				if (iMusic == iPlayMusic)
				{
					if (bPlay)
						pMusic->Stop();
					else
						pMusic->Play(-1);
					bPlay = !bPlay;
				}
				else
				{
					if (NULL != pMusic) pMusic->End();
					SAFE_DELETE(pMusic);
					iPlayMusic = iMusic;
					pMusic = new Sound();
					pMusic->Register(ID_MUSICBOX, ID_SUBMIX_BACKGROUND);
					pMusic->AddSoundFromRsrc(GetSoundDLLHandle(), iMusicID[iPlayMusic], L"MP3");
					pMusic->Play(0);
					bPlay = true;
				}
			}
			bKeyUp[4] = false;
		}
		else
			bKeyUp[4] = true;
	}

	if (iBox == 1)
	{
		if (IsKeyDown(DIK_TAB))
		{
			if (bKeyUp[6] || fKeyUp[6] == 0.0f)
			{
				pSE->Play(0);
				pBoxCurtain->SetRight();
				iBox = 0;

				fKeyUp[6] = fBaseSPB / 4;
				if (bKeyUp[6])
					fKeyUp[6] = fBaseSPB;
			}
			bKeyUp[6] = false;
		}
		else
			bKeyUp[6] = true;

		if (!bDetail)
		{
			if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
			{
				if (bKeyUp[2])
				{
					pSE->Play(0);
					pBoxCurtain->SetRight();
					iBox = 0;
				}
				bKeyUp[2] = false;
			}
			else
				bKeyUp[2] = true;

			if (!IsCurtainOn())
			{
				if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
				{
					if (bKeyUp[5])
					{
						pSE->Play(3);
						SetCurtain(0, 255, 500.0f, OpenSelectMenu);
					}
					bKeyUp[5] = false;
				}
				else
					bKeyUp[5] = true;
			}

			if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
			{
				if (bKeyUp[0] || fKeyUp[0] == 0.0f)
				{
					pSE->Play(1);
					if (iCharaListSel == 0)
					{
						if (iCharaListBeg == 0)
							iCharaListBeg = TOTALCHARACOUNT - 1;
						else
							iCharaListBeg--;
					}
					else
						iCharaListSel--;

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
					pSE->Play(1);
					if (iCharaListSel == 3)
					{
						if (iCharaListBeg == TOTALCHARACOUNT - 1)
							iCharaListBeg = 0;
						else
							iCharaListBeg++;
					}
					else
						iCharaListSel++;

					fKeyUp[1] = fBaseSPB / 4;
					if (bKeyUp[1])
						fKeyUp[1] = fBaseSPB;
				}
				bKeyUp[1] = false;
			}
			else
				bKeyUp[1] = true;

			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (bKeyUp[4])
				{
					pSE->Play(2);
					bDetail = true;
					pChara = new Chara();
				}
				bKeyUp[4] = false;
			}
			else
				bKeyUp[4] = true;
		}
		else
		{
			int iCharaState = pChara->GetState();

			if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
				pChara->SetState(U_MOVE);
			else if (iCharaState == U_MOVE)
				pChara->SetState(U_STAY);
			
			if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
				pChara->SetState(D_MOVE);
			else if (iCharaState == D_MOVE)
				pChara->SetState(D_STAY);

			if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
				pChara->SetState(L_MOVE);
			else if (iCharaState == L_MOVE)
				pChara->SetState(L_STAY);

			if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
				pChara->SetState(R_MOVE);
			else if (iCharaState == R_MOVE)
				pChara->SetState(R_STAY);

			iCharaState = pChara->GetState();

			if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_J))
			{
				switch (iCharaState)
				{
				case U_STAY:
				case U_MOVE:
				case U_JUMP:
					pChara->SetState(U_JUMP);
					break;

				case D_STAY:
				case D_MOVE:
				case D_JUMP:
					pChara->SetState(D_JUMP);
					break;

				case L_STAY:
				case L_MOVE:
				case L_JUMP:
					pChara->SetState(L_JUMP);
					break;

				case R_STAY:
				case R_MOVE:
				case R_JUMP:
					pChara->SetState(R_JUMP);
					break;

				default:
					pChara->SetState(D_JUMP);
					break;
				}
			}
			else
			{
				switch (iCharaState)
				{
				case U_JUMP:
					pChara->SetState(U_STAY);
					break;

				case D_JUMP:
					pChara->SetState(D_STAY);
					break;

				case L_JUMP:
					pChara->SetState(L_STAY);
					break;

				case R_JUMP:
					pChara->SetState(R_STAY);
					break;
				}
			}

			static bool bSpaceUp = false;
			if (IsKeyDown(DIK_SPACE))
			{
				if (bSpaceUp)
				{
					switch (iCharaState)
					{
					case SMILE:
						pChara->SetState(SHOCK);
						break;

					case SHOCK:
						pChara->SetState(CRY);
						break;

					case CRY:
						pChara->SetState(HAPPY);
						break;

					default:
						pChara->SetState(SMILE);
						break;
					}
				}
				bSpaceUp = false;
			}
			else
				bSpaceUp = true;

			if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
			{
				if (bKeyUp[5])
				{
					pSE->Play(3);
					bDetail = false;
					if (NULL != pChara)
					{
						pChara->~Chara();
						SAFE_DELETE(pChara);
					}
				}
				bKeyUp[5] = false;
			}
			else
				bKeyUp[5] = true;
		}
	}


	//Pointers

	pLCursor->Update(fTime, fElapsedTime);

	if (bDetail)
	{
		pChara->Update(fTime, fElapsedTime);
	}
	else
	{
		pRCursor->Update(fTime, fElapsedTime);
		pCharaList->Update(fTime, fElapsedTime);
	}

	pBackground->Update(fTime, fElapsedTime);
}

void SceneOmake::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	SetView();
	SetProj(NULL, GAME_WIDTH, GAME_HEIGHT);
	SetViewPort();

	pBackground->Render(fTime, fElapsedTime);

	//Music Box

	pBoxMusic->Render(fElapsedTime);

	RECT rcTitle;		
	SetGridRect(&rcTitle, 4.0f, 2.0, 10.0f, 3.0f, fActSpacing);
	pLLFont->DrawText(NULL, L"Music Box", -1, &rcTitle, DT_CENTER | DT_VCENTER, BLACK);
	for (int i = 0; i < 5; i++)
	{
		int iMusic = (iMusicListBeg + i) % cMusic;
		SetGridRect(&rcTitle, 4.0f, (4.0f + 1.5f * i), 10.0f, (5.0f + 1.5f * i), fActSpacing);
		pSFont->DrawText(NULL, szMusic[iMusic], -1, &rcTitle, DT_LEFT | DT_VCENTER, BLACK);
	}

	SetGridRect(&rcTitle, 4.0f, 12.0f, 10.0f, 13.0f, fActSpacing);
	pLFont->DrawText(NULL, L"当前播放：", -1, &rcTitle, DT_LEFT | DT_VCENTER, BLACK);
	
	SetGridRect(&rcTitle, 4.0f, 13.0f, 10.0f, 14.0f, fActSpacing);
	if (-1 != iPlayMusic)
		pSFont->DrawText(NULL, szMusic[iPlayMusic], -1, &rcTitle, DT_LEFT | DT_VCENTER, BLACK);

	pLCursor->Render(fTime, fElapsedTime);

	//Chara Box

	pBoxChara->Render(fElapsedTime);

	if (bDetail)
	{
		pChara->Render(fTime, fElapsedTime);
	}
	else
	{
		pCharaList->Render(fTime, fElapsedTime);
		pRCursor->Render(fTime, fElapsedTime);
	}

	pBoxCurtain->Render();
}
