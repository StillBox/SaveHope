#pragma once
#ifndef MFDECODER_H
#define MFDECODER_H

#include <windows.h>

HRESULT GetAudioData(WCHAR *szFile, WAVEFORMATEX **pFmt, DWORD *pdwSize, BYTE **pDataBuffer);

#endif // !MFDECODER_H