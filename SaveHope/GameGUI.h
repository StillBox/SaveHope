#pragma once

#ifndef GAMEGUI_H
#define GAMEGUI_H

#include"DXGUI.h"
#include "SaveData.h"
#include "GameObject.h"

//
//MenuDialogs
//

#define IDD_NULL		-1
#define IDD_TITLE		0
#define IDD_MAIN		1
#define IDD_AUDIO		2
#define IDD_VIDEO		3
#define IDD_SELECT		4
#define IDD_FREE		5
#define IDD_SHOP		6
#define IDD_PAUSE		7
#define IDD_LOAD		8
#define IDD_SAVE		9
#define IDD_SHOOTER		10
#define IDD_SNAKE		11
#define IDD_UPSTAIRS	12

//
//Title
//

class TitleMenu :public DXDialog
{
private:
	float m_fUpdateTime;
public:
	TitleMenu();
	~TitleMenu();
	void Init();
	void Reset();
	void Default();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);
};

//
//MainMenu
//

#define IDC_NEW 		1
#define IDC_LOAD		2
#define IDC_SCORE		3
#define IDC_SETTING		4
#define IDC_QUIT		5

class MainMenu :public DXDialog
{
public:
	MainMenu();
	~MainMenu();
	void Init();
	void Default();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//AudioMenu
//

#define IDC_VOL			11
#define IDC_BGM_VOL		12
#define IDC_SE_VOL		13
#define IDC_VOICE_VOL	14
#define IDC_AUDIO		10

class AudioMenu :public DXDialog
{
public:
	AudioMenu();
	~AudioMenu();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//VideoMenu
//

#define IDC_RESOLUTION	21
#define	IDC_WINDOWED	22
#define IDC_TEXRENDER	23
#define IDC_FPSMODE		24
#define	IDC_VIDEO		20

class VideoMenu :public DXDialog
{
public:
	VideoMenu();
	~VideoMenu();
	void Init();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//GameSelect
//

#define IDC_STORY		101
#define IDC_FREE		102
#define IDC_SHOP		103
#define IDC_OMAKE		104
#define IDC_SAVE		105

class GameSelect :public DXDialog
{
public:
	GameSelect();
	~GameSelect();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//FreeSelect
//

#define IDC_SHOOTER		201
#define	IDC_SNAKE		202
#define IDC_UPSTAIRS	203
#define IDC_INFUP		204

class FreeSelect :public DXDialog
{
public:
	FreeSelect();
	~FreeSelect();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//
//PauseMenu
//

#define IDC_CONTINUE	31
#define IDC_RETRY		32
#define IDC_MAIN		33

class PauseMenu :public DXDialog
{
public:
	PauseMenu();
	~PauseMenu();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);

private:
	Box *m_pBox;
};

//
//ShooterSetting
//

#define IDC_ST_DECORATE		1000
#define IDC_ST_SCANALPHA	1001
#define IDC_ST_ADJUST		1002

class ShooterSetting :public DXDialog
{
public:
	ShooterSetting();
	~ShooterSetting();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);

private:
	Board *m_pBoard;
};

//
//SnakeSetting
//

#define IDC_SK_RANGE		2000
#define IDC_SK_SCANALPHA	2001
#define IDC_SK_ADJUST		2002

class SnakeSetting :public DXDialog
{
public:
	SnakeSetting();
	~SnakeSetting();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);

private:
	Board *m_pBoard;
};

//
//UpstairsSetting
//

#define IDC_US_RANGE		3000

class UpstairsSetting :public DXDialog
{
public:
	UpstairsSetting();
	~UpstairsSetting();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);

private:
	Board *m_pBoard;
};

//
//UpstairsSetting
//

//
//LoadMenu
//

#define IDC_LOAD_TAG	40
#define IDC_SAVEDATA	41

class LoadDataMenu :public DXDialog
{
public:
	LoadDataMenu();
	~LoadDataMenu();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);

private:
	SaveInfo m_SaveInfo[5];
};

//
//SaveMenu
//

#define IDC_SAVE_TAG	40
#define IDC_SAVEDATA	41

class SaveDataMenu :public DXDialog
{
public:
	SaveDataMenu();
	~SaveDataMenu();
	void Init();
	void Reset();
	void OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam);
	void Render(double fTime = 0.0f, float fElapsedTime = 0.0f);

private:
	SaveInfo m_SaveInfo[5];
};

#endif // !GAMEGUI_H
