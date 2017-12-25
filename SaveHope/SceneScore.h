#pragma once
#ifndef SCENESCORE_H
#define SCENESCORE_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"
#include "SaveData.h"

namespace ScoreClass
{
	class Arrow :public DXObject
	{
	public:
		Arrow();
		~Arrow();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		float m_fOffset;
	};
}

class SceneScore :public GameScene
{
public:
	SceneScore();
	~SceneScore();

	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENESCORE_H
