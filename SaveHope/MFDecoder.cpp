#include "DXUT.h"
#include "MFDecoder.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <stdio.h>
#include <mferror.h>

#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")

HRESULT GetAudioData(WCHAR *szFile, WAVEFORMATEX **pFmt, DWORD *pdwSize, BYTE **pDataBuffer)
{
	IMFSourceReader *pReader = NULL;

	HRESULT hr = S_OK;
	hr = MFStartup(MF_VERSION);

	if (SUCCEEDED(hr))
		hr = MFCreateSourceReaderFromURL(szFile, NULL, &pReader);

	//Get Audio Format

	IMFMediaType *pAudioType = NULL;
	IMFMediaType *pPartialType = NULL;
	WAVEFORMATEX *pWav = NULL;

	if (SUCCEEDED(hr))
		hr = MFCreateMediaType(&pPartialType);

	if (SUCCEEDED(hr))
		hr = pPartialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);

	if (SUCCEEDED(hr))
		hr = pPartialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

	if (SUCCEEDED(hr))
		hr = pReader->SetCurrentMediaType(
		(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			NULL, pPartialType);

	if (SUCCEEDED(hr))
		hr = pReader->GetCurrentMediaType(
		(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			&pAudioType);

	if (SUCCEEDED(hr))
		hr = pReader->SetStreamSelection(
		(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			TRUE);

	if (SUCCEEDED(hr))
		hr = MFCreateWaveFormatExFromMFMediaType(pAudioType, &pWav, NULL);

	**pFmt = *pWav;
	CoTaskMemFree(pWav);
	SAFE_RELEASE(pPartialType);
	SAFE_RELEASE(pAudioType);

	//Get Size and Data

	DWORD cbAudioData = 0;
	DWORD cbBuffer = 0;
	DWORD cbMax = 1024;
	BYTE *pAudioData = new BYTE[cbMax];
	BYTE *pNewBuffer = NULL;

	IMFSample *pSample = NULL;
	IMFMediaBuffer *pBuffer = NULL;

	while (true)
	{
		DWORD dwFlags = 0;

		// Read the next sample.
		hr = pReader->ReadSample(
			(DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0, NULL, &dwFlags, NULL, &pSample);

		if (FAILED(hr)) { break; }

		if (dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED) { break; }
		if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) { break; }

		if (pSample == NULL) { continue; }

		// Get a pointer to the audio data in the sample.

		hr = pSample->ConvertToContiguousBuffer(&pBuffer);

		if (FAILED(hr)) { break; }

		hr = pBuffer->Lock(&pNewBuffer, NULL, &cbBuffer);

		if (FAILED(hr)) { break; }

		if (cbAudioData + cbBuffer > cbMax)
		{
			BYTE *pOldBuffer = NULL;
			pOldBuffer = pAudioData;
			while (cbAudioData + cbBuffer > cbMax) cbMax *= 2;
			pAudioData = new BYTE[cbMax];
			memcpy(pAudioData, pOldBuffer, cbAudioData);
			SAFE_DELETE(pOldBuffer);
		}
		memcpy(pAudioData + cbAudioData, pNewBuffer, cbBuffer);

		// Unlock the buffer.
		hr = pBuffer->Unlock();
		pNewBuffer = NULL;

		if (FAILED(hr)) { break; }

		// Update running total of audio data.
		cbAudioData += cbBuffer;

		SAFE_RELEASE(pSample);
		SAFE_RELEASE(pBuffer);
	}

	if (pNewBuffer)
	{
		pBuffer->Unlock();
		pNewBuffer = NULL;
	}

	BYTE *pOldBuffer = NULL;
	pOldBuffer = pAudioData;
	pAudioData = new BYTE[cbAudioData];
	memcpy(pAudioData, pOldBuffer, cbAudioData);
	SAFE_DELETE(pOldBuffer);

	*pdwSize = cbAudioData;
	*pDataBuffer = pAudioData;

	pAudioData = NULL;
	SAFE_RELEASE(pBuffer);
	SAFE_RELEASE(pSample);
	SAFE_RELEASE(pReader);
	MFShutdown();

	return hr;
}