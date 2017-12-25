#pragma once

#ifndef DXGUI_H
#define DXGUI_H

#include <vector>
#include <algorithm>
#include "DXUT.h"
#include "DXAudio.h"

class DXDialogResourceManager;
class DXDialog;
class DXControl;
class DXStatic;
class DXButton;
class DXSlider;
class DXCheckBox;
class DXEditBox;
class DXListBox;

enum DXCONTROL_TYPE
{
	DXCONTROL_STATIC,
	DXCONTROL_BUTTON,
	DXCONTROL_SLIDER,
	DXCONTROL_CHECKBOX,
	DXCONTROL_RADIOBUTTON,
	DXCONTROL_COMBOBOX,
	DXCONTROL_EDITBOX,
	DXCONTROL_LISTBOX,
};

enum DXCONTROL_STATE
{
	DXSTATE_NORMAL = 0,
	DXSTATE_DISABLED,
	DXSTATE_HIDDEN,
	DXSTATE_FOCUS,
	DXSTATE_MOUSEOVER,
	DXSTATE_PRESSED,
};

struct DXFontData
{
	WCHAR szFaceName[MAX_PATH];
	LONG nHeight;
	LONG nWeight;
	LPD3DXFONT pFont;
};

struct DXElement
{
	int iFont;
	DWORD dwFormat;
	D3DXCOLOR TextColor;
	DXElement(int font = 0, DWORD format = DT_CENTER | DT_VCENTER, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) :
		iFont(font), dwFormat(format), TextColor(color) {};
};

class DXDialogResourceManager
{
private:
	IDirect3DDevice9 *m_pd3dDevice;

	std::vector<DXDialog*> m_pDialogs;
	std::vector<DXFontData*> m_pFonts;


public:
	IDirect3DTexture9 *m_pTexture;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	float m_fScale;
	Sound *m_pSoundCursor;
	Sound *m_pSoundSelect;
	Sound *m_pSoundDecide;
	Sound *m_pSoundCancel;

public:
	DXDialogResourceManager();
	~DXDialogResourceManager();

	void SetScale(float fScale);

	int AddFont(LPCWSTR szFaceName, LONG height, LONG weight);
	void SetFont(int iFont);
	DXFontData *GetFont(int iFont);

	void SetCursorSound(int SoundID, int SubmixID, WCHAR* szFile);
	void SetSelectSound(int SoundID, int SubmixID, WCHAR* szFile);
	void SetDecideSound(int SoundID, int SubmixID, WCHAR* szFile);
	void SetCancelSound(int SoundID, int SubmixID, WCHAR* szFile);

	void SetCursorSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType);
	void SetSelectSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType);
	void SetDecideSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType);
	void SetCancelSound(int SoundID, int SubmixID, HMODULE hModule, int ID, LPCWSTR lpType);

	void RegisterDialog(DXDialog *pDialog, int IDD);
	void UnregisterDialog(DXDialog *pDialog);
	void UnregisterDialog(int IDD);

	HRESULT OnCreateDevice();
	HRESULT OnResetDevice();
	HRESULT OnLostDevice();
	IDirect3DDevice9* GetD3DDevice();

	DXDialog* GetDialog(int IDD);
};

class DXDialog
{
	friend class DXDialogResourceManager;

protected:
	DXDialogResourceManager *m_pManager;
	int m_ID;
	int m_x;
	int m_y;
	int m_Width;
	int m_Height;
	int m_CaptionHeight;

	DXControl *m_pFocus;
	DXControl *m_pPressed;
	std::vector<DXControl*> m_pControls;
	std::vector<int> m_pFonts;

public:
	DXDialog();
	virtual ~DXDialog();
	virtual void Init() {};
	virtual void Reset() {};
	virtual void Default();

	void SetFont(int iFont, LPWSTR szFaceName, LONG height, LONG weight);
	DXFontData* GetFont(int iFont);

	void AddControl(DXControl* pControl);
	DXStatic* AddStatic(int ID, int x, int y, int width, int height, LPWSTR szText);
	DXButton* AddButton(int ID, int x, int y, int width, int height, LPWSTR szText);
	DXSlider* AddSlider(int ID, int x, int y, int width, int height, 
						int sx, int sy, int swidth, int sheight, LPWSTR szText);
	DXListBox* AddListBox(int ID, int x, int y, int width, int height,
						int sx, int sy, int swidth, int sheight, LPWSTR szText);

