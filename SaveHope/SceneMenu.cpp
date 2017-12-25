 #include "SceneUtil.h"
#include "SceneMenu.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "GameAudio.h"
#include "Settings.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace MenuClass;

namespace MenuData
{
	IDirect3DDevice9 *pd3dDevice = NULL;

	IDirect3DTexture9 *pTexLogo = NULL;
	IDirect3DTexture9 *pTexMenu = NULL;
	float fTexScale = 1.0f;

	IDirect3DVertexBuffer9 *pVertBG = NULL;

	IDirect3DVertexBuffer9 *pVertLogo = NULL;
	D3DXVECTOR2 LogoPos[3];
	float fRollTitle;
	float fScaleTitle = 1.0;
	IDirect3DVertexBuffer9 *pVertMenu = NULL;
	D3DXVECTOR2 MenuPos[10];

	Chara *pChara1 = NULL;
	Chara *pChara2 = NULL;
	float fTimeChara;
	int cChara = CHARACOUNT - 2;
	bool bNewChara = true;

	Sound *pBG = NULL;

	float fWidth = (float)GAME_WIDTH;
	float fHeight = (float)GAME_HEIGHT;

	float fMenuX = 128.0f;
	float fMenuY = 128.0f;
	float fDelta = -1.0f;

	float fSpacing = 64.0f;

	bool bResize = false;
	bool bReady = false;
	bool bZoomIn = false;
	bool bZoomOut = false;
	bool bBoardChange = false;
	bool bMenuChange = false;

	int iUnlock = 0;
}

using namespace MenuData;

//
//Chara
//

Chara::Chara(int iChara)
{
	m_ID = GetCharaID(iChara, 1);
	if (m_ID == IDC_HINATA)
	{
		if (rand() % 5 == 0)
		{
			if (rand() % 2 == 0)
				m_ID = IDC_KAMUKURA;
			else
				m_ID = IDC_HINATASUPER;
		}
	}
	SetRange(-64, -144, 64, 0);
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(m_ID), 256, 256);
	Init();
	AddTexRect( 64,  72,  96, 108);				 // 0 -- L-3
	AddTexRect( 96,  72, 128, 108);				 // 1 -- L-4
	AddTexRect( 64, 108,  96, 144);				 // 2 -- R-3
	AddTexRect( 96, 108, 128, 144);				 // 3 -- R-4
	AddState(L_MOVE, LOOP, 160.0f, 0, 1);
	AddState(R_MOVE, LOOP, 160.0f, 2, 3);
	AddState(L_JUMP, SINGLE, 0.0f, 1);
	AddState(R_JUMP, SINGLE, 0.0f, 3);

	SetPos(fWidth / 16 + (fWidth * 3 / 16 * (rand() % 1000) / 1000), 0.0f);

	m_fSpeed = m_fMaxSpeed = 0.6f;
	m_fAccel = 0.0028f;
	m_fJumpSpeed = 0.8f;

	SetState(R_JUMP);
	m_bMove = false;
	m_bOut = false;
}

Chara::~Chara()
{
	DXObject::~DXObject();
}

BOOL Chara::Update(double fTime, float fElapsedTime)
{
	switch (m_iState)
	{
	case R_JUMP:
		m_ObjPos.y += fElapsedTime * m_fJumpSpeed;
		if (m_ObjPos.y >= fHeight)
		{
			m_ObjPos.y = fHeight;
			SetState(R_MOVE);
		}
		break;

	case R_MOVE:
		if (m_ObjPos.x >= fWidth / 3)
		{
			if (m_bMove)
			{				
				m_fSpeed = min(m_fMaxSpeed, m_fSpeed + fElapsedTime * m_fAccel);
				m_ObjPos.x += fElapsedTime * m_fSpeed;
			}
			else
			{
				m_ObjPos.x = fWidth / 3;
				m_fSpeed = 0.0f;
			}
		}
		else if (m_ObjPos.x >= fWidth / 3 - 64.0f)
		{
			if (!m_bMove)
			{
				m_fSpeed = max(0.001f, m_fSpeed - fElapsedTime * m_fAccel);
			}
			m_ObjPos.x += fElapsedTime * m_fSpeed;
		}
		else
		{
			m_ObjPos.x += fElapsedTime * m_fSpeed;
		}
		if (m_ObjPos.x > fWidth + 128.0f)
			m_bOut = true;
		break;
	}

	DXObject::Update(fTime, fElapsedTime);
	return TRUE;
}

BOOL Chara::Render(double fTime, float fElapsedTime)
{
	DXObject::Render(fTime, fElapsedTime);
	return TRUE;
}

void Chara::Move()
{
	m_bMove = true;
}

bool Chara::IsOut()
{
	return m_bOut;
}

//
//Scene
//

SceneMenu::SceneMenu()
{

}

SceneMenu::~SceneMenu()
{
	End();
}

