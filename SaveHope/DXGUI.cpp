#include "DXGUI.h"
#include "DXObject.h"

//
//DXDialogResourceManager
//

DXDialogResourceManager::DXDialogResourceManager()
{
	m_pd3dDevice = NULL;
	m_pTexture = NULL;
	m_pVertexBuffer = NULL;
	m_fScale = 1.0f;

	m_pSoundCursor = NULL;
	m_pSoundSelect = NULL;
	m_pSoundDecide = NULL;
	m_pSoundCancel = NULL;
}

DXDialogResourceManager::~DXDialogResourceManager()
{
	for (auto p : m_pFonts)
	{
		SAFE_RELEASE(p->pFont);
	}
	std::vector<DXFontData*>().swap(m_pFonts);
	std::vector<DXDialog*>().swap(m_pDialogs);

	SAFE_DELETE(m_pSoundCancel);
	SAFE_DELETE(m_pSoundDecide);
	SAFE_DELETE(m_pSoundSelect);
	SAFE_DELETE(m_pSoundCursor);

	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVertexBuffer);
}

void DXDialogResourceManager::SetScale(float fScale)
{
	m_fScale = fScale;
}

int DXDialogResourceManager::AddFont(LPCWSTR szFaceName, LONG height, LONG weight)
{
	for (unsigned int i = 0; i < m_pFonts.size(); i++)
	{
		DXFontData *pFont = m_pFonts[i];
		auto len = wcslen(pFont->szFaceName);
		if (0 == _wcsnicmp(szFaceName, pFont->szFaceName, len)
			&& height == pFont->nHeight && weight == pFont->nWeight)
			return i;
	}

	DXFontData *pNewFont = new DXFontData();
	ZeroMemory(pNewFont, sizeof(DXFontData));
	wcscpy_s(pNewFont->szFaceName, MAX_PATH, szFaceName);
	pNewFont->nHeight = height;
	pNewFont->nWeight = weight;
	m_pFonts.push_back(pNewFont);
	int i = m_pFonts.size() - 1;
	if (m_pd3dDevice)
		SetFont(i);
	return i;
}

void DXDialogResourceManager::SetFont(int iFont)
{
	SAFE_RELEASE(m_pFonts[iFont]->pFont);
	D3DXCreateFont(m_pd3dDevice, (INT)(m_pFonts[iFont]->nHeight * m_fScale), 0, m_pFonts[iFont]->nWeight, 1, 
		FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		m_pFonts[iFont]->szFaceName, &m_pFonts[iFont]->pFont);
}

DXFontData* DXDialogResourceManager::GetFont(int iFont)
{
	return m_pFonts[iFont];
}

void DXDialogResourceManager::SetCursorSound(int SoundID, int SubmixID, WCHAR* szFile)
{
	m_pSoundCursor = new Sound();
	m_pSoundCursor->Register(SoundID, SubmixID);
	m_pSoundCursor->AddSound(szFile);
}

void DXDialogResourceManager::SetSelectSound(int SoundID, int SubmixID, WCHAR* szFile)
{
	m_pSoundSelect = new Sound();
	m_pSoundSelect->Register(SoundID, SubmixID);
	m_pSoundSelect->AddSound(szFile);
}

void DXDialogResourceManager::SetDecideSound(int SoundID, int SubmixID, WCHAR* szFile)
{
	m_pSoundDecide = new Sound();
	m_pSoundDecide->Register(SoundID, SubmixID);
	m_pSoundDecide->AddSound(szFile);
}

void DXDialogResourceManager::SetCancelSound(int SoundID, int SubmixID, WCHAR* szFile)
{
	m_pSoundCancel = new Sound();
	m_pSoundCancel->Register(SoundID, SubmixID);
	m_pSoundCancel->AddSound(szFile);
}

void DXDialogResourceManager::SetCursorSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType)
{
	m_pSoundCursor = new Sound();
	m_pSoundCursor->Register(SoundID, SubmixID);
	m_pSoundCursor->AddSoundFromRsrc(hModule, ID, lpType);
}

void DXDialogResourceManager::SetSelectSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType)
{
	m_pSoundSelect = new Sound();
	m_pSoundSelect->Register(SoundID, SubmixID);
	m_pSoundSelect->AddSoundFromRsrc(hModule, ID, lpType);
}