	void SetNormal(DXControl* pControl);
	void SetNormal(int ID);
	void SetDisabled(DXControl* pControl);
	void SetDisabled(int ID);
	void SetHidden(DXControl* pControl);
	void SetHidden(int ID);
	bool SetFocus(DXControl* pControl);
	void SetFocus(int ID);
	DXControl* GetFocus();
	DXControl* PrevFocus();
	DXControl* NextFocus();
	void SetPressed(DXControl* pControl);
	void SetPressed(int ID);
	DXControl* GetPressed();
	DXControl* GetControl(int ID);

	void DrawControlText(LPWSTR szText, RECT *prcRect, DXElement *pElement = NULL);
	void DrawControlRect(RECT rc, DWORD dwColor);
	void DrawControlRectFrame(RECT rc, DWORD dwColor);

	void PlayFocusSound();
	void PlayPressSound();
	void PlaySelectSound();
	void PlayCancelSound();

	virtual void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);
};

class DXControl
{
protected:
	DXDialog *m_pDlgParent;
	DXCONTROL_TYPE m_Type;
	DXCONTROL_STATE m_State;

	int m_ID;
	int m_x, m_y;
	int m_Width, m_Height;
	RECT m_rectBound;

	WCHAR m_szText[MAX_PATH];
	std::vector<DXElement*> m_pElements;

public:
	DXControl();
	virtual ~DXControl();

	DXCONTROL_TYPE GetType() { return m_Type; }
	DXCONTROL_STATE GetState() { return m_State; }
	virtual BOOL SetState(DXCONTROL_STATE state) { m_State = state; return TRUE; }

	void SetID(int ID) { m_ID = ID; }
	int GetID() { return m_ID; }

	void SetLocation(int x, int y)
	{
		m_x = x; m_y = y; UpdateRect();
	}

	void SetSize(int width, int height)
	{
		m_Width = width; m_Height = height; UpdateRect();
	}

	void UpdateRect();

	void SetParentDialog(DXDialog*);

	void SetText(LPWSTR szText)
	{
		wcscpy_s(m_szText, MAX_PATH, szText);
	}

	void AddElement(DXElement *pElement);
	void AddElement(DXElement Element);
	void AddElement(int iFont, DWORD dwFormat, D3DXCOLOR TextColor);

	virtual void Render(double fTime, float fElapsedTime) {};
};

class DXStatic : public DXControl
{
public:
	DXStatic();
	~DXStatic();

	BOOL SetState(DXCONTROL_STATE state);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);
};

class DXButton :public DXControl
{
public:
	DXButton();
	~DXButton();

	BOOL SetState(DXCONTROL_STATE state);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);
};

class DXSlider :public DXControl
{
public:
	DXSlider();
	~DXSlider();
	
	BOOL SetState(DXCONTROL_STATE state);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);
	void SetSlider(int x, int y, int width, int height);

	void SetRange(int iBeg, int iEnd);
	void SetValue(int iValue);
	void AddValue(int iDelta);
	void SubValue(int iDelta);
	int GetValue();

private:
	int m_iBeg, m_iEnd;
	int m_iCurrent;
	float m_fPressTime;
	RECT m_rectSlider;
	DWORD m_colorLeft, m_colorRight;
};

class DXListBox : public DXControl
{
public:
	DXListBox();
	~DXListBox();

	BOOL SetState(DXCONTROL_STATE state);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);
	void SetBox(int x, int y, int width, int height);
	void AddListText(WCHAR* szList);
	void ChangeListText(int index, WCHAR* szList);
	void SetListSize(int cList);
	void OpenList();
	bool CloseList(bool bSelect);
	bool IsListOpen();
	void SelectUp();
	void SelectDown();
	void SetSelect(int index);
	int GetSelect();

private:
	int m_xBox, m_yBox, m_widthBox, m_heightBox;
	RECT m_rectBox;
	std::vector<WCHAR*> m_szList;
	int m_iCurrent;
	bool m_bListOpen;
	int m_iList;
	int m_cList;
	float m_yDelta;
	float m_yDeltaMax;
	float m_fPressTime;
	DWORD m_colorU, m_colorD;
};

#endif // !DXGUI_H