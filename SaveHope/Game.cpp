#include "Game.h"
#include "DXInput.h"
#include "DXAudio.h"
#include "DXCamera.h"

#include "GameAudio.h"
#include "GameObject.h"
#include "GameGUI.h"
#include "Settings.h"
#include "SaveData.h"

#include "SceneOP.h"
#include "SceneMenu.h"
#include "SceneScore.h"
#include "SceneStory.h"
#include "SceneMidSave.h"
#include "SceneSnake.h"
#include "SceneShooter.h"
#include "SceneUpstairs.h"
#include "SceneShop.h"
#include "SceneOmake.h"

#include "MiscData.h"
#include "CharaData.h"
#include "SoundData.h"

using std::vector;

GAME_STATE g_GameState;
BOOL g_IsKeyUp[256];

LPD3DXFONT g_pSysInfoText = NULL;

DXDialogResourceManager *g_pDlgManager = NULL;
GameScene *g_pScene = NULL;

HRESULT InitScene();
void UpdateScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);
void RenderScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime);

LRESULT CALLBACK OnMsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (WM_KEYDOWN == message)
	{
		switch (wParam)
		{
		case VK_F5:
			SetMode(LOADING);
			SetScene(OP);
			InitScene();
			break;

//		case VK_F2:
//			NewSaveData(); 
//			SetDifficulty(1);
//			SetScene(SHOOTER);
//			SetCurtain(0, 255, 500.0f, GameStart);
//			SetStoryMode(false);
//			break;
//
//		case VK_F3:
//			NewSaveData();
//			SetDifficulty(3);
//			SetScene(SNAKE);
//			SetCurtain(0, 255, 500.0f, GameStart);
//			SetStoryMode(false);
//			break;
//
//		case VK_F4:
//			NewSaveData();
//			SetDifficulty(3);
//			SetScene(UPSTAIRS);
//			SetCurtain(0, 255, 500.0f, GameStart);
//			SetStoryMode(false);
//			break;
//
//		case VK_F6:
//			NewSaveData();
//			GetSaveData().m_iStory = 3;
//			GetSaveData().m_iDifficulty = 3;
//			SetDifficulty(3);
//			SetScene(STORY);
//			SetCurtain(0, 255, 500.0f, GameStart);
//			SetStoryMode(true);
//			break;
//
//		case VK_F7:
//			NewSaveData();
//			SetScene(SHOP);
//			SetCurtain(0, 255, 500.0f, GameStart);
//			break;
//
//		case VK_F8:
//			NewSaveData();
//			SetScene(OMAKE);
//			SetCurtain(0, 255, 500.0f, GameStart);
//			break;

		case VK_F9:
			if (IsFPSShow())
				SetFPSShow(false);
			else
				SetFPSShow(true);
			break;

		case 87:
			SendMessage(DXUTGetHWnd(), WM_KEYDOWN, VK_UP, 0);
			break;

		case 83:
			SendMessage(DXUTGetHWnd(), WM_KEYDOWN, VK_DOWN, 0);
			break;

		case 65:
			SendMessage(DXUTGetHWnd(), WM_KEYDOWN, VK_LEFT, 0);
			break;

		case 68:
			SendMessage(DXUTGetHWnd(), WM_KEYDOWN, VK_RIGHT, 0);
			break;

		case 90:
			SendMessage(DXUTGetHWnd(), WM_KEYDOWN, VK_RETURN, 0);
			return 0;

		case 88:
			SendMessage(DXUTGetHWnd(), WM_KEYDOWN, VK_ESCAPE, 0);
			return 0;

		default:
			break;
		}
	}

	if (WM_SIZE == message || WM_MOVE == message || WM_MOVING == message)
	{
		if (GAME_PLAY == g_GameState.m_GameMode && (SHOOTER == g_GameState.m_GameScene || SNAKE == g_GameState.m_GameScene || UPSTAIRS == g_GameState.m_GameScene))
			GamePause();
	}

	if (IsCurtainOn())
		return 0;

	if (IsCheckBoxOpen())
	{
		OnMsgProcCheckBox(message, wParam, lParam);
		return 0;
	}

	if (IDD_NULL != g_GameState.m_MenuID)
	{
		g_pDlgManager->GetDialog(g_GameState.m_MenuID)->OnMsgProc(message, wParam, lParam);
	}

	return 0;
}

