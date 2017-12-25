#include "Game.h"
#include "GameGUI.h"
#include "GameAudio.h"
#include "Settings.h"
#include "SceneMenu.h"
#include "SceneShooter.h"
#include "SceneSnake.h"
#include "SceneUpstairs.h"

//
//MenuFunctions
//

enum CallMenuType
{
	Resize = 0,
	ZoomIn,
	ZoomOut
};

void CallMenu(CallMenuType type, int MenuID, bool bReset = true)
{
	float fWidth = 0.0f, fHeight = 0.0f;
	
	SetMenu(MenuID);
	
	if (bReset)
		ResetMenu(MenuID);
	
	switch (MenuID)
	{
	case IDD_TITLE:
		fWidth = 128.0f;
		fHeight = 0.0f;
		break;
	case IDD_MAIN:
		fWidth = 128.0f;
		fHeight = 168.0f;
		break;
	case IDD_AUDIO:
		fWidth = 96.0f;
		fHeight = 48.0f;
		break;
	case IDD_SELECT:
		fWidth = 128.0f;
		fHeight = 168.0f;
		break;
	case IDD_FREE:
		fWidth = 128.0f;
		fHeight = 128.0f;
		break;
	case IDD_LOAD:
	case IDD_SAVE:
		fWidth = 96.0f;
		fHeight = 0.0f;
		break;
	}
	switch (type)
	{
	case Resize:
		SetMenuSize(fWidth, fHeight);
		break;
	case ZoomIn:
		MenuZoomIn(fWidth, fHeight);
		break;
	case ZoomOut:
		MenuZoomOut(fWidth, fHeight);
		break;
	}
}

void CallSettingMenu()
{
	switch (GetScene())
	{
	case SHOOTER:
		SetMenu(IDD_SHOOTER);
		ResetMenu(IDD_SHOOTER);
		break;
	case SNAKE:
		SetMenu(IDD_SNAKE);
		ResetMenu(IDD_SNAKE);
		break;
	case UPSTAIRS:
		SetMenu(IDD_UPSTAIRS);
		ResetMenu(IDD_UPSTAIRS);
		break;
	}
}

//
//Title
//

TitleMenu::TitleMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;

	m_fUpdateTime = 800.0f;
}

TitleMenu::~TitleMenu()
{
	DXDialog::~DXDialog();
}

void TitleMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	int x = m_Width / 2;
	int y = 2 * m_Height / 3 + 32;

	DXStatic *pPress = AddStatic(-1, x - 150, y - 32, 300, 64, L"请按任意键");
	pPress->AddElement(NormalElement);
}

void TitleMenu::Reset()
{
	m_fUpdateTime = 800.0f;
}

void TitleMenu::Default()
{
	DXDialog::Default();
}

void TitleMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		PlayPressSound();
		CallMenu(Resize, IDD_MAIN);
		break;
	default:
		break;
	}
}

void TitleMenu::Render(double fTime, float fElapsedTime)
{
	m_fUpdateTime += fElapsedTime;
	if (m_fUpdateTime >= 800.0f)
		DXDialog::Render(fTime, fElapsedTime);
	if (m_fUpdateTime >= 1600.0f)
		while (0.0f > (m_fUpdateTime -= 1600.0f));
}

//
//MainMenu
//

MainMenu::MainMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

MainMenu::~MainMenu()
{
	DXDialog::~DXDialog();
}

void MainMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);

	DXElement NormalElement		(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement	(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement	(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	int x = m_Width / 2;
	int y = 2 * m_Height / 3 + 32;

	DXButton *pStart = AddButton(IDC_NEW, x - 120, y - 192 , 240, 64, L"新 游 戏");
	pStart->AddElement(NormalElement);
	pStart->AddElement(PressedElement);
	pStart->AddElement(DisabledElement);

	DXButton *pLoad = AddButton(IDC_LOAD, x - 120, y - 112, 240, 64, L"读取进度");
	pLoad->AddElement(NormalElement);
	pLoad->AddElement(PressedElement);
	pLoad->AddElement(DisabledElement);

	DXButton *pScore = AddButton(IDC_SCORE, x - 120, y - 32, 240, 64, L"高分记录");
	pScore->AddElement(NormalElement);
	pScore->AddElement(PressedElement);
	pScore->AddElement(DisabledElement);

	DXButton *pSetting = AddButton(IDC_SETTING, x - 120, y + 48, 240, 64, L"设　　定");
	pSetting->AddElement(NormalElement);
	pSetting->AddElement(PressedElement);
	pSetting->AddElement(DisabledElement);

	DXButton *pQuit = AddButton(IDC_QUIT, x - 120, y + 128, 240, 64, L"结束游戏");
	pQuit->AddElement(NormalElement);
	pQuit->AddElement(PressedElement);
	pQuit->AddElement(DisabledElement);
}

void MainMenu::Default()
{
	DXDialog::Default();
}

void MainMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMenuReady())
	{
		switch (message)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				CallMenu(Resize, IDD_TITLE);
				PlayCancelSound();
			}
			break;
		default:
			break;
		}
		DXDialog::OnMsgProc(message, wParam, lParam);

		if (NULL != m_pPressed)
		{
			switch (GetPressed()->GetID())
			{
			case IDC_NEW:
				InitSaveData();
				CallMenu(Resize, IDD_SELECT);
				break;
			case IDC_LOAD:
				CallMenu(ZoomIn, IDD_LOAD);
				break;
			case IDC_SCORE:
				SetScene(SCORE);
				SetCurtain(0, 255, 500.0f, GameStart);
				break;
			case IDC_SETTING:
				CallMenu(ZoomIn, IDD_AUDIO);
				break;
			case IDC_QUIT:
				SetCurtain(0, 255, 500.0f, QuitGame);
				break;
			default:
				break;
			}
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
			AccelResize(10.0f);
	}
}

//
//AudioMenu
//

AudioMenu::AudioMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

AudioMenu::~AudioMenu()
{
	DXDialog::~DXDialog();
}

void AudioMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 44, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	DXElement ValueNormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXButton *pAudio = AddButton(IDC_AUDIO, x - 120, y - 72, 240, 64, L"声音设定");
	pAudio->AddElement(NormalElement);
	pAudio->AddElement(PressedElement);
	pAudio->AddElement(DisabledElement);

	DXStatic *pVideo = AddStatic(IDC_VIDEO, x - 120, y + 8, 240, 64, L"显示设定");
	pVideo->AddElement(NormalElement);

	DXSlider *pMainVol = AddSlider(IDC_VOL, 220, 200, 240, 64, 750, 200, 100, 64, L"主 音 量");
	pMainVol->AddElement(NormalElement);
	pMainVol->AddElement(PressedElement);
	pMainVol->AddElement(DisabledElement);
	pMainVol->AddElement(ValueNormalElement);
	pMainVol->SetValue(GetSettings().m_iMasterVol);

	DXSlider *pBGMVol = AddSlider(IDC_BGM_VOL, 220, 400, 240, 64, 750, 400, 100, 64, L"背景音量");
	pBGMVol->AddElement(NormalElement);
	pBGMVol->AddElement(PressedElement);
	pBGMVol->AddElement(DisabledElement);
	pBGMVol->AddElement(ValueNormalElement);
	pBGMVol->SetValue(GetSettings().m_iBGMVol);

	DXSlider *pSEVol = AddSlider(IDC_SE_VOL, 220, 600, 240, 64, 750, 600, 100, 64, L"音效音量");
	pSEVol->AddElement(NormalElement);
	pSEVol->AddElement(PressedElement);
	pSEVol->AddElement(DisabledElement);
	pSEVol->AddElement(ValueNormalElement);
	pSEVol->SetValue(GetSettings().m_iSEVol);

	DXSlider *pVoiceVol = AddSlider(IDC_VOICE_VOL, 220, 800, 240, 64, 750, 800, 100, 64, L"语音音量");
	pVoiceVol->AddElement(NormalElement);
	pVoiceVol->AddElement(PressedElement);
	pVoiceVol->AddElement(DisabledElement);
	pVoiceVol->AddElement(ValueNormalElement);
	pVoiceVol->SetValue(GetSettings().m_iVoiceVol);
}

void AudioMenu::Reset()
{
	DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
	pMainVol->SetValue(GetSettings().m_iMasterVol);
	DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
	pBGMVol->SetValue(GetSettings().m_iBGMVol);
	DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
	pSEVol->SetValue(GetSettings().m_iSEVol);
	DXSlider *pVoiceVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOICE_VOL));
	pVoiceVol->SetValue(GetSettings().m_iSEVol);
}

void AudioMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMenuReady())
	{
		switch (m_pFocus->GetID())
		{
		case IDC_AUDIO:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_DOWN:
					SetMenu(IDD_VIDEO);
				case VK_UP:
					PlayFocusSound();
					break;
				case VK_RETURN:
				case VK_LEFT:
					SetFocus(IDC_VOL);
					SetPressed(IDC_AUDIO);
					PlayPressSound();
					break;
				case VK_ESCAPE:
					CallMenu(ZoomOut, IDD_MAIN);
					PlayCancelSound();
					break;
				}
			}
			break;

		case IDC_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
					{
						DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
						if (pMainVol)
						{
							if (VK_LEFT == wParam)
								pMainVol->SubValue(5);
							else
								pMainVol->AddValue(5);
						}
						GetSettings().m_iMasterVol = pMainVol->GetValue();
						SetMasterVolumn((float)GetSettings().m_iMasterVol / 100.0f);
					}
					PlaySelectSound();
					break;
				case VK_ESCAPE:
					SetFocus(IDC_AUDIO);
					PlayCancelSound();
					break;
				}
			}
			break;

		case IDC_BGM_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
					{
						DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
						if (pBGMVol)
						{
							if (VK_LEFT == wParam)
								pBGMVol->SubValue(5);
							else
								pBGMVol->AddValue(5);
						}
						GetSettings().m_iBGMVol = pBGMVol->GetValue();
						SetSubmixVolumn(ID_SUBMIX_BACKGROUND, (float)GetSettings().m_iBGMVol / 100.0f);
					}
					PlaySelectSound();
					break;
				case VK_ESCAPE:
					SetFocus(IDC_AUDIO);
					PlayCancelSound();
					break;
				}
			}
			break;

		case IDC_SE_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_VOICE_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
					{
						DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
						if (pSEVol)
						{
							if (VK_LEFT == wParam)
								pSEVol->SubValue(5);
							else
								pSEVol->AddValue(5);
						}
						GetSettings().m_iSEVol = pSEVol->GetValue();
						SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, (float)GetSettings().m_iSEVol / 100.0f);
					}
					PlaySelectSound();
					break;
				case VK_ESCAPE:
					SetFocus(IDC_AUDIO);
					PlayCancelSound();
					break;
				}
			}
			break;

		case IDC_VOICE_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pVoiceVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOICE_VOL));
					if (pVoiceVol)
					{
						if (VK_LEFT == wParam)
							pVoiceVol->SubValue(5);
						else
							pVoiceVol->AddValue(5);
					}
					GetSettings().m_iVoiceVol = pVoiceVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_VOICE, (float)GetSettings().m_iVoiceVol / 100.0f);
				}
				PlaySelectSound();
				break;
				case VK_ESCAPE:
					SetFocus(IDC_AUDIO);
					PlayCancelSound();
					break;
				}
			}
			break;
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
		{
			if (VK_ESCAPE == wParam)
			{
				CallMenu(ZoomOut, IDD_MAIN);
				PlayCancelSound();
			}
			else
				AccelResize(10.0f);
		}
	}

	SaveSettings();
}

//
//VideoMenu
//

VideoMenu::VideoMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

VideoMenu::~VideoMenu()
{
	DXDialog::~DXDialog();
}

void VideoMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 44, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	DXElement BoxNormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement BoxSelectElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXStatic *pAudio = AddStatic(IDC_AUDIO, x - 120, y - 72, 240, 64, L"声音设定");
	pAudio->AddElement(NormalElement);

	DXButton *pVideo = AddButton(IDC_VIDEO, x - 120, y + 8, 240, 64, L"显示设定");
	pVideo->AddElement(NormalElement);
	pVideo->AddElement(PressedElement);
	pVideo->AddElement(DisabledElement);

	DXListBox *pWindowed = AddListBox(IDC_WINDOWED, 220, 200, 240, 64, 600, 200, 400, 64, L"窗口大小");
	pWindowed->AddElement(NormalElement);
	pWindowed->AddElement(PressedElement);
	pWindowed->AddElement(DisabledElement);
	pWindowed->AddElement(BoxNormalElement);
	pWindowed->AddElement(BoxSelectElement);
	pWindowed->SetListSize(3);
	pWindowed->ChangeListText(0, L"全屏窗口");
	pWindowed->AddListText(L"480 x 270");
	pWindowed->AddListText(L"960 x 540");
	pWindowed->AddListText(L"1440 x 810L");
	pWindowed->AddListText(L"1920 x 1080");
	pWindowed->SetSelect(GetSettings().m_iWindowed);

	DXListBox *pResolution = AddListBox(IDC_RESOLUTION, 220, 400, 240, 64, 600, 400, 400, 64, L"分 辨 率");
	pResolution->AddElement(NormalElement);
	pResolution->AddElement(PressedElement);
	pResolution->AddElement(DisabledElement);
	pResolution->AddElement(BoxNormalElement);
	pResolution->AddElement(BoxSelectElement);
	pResolution->SetListSize(3);
	pResolution->ChangeListText(0, L"自动选择");
	pResolution->AddListText(L"480 x 270");
	pResolution->AddListText(L"960 x 540");
	pResolution->AddListText(L"1920 x 1080");
	pResolution->SetSelect(GetSettings().m_iResolution - 1);

	DXListBox *pTexRender = AddListBox(IDC_TEXRENDER, 220, 600, 240, 64, 600, 600, 400, 64, L"渲染模式");
	pTexRender->AddElement(NormalElement);
	pTexRender->AddElement(PressedElement);
	pTexRender->AddElement(DisabledElement);
	pTexRender->AddElement(BoxNormalElement);
	pTexRender->AddElement(BoxSelectElement);
	pTexRender->SetListSize(3);
	pTexRender->ChangeListText(0, L"点 模 式");
	pTexRender->SetSelect(GetSettings().m_iTexRender);

	DXListBox *pFPSMode = AddListBox(IDC_FPSMODE, 220, 800, 240, 64, 600, 800, 400, 64, L"FPS 设定");
	pFPSMode->AddElement(NormalElement);
	pFPSMode->AddElement(PressedElement);
	pFPSMode->AddElement(DisabledElement);
	pFPSMode->AddElement(BoxNormalElement);
	pFPSMode->AddElement(BoxSelectElement);
	pFPSMode->SetListSize(3);
	pFPSMode->ChangeListText(0, L"无 限 制");
	pFPSMode->SetSelect(GetSettings().m_iFPSMode);
}

void VideoMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bWindowChange = false;
	bool bDeviceChange = false;

	if (IsMenuReady())
	{
		switch (m_pFocus->GetID())
		{
		case IDC_VIDEO:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetMenu(IDD_AUDIO);
				case VK_DOWN:
					PlayFocusSound();
					break;
				case VK_RETURN:
				case VK_LEFT:
					SetFocus(IDC_WINDOWED);
					SetPressed(IDC_VIDEO);
					PlayPressSound();
					break;
				case VK_ESCAPE:
					CallMenu(ZoomOut, IDD_MAIN);
					PlayCancelSound();
					break;
				}
			}
			break;

		case IDC_WINDOWED:
			if (WM_KEYDOWN == message)
			{
				DXListBox *pWindowed = dynamic_cast<DXListBox*>(GetControl(IDC_WINDOWED));
				if (!pWindowed->IsListOpen())
				{
					switch (wParam)
					{
					case VK_UP:
						PlayFocusSound();
						break;
					case VK_DOWN:
						SetFocus(IDC_RESOLUTION);
						PlayFocusSound();
						break;
					case VK_RETURN:
						pWindowed->OpenList();						
						PlayPressSound();
						break;
					case VK_ESCAPE:
						SetFocus(IDC_VIDEO);
						PlayCancelSound();
						break;
					}
				}
				else
				{
					switch (wParam)
					{
					case VK_UP:
						pWindowed->SelectUp();
						PlaySelectSound();
						break;
					case VK_DOWN:
						pWindowed->SelectDown();
						PlaySelectSound();
						break;
					case VK_RETURN:
						bWindowChange = pWindowed->CloseList(true);
						PlayPressSound();
						GetSettings().m_iWindowed = pWindowed->GetSelect();
						break;
					case VK_ESCAPE:
						pWindowed->CloseList(false);
						PlayCancelSound();
						break;
					}
				}
			}
			break;

		case IDC_RESOLUTION:
			if (WM_KEYDOWN == message)
			{
				DXListBox *pResolution = dynamic_cast<DXListBox*>(GetControl(IDC_RESOLUTION));
				if (!pResolution->IsListOpen())
				{
					switch (wParam)
					{
					case VK_UP:
						SetFocus(IDC_WINDOWED);
						PlayFocusSound();
						break;
					case VK_DOWN:
						SetFocus(IDC_TEXRENDER);
						PlayFocusSound();
						break;
					case VK_RETURN:
						pResolution->OpenList();
						PlayPressSound();
						break;
					case VK_ESCAPE:
						SetFocus(IDC_VIDEO);
						PlayCancelSound();
						break;
					}
				}
				else
				{
					switch (wParam)
					{
					case VK_UP:
						pResolution->SelectUp();
						PlaySelectSound();
						break;
					case VK_DOWN:
						pResolution->SelectDown();
						PlaySelectSound();
						break;
					case VK_RETURN:
						bDeviceChange = pResolution->CloseList(true);
						PlayPressSound();
						GetSettings().m_iResolution = pResolution->GetSelect();
						break;
					case VK_ESCAPE:
						pResolution->CloseList(false);
						PlayCancelSound();
						break;
					}
				}
			}
			break;

		case IDC_TEXRENDER:
			if (WM_KEYDOWN == message)
			{
				DXListBox *pTexRender = dynamic_cast<DXListBox*>(GetControl(IDC_TEXRENDER));
				if (!pTexRender->IsListOpen())
				{
					switch (wParam)
					{
					case VK_UP:
						SetFocus(IDC_RESOLUTION);
						PlayFocusSound();
						break;
					case VK_DOWN:
						SetFocus(IDC_FPSMODE);
						PlayFocusSound();
						break;
					case VK_RETURN:
						pTexRender->OpenList();
						PlayPressSound();
						break;
					case VK_ESCAPE:
						SetFocus(IDC_VIDEO);
						PlayCancelSound();
						break;
					}
				}
				else
				{
					switch (wParam)
					{
					case VK_UP:
						pTexRender->SelectUp();
						PlaySelectSound();
						break;
					case VK_DOWN:
						pTexRender->SelectDown();
						PlaySelectSound();
						break;
					case VK_RETURN:
						pTexRender->CloseList(true);
						PlayPressSound();
						break;
					case VK_ESCAPE:
						pTexRender->CloseList(false);
						PlayCancelSound();
						break;
					}
				}
			}
			break;

		case IDC_FPSMODE:
			if (WM_KEYDOWN == message)
			{
				DXListBox *pFPSMode = dynamic_cast<DXListBox*>(GetControl(IDC_FPSMODE));
				if (!pFPSMode->IsListOpen())
				{
					switch (wParam)
					{
					case VK_UP:
						SetFocus(IDC_TEXRENDER);
						PlayFocusSound();
						break;
					case VK_DOWN:
						PlayFocusSound();
						break;
					case VK_RETURN:
						pFPSMode->OpenList();
						PlayPressSound();
						break;
					case VK_ESCAPE:
						SetFocus(IDC_VIDEO);
						PlayCancelSound();
						break;
					}
				}
				else
				{
					switch (wParam)
					{
					case VK_UP:
						pFPSMode->SelectUp();
						PlaySelectSound();
						break;
					case VK_DOWN:
						pFPSMode->SelectDown();
						PlaySelectSound();
						break;
					case VK_RETURN:
						pFPSMode->CloseList(true);
						PlayPressSound();
						break;
					case VK_ESCAPE:
						pFPSMode->CloseList(false);
						PlayCancelSound();
						break;
					}
				}
			}
			break;
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
		{
			if (VK_ESCAPE == wParam)
			{
				CallMenu(ZoomOut, IDD_MAIN);
				PlayCancelSound();
			}
			else
				AccelResize(10.0f);
		}
	}

	if (bWindowChange || bDeviceChange)
	{
		UpdateSettings();
		SaveSettings();

		if (bWindowChange)
		{
			if (GetSettings().m_iWindowed != 0)
			{
				int cx = 480 * GetSettings().m_iWindowed;
				int cy = 270 * GetSettings().m_iWindowed;
				DXUTChangeWindow(cx, cy, WS_OVERLAPPEDWINDOW);
			}
			else
			{
				int cx = GetSystemMetrics(SM_CXSCREEN);
				int cy = GetSystemMetrics(SM_CYSCREEN);
				DXUTChangeWindow(cx, cy, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP);
			}
		}

		DXUTChangeDevice(GetSettings().m_cxDevice, GetSettings().m_cyDevice);
	}
}

//
//GameSelect
//

GameSelect::GameSelect()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

GameSelect::~GameSelect()
{
	DXDialog::~DXDialog();
}

void GameSelect::Init()
{
	SetFont(0, L"黑体", 48, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	int x = m_Width / 2;
	int y = 2 * m_Height / 3 + 32;

	DXButton *pStory = AddButton(IDC_STORY, x - 120, y - 192, 240, 64, L"故事模式");
	pStory->AddElement(NormalElement);
	pStory->AddElement(PressedElement);
	pStory->AddElement(DisabledElement);

	DXButton *pFree = AddButton(IDC_FREE, x - 120, y - 112, 240, 64, L"自由模式");
	pFree->AddElement(NormalElement);
	pFree->AddElement(PressedElement);
	pFree->AddElement(DisabledElement);

	DXButton *pShop = AddButton(IDC_SHOP, x - 120, y - 32, 240, 64, L"碎片兑换");
	pShop->AddElement(NormalElement);
	pShop->AddElement(PressedElement);
	pShop->AddElement(DisabledElement);

	DXButton *pOmake = AddButton(IDC_OMAKE, x - 120, y + 48, 240, 64, L"额外内容");
	pOmake->AddElement(NormalElement);
	pOmake->AddElement(PressedElement);
	pOmake->AddElement(DisabledElement);

	DXButton *pSave = AddButton(IDC_SAVE, x - 120, y + 128, 240, 64, L"保存进度");
	pSave->AddElement(NormalElement);
	pSave->AddElement(PressedElement);
	pSave->AddElement(DisabledElement);
}

void GameSelect::Reset()
{
	if (GetSaveData().m_iUnlock[0])
	{
		DXButton *pFree = static_cast<DXButton*>(GetControl(IDC_FREE));
		pFree->SetText(L"自由模式");
		pFree->SetState(DXSTATE_NORMAL);
		DXButton *pShop = static_cast<DXButton*>(GetControl(IDC_SHOP));
		pShop->SetText(L"碎片兑换");
		pShop->SetState(DXSTATE_NORMAL);
	}
	else
	{
		DXButton *pFree = static_cast<DXButton*>(GetControl(IDC_FREE));
		pFree->SetText(L"？？？");
		pFree->SetState(DXSTATE_DISABLED);
		DXButton *pShop = static_cast<DXButton*>(GetControl(IDC_SHOP));
		pShop->SetText(L"？？？");
		pShop->SetState(DXSTATE_DISABLED);
	}

	if (GetSaveData().m_iUnlock[1])
	{
		DXButton *pShop = static_cast<DXButton*>(GetControl(IDC_OMAKE));
		pShop->SetText(L"额外内容");
		pShop->SetState(DXSTATE_NORMAL);
	}
	else
	{
		DXButton *pShop = static_cast<DXButton*>(GetControl(IDC_OMAKE));
		pShop->SetText(L"？？？");
		pShop->SetState(DXSTATE_DISABLED);
	}
	
	SetFocus(IDC_STORY);
}

void GameSelect::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMenuReady())
	{
		static bool bReturnOption = false;
		static bool bStoryOption = false;
		static bool bNewOption = false;

		bool bReturn = false;

		switch (message)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				PlayCancelSound();
				if (IsSaveDataUpdated())
				{
					SetCheckBox(256, 64, L"继续将丢失未保存的变化");
					AddCheckBoxOption(60, 64, 152, 64, L"确定");
					AddCheckBoxOption(300, 64, 152, 64, L"取消");
					bReturnOption = true;
				}
				else
					bReturn = true;
			}
		default:
			break;
		}

		if (bReturnOption)
		{
			int iOption;
			if (GetCheckBoxOption(&iOption) && 0 == iOption)
			{
				bReturn = true;
				bReturnOption = false;
			}
		}
		if (bReturn)
			OpenMainMenu();

		DXDialog::OnMsgProc(message, wParam, lParam);

		bool bStart = false;
		bool bNew = false;

		if (m_pPressed != NULL)
		{
			switch (GetPressed()->GetID())
			{
			case IDC_STORY:
				if (GetSaveData().m_iStory <= 1 || GetSaveData().m_iStory == 4)
					bNew = true;
				else
				{
					SetCheckBox(256, 64, L"继续之前的游戏进度？");
					AddCheckBoxOption(50, 64, 152, 64, L"继续");
					AddCheckBoxOption(265, 64, 200, 64, L"新进度");
					bStoryOption = true;
				}
				SetStoryMode(true);
				break;
			case IDC_FREE:
				CallMenu(Resize, IDD_FREE);
				SetStoryMode(false);
				break;
			case IDC_SHOP:
				SetScene(SHOP);
				SetCurtain(0, 255, 500.0f, GameStart);
				break;
			case IDC_OMAKE:
				SetScene(OMAKE);
				SetCurtain(0, 255, 500.0f, GameStart);
				break;
			case IDC_SAVE:
				CallMenu(ZoomIn, IDD_SAVE);
				break;
			}
		}
		
		if (bStoryOption)
		{
			int iOption;

			if (GetCheckBoxOption(&iOption))
			{
				bStoryOption = false;
				if (0 == iOption)
				{
					bStart = true;
					SetDifficulty(GetSaveData().m_iDifficulty);
				}
				if (1 == iOption)
				{
					bNew = true;
				}
			}
		}

		if (bNew)
		{
			SetCheckBox(384, 64, L"选择游戏难度");
			AddCheckBoxOption(20, 64, 152, 64, L"简单");
			AddCheckBoxOption(205, 64, 152, 64, L"普通");
			AddCheckBoxOption(390, 64, 152, 64, L"困难");
			AddCheckBoxOption(570, 64, 192, 64, L"欺负人");
			bNewOption = true;
		}

		if (bNewOption)
		{
			int iOption;

			if (GetCheckBoxOption(&iOption))
			{
				bStart = true;
				bNewOption = false;
				GetSaveData().m_iStory = 0;
				GetSaveData().m_iDifficulty = iOption;
				SetDifficulty(iOption);
			}
		}

		if (bStart)
		{
			SetScene(STORY);
			SetCurtain(127, 255, 500.0f, GameStart);
		}

		if (!IsCheckBoxOpen())
		{
			bReturnOption = false;
			bStoryOption = false;
			bNewOption = false;
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
			AccelResize(10.0f);
	}
}