void DXDialogResourceManager::SetDecideSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType)
{
	m_pSoundDecide = new Sound();
	m_pSoundDecide->Register(SoundID, SubmixID);
	m_pSoundDecide->AddSoundFromRsrc(hModule, ID, lpType);
}

void DXDialogResourceManager::SetCancelSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType)
{
	m_pSoundCancel = new Sound();
	m_pSoundCancel->Register(SoundID, SubmixID);
	m_pSoundCancel->AddSoundFromRsrc(hModule, ID, lpType);
}

void DXDialogResourceManager::RegisterDialog(DXDialog *pDialog, int IDD)
{
	m_pDialogs.push_back(pDialog);
	pDialog->m_pManager = this;
	pDialog->m_ID = IDD;
}

void DXDialogResourceManager::UnregisterDialog(DXDialog *pDialog)
{
	auto iter = m_pDialogs.begin();
	while (iter!= m_pDialogs.end())
	{
		if (*iter == pDialog)
		{
			m_pDialogs.erase(iter);
			break;
		}
		iter++;
	}
}

void DXDialogResourceManager::UnregisterDialog(int IDD)
{
	auto iter = m_pDialogs.begin();
	while (iter != m_pDialogs.end())
	{
		if ((*iter)->m_ID == IDD)
		{
			m_pDialogs.erase(iter);
			break;
		}
		iter++;
	}
}

HRESULT DXDialogResourceManager::OnCreateDevice()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	for (auto p : m_pFonts)
	{
		if (NULL != p)
		{
			SAFE_RELEASE(p->pFont);
			D3DXCreateFont(m_pd3dDevice, (INT)(p->nHeight * m_fScale), 0, p->nWeight, 1, FALSE,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
				p->szFaceName, &p->pFont);
		}
	}
	m_pd3dDevice->CreateVertexBuffer(8 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
			D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	return S_OK;
}

HRESULT DXDialogResourceManager::OnResetDevice()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	for (auto p : m_pFonts)
	{
		SAFE_RELEASE(p->pFont);
		D3DXCreateFont(m_pd3dDevice, (INT)(p->nHeight * m_fScale), 0, p->nWeight, 1, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			p->szFaceName, &p->pFont);
	}
	return S_OK;
}

HRESULT DXDialogResourceManager::OnLostDevice()
{
	for (auto p : m_pFonts)
	{
		SAFE_RELEASE(p->pFont);
	}
	return S_OK;
}

IDirect3DDevice9* DXDialogResourceManager::GetD3DDevice()
{
	return m_pd3dDevice;
}

DXDialog* DXDialogResourceManager::GetDialog(int IDD)
{
	for (auto p : m_pDialogs)
	{
		if (IDD == p->m_ID)
			return p;
	}
	return NULL;
}


//
//DXDialog
//

DXDialog::DXDialog()
{
	m_pManager = NULL;
	m_x = 0;
	m_y = 0;
	m_Width = 0;
	m_Height = 0;
	m_CaptionHeight = 18;

	m_pFocus = NULL;
	m_pPressed = NULL;
}

DXDialog::~DXDialog()
{
	std::vector<DXControl*>().swap(m_pControls);
	std::vector<int>().swap(m_pFonts);
}

void DXDialog::Default()
{
	for (auto p : m_pControls)
		p->SetState(DXSTATE_NORMAL);

	for (auto p : m_pControls)
		if (p->SetState(DXSTATE_FOCUS))
		{
			m_pFocus = p;
			break;
		}

	m_pPressed = NULL;
}

void DXDialog::SetFont(int iFont, LPWSTR szFaceName, LONG height, LONG weight)
{
	auto size = m_pFonts.size();
	int iFontManager = m_pManager->AddFont(szFaceName, height, weight);

	if (iFont >= (int)size)
	{
		for (int i = size; i < iFont; i++)
		{
			m_pFonts.push_back(-1);
		}
		m_pFonts.push_back(iFontManager);
	}
	else
		m_pFonts[iFont] = iFontManager;
}

DXFontData* DXDialog::GetFont(int iFont)
{
	if (NULL == m_pManager)
		return NULL;
	else
		return m_pManager->GetFont(iFont);
}

void DXDialog::AddControl(DXControl *pControl)
{
	m_pControls.push_back(pControl);
	pControl->SetParentDialog(this);
	if (NULL == m_pFocus)
		SetFocus(pControl);
}

