#include "SceneMidSave.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "SaveData.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace MidSaveClass;

namespace MidSaveData
{
	const float fSpacing = 64.0f;
	float fScale;
	float fActSpacing;

	bool bConfirm;
	bool bSelect;
	bool bSave;
	bool bOver;

	bool bKeyUp[6];
	float fKeyUp[6];

	SaveInfo arrSaveInfo[5];
	SaveCheckBox *pSaveCheckBox;
	DataBox *pDataBox;
	ID3DXFont *pFont;
	ID3DXFont *pSFont;

	Sound *pSE;
}

using namespace MidSaveData;

//
//CheckBox
//

SaveCheckBox::SaveCheckBox()
{
	m_pd3dDevice = DXUTGetD3DDevice();

	m_pd3dDevice->CreateVertexBuffer(8 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertShade, NULL);

	for (int i = 0; i < 2; i++)
	{
		RHWCOLORVERTEX *pVertices;
		m_pVertShade->Lock(4 * i * sizeof(RHWCOLORVERTEX), 0, (void**)&pVertices, 0);

		float left = (13.0f + 2.5f * i) * fActSpacing;
		float right = (14.5f + 2.5f * i) * fActSpacing;
		float top = 8.5f * fActSpacing;
		float bottom = 9.5f * fActSpacing;

		pVertices[0] = { left,	bottom, 0.0f, 1.0f, BLACK };
		pVertices[1] = { left,	top,	0.0f, 1.0f, BLACK };
		pVertices[2] = { right, bottom, 0.0f, 1.0f, BLACK };
		pVertices[3] = { right, top,	0.0f, 1.0f, BLACK };

		m_pVertShade->Unlock();
	}

	m_pBox = new Box();
	m_iFocus = 0;
}

SaveCheckBox::~SaveCheckBox()
{
	SAFE_DELETE(m_pBox);
	SAFE_RELEASE(m_pVertShade);
	m_pd3dDevice = NULL;
}

void SaveCheckBox::Render(float fElapsedTime)
{
	m_pBox->Render(fElapsedTime);

	if (m_pBox->IsReady())
	{
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pd3dDevice->SetStreamSource(0, m_pVertShade, 0, sizeof(RHWCOLORVERTEX));
		m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * m_iFocus, 2);

		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		RECT rcText;
		SetGridRect(&rcText, 10.0f, 7.0f, 20.0f, 8.0f, fActSpacing);
		pFont->DrawTextW(NULL, bConfirm ? L"是否覆盖原存档" : L"是否保存当前进度", -1, &rcText, DT_CENTER | DT_VCENTER, BLACK);
		SetGridRect(&rcText, 13.0f, 8.5f, 14.5f, 9.5f, fActSpacing);
		pFont->DrawTextW(NULL, L"是", -1, &rcText, DT_CENTER | DT_VCENTER, m_iFocus == 0 ? WHITE : BLACK);
		SetGridRect(&rcText, 15.5f, 8.5f, 17.0f, 9.5f, fActSpacing);
		pFont->DrawTextW(NULL, L"否", -1, &rcText, DT_CENTER | DT_VCENTER, m_iFocus == 1 ? WHITE : BLACK);
	}
}

void SaveCheckBox::SetFocus(int iFocus)
{
	m_iFocus = iFocus;
}

int SaveCheckBox::GetFocus()
{
	return m_iFocus;
}

void SaveCheckBox::Open()
{
	m_pBox->Set(960, 530, 192, 64);
}

bool SaveCheckBox::IsReady()
{
	return m_pBox->IsReady();
}

void SaveCheckBox::Close()
{
	m_pBox->Close();
}

bool SaveCheckBox::IsClosed()
{
	return m_pBox->IsClosed();
}

//
//DataBox
//

