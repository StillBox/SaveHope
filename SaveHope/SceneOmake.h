#pragma once
#ifndef SCENEOMAKE_H
#define SCENEOMAKE_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"
#include "SaveData.h"

#define CHARALISTCOUNT 4

namespace OmakeClass
{
	class Chara :public DXObject
	{
	public:
		Chara();
		~Chara();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);
		
		BOOL SetState(int iState);
		int GetState();

	private:
		IDirect3DVertexBuffer9 *m_pVertSplit;

		int m_ID;
		int m_iHeight;
		int m_iWeight;
		int m_iBust;

		WCHAR m_szName[MAX_PATH];
		WCHAR m_szKata[MAX_PATH];
		WCHAR m_szTitle[MAX_PATH];
		WCHAR m_szBirth[MAX_PATH];
		WCHAR m_szBlood[MAX_PATH];
		WCHAR m_szStage[MAX_PATH];
		WCHAR m_szDegree[MAX_PATH];
	};

	//
	//Character List
	//

	class CharaList
	{
	public:
		CharaList();
		~CharaList();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		LPDIRECT3DDEVICE9		m_pd3dDevice;
		IDirect3DVertexBuffer9  *m_pVertexBuffer;
		IDirect3DTexture9		*m_pTexture[4];

		int m_iCharaID[CHARALISTCOUNT];
		WCHAR m_szName[CHARALISTCOUNT][MAX_PATH];
		WCHAR m_szTitle[CHARALISTCOUNT][MAX_PATH];

		int m_iCharaListBeg;
		float m_timeUpdate;
	};

	class LCursor :public DXObject
	{
	public:
		LCursor();
		~LCursor();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		BOOL SetState(int iState);
	};

	class RCursor :public DXObject
	{
	public:
		RCursor();
		~RCursor();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		float m_fOffset;
	};

	class BoxCurtain
	{
	public:
		BoxCurtain();
		~BoxCurtain();

		BOOL Render();
		void SetLeft();
		void SetRight();

	private:
		LPDIRECT3DDEVICE9		m_pd3dDevice;
		IDirect3DVertexBuffer9 *m_pVertexBuffer;
		int m_iOn;
	};
}

class SceneOmake :public GameScene
{
public:
	SceneOmake();
	~SceneOmake();

	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENEOMAKE_H