DXStatic* DXDialog::AddStatic(int ID, int x, int y, int width, int height, LPWSTR szText)
{
	DXStatic *pNewStatic = new DXStatic();
	AddControl(pNewStatic);

	pNewStatic->SetID(ID);
	pNewStatic->SetLocation(x, y);
	pNewStatic->SetSize(width, height);
	pNewStatic->UpdateRect();
	pNewStatic->SetText(szText);

	return pNewStatic;
}

DXButton* DXDialog::AddButton(int ID, int x, int y, int width, int height, LPWSTR szText)
{
	DXButton *pNewStatic = new DXButton();
	AddControl(pNewStatic);

	pNewStatic->SetID(ID);
	pNewStatic->SetLocation(x, y);
	pNewStatic->SetSize(width, height);
	pNewStatic->UpdateRect();
	pNewStatic->SetText(szText);

	return pNewStatic;
}

DXSlider* DXDialog::AddSlider(int ID, int x, int y, int width, int height,
							  int sx, int sy, int swidth, int sheight, LPWSTR szText)
{
	DXSlider *pNewSlider = new DXSlider();
	AddControl(pNewSlider);

	pNewSlider->SetID(ID);
	pNewSlider->SetLocation(x, y);
	pNewSlider->SetSize(width, height);
	pNewSlider->UpdateRect();
	pNewSlider->SetText(szText);
	pNewSlider->SetSlider(sx, sy, swidth, sheight);

	return pNewSlider;
}

DXListBox* DXDialog::AddListBox(int ID, int x, int y, int width, int height,
								int sx, int sy, int swidth, int sheight, LPWSTR szText)
{
	DXListBox *pNewListBox = new DXListBox();
	AddControl(pNewListBox);

	pNewListBox->SetID(ID);
	pNewListBox->SetLocation(x, y);
	pNewListBox->SetSize(width, height);
	pNewListBox->UpdateRect();
	pNewListBox->SetText(szText);
	pNewListBox->SetBox(sx, sy, swidth, sheight);

	return pNewListBox;
}

void DXDialog::SetNormal(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl == m_pFocus)
	{
		if (NULL == NextFocus())
			PrevFocus();
	}
	pControl->SetState(DXSTATE_NORMAL);
}

void DXDialog::SetNormal(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetNormal(p);
			break;
		}
	}
}

void DXDialog::SetDisabled(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl == m_pFocus)
	{
		if (NULL == NextFocus())
			PrevFocus();
	}
	pControl->SetState(DXSTATE_DISABLED);
}

void DXDialog::SetDisabled(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetDisabled(p);
			break;
		}
	}
}

void DXDialog::SetHidden(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl == m_pFocus)
	{
		if (NULL == NextFocus())
			PrevFocus();
	}
	pControl->SetState(DXSTATE_HIDDEN);

}

void DXDialog::SetHidden(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetHidden(p);
			break;
		}
	}
}

bool DXDialog::SetFocus(DXControl* pControl)
{
	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (NULL != m_pFocus)
		m_pFocus->SetState(DXSTATE_NORMAL);

	if (pControl->SetState(DXSTATE_FOCUS))
	{
		m_pFocus = pControl;
		return true;
	}
	else
	{
		m_pFocus = NULL;
		return false;
	}
}

void DXDialog::SetFocus(int ID)
{
	m_pFocus->SetState(DXSTATE_NORMAL);
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
		{
			SetFocus(p);
			break;
		}
	}
}

DXControl* DXDialog::GetFocus()
{
	return m_pFocus;
}

DXControl* DXDialog::PrevFocus()
{
	DXControl *pNewFocus = NULL;
	auto iter = find(m_pControls.begin(), m_pControls.end(), m_pFocus);
	while (iter != m_pControls.begin())
	{
		iter--;
		if ((*iter)->SetState(DXSTATE_FOCUS))
		{
			pNewFocus = *iter;
			break;
		}
	}
	if (pNewFocus != NULL)
		SetFocus(pNewFocus);

	return pNewFocus;
}

DXControl* DXDialog::NextFocus()
{
	DXControl *pNewFocus = NULL;
	auto iter = find(m_pControls.begin(), m_pControls.end(), m_pFocus);
	if (iter == m_pControls.end())
		return NULL;

	iter++;
	while (iter != m_pControls.end())
	{
		if ((*iter)->SetState(DXSTATE_FOCUS))
		{
			pNewFocus = *iter;
			break;
		}
		iter++;
	}
	if (pNewFocus != NULL)
		SetFocus(pNewFocus);

	return pNewFocus;
}

