#include "GameObject.h"
#include "Settings.h"
#include "MiscData.h"
#include "SoundData.h"

//
//Curtain
//

Curtain::Curtain()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertexBuffer = NULL;
	m_Range = RECTRANGE(0.0f, 0.0f, (float)DXUTGetDeviceWidth(), (float)DXUTGetDeviceHeight());
	m_IsOn = FALSE;
}

Curtain::Curtain(float left, float top, float right, float bottom)
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertexBuffer = NULL;
	m_Range = RECTRANGE(left, top, right, bottom);
	m_IsOn = FALSE;
}

Curtain::~Curtain()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

void Curtain::Resize(float left, float top, float right, float bottom)
{
	m_Range = RECTRANGE(left, top, right, bottom);
}

void Curtain::Set(int AlphaBeg, int AlphaEnd, float fDuration, void(*pFunc)(), int iLayer)
{
	m_IsOn = TRUE;
	m_iLayer = iLayer;
	m_AlphaBeg = AlphaBeg;
	m_AlphaEnd = AlphaEnd;
	m_fDuration = fDuration;
	m_fPassTime = 0.0f;
	m_pOnCurtainEnd = pFunc;
}

BOOL Curtain::IsOn(int iLayer)
{
	if (iLayer == m_iLayer || iLayer == -1)
		return m_IsOn;
	else
		return FALSE;
}

void Curtain::RenderAlpha(int Alpha)
{
	if (NULL == m_pVertexBuffer)
		m_pd3dDevice->CreateVertexBuffer(4 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
			D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	RHWCOLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { m_Range.left,	 m_Range.bottom, 0.0f, 1.0f, D3DCOLOR_ARGB(Alpha,31,31,31) };
	pVertices[1] = { m_Range.left,	 m_Range.top,	 0.0f, 1.0f, D3DCOLOR_ARGB(Alpha,31,31,31) };
	pVertices[2] = { m_Range.right,	 m_Range.bottom, 0.0f, 1.0f, D3DCOLOR_ARGB(Alpha,31,31,31) };
	pVertices[3] = { m_Range.right,	 m_Range.top,	 0.0f, 1.0f, D3DCOLOR_ARGB(Alpha,31,31,31) };

	m_pVertexBuffer->Unlock();

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(RHWCOLORVERTEX));
	m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Curtain::Render(float fElapsedTime)
{
	m_fPassTime += fElapsedTime;
	int Alpha = m_AlphaBeg + (int)((float)(m_AlphaEnd - m_AlphaBeg)*m_fPassTime / m_fDuration);
	RenderAlpha(Alpha);
	if (m_fPassTime >= m_fDuration)
	{
		m_IsOn = FALSE;
		if (NULL != m_pOnCurtainEnd) m_pOnCurtainEnd();
	}
}

Curtain *g_pCurtain = NULL;

HRESULT CreateCurtain()
{
	g_pCurtain = new Curtain();
	return S_OK;
}

HRESULT DeleteCurtain()
{
	g_pCurtain->~Curtain();
	g_pCurtain = NULL;
	return S_OK;
}

void ResizeCurtain()
{
	g_pCurtain->Resize(0.0f, 0.0f, (float)DXUTGetDeviceWidth(), (float)DXUTGetDeviceHeight());
}

void SetCurtain(int AlphaBeg, int AlphaEnd, float fDuration, void(*pFunc)(), int iLayer)
{
	assert(g_pCurtain != NULL);
	g_pCurtain->Set(AlphaBeg, AlphaEnd, fDuration, pFunc);
}

BOOL IsCurtainOn(int iLayer)
{
	if (g_pCurtain)
		return g_pCurtain->IsOn(iLayer);
	else
		return FALSE;
}

void RenderCutain(float fElapsedTime)
{
	assert(g_pCurtain != NULL);
	g_pCurtain->Render(fElapsedTime);
}

void RenderAlphaCurtain(int Alpha)
{
	assert(g_pCurtain != NULL);
	g_pCurtain->RenderAlpha(Alpha);
}

//
//Background
//

Background::Background(bool bClear)
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_fSize = 16.0f * GetSettings().m_fScale;
	m_cBeam = 60;
	m_iLen = 7;
	m_xCount = (int)(GAME_WIDTH * GetSettings().m_fScale / m_fSize);
	m_yCount = (int)(GAME_HEIGHT * GetSettings().m_fScale / m_fSize);

	m_timeUpdate = 0.0f;
	m_fSPB = 60.0f;
	m_bReverse = false;

	m_vOffset = { 0.0f,0.0f };

	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	RHWCOLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { 0.0f,		m_fSize,	0.0f, 1.0f, D3DCOLOR_ARGB(255,0,255,0) };
	pVertices[1] = { 0.0f,		0.0f,		0.0f, 1.0f, D3DCOLOR_ARGB(255,0,255,0) };
	pVertices[2] = { m_fSize,	m_fSize,	0.0f, 1.0f, D3DCOLOR_ARGB(255,0,255,0) };
	pVertices[3] = { m_fSize,	0.0f,		0.0f, 1.0f, D3DCOLOR_ARGB(255,0,255,0) };

	m_pVertexBuffer->Unlock();

	if (!bClear)
	{
		m_vecBeam.clear();
		for (int i = 0; i < m_cBeam; i++)
		{
			Beam newBeam;
			newBeam.x[0] = rand() % m_xCount;
			newBeam.y[0] = rand() % m_yCount;
			for (int j = 1; j < m_iLen; j++)
			{
				newBeam.x[j] = newBeam.x[0];
				newBeam.y[j] = newBeam.y[0] - j;
			}
			m_vecBeam.push_back(newBeam);
		}
	}
}