DataBox::DataBox()
{
	m_pd3dDevice = DXUTGetD3DDevice();

	m_pd3dDevice->CreateVertexBuffer(75 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertFrame, NULL);
	m_pd3dDevice->CreateVertexBuffer(60 * sizeof(RHWCOLORVERTEX), 0, RHWCOLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertShade, NULL);

	for (int i = 0; i < 5; i++)
	{
		float left   = 6.0f * fActSpacing;
		float right  = 24.0f * fActSpacing;
		float top    = (1.2f + 3.0f * i) * fActSpacing;
		float bottom = (3.6f + 3.0f * i) * fActSpacing;

		RHWCOLORVERTEX *pVertices;

		m_pVertFrame->Lock(5 * i * sizeof(RHWCOLORVERTEX), 0, (void**)&pVertices, 0);
		pVertices[0] = { left,	 bottom, 1.0f, 1.0f, GRAY };
		pVertices[1] = { right,	 bottom, 1.0f, 1.0f, GRAY };
		pVertices[2] = { right,	 top,	 1.0f, 1.0f, GRAY };
		pVertices[3] = { left,	 top,	 1.0f, 1.0f, GRAY };
		pVertices[4] = { left,	 bottom, 1.0f, 1.0f, GRAY };
		pVertices[25] = { left,	 bottom, 1.0f, 1.0f, WHITE };
		pVertices[26] = { right, bottom, 1.0f, 1.0f, WHITE };
		pVertices[27] = { right, top,	 1.0f, 1.0f, WHITE };
		pVertices[28] = { left,	 top,	 1.0f, 1.0f, WHITE };
		pVertices[29] = { left,	 bottom, 1.0f, 1.0f, WHITE };
		m_pVertFrame->Unlock();

		m_pVertShade->Lock(4 * i * sizeof(RHWCOLORVERTEX), 0, (void**)&pVertices, 0);
		pVertices[0] = { left,	 bottom, 1.0f, 1.0f, DD_GRAY };
		pVertices[1] = { left,	 top,	 1.0f, 1.0f, DD_GRAY };
		pVertices[2] = { right,	 bottom, 1.0f, 1.0f, DD_GRAY };
		pVertices[3] = { right,	 top,	 1.0f, 1.0f, DD_GRAY };
		pVertices[20] = { left,	 bottom, 1.0f, 1.0f, D_GRAY };
		pVertices[21] = { left,	 top,	 1.0f, 1.0f, D_GRAY };
		pVertices[22] = { right, bottom, 1.0f, 1.0f, D_GRAY };
		pVertices[23] = { right, top,	 1.0f, 1.0f, D_GRAY };
		m_pVertShade->Unlock();

		right = 8.0f * fActSpacing;
		bottom = (2.2f + 3.0f * i) * fActSpacing;

		m_pVertFrame->Lock((50 + 5 * i) * sizeof(RHWCOLORVERTEX), 0, (void**)&pVertices, 0);
		pVertices[0] = { left,	 bottom, 1.0f, 1.0f, GRAY };
		pVertices[1] = { right,	 bottom, 1.0f, 1.0f, GRAY };
		pVertices[2] = { right,	 top,	 1.0f, 1.0f, GRAY };
		pVertices[3] = { left,	 top,	 1.0f, 1.0f, GRAY };
		pVertices[4] = { left,	 bottom, 1.0f, 1.0f, GRAY };
		m_pVertFrame->Unlock();

		m_pVertShade->Lock((40 + 4 * i) * sizeof(RHWCOLORVERTEX), 0, (void**)&pVertices, 0);
		pVertices[0] = { left,	 bottom, 1.0f, 1.0f, WHITE };
		pVertices[1] = { left,	 top,	 1.0f, 1.0f, WHITE };
		pVertices[2] = { right,	 bottom, 1.0f, 1.0f, WHITE };
		pVertices[3] = { right,	 top,	 1.0f, 1.0f, WHITE };
		m_pVertShade->Unlock();
	}

	m_iFocus = 0;
}

DataBox::~DataBox()
{
	m_pd3dDevice = NULL;
	SAFE_RELEASE(m_pVertFrame);
	SAFE_RELEASE(m_pVertShade);
}