//
//FreeSelect
//

FreeSelect::FreeSelect()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

FreeSelect::~FreeSelect()
{
	DXDialog::~DXDialog();
}

void FreeSelect::Init()
{
	SetFont(0, L"黑体", 48, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	int x = m_Width / 2;
	int y = 2 * m_Height / 3 + 32;

	DXButton *pShooter = AddButton(IDC_SHOOTER, x - 120, y - 152, 240, 64, L"精准射击");
	pShooter->AddElement(NormalElement);
	pShooter->AddElement(PressedElement);
	pShooter->AddElement(DisabledElement);

	DXButton *pSnake = AddButton(IDC_SNAKE, x - 120, y - 72, 240, 64, L"节奏闪避");
	pSnake->AddElement(NormalElement);
	pSnake->AddElement(PressedElement);
	pSnake->AddElement(DisabledElement);

	DXButton *pUpstairs = AddButton(IDC_UPSTAIRS, x - 120, y + 8, 240, 64, L"极限攀登");
	pUpstairs->AddElement(NormalElement);
	pUpstairs->AddElement(PressedElement);
	pUpstairs->AddElement(DisabledElement);

	DXButton *pInfUpstairs = AddButton(IDC_INFUP, x - 120, y + 88, 240, 64, L"无限攀登");
	pInfUpstairs->AddElement(NormalElement);
	pInfUpstairs->AddElement(PressedElement);
	pInfUpstairs->AddElement(DisabledElement);
}

void FreeSelect::Reset()
{
	if (GetSaveData().m_iUnlock[2])
	{
		DXButton *pInfUpstairs = static_cast<DXButton*>(GetControl(IDC_INFUP));
		pInfUpstairs->SetState(DXSTATE_NORMAL);
	}
	else
	{
		DXButton *pInfUpstairs = static_cast<DXButton*>(GetControl(IDC_INFUP));
		pInfUpstairs->SetState(DXSTATE_DISABLED);
	}

	SetFocus(IDC_SHOOTER);
}

void FreeSelect::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMenuReady())
	{
		switch (message)
		{
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				PlayCancelSound();
				CallMenu(Resize, IDD_SELECT);
			}
		default:
			break;
		}
		DXDialog::OnMsgProc(message, wParam, lParam);

		bool bStart = false;

		if (m_pPressed != NULL)
		{
			switch (GetPressed()->GetID())
			{
			case IDC_SHOOTER:
				SetScene(SHOOTER);
				SetCheckBox(384, 64, L"选择游戏难度");
				AddCheckBoxOption(20, 64, 152, 64, L"简单");
				AddCheckBoxOption(205, 64, 152, 64, L"普通");
				AddCheckBoxOption(390, 64, 152, 64, L"困难");
				AddCheckBoxOption(570, 64, 192, 64, L"欺负人");
				SetCheckBoxOption(GetDifficulty());
				break;
			case IDC_SNAKE:
				SetScene(SNAKE);
				SetCheckBox(384, 64, L"选择游戏难度");
				AddCheckBoxOption(20, 64, 152, 64, L"简单");
				AddCheckBoxOption(205, 64, 152, 64, L"普通");
				AddCheckBoxOption(390, 64, 152, 64, L"困难");
				AddCheckBoxOption(570, 64, 192, 64, L"欺负人");
				SetCheckBoxOption(GetDifficulty());
				break;
			case IDC_UPSTAIRS:
				SetScene(UPSTAIRS);
				SetCheckBox(384, 64, L"选择游戏难度");
				AddCheckBoxOption(20, 64, 152, 64, L"简单");
				AddCheckBoxOption(205, 64, 152, 64, L"普通");
				AddCheckBoxOption(390, 64, 152, 64, L"困难");
				AddCheckBoxOption(570, 64, 192, 64, L"欺负人");
				SetCheckBoxOption(GetDifficulty());
				break;
			case IDC_INFUP:
				SetScene(UPSTAIRS);
				SetInfiniteMode(true);
				bStart = true;
				break;
			}
		}

		int iOption;
		if (GetCheckBoxOption(&iOption))
		{
			bStart = true;
			SetDifficulty(iOption);
		}

		if (bStart)
		{
			if (IsInfiniteMode())
				SetCurtain(0, 255, 500.0f, GameStart);
			else
				SetCurtain(127, 255, 500.0f, GameStart);
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
			AccelResize(10.0f);
	}
}

//
//PauseMenu
//

PauseMenu::PauseMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
	m_pBox = NULL;
}

PauseMenu::~PauseMenu()
{
	SAFE_DELETE(m_pBox);
	DXDialog::~DXDialog();
}

void PauseMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	int x = m_Width / 2;
	int y = m_Height / 2;

	DXButton *pContinue = AddButton(IDC_CONTINUE, x - 120, y - 192, 240, 64, L"继续游戏");
	pContinue->AddElement(NormalElement);
	pContinue->AddElement(PressedElement);
	pContinue->AddElement(DisabledElement);

	DXButton *pRetry = AddButton(IDC_RETRY, x - 120, y - 112, 240, 64, L"重新开始");
	pRetry->AddElement(NormalElement);
	pRetry->AddElement(PressedElement);
	pRetry->AddElement(DisabledElement);

	DXButton *pSetting = AddButton(IDC_SETTING, x - 120, y - 32, 240, 64, L"游戏设定");
	pSetting->AddElement(NormalElement);
	pSetting->AddElement(PressedElement);
	pSetting->AddElement(DisabledElement);

	DXButton *pMenu = AddButton(IDC_MAIN, x - 120, y + 48, 240, 64, L"返回菜单");
	pMenu->AddElement(NormalElement);
	pMenu->AddElement(PressedElement);
	pMenu->AddElement(DisabledElement);

	DXButton *pQuit = AddButton(IDC_QUIT, x - 120, y + 128, 240, 64, L"结束游戏");
	pQuit->AddElement(NormalElement);
	pQuit->AddElement(PressedElement);
	pQuit->AddElement(DisabledElement);

	m_pBox = new Box();
	m_pBox->Set(m_Width / 2, m_Height / 2, 128, 168);
}