Background::~Background()
{
	m_vecBeam.clear();
	SAFE_RELEASE(m_pVertexBuffer);
}

BOOL Background::Update(double fTime, float fElapsedTime)
{
	m_timeUpdate += fElapsedTime;
	if (m_timeUpdate > m_fSPB)
	{
		auto iter = m_vecBeam.begin();
		while (iter != m_vecBeam.end())
		{
			for (int i = m_iLen - 1; i > 0; i--)
			{
				iter->x[i] = iter->x[i - 1];
				iter->y[i] = iter->y[i - 1];
			}
			if (rand() % 20 == 0)
			{
				if (rand() % 2 == 0)
					iter->x[0]--;
				else
					iter->x[0]++;
			}
			else
			{
				if (m_bReverse)
					iter->y[0]--;
				else
					iter->y[0]++;
			}

			if (m_bReverse)
			{
				if (iter->y[m_iLen - 1] < 0)
					iter = m_vecBeam.erase(iter);
				else
					iter++;
			}
			else
			{
				if (iter->y[m_iLen - 1] > m_yCount)
					iter = m_vecBeam.erase(iter);
				else
					iter++;
			}
		}

		if ((int)m_vecBeam.size() < m_cBeam && rand() % 3 > 0)
		{
			Beam newBeam;
			newBeam.x[0] = rand() % m_xCount;
			newBeam.y[0] = m_bReverse ? m_yCount : 0;
			for (int j = 1; j < m_iLen; j++)
			{
				newBeam.x[j] = newBeam.x[0];
				newBeam.y[j] = newBeam.y[0] + m_bReverse? j : -j;
			}
			m_vecBeam.push_back(newBeam);
		}

		m_timeUpdate -= m_fSPB;
	}

	return TRUE;
}