void SceneMenu::Init()
{
	pd3dDevice = DXUTGetD3DDevice();
	
	int iResolution, iWindowed, iRsrc;

	iResolution = GetSettings().m_iResolution;
	iWindowed = GetSettings().m_iWindowed;

	if (iResolution == 1 || iResolution == 2 || iResolution == 3)
		iRsrc = iResolution;
	else
	{
		if (iWindowed == 1 || iWindowed == 2)
			iRsrc = iWindowed;
		else
			iRsrc = 3;
	}

	D3DXCreateTextureFromResource(pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO0 + iRsrc), &pTexLogo);
	D3DXCreateTextureFromResource(pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU0 + iRsrc), &pTexMenu);

	//Black and White Backgrounds

	{
		pd3dDevice->CreateVertexBuffer(8 * sizeof(COLORVERTEX), 0, COLOR_FVF,
			D3DPOOL_MANAGED, &pVertBG, NULL);
		COLORVERTEX *pVertices;
		pVertBG->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0] = { 0.0f,       fHeight, 0.0f, WHITE };
		pVertices[1] = { 0.0f,          0.0f, 0.0f, WHITE };
		pVertices[2] = { fWidth / 2, fHeight, 0.0f, WHITE };
		pVertices[3] = { fWidth / 2,    0.0f, 0.0f, WHITE };
		pVertices[4] = { fWidth / 2, fHeight, 0.0f, BLACK };
		pVertices[5] = { fWidth / 2,    0.0f, 0.0f, BLACK };
		pVertices[6] = { fWidth,     fHeight, 0.0f, BLACK };
		pVertices[7] = { fWidth,        0.0f, 0.0f, BLACK };

		pVertBG->Unlock();
	}

	//Logo etc.

	{
		pd3dDevice->CreateVertexBuffer(12 * sizeof(TEXVERTEX), 0, TEX_FVF,
			D3DPOOL_MANAGED, &pVertLogo, NULL);
		TEXVERTEX *pVertices;
		pVertLogo->Lock(0, 0, (void**)&pVertices, 0);

		float x = fWidth / 2;
		float y = fHeight / 3;
		fScaleTitle = 1.0;

		pVertices[0] = { -256.0f,  160.0f, 0.0f, 0.0f * fTexScale, 0.625f * fTexScale };
		pVertices[1] = { -256.0f, -160.0f, 0.0f, 0.0f * fTexScale,   0.0f * fTexScale };
		pVertices[2] = {  256.0f,  160.0f, 0.0f, 1.0f * fTexScale, 0.625f * fTexScale };
		pVertices[3] = {  256.0f, -160.0f, 0.0f, 1.0f * fTexScale,   0.0f * fTexScale };
		LogoPos[0] = D3DXVECTOR2(x + 320.0f, y);

		pVertices[4] = { -128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.0f * fTexScale,   1.0f * fTexScale };
		pVertices[5] = { -128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.0f * fTexScale, 0.625f * fTexScale };
		pVertices[6] = {  128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.5f * fTexScale,   1.0f * fTexScale };
		pVertices[7] = {  128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.5f * fTexScale, 0.625f * fTexScale };
		LogoPos[1] = D3DXVECTOR2(x - 320.0f, y);
		
		pVertices[8] = { -128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.5f * fTexScale,   1.0f * fTexScale };
		pVertices[9] = { -128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.5f * fTexScale, 0.625f * fTexScale };
		pVertices[10] = { 128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 1.0f * fTexScale,   1.0f * fTexScale };
		pVertices[11] = { 128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 1.0f * fTexScale, 0.625f * fTexScale };
		LogoPos[2] = D3DXVECTOR2(x - 320.0f, y);
		fRollTitle = -D3DX_PI / 32;

		pVertLogo->Unlock();
	}

	//MenuMenu

	{
		pd3dDevice->CreateVertexBuffer(40 * sizeof(TEXVERTEX), 0, TEX_FVF,
			D3DPOOL_MANAGED, &pVertMenu, NULL);
		TEXVERTEX *pVertices;
		pVertMenu->Lock(0, 0, (void**)&pVertices, 0);
		
		float fBegX = 128.0f;
		float fBegY = 0.0f;

		float x = fWidth / 2;
		float y = fHeight * 2 / 3 + 32.0f;

		pVertices[0]  = { -64.0f,    0.0f, 0.0f,   0.0f * fTexScale, 0.25f * fTexScale };
		pVertices[1]  = { -64.0f,  -64.0f, 0.0f,   0.0f * fTexScale,  0.0f * fTexScale };
		pVertices[2]  = {   0.0f,    0.0f, 0.0f, 0.125f * fTexScale, 0.25f * fTexScale };
		pVertices[3]  = {   0.0f,  -64.0f, 0.0f, 0.125f * fTexScale,  0.0f * fTexScale };
		pVertices[4]  = { -fBegX,    0.0f, 0.0f, 0.125f * fTexScale, 0.25f * fTexScale };
		pVertices[5]  = { -fBegX,  -64.0f, 0.0f, 0.125f * fTexScale,  0.0f * fTexScale };
		pVertices[6]  = {  fBegX,    0.0f, 0.0f,  0.25f * fTexScale, 0.25f * fTexScale };
		pVertices[7]  = {  fBegX,  -64.0f, 0.0f,  0.25f * fTexScale,  0.0f * fTexScale };
		pVertices[8]  = {   0.0f,    0.0f, 0.0f,  0.25f * fTexScale, 0.25f * fTexScale };
		pVertices[9]  = {   0.0f,  -64.0f, 0.0f,  0.25f * fTexScale,  0.0f * fTexScale };
		pVertices[10] = {  64.0f,    0.0f, 0.0f, 0.375f * fTexScale, 0.25f * fTexScale };
		pVertices[11] = {  64.0f,  -64.0f, 0.0f, 0.375f * fTexScale,  0.0f * fTexScale };
		MenuPos[0] = D3DXVECTOR2(x - fBegX, y - fBegY);
		MenuPos[1] = D3DXVECTOR2(x        , y - fBegY);
		MenuPos[2] = D3DXVECTOR2(x + fBegX, y - fBegY);

		pVertices[12] = { -64.0f,  fBegY, 0.0f,   0.0f * fTexScale,  0.5f * fTexScale };
		pVertices[13] = { -64.0f, -fBegY, 0.0f,   0.0f * fTexScale, 0.25f * fTexScale };
		pVertices[14] = {   0.0f,  fBegY, 0.0f, 0.125f * fTexScale,  0.5f * fTexScale };
		pVertices[15] = {   0.0f, -fBegY, 0.0f, 0.125f * fTexScale, 0.25f * fTexScale };
		pVertices[16] = { -fBegX,  fBegY, 0.0f, 0.125f * fTexScale,  0.5f * fTexScale };
		pVertices[17] = { -fBegX, -fBegY, 0.0f, 0.125f * fTexScale, 0.25f * fTexScale };
		pVertices[18] = {  fBegX,  fBegY, 0.0f,  0.25f * fTexScale,  0.5f * fTexScale };
		pVertices[19] = {  fBegX, -fBegY, 0.0f,  0.25f * fTexScale, 0.25f * fTexScale };
		pVertices[20] = {   0.0f,  fBegY, 0.0f,  0.25f * fTexScale,  0.5f * fTexScale };
		pVertices[21] = {   0.0f, -fBegY, 0.0f,  0.25f * fTexScale, 0.25f * fTexScale };
		pVertices[22] = {  64.0f,  fBegY, 0.0f, 0.375f * fTexScale,  0.5f * fTexScale };
		pVertices[23] = {  64.0f, -fBegY, 0.0f, 0.375f * fTexScale, 0.25f * fTexScale };
		MenuPos[3] = D3DXVECTOR2(x - fBegX, y);
		MenuPos[4] = D3DXVECTOR2(x        , y);
		MenuPos[5] = D3DXVECTOR2(x + fBegX, y);

		pVertices[24] = { -64.0f,   64.0f, 0.0f,   0.0f * fTexScale, 0.75f * fTexScale };
		pVertices[25] = { -64.0f,    0.0f, 0.0f,   0.0f * fTexScale,  0.5f * fTexScale };
		pVertices[26] = {   0.0f,   64.0f, 0.0f, 0.125f * fTexScale, 0.75f * fTexScale };
		pVertices[27] = {   0.0f,    0.0f, 0.0f, 0.125f * fTexScale,  0.5f * fTexScale };
		pVertices[28] = { -fBegX,   64.0f, 0.0f, 0.125f * fTexScale, 0.75f * fTexScale };
		pVertices[29] = { -fBegX,    0.0f, 0.0f, 0.125f * fTexScale,  0.5f * fTexScale };
		pVertices[30] = {  fBegX,   64.0f, 0.0f,  0.25f * fTexScale, 0.75f * fTexScale };
		pVertices[31] = {  fBegX,    0.0f, 0.0f,  0.25f * fTexScale,  0.5f * fTexScale };
		pVertices[32] = {   0.0f,   64.0f, 0.0f,  0.25f * fTexScale, 0.75f * fTexScale };
		pVertices[33] = {   0.0f,    0.0f, 0.0f,  0.25f * fTexScale,  0.5f * fTexScale };
		pVertices[34] = {  64.0f,   64.0f, 0.0f, 0.375f * fTexScale, 0.75f * fTexScale };
		pVertices[35] = {  64.0f,    0.0f, 0.0f, 0.375f * fTexScale,  0.5f * fTexScale };
		MenuPos[6] = D3DXVECTOR2(x - fBegX, y + fBegY);
		MenuPos[7] = D3DXVECTOR2(x        , y + fBegY);
		MenuPos[8] = D3DXVECTOR2(x + fBegX, y + fBegY);

		pVertices[36] = {   0.0f,   52.0f, 0.0f, 0.375f * fTexScale, 0.75f * fTexScale };
		pVertices[37] = {   0.0f, -140.0f, 0.0f, 0.375f * fTexScale,  0.0f * fTexScale };
		pVertices[38] = { 384.0f,   52.0f, 0.0f,   1.0f * fTexScale, 0.75f * fTexScale };
		pVertices[39] = { 384.0f, -140.0f, 0.0f,   1.0f * fTexScale,  0.0f * fTexScale };
		MenuPos[9] = D3DXVECTOR2(x + fBegX + 64.0f, y);

		pVertMenu->Unlock();
	}

	fTimeChara = 5000.0f;

	pBG = new Sound();
	pBG->Register(ID_BGM_MENU, ID_SUBMIX_BACKGROUND);
	pBG->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_PREPARE, L"MP3", 255);
	pBG->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_GET, L"MP3");
	pBG->Play(0); 
	
	iUnlock = 0;
}

