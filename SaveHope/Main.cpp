#include "Game.h"
#include "Settings.h"
#include "resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	InitSettings();

	SetCallbackMsgProc(OnMsgProc);
	SetCallbackCreateDevice(OnCreateDevice);
	SetCallbackResetDevice(OnResetDevice);
	SetCallbackLostDevice(OnLostDevice);
	SetCallbackDestroyDevice(OnDestroyDevice);
	SetCallbackD3DFrameRender(OnFrameRender);

	DXUTCreateWindow(GAME_TITLE, GetSettings().m_iWindowed ? WS_OVERLAPPEDWINDOW : WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
		hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON)), NULL, GetSettings().m_cxWindow, GetSettings().m_cyWindow);
	DXUTCreateDevice(GetSettings().m_cxDevice, GetSettings().m_cyDevice);

	InitGame();
	DXUTMainLoop();

	return DXUTGetExitCode();
}