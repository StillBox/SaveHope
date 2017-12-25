#include "SceneOP.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "SaveData.h"

using namespace OPClass;

namespace OPData
{
	const float fSpacing = 12.0f;

	float		fScale;
	float		fActSpacing;

	bool		bSceneOver;

	BackBoard	*pBackBoard;
	Block		*pBlock;
	Tail		*pTail;
}

using namespace OPData;

//
//Board
//

BackBoard::BackBoard()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(COLORVERTEX), 0, COLORTEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	COLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { 0.0f,		 GAME_HEIGHT, 0.0f, WHITE };
	pVertices[1] = { 0.0f,		 0.0f,		  0.0f, WHITE };
	pVertices[2] = { GAME_WIDTH, GAME_HEIGHT, 0.0f, WHITE };
	pVertices[3] = { GAME_WIDTH, 0.0f,		  0.0f, WHITE };

	m_pVertexBuffer->Unlock();	

	m_bOver = false;
	m_bUpdate = false;
	m_timeUpdate = 0.0f;
}

BackBoard::~BackBoard()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

BOOL BackBoard::Update(float fElapsedTime)
{
	GetInput();

	if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_X) || 
		IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_ESCAPE))
		m_bUpdate = true;

	if (m_bUpdate)
	{
		m_timeUpdate += fElapsedTime;

		D3DXCOLOR color = WHITE;
		color.a = 1.0f - m_timeUpdate / 1000.0f;

		COLORVERTEX *pVertices;
		m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0] = { 0.0f,		 GAME_HEIGHT, 0.0f, color };
		pVertices[1] = { 0.0f,		 0.0f,		  0.0f, color };
		pVertices[2] = { GAME_WIDTH, GAME_HEIGHT, 0.0f, color };
		pVertices[3] = { GAME_WIDTH, 0.0f,		  0.0f, color };

		m_pVertexBuffer->Unlock();
	}

	if (m_timeUpdate >= 1000.0f)
		m_bOver = true;

	return TRUE;
}

BOOL BackBoard::Render(float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(COLORVERTEX));
	m_pd3dDevice->SetFVF(COLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
	return TRUE;
}

bool BackBoard::IsOver()
{
	return m_bOver;
}

//
//Block
//

