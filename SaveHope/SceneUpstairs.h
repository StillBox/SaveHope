#pragma once
#ifndef SCENEUPSTAIRS
#define SCENEUPSTAIRS

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"

namespace UpstairsClass
{
	//
	//StageInfo
	//

	struct StageInfo
	{
		int cFloor;
		int cWidth;
		int iTrapBegFloor;
		int iEnemyBegFloor;
		int iFlyEnemyBegFloor;
	};

	//
	//Map
	//

	class Map :public DXObject
	{
		friend void RollScene();

	public:
		Map();
		~Map();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL IsCollision(int row, int col);

		BOOL Roll(int iType = NORMAL_SPACE);
		BOOL Build(int row);
		BOOL Break(int row, int col, int iDir);

	private:
		struct MapBlock
		{
			int iTex = -1;
			bool bCollision = false;
		}m_MapData[MAXSIZE][MAXSIZE];
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

	public:
		BOOL CheckLeft(float *x, float *y);
		BOOL CheckRight(float *x, float *y);
		BOOL CheckTop(float *x, float *y);
		BOOL CheckBottom(float *x, float *y);

		BOOL CheckLeftEnemy(float *x, float *y);
		BOOL CheckRightEnemy(float *x, float *y);
		BOOL CheckTopEnemy(float *x, float *y);
		BOOL CheckBottomEnemy(float *x, float *y, float *v);

		BOOL Roll();

		BOOL SetState(int iState, float u = 0.0f, float v = 0.0f);

		bool IsTarget() { return m_bTarget; }
		bool IsFall() { return m_bFall; }

		bool Damage();

	public:
		int GetState() { return m_iState; }

	private:
		RECTRANGE m_Bound;
		float m_fSpeed;
		float m_fJumpSpeed;
		float m_fGravity;
		float m_u;
		float m_v;

		DXObject *m_pGhost;
		D3DXVECTOR2 m_vDashStart, m_vDashEnd;

		std::vector<int> m_vecBreak;

		bool m_bTarget;
		bool m_bFall;
		float m_timeDamage;
	};

	//
	//Enemy
	//

	class Enemy :public DXObject
	{
		friend class Player;

	public:
		Enemy();
		~Enemy();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL CheckLeft(float *x, float *y);
		BOOL CheckRight(float *x, float *y);
		BOOL Roll();

		BOOL SetState(int iState, float u = 0.0f);
		bool Damage();
		bool IsDestroy();
		bool IsIn(int row, int col);

	private:
		RECTRANGE m_Bound;
		float m_fSpeed;
		float m_u;
		float m_timeDamage;
	};

	class FlyEnemy :public DXObject
	{
		friend class Player;
		friend class FlyEnemy;

	public:
		FlyEnemy();
		~FlyEnemy();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL CheckLeft(float *x, float *y);
		BOOL CheckRight(float *x, float *y);
		BOOL Roll();

		BOOL SetState(int iState, float u = 0.0f);
		bool Damage();
		bool IsDestroy();
		bool IsIn(int row, int col);

	private:
		RECTRANGE m_Bound;
		float m_fSpeed;
		float m_u;
		float m_timeDamage;
	};

	//
	//Prick
	//

	class Prick :public DXObject
	{
		friend void RollScene();

	public:
		Prick();
		~Prick();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL AddPrick(int row, int col);
		BOOL Roll();

	private:
		struct PrickData
		{
			int row;
			int col;
		};
		std::vector<PrickData> m_vecPrick;
	};


	//
	//Item
	//

	class Item :public DXObject
	{
	public:
		Item();
		~Item();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);
		bool Add(int iType, int row, int col);
		BOOL Roll();

	private:
		struct ItemData
		{
			int iType;
			int row;
			int col;
		};
		std::vector<ItemData> m_vecPoints;
	};

	//
	//Fragment
	//

	class Fragment :public DXObject
	{
	public:
		Fragment();
		~Fragment();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);
		bool Add(int row, int col);
		BOOL Roll();

	private:
		struct FragmentData
		{
			int iType;
			int row;
			int col;
		};
		std::vector<FragmentData> m_vecPoints;
		int m_iTex;
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
		ID3DXFont			*m_pFont;
		ID3DXFont			*m_pCNFont;
		ID3DXFont			*m_pLFont;
		ID3DXFont			*m_pLIFont;
		float				m_fScale;

		DXRHWObject			*m_pHopeFragment;
		DXRHWObject			*m_pGoldFragment;
		DXRHWObject			*m_pConnection;
		DXRHWObject			*m_pDash;
		DXRHWObject			*m_pTurn;
		DXRHWObject			*m_pDJump;
		DXRHWObject			*m_pBuild;
		DXRHWObject			*m_pCharge;
		Box					*m_pBox;
		bool				m_bSoundPlay[6];
	};
}


//
//Scene
//

class SceneUpstairs :public GameScene
{
public:
	SceneUpstairs();
	~SceneUpstairs();

	void Init();
	void End();
	void Reset();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

void SetUpstairsCameraRange(int iVal);

#endif // !SCENEUPSTAIRS
