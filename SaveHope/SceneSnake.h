#pragma once
#ifndef SCENESNAKE_H
#define SCENESNAKE_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"

namespace SnakeClass
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

	public:
		BOOL IsCollision(int row, int col);

	private:
		struct MapBlock
		{
			int iTex = -1;
			bool bCollision = true;
		}m_MapData[MAXSIZE][MAXSIZE];
	};

	struct MapInfo
	{
		int cRow;
		int cCol;
		int cGap;
		int cHole;
		int cItem;
		int cEnemy;
		int cPunch;
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
		BOOL SetState(int iState);
		BOOL SetGrid(int row, int col);
		int GetRow(bool bAct = true);
		int GetCol(bool bAct = true);
		void Damaged();
		void Crashed();
		bool Access(int row, int col);

	private:
		DXObject *m_pShadow;
		int m_iRow;
		int m_iCol;
		float m_timeMove;
		float m_timeDamage;
		bool m_bCrashed;
	};

	//
	//Scanner
	//

	class Scanner :public DXObject
	{
	public:
		Scanner();
		~Scanner();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:

	private:
		int m_iRow;
		int m_iCol;
		int m_iRange;
		struct ScanPoint
		{
			int row;
			int col;
		};
		std::vector<ScanPoint> m_vecPoints;
	};

	//
	//Enemy
	//

	class Enemy :public DXObject
	{
		friend class Enemy;
	public:
		Enemy();
		~Enemy();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL SetGrid(int row, int col);
		int GetRow(bool bAct = true);
		int GetCol(bool bAct = true);
		void Damaged();
		void Crashed();
		bool IsCrashed();
		void Observed(bool bVal);
		void Move();
		void LevelUp();
		bool Access(int row, int col);

	private:
		DXObject *m_pShadow;
		int m_iRow, m_iTarRow;
		int m_iCol, m_iTarCol;
		int m_iLP;
		bool m_bMove;
		float m_timeMove;
		float m_timeDamage;
		bool m_bCrashed;
		bool m_bObserved;
		bool m_bLevelUp;
	};

	//
	//Monomi
	//

	class Monomi :public DXObject
	{
	public:
		Monomi();
		~Monomi();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		DXObject *m_pShadow;
	};

	//
	//Item
	//

	class Item :public DXObject
	{
		friend class Punch;
	public:
		Item();
		~Item();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		bool AddItem(int iType, int row, int col);
		BOOL Render(int iRow);

	private:
		DXObject *m_pShadow;
		struct ItemData
		{
			int iType;
			int row;
			int col;
		};
		std::vector<ItemData> m_vecPoints;
		int m_iTex;
	};

	//
	//Punch
	//

	class Punch :public DXObject
	{
		friend class Scanner;
		friend class Enemy;
	public:
		Punch(int cPunch = 0);
		~Punch();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL AddPunch(int row, int col);
		BOOL IsPunched(int row, int col);
		BOOL Render(int iRow);

	private:
		struct PunchPos
		{
			int row;
			int col;
		};
		std::vector<PunchPos> m_vecPunch;

		int m_cPunch;
		float m_fHeight;
		float m_fTime;
	};

	//
	//Prick
	//

	class Prick :public DXObject
	{
	public:
		Prick();
		~Prick();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	public:
		BOOL AddPrick(int row, int col);

	private:
		struct PrickData
		{
			int row;
			int col;
			bool bDamage = true;
		};
		std::vector<PrickData> m_vecPrick;
	};

	//
	//Blood
	//

	class Blood :public DXObject
	{
	public:
		Blood();
		~Blood();
		BOOL Render(double fTime, float fElapsedTime);

		BOOL Add(int row, int col, int iType = 0);

	private:
		struct BloodData
		{
			int row;
			int col;
			int iTex;
		};
		std::vector<BloodData> m_vecBlood;
	};

	//
	//FLInfo
	//
	
	class FLInfo
	{
	public:
		FLInfo();
		~FLInfo();

		void OnResetDevice();
		void OnLostDevice();
		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);
		
		void Add(int iType, float x, float y);

	private:
		IDirect3DDevice9	*m_pd3dDevice;
		ID3DXFont			*m_pFont;
		float				m_fScale;
		DXObject			*m_pArrow;
		float				m_fOffsetArrow;

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
		ID3DXFont			*m_pFont;
		ID3DXFont			*m_pCNFont;
		ID3DXFont			*m_pLFont;
		ID3DXFont			*m_pLIFont;
		float				m_fScale;

		DXRHWObject			*m_pHopeFragment;
		DXRHWObject			*m_pGoldFragment;
		DXRHWObject			*m_pConnection;
		Box					*m_pBox;
		bool				m_bSoundPlay[6];
	};
}

//
//Scene
//

class SceneSnake :public GameScene
{
public:
	SceneSnake();
	~SceneSnake();

	void Init();
	void End();
	void Reset();
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

void SetSnakeCameraRange(int iVal);
void SetSnakeScanAlpha(int iVal);
void SetSnakeAdjust(int iVal);

#endif // !SCENESNAKE_H