Block::Block()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pd3dDevice->CreateVertexBuffer(18 * 4 * sizeof(COLORVERTEX), 0, COLORTEX_FVF, 
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	m_fSize = fSpacing - (fScale == 1.0f ? 2.0f : 1.0f) / fScale;
	m_timeShow = 1500.0f;

	for (int i = 0; i < 18; i++)
	{
		D3DXCOLOR color = DD_GRAY;
		color.a = 15.0f * i / 256;

		COLORVERTEX *pVertices;
		m_pVertexBuffer->Lock(4 * i * sizeof(COLORVERTEX), 0, (void**)&pVertices, 0);

		pVertices[0] = { 0.0f,		m_fSize,	0.0f, color };
		pVertices[1] = { 0.0f,		0.0f,		0.0f, color };
		pVertices[2] = { m_fSize,	m_fSize,	0.0f, color };
		pVertices[3] = { m_fSize,	0.0f,		0.0f, color };

		m_pVertexBuffer->Unlock();
	}

	int xLB, yLB;

	//S

	xLB = 50;
	yLB = 45;
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 2 });
	m_vecBlock.push_back({ xLB + 0,yLB - 6 });
	m_vecBlock.push_back({ xLB + 0,yLB - 7 });
	m_vecBlock.push_back({ xLB + 0,yLB - 8 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 6 });
	m_vecBlock.push_back({ xLB + 1,yLB - 7 });
	m_vecBlock.push_back({ xLB + 1,yLB - 8 });
	m_vecBlock.push_back({ xLB + 1,yLB - 9 });
	m_vecBlock.push_back({ xLB + 2,yLB - 0 });
	m_vecBlock.push_back({ xLB + 2,yLB - 5 });
	m_vecBlock.push_back({ xLB + 2,yLB - 9 });
	m_vecBlock.push_back({ xLB + 3,yLB - 0 });
	m_vecBlock.push_back({ xLB + 3,yLB - 4 });
	m_vecBlock.push_back({ xLB + 3,yLB - 5 });
	m_vecBlock.push_back({ xLB + 3,yLB - 9 });
	m_vecBlock.push_back({ xLB + 4,yLB - 0 });
	m_vecBlock.push_back({ xLB + 4,yLB - 4 });
	m_vecBlock.push_back({ xLB + 4,yLB - 9 });
	m_vecBlock.push_back({ xLB + 5,yLB - 0 });
	m_vecBlock.push_back({ xLB + 5,yLB - 1 });
	m_vecBlock.push_back({ xLB + 5,yLB - 2 });
	m_vecBlock.push_back({ xLB + 5,yLB - 3 });
	m_vecBlock.push_back({ xLB + 5,yLB - 4 });
	m_vecBlock.push_back({ xLB + 5,yLB - 7 });
	m_vecBlock.push_back({ xLB + 5,yLB - 8 });
	m_vecBlock.push_back({ xLB + 5,yLB - 9 });
	m_vecBlock.push_back({ xLB + 6,yLB - 1 });
	m_vecBlock.push_back({ xLB + 6,yLB - 2 });
	m_vecBlock.push_back({ xLB + 6,yLB - 3 });
	m_vecBlock.push_back({ xLB + 6,yLB - 7 });
	m_vecBlock.push_back({ xLB + 6,yLB - 8 });

	//t

	xLB = 59;
	m_vecBlock.push_back({ xLB + 0,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 3 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 6 });
	m_vecBlock.push_back({ xLB + 1,yLB - 7 });
	m_vecBlock.push_back({ xLB + 2,yLB - 0 });
	m_vecBlock.push_back({ xLB + 2,yLB - 1 });
	m_vecBlock.push_back({ xLB + 2,yLB - 2 });
	m_vecBlock.push_back({ xLB + 2,yLB - 3 });
	m_vecBlock.push_back({ xLB + 2,yLB - 4 });
	m_vecBlock.push_back({ xLB + 2,yLB - 5 });
	m_vecBlock.push_back({ xLB + 2,yLB - 6 });
	m_vecBlock.push_back({ xLB + 2,yLB - 7 });
	m_vecBlock.push_back({ xLB + 3,yLB - 0 });
	m_vecBlock.push_back({ xLB + 3,yLB - 5 });
	m_vecBlock.push_back({ xLB + 4,yLB - 0 });
	m_vecBlock.push_back({ xLB + 4,yLB - 5 });

	//i

	xLB = 66;
	m_vecBlock.push_back({ xLB + 0,yLB - 0 });
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 2 });
	m_vecBlock.push_back({ xLB + 0,yLB - 3 });
	m_vecBlock.push_back({ xLB + 0,yLB - 4 });
	m_vecBlock.push_back({ xLB + 0,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 3 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });

	//l

	xLB = 71;
	m_vecBlock.push_back({ xLB + 0,yLB - 0 });
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 2 });
	m_vecBlock.push_back({ xLB + 0,yLB - 3 });
	m_vecBlock.push_back({ xLB + 0,yLB - 4 });
	m_vecBlock.push_back({ xLB + 0,yLB - 5 });
	m_vecBlock.push_back({ xLB + 0,yLB - 6 });
	m_vecBlock.push_back({ xLB + 0,yLB - 7 });
	m_vecBlock.push_back({ xLB + 0,yLB - 8 });
	m_vecBlock.push_back({ xLB + 0,yLB - 9 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 3 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 6 });
	m_vecBlock.push_back({ xLB + 1,yLB - 7 });
	m_vecBlock.push_back({ xLB + 1,yLB - 8 });
	m_vecBlock.push_back({ xLB + 1,yLB - 9 });

	//l

	xLB = 76;
	m_vecBlock.push_back({ xLB + 0,yLB - 0 });
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 2 });
	m_vecBlock.push_back({ xLB + 0,yLB - 3 });
	m_vecBlock.push_back({ xLB + 0,yLB - 4 });
	m_vecBlock.push_back({ xLB + 0,yLB - 5 });
	m_vecBlock.push_back({ xLB + 0,yLB - 6 });
	m_vecBlock.push_back({ xLB + 0,yLB - 7 });
	m_vecBlock.push_back({ xLB + 0,yLB - 8 });
	m_vecBlock.push_back({ xLB + 0,yLB - 9 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 3 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 6 });
	m_vecBlock.push_back({ xLB + 1,yLB - 7 });
	m_vecBlock.push_back({ xLB + 1,yLB - 8 });
	m_vecBlock.push_back({ xLB + 1,yLB - 9 });

	//B

	xLB = 84;
	m_vecBlock.push_back({ xLB + 0,yLB - 0 });
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 2 });
	m_vecBlock.push_back({ xLB + 0,yLB - 3 });
	m_vecBlock.push_back({ xLB + 0,yLB - 4 });
	m_vecBlock.push_back({ xLB + 0,yLB - 5 });
	m_vecBlock.push_back({ xLB + 0,yLB - 6 });
	m_vecBlock.push_back({ xLB + 0,yLB - 7 });
	m_vecBlock.push_back({ xLB + 0,yLB - 8 });
	m_vecBlock.push_back({ xLB + 0,yLB - 9 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 3 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 6 });
	m_vecBlock.push_back({ xLB + 1,yLB - 7 });
	m_vecBlock.push_back({ xLB + 1,yLB - 8 });
	m_vecBlock.push_back({ xLB + 1,yLB - 9 });
	m_vecBlock.push_back({ xLB + 2,yLB - 0 });
	m_vecBlock.push_back({ xLB + 2,yLB - 5 });
	m_vecBlock.push_back({ xLB + 2,yLB - 9 });
	m_vecBlock.push_back({ xLB + 3,yLB - 0 });
	m_vecBlock.push_back({ xLB + 3,yLB - 5 });
	m_vecBlock.push_back({ xLB + 3,yLB - 9 });
	m_vecBlock.push_back({ xLB + 4,yLB - 0 });
	m_vecBlock.push_back({ xLB + 4,yLB - 5 });
	m_vecBlock.push_back({ xLB + 4,yLB - 9 });
	m_vecBlock.push_back({ xLB + 5,yLB - 0 });
	m_vecBlock.push_back({ xLB + 5,yLB - 1 });
	m_vecBlock.push_back({ xLB + 5,yLB - 2 });
	m_vecBlock.push_back({ xLB + 5,yLB - 3 });
	m_vecBlock.push_back({ xLB + 5,yLB - 4 });
	m_vecBlock.push_back({ xLB + 5,yLB - 5 });
	m_vecBlock.push_back({ xLB + 5,yLB - 6 });
	m_vecBlock.push_back({ xLB + 5,yLB - 7 });
	m_vecBlock.push_back({ xLB + 5,yLB - 8 });
	m_vecBlock.push_back({ xLB + 5,yLB - 9 });
	m_vecBlock.push_back({ xLB + 6,yLB - 1 });
	m_vecBlock.push_back({ xLB + 6,yLB - 2 });
	m_vecBlock.push_back({ xLB + 6,yLB - 3 });
	m_vecBlock.push_back({ xLB + 6,yLB - 4 });
	m_vecBlock.push_back({ xLB + 6,yLB - 6 });
	m_vecBlock.push_back({ xLB + 6,yLB - 7 });
	m_vecBlock.push_back({ xLB + 6,yLB - 8 });

	//o

	xLB = 93;
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 2 });
	m_vecBlock.push_back({ xLB + 0,yLB - 3 });
	m_vecBlock.push_back({ xLB + 0,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 2 });
	m_vecBlock.push_back({ xLB + 1,yLB - 3 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 2,yLB - 0 });
	m_vecBlock.push_back({ xLB + 2,yLB - 5 });
	m_vecBlock.push_back({ xLB + 3,yLB - 0 });
	m_vecBlock.push_back({ xLB + 3,yLB - 5 });
	m_vecBlock.push_back({ xLB + 4,yLB - 0 });
	m_vecBlock.push_back({ xLB + 4,yLB - 1 });
	m_vecBlock.push_back({ xLB + 4,yLB - 2 });
	m_vecBlock.push_back({ xLB + 4,yLB - 3 });
	m_vecBlock.push_back({ xLB + 4,yLB - 4 });
	m_vecBlock.push_back({ xLB + 4,yLB - 5 });
	m_vecBlock.push_back({ xLB + 5,yLB - 1 });
	m_vecBlock.push_back({ xLB + 5,yLB - 2 });
	m_vecBlock.push_back({ xLB + 5,yLB - 3 });
	m_vecBlock.push_back({ xLB + 5,yLB - 4 });

	//x

	xLB = 101;
	m_vecBlock.push_back({ xLB + 0,yLB - 0 });
	m_vecBlock.push_back({ xLB + 0,yLB - 1 });
	m_vecBlock.push_back({ xLB + 0,yLB - 4 });
	m_vecBlock.push_back({ xLB + 0,yLB - 5 });
	m_vecBlock.push_back({ xLB + 1,yLB - 0 });
	m_vecBlock.push_back({ xLB + 1,yLB - 1 });
	m_vecBlock.push_back({ xLB + 1,yLB - 4 });
	m_vecBlock.push_back({ xLB + 1,yLB - 5 });
	m_vecBlock.push_back({ xLB + 2,yLB - 1 });
	m_vecBlock.push_back({ xLB + 2,yLB - 2 });
	m_vecBlock.push_back({ xLB + 2,yLB - 3 });
	m_vecBlock.push_back({ xLB + 2,yLB - 4 });
	m_vecBlock.push_back({ xLB + 3,yLB - 1 });
	m_vecBlock.push_back({ xLB + 3,yLB - 2 });
	m_vecBlock.push_back({ xLB + 3,yLB - 3 });
	m_vecBlock.push_back({ xLB + 3,yLB - 4 });
	m_vecBlock.push_back({ xLB + 4,yLB - 0 });
	m_vecBlock.push_back({ xLB + 4,yLB - 1 });
	m_vecBlock.push_back({ xLB + 4,yLB - 4 });
	m_vecBlock.push_back({ xLB + 4,yLB - 5 });
	m_vecBlock.push_back({ xLB + 5,yLB - 0 });
	m_vecBlock.push_back({ xLB + 5,yLB - 1 });
	m_vecBlock.push_back({ xLB + 5,yLB - 4 });
	m_vecBlock.push_back({ xLB + 5,yLB - 5 });
}

