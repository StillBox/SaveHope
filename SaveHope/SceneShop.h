#pragma once
#ifndef SCENESHOP_H
#define SCENESHOP_H

#include "DXObject.h"
#include "Game.h"
#include "SceneUtil.h"
#include "GameObject.h"
#include "SaveData.h"

namespace ShopClass
{
	//
	//Chara
	//

	class Chara :public DXObject
	{
	public:
		Chara();
		~Chara();
		
		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

		BOOL SetState(int iState, float u = 0.0f, bool bIn = true);
		int GetState();
		bool IsReady();

	private:
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

		float m_fSpeed;
		float m_u;
		bool m_bIn;
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

	private:
		int m_iHope, m_iGold;
		int m_iTex;
		float m_timeTex;
	};

	//
	//SkillBar
	//

	class SkillBar :public DXObject
	{
	public:
		SkillBar();
		~SkillBar();

		BOOL Update(double fTime, float fElapsedTime);
		BOOL Render(double fTime, float fElapsedTime);

	private:
		int m_iLevel[SKILLCOUNT][3];
	};

	//
	//
	//

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

	//
	//CharaProp
	//

	struct CharaProp
	{
		int iLvStr;
		int iLvAgi;
		int iLvInt;
		int iLvLuc;
	};
}

class SceneShop :public GameScene
{
public:
	SceneShop();
	~SceneShop();
	
	void Init();
	void End();
	void Reset() {};
	void OnResetDevice();
	void OnLostDevice();
	void Update(double fTime, float fElapsedTime);
	void Render(double fTime, float fElapsedTime);
};

#endif // !SCENESHOP_H