void DXDialog::SetPressed(DXControl* pControl)
{
	if (NULL == pControl)
		return;

	if (find(m_pControls.begin(), m_pControls.end(), pControl) == m_pControls.end())
	{
		m_pControls.push_back(pControl);
		pControl->SetParentDialog(this);
	}

	if (pControl->SetState(DXSTATE_PRESSED))
		m_pPressed = pControl;
	else
		m_pPressed = NULL;
}

void DXDialog::SetPressed(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
			SetPressed(p);
	}
}

DXControl* DXDialog::GetPressed()
{
	DXControl *pReturn = m_pPressed;
	if (NULL != m_pPressed)
	{
		m_pPressed->SetState(DXSTATE_FOCUS);
		m_pPressed = NULL;
	}
	return pReturn;
}

DXControl* DXDialog::GetControl(int ID)
{
	for (auto p : m_pControls)
	{
		if (ID == p->GetID())
			return p;
	}
	return NULL;
}

void DXDialog::Render(double fTime, float fElapsedTime)
{
	for (auto p : m_pControls)
	{
		p->Render(fTime, fElapsedTime);
	}
}

void DXDialog::DrawControlText(LPWSTR szText, RECT *prcRect, DXElement *pElement)
{
	RECT destRect;
	destRect.left   = (int)((*prcRect).left   * m_pManager->m_fScale);
	destRect.right  = (int)((*prcRect).right  * m_pManager->m_fScale);
	destRect.top    = (int)((*prcRect).top    * m_pManager->m_fScale);
	destRect.bottom = (int)((*prcRect).bottom * m_pManager->m_fScale);

	//OffsetRect(&destRect, m_x * m_pManager->m_fScale, m_y * m_pManager->m_fScale);
	//	if (m_CaptionHeight)
	//		OffsetRect(&destRect, 0, m_CaptionHeight);

	int iFont = 0;
	DWORD dwFormat = DT_CENTER | DT_VCENTER;
	D3DXCOLOR TextColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	if (NULL != pElement)
	{
		iFont = pElement->iFont;
		dwFormat = pElement->dwFormat;
		TextColor = pElement->TextColor;
	}

	DXFontData *pFontData = GetFont(m_pFonts[iFont]);
	pFontData->pFont->DrawText(NULL, szText, -1, &destRect, dwFormat, TextColor);
}