BOOL Background::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (auto beam : m_vecBeam)
	{
		for (int i = 0; i < m_iLen; i++)
		{
			RHWCOLORVERTEX *pVertices;
			m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

			int iAlpha		= (i == 0) ? 200 : 18 * (m_iLen - i);
			float left		= beam.x[i] * m_fSize;
			float right		= beam.x[i] * m_fSize + m_fSize - 1.0f;
			float top		= beam.y[i] * m_fSize;
			float bottom	= beam.y[i] * m_fSize + m_fSize - 1.0f;

			DWORD color = m_bReverse ? D3DCOLOR_ARGB(iAlpha, 255, 0, 0) : D3DCOLOR_ARGB(iAlpha, 0, 255, 0);

			pVertices[0] = { left,  bottom, 1.0f, 1.0f, color };
			pVertices[1] = { left,  top,    1.0f, 1.0f, color };
			pVertices[2] = { right,	bottom, 1.0f, 1.0f, color };
			pVertices[3] = { right,	top,    1.0f, 1.0f, color };

			m_pVertexBuffer->Unlock();

			m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(RHWCOLORVERTEX));
			m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

void Background::SetSPB(float fSPB)
{
	m_fSPB = fSPB;
}

void Background::SetOffset(float x, float y)
{
	m_vOffset.x = x;
	m_vOffset.y = y;
}

void Background::Reverse(bool bReverse)
{
	m_bReverse = bReverse;
}

//
//BackBoard
//

Board::Board()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pTexture = NULL;
	m_pVertBoard = NULL;
	m_pVertLogo = NULL;
}

Board::~Board()
{
	SAFE_RELEASE(m_pVertLogo);
	SAFE_RELEASE(m_pVertBoard);
	SAFE_RELEASE(m_pTexture);
}

void Board::Set(int width)
{
	m_width = width;

	m_fScale = GetSettings().m_fScale;
	switch (GetSettings().m_iResolution)
	{
	case 1:
		D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO1), &m_pTexture);
		break;
	case 2:
		D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO2), &m_pTexture);
		break;
	case 3:
		D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO4), &m_pTexture);
		break;
	default:
		switch (GetSettings().m_iWindowed)
		{
		case 1:
			D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO1), &m_pTexture);
			break;
		case 2:
			D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO2), &m_pTexture);
			break;
		default:
			D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_LOGO4), &m_pTexture);
			break;
		}
		break;
	}
	
	{
		SAFE_RELEASE(m_pVertBoard);
		m_pd3dDevice->CreateVertexBuffer(8 * sizeof(RHWCOLORVERTEX), 0, RHWTEX_FVF,
			D3DPOOL_MANAGED, &m_pVertBoard, NULL);
		RHWCOLORVERTEX *pVertices;
		m_pVertBoard->Lock(0, 0, (void**)&pVertices, 0);

		float x1 = (float)m_width * m_fScale;
		float x2 = (float)GAME_WIDTH * m_fScale;
		float y = (float)GAME_HEIGHT * m_fScale;

		pVertices[0] = { 0.0f,    y, 0.0f, 1.0f, WHITE };
		pVertices[1] = { 0.0f, 0.0f, 0.0f, 1.0f, WHITE };
		pVertices[2] = {   x1,    y, 0.0f, 1.0f, WHITE };
		pVertices[3] = {   x1, 0.0f, 0.0f, 1.0f, WHITE };
		pVertices[4] = {   x1,    y, 0.0f, 1.0f, BLACK };
		pVertices[5] = {   x1, 0.0f, 0.0f, 1.0f, BLACK };
		pVertices[6] = {   x2,    y, 0.0f, 1.0f, BLACK };
		pVertices[7] = {   x2, 0.0f, 0.0f, 1.0f, BLACK };

		m_pVertBoard->Unlock();
	}

	{
		SAFE_RELEASE(m_pVertLogo);
		m_pd3dDevice->CreateVertexBuffer(8 * sizeof(RHWTEXVERTEX), 0, RHWTEX_FVF,
			D3DPOOL_MANAGED, &m_pVertLogo, NULL);
		RHWTEXVERTEX *pVertices;
		m_pVertLogo->Lock(0, 0, (void**)&pVertices, 0);

		float left = ((float)m_width + 64.0f) * m_fScale;
		float right = ((float)m_width + 576.0f) * m_fScale;
		float top = ((float)(GAME_HEIGHT / 3) - 160.0f) * m_fScale;
		float bottom = ((float)(GAME_HEIGHT / 3) + 160.0f) * m_fScale;
		
		pVertices[0] = {  left, bottom, 0.0f, 1.0f, 0.0f, 0.625f };
		pVertices[1] = {  left,    top, 0.0f, 1.0f, 0.0f,   0.0f };
		pVertices[2] = { right, bottom, 0.0f, 1.0f, 1.0f, 0.625f };
		pVertices[3] = { right,    top, 0.0f, 1.0f, 1.0f,   0.0f };

		m_pVertLogo->Unlock();
	}
}