void PauseMenu::Reset()
{
	m_pBox->Set(m_Width / 2, m_Height / 2, 128, 168);
}

void PauseMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PlayCancelSound();
			SetFocus(IDC_QUIT);
		}
	}

	DXDialog::OnMsgProc(message, wParam, lParam);

	if (m_pPressed != NULL)
	{
		switch (GetPressed()->GetID())
		{
		case IDC_CONTINUE:
			m_pBox->Close(GameContinue);
			break;
		case IDC_RETRY:
			SetCurtain(127, 255, 500.0f, GameReset);
			break;
		case IDC_SETTING:
			m_pBox->Close(CallSettingMenu);
			break;
		case IDC_MAIN:
			if (IsInfiniteMode())
				SetInfiniteMode(false);
			SetCurtain(127, 255, 500.0f, OpenSelectMenu);
			break;
		case IDC_QUIT:
			SetCurtain(127, 255, 500.0f, QuitGame);
			break;
		default:
			break;
		}
	}
}

void PauseMenu::Render(double fTime, float fElapsedTime)
{
	m_pBox->Render(fElapsedTime);
	if (m_pBox->IsReady())
		DXDialog::Render();
}

//
//ShooterSetting
//

ShooterSetting::ShooterSetting()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
	m_pBoard = NULL;
}

ShooterSetting::~ShooterSetting()
{
	SAFE_DELETE(m_pBoard);
	DXDialog::~DXDialog();
}

void ShooterSetting::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 44, 0);
	SetFont(2, L"黑体", 24, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	DXElement ValueNormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXSlider *pMainVol = AddSlider(IDC_VOL, 220, 200, 240, 64, 750, 200, 100, 64, L"主 音 量");
	pMainVol->AddElement(NormalElement);
	pMainVol->AddElement(PressedElement);
	pMainVol->AddElement(DisabledElement);
	pMainVol->AddElement(ValueNormalElement);
	pMainVol->SetValue(GetSettings().m_iMasterVol);

	DXSlider *pBGMVol = AddSlider(IDC_BGM_VOL, 220, 300, 240, 64, 750, 300, 100, 64, L"背景音量");
	pBGMVol->AddElement(NormalElement);
	pBGMVol->AddElement(PressedElement);
	pBGMVol->AddElement(DisabledElement);
	pBGMVol->AddElement(ValueNormalElement);
	pBGMVol->SetValue(GetSettings().m_iBGMVol);

	DXSlider *pSEVol = AddSlider(IDC_SE_VOL, 220, 400, 240, 64, 750, 400, 100, 64, L"音效音量");
	pSEVol->AddElement(NormalElement);
	pSEVol->AddElement(PressedElement);
	pSEVol->AddElement(DisabledElement);
	pSEVol->AddElement(ValueNormalElement);
	pSEVol->SetValue(GetSettings().m_iSEVol);

	DXSlider *pDecorate = AddSlider(IDC_ST_DECORATE, 220, 600, 240, 64, 750, 600, 100, 64, L"环境细节");
	pDecorate->AddElement(NormalElement);
	pDecorate->AddElement(PressedElement);
	pDecorate->AddElement(DisabledElement);
	pDecorate->AddElement(ValueNormalElement);
	pDecorate->SetRange(0, 2);
	pDecorate->SetValue(GetSettings().m_iSTDecorate);

	DXSlider *pScanAlpha = AddSlider(IDC_ST_SCANALPHA, 180, 700, 320, 64, 750, 700, 100, 64, L"判定区透明度");
	pScanAlpha->AddElement(NormalElement);
	pScanAlpha->AddElement(PressedElement);
	pScanAlpha->AddElement(DisabledElement);
	pScanAlpha->AddElement(ValueNormalElement);
	pScanAlpha->SetRange(0, 255);
	pScanAlpha->SetValue(GetSettings().m_iSTScanAlpha);

	DXSlider *pAdjust = AddSlider(IDC_ST_ADJUST, 220, 800, 240, 64, 750, 800, 100, 64, L"节奏调整");
	pAdjust->AddElement(NormalElement);
	pAdjust->AddElement(PressedElement);
	pAdjust->AddElement(DisabledElement);
	pAdjust->AddElement(ValueNormalElement);
	pAdjust->SetRange(-127, 127);
	pAdjust->SetValue(GetSettings().m_iSTAdjust);

	m_pBoard = new Board();
	m_pBoard->Set(1280);
}

void ShooterSetting::Reset()
{
	m_pBoard->Set(1280);
	DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
	pMainVol->SetValue(GetSettings().m_iMasterVol);
	DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
	pBGMVol->SetValue(GetSettings().m_iBGMVol);
	DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
	pSEVol->SetValue(GetSettings().m_iSEVol);
	DXSlider *pDecorate = dynamic_cast<DXSlider*>(GetControl(IDC_ST_DECORATE));
	pDecorate->SetValue(GetSettings().m_iSTDecorate);
	DXSlider *pScanAlpha = dynamic_cast<DXSlider*>(GetControl(IDC_ST_SCANALPHA));
	pScanAlpha->SetValue(GetSettings().m_iSTScanAlpha);
	DXSlider *pAdjust = dynamic_cast<DXSlider*>(GetControl(IDC_ST_ADJUST));
	pAdjust->SetValue(GetSettings().m_iSTAdjust);
}

void ShooterSetting::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pBoard->IsReady())
	{
		switch (m_pFocus->GetID())
		{
		case IDC_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
					if (pMainVol)
					{
						if (VK_LEFT == wParam)
							pMainVol->SubValue(5);
						else
							pMainVol->AddValue(5);
					}
					GetSettings().m_iMasterVol = pMainVol->GetValue();
					SetMasterVolumn((float)GetSettings().m_iMasterVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_BGM_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
					if (pBGMVol)
					{
						if (VK_LEFT == wParam)
							pBGMVol->SubValue(5);
						else
							pBGMVol->AddValue(5);
					}
					GetSettings().m_iBGMVol = pBGMVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_BACKGROUND, (float)GetSettings().m_iBGMVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_SE_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_ST_DECORATE);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
					if (pSEVol)
					{
						if (VK_LEFT == wParam)
							pSEVol->SubValue(5);
						else
							pSEVol->AddValue(5);
					}
					GetSettings().m_iSEVol = pSEVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, (float)GetSettings().m_iSEVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_ST_DECORATE:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_ST_SCANALPHA);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pDecorate = dynamic_cast<DXSlider*>(GetControl(IDC_ST_DECORATE));
					if (pDecorate)
					{
						if (VK_LEFT == wParam)
							pDecorate->SubValue(1);
						else
							pDecorate->AddValue(1);
					}
					GetSettings().m_iSTDecorate = pDecorate->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_ST_SCANALPHA:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_ST_DECORATE);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_ST_ADJUST);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pScanAlpha = dynamic_cast<DXSlider*>(GetControl(IDC_ST_SCANALPHA));
					if (pScanAlpha)
					{
						if (VK_LEFT == wParam)
							pScanAlpha->SubValue(5);
						else
							pScanAlpha->AddValue(5);
					}
					GetSettings().m_iSTScanAlpha = pScanAlpha->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_ST_ADJUST:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_ST_SCANALPHA);
					PlayFocusSound();
					break;
				case VK_DOWN:
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pAdjust = dynamic_cast<DXSlider*>(GetControl(IDC_ST_ADJUST));
					if (pAdjust)
					{
						if (VK_LEFT == wParam)
							pAdjust->SubValue(1);
						else
							pAdjust->AddValue(1);
					}
					GetSettings().m_iSTAdjust = pAdjust->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;
		}
	}
	
	if (WM_KEYDOWN == message)
	{
		if (VK_ESCAPE == wParam)
		{
			SetShooterDecorate(GetSettings().m_iSTDecorate);
			SetShooterScanAlpha(GetSettings().m_iSTScanAlpha);
			SetShooterAdjust(GetSettings().m_iSTAdjust);
			SetMenu(IDD_PAUSE);
			ResetMenu(IDD_PAUSE);
			PlayCancelSound();
		}
	}

	SaveSettings();
}