void DXDialog::DrawControlRect(RECT rc, DWORD dwColor)
{
	IDirect3DDevice9 *pd3dDevice = m_pManager->GetD3DDevice();

	RECT destRect;
	destRect.left   = (int)(rc.left   * m_pManager->m_fScale);
	destRect.right  = (int)(rc.right  * m_pManager->m_fScale);
	destRect.top    = (int)(rc.top    * m_pManager->m_fScale);
	destRect.bottom = (int)(rc.bottom * m_pManager->m_fScale);

	OffsetRect(&destRect, (int)(m_x * m_pManager->m_fScale), (int)(m_y * m_pManager->m_fScale));

	RHWCOLORVERTEX *pVertices;
	m_pManager->m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { (float)destRect.left,  (float)destRect.bottom, 0.0f, 1.0f, dwColor };
	pVertices[1] = { (float)destRect.left,  (float)destRect.top,    0.0f, 1.0f, dwColor };
	pVertices[2] = { (float)destRect.right, (float)destRect.bottom, 0.0f, 1.0f, dwColor };
	pVertices[3] = { (float)destRect.right, (float)destRect.top,    0.0f, 1.0f, dwColor };

	m_pManager->m_pVertexBuffer->Unlock();


	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pd3dDevice->SetTexture(0, NULL);

	pd3dDevice->SetStreamSource(0, m_pManager->m_pVertexBuffer, 0, sizeof(RHWCOLORVERTEX));
	pd3dDevice->SetFVF(RHWCOLOR_FVF);
	pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void DXDialog::DrawControlRectFrame(RECT rc, DWORD dwColor)
{
	IDirect3DDevice9 *pd3dDevice = m_pManager->GetD3DDevice();

	RECT destRect;
	destRect.left   = (int)(rc.left   * m_pManager->m_fScale);
	destRect.right  = (int)(rc.right  * m_pManager->m_fScale);
	destRect.top    = (int)(rc.top    * m_pManager->m_fScale);
	destRect.bottom = (int)(rc.bottom * m_pManager->m_fScale);

	OffsetRect(&destRect, (int)(m_x * m_pManager->m_fScale), (int)(m_y * m_pManager->m_fScale));

	RHWCOLORVERTEX *pVertices;
	m_pManager->m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { (float)destRect.left,  (float)destRect.bottom, 0.0f, 1.0f, dwColor };
	pVertices[1] = { (float)destRect.left,  (float)destRect.top,    0.0f, 1.0f, dwColor };
	pVertices[2] = { (float)destRect.right, (float)destRect.top,    0.0f, 1.0f, dwColor };
	pVertices[3] = { (float)destRect.right, (float)destRect.bottom, 0.0f, 1.0f, dwColor };
	pVertices[4] = { (float)destRect.left,  (float)destRect.bottom, 0.0f, 1.0f, dwColor };

	m_pManager->m_pVertexBuffer->Unlock();


	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pd3dDevice->SetTexture(0, NULL);

	pd3dDevice->SetStreamSource(0, m_pManager->m_pVertexBuffer, 0, sizeof(RHWCOLORVERTEX));
	pd3dDevice->SetFVF(RHWCOLOR_FVF);
	pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, 4);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


void DXDialog::PlayFocusSound()
{
	if (NULL != m_pManager->m_pSoundCursor)
		m_pManager->m_pSoundCursor->Play(0);
}

void DXDialog::PlayPressSound()
{
	if (NULL != m_pManager->m_pSoundDecide)
		m_pManager->m_pSoundDecide->Play(0);
}

void DXDialog::PlaySelectSound()
{
	if (NULL != m_pManager->m_pSoundSelect)
		m_pManager->m_pSoundSelect->Play(0);
}

void DXDialog::PlayCancelSound()
{
	if (NULL != m_pManager->m_pSoundCancel)
		m_pManager->m_pSoundCancel->Play(0);
}

void DXDialog::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			PrevFocus();
			PlayFocusSound();
			break;
		case VK_DOWN:
			NextFocus();
			PlayFocusSound();
			break;
		case VK_RETURN:
			SetPressed(m_pFocus);
			PlayPressSound();
			break;
		default:
			break;
		}
	default:
		break;
	}
}

//
//DXControl
//

DXControl::DXControl()
{
	m_ID = -1;
	m_x = 0;
	m_y = 0;
	m_Width = 0;
	m_Height = 0;
	m_pDlgParent = NULL;
}

DXControl::~DXControl()
{
}

void DXControl::UpdateRect()
{
	SetRect(&m_rectBound, m_x, m_y, m_x + m_Width, m_y + m_Height);
}

void DXControl::SetParentDialog(DXDialog *pDialog)
{
	m_pDlgParent = pDialog;
}

void DXControl::AddElement(DXElement *pElement)
{
	m_pElements.push_back(pElement);
}

void DXControl::AddElement(DXElement Element)
{
	DXElement *pNewElement = new DXElement();
	pNewElement->iFont = Element.iFont;
	pNewElement->dwFormat = Element.dwFormat;
	pNewElement->TextColor = Element.TextColor;

	m_pElements.push_back(pNewElement);
}

void DXControl::AddElement(int iFont, DWORD dwFormat, D3DXCOLOR TextColor)
{
	DXElement *pNewElement = new DXElement();
	pNewElement->iFont = iFont;
	pNewElement->dwFormat = dwFormat;
	pNewElement->TextColor = TextColor;

	m_pElements.push_back(pNewElement);
}

//
//DXStatic
//

DXStatic::DXStatic()
{
	DXControl::DXControl();
	m_Type = DXCONTROL_STATIC;
	m_State = DXSTATE_NORMAL;
}

DXStatic::~DXStatic()
{
	DXControl::~DXControl();
}

BOOL DXStatic::SetState(DXCONTROL_STATE state)
{
	if (state == DXSTATE_FOCUS ||
		state == DXSTATE_MOUSEOVER ||
		state == DXSTATE_PRESSED)
		return FALSE;
	else
	{
		m_State = state;
		return TRUE;
	}
}

void DXStatic::Render(double fTime, float fElapsedTime)
{
	if (m_szText)
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound,
			m_pElements.size() > 0 ? m_pElements[0] : NULL);
}

//
//DXButton
//

DXButton::DXButton()
{
	DXControl::DXControl();
	m_Type = DXCONTROL_BUTTON;
	m_State = DXSTATE_NORMAL;
}

