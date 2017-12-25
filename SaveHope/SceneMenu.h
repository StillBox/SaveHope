#pragma once
#ifndef SCENEMENU_H
#define SCENEMENU_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"

namespace MenuClass
{
	class Chara :public DXObject
	{
	public:
		Chara(int iChara = 0);
		~Chara();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		void Move();
		bool IsOut();

	private:
		int m_ID;
		float m_fSpeed;
		float m_fMaxSpeed;
		float m_fAccel;
		float m_fJumpSpeed;
		bool m_bMove;
		bool m_bOut;
	};
}

//
//Scene
//

class SceneMenu :public GameScene
{
public:
	SceneMenu();
	~SceneMenu();

	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

void SetMenuSize(float fX, float fY);
void MenuZoomIn(float fX, float fY);
void MenuZoomOut(float fX, float fY);
void AccelResize(float fMul);
bool IsMenuReady();

#endif // !SCENEMENU_H