void DataBox::Render()
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int i = 0; i < 5; i++)
	{
		DWORD colorText = GRAY;

		if (i == m_iFocus)
		{
			colorText = WHITE;

			m_pd3dDevice->SetStreamSource(0, m_pVertShade, 0, sizeof(RHWCOLORVERTEX));
			m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20 + 4 * i, 2);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 40 + 4 * i, 2);

			m_pd3dDevice->SetStreamSource(0, m_pVertFrame, 0, sizeof(RHWCOLORVERTEX));
			m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 25 + 5 * i, 4);
		}
		else
		{
			m_pd3dDevice->SetStreamSource(0, m_pVertShade, 0, sizeof(RHWCOLORVERTEX));
			m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);

			m_pd3dDevice->SetStreamSource(0, m_pVertFrame, 0, sizeof(RHWCOLORVERTEX));
			m_pd3dDevice->SetFVF(RHWCOLOR_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 5 * i, 4);
			m_pd3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 50 + 5 * i, 4);
		}

		RECT rcText;
		WCHAR szBuffer[MAX_PATH];

		SetGridRect(&rcText, 6.0f, 1.2f + 3.0f * i, 8.0f, 2.2f + 3.0f * i, fActSpacing);

		swprintf_s(szBuffer, L"存档%d", i + 1);
		pSFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, i == m_iFocus ? DD_GRAY : GRAY);

		SetGridRect(&rcText, 9.0f, 1.6f + 3.0f * i, 23.0f, 3.2f + 3.0f * i, fActSpacing);

		if (arrSaveInfo[i].bExist)
		{
			swprintf_s(szBuffer, L"进度 - ");
			switch (arrSaveInfo[i].iDifficulty)
			{
			case 0:
				wcscat_s(szBuffer, L"简单");
				break;
			case 1:
				wcscat_s(szBuffer, L"普通");
				break;
			case 2:
				wcscat_s(szBuffer, L"困难");
				break;
			case 3:
				wcscat_s(szBuffer, L"欺负人");
				break;
			}
			switch (arrSaveInfo[i].iStory)
			{
			case 0:
			case 1:
				wcscat_s(szBuffer, L" S1");
				break;
			case 2:
				wcscat_s(szBuffer, L" S2");
				break;
			case 3:
				wcscat_s(szBuffer, L" S3");
				break;
			case 4:
				wcscat_s(szBuffer, L" CLEAR");
				break;
			}
			pSFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, colorText);

			swprintf_s(szBuffer, L"");
			for (int j = 0; j < 4; j++)
			{
				if (arrSaveInfo[i].iClear[j] == 1)
					wcscat_s(szBuffer, L"★");
				else
					wcscat_s(szBuffer, L"☆");
			}
			pSFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_BOTTOM, colorText);

			swprintf_s(szBuffer, L"%d-%02d-%02d %02d:%02d", arrSaveInfo[i].iYear, arrSaveInfo[i].iMonth,
				arrSaveInfo[i].iDay, arrSaveInfo[i].iHour, arrSaveInfo[i].iMinute);
			pSFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, colorText);

			swprintf_s(szBuffer, L"碎片收集 - %2d | %3d",
				arrSaveInfo[i].iGoldFragment, arrSaveInfo[i].iHopeFragment);
			pSFont->DrawTextW(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_BOTTOM, colorText);
		}
		else
		{
			pSFont->DrawTextW(NULL, L"(空)", -1, &rcText, DT_CENTER | DT_VCENTER, colorText);
		}

	}

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void DataBox::SetFocus(int iFocus)
{
	m_iFocus = iFocus;
}

int DataBox::GetFocus()
{
	return m_iFocus;
}

//
//Scene
//

SceneMidSave::SceneMidSave()
{
	pDataBox = NULL;
	pFont = NULL;
}

SceneMidSave::~SceneMidSave()
{
	End();
}