DXButton::~DXButton()
{
	DXControl::~DXControl();
}

BOOL DXButton::SetState(DXCONTROL_STATE state)
{
	if (DXSTATE_DISABLED == m_State || DXSTATE_HIDDEN == m_State)
	{
		if (DXSTATE_NORMAL == state)
		{
			m_State = state;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		m_State = state;
		return TRUE;
	}

}

void DXButton::Render(double fTime, float fElapsedTime)
{
	if (m_szText)
	{
		switch (m_State)
		{
		case DXSTATE_NORMAL:
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[0]);
			break;
		case DXSTATE_DISABLED:
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[2]);
			break;
		case DXSTATE_HIDDEN:
			break;
		case DXSTATE_FOCUS:
			m_pDlgParent->DrawControlRect(m_rectBound, BLACK);
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);
			break;
		case DXSTATE_MOUSEOVER:
			break;
		case DXSTATE_PRESSED:
			m_pDlgParent->DrawControlRect(m_rectBound, GRAY);
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);
			break;
		default:
			break;
		}
	}
}

//DXSlider

DXSlider::DXSlider()
{
	DXControl::DXControl();
	m_Type = DXCONTROL_BUTTON;
	m_State = DXSTATE_NORMAL;
	m_iBeg = 0;
	m_iEnd = 100;
	m_iCurrent = 100;
	m_colorLeft = BLACK;
	m_colorRight = BLACK;
}

DXSlider::~DXSlider()
{
	DXControl::~DXControl();
}

BOOL DXSlider::SetState(DXCONTROL_STATE state)
{
	if (DXSTATE_DISABLED == m_State || DXSTATE_HIDDEN == m_State)
	{
		if (DXSTATE_NORMAL == state)
		{
			m_State = state;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		if (state == DXSTATE_PRESSED)
			return FALSE;
		else
		{
			m_State = state;
			return TRUE;
		}
	}

}

void DXSlider::Render(double fTime, float fElapsedTime)
{
	RECT rcLeft ,rcRight;
	WCHAR szBuffer[MAX_PATH];
	int height = m_rectSlider.bottom - m_rectSlider.top;
	SetRect(&rcLeft, m_rectSlider.left - 3 * height / 4, m_rectSlider.top,
					 m_rectSlider.left -     height / 8, m_rectSlider.bottom);
	SetRect(&rcRight, m_rectSlider.right +     height / 8, m_rectSlider.top, 
					  m_rectSlider.right + 3 * height / 4, m_rectSlider.bottom);
	swprintf_s(szBuffer, TEXT("%d"), m_iCurrent);

	switch (m_State)
	{
	case DXSTATE_NORMAL:
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[0]);
		m_pDlgParent->DrawControlText(szBuffer, &m_rectSlider, m_pElements[0]);
		break;
	case DXSTATE_DISABLED:
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[2]);
		break;
	case DXSTATE_HIDDEN:
		break;
	case DXSTATE_FOCUS:
		m_pDlgParent->DrawControlRect(m_rectBound, BLACK);
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);
		m_pDlgParent->DrawControlText(szBuffer, &m_rectSlider, m_pElements[3]);
		m_pDlgParent->DrawControlRect(rcLeft, m_colorLeft);
		m_pDlgParent->DrawControlText(L"<", &rcLeft, m_pElements[1]);
		m_pDlgParent->DrawControlRect(rcRight, m_colorRight);
		m_pDlgParent->DrawControlText(L">", &rcRight, m_pElements[1]);
		break;
	case DXSTATE_MOUSEOVER:
		break;
	case DXSTATE_PRESSED:
		break;
	default:
		break;
	}

	if (BLACK != m_colorLeft || BLACK != m_colorRight)
	{
		m_fPressTime += fElapsedTime;
		if (m_fPressTime >= 240.0f)
		{
			m_colorLeft = BLACK;
			m_colorRight = BLACK;
		}
	}
}

void DXSlider::SetSlider(int x, int y, int width, int height)
{
	SetRect(&m_rectSlider, x, y, x + width, y + height);
}

void DXSlider::SetRange(int iBeg, int iEnd)
{
	m_iBeg = iBeg;
	m_iEnd = iEnd;
}

void DXSlider::SetValue(int iValue)
{
	m_iCurrent = max(m_iBeg, min(m_iEnd, iValue));
}

