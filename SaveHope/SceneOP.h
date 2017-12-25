#pragma once
#ifndef SCENEOP_H
#define SCENEOP_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"
#include <vector>

namespace OPClass
{
	class BackBoard
	{
	public:
		BackBoard();
		~BackBoard();

		BOOL Update(float fElapsedTime);
		BOOL Render(float fElapsedTime);

		bool IsOver();

	private:
		IDirect3DDevice9 *m_pd3dDevice;
		IDirect3DVertexBuffer9 *m_pVertexBuffer;

		float m_timeUpdate;
		bool m_bUpdate;
		bool m_bOver;
	};

	class Block
	{
	public:
		Block();
		~Block();

		BOOL Update(float fElapsedTime);
		BOOL Render(float fElapsedTime);

	private:
		IDirect3DDevice9 *m_pd3dDevice;
		IDirect3DVertexBuffer9 *m_pVertexBuffer;
		
		float m_fSize;
		float m_timeShow;

		struct BlockState
		{
			int _x;
			int _y;
			bool _bOn = false;
			bool _bShow = false;
			float _fTime = 0.0f;
			float _fOffset = 0.0f;
		};
		std::vector<BlockState> m_vecBlock;
	};

	class Tail
	{
		friend class Block;
	public:
		Tail();
		~Tail();

		BOOL Update(float fElapsedTime);
		BOOL Render(float fElapsedTime);

		bool IsOver();

	private:
		IDirect3DDevice9 *m_pd3dDevice;
		IDirect3DVertexBuffer9 *m_pVertexBuffer;

		float m_fSize;
		float m_x, m_y, m_roll, m_scale;
		float m_timeUpdate;
		bool m_bOver;
	};
}

//Scene

class SceneOP :public GameScene
{
public:
	SceneOP();
	~SceneOP();

	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};


#endif // !SCENEOP_H