void SceneMenu::End()
{
	SAFE_DELETE(pBG);
	SAFE_DELETE(pChara1);
	SAFE_DELETE(pChara2);
	SAFE_RELEASE(pVertBG);
	SAFE_RELEASE(pVertLogo);
	SAFE_RELEASE(pVertMenu);
	SAFE_RELEASE(pTexLogo);
	SAFE_RELEASE(pTexMenu);
}

void SceneMenu::OnResetDevice()
{
	SAFE_RELEASE(pTexLogo);
	SAFE_RELEASE(pTexMenu);

	int iResolution, iWindowed, iRsrc;

	iResolution = GetSettings().m_iResolution;
	iWindowed = GetSettings().m_iWindowed;

	if (iResolution == 1 || iResolution == 2 || iResolution == 3)
		iRsrc = iResolution;
	else
	{
		if (iWindowed == 1 || iWindowed == 2)
			iRsrc = iWindowed;
		else
			iRsrc = 3;
	}

	D3DXCreateTextureFromResource(pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO0 + iRsrc), &pTexLogo);
	D3DXCreateTextureFromResource(pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU0 + iRsrc), &pTexMenu);
}

void SceneMenu::OnLostDevice()
{
	SAFE_RELEASE(pTexLogo);
	SAFE_RELEASE(pTexMenu);
}

