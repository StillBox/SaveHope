#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#include "DXUT.h"

struct Settings
{
	//Video
	int m_iResolution = 0;
	int m_iTexRender = 0;
	int m_iWindowed = 2;
	int m_iFPSMode = 0;

	float m_fScale = 1.0f;
	int m_cxWindow = 0;
	int m_cyWindow = 0;
	int m_cxDevice = 0;
	int m_cyDevice = 0;

	//Audio
	int m_iMasterVol = 60;
	int m_iBGMVol = 60;
	int m_iSEVol = 60;
	int m_iVoiceVol = 0;

	//Shooter
	int m_iSTDecorate = 2;
	int m_iSTScanAlpha = 60;
	int m_iSTAdjust = 0;

	//Snake
	int m_iSKRange = 3;
	int m_iSKScanAlpha = 60;
	int m_iSKAdjust = 0;

	//Upstairs
	int m_iUSRange = 2;
};

Settings &GetSettings();
BOOL InitSettings();
BOOL SaveSettings();
BOOL UpdateSettings();

#endif // !SETTINGS_H