void ShooterSetting::Render(double fTime, float fElapsedTime)
{
	m_pBoard->Render(fElapsedTime);
	DXDialog::Render();

	RECT rcText;
	DXElement TextElement = DXElement(2, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rcText, 180, 575, 500, 600);
	DrawControlText(L"游戏中Ins键/Del键调节", &rcText, &TextElement);
	SetRect(&rcText, 180, 675, 500, 700);
	DrawControlText(L"游戏中Home键/End键调节", &rcText, &TextElement);
	SetRect(&rcText, 180, 775, 500, 800);
	DrawControlText(L"游戏中PgUp键/PgDn键调节", &rcText, &TextElement);
}

//
//SnakeSetting
//

SnakeSetting::SnakeSetting()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
	m_pBoard = NULL;
}

SnakeSetting::~SnakeSetting()
{
	SAFE_DELETE(m_pBoard);
	DXDialog::~DXDialog();
}

void SnakeSetting::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 44, 0);
	SetFont(2, L"黑体", 24, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	DXElement ValueNormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXSlider *pMainVol = AddSlider(IDC_VOL, 220, 200, 240, 64, 750, 200, 100, 64, L"主 音 量");
	pMainVol->AddElement(NormalElement);
	pMainVol->AddElement(PressedElement);
	pMainVol->AddElement(DisabledElement);
	pMainVol->AddElement(ValueNormalElement);
	pMainVol->SetValue(GetSettings().m_iMasterVol);

	DXSlider *pBGMVol = AddSlider(IDC_BGM_VOL, 220, 300, 240, 64, 750, 300, 100, 64, L"背景音量");
	pBGMVol->AddElement(NormalElement);
	pBGMVol->AddElement(PressedElement);
	pBGMVol->AddElement(DisabledElement);
	pBGMVol->AddElement(ValueNormalElement);
	pBGMVol->SetValue(GetSettings().m_iBGMVol);

	DXSlider *pSEVol = AddSlider(IDC_SE_VOL, 220, 400, 240, 64, 750, 400, 100, 64, L"音效音量");
	pSEVol->AddElement(NormalElement);
	pSEVol->AddElement(PressedElement);
	pSEVol->AddElement(DisabledElement);
	pSEVol->AddElement(ValueNormalElement);
	pSEVol->SetValue(GetSettings().m_iSEVol);

	DXSlider *pRange = AddSlider(IDC_SK_RANGE, 180, 600, 320, 64, 750, 600, 100, 64, L"镜头范围");
	pRange->AddElement(NormalElement);
	pRange->AddElement(PressedElement);
	pRange->AddElement(DisabledElement);
	pRange->AddElement(ValueNormalElement);
	pRange->SetRange(1, 3);
	pRange->SetValue(GetSettings().m_iSKRange);

	DXSlider *pScanAlpha = AddSlider(IDC_SK_SCANALPHA, 180, 700, 320, 64, 750, 700, 100, 64, L"观测场透明度");
	pScanAlpha->AddElement(NormalElement);
	pScanAlpha->AddElement(PressedElement);
	pScanAlpha->AddElement(DisabledElement);
	pScanAlpha->AddElement(ValueNormalElement);
	pScanAlpha->SetRange(0, 255);
	pScanAlpha->SetValue(GetSettings().m_iSKScanAlpha);

	DXSlider *pAdjust = AddSlider(IDC_SK_ADJUST, 220, 800, 240, 64, 750, 800, 100, 64, L"节奏调整");
	pAdjust->AddElement(NormalElement);
	pAdjust->AddElement(PressedElement);
	pAdjust->AddElement(DisabledElement);
	pAdjust->AddElement(ValueNormalElement);
	pAdjust->SetRange(-127, 127);
	pAdjust->SetValue(GetSettings().m_iSKAdjust);

	m_pBoard = new Board();
	m_pBoard->Set(1280);
}

void SnakeSetting::Reset()
{
	m_pBoard->Set(1280);
	DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
	pMainVol->SetValue(GetSettings().m_iMasterVol);
	DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
	pBGMVol->SetValue(GetSettings().m_iBGMVol);
	DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
	pSEVol->SetValue(GetSettings().m_iSEVol);
	DXSlider *pRange = dynamic_cast<DXSlider*>(GetControl(IDC_SK_RANGE));
	pRange->SetValue(GetSettings().m_iSKRange);
	DXSlider *pScanAlpha = dynamic_cast<DXSlider*>(GetControl(IDC_SK_SCANALPHA));
	pScanAlpha->SetValue(GetSettings().m_iSKScanAlpha);
	DXSlider *pAdjust = dynamic_cast<DXSlider*>(GetControl(IDC_SK_ADJUST));
	pAdjust->SetValue(GetSettings().m_iSKAdjust);
}

void SnakeSetting::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pBoard->IsReady())
	{
		switch (m_pFocus->GetID())
		{
		case IDC_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
					if (pMainVol)
					{
						if (VK_LEFT == wParam)
							pMainVol->SubValue(5);
						else
							pMainVol->AddValue(5);
					}
					GetSettings().m_iMasterVol = pMainVol->GetValue();
					SetMasterVolumn((float)GetSettings().m_iMasterVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_BGM_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
					if (pBGMVol)
					{
						if (VK_LEFT == wParam)
							pBGMVol->SubValue(5);
						else
							pBGMVol->AddValue(5);
					}
					GetSettings().m_iBGMVol = pBGMVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_BACKGROUND, (float)GetSettings().m_iBGMVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_SE_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SK_RANGE);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
					if (pSEVol)
					{
						if (VK_LEFT == wParam)
							pSEVol->SubValue(5);
						else
							pSEVol->AddValue(5);
					}
					GetSettings().m_iSEVol = pSEVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, (float)GetSettings().m_iSEVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_SK_RANGE:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SK_SCANALPHA);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pRange = dynamic_cast<DXSlider*>(GetControl(IDC_SK_RANGE));
					if (pRange)
					{
						if (VK_LEFT == wParam)
							pRange->SubValue(1);
						else
							pRange->AddValue(1);
					}
					GetSettings().m_iSKRange = pRange->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_SK_SCANALPHA:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_SK_RANGE);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SK_ADJUST);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pScanAlpha = dynamic_cast<DXSlider*>(GetControl(IDC_SK_SCANALPHA));
					if (pScanAlpha)
					{
						if (VK_LEFT == wParam)
							pScanAlpha->SubValue(5);
						else
							pScanAlpha->AddValue(5);
					}
					GetSettings().m_iSKScanAlpha = pScanAlpha->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_SK_ADJUST:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_SK_SCANALPHA);
					PlayFocusSound();
					break;
				case VK_DOWN:
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pAdjust = dynamic_cast<DXSlider*>(GetControl(IDC_SK_ADJUST));
					if (pAdjust)
					{
						if (VK_LEFT == wParam)
							pAdjust->SubValue(1);
						else
							pAdjust->AddValue(1);
					}
					GetSettings().m_iSKAdjust = pAdjust->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;
		}
	}

	if (WM_KEYDOWN == message)
	{
		if (VK_ESCAPE == wParam)
		{
			SetSnakeCameraRange(GetSettings().m_iSKRange);
			SetSnakeScanAlpha(GetSettings().m_iSKScanAlpha);
			SetSnakeAdjust(GetSettings().m_iSKAdjust);
			SetMenu(IDD_PAUSE);
			ResetMenu(IDD_PAUSE);
			PlayCancelSound();
		}
	}

	SaveSettings();
}

void SnakeSetting::Render(double fTime, float fElapsedTime)
{
	m_pBoard->Render(fElapsedTime);
	DXDialog::Render();

	RECT rcText;
	DXElement TextElement = DXElement(2, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rcText, 180, 575, 500, 600);
	DrawControlText(L"游戏中Ins键/Del键调节", &rcText, &TextElement);
	SetRect(&rcText, 180, 675, 500, 700);
	DrawControlText(L"游戏中Home键/End键调节", &rcText, &TextElement);
	SetRect(&rcText, 180, 775, 500, 800);
	DrawControlText(L"游戏中PgUp键/PgDn键调节", &rcText, &TextElement);
}

//
//UpstairsSetting
//

UpstairsSetting::UpstairsSetting()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
	m_pBoard = NULL;
}

UpstairsSetting::~UpstairsSetting()
{
	SAFE_DELETE(m_pBoard);
	DXDialog::~DXDialog();
}

