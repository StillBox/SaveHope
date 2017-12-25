#pragma once
#ifndef SCENESTORY_H
#define SCENESTORY_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"

namespace StoryClass
{
	//Character

	class Chara :public DXObject
	{
	public:
		Chara(int ID);
		~Chara();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		BOOL SetState(int iState);
		int GetState();

	private:
		int m_ID;
		float m_fScale;
	};

	//TextInfo

	struct TextInfo
	{
		int x = 0;
		int y = 0;
		int iFont = 0;
		int iSent = 0;
		float fAlpha = 0.0f;
		float fSpeedAlpha = 0.001f;
	};
}

class SceneStory :public GameScene
{
public:
	SceneStory();
	~SceneStory();

	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENESTORY_H