void SceneMenu::Update(double fTime, float fElapsedTime)
{
	//11037
	GetInput();
	
	static bool bKeyUp = false;
	
	if (IsKeyDown())
	{
		if (bKeyUp)
		{
			if (IsKeyDown(DIK_1) || IsKeyDown(DIK_NUMPAD1))
			{
				if (iUnlock == 0 || iUnlock == 1)
					iUnlock++;
				else
					iUnlock = 0;
			}
			else if (IsKeyDown(DIK_0) || IsKeyDown(DIK_NUMPAD0))
			{
				if (iUnlock == 2)
					iUnlock++;
				else
					iUnlock = 0;
			}
			else if (IsKeyDown(DIK_3) || IsKeyDown(DIK_NUMPAD3))
			{
				if (iUnlock == 3)
					iUnlock++;
				else
					iUnlock = 0;
			}
			else if (IsKeyDown(DIK_7) || IsKeyDown(DIK_NUMPAD7))
			{
				if (iUnlock == 4)
					iUnlock++;
				else
					iUnlock = 0;
			}
			else
				iUnlock = 0;
		}
		bKeyUp = false;
	}
	else
		bKeyUp = true;

	if (iUnlock == 5 && !IsDebugMode())
	{
		iUnlock = 6;
		pBG->Play(1);
		SetDebugMode(true);
	}

	//Title

	static float fRollDelta = 0.0f;
	fRollTitle += fElapsedTime * fRollDelta;
	fRollDelta -= fElapsedTime * 0.000002f;
	if (fRollTitle <= -2 * D3DX_PI / 5)
	{
		fRollTitle = -2 * D3DX_PI / 5;
		fRollDelta = -fRollDelta;
	}
	if (fRollTitle > -D3DX_PI / 32)
	{
		fRollTitle = -D3DX_PI / 32;
		fRollDelta = 0.0f;
	}

	//MenuBox

	if (!bZoomIn && !bZoomOut)
	{
		if (bResize)
		{
			float x = fWidth / 2;
			float y = fHeight * 2 / 3 + 32.0f;

			float fNewY = y - MenuPos[0].y + fElapsedTime * fDelta;

			if (fDelta < 0.0f && fNewY <= 0.0f)
			{
				fNewY = 0.0f;
				fDelta = -fDelta;
			}

			if (fDelta > 0.0f && fNewY >= fMenuY)
			{
				fNewY = fMenuY;
				bResize = false;
				bReady = true;
			}

			TEXVERTEX *pVertices;
			pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

			MenuPos[0] = D3DXVECTOR2(x - fMenuX, y - fNewY);
			MenuPos[1] = D3DXVECTOR2(x         , y - fNewY);
			MenuPos[2] = D3DXVECTOR2(x + fMenuX, y - fNewY);

			pVertices[12].y =  fNewY;
			pVertices[13].y = -fNewY;
			pVertices[14].y =  fNewY;
			pVertices[15].y = -fNewY;
			pVertices[16].y =  fNewY;
			pVertices[17].y = -fNewY;
			pVertices[18].y =  fNewY;
			pVertices[19].y = -fNewY;
			pVertices[20].y =  fNewY;
			pVertices[21].y = -fNewY;
			pVertices[22].y =  fNewY;
			pVertices[23].y = -fNewY;

			MenuPos[6] = D3DXVECTOR2(x - fMenuX, y + fNewY);
			MenuPos[7] = D3DXVECTOR2(x         , y + fNewY);
			MenuPos[8] = D3DXVECTOR2(x + fMenuX, y + fNewY);

			pVertMenu->Unlock();
		}
	}

	if (bZoomIn)
	{
		if (!bMenuChange)
		{
			if (!bBoardChange)
			{
				//MenuBox
				{
					float x = fWidth / 2;
					float y = fHeight * 2 / 3 + 32.0f;

					float fNewX = x - MenuPos[0].x;
					float fNewY = y - MenuPos[0].y;

					fNewY = max(0.0f, fNewY + fElapsedTime * fDelta);
					if (fNewY == 0.0f)
						fNewX = max(0.0f, fNewX + fElapsedTime * fDelta);

					TEXVERTEX *pVertices;
					pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[4] = { -fNewX,    0.0f, 0.0f, 0.125f, 0.25f };
					pVertices[5] = { -fNewX,  -64.0f, 0.0f, 0.125f,  0.0f };
					pVertices[6] = {  fNewX,    0.0f, 0.0f,  0.25f, 0.25f };
					pVertices[7] = {  fNewX,  -64.0f, 0.0f,  0.25f,  0.0f };
					MenuPos[0] = D3DXVECTOR2(x - fNewX, y - fNewY);
					MenuPos[1] = D3DXVECTOR2(x        , y - fNewY);
					MenuPos[2] = D3DXVECTOR2(x + fNewX, y - fNewY);

					pVertices[12] = { -64.0f,  fNewY, 0.0f,   0.0f,  0.5f };
					pVertices[13] = { -64.0f, -fNewY, 0.0f,   0.0f, 0.25f };
					pVertices[14] = { 0.0f,    fNewY, 0.0f, 0.125f,  0.5f };
					pVertices[15] = { 0.0f,   -fNewY, 0.0f, 0.125f, 0.25f };
					pVertices[16] = { -fNewX,  fNewY, 0.0f, 0.125f,  0.5f };
					pVertices[17] = { -fNewX, -fNewY, 0.0f, 0.125f, 0.25f };
					pVertices[18] = {  fNewX,  fNewY, 0.0f,  0.25f,  0.5f };
					pVertices[19] = {  fNewX, -fNewY, 0.0f,  0.25f, 0.25f };
					pVertices[20] = { 0.0f,    fNewY, 0.0f,  0.25f,  0.5f };
					pVertices[21] = { 0.0f,   -fNewY, 0.0f,  0.25f, 0.25f };
					pVertices[22] = { 64.0f,   fNewY, 0.0f, 0.375f,  0.5f };
					pVertices[23] = { 64.0f,  -fNewY, 0.0f, 0.375f, 0.25f };
					MenuPos[3] = D3DXVECTOR2(x - fNewX, y);
					MenuPos[4] = D3DXVECTOR2(x        , y);
					MenuPos[5] = D3DXVECTOR2(x + fNewX, y);

					pVertices[28] = { -fNewX,  64.0f, 0.0f, 0.125f, 0.75f };
					pVertices[29] = { -fNewX,   0.0f, 0.0f, 0.125f,  0.5f };
					pVertices[30] = {  fNewX,  64.0f, 0.0f,  0.25f, 0.75f };
					pVertices[31] = {  fNewX,   0.0f, 0.0f,  0.25f,  0.5f };
					MenuPos[6] = D3DXVECTOR2(x - fNewX, y + fNewY);
					MenuPos[7] = D3DXVECTOR2(x        , y + fNewY);
					MenuPos[8] = D3DXVECTOR2(x + fNewX, y + fNewY);

					MenuPos[9] = D3DXVECTOR2(x + fNewX + 64.0f, y);

					pVertMenu->Unlock();
				}

				//Title and Mouth
				{
					float x = fWidth / 2;
					float y = fHeight / 3;

					fScaleTitle = max(0.0f, fScaleTitle + fElapsedTime * 0.002f * fDelta);

					TEXVERTEX *pVertices;

					pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[36] = { 0.0f * fScaleTitle,     52.0f * fScaleTitle, 0.0f, 0.375f, 0.75f };
					pVertices[37] = { 0.0f * fScaleTitle,   -140.0f * fScaleTitle, 0.0f, 0.375f,  0.0f };
					pVertices[38] = { 384.0f * fScaleTitle,   52.0f * fScaleTitle, 0.0f,   1.0f, 0.75f };
					pVertices[39] = { 384.0f * fScaleTitle, -140.0f * fScaleTitle, 0.0f,   1.0f,  0.0f };

					pVertMenu->Unlock();

					pVertLogo->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[4] = { -128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.0f, 1.0f };
					pVertices[5] = { -128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.0f, 0.625f };
					pVertices[6] = { 128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.5f, 1.0f };
					pVertices[7] = { 128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.5f, 0.625f };

					pVertices[8] = { -128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.5f, 1.0f };
					pVertices[9] = { -128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.5f, 0.625f };
					pVertices[10] = { 128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 1.0f, 1.0f };
					pVertices[11] = { 128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 1.0f, 0.625f };

					pVertLogo->Unlock();
				}

				if (fScaleTitle == 0.0f && MenuPos[0].x == fWidth / 2)
					bBoardChange = true;
			}
			else
			{
				float x = fWidth / 2;
				float y = fHeight / 3;

				float xDelta = LogoPos[0].x - x - 320.0f - fElapsedTime * fDelta;
				if (xDelta >= 320.0f)
				{
					xDelta = 320.0f;
					bMenuChange = true;
				}

				COLORVERTEX *pVertices;

				pVertBG->Lock(0, 0, (void**)&pVertices, 0);

				pVertices[0] = { 0.0f, fHeight, 0.0f, WHITE };
				pVertices[1] = { 0.0f,    0.0f, 0.0f, WHITE };
				pVertices[2] = { fWidth / 2 + xDelta, fHeight, 0.0f, WHITE };
				pVertices[3] = { fWidth / 2 + xDelta,    0.0f, 0.0f, WHITE };
				pVertices[4] = { fWidth / 2 + xDelta, fHeight, 0.0f, BLACK };
				pVertices[5] = { fWidth / 2 + xDelta,    0.0f, 0.0f, BLACK };
				pVertices[6] = { fWidth, fHeight, 0.0f, BLACK };
				pVertices[7] = { fWidth,    0.0f, 0.0f, BLACK };

				pVertBG->Unlock();

				LogoPos[0] = D3DXVECTOR2(x + 320.0f + xDelta, y);

				y = fHeight * 2 / 3 + 32.0f;
				MenuPos[0] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[1] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[2] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[3] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[4] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[5] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[6] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[7] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[8] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[9] = D3DXVECTOR2(x + xDelta + 64.0f, y);
			}
		}
		else
		{
			float x = fWidth / 2 + 320.0f;
			float y = fHeight * 2 / 3 + 32.0f;

			float fNewX = MenuPos[1].x - x;
			float fNewY = y - MenuPos[1].y;

			fNewY = min(fMenuY, fNewY - fElapsedTime * fDelta);
			if (fNewY == fMenuY)
				fNewX = min(fMenuX, fNewX - fElapsedTime * fDelta);

			if (fNewX == fMenuX)
			{
				bResize = false;
				bZoomIn = false;
				bReady = true;
			}

			TEXVERTEX *pVertices;
			pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

			pVertices[4] = { -fNewX,    0.0f, 0.0f, 0.125f, 0.25f };
			pVertices[5] = { -fNewX,  -64.0f, 0.0f, 0.125f,  0.0f };
			pVertices[6] = {  fNewX,    0.0f, 0.0f,  0.25f, 0.25f };
			pVertices[7] = {  fNewX,  -64.0f, 0.0f,  0.25f,  0.0f };
			MenuPos[0] = D3DXVECTOR2(x            , y - fNewY);
			MenuPos[1] = D3DXVECTOR2(x + fNewX    , y - fNewY);
			MenuPos[2] = D3DXVECTOR2(x + 2 * fNewX, y - fNewY);

			pVertices[12] = { -64.0f,  fNewY, 0.0f,   0.0f,  0.5f };
			pVertices[13] = { -64.0f, -fNewY, 0.0f,   0.0f, 0.25f };
			pVertices[14] = {   0.0f,  fNewY, 0.0f, 0.125f,  0.5f };
			pVertices[15] = {   0.0f, -fNewY, 0.0f, 0.125f, 0.25f };
			pVertices[16] = { -fNewX,  fNewY, 0.0f, 0.125f,  0.5f };
			pVertices[17] = { -fNewX, -fNewY, 0.0f, 0.125f, 0.25f };
			pVertices[18] = {  fNewX,  fNewY, 0.0f,  0.25f,  0.5f };
			pVertices[19] = {  fNewX, -fNewY, 0.0f,  0.25f, 0.25f };
			pVertices[20] = {   0.0f,  fNewY, 0.0f,  0.25f,  0.5f };
			pVertices[21] = {   0.0f, -fNewY, 0.0f,  0.25f, 0.25f };
			pVertices[22] = { 64.0f,  fNewY, 0.0f, 0.375f,  0.5f };
			pVertices[23] = { 64.0f, -fNewY, 0.0f, 0.375f, 0.25f };
			MenuPos[3] = D3DXVECTOR2(x            , y);
			MenuPos[4] = D3DXVECTOR2(x + fNewX    , y);
			MenuPos[5] = D3DXVECTOR2(x + 2 * fNewX, y);

			pVertices[28] = { -fNewX,  64.0f, 0.0f, 0.125f, 0.75f };
			pVertices[29] = { -fNewX,   0.0f, 0.0f, 0.125f,  0.5f };
			pVertices[30] = {  fNewX,  64.0f, 0.0f,  0.25f, 0.75f };
			pVertices[31] = {  fNewX,   0.0f, 0.0f,  0.25f,  0.5f };
			MenuPos[6] = D3DXVECTOR2(x            , y + fNewY);
			MenuPos[7] = D3DXVECTOR2(x + fNewX    , y + fNewY);
			MenuPos[8] = D3DXVECTOR2(x + 2 * fNewX, y + fNewY);

			MenuPos[9] = D3DXVECTOR2(x + 2 * fNewX + 64.0f, y);

			pVertMenu->Unlock();
		}
	}

	if (bZoomOut)
	{
		if (bMenuChange)
		{
			float x = fWidth / 2 + 320.0f;
			float y = fHeight * 2 / 3 + 32.0f;

			float fNewX = MenuPos[1].x - x;
			float fNewY = y - MenuPos[1].y;

			fNewY = max(0.0f, fNewY - fElapsedTime * fDelta);
			if (fNewY == 0.0f)
				fNewX = max(0.0f, fNewX - fElapsedTime * fDelta);
			if (fNewX == 0.0f)
				bMenuChange = false;

			TEXVERTEX *pVertices;
			pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

			pVertices[4] = { -fNewX,    0.0f, 0.0f, 0.125f, 0.25f };
			pVertices[5] = { -fNewX,  -64.0f, 0.0f, 0.125f,  0.0f };
			pVertices[6] = {  fNewX,    0.0f, 0.0f,  0.25f, 0.25f };
			pVertices[7] = {  fNewX,  -64.0f, 0.0f,  0.25f,  0.0f };
			MenuPos[0] = D3DXVECTOR2(x            , y - fNewY);
			MenuPos[1] = D3DXVECTOR2(x + fNewX    , y - fNewY);
			MenuPos[2] = D3DXVECTOR2(x + 2 * fNewX, y - fNewY);

			pVertices[12] = { -64.0f,  fNewY, 0.0f,   0.0f,  0.5f };
			pVertices[13] = { -64.0f, -fNewY, 0.0f,   0.0f, 0.25f };
			pVertices[14] = { 0.0f,  fNewY, 0.0f, 0.125f,  0.5f };
			pVertices[15] = { 0.0f, -fNewY, 0.0f, 0.125f, 0.25f };
			pVertices[16] = { -fNewX,  fNewY, 0.0f, 0.125f,  0.5f };
			pVertices[17] = { -fNewX, -fNewY, 0.0f, 0.125f, 0.25f };
			pVertices[18] = {  fNewX,  fNewY, 0.0f,  0.25f,  0.5f };
			pVertices[19] = {  fNewX, -fNewY, 0.0f,  0.25f, 0.25f };
			pVertices[20] = { 0.0f,  fNewY, 0.0f,  0.25f,  0.5f };
			pVertices[21] = { 0.0f, -fNewY, 0.0f,  0.25f, 0.25f };
			pVertices[22] = { 64.0f,  fNewY, 0.0f, 0.375f,  0.5f };
			pVertices[23] = { 64.0f, -fNewY, 0.0f, 0.375f, 0.25f };
			MenuPos[3] = D3DXVECTOR2(x            , y);
			MenuPos[4] = D3DXVECTOR2(x + fNewX    , y);
			MenuPos[5] = D3DXVECTOR2(x + 2 * fNewX, y);

			pVertices[28] = { -fNewX,  64.0f, 0.0f, 0.125f, 0.75f };
			pVertices[29] = { -fNewX,   0.0f, 0.0f, 0.125f,  0.5f };
			pVertices[30] = {  fNewX,  64.0f, 0.0f,  0.25f, 0.75f };
			pVertices[31] = {  fNewX,   0.0f, 0.0f,  0.25f,  0.5f };
			MenuPos[6] = D3DXVECTOR2(x            , y + fNewY);
			MenuPos[7] = D3DXVECTOR2(x + fNewX    , y + fNewY);
			MenuPos[8] = D3DXVECTOR2(x + 2 * fNewX, y + fNewY);

			MenuPos[9] = D3DXVECTOR2(x + 2 * fNewX + 64.0f, y);

			pVertMenu->Unlock();
		}
		else
		{
			if (bBoardChange)
			{
				float x = fWidth / 2;
				float y = fHeight / 3;

				float xDelta = LogoPos[0].x - x - 320.0f - fElapsedTime * fDelta;
				if (xDelta <= 0.0f)
				{
					xDelta = 0.0f;
					bBoardChange = false;
				}

				COLORVERTEX *pVertices;

				pVertBG->Lock(0, 0, (void**)&pVertices, 0);

				pVertices[0] = { 0.0f, fHeight, 0.0f, WHITE };
				pVertices[1] = { 0.0f,    0.0f, 0.0f, WHITE };
				pVertices[2] = { fWidth / 2 + xDelta, fHeight, 0.0f, WHITE };
				pVertices[3] = { fWidth / 2 + xDelta,    0.0f, 0.0f, WHITE };
				pVertices[4] = { fWidth / 2 + xDelta, fHeight, 0.0f, BLACK };
				pVertices[5] = { fWidth / 2 + xDelta,    0.0f, 0.0f, BLACK };
				pVertices[6] = { fWidth, fHeight, 0.0f, BLACK };
				pVertices[7] = { fWidth,    0.0f, 0.0f, BLACK };

				pVertBG->Unlock();

				LogoPos[0] = D3DXVECTOR2(x + 320.0f + xDelta, y);

				y = fHeight * 2 / 3 + 32.0f;
				MenuPos[0] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[1] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[2] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[3] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[4] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[5] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[6] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[7] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[8] = D3DXVECTOR2(x + xDelta, y);
				MenuPos[9] = D3DXVECTOR2(x + xDelta + 64.0f, y);
			}
			else
			{
				//MenuBox
				{
					float x = fWidth / 2;
					float y = fHeight * 2 / 3 + 32.0f;

					float fNewX = x - MenuPos[0].x;
					float fNewY = y - MenuPos[0].y;
					
					fNewX = min(fMenuX, fNewX + fElapsedTime * fDelta);
					if (fNewX == fMenuX)
						fNewY = min(fMenuY, fNewY + fElapsedTime * fDelta);

					TEXVERTEX *pVertices;
					pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[4] = { -fNewX,    0.0f, 0.0f, 0.125f, 0.25f };
					pVertices[5] = { -fNewX,  -64.0f, 0.0f, 0.125f,  0.0f };
					pVertices[6] = { +fNewX,    0.0f, 0.0f,  0.25f, 0.25f };
					pVertices[7] = { +fNewX,  -64.0f, 0.0f,  0.25f,  0.0f };
					MenuPos[0] = D3DXVECTOR2(x - fNewX, y - fNewY);
					MenuPos[1] = D3DXVECTOR2(x, y - fNewY);
					MenuPos[2] = D3DXVECTOR2(x + fNewX, y - fNewY);

					pVertices[12] = { -64.0f,  fNewY, 0.0f,   0.0f,  0.5f };
					pVertices[13] = { -64.0f, -fNewY, 0.0f,   0.0f, 0.25f };
					pVertices[14] = { 0.0f,  fNewY, 0.0f, 0.125f,  0.5f };
					pVertices[15] = { 0.0f, -fNewY, 0.0f, 0.125f, 0.25f };
					pVertices[16] = { -fNewX,  fNewY, 0.0f, 0.125f,  0.5f };
					pVertices[17] = { -fNewX, -fNewY, 0.0f, 0.125f, 0.25f };
					pVertices[18] = { +fNewX,  fNewY, 0.0f,  0.25f,  0.5f };
					pVertices[19] = { +fNewX, -fNewY, 0.0f,  0.25f, 0.25f };
					pVertices[20] = { 0.0f,  fNewY, 0.0f,  0.25f,  0.5f };
					pVertices[21] = { 0.0f, -fNewY, 0.0f,  0.25f, 0.25f };
					pVertices[22] = { 64.0f,  fNewY, 0.0f, 0.375f,  0.5f };
					pVertices[23] = { 64.0f, -fNewY, 0.0f, 0.375f, 0.25f };
					MenuPos[3] = D3DXVECTOR2(x - fNewX, y);
					MenuPos[4] = D3DXVECTOR2(x, y);
					MenuPos[5] = D3DXVECTOR2(x + fNewX, y);

					pVertices[28] = { -fNewX,  64.0f, 0.0f, 0.125f, 0.75f };
					pVertices[29] = { -fNewX,   0.0f, 0.0f, 0.125f,  0.5f };
					pVertices[30] = { +fNewX,  64.0f, 0.0f,  0.25f, 0.75f };
					pVertices[31] = { +fNewX,   0.0f, 0.0f,  0.25f,  0.5f };
					MenuPos[6] = D3DXVECTOR2(x - fNewX, y + fNewY);
					MenuPos[7] = D3DXVECTOR2(x, y + fNewY);
					MenuPos[8] = D3DXVECTOR2(x + fNewX, y + fNewY);

					MenuPos[9] = D3DXVECTOR2(x + fNewX + 64.0f, y);

					pVertMenu->Unlock();
				}

				//Title
				{
					float x = fWidth / 2;
					float y = fHeight / 3;

					fScaleTitle = min(1.0f, fScaleTitle + fElapsedTime * 0.002f * fDelta);

					TEXVERTEX *pVertices;

					pVertMenu->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[36] = { 0.0f * fScaleTitle,     52.0f * fScaleTitle, 0.0f, 0.375f, 0.75f };
					pVertices[37] = { 0.0f * fScaleTitle,   -140.0f * fScaleTitle, 0.0f, 0.375f,  0.0f };
					pVertices[38] = { 384.0f * fScaleTitle,   52.0f * fScaleTitle, 0.0f,   1.0f, 0.75f };
					pVertices[39] = { 384.0f * fScaleTitle, -140.0f * fScaleTitle, 0.0f,   1.0f,  0.0f };

					pVertMenu->Unlock();

					pVertLogo->Lock(0, 0, (void**)&pVertices, 0);

					pVertices[4] = { -128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.0f, 1.0f };
					pVertices[5] = { -128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.0f, 0.625f };
					pVertices[6] = { 128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.5f, 1.0f };
					pVertices[7] = { 128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.5f, 0.625f };

					pVertices[8] = { -128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 0.5f, 1.0f };
					pVertices[9] = { -128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 0.5f, 0.625f };
					pVertices[10] = { 128.0f * fScaleTitle,  96.0f * fScaleTitle, 0.0f, 1.0f, 1.0f };
					pVertices[11] = { 128.0f * fScaleTitle, -96.0f * fScaleTitle, 0.0f, 1.0f, 0.625f };

					pVertLogo->Unlock();
				}

				if (fScaleTitle == 1.0f && MenuPos[0].x == fWidth / 2 - fMenuX)
				{
					bZoomOut = false;
					bNewChara = true;
					bReady = true;
				}
			}
		}
	}

	//Chara

	static int iNewChara = 0;
	fTimeChara += fElapsedTime;

	if (bNewChara)
	{
		if (fTimeChara >= 5000.0f)
		{
			if (NULL == pChara1)
			{
				pChara1 = new Chara(iNewChara);
				if (pChara2) pChara2->Move();
				iNewChara = (iNewChara + 1 + rand() % (cChara - 1)) % cChara;
			}
		}
		if (fTimeChara >= 10000.0f)
		{
			if (NULL == pChara2)
			{
				pChara2 = new Chara(iNewChara);
				if (pChara1) pChara1->Move();
				iNewChara = (iNewChara + 1 + rand() % (cChara - 1)) % cChara;
			}
			else
			{
				pChara2->Move();
			}
			fTimeChara -= 10000.0f;
		}
	}
	else
	{
		if (pChara1) pChara1->Move();
		if (pChara2) pChara2->Move();
		fTimeChara = 5000.0f;
	}

	if (pChara1)
	{
		pChara1->Update(fTime, fElapsedTime);
		if (pChara1->IsOut())
			SAFE_DELETE(pChara1);
	}
	if (pChara2)
	{
		pChara2->Update(fTime, fElapsedTime);
		if (pChara2->IsOut())
			SAFE_DELETE(pChara2);
	}
}