void UpstairsSetting::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 44, 0);
	SetFont(2, L"黑体", 24, 0);

	DXElement NormalElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	DXElement ValueNormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXSlider *pMainVol = AddSlider(IDC_VOL, 220, 200, 240, 64, 750, 200, 100, 64, L"主 音 量");
	pMainVol->AddElement(NormalElement);
	pMainVol->AddElement(PressedElement);
	pMainVol->AddElement(DisabledElement);
	pMainVol->AddElement(ValueNormalElement);
	pMainVol->SetValue(GetSettings().m_iMasterVol);

	DXSlider *pBGMVol = AddSlider(IDC_BGM_VOL, 220, 300, 240, 64, 750, 300, 100, 64, L"背景音量");
	pBGMVol->AddElement(NormalElement);
	pBGMVol->AddElement(PressedElement);
	pBGMVol->AddElement(DisabledElement);
	pBGMVol->AddElement(ValueNormalElement);
	pBGMVol->SetValue(GetSettings().m_iBGMVol);

	DXSlider *pSEVol = AddSlider(IDC_SE_VOL, 220, 400, 240, 64, 750, 400, 100, 64, L"音效音量");
	pSEVol->AddElement(NormalElement);
	pSEVol->AddElement(PressedElement);
	pSEVol->AddElement(DisabledElement);
	pSEVol->AddElement(ValueNormalElement);
	pSEVol->SetValue(GetSettings().m_iSEVol);

	DXSlider *pRange = AddSlider(IDC_US_RANGE, 180, 600, 320, 64, 750, 600, 100, 64, L"镜头范围");
	pRange->AddElement(NormalElement);
	pRange->AddElement(PressedElement);
	pRange->AddElement(DisabledElement);
	pRange->AddElement(ValueNormalElement);
	pRange->SetRange(1, 2);
	pRange->SetValue(GetSettings().m_iUSRange);

//	DXSlider *pScanAlpha = AddSlider(IDC_SK_SCANALPHA, 180, 700, 320, 64, 750, 700, 100, 64, L"观测区透明度");
//	pScanAlpha->AddElement(NormalElement);
//	pScanAlpha->AddElement(PressedElement);
//	pScanAlpha->AddElement(DisabledElement);
//	pScanAlpha->AddElement(ValueNormalElement);
//	pScanAlpha->SetRange(0, 255);
//	pScanAlpha->SetValue(GetSettings().m_iSKScanAlpha);
//
//	DXSlider *pAdjust = AddSlider(IDC_SK_ADJUST, 220, 800, 240, 64, 750, 800, 100, 64, L"节奏调整");
//	pAdjust->AddElement(NormalElement);
//	pAdjust->AddElement(PressedElement);
//	pAdjust->AddElement(DisabledElement);
//	pAdjust->AddElement(ValueNormalElement);
//	pAdjust->SetRange(-127, 127);
//	pAdjust->SetValue(GetSettings().m_iSKAdjust);

	m_pBoard = new Board();
	m_pBoard->Set(1280);
}

void UpstairsSetting::Reset()
{
	m_pBoard->Set(1280);
	DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
	pMainVol->SetValue(GetSettings().m_iMasterVol);
	DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
	pBGMVol->SetValue(GetSettings().m_iBGMVol);
	DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
	pSEVol->SetValue(GetSettings().m_iSEVol);
	DXSlider *pRange = dynamic_cast<DXSlider*>(GetControl(IDC_US_RANGE));
	pRange->SetValue(GetSettings().m_iUSRange);
//	DXSlider *pScanAlpha = dynamic_cast<DXSlider*>(GetControl(IDC_SK_SCANALPHA));
//	pScanAlpha->SetValue(GetSettings().m_iSKScanAlpha);
//	DXSlider *pAdjust = dynamic_cast<DXSlider*>(GetControl(IDC_SK_ADJUST));
//	pAdjust->SetValue(GetSettings().m_iSKAdjust);
}

void UpstairsSetting::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pBoard->IsReady())
	{
		switch (m_pFocus->GetID())
		{
		case IDC_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pMainVol = dynamic_cast<DXSlider*>(GetControl(IDC_VOL));
					if (pMainVol)
					{
						if (VK_LEFT == wParam)
							pMainVol->SubValue(5);
						else
							pMainVol->AddValue(5);
					}
					GetSettings().m_iMasterVol = pMainVol->GetValue();
					SetMasterVolumn((float)GetSettings().m_iMasterVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_BGM_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pBGMVol = dynamic_cast<DXSlider*>(GetControl(IDC_BGM_VOL));
					if (pBGMVol)
					{
						if (VK_LEFT == wParam)
							pBGMVol->SubValue(5);
						else
							pBGMVol->AddValue(5);
					}
					GetSettings().m_iBGMVol = pBGMVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_BACKGROUND, (float)GetSettings().m_iBGMVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_SE_VOL:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_BGM_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					SetFocus(IDC_US_RANGE);
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pSEVol = dynamic_cast<DXSlider*>(GetControl(IDC_SE_VOL));
					if (pSEVol)
					{
						if (VK_LEFT == wParam)
							pSEVol->SubValue(5);
						else
							pSEVol->AddValue(5);
					}
					GetSettings().m_iSEVol = pSEVol->GetValue();
					SetSubmixVolumn(ID_SUBMIX_SOUNDEFFECT, (float)GetSettings().m_iSEVol / 100.0f);
				}
				PlaySelectSound();
				break;
				}
			}
			break;

		case IDC_US_RANGE:
			if (WM_KEYDOWN == message)
			{
				switch (wParam)
				{
				case VK_UP:
					SetFocus(IDC_SE_VOL);
					PlayFocusSound();
					break;
				case VK_DOWN:
					PlayFocusSound();
					break;
				case VK_LEFT:
				case VK_RIGHT:
				{
					DXSlider *pRange = dynamic_cast<DXSlider*>(GetControl(IDC_US_RANGE));
					if (pRange)
					{
						if (VK_LEFT == wParam)
							pRange->SubValue(1);
						else
							pRange->AddValue(1);
					}
					GetSettings().m_iUSRange = pRange->GetValue();
				}
				PlaySelectSound();
				break;
				}
			}
			break;
		}
	}

	if (WM_KEYDOWN == message)
	{
		if (VK_ESCAPE == wParam)
		{
			SetUpstairsCameraRange(GetSettings().m_iSKRange);
			SetMenu(IDD_PAUSE);
			ResetMenu(IDD_PAUSE);
			PlayCancelSound();
		}
	}

	SaveSettings();
}

void UpstairsSetting::Render(double fTime, float fElapsedTime)
{
	m_pBoard->Render(fElapsedTime);
	DXDialog::Render();

	RECT rcText;
	DXElement TextElement = DXElement(2, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	SetRect(&rcText, 180, 575, 500, 600);
	DrawControlText(L"游戏中Ins键/Del键调节", &rcText, &TextElement);
}

//
//LoadMenu
//

LoadDataMenu::LoadDataMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

LoadDataMenu::~LoadDataMenu()
{
	DXDialog::~DXDialog();
}

void LoadDataMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 40, 0);

	DXElement TagElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	DXElement NormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXStatic *pLoadTag = AddStatic(IDC_LOAD_TAG, x - 120, y - 32, 240, 64, L"读取进度");
	pLoadTag->AddElement(TagElement);

	DXButton *pData[5];
	for (int i = 0; i < 5; i++)
	{
		WCHAR szBuffer[MAX_PATH];
		swprintf_s(szBuffer, TEXT("存档%d"), i + 1);
		pData[i] = AddButton(IDC_SAVEDATA + i, 100, 100 + 180 * i, 120, 60, szBuffer);
		pData[i]->AddElement(NormalElement);
		pData[i]->AddElement(PressedElement);
		pData[i]->AddElement(DisabledElement);

		GetSaveInfo(i + 1, &m_SaveInfo[i]);
	}
}

void LoadDataMenu::Reset()
{
	for (int i = 0; i < 5; i++)
		GetSaveInfo(i + 1, &m_SaveInfo[i]);
}

void LoadDataMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMenuReady())
	{
		if (WM_KEYDOWN == message)
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				CallMenu(ZoomOut, IDD_MAIN);
				PlayCancelSound();
				break;
			}
		}

		DXDialog::OnMsgProc(message, wParam, lParam);

		static int iData = 0;

		if (NULL != m_pPressed)
		{
			iData = GetPressed()->GetID() - IDC_SAVEDATA;

			if (m_SaveInfo[iData].bExist)
			{
				SetCheckBox(192, 64, L"载入所选存档");
				AddCheckBoxOption(20, 64, 152, 64, L"确定");
				AddCheckBoxOption(220, 64, 152, 64, L"取消");
			}
		}

		int iOption = 0;
		if (GetCheckBoxOption(&iOption) && 0 == iOption)
		{
			ReadSaveData(iData + 1);
			CallMenu(ZoomOut, IDD_SELECT);
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
		{
			if (VK_ESCAPE == wParam)
			{
				CallMenu(ZoomOut, IDD_MAIN);
				PlayCancelSound();
			}
			else
				AccelResize(10.0f);
		}
	}
}