void SceneMidSave::Init()
{
	fScale = GetSettings().m_fScale;
	fActSpacing = fSpacing * fScale;

	bConfirm = false;
	bSelect = false;
	bSave = false;
	bOver = false;

	for (int i = 0; i < 6; i++)
	{
		bKeyUp[i] = false;
		fKeyUp[i] = 800.0f;
	}

	for (int i = 0; i < 5; i++)
	{
		GetSaveInfo(i + 1, &arrSaveInfo[i]);
	}

	pSaveCheckBox = new SaveCheckBox();
	pSaveCheckBox->Open();

	pDataBox = new DataBox();

	pSE = new Sound();
	pSE->Register(ID_SE_MS, ID_SUBMIX_SOUNDEFFECT);
	pSE->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SELECT, L"WAVE");
	
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(48.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(40.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
}

void SceneMidSave::End()
{
	SAFE_DELETE(pSE);
	SAFE_DELETE(pSaveCheckBox);
	SAFE_DELETE(pDataBox);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
}

void SceneMidSave::OnResetDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(48.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
	D3DXCreateFont(DXUTGetD3DDevice(), (int)(40.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
}

void SceneMidSave::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
}

void SceneMidSave::Update(double fTime, float fElapsedTime)
{
	GetInput();

	for (int i = 0; i < 6; i++)
	{
		if (fKeyUp[i] <= 0.0f)
			fKeyUp[i] = 0.0f;
		else
			fKeyUp[i] -= fElapsedTime;
	}

	if (!bConfirm)
	{
		if (pSaveCheckBox->IsReady())
		{
			if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
			{
				if (bKeyUp[2])
				{
					pSE->Play(0);
					pSaveCheckBox->SetFocus(0);
				}
				bKeyUp[2] = false;
			}
			else
				bKeyUp[2] = true;

			if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
			{
				if (bKeyUp[3])
				{
					pSE->Play(0);
					pSaveCheckBox->SetFocus(1);
				}
				bKeyUp[3] = false;
			}
			else
				bKeyUp[3] = true;

			if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
			{
				if (bKeyUp[5])
				{
					pSE->Play(0);
					pSaveCheckBox->SetFocus(1);
				}
				bKeyUp[5] = false;
			}
			else
				bKeyUp[5] = true;

			if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
			{
				if (bKeyUp[4])
				{
					pSE->Play(0);
					if (0 == pSaveCheckBox->GetFocus())
						bSelect = true;
					pSaveCheckBox->Close();
				}
				bKeyUp[4] = false;
			}
			else
				bKeyUp[4] = true;
		}

		if (pSaveCheckBox->IsClosed())
			bConfirm = true;
	}
	else
	{
		if (bSelect)
		{
			if (pSaveCheckBox->IsClosed())
			{
				if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
				{
					if (bKeyUp[0] || fKeyUp[0] == 0.0f)
					{
						pSE->Play(0);
						int index = pDataBox->GetFocus();
						if (index > 0)
							pDataBox->SetFocus(index - 1);

						fKeyUp[0] = 200.0f;
						if (bKeyUp[0])
							fKeyUp[0] = 800.0f;
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
						int index = pDataBox->GetFocus();
						if (index < 4)
							pDataBox->SetFocus(index + 1);

						fKeyUp[1] = 200.0f;
						if (bKeyUp[1])
							fKeyUp[1] = 800.0f;
					}
					bKeyUp[1] = false;
				}
				else
					bKeyUp[1] = true;

				if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
				{
					if (bKeyUp[5])
					{
						pSE->Play(0);
						bSelect = false;
						bConfirm = false;
						pSaveCheckBox->Open();
						pSaveCheckBox->SetFocus(0);
					}
					bKeyUp[5] = false;
				}
				else
					bKeyUp[5] = true;

				if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
				{
					if (bKeyUp[4])
					{
						pSE->Play(0);
						int index = pDataBox->GetFocus();
						if (arrSaveInfo[index].bExist)
						{
							pSaveCheckBox->Open();
							pSaveCheckBox->SetFocus(1);
						}
						else
							bSave = true;
					}
					bKeyUp[4] = false;
				}
				else
					bKeyUp[4] = true;
				
				if (bSave)
				{
					int index = pDataBox->GetFocus();
					WriteSaveData(index + 1);
					GetSaveInfo(index + 1, &arrSaveInfo[index]);
					bSelect = false;
				}
			}

			if (pSaveCheckBox->IsReady())
			{
				if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
				{
					if (bKeyUp[2])
					{
						pSE->Play(0);
						pSaveCheckBox->SetFocus(0);
					}
					bKeyUp[2] = false;
				}
				else
					bKeyUp[2] = true;

				if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
				{
					if (bKeyUp[3])
					{
						pSE->Play(0);
						pSaveCheckBox->SetFocus(1);
					}
					bKeyUp[3] = false;
				}
				else
					bKeyUp[3] = true;

				if (IsKeyDown(DIK_ESCAPE) || IsKeyDown(DIK_X))
				{
					if (bKeyUp[5])
					{
						pSE->Play(0);
						pSaveCheckBox->Close();
					}
					bKeyUp[5] = false;
				}
				else
					bKeyUp[5] = true;

				if (IsKeyDown(DIK_RETURN) || IsKeyDown(DIK_Z))
				{
					if (bKeyUp[4])
					{
						pSE->Play(0);
						if (0 == pSaveCheckBox->GetFocus())
							bSave = true;
						pSaveCheckBox->Close();
					}
					bKeyUp[4] = false;
				}
				else
					bKeyUp[4] = true;
			}
		}
		else
		{
			if (!bOver)
			{
				if (GetSaveData().m_iStory == 4)
					SetCurtain(127, 255, 500.0f, OpenSelectMenu);
				else
				{
					SetScene(STORY);
					SetCurtain(127, 255, 500.0f, GameStart);
				}
				bOver = true;
			}
		}
	}
}

void SceneMidSave::Render(double fTime, float fElapsedTime)
{
	pDataBox->Render();
	if (!pSaveCheckBox->IsClosed())
	{
		RenderAlphaCurtain(127);
		pSaveCheckBox->Render(fElapsedTime);
	}
}