void DXSlider::AddValue(int iDelta)
{
	SetValue(m_iCurrent + iDelta);
	m_colorLeft = BLACK;
	m_colorRight = GRAY;
	m_fPressTime = 0.0f;
}

void DXSlider::SubValue(int iDelta)
{
	SetValue(m_iCurrent - iDelta);
	m_colorLeft = GRAY;
	m_colorRight = BLACK;
	m_fPressTime = 0.0f;
}

int DXSlider::GetValue()
{
	return m_iCurrent;
}

//ListBox

DXListBox::DXListBox()
{
	DXControl::DXControl();
	m_Type = DXCONTROL_BUTTON;
	m_State = DXSTATE_NORMAL;
	m_szList.push_back(L"--default--");
	m_iCurrent = 0;
	m_bListOpen = false;
	SetListSize(3);
	m_yDelta = 0.0f;
	m_colorU = BLACK;
	m_colorD = BLACK;
}

DXListBox::~DXListBox()
{
	DXControl::~DXControl();
}

BOOL DXListBox::SetState(DXCONTROL_STATE state)
{
	if (DXSTATE_DISABLED == m_State || DXSTATE_HIDDEN == m_State)
	{
		if (DXSTATE_NORMAL == state)
		{
			m_State = state;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		if (state == DXSTATE_PRESSED)
			return FALSE;
		else
		{
			m_State = state;
			return TRUE;
		}
	}
}

void DXListBox::Render(double fTime, float fElapsedTime)
{
	switch (m_State)
	{
	case DXSTATE_NORMAL:
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[0]);
		m_pDlgParent->DrawControlText(m_szList[m_iCurrent], &m_rectBox, m_pElements[0]);
		break;
	case DXSTATE_DISABLED:
		m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[2]);
		break;
	case DXSTATE_HIDDEN:
		break;
	case DXSTATE_FOCUS:
		if (!m_bListOpen)
		{
			m_pDlgParent->DrawControlRect(m_rectBound, BLACK);
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);

			if (m_yDelta > 0.0f)
			{
				m_yDelta = max(0.0f, m_yDelta - 0.8f * fElapsedTime);
				SetRect(&m_rectBox, m_xBox, m_yBox - (int)m_yDelta, 
						m_xBox + m_widthBox, m_yBox + m_heightBox + (int)m_yDelta - 1);
			}

			m_pDlgParent->DrawControlRectFrame(m_rectBox, BLACK);
			m_pDlgParent->DrawControlText(m_szList[m_iCurrent], &m_rectBox, m_pElements[3]);

			RECT rcArrowU, rcArrowD;
			int height = m_heightBox / 2;
			int left = m_rectBox.right + height / 4;
			int right = m_rectBox.right + 3 * height / 2;
			SetRect(&rcArrowU, left, m_rectBox.top, right, m_rectBox.top + height);
			SetRect(&rcArrowD, left, m_rectBox.bottom - height, right, m_rectBox.bottom + 1);
			m_pDlgParent->DrawControlRect(rcArrowU, BLACK);
			m_pDlgParent->DrawControlRect(rcArrowD, BLACK);
			OffsetRect(&rcArrowU, 0, -height / 5);
			OffsetRect(&rcArrowD, 0, -height / 5);
			m_pDlgParent->DrawControlText(L"▴", &rcArrowU, m_pElements[1]);
			m_pDlgParent->DrawControlText(L"▾", &rcArrowD, m_pElements[1]);
		}
		else
		{
			m_pDlgParent->DrawControlRect(m_rectBound, GRAY);
			m_pDlgParent->DrawControlText(m_szText, &m_rectBound, m_pElements[1]);

			if (m_yDelta < m_yDeltaMax)
			{
				m_yDelta = min(m_yDeltaMax, m_yDelta + 0.8f * fElapsedTime);
				SetRect(&m_rectBox, m_xBox, m_yBox - (int)m_yDelta, 
						m_xBox + m_widthBox, m_yBox + m_heightBox + (int)m_yDelta - 1);
				m_pDlgParent->DrawControlRectFrame(m_rectBox, BLACK);
				RECT rcText;
				SetRect(&rcText, m_xBox + m_heightBox / 4, m_yBox, 
					m_xBox + m_widthBox - m_heightBox / 4, m_yBox + m_heightBox);
				m_pDlgParent->DrawControlText(m_szList[m_iList], &rcText, m_pElements[3]);
			}
			else
			{
				m_pDlgParent->DrawControlRectFrame(m_rectBox, BLACK);
				for (int i = 0; i < m_cList; i++)
				{
					RECT rcText;
					int iOffset = m_heightBox * (i - m_cList / 2);
					int index = m_iList + i - m_cList / 2;
					if (index < 0)
						index += (int)m_szList.size();
					if (index >= (int)m_szList.size())
						index -= (int)m_szList.size();

					SetRect(&rcText, m_xBox + m_heightBox / 4, m_yBox + iOffset,
						m_xBox + m_widthBox - m_heightBox / 4, m_yBox + iOffset + m_heightBox);
					if (i == m_cList / 2)
					{
						m_pDlgParent->DrawControlRect(rcText, BLACK);
						m_pDlgParent->DrawControlText(m_szList[index], &rcText, m_pElements[4]);
					}
					else
						m_pDlgParent->DrawControlText(m_szList[index], &rcText, m_pElements[3]);
				}
			}

			RECT rcArrowU, rcArrowD, rcArrow;
			int height = m_heightBox / 2;
			int left = m_rectBox.right + height / 4;
			int right = m_rectBox.right + 3 * height / 2;
			SetRect(&rcArrow, left, m_rectBox.top, right - 1, m_rectBox.bottom);
			SetRect(&rcArrowU, left, m_rectBox.top, right, m_rectBox.top + height);
			SetRect(&rcArrowD, left, m_rectBox.bottom - height, right, m_rectBox.bottom + 1);
			m_pDlgParent->DrawControlRectFrame(rcArrow, BLACK);
			m_pDlgParent->DrawControlRect(rcArrowU, m_colorU);
			m_pDlgParent->DrawControlRect(rcArrowD, m_colorD);
			OffsetRect(&rcArrowU, 0, -height / 5);
			OffsetRect(&rcArrowD, 0, -height / 5);
			m_pDlgParent->DrawControlText(L"▴", &rcArrowU, m_pElements[1]);
			m_pDlgParent->DrawControlText(L"▾", &rcArrowD, m_pElements[1]);

			if (BLACK != m_colorU || BLACK != m_colorD)
			{
				m_fPressTime += fElapsedTime;
				if (m_fPressTime >= 240.0f)
				{
					m_colorU = BLACK;
					m_colorD = BLACK;
				}
			}
		}
		break;
	case DXSTATE_MOUSEOVER:
		break;
	case DXSTATE_PRESSED:
		break;
	default:
		break;
	}
}