HRESULT CALLBACK OnCreateDevice()
{
	InitInput(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	
	InitDXAudio();
	SetMasterVolumn((float)GetSettings().m_iMasterVol / 100.0f);
	SetSubmixVolumn(ID_SUBMIX_BACKGROUND, (float)GetSettings().m_iBGMVol / 100.0f);
	SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, (float)GetSettings().m_iSEVol / 100.0f);
	SetSubmixVolumn(ID_SUBMIX_VOICE, (float)GetSettings().m_iVoiceVol / 100.0f);

	InitDXCamera();

	g_pDlgManager = new DXDialogResourceManager();
	g_pDlgManager->SetScale(GetSettings().m_fScale);
	g_pDlgManager->OnCreateDevice();

	CreateCurtain();

	return S_OK;
}

HRESULT CALLBACK OnResetDevice()
{
	g_pDlgManager->SetScale(GetSettings().m_fScale);
	g_pDlgManager->OnResetDevice();

	ResizeCurtain();

	CheckBoxOnResetDevice();

	SAFE_RELEASE(g_pSysInfoText);
	D3DXCreateFont(DXUTGetD3DDevice(), (INT)(32 * GetSettings().m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &g_pSysInfoText);

	if (GAME_PLAY == g_GameState.m_GameMode && (SHOOTER == g_GameState.m_GameScene || SNAKE == g_GameState.m_GameScene || UPSTAIRS == g_GameState.m_GameScene))
		GamePause();

	if (NULL != g_pScene)
		g_pScene->OnResetDevice();

	return S_OK;
}

HRESULT CALLBACK OnLostDevice()
{
	g_pDlgManager->OnLostDevice();

	CheckBoxOnLostDevice();

	SAFE_RELEASE(g_pSysInfoText);

	if (GAME_PLAY == g_GameState.m_GameMode && (SHOOTER == g_GameState.m_GameScene || SNAKE == g_GameState.m_GameScene || UPSTAIRS == g_GameState.m_GameScene))
		GamePause();

	if (NULL != g_pScene)
		g_pScene->OnLostDevice();

	return S_OK;
}

HRESULT CALLBACK OnDestroyDevice()
{
	HRESULT hr;
	HR(hr, DeleteCurtain());
	HR(hr, ReleaseInput());
	HR(hr, ReleaseDXAudio());
	SAFE_DELETE(g_pDlgManager);
	SAFE_RELEASE(g_pSysInfoText);
	HR(hr, EndGame());

	return S_OK;
}

HRESULT InitGame()
{
	D3DXCreateFont(DXUTGetD3DDevice(), (INT)(32 * GetSettings().m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &g_pSysInfoText);

	InitSaveData();
	LoadCharaDLL();
	LoadMiscDLL();
	LoadSoundDLL();

	g_pDlgManager->SetCursorSound(ID_SE_MENU_CURSOR, ID_SUBMIX_SOUNDEFFECT, GetSoundDLLHandle(), IDR_CURSOR, L"WAVE");
	g_pDlgManager->SetSelectSound(ID_SE_MENU_SELECT, ID_SUBMIX_SOUNDEFFECT, GetSoundDLLHandle(), IDR_SELECT, L"WAVE");
	g_pDlgManager->SetDecideSound(ID_SE_MENU_DECIDE, ID_SUBMIX_SOUNDEFFECT, GetSoundDLLHandle(), IDR_DECIDE, L"WAVE");
	g_pDlgManager->SetCancelSound(ID_SE_MENU_CANCEL, ID_SUBMIX_SOUNDEFFECT, GetSoundDLLHandle(), IDR_CANCEL, L"WAVE");
	g_pDlgManager->RegisterDialog(new TitleMenu(), IDD_TITLE);
	g_pDlgManager->GetDialog(IDD_TITLE)->Init();
	g_pDlgManager->RegisterDialog(new MainMenu(), IDD_MAIN);
	g_pDlgManager->GetDialog(IDD_MAIN)->Init();
	g_pDlgManager->RegisterDialog(new AudioMenu(), IDD_AUDIO);
	g_pDlgManager->GetDialog(IDD_AUDIO)->Init();
	g_pDlgManager->RegisterDialog(new VideoMenu(), IDD_VIDEO);
	g_pDlgManager->GetDialog(IDD_VIDEO)->Init();
	g_pDlgManager->RegisterDialog(new GameSelect(), IDD_SELECT);
	g_pDlgManager->GetDialog(IDD_SELECT)->Init();
	g_pDlgManager->RegisterDialog(new FreeSelect(), IDD_FREE);
	g_pDlgManager->GetDialog(IDD_FREE)->Init();
	g_pDlgManager->RegisterDialog(new PauseMenu(), IDD_PAUSE);
	g_pDlgManager->GetDialog(IDD_PAUSE)->Init();
	g_pDlgManager->RegisterDialog(new ShooterSetting(), IDD_SHOOTER);
	g_pDlgManager->GetDialog(IDD_SHOOTER)->Init();
	g_pDlgManager->RegisterDialog(new SnakeSetting(), IDD_SNAKE);
	g_pDlgManager->GetDialog(IDD_SNAKE)->Init();
	g_pDlgManager->RegisterDialog(new UpstairsSetting(), IDD_UPSTAIRS);
	g_pDlgManager->GetDialog(IDD_UPSTAIRS)->Init();
	g_pDlgManager->RegisterDialog(new LoadDataMenu(), IDD_LOAD);
	g_pDlgManager->GetDialog(IDD_LOAD)->Init();
	g_pDlgManager->RegisterDialog(new SaveDataMenu(), IDD_SAVE);
	g_pDlgManager->GetDialog(IDD_SAVE)->Init();

	srand(timeGetTime());
	for (int i = 0; i < 256; i++) g_IsKeyUp[i] = TRUE;

	g_GameState.m_ViewWidth = g_GameState.m_SetWidth = 960.0f;
	g_GameState.m_ViewHeight = g_GameState.m_SetHeight = 720.0f;
	g_GameState.m_ScaleWidth = g_GameState.m_ScaleHeight = 1.0f;
	g_GameState.m_IsotropicView = true;
	g_GameState.m_MenuID = IDD_NULL;

	g_pScene = new SceneOP();

	return S_OK;
}

HRESULT EndGame()
{
	FreeCharaDLL();
	FreeMiscDLL();
	FreeSoundDLL();

	return S_OK;
}

void CALLBACK OnFrameUpdate(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{

}

void CALLBACK OnFrameRender(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{
	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, DD_GRAY, 1.0f, 0);

	pd3dDevice->BeginScene();

	RECT rect;
	SetRect(&rect, 0, 0, DXUTGetDeviceWidth(), DXUTGetDeviceHeight());
	TCHAR szBuffer[20];

	if (!g_GameState.m_bSceneInit)
	{
		g_GameState.m_bUpdate = false;
		InitScene();
		g_GameState.m_bLoading = false;
		g_GameState.m_bSceneInit = true;
		return;
	}

	if (g_GameState.m_bLoading)
	{
		g_pSysInfoText->DrawText(NULL, L"Loading...", -1, &rect,
			DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		g_GameState.m_bSceneInit = false;
	}
	else
	{
		if (g_GameState.m_bUpdate)
		{
			UpdateScene(pd3dDevice, fTime, fElapsedTime);
			RenderScene(pd3dDevice, fTime, fElapsedTime);

			if (GAME_PAUSE == g_GameState.m_GameMode)
				RenderAlphaCurtain(127);

			if ((IsMenuReady() && IDD_NULL != GetMenuID()) || GAME_PAUSE == g_GameState.m_GameMode)
			{
				g_pDlgManager->GetDialog(g_GameState.m_MenuID)->Render(fTime, fElapsedTime);
			}

			if (IsCurtainOn(0))
				RenderCutain(fElapsedTime);

			if (IsCheckBoxOpen())
			{
				RenderAlphaCurtain(127);
				RenderCheckBox(fElapsedTime);
			}
		}
		else
		{
			g_GameState.m_bUpdate = true;
		}
	}
	
	if (IsDebugMode())
	{
		g_pSysInfoText->DrawText(NULL, L"KiBou Mode: On", -1, &rect,
			DT_BOTTOM | DT_RIGHT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
	}

	if (IsFPSShow())
	{
		g_pSysInfoText->DrawText(NULL, szBuffer, swprintf_s(szBuffer, TEXT("FPS:%0.2f"), GetRealtimeFPS()), &rect,
			DT_TOP | DT_RIGHT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
	}

	pd3dDevice->EndScene();
}

HRESULT InitScene()
{
	if (NULL!= g_pScene)
	{
		g_pScene->End();
		SAFE_DELETE(g_pScene);
	}

	switch (g_GameState.m_GameScene)
	{
	case MENU:
		g_pScene = new SceneMenu();
		break;
	case SCORE:
		g_pScene = new SceneScore();
		break;
	case STORY:
		g_pScene = new SceneStory();
		break;
	case MIDSAVE:
		g_pScene = new SceneMidSave();
		break;
	case SHOOTER:
		g_pScene = new SceneShooter();
		break;
	case SNAKE:
		g_pScene = new SceneSnake();
		break;
	case UPSTAIRS:
		g_pScene = new SceneUpstairs();
		break;
	case SHOP:
		g_pScene = new SceneShop();
		break;
	case OMAKE:
		g_pScene = new SceneOmake();
		break;
	default:
		g_pScene = new SceneOP();
		SetMenu(IDD_NULL);
		break;
	}

	g_pScene->Init();

	return S_OK;
}

void UpdateScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{
	if (GAME_PAUSE != g_GameState.m_GameMode)
		g_pScene->Update(fTime, fElapsedTime);
}

void RenderScene(LPDIRECT3DDEVICE9 pd3dDevice, double fTime, float fElapsedTime)
{
	g_pScene->Render(fTime, fElapsedTime);
}

//
//Control Functions
//

void ShowTitle()
{
	if (g_GameState.m_GameScene != MENU) g_GameState.m_bLoading = true;
	g_GameState.m_GameScene = MENU;
	
	SetMenuSize(128.0f, 0.0f);
	g_GameState.m_MenuID = IDD_TITLE;
	g_pDlgManager->GetDialog(IDD_TITLE)->Default();
}

void OpenMainMenu()
{
	if (g_GameState.m_GameScene != MENU) g_GameState.m_bLoading = true;
	g_GameState.m_GameScene = MENU;

	SetMenuSize(128.0f, 168.0f);

	g_GameState.m_GameMode = GAME_MENU;
	g_GameState.m_MenuID = IDD_MAIN;
	g_pDlgManager->GetDialog(IDD_MAIN)->Default();
}

void OpenSelectMenu()
{
	if (g_GameState.m_GameScene != MENU) g_GameState.m_bLoading = true;
	g_GameState.m_GameScene = MENU;

	SetMenuSize(128.0f, 168.0f);

	g_GameState.m_GameMode = GAME_MENU;
	g_GameState.m_MenuID = IDD_SELECT;
	g_pDlgManager->GetDialog(IDD_SELECT)->Reset();
}

void GameStart()
{
	SetMenu(IDD_NULL);
	SetMode(GAME_PLAY);
	g_GameState.m_bLoading = true;
}

void GameReset()
{
	SetMenu(IDD_NULL);
	SetMode(GAME_PLAY);
	g_pScene->Reset();
}

void GamePause()
{
	DXAudioStop(-1);
	SetMenu(IDD_PAUSE);
	SetMode(GAME_PAUSE);
	ResetMenu(IDD_PAUSE);
	g_pDlgManager->GetDialog(IDD_PAUSE)->Default();
	g_pDlgManager->GetDialog(IDD_PAUSE)->PlayCancelSound();
}

void GameContinue()
{
	DXAudioPlay(-1);
	SetMenu(IDD_NULL);
	SetMode(GAME_PLAY);
}

void GameOver()
{
	GamePause();
	g_pDlgManager->GetDialog(IDD_PAUSE)->SetDisabled(IDC_CONTINUE);
}

void QuitGame() 
{
	;
	DestroyWindow(DXUTGetHWnd());
}

void SetMode(GAME_MODE mode)
{
	g_GameState.m_GameMode = mode;
}

GAME_MODE GetMode()
{
	return g_GameState.m_GameMode;
}

void SetScene(GAME_SCENE scene)
{
	g_GameState.m_GameScene = scene;
}

GAME_SCENE GetScene()
{
	return g_GameState.m_GameScene;
}

void SetMenu(int MenuID)
{
	g_GameState.m_MenuID = MenuID;
}

void ResetMenu(int MenuID)
{
	g_pDlgManager->GetDialog(MenuID)->Reset();
}

int GetMenuID()
{
	return g_GameState.m_MenuID;
}

void SetDifficulty(int iDifficulty)
{
	g_GameState.m_iDifficulty = iDifficulty;
}

int GetDifficulty()
{
	return g_GameState.m_iDifficulty;
}

void SetStoryMode(bool bStory)
{
	g_GameState.m_bStory = bStory;
}

bool IsStoryMode()
{
	return g_GameState.m_bStory;
}

void SetInfiniteMode(bool bInf)
{
	g_GameState.m_bInfinite = bInf;
}

bool IsInfiniteMode()
{
	return g_GameState.m_bInfinite;
}

void SetDebugMode(bool bDebug)
{
	g_GameState.m_bDebug = bDebug;
}

bool IsDebugMode()
{
	return g_GameState.m_bDebug;
}

void SetFPSShow(bool bFPSShow)
{
	g_GameState.m_bFPSShow = bFPSShow;
}

bool IsFPSShow()
{
	return g_GameState.m_bFPSShow;
}