void Board::Render(float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetStreamSource(0, m_pVertBoard, 0, sizeof(RHWCOLORVERTEX));
	m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

	m_pd3dDevice->SetTexture(0, m_pTexture);
	m_pd3dDevice->SetStreamSource(0, m_pVertLogo, 0, sizeof(RHWTEXVERTEX));
	m_pd3dDevice->SetFVF(RHWTEX_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	m_pd3dDevice->SetTexture(0, NULL);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

bool Board::IsReady()
{
	return true;
}

//
//Box
//

Box::Box()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pTexture = NULL;
	m_pVertBox = NULL;
}

Box::~Box()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVertBox);
}

void Box::Set(int xCenter, int yCenter, int width, int height, float fDeep)
{
	m_bOpen = true;
	m_pOnBoxClosed = NULL;
	m_xCenter = xCenter;
	m_yCenter = yCenter;
	m_width = width;
	m_height = height;
	m_fDeep = fDeep;
	m_fSize = 0.0f;

	float fTexScale = 1.0f;

	m_fScale = GetSettings().m_fScale;
	switch (GetSettings().m_iResolution)
	{
	case 1:
		D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU1), &m_pTexture);
		break;
	case 2:
		D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU2), &m_pTexture);
		break;
	case 3:
		D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU4), &m_pTexture);
		break;
	default:
		switch (GetSettings().m_iWindowed)
		{
		case 1:
			D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU1), &m_pTexture);
			break;
		case 2:
			D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU2), &m_pTexture);
			break;
		default:
			D3DXCreateTextureFromResource(m_pd3dDevice, GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MENU4), &m_pTexture);
			break;
		}
		break;
	}

	m_x[0] = (float)(m_xCenter - 64) * m_fScale;
	m_x[1] = (float) m_xCenter       * m_fScale;
	m_x[2] = (float) m_xCenter       * m_fScale;
	m_x[3] = (float)(m_xCenter + 64) * m_fScale;

	m_y[0] = (float)(m_yCenter - 64) * m_fScale;
	m_y[1] = (float) m_yCenter       * m_fScale;
	m_y[2] = (float) m_yCenter       * m_fScale;
	m_y[3] = (float)(m_yCenter + 64) * m_fScale;

	SAFE_RELEASE(m_pVertBox);
	m_pd3dDevice->CreateVertexBuffer(36 * sizeof(RHWTEXVERTEX), 0, RHWTEX_FVF,
		D3DPOOL_MANAGED, &m_pVertBox, NULL);
	RHWTEXVERTEX *pVertices;
	m_pVertBox->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0]  = { m_x[0], m_y[1], m_fDeep, 1.0f,   0.0f * fTexScale, 0.25f * fTexScale };
	pVertices[1]  = { m_x[0], m_y[0], m_fDeep, 1.0f,   0.0f * fTexScale,  0.0f * fTexScale };
	pVertices[2]  = { m_x[1], m_y[1], m_fDeep, 1.0f, 0.125f * fTexScale, 0.25f * fTexScale };
	pVertices[3]  = { m_x[1], m_y[0], m_fDeep, 1.0f, 0.125f * fTexScale,  0.0f * fTexScale };
	pVertices[4]  = { m_x[1], m_y[1], m_fDeep, 1.0f, 0.125f * fTexScale, 0.25f * fTexScale };
	pVertices[5]  = { m_x[1], m_y[0], m_fDeep, 1.0f, 0.125f * fTexScale,  0.0f * fTexScale };
	pVertices[6]  = { m_x[2], m_y[1], m_fDeep, 1.0f,  0.25f * fTexScale, 0.25f * fTexScale };
	pVertices[7]  = { m_x[2], m_y[0], m_fDeep, 1.0f,  0.25f * fTexScale,  0.0f * fTexScale };
	pVertices[8]  = { m_x[2], m_y[1], m_fDeep, 1.0f,  0.25f * fTexScale, 0.25f * fTexScale };
	pVertices[9]  = { m_x[2], m_y[0], m_fDeep, 1.0f,  0.25f * fTexScale,  0.0f * fTexScale };
	pVertices[10] = { m_x[3], m_y[1], m_fDeep, 1.0f, 0.375f * fTexScale, 0.25f * fTexScale };
	pVertices[11] = { m_x[3], m_y[0], m_fDeep, 1.0f, 0.375f * fTexScale,  0.0f * fTexScale };

	pVertices[12] = { m_x[0], m_y[2], m_fDeep, 1.0f,   0.0f * fTexScale,  0.5f * fTexScale };
	pVertices[13] = { m_x[0], m_y[1], m_fDeep, 1.0f,   0.0f * fTexScale, 0.25f * fTexScale };
	pVertices[14] = { m_x[1], m_y[2], m_fDeep, 1.0f, 0.125f * fTexScale,  0.5f * fTexScale };
	pVertices[15] = { m_x[1], m_y[1], m_fDeep, 1.0f, 0.125f * fTexScale, 0.25f * fTexScale };
	pVertices[16] = { m_x[1], m_y[2], m_fDeep, 1.0f, 0.125f * fTexScale,  0.5f * fTexScale };
	pVertices[17] = { m_x[1], m_y[1], m_fDeep, 1.0f, 0.125f * fTexScale, 0.25f * fTexScale };
	pVertices[18] = { m_x[2], m_y[2], m_fDeep, 1.0f,  0.25f * fTexScale,  0.5f * fTexScale };
	pVertices[19] = { m_x[2], m_y[1], m_fDeep, 1.0f,  0.25f * fTexScale, 0.25f * fTexScale };
	pVertices[20] = { m_x[2], m_y[2], m_fDeep, 1.0f,  0.25f * fTexScale,  0.5f * fTexScale };
	pVertices[21] = { m_x[2], m_y[1], m_fDeep, 1.0f,  0.25f * fTexScale, 0.25f * fTexScale };
	pVertices[22] = { m_x[3], m_y[2], m_fDeep, 1.0f, 0.375f * fTexScale,  0.5f * fTexScale };
	pVertices[23] = { m_x[3], m_y[1], m_fDeep, 1.0f, 0.375f * fTexScale, 0.25f * fTexScale };

	pVertices[24] = { m_x[0], m_y[3], m_fDeep, 1.0f,   0.0f * fTexScale, 0.75f * fTexScale };
	pVertices[25] = { m_x[0], m_y[2], m_fDeep, 1.0f,   0.0f * fTexScale,  0.5f * fTexScale };
	pVertices[26] = { m_x[1], m_y[3], m_fDeep, 1.0f, 0.125f * fTexScale, 0.75f * fTexScale };
	pVertices[27] = { m_x[1], m_y[2], m_fDeep, 1.0f, 0.125f * fTexScale,  0.5f * fTexScale };
	pVertices[28] = { m_x[1], m_y[3], m_fDeep, 1.0f, 0.125f * fTexScale, 0.75f * fTexScale };
	pVertices[29] = { m_x[1], m_y[2], m_fDeep, 1.0f, 0.125f * fTexScale,  0.5f * fTexScale };
	pVertices[30] = { m_x[2], m_y[3], m_fDeep, 1.0f,  0.25f * fTexScale, 0.75f * fTexScale };
	pVertices[31] = { m_x[2], m_y[2], m_fDeep, 1.0f,  0.25f * fTexScale,  0.5f * fTexScale };
	pVertices[32] = { m_x[2], m_y[3], m_fDeep, 1.0f,  0.25f * fTexScale, 0.75f * fTexScale };
	pVertices[33] = { m_x[2], m_y[2], m_fDeep, 1.0f,  0.25f * fTexScale,  0.5f * fTexScale };
	pVertices[34] = { m_x[3], m_y[3], m_fDeep, 1.0f, 0.375f * fTexScale, 0.75f * fTexScale };
	pVertices[35] = { m_x[3], m_y[2], m_fDeep, 1.0f, 0.375f * fTexScale,  0.5f * fTexScale };

	m_pVertBox->Unlock();
}