void DXListBox::SetBox(int x, int y, int width, int height)
{
	m_xBox = x;
	m_yBox = y;
	m_widthBox = width;
	m_heightBox = height;
	SetRect(&m_rectBox, m_xBox, m_yBox, m_xBox + m_widthBox, m_yBox + m_heightBox - 1);
}

void DXListBox::AddListText(WCHAR* szList)
{
	m_szList.push_back(szList);
}

void DXListBox::ChangeListText(int index, WCHAR* szList)
{
	int size = (int)m_szList.size();
	if (index >= size)
	{
		for (int i = size; i <= size; i++)
			m_szList.push_back(NULL);
	}
	m_szList[index] = szList;
}

void DXListBox::SetListSize(int cList)
{
	m_cList = cList;
	m_yDeltaMax = (float)(m_cList - 1) / 2.0f * (float)m_heightBox;
}

void DXListBox::OpenList()
{
	m_bListOpen = true;
	m_iList = m_iCurrent;
}

bool DXListBox::CloseList(bool bSelect)
{
	m_bListOpen = false;
	if (bSelect)
	{
		if (m_iCurrent != m_iList)
		{
			m_iCurrent = m_iList;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool DXListBox::IsListOpen()
{
	return m_bListOpen;
}

void DXListBox::SelectUp()
{
	m_iList--;
	if (m_iList < 0)
		m_iList += (int)m_szList.size();

	m_colorU = GRAY;
	m_colorD = BLACK;
	m_fPressTime = 0.0f;
}

void DXListBox::SelectDown()
{
	m_iList++;
	if (m_iList >= (int)m_szList.size())
		m_iList -= (int)m_szList.size();

	m_colorU = BLACK;
	m_colorD = GRAY;
	m_fPressTime = 0.0f;
}

void DXListBox::SetSelect(int index)
{
	m_iCurrent = max(0, min((int)m_szList.size(), index));
}

int DXListBox::GetSelect()
{
	return m_iCurrent;
}