Block::~Block()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

BOOL Block::Update(float fElapsedTime)
{
	for (auto &block : m_vecBlock)
	{
		if (block._x - 2 * block._y - (pTail->m_x - 2 * pTail->m_y)/fSpacing <= 5)
			block._bOn = true;

		if (!block._bShow)
		{
			if (block._bOn)
			{
				block._fTime += fElapsedTime;
				
				if (rand() % 500 == 0 || block._fTime >= 500.0f)
				{
					block._bShow = true;
					block._fTime = 0.0f;
				}
			}

		}
		else
		{
			if (block._fTime < 0.75f * m_timeShow)
				block._fTime += fElapsedTime;
			if (block._fTime > 0.75f * m_timeShow)
				block._fTime = 0.75f * m_timeShow;

			block._fOffset = (m_timeShow - block._fTime) * block._fTime / 10000.0f;
		}

	}

	return TRUE;
}

BOOL Block::Render(float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;
	for (auto block : m_vecBlock)
	{
		if (!block._bShow)
			continue;

		int iTex = (int)(block._fTime / (0.75f * m_timeShow) * 17.0f);

		D3DXMatrixTranslation(&matWorld, block._x * fSpacing, 
			block._y * fSpacing - block._fOffset, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(COLORVERTEX));
		m_pd3dDevice->SetFVF(COLOR_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * iTex, 2);
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Tail
//

Tail::Tail()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pd3dDevice->CreateVertexBuffer(18 * 4 * sizeof(COLORVERTEX), 0, COLORTEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
	m_fSize = fSpacing - (fScale == 1.0f ? 2.0f : 1.0f) / fScale;

	COLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { -m_fSize,  m_fSize, 0.0f, DD_GRAY };
	pVertices[1] = { -m_fSize, -m_fSize, 0.0f, DD_GRAY };
	pVertices[2] = {  m_fSize,  m_fSize, 0.0f, DD_GRAY };
	pVertices[3] = {  m_fSize, -m_fSize, 0.0f, DD_GRAY };

	m_pVertexBuffer->Unlock();

	m_x = 0.0f;
	m_y = 44.0f * fSpacing;
	m_roll = 0.0f;
	m_scale = 1.0f;
	m_timeUpdate = 0.0f;
	m_bOver = false;
}

Tail::~Tail()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

BOOL Tail::Update(float fElapsedTime)
{
	m_timeUpdate += fElapsedTime;

	m_roll = m_timeUpdate * 0.005f;

	if (m_timeUpdate <= 1500.0f)
	{
		m_x = 107.0f * fSpacing * m_timeUpdate / 1500.0f;
		m_y = 44.0f * fSpacing;
	}
	else if (m_timeUpdate <= 1750.0f)
	{
		m_x = 107.0f * fSpacing + 10.0f * fSpacing * sin(D3DX_PI * 0.5f * (m_timeUpdate - 1500.0f) / 250.0f);
		m_y =  34.0f * fSpacing + 10.0f * fSpacing * cos(D3DX_PI * 0.5f * (m_timeUpdate - 1500.0f) / 250.0f);
	}
	else if (m_timeUpdate <= 2375.0f)
	{
		m_x = 92.0f * fSpacing + 25.0f * fSpacing * sin(D3DX_PI * 0.5f * ((m_timeUpdate - 1750.0f) / 625.0f + 1.0f));
		m_y = 34.0f * fSpacing + 25.0f * fSpacing * cos(D3DX_PI * 0.5f * ((m_timeUpdate - 1750.0f) / 625.0f + 1.0f));
	}
	else if (m_timeUpdate <= 3000.0f)
	{
		m_x = 92.0f * fSpacing + 25.0f * fSpacing * sin(D3DX_PI * 0.5f * ((m_timeUpdate - 2375.0f) / 625.0f + 2.0f));
		m_y = 34.0f * fSpacing + 25.0f * fSpacing * cos(D3DX_PI * 0.5f * ((m_timeUpdate - 2375.0f) / 625.0f + 2.0f));
	}
	else
	{
		m_x = 67.0f * fSpacing;
		m_y = 34.0f * fSpacing;
	}

	if (m_timeUpdate >= 4000.0f && m_timeUpdate <= 6000.0f)
	{
		m_scale = 1.0f + pow((m_timeUpdate - 4000.0f) / 100.0f, 2.0f);
		
		COLORVERTEX *pVertices;
		m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

		pVertices[0] = { -m_fSize * m_scale,  m_fSize * m_scale, 0.0f, DD_GRAY };
		pVertices[1] = { -m_fSize * m_scale, -m_fSize * m_scale, 0.0f, DD_GRAY };
		pVertices[2] = {  m_fSize * m_scale,  m_fSize * m_scale, 0.0f, DD_GRAY };
		pVertices[3] = {  m_fSize * m_scale, -m_fSize * m_scale, 0.0f, DD_GRAY };

		m_pVertexBuffer->Unlock();
	}

	if (m_timeUpdate > 6000.0f)
		m_bOver = true;


	return TRUE;
}

BOOL Tail::Render(float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld, matRoll, matScale;
	D3DXMatrixTranslation(&matWorld, m_x, m_y, 0.0f);
	D3DXMatrixRotationZ(&matRoll, m_roll);
	matWorld = matRoll * matWorld;

	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(COLORVERTEX));
	m_pd3dDevice->SetFVF(COLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

bool Tail::IsOver()
{
	return m_bOver;
}

//
//Scene
//

SceneOP::SceneOP()
{

}

SceneOP::~SceneOP()
{

}

void SceneOP::Init()
{
	fScale = GetSettings().m_fScale;
	fActSpacing = fSpacing * fScale;
	bSceneOver = false;
	pBackBoard = new BackBoard();
	pBlock = new Block();
	pTail = new Tail();
}

void SceneOP::End()
{

}

void SceneOP::OnResetDevice()
{

}

void SceneOP::OnLostDevice()
{

}

void SceneOP::Update(double fTime, float fElapsedTime)
{
	if (!bSceneOver)
	{
		if (pTail->IsOver() || pBackBoard->IsOver())
		{
			bSceneOver = true;
			ShowTitle();
		}
	}

	pBackBoard->Update(fElapsedTime);
	pBlock->Update(fElapsedTime);
	pTail->Update(fElapsedTime);
}

void SceneOP::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(GAME_WIDTH / 2, GAME_HEIGHT/2);
	SetView();
	SetProj(NULL, GAME_WIDTH, GAME_HEIGHT);
	SetViewPort();

	pBackBoard->Render(fElapsedTime);
	pBlock->Render(fElapsedTime);
	pTail->Render(fElapsedTime);
}