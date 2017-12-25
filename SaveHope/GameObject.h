#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include <algorithm>
#include "Game.h"
#include "DXObject.h"
#include "GameAudio.h"

//
//Curtain
//

class Curtain
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	RECTRANGE				m_Range;

	BOOL					m_IsOn;
	int						m_iLayer;
	int						m_AlphaBeg, m_AlphaEnd;
	float					m_fDuration;
	float					m_fPassTime;
	void					(*m_pOnCurtainEnd)();

public:
	Curtain();
	Curtain(float left, float top, float right, float bottom);
	~Curtain();

	void Resize(float left, float top, float right, float bottom);
	void Set(int AlphaBeg, int AlphaEnd, float fDuration, void(*pFunc)() = NULL, int iLayer = 0);
	BOOL IsOn(int iLayer = -1);

	void RenderAlpha(int Alpha);
	void Render(float fElapsedTime);
};

HRESULT CreateCurtain();
HRESULT DeleteCurtain();

void ResizeCurtain();
void SetCurtain(int AlphaBeg, int AlphaEnd, float fDuration, void(*pFunc)() = NULL, int iLayer = 0);
BOOL IsCurtainOn(int iLayer = -1);
void RenderCutain(float fElapsedTime);
void RenderAlphaCurtain(int Alpha);

//
//Background
//

class Background
{
public:
	Background(bool bClear = false);
	~Background();

	BOOL Update(double fTime, float fElapsedTime);
	BOOL Render(double fTime, float fElapsedTime);

	void SetSPB(float fSPB);
	void SetOffset(float x, float y);
	void Reverse(bool bReverse);

private:
	IDirect3DDevice9 *m_pd3dDevice;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	D3DXVECTOR2 m_vOffset;
	float m_timeUpdate;
	float m_fSPB;
	int m_cBeam;
	int m_iLen;

	bool m_bReverse;
	float m_fSize;
	int m_xCount, m_yCount;

	struct Beam
	{
		int x[7];
		int y[7];
	};
	std::vector<Beam> m_vecBeam;
};

//
//BackBoard
//

class Board
{
public:
	Board();
	~Board();
	void Set(int width);
	void Render(float fElapsedTime);

	bool IsReady();

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	IDirect3DTexture9		*m_pTexture;
	IDirect3DVertexBuffer9  *m_pVertBoard;
	IDirect3DVertexBuffer9	*m_pVertLogo;

	float m_fScale;
	int m_width;
};

//
//MessageBox
//

class Box
{
public:
	Box();
	~Box();
	void Set(int xCenter, int yCenter, int width, int height, float fDeep = 0.0f);
	void Render(float fElapsedTime);

	void Close(void(*pFunc)() = NULL);
	bool IsReady();
	bool IsClosed();

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	IDirect3DTexture9		*m_pTexture;
	IDirect3DVertexBuffer9  *m_pVertBox;

	int m_xCenter, m_yCenter, m_width, m_height;
	float m_fScale, m_fSize, m_x[4], m_y[4], m_fDeep;
	bool m_bOpen;
	void(*m_pOnBoxClosed)();
};

//
//CheckBox
//

#define WM_CBCLOSED	WM_USER + 1

class CheckBox
{
public:
	CheckBox();
	~CheckBox();

	void Set(int width, int height, WCHAR* szText);
	void AddOption(int x, int y, int width, int height, WCHAR* szOption);
	void SetOption(int iOption);
	void Close();

	bool IsOpen();
	bool GetOption(int *iOption);

	void OnLostDevice();
	void OnResetDevice();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(float fElapsedTime);

private:
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	IDirect3DVertexBuffer9	*m_pVertText;
	ID3DXFont				*m_pFont;
	Box						*m_pBox;
	Sound					*m_pSEChoose, *m_pSECancel, *m_pSEDecide;

	bool m_bOpen, m_bDecide;
	int m_width, m_height;
	float m_fScale;
	WCHAR m_szText[MAX_PATH];

	int m_iOption;
	struct Option
	{
		RECT rectOption;
		WCHAR szOption[MAX_PATH];
	};
	std::vector<Option> m_vecOption;
};

void SetCheckBox(int width, int height, WCHAR* szText);
void AddCheckBoxOption(int x, int y, int width, int height, WCHAR* szOption);
void SetCheckBoxOption(int iOption);
bool IsCheckBoxOpen();
bool GetCheckBoxOption(int *iOption);

void CheckBoxOnLostDevice();
void CheckBoxOnResetDevice();

void RenderCheckBox(float fElapsedTime);
void OnMsgProcCheckBox(UINT message, WPARAM wParam, LPARAM lParam);

#endif // !GAMEOBJECT_H