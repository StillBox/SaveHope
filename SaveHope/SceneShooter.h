#pragma once
#ifndef SCENESHOOTER_H
#define SCENESHOOTER_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"

namespace ShooterClass
{
	//
	//Map
	//

	class Map :public DXObject
	{
	public:
		Map();
		~Map();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);
	};

	//
	//Player
	//

	class Player :public DXObject
	{
	public:
		Player();
		~Player();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		DXObject *m_pShadow;
	};

	//
	//Pitcher
	//

	class Pitcher :public DXObject
	{
		friend class Beats;

	public:
		Pitcher(int row = 0, int iType = 0);
		~Pitcher();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		DXObject *m_pShadow;
		int m_iRow;
		float m_fTime;
	};

	//
	//Beat
	//

	class Beats :public DXObject
	{
	public:
		Beats();
		~Beats();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		void AddBeat(int iBeat, int iLastBeat = 0, int iRateSpeed = 1);

	private:
		float m_fTime;

		struct BeatData
		{
			int iTex;
			int iRate;
			int iRow;
			double fBegTime;
			double fBeatTime;
			double fLastTime;
			float xPos;
			float yPos;
			bool bIn;
			bool bPitch;
			bool bActive;
		};

		std::vector<BeatData> m_vecBeats;
		std::vector<Pitcher*> m_vecPitcher;
	};

	//
	//Scanner
	//

	class Scanner :public DXObject
	{
	public:
		Scanner();
		~Scanner();

		BOOL Render(double fTime, float fElapsedTime);
	};

	//
	//Fire
	//

	class Fire :public DXObject
	{
	public:
		Fire();
		~Fire();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		void Add();
		void Del();
		void Set(int cFire);

	private:
		struct FireData
		{
			int row, col;
			int iTex;
			float fTime;
		};

		FireData m_arrLifeFire[12];
		FireData m_arrMapFire[4];
		std::vector<FireData> m_vecRndFire;

		POINT m_arrMapPos[5];
		int m_arrMapTex[20];
		int m_arrTex[11];
		int m_cFire;
		int m_iMinFire;
		float m_fTime;
	};

	//
	//Smoke
	//

	class Smoke :public DXObject
	{
	public:
		Smoke();
		~Smoke();

		void Open(int iColBeg = 18, int iColEnd = 27, int cSmoke = 400);
		void Close();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		bool IsOn() { return m_bOn; }

	private:
		static const int maxSmoke = 512;

		int m_iColBeg, m_iColEnd;
		int m_iRowBeg, m_iRowEnd;
		int m_cSmoke;

		struct SmokeData
		{
			float x, y;
			float fTime;
			float fSpeed;
		};
		std::vector<SmokeData> m_vecSmoke;

		bool m_bOn;
	};

	//
	//TexJudge
	//

	class Judge
	{
	public:
		Judge();
		~Judge();

		void OnResetDevice();
		void OnLostDevice();
		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);
		
		void Add(int iType, float x, float y);

	private:
		IDirect3DDevice9	*m_pd3dDevice;
		ID3DXFont			*m_pFont;
		float				m_fScale;

		struct TextInfo
		{
			int iType;
			float x, y;
			float fTime;
			WCHAR szText[MAX_PATH];
		};
		std::vector<TextInfo> m_vecText;
	};

	//
	//HUD
	//

	class HUD
	{
	public:
		HUD();
		~HUD();

		void OnResetDevice();
		void OnLostDevice();
		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		IDirect3DDevice9	*m_pd3dDevice;
		IDirect3DTexture9	*m_pTexture;
		IDirect3DVertexBuffer9	*m_pVertexBuffer;
		ID3DXFont			*m_pFont;
		ID3DXFont			*m_pCNFont;
		ID3DXFont			*m_pLFont;
		ID3DXFont			*m_pLIFont;
		float				m_fScale;

		DXRHWObject			*m_pHopeFragment, *m_pGoldFragment;
		Box					*m_pBox;
		bool				m_bSoundPlay[9];
	};
}


///////////////////////////////////
//
//Scene
//
///////////////////////////////////

class SceneShooter :public GameScene
{
public:
	SceneShooter();
	~SceneShooter();

	void Init();
	void End();
	void Reset();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

void SetShooterNanami(bool bOn);
void SetShooterOma(bool bOn);

void SetShooterDecorate(int iVal);
void SetShooterScanAlpha(int iVal);
void SetShooterAdjust(int iVal);

#endif // !SCENESHOOTER_H
