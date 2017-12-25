#pragma once

#include <vector>
#include "DXUT.h"

#define GAME_WIDTH	1920
#define GAME_HEIGHT	1080
#define GAME_TITLE	L"¾ø¶ÔÏ£ÍûSAVE"

enum GAME_MODE
{
	LOADING = 0,
	GAME_MENU,
	GAME_PLAY,
	GAME_PAUSE
};

enum GAME_SCENE
{
	OP,
	MENU,
	SCORE,
	STORY,
	MIDSAVE,
	SHOOTER,
	SNAKE,
	UPSTAIRS,
	SHOP,
	OMAKE
};

struct GAME_STATE
{
	//Game Show
	float m_SetWidth, m_ScaleWidth, m_ViewWidth;
	float m_SetHeight, m_ScaleHeight, m_ViewHeight;
	float m_fScale;
	bool m_IsotropicView;

	//Game Generals

	bool m_bLoading = true;
	bool m_bSceneInit = true;
	bool m_bUpdate = true;
	bool m_bGameOver = false;

	GAME_MODE m_GameMode;
	GAME_SCENE m_GameScene;
	bool m_bStory = false;
	bool m_bInfinite = false;
	bool m_bDebug = false;
	bool m_bFPSShow = false;
	int m_MenuID = -1;
	int m_iDifficulty = 0;
};

class GameScene
{
public:
	GameScene() = default;
	virtual ~GameScene() = default;

	virtual void Init() = 0;
	virtual void End() = 0;
	virtual void Reset() = 0;
	virtual void OnResetDevice() = 0;
	virtual void OnLostDevice() = 0;
	virtual void Update(double fTime, float fElapsedTime) = 0;
	virtual void Render(double fTime, float fElapsedTime) = 0;
};

HRESULT InitGame();
HRESULT EndGame();

LRESULT CALLBACK OnMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HRESULT CALLBACK OnCreateDevice();
HRESULT CALLBACK OnResetDevice();
HRESULT CALLBACK OnLostDevice();
HRESULT CALLBACK OnDestroyDevice();
void CALLBACK OnFrameUpdate(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);
void CALLBACK OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);

void ShowTitle();
void OpenMainMenu();
void OpenSelectMenu();
void GameStart();
void GameReset();
void GamePause();
void GameContinue();
void GameOver();
void QuitGame();

void SetMode(GAME_MODE mode);
GAME_MODE GetMode();
void SetScene(GAME_SCENE scene);
GAME_SCENE GetScene();
void SetMenu(int MenuID);
void ResetMenu(int MenuID);
int GetMenuID();
void SetDifficulty(int iDifficulty);
int GetDifficulty();
void SetStoryMode(bool bStory);
bool IsStoryMode();
void SetInfiniteMode(bool bInf);
bool IsInfiniteMode();
void SetDebugMode(bool bDebug);
bool IsDebugMode();
void SetFPSShow(bool bFPSShow);
bool IsFPSShow();