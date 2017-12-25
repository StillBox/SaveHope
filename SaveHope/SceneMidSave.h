#pragma once
#ifndef SCENEMIDSAVE_H
#define SCENEMIDSAVE_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"

namespace MidSaveClass
{
	//CheckBox

	class SaveCheckBox
	{
	public:
		SaveCheckBox();
		~SaveCheckBox();

		void Render(float fElapsedTime);
		void SetFocus(int iFocus);
		int GetFocus();

		void Open();
		bool IsReady();
		void Close();
		bool IsClosed();

	private:
		IDirect3DDevice9		*m_pd3dDevice;
		IDirect3DVertexBuffer9	*m_pVertShade;
		Box *m_pBox;

		int m_iFocus;
	};

	//DataBox

	class DataBox
	{
	public:
		DataBox();
		~DataBox();

		void Render();
		void SetFocus(int iFocus);
		int GetFocus();

	private:
		IDirect3DDevice9		*m_pd3dDevice;
		IDirect3DVertexBuffer9	*m_pVertFrame;
		IDirect3DVertexBuffer9	*m_pVertShade;

		int m_iFocus;
	};

}

class SceneMidSave :public GameScene
{
public:
	SceneMidSave();
	~SceneMidSave();

	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENEMIDSAVE_H