void SceneMenu::Render(double fTime, float fElapsedTime)
{
	float fCameraX = fWidth / 2;
	float fCameraY = fHeight / 2;

	SetCameraPos(fCameraX, fCameraY);
	SetView();
	SetProj(NULL, fWidth, fHeight);
	SetViewPort();

	pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pd3dDevice->SetTexture(0, NULL);

	//Board

	pd3dDevice->SetStreamSource(0, pVertBG, 0, sizeof(COLORVERTEX));
	pd3dDevice->SetFVF(COLOR_FVF);
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	//Logo

	D3DXMATRIX matWorld;

	pd3dDevice->SetTexture(0, pTexLogo);
	pd3dDevice->SetStreamSource(0, pVertLogo, 0, sizeof(TEXVERTEX));
	pd3dDevice->SetFVF(TEX_FVF);

	for (int i = 0; i < 2; i++)
	{
		D3DXMatrixTranslation(&matWorld, LogoPos[i].x, LogoPos[i].y, 0.0f);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);
	}

	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	D3DXMatrixTranslation(&matWorld, LogoPos[2].x, LogoPos[2].y, 0.0f);
	D3DXMATRIX matRoll;
	D3DXMatrixRotationZ(&matRoll, fRollTitle);
	matWorld = matRoll * matWorld;
	pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);

	pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	//Menu

	pd3dDevice->SetTexture(0, pTexMenu);
	pd3dDevice->SetStreamSource(0, pVertMenu, 0, sizeof(TEXVERTEX));
	pd3dDevice->SetFVF(TEX_FVF);
	for (int i = 0; i < 10; i++)
	{
		D3DXMatrixTranslation(&matWorld, MenuPos[i].x, MenuPos[i].y, 0.0f);
		pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	pd3dDevice->SetTexture(0, NULL);

	pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	if (pChara1)
		pChara1->Render(fTime, fElapsedTime);
	if (pChara2)
		pChara2->Render(fTime, fElapsedTime);
}

void SetMenuSize(float fX, float fY)
{
	fMenuX = fX;
	fMenuY = fY;
	fDelta = -1.0f;
	bResize = true;
	bReady = false;
}

void MenuZoomIn(float fX, float fY)
{
	fMenuX = fX;
	fMenuY = fY;
	fDelta = -1.0f;
	bZoomIn = true;
	bZoomOut = false;
	bReady = false;
	bNewChara = false;
}

void MenuZoomOut(float fX, float fY)
{
	fMenuX = fX;
	fMenuY = fY;
	fDelta = 1.0f;
	bZoomIn = false;
	bZoomOut = true;
	bReady = false;
}

void AccelResize(float fMul)
{
	fDelta *= fMul;
}

bool IsMenuReady()
{
	return bReady;
}