void LoadDataMenu::Render(double fTime, float fElapsedTime)
{
	for (int i = 0; i < 5; i++)
	{
		RECT rcBox;
		DXElement TextElement;
		SetRect(&rcBox, 100, 100 + 180 * i, 1150, 250 + 180 * i);

		if (GetFocus()->GetID() - IDC_SAVEDATA == i)
		{
			DrawControlRectFrame(rcBox, BLACK);
			TextElement = DXElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			DrawControlRectFrame(rcBox, L_GRAY);
			TextElement = DXElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}

		if (m_SaveInfo[i].bExist)
		{
			RECT rcText;
			WCHAR szBuffer[MAX_PATH];

			TextElement.dwFormat = DT_LEFT | DT_VCENTER;

			swprintf_s(szBuffer, L"进度 - ");
			SetRect(&rcText, 280, 120 + 180 * i, 1080, 175 + 180 * i);
			switch (m_SaveInfo[i].iDifficulty)
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
			switch (m_SaveInfo[i].iStory)
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
			DrawControlText(szBuffer, &rcText, &TextElement);

			swprintf_s(szBuffer, L"");
			SetRect(&rcText, 280, 175 + 180 * i, 1080, 230 + 180 * i);
			for (int j = 0; j < 4; j++)
			{
				if (m_SaveInfo[i].iClear[j] == 1)
					wcscat_s(szBuffer, L"★");
				else
					wcscat_s(szBuffer, L"☆");
			}
			DrawControlText(szBuffer, &rcText, &TextElement);

			TextElement.dwFormat = DT_RIGHT | DT_VCENTER;

			SetRect(&rcText, 580, 120 + 180 * i, 1080, 175 + 180 * i);
			swprintf_s(szBuffer, L"%d-%02d-%02d %02d:%02d", m_SaveInfo[i].iYear, m_SaveInfo[i].iMonth,
				m_SaveInfo[i].iDay, m_SaveInfo[i].iHour, m_SaveInfo[i].iMinute);
			DrawControlText(szBuffer, &rcText, &TextElement);

			SetRect(&rcText, 200, 175 + 180 * i, 1080, 230 + 180 * i);
			swprintf_s(szBuffer, L"碎片收集 - %2d | %3d",
				m_SaveInfo[i].iGoldFragment, m_SaveInfo[i].iHopeFragment);
			DrawControlText(szBuffer, &rcText, &TextElement);
		}
		else
		{
			DrawControlText(L"(空)", &rcBox, &TextElement);
		}
	}
	DXDialog::Render();
}

//
//SaveMenu
//

SaveDataMenu::SaveDataMenu()
{
	DXDialog::DXDialog();
	m_Width = GAME_WIDTH;
	m_Height = GAME_HEIGHT;
}

SaveDataMenu::~SaveDataMenu()
{
	DXDialog::~DXDialog();
}

void SaveDataMenu::Init()
{
	SetFont(0, L"黑体", 48, 0);
	SetFont(1, L"黑体", 40, 0);

	DXElement TagElement(0, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	DXElement NormalElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	DXElement PressedElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	DXElement DisabledElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	int x = m_Width / 2 + 416;
	int y = 2 * m_Height / 3 + 32;

	DXStatic *pSaveTag = AddStatic(IDC_LOAD_TAG, x - 120, y - 32, 240, 64, L"保存进度");
	pSaveTag->AddElement(TagElement);

	DXButton *pData[5];
	for (int i = 0; i < 5; i++)
	{
		WCHAR szBuffer[MAX_PATH];
		swprintf_s(szBuffer, TEXT("存档%d"), i + 1);
		pData[i] = AddButton(IDC_SAVEDATA + i, 100, 100 + 180 * i, 120, 60, szBuffer);
		pData[i]->AddElement(NormalElement);
		pData[i]->AddElement(PressedElement);
		pData[i]->AddElement(DisabledElement);

		GetSaveInfo(i + 1, &m_SaveInfo[i]);
	}
}

void SaveDataMenu::Reset()
{
	for (int i = 0; i < 5; i++)
		GetSaveInfo(i + 1, &m_SaveInfo[i]);
}

void SaveDataMenu::OnMsgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (IsMenuReady())
	{
		if (WM_KEYDOWN == message)
		{
			switch (wParam)
			{
			case VK_ESCAPE:
				CallMenu(ZoomOut, IDD_SELECT);
				PlayCancelSound();
				break;
			}
		}

		DXDialog::OnMsgProc(message, wParam, lParam);

		static int iData = 0;
		bool bSave = false;

		if (NULL != m_pPressed)
		{
			iData = GetPressed()->GetID() - IDC_SAVEDATA;

			if (m_SaveInfo[iData].bExist)
			{
				SetCheckBox(192, 64, L"原存档将被覆盖");
				AddCheckBoxOption(20, 64, 152, 64, L"确定");
				AddCheckBoxOption(220, 64, 152, 64, L"取消");
			}
			else
			{
				bSave = true;
			}
		}

		int iOption = 0;
		if (GetCheckBoxOption(&iOption) && 0 == iOption)
		{
			bSave = true;
		}

		if (bSave)
		{
			WriteSaveData(iData + 1);
			Reset();
		}
	}
	else
	{
		if (WM_KEYDOWN == message)
		{
			if (VK_ESCAPE == wParam)
			{
				CallMenu(ZoomOut, IDD_SELECT);
				PlayCancelSound();
			}
			else
				AccelResize(10.0f);
		}
	}
}

void SaveDataMenu::Render(double fTime, float fElapsedTime)
{
	for (int i = 0; i < 5; i++)
	{
		RECT rcBox;
		DXElement TextElement;
		SetRect(&rcBox, 100, 100 + 180 * i, 1150, 250 + 180 * i);
		if (GetFocus()->GetID() - IDC_SAVEDATA == i)
		{
			DrawControlRectFrame(rcBox, BLACK);
			TextElement = DXElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			DrawControlRectFrame(rcBox, L_GRAY);
			TextElement = DXElement(1, DT_CENTER | DT_VCENTER, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		}
		
		if (m_SaveInfo[i].bExist)
		{
			RECT rcText;
			WCHAR szBuffer[MAX_PATH];

			TextElement.dwFormat = DT_LEFT | DT_VCENTER;

			swprintf_s(szBuffer, L"进度 - ");
			SetRect(&rcText, 280, 120 + 180 * i, 1080, 175 + 180 * i);
			switch (m_SaveInfo[i].iDifficulty)
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
			switch (m_SaveInfo[i].iStory)
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
			DrawControlText(szBuffer, &rcText, &TextElement);

			swprintf_s(szBuffer, L"");
			SetRect(&rcText, 280, 175 + 180 * i, 1080, 230 + 180 * i);
			for (int j = 0; j < 4; j++)
			{
				if (m_SaveInfo[i].iClear[j] == 1)
					wcscat_s(szBuffer, L"★");
				else
					wcscat_s(szBuffer, L"☆");
			}
			DrawControlText(szBuffer, &rcText, &TextElement);
			
			TextElement.dwFormat = DT_RIGHT | DT_VCENTER;

			SetRect(&rcText, 280, 120 + 180 * i, 1080, 175 + 180 * i);
			swprintf_s(szBuffer, L"%d-%02d-%02d %02d:%02d", m_SaveInfo[i].iYear, m_SaveInfo[i].iMonth,
				m_SaveInfo[i].iDay, m_SaveInfo[i].iHour, m_SaveInfo[i].iMinute);
			DrawControlText(szBuffer, &rcText, &TextElement);

			SetRect(&rcText, 280, 175 + 180 * i, 1080, 230 + 180 * i);
			swprintf_s(szBuffer, L"碎片收集 - %2d | %3d",
				m_SaveInfo[i].iGoldFragment, m_SaveInfo[i].iHopeFragment);
			DrawControlText(szBuffer, &rcText, &TextElement);
		}
		else
		{
			DrawControlText(L"(空)", &rcBox, &TextElement);
		}
	}
	DXDialog::Render();
}