#include "Settings.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Settings *g_pSettings = new Settings();

Settings &GetSettings()
{
	assert(g_pSettings != NULL);
	return *g_pSettings;
}

void DeleteSpace(std::string &str)
{
	int index = 0;
	while (std::string::npos != (index = str.find(' ')))
	{
		str.erase(index, 1);
	}
}

BOOL InitSettings()
{
	//Read Setting File

	std::ifstream infile(L"Settings.ini");
	std::string line;
	while (std::getline(infile, line))
	{
		auto iter = line.find('=');
		if (std::string::npos != iter)
		{
			std::string parameter = line.substr(0, iter);
			std::string value = line.substr(iter + 1);
			DeleteSpace(parameter);
			DeleteSpace(value);
			std::istringstream inStr(value);
			if		(0 == parameter.compare("Resolution"))
					inStr >> g_pSettings->m_iResolution;
			else if (0 == parameter.compare("TexRender"))
					inStr >> g_pSettings->m_iTexRender;
			else if (0 == parameter.compare("Windowed"))
					inStr >> g_pSettings->m_iWindowed;
			else if (0 == parameter.compare("FPSMode"))
					inStr >> g_pSettings->m_iFPSMode;
			else if (0 == parameter.compare("MasterVol"))
					inStr >> g_pSettings->m_iMasterVol;
			else if (0 == parameter.compare("BGMVol"))
					inStr >> g_pSettings->m_iBGMVol;
			else if (0 == parameter.compare("SEVol"))
					inStr >> g_pSettings->m_iSEVol;
			else if (0 == parameter.compare("VoiceVol"))
					inStr >> g_pSettings->m_iVoiceVol;
			else if (0 == parameter.compare("STDecorate"))
					inStr >> g_pSettings->m_iSTDecorate;
			else if (0 == parameter.compare("STScanAlpha"))
					inStr >> g_pSettings->m_iSTScanAlpha;
			else if (0 == parameter.compare("STAdjust"))
					inStr >> g_pSettings->m_iSTAdjust;
			else if (0 == parameter.compare("SKRange"))
					inStr >> g_pSettings->m_iSKRange;
			else if (0 == parameter.compare("SKScanAlpha"))
					inStr >> g_pSettings->m_iSKScanAlpha;
			else if (0 == parameter.compare("SKAdjust"))
					inStr >> g_pSettings->m_iSKAdjust;
			else if (0 == parameter.compare("USRange"))
					inStr >> g_pSettings->m_iUSRange;
		}
	}
	infile.close();

	SaveSettings();

	//Initialization of other data

	if (g_pSettings->m_iWindowed == 0)
	{
		g_pSettings->m_cxWindow = GetSystemMetrics(SM_CXSCREEN);
		g_pSettings->m_cyWindow = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		g_pSettings->m_cxWindow = 480 * g_pSettings->m_iWindowed;
		g_pSettings->m_cyWindow = 270 * g_pSettings->m_iWindowed;
	}

	switch (g_pSettings->m_iResolution)
	{
	case 1:
		g_pSettings->m_fScale = 0.25f;
		g_pSettings->m_cxDevice = 480;
		g_pSettings->m_cyDevice = 270;
		break;
	case 2:
		g_pSettings->m_fScale = 0.5f;
		g_pSettings->m_cxDevice = 960;
		g_pSettings->m_cyDevice = 540;
		break;
	case 3:
		g_pSettings->m_fScale = 1.0f;
		g_pSettings->m_cxDevice = 1920;
		g_pSettings->m_cyDevice = 1080;
		break;
	default:
		if (g_pSettings->m_iWindowed == 3 || g_pSettings->m_iWindowed == 0)
		{
			g_pSettings->m_fScale = 1.0f;
			g_pSettings->m_cxDevice = 1920;
			g_pSettings->m_cyDevice = 1080;
		}
		else
		{
			g_pSettings->m_fScale = 0.25f * g_pSettings->m_iWindowed;
			g_pSettings->m_cxDevice = g_pSettings->m_cxWindow;
			g_pSettings->m_cyDevice = g_pSettings->m_cyWindow;
		}
		break;
	}
	
	return TRUE;
}

