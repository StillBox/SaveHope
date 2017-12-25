#include "DXObject.h"

//
//DXobject
//

DXObject::DXObject()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
	m_ObjRange = RECTRANGE();
	m_ObjPos = D3DXVECTOR2(0.0f, 0.0f);
	m_Layer = 0;
	m_Roll = 0.0f;
	m_Pitch = 0.0f;
	m_Yaw = 0.0f;
	m_iState = -1;
	m_UpdateTime = 0.0f;
}

DXObject::~DXObject()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pTexture);
}

void DXObject::SetPos(float xPos, float yPos)
{
	m_ObjPos.x = xPos;
	m_ObjPos.y = yPos;
}

void DXObject::SetLayer(int nLayer)
{
	m_Layer = nLayer;
}

void DXObject::SetRoll(float fRoll)
{
	m_Roll = fRoll;
}

void DXObject::SetPitch(float fPitch)
{
	m_Pitch = fPitch;
}

void DXObject::SetYaw(float fYaw)
{
	m_Yaw = fYaw;
}

void DXObject::SetRange(float left, float top, float right, float bottom)
{
	m_ObjRange.left = left;
	m_ObjRange.top = top;
	m_ObjRange.right = right;
	m_ObjRange.bottom = bottom;
}

void DXObject::GetPos(D3DXVECTOR2 *pVec)
{
	*pVec = m_ObjPos;
}

void DXObject::AddTexFromFile(WCHAR *szTexFile, int width, int height)
{
	D3DXCreateTextureFromFile(m_pd3dDevice, szTexFile, &m_pTexture);
	m_TexWidth = width;
	m_TexHeight = height;
}

void DXObject::AddTexFromResc(HMODULE hModule, WCHAR *szResc, int width, int height)
{
	D3DXCreateTextureFromResource(m_pd3dDevice, hModule, szResc, &m_pTexture);
	m_TexWidth = width;
	m_TexHeight = height;
}

void DXObject::AddTexRect(int left, int top, int right, int bottom)
{
	RECTRANGE newRect;
	newRect.left	= (float)left	/ m_TexWidth;
	newRect.right	= (float)right	/ m_TexWidth;
	newRect.top		= (float)top	/ m_TexHeight;
	newRect.bottom	= (float)bottom / m_TexHeight;
	m_TexRects.push_back(newRect);
}