void Box::Render(float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_bOpen)
	{
		m_fSize = min(1.0f, m_fSize + 0.01f * fElapsedTime);

		int width = (int)((float)m_width * m_fSize);
		int height = (int)((float)m_height * m_fSize);

		m_x[0] = (float)(m_xCenter - width - 64) * m_fScale;
		m_x[1] = (float)(m_xCenter - width) * m_fScale;
		m_x[2] = (float)(m_xCenter + width) * m_fScale;
		m_x[3] = (float)(m_xCenter + width + 64) * m_fScale;

		m_y[0] = (float)(m_yCenter - height - 64) * m_fScale;
		m_y[1] = (float)(m_yCenter - height) * m_fScale;
		m_y[2] = (float)(m_yCenter + height) * m_fScale;
		m_y[3] = (float)(m_yCenter + height + 64) * m_fScale;
	}
	else
	{
		m_fSize = max(0.0f, m_fSize - 0.01f * fElapsedTime);

		int width = (int)((float)m_width * m_fSize);
		int height = (int)((float)m_height * m_fSize);

		m_x[0] = (float)(m_xCenter - width - 64) * m_fScale;
		m_x[1] = (float)(m_xCenter - width) * m_fScale;
		m_x[2] = (float)(m_xCenter + width) * m_fScale;
		m_x[3] = (float)(m_xCenter + width + 64) * m_fScale;

		m_y[0] = (float)(m_yCenter - height - 64) * m_fScale;
		m_y[1] = (float)(m_yCenter - height) * m_fScale;
		m_y[2] = (float)(m_yCenter + height) * m_fScale;
		m_y[3] = (float)(m_yCenter + height + 64) * m_fScale;

		if (m_fSize == 0.0f && NULL != m_pOnBoxClosed)
			m_pOnBoxClosed();
	}

	RHWTEXVERTEX *pVertices;
	m_pVertBox->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = 4 * (3 * i + j);

			pVertices[index].x = m_x[j];
			pVertices[index].y = m_y[i + 1];
			pVertices[index + 1].x = m_x[j];
			pVertices[index + 1].y = m_y[i];
			pVertices[index + 2].x = m_x[j + 1];
			pVertices[index + 2].y = m_y[i + 1];
			pVertices[index + 3].x = m_x[j + 1];
			pVertices[index + 3].y = m_y[i];
		}
	}

	m_pVertBox->Unlock();
	m_pd3dDevice->SetTexture(0, m_pTexture);
	m_pd3dDevice->SetStreamSource(0, m_pVertBox, 0, sizeof(RHWTEXVERTEX));
	m_pd3dDevice->SetFVF(RHWTEX_FVF);
	for (int i = 0; i < 9; i++)
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Box::Close(void(*pFunc)())
{
	m_bOpen = false;
	m_pOnBoxClosed = pFunc;
}

bool Box::IsReady()
{
	if (m_bOpen && 1.0f == m_fSize)
		return true;
	else
		return false;
}

bool Box::IsClosed()
{
	if (!m_bOpen && 0.0f == m_fSize)
		return true;
	else
		return false;
}

//
//MessageBox
//

CheckBox::CheckBox()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertText = NULL;
	m_pFont = NULL;
	m_pBox = NULL;
}

CheckBox::~CheckBox()
{
	SAFE_DELETE(m_pSEChoose);
	SAFE_DELETE(m_pSECancel);
	SAFE_DELETE(m_pSEDecide);
	SAFE_DELETE(m_pBox);

	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pVertText);
}

void CheckBox::Set(int width, int height, WCHAR *szText)
{
	m_width = width;
	m_height = height;
	wcscpy_s(m_szText, MAX_PATH, szText);

	m_fScale = GetSettings().m_fScale;

	int x = GAME_WIDTH / 2;
	int y = GAME_HEIGHT / 2;

	m_pBox = new Box();
	m_pBox->Set(x, y, width, height);

	SAFE_RELEASE(m_pVertText);
	m_pd3dDevice->CreateVertexBuffer(4 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertText, NULL);

	D3DXCreateFont(m_pd3dDevice, (int)(48.0f * m_fScale), 0, 0, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"ºÚÌå", &m_pFont);

	m_pSEChoose = new Sound();
	m_pSEChoose->Register(ID_SE_MB_CHOOSE, ID_SUBMIX_SOUNDEFFECT);
	m_pSEChoose->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CURSOR, L"WAVE");

	m_pSEDecide = new Sound();
	m_pSEDecide->Register(ID_SE_MB_DECIDE, ID_SUBMIX_SOUNDEFFECT);
	m_pSEDecide->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_DECIDE, L"WAVE");

	m_pSECancel = new Sound();
	m_pSECancel->Register(ID_SE_MB_CANCEL, ID_SUBMIX_SOUNDEFFECT);
	m_pSECancel->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CANCEL, L"WAVE");

	m_vecOption.clear();
	m_iOption = 0;

	m_bOpen = true;
	m_bDecide = false;
}

void CheckBox::AddOption(int x, int y, int width, int height, WCHAR* szOption)
{
	Option newOption;
	SetRect(&newOption.rectOption, x, y, x + width, y + height);
	wcscpy_s(newOption.szOption, MAX_PATH, szOption);
	m_vecOption.push_back(newOption);
}

void CheckBox::SetOption(int iOption)
{
	m_iOption = iOption;
}

void CheckBox::Close()
{
	m_bOpen = false;
}

void CheckBox::OnLostDevice()
{
	SAFE_RELEASE(m_pFont);
}

void CheckBox::OnResetDevice()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	SAFE_RELEASE(m_pFont);
	D3DXCreateFont(m_pd3dDevice, (int)(48.0f * m_fScale), 0, 0, 1, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"ºÚÌå", &m_pFont);
}

void CheckBox::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_pBox->IsReady())
		return;

	if (WM_KEYDOWN == message)
	{
		switch (wParam)
		{
		case VK_LEFT:
			m_pSEChoose->Play(0);
			m_iOption = max(0, m_iOption - 1);
			break;

		case VK_RIGHT:
			m_pSEChoose->Play(0);
			m_iOption = min((int)m_vecOption.size() - 1, m_iOption + 1);
			break;

		case VK_RETURN:
			m_pSEDecide->Play(0);
			m_bDecide = true;
			m_pBox->Close();
			break;

		case VK_ESCAPE:
			m_pSECancel->Play(0);
			m_pBox->Close();
			break;
		}
	}
}

void CheckBox::Render(float fElapsedTime)
{
	//Box
	m_pBox->Render(fElapsedTime);
	
	if (m_pBox->IsReady())
	{
		int x = GAME_WIDTH / 2;
		int y = GAME_HEIGHT / 2;

		//Text
		RECT rcText;
		int left, top, right, bottom;

		left   = (int)((float)(x - m_width - 32) * m_fScale);
		top    = (int)((float)(y - m_height - 32) * m_fScale);
		right  = (int)((float)(x + m_width + 32) * m_fScale);
		bottom = (int)((float)(y - m_height + 64) * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		m_pFont->DrawText(NULL, m_szText, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);

		if (m_vecOption.size() > 0)
		{
			for (int i = 0; i < (int)m_vecOption.size(); i++)
			{
				left   = (int)((float)(x - m_width + m_vecOption[i].rectOption.left) * m_fScale);
				top    = (int)((float)(y - m_height + m_vecOption[i].rectOption.top) * m_fScale);
				right  = (int)((float)(x - m_width + m_vecOption[i].rectOption.right)* m_fScale);
				bottom = (int)((float)(y - m_height + m_vecOption[i].rectOption.bottom) * m_fScale);
				SetRect(&rcText, left, top, right, bottom);

				if (m_iOption == i)
				{
					RHWCOLORVERTEX *pVertices;
					m_pVertText->Lock(0, 0, (void**)&pVertices, 0);
					pVertices[0] = { (float) left, (float)bottom, 0.0f, 1.0f, BLACK };
					pVertices[1] = { (float) left, (float)   top, 0.0f, 1.0f, BLACK };
					pVertices[2] = { (float)right, (float)bottom, 0.0f, 1.0f, BLACK };
					pVertices[3] = { (float)right, (float)   top, 0.0f, 1.0f, BLACK };
					m_pVertText->Unlock();

					m_pd3dDevice->SetTexture(0, NULL);
					m_pd3dDevice->SetStreamSource(0, m_pVertText, 0, sizeof(RHWCOLORVERTEX));
					m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
					m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

					m_pFont->DrawText(NULL, m_vecOption[i].szOption, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
				}
				else
					m_pFont->DrawText(NULL, m_vecOption[i].szOption, -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
			}
		}
	}

	if (m_pBox->IsClosed())
	{
		m_bOpen = false;
		SendMessage(DXUTGetHWnd(), WM_CBCLOSED, 0, 0);
	}
}

bool CheckBox::IsOpen()
{
	return m_bOpen;
}

bool CheckBox::GetOption(int *iOption)
{
	if (m_bDecide)
	{
		*iOption = m_iOption;
		m_bDecide = false;
		return true;
	}
	else
		return false;
}

CheckBox *g_pCheckBox = NULL;

HRESULT CreateCheckBox()
{
	g_pCheckBox = new CheckBox();
	return S_OK;
}

HRESULT DeleteCheckBox()
{
	g_pCheckBox->~CheckBox();
	g_pCheckBox = NULL;
	return S_OK;
}

void SetCheckBox(int width, int height, WCHAR* szText)
{
	if (g_pCheckBox)
		DeleteCheckBox();

	CreateCheckBox();

	g_pCheckBox->Set(width, height, szText);
}

void AddCheckBoxOption(int x, int y, int width, int height, WCHAR* szOption)
{
	assert(g_pCheckBox != NULL);
	g_pCheckBox->AddOption(x, y, width, height, szOption);
}

void SetCheckBoxOption(int iOption)
{
	assert(g_pCheckBox != NULL);
	g_pCheckBox->SetOption(iOption);
}

bool IsCheckBoxOpen()
{
	if (g_pCheckBox)
		return g_pCheckBox->IsOpen();
	else
		return false;
}

bool GetCheckBoxOption(int *iOption)
{
	if (g_pCheckBox)
		return g_pCheckBox->GetOption(iOption);
	else
		return false;
}

void CheckBoxOnLostDevice()
{
	if (NULL != g_pCheckBox)
		g_pCheckBox->OnLostDevice();
}

void CheckBoxOnResetDevice()
{
	if (NULL != g_pCheckBox)
		g_pCheckBox->OnResetDevice();
}

void RenderCheckBox(float fElapsedTime)
{
	assert(g_pCheckBox != NULL);
	g_pCheckBox->Render(fElapsedTime);
}

void OnMsgProcCheckBox(UINT message, WPARAM wParam, LPARAM lParam)
{
	assert(g_pCheckBox != NULL);
	g_pCheckBox->OnMsgProc(message, wParam, lParam);
}