BOOL UpdateSettings()
{
	if (g_pSettings->m_iWindowed == 0)
	{
		g_pSettings->m_cxWindow = GetSystemMetrics(SM_CXSCREEN);
		g_pSettings->m_cyWindow = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		g_pSettings->m_cxWindow = 480 * g_pSettings->m_iWindowed;
		g_pSettings->m_cyWindow = 270 * g_pSettings->m_iWindowed;
	}

	switch (g_pSettings->m_iResolution)
	{
	case 1:
		g_pSettings->m_fScale = 0.25f;
		g_pSettings->m_cxDevice = 480;
		g_pSettings->m_cyDevice = 270;
		break;
	case 2:
		g_pSettings->m_fScale = 0.5f;
		g_pSettings->m_cxDevice = 960;
		g_pSettings->m_cyDevice = 540;
		break;
	case 3:
		g_pSettings->m_fScale = 1.0f;
		g_pSettings->m_cxDevice = 1920;
		g_pSettings->m_cyDevice = 1080;
		break;
	default:
		if (g_pSettings->m_iWindowed == 3 || g_pSettings->m_iWindowed == 0)
		{
			g_pSettings->m_fScale = 1.0f;
			g_pSettings->m_cxDevice = 1920;
			g_pSettings->m_cyDevice = 1080;
		}
		else
		{
			g_pSettings->m_fScale = 0.25f * g_pSettings->m_iWindowed;
			g_pSettings->m_cxDevice = g_pSettings->m_cxWindow;
			g_pSettings->m_cyDevice = g_pSettings->m_cyWindow;
		}
		break;
	}

	return TRUE;
}

BOOL SaveSettings()
{
	std::ofstream outfile(L"Settings.ini");

	outfile << "[Video Setting]\n" << std::endl;

	outfile << "Resolution = "	<< g_pSettings->m_iResolution	<< std::endl;
	outfile << "TexRender = "	<< g_pSettings->m_iTexRender	<< std::endl;
	outfile << "Windowed = "	<< g_pSettings->m_iWindowed		<< std::endl;
	outfile << "FPSMode = "		<< g_pSettings->m_iFPSMode		<< std::endl;

	outfile << "\n[Audio Setting]\n" << std::endl;

	outfile << "MasterVol = "	<< g_pSettings->m_iMasterVol	<< std::endl;
	outfile << "BGMVol = "		<< g_pSettings->m_iBGMVol		<< std::endl;
	outfile << "SEVol = "		<< g_pSettings->m_iSEVol		<< std::endl;
	outfile << "VoiceVol = "	<< g_pSettings->m_iVoiceVol		<< std::endl;

	outfile << "\n[Shooter Setting]\n" << std::endl;
	
	outfile << "STDecorate = "	<< g_pSettings->m_iSTDecorate	<< std::endl;
	outfile << "STScanAlpha = "	<< g_pSettings->m_iSTScanAlpha	<< std::endl;
	outfile << "STAdjust = "	<< g_pSettings->m_iSTAdjust		<< std::endl;

	outfile << "\n[Snake Setting]\n" << std::endl;
	
	outfile << "SKRange = "		<< g_pSettings->m_iSKRange		<< std::endl;
	outfile << "SKScanAlpha = "	<< g_pSettings->m_iSKScanAlpha	<< std::endl;
	outfile << "SKAdjust = "	<< g_pSettings->m_iSKAdjust		<< std::endl;

	outfile << "\n[Upstairs Setting]\n" << std::endl;
	
	outfile << "USRange = " << g_pSettings->m_iUSRange << std::endl;

	outfile.close();

	return TRUE;
}