BOOL DXObject::SetTexRect(int iTexRec)
{
	if (NULL == m_pVertexBuffer)
		return FALSE;

	RECTRANGE TexRc = m_TexRects[iTexRec];
	TEXVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = { m_ObjRange.left,	m_ObjRange.bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
	pVertices[1] = { m_ObjRange.left,	m_ObjRange.top,		0.1f*m_Layer,	TexRc.left,		TexRc.top };
	pVertices[2] = { m_ObjRange.right,	m_ObjRange.bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
	pVertices[3] = { m_ObjRange.right,	m_ObjRange.top,		0.1f*m_Layer,	TexRc.right,	TexRc.top };
	m_pVertexBuffer->Unlock();
	m_UpdateTime = 0.0f;

	return TRUE;
}

void DXObject::AddState(int iState, TEXLOOPTYPE LoopType, float fTimeGap)
{
	if (iState >= (int)m_States.size())
	{
		STATEDATA newState;
		for (int i = m_States.size(); i <= iState; i++)
			m_States.push_back(newState);
	}
	m_States[iState].LoopType = LoopType;
	m_States[iState].fTimeGap = fTimeGap;
}

void DXObject::AddStateTexRect(int iState, int iTex)
{
	m_States[iState].vecTex.push_back(iTex);
}

void DXObject::AddStateChain(int iState, int iBegState, int iEndState)
{
	m_States[iState].iBegState = iBegState;
	m_States[iState].iEndState = iEndState;
}

BOOL DXObject::SetState(int iState)
{
	m_iState = iState;
	return DXObject::Init();
}

BOOL DXObject::SetInvert(int iState, bool bInvert)
{
	m_States[iState].bInvert = bInvert;
	return TRUE;
}

BOOL DXObject::Init()
{
	if (NULL != m_pTexture && m_TexRects.size() > 0)
	{
		if (NULL == m_pVertexBuffer)
			m_pd3dDevice->CreateVertexBuffer(4 * sizeof(TEXVERTEX), 0, TEX_FVF,
			D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
		int iTexRec = 0;
		if (-1 != m_iState)
		{
			m_States[m_iState].curIndex = m_States[m_iState].bInvert ? 
										  m_States[m_iState].vecTex.size() - 1 : 0;
			iTexRec = m_States[m_iState].vecTex[m_States[m_iState].curIndex];
		}
		return SetTexRect(iTexRec);
	}
	else
		return FALSE;
}

BOOL DXObject::Update(double fTime, float fElapsedTime)
{
	if (m_iState == -1)
		return TRUE;

	m_UpdateTime += fElapsedTime;

	switch (m_States[m_iState].LoopType)
	{
	case SINGLE:
		return FALSE;

	case LOOP:
		if (m_UpdateTime > m_States[m_iState].fTimeGap)
		{
			m_States[m_iState].curIndex++;
			if (m_States[m_iState].curIndex >= (int)m_States[m_iState].vecTex.size())
				m_States[m_iState].curIndex = 0;
			return SetTexRect(m_States[m_iState].vecTex[m_States[m_iState].curIndex]);
		}
		return FALSE;

	case NOLOOP:
		if (m_UpdateTime > m_States[m_iState].fTimeGap)
		{
			if (!m_States[m_iState].bInvert)
			{
				m_States[m_iState].curIndex++;
				if (m_States[m_iState].curIndex >= (int)m_States[m_iState].vecTex.size())
				{
					m_States[m_iState].bInvert = true;
					return SetState(m_States[m_iState].iEndState);
				}
				else
					return SetTexRect(m_States[m_iState].vecTex[m_States[m_iState].curIndex]);
			}
			else
			{
				m_States[m_iState].curIndex--;
				if (m_States[m_iState].curIndex <= 0)
				{
					m_States[m_iState].bInvert = false;
					return SetState(m_States[m_iState].iBegState);
				}
				else
					return SetTexRect(m_States[m_iState].vecTex[m_States[m_iState].curIndex]);

			}
		}
		return FALSE;

	default:
		break;
	}

	return TRUE;
}

BOOL DXObject::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld, matRoll, matYaw, matPitch;
	D3DXMatrixTranslation(&matWorld, m_ObjPos.x, m_ObjPos.y, 0.0f);
	D3DXMatrixRotationX(&matPitch, m_Pitch);
	D3DXMatrixRotationY(&matYaw, m_Yaw);
	D3DXMatrixRotationZ(&matRoll, m_Roll);
	matWorld = matPitch * matYaw * matRoll * matWorld;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, m_pTexture);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
	m_pd3dDevice->SetFVF(TEX_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//DXRHWObject
//

DXRHWObject::DXRHWObject()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_pVertexBuffer = NULL;
	m_pTexture = NULL;
	m_ObjRange = RECTRANGE();
	m_ObjPos = D3DXVECTOR2(0.0f, 0.0f);
	m_Layer = 0;
	m_Roll = 0.0f;
	m_Pitch = 0.0f;
	m_Yaw = 0.0f;
	m_iState = -1;
	m_UpdateTime = 0.0f;
}

DXRHWObject::~DXRHWObject()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pTexture);
}

void DXRHWObject::SetPos(float xPos, float yPos)
{
	m_ObjPos.x = xPos;
	m_ObjPos.y = yPos;

	RHWTEXVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	float left = m_ObjPos.x + m_ObjRange.left;
	float right = m_ObjPos.x + m_ObjRange.right;
	float top = m_ObjPos.y + m_ObjRange.top;
	float bottom = m_ObjPos.y + m_ObjRange.bottom;

	pVertices[0].x = left,  pVertices[0].y = bottom;
	pVertices[1].x = left,  pVertices[1].y = top;
	pVertices[2].x = right, pVertices[2].y = bottom;
	pVertices[3].x = right,	pVertices[3].y = top;

	m_pVertexBuffer->Unlock();
}

void DXRHWObject::SetLayer(int nLayer)
{
	m_Layer = nLayer;
}

void DXRHWObject::SetRoll(float fRoll)
{
	m_Roll = fRoll;
}

void DXRHWObject::SetPitch(float fPitch)
{
	m_Pitch = fPitch;
}

void DXRHWObject::SetYaw(float fYaw)
{
	m_Yaw = fYaw;
}

void DXRHWObject::SetRange(float left, float top, float right, float bottom)
{
	m_ObjRange.left = left;
	m_ObjRange.top = top;
	m_ObjRange.right = right;
	m_ObjRange.bottom = bottom;
}

void DXRHWObject::GetPos(D3DXVECTOR2 *pVec)
{
	*pVec = m_ObjPos;
}

void DXRHWObject::AddTexFromFile(WCHAR *szTexFile, int width, int height)
{
	D3DXCreateTextureFromFile(m_pd3dDevice, szTexFile, &m_pTexture);
	m_TexWidth = width;
	m_TexHeight = height;
}

void DXRHWObject::AddTexFromResc(HMODULE hModule, WCHAR *szResc, int width, int height)
{
	D3DXCreateTextureFromResource(m_pd3dDevice, hModule, szResc, &m_pTexture);
	m_TexWidth = width;
	m_TexHeight = height;
}

void DXRHWObject::AddTexRect(int left, int top, int right, int bottom)
{
	RECTRANGE newRect;
	newRect.left = (float)left / m_TexWidth;
	newRect.right = (float)right / m_TexWidth;
	newRect.top = (float)top / m_TexHeight;
	newRect.bottom = (float)bottom / m_TexHeight;
	m_TexRects.push_back(newRect);
}

BOOL DXRHWObject::SetTexRect(int iTexRec)
{
	if (NULL == m_pVertexBuffer)
		return FALSE;

	RECTRANGE TexRc = m_TexRects[iTexRec];
	RHWTEXVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	float left = m_ObjPos.x + m_ObjRange.left;
	float right = m_ObjPos.x + m_ObjRange.right;
	float top = m_ObjPos.y + m_ObjRange.top;
	float bottom = m_ObjPos.y + m_ObjRange.bottom;

	pVertices[0] = { left,	bottom,	0.1f*m_Layer, 1.0f,	TexRc.left,		TexRc.bottom };
	pVertices[1] = { left,	top,	0.1f*m_Layer, 1.0f,	TexRc.left,		TexRc.top };
	pVertices[2] = { right,	bottom,	0.1f*m_Layer, 1.0f,	TexRc.right,	TexRc.bottom };
	pVertices[3] = { right,	top,	0.1f*m_Layer, 1.0f,	TexRc.right,	TexRc.top };
	
	m_pVertexBuffer->Unlock();
	m_UpdateTime = 0.0f;

	return TRUE;
}

void DXRHWObject::AddState(int iState, TEXLOOPTYPE LoopType, float fTimeGap)
{
	if (iState >= (int)m_States.size())
	{
		STATEDATA newState;
		for (int i = m_States.size(); i <= iState; i++)
			m_States.push_back(newState);
	}
	m_States[iState].LoopType = LoopType;
	m_States[iState].fTimeGap = fTimeGap;
}

void DXRHWObject::AddStateTexRect(int iState, int iTex)
{
	m_States[iState].vecTex.push_back(iTex);
}

void DXRHWObject::AddStateChain(int iState, int iBegState, int iEndState)
{
	m_States[iState].iBegState = iBegState;
	m_States[iState].iEndState = iEndState;
}

BOOL DXRHWObject::SetState(int iState)
{
	m_iState = iState;
	return DXRHWObject::Init();
}

BOOL DXRHWObject::SetInvert(int iState, bool bInvert)
{
	m_States[iState].bInvert = bInvert;
	return TRUE;
}

BOOL DXRHWObject::Init()
{
	if (NULL != m_pTexture && m_TexRects.size() > 0)
	{
		if (NULL == m_pVertexBuffer)
			m_pd3dDevice->CreateVertexBuffer(4 * sizeof(RHWTEXVERTEX), 0, RHWTEX_FVF,
				D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);
		int iTexRec = 0;
		if (-1 != m_iState)
		{
			m_States[m_iState].curIndex = m_States[m_iState].bInvert ?
				m_States[m_iState].vecTex.size() - 1 : 0;
			iTexRec = m_States[m_iState].vecTex[m_States[m_iState].curIndex];
		}
		return SetTexRect(iTexRec);
	}
	else
		return FALSE;
}

BOOL DXRHWObject::Update(double fTime, float fElapsedTime)
{
	if (m_iState == -1)
		return TRUE;

	m_UpdateTime += fElapsedTime;

	switch (m_States[m_iState].LoopType)
	{
	case SINGLE:
		return FALSE;

	case LOOP:
		if (m_UpdateTime > m_States[m_iState].fTimeGap)
		{
			m_States[m_iState].curIndex++;
			if (m_States[m_iState].curIndex >= (int)m_States[m_iState].vecTex.size())
				m_States[m_iState].curIndex = 0;
			return SetTexRect(m_States[m_iState].vecTex[m_States[m_iState].curIndex]);
		}
		return FALSE;

	case NOLOOP:
		if (m_UpdateTime > m_States[m_iState].fTimeGap)
		{
			if (!m_States[m_iState].bInvert)
			{
				m_States[m_iState].curIndex++;
				if (m_States[m_iState].curIndex >= (int)m_States[m_iState].vecTex.size())
				{
					m_States[m_iState].bInvert = true;
					return SetState(m_States[m_iState].iEndState);
				}
				else
					return SetTexRect(m_States[m_iState].vecTex[m_States[m_iState].curIndex]);
			}
			else
			{
				m_States[m_iState].curIndex--;
				if (m_States[m_iState].curIndex <= 0)
				{
					m_States[m_iState].bInvert = false;
					return SetState(m_States[m_iState].iBegState);
				}
				else
					return SetTexRect(m_States[m_iState].vecTex[m_States[m_iState].curIndex]);

			}
		}
		return FALSE;

	default:
		break;
	}

	return TRUE;
}

BOOL DXRHWObject::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld, matRoll, matYaw, matPitch;
	D3DXMatrixTranslation(&matWorld, m_ObjPos.x, m_ObjPos.y, 0.0f);
	D3DXMatrixRotationX(&matPitch, m_Pitch);
	D3DXMatrixRotationY(&matYaw, m_Yaw);
	D3DXMatrixRotationZ(&matRoll, m_Roll);
	matWorld = matPitch * matYaw * matRoll * matWorld;
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, m_pTexture);
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(RHWTEXVERTEX));
	m_pd3dDevice->SetFVF(RHWTEX_FVF);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Miscellaneous Functions
//

BOOL SetGridRect(LPRECT lprc, float left, float top, float right, float bottom, float fGridSize)
{
	return SetRect(lprc, (int)(left * fGridSize), (int)(top * fGridSize),
		(int)(right * fGridSize), (int)(bottom * fGridSize));
}

BOOL DrawGridText(ID3DXFont *pFont, LPCWSTR pStr, int cTotal, int cCharPerLine, float fHeight, RECT *pRect, D3DCOLOR Color)
{
	RECT rcText = *pRect;
	int cLine = 0;
	int cChar = cTotal;
	int iCharBeg = 0;
	while (cChar > 0)
	{
		rcText.top = pRect->top + (int)(cLine * fHeight);
		if (rcText.top > rcText.bottom)
			return FALSE;

		int cCharDraw = min(cCharPerLine, cChar);
		for (int i = 0; i < cCharDraw; i++)
		{
			if (pStr[iCharBeg + i] == L'\n')
			{
				cCharDraw = i + 1;
				break;
			}
		}

		pFont->DrawTextW(NULL, pStr + iCharBeg, cCharDraw, &rcText, DT_LEFT | DT_TOP, Color);
		
		iCharBeg += cCharDraw;
		cChar -= cCharDraw;
		cLine++;
	}

	return TRUE;
}