#include "SceneUtil.h"
#include "SceneSnake.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "SaveData.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace SnakeClass;

namespace SnakeData
{
	//
	//Const Parameters
	//

	const int cMinRow = 17;
	const int cMinCol = 30;

	const float fSpacing = 64.0f;
	const double fBPM = 128.01;
	const double fBegTime = 540.0;

	//
	//Player Character
	//

	int		iCharaID;
	int		iStr;
	int		iAgi;
	int		iEnd;
	int		iInt;
	int		iLuc;

	int		iScanRange;
	int		iMaxLP;

	//
	//Parameters to be Setted / Edited
	//

	//Relating to Setting

	float	fScale;

	int		iCameraRange;
	int		iScanAlpha;
	int		iAdjust;
	float	timeAdjust;

	//Relating to difficulty

	float	fRate;
	float	fFrqPunch;
	float	timeItem;

	std::vector<MapInfo> vecMap;
	int		cStage;

	int		cRow;
	int		cCol;
	int		cGap;
	int		cHole;
	int		cItem;
	int		cEnemy;
	int		cPunch;

	int		cBoundCol;
	int		iMapRowBeg;
	int		iMapRowEnd;
	int		iMapColBeg;
	int		iMapColEnd;

	//
	//Parameters to be Calculated
	//

	float	fActSpacing;
	double	fPreTime;
	double	timeTotal;
	double	fBaseSPB;
	double	fSPB;

	//
	//Timers and Counters
	//

	int		iPlayerLP;
	int		iStage;

	float	fPrickTime;
	float	fPunchTime;

	double	timeTimer;
	float	timeBonus;
	float	timeOver;
	float	timeComplete;
	float	timeCount;

	float	timeReRange;
	float	timeReAlpha;
	float	timeReAdjust;

	bool	bStageChange;
	bool	bInterStage;
	float	timeInter;

	bool	bWarning;
	float	fWarning;

	//Controls

	D3DXVECTOR2 vPlayerPos;
	D3DXVECTOR2	vCameraPos;
	float	fCameraScale;
	float	fRangeX, fRangeY;

	//
	//Parameters for Game Results
	//

	//Total
	int		iDeathReason;
	bool	bGameOver;
	bool	bComplete;
	bool	bCount;

	int		iScore;

	float	timeCostTotal;
	int		iDamageTotal;

	int		iFragmentTotal;
	int		iHopeFragment;
	int		iGoldFragment;

	//Stage

	float	timeCost;
	int		iFragment;
	int		iDamage;

	//
	//Pointers to Objects & Sounds
	//

	Background	*pBackground;
	Map		*pMap = NULL;
	Player	*pPlayer = NULL;
	Scanner *pScanner = NULL;
	Item	*pItem = NULL;
	std::vector<Enemy*>	vecEnemy;
	
	Prick	*pPrick = NULL;
	Punch	*pPunch = NULL;
	Blood	*pBlood = NULL;

	FLInfo	*pFLInfo = NULL;
	HUD		*pHUD = NULL;

	Sound	*pBGM = NULL;
	Sound	*pSE0 = NULL;
	Sound	*pSE1 = NULL;

	//
	//Story
	//

	const int cSent = 13;
	LPCWSTR szStory[cSent] =
	{
		L"果然不是能轻松追赶过去的路线呢。",
		L"我已经解析出了盾子布置的机关的运作机制。",
		L"首先，在进入「游戏区域」后，你所处的位置和隔一块地砖的周围位置会周期性地出现地刺，你需要掌握好节奏进行移动才能避免被刺中。",
		L"同时，在你的附近会随机产生冲压机，如果不及时避开冲压机的范围，下场一定……",
		L"因为冲压机出现时的高度相当高，仅靠你的肉眼观察是很难判断准确的砸落位置的。",
		L"我尽力在你身边布置了观测场，当观测场显示红色时就表明这块区域会有冲压机砸落。",
		L"这里已经布置了不少黑白熊守卫，虽然因为布置匆忙，还没有开启攻击系统，但它们仍然会阻碍你的移动，还能掠夺接触到的碎片。",
		L"我的观测场能够干扰它们的系统使它们停止运动，你可以利用机关来摧毁这些黑白熊，它们会掉落额外的碎片，并且掠夺过碎片的黑白熊被摧毁必定会掉落一个金色碎片。",
		L"你的总活动时间是有限制的，每通过一个区域会获得额外的活动时间，根据你在当前区域的收集情况会有额外时间奖励，而受伤则会有惩罚扣除时间。",
		L"你也可以收集碎片立刻恢复一定的活动时间和连接同步值。",
		L"但获取碎片必然需要绕一些远路，甚至会受到更多的刺伤，对这之中的得失还请自行进行权衡。",
		L"每个区域的范围会越来越大，冲压机和黑白熊的数量也会增多，通过区域所需要的时间也会增加，所以在前期区域获得尽量多的活动时间来保证后期有充足的活动时间也是很有必要的。",
		L"那么，就请小心地前进吧！"
	};
	int iSent;
	int iChar;
	float timeChar;
	float timeTalk;
	float timeStoryOver;
	float fSPChar;

	bool bSentOver;
	bool bStoryOver;
	Box *pTalkBox = NULL;

	Monomi *pMonomi = NULL;

	ID3DXFont *pFont = NULL;
	ID3DXFont *pSFont = NULL;
	ID3DXFont *pLFont = NULL;

	Sound *pPrepare = NULL;
	Sound *pSEText = NULL;
}

using namespace SnakeData;

void SetSnakeCameraRange(int iVal)
{
	iCameraRange = iVal;
	switch (iCameraRange)
	{
	case 1:
		fCameraScale = 0.25f;
		break;
	case 2:
		fCameraScale = 0.5f;
		break;
	case 3:
		fCameraScale = 1.0f;
		break;
	}
}

void SetSnakeScanAlpha(int iVal)
{
	iScanAlpha = iVal;
	pScanner->~Scanner();
	pScanner = NULL;
	pScanner = new Scanner();
}

void SetSnakeAdjust(int iVal)
{
	iAdjust = iVal;
	timeAdjust = (float)iAdjust;
}

void LoadMapInfo(int iStage)
{
	cRow = vecMap[iStage].cRow;
	cCol = vecMap[iStage].cCol;
	cGap = vecMap[iStage].cGap;
	cHole = vecMap[iStage].cHole;
	cItem = vecMap[iStage].cItem;
	cEnemy = vecMap[iStage].cEnemy;
	cPunch = (int)(vecMap[iStage].cPunch * (1.0f - 0.5f * (float)iStr / 18.0f));

	cBoundCol = max((cMinCol - cCol) / 2, 1);
	iMapRowBeg = HALFSIZE - cRow / 2;
	iMapRowEnd = HALFSIZE + (cRow + 1) / 2;
	iMapColBeg = 1;
	iMapColEnd = cCol + 1;
}

//
//Map
//

Map::Map()
{
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MAP), 128, 128);

	AddTexRect(  0,   0,  16,  16);			// 0  -- 1-1
	AddTexRect( 16,   0,  32,  16);			// 1  -- 1-2
	AddTexRect( 32,   0,  48,  16);			// 2  -- 1-3
	AddTexRect( 48,   0,  64,  16);			// 3  -- 1-4
	AddTexRect(  0,  16,  16,  32);			// 4  -- 2-1
	AddTexRect( 16,  16,  32,  32);			// 5  -- 2-2
	AddTexRect( 32,  16,  48,  32);			// 6  -- 2-3
	AddTexRect( 48,  16,  64,  32);			// 7  -- 2-4
	AddTexRect(  0,  32,  16,  48);			// 8  -- 3-1
	AddTexRect( 16,  32,  32,  48);			// 9  -- 3-2
	AddTexRect( 32,  32,  48,  48);			// 10 -- 3-3
	AddTexRect( 48,  32,  64,  48);			// 11 -- 3-4
	AddTexRect(  0,  48,  16,  64);			// 12 -- 4-1
	AddTexRect( 16,  48,  32,  64);			// 13 -- 4-2
	AddTexRect( 32,  48,  48,  64);			// 14 -- 4-3
	AddTexRect( 48,  48,  64,  64);			// 15 -- 4-4
	AddTexRect(  0,  64,  16,  80);			// 16 -- 5-1
	AddTexRect( 16,  64,  32,  80);			// 17 -- 5-2
	AddTexRect( 32,  64,  48,  80);			// 18 -- 5-3
	AddTexRect( 48,  64,  64,  80);			// 19 -- 5-4
	AddTexRect(  0,  80,  16,  96);			// 20 -- 6-1
	AddTexRect( 16,  80,  32,  96);			// 21 -- 6-2
	AddTexRect( 32,  80,  48,  96);			// 22 -- 6-3
	AddTexRect( 48,  80,  64,  96);			// 23 -- 6-4
	AddTexRect(  0,  96,  16, 112);			// 24 -- 7-1
	AddTexRect( 16,  96,  32, 112);			// 25 -- 7-2
	AddTexRect( 32,  96,  48, 112);			// 26 -- 7-3
	AddTexRect( 48,  96,  64, 112);			// 27 -- 7-4
	AddTexRect(  0, 112,  16, 128);			// 28 -- 8-1
	AddTexRect( 16, 112,  32, 128);			// 29 -- 8-2
	AddTexRect( 32, 112,  48, 128);			// 30 -- 8-3
	AddTexRect( 48, 112,  64, 128);			// 31 -- 8-4
	AddTexRect(112,  80, 128,  96);			// 32 -- 6-8
	AddTexRect(112,  96, 128, 112);			// 33 -- 7-8
	
	for (int i = 0; i < cMinCol; i++)
	{
		m_MapData[HALFSIZE][iMapColEnd + i].iTex = 0;
		m_MapData[HALFSIZE][iMapColEnd + i].bCollision = false;
		m_MapData[HALFSIZE + 1][iMapColEnd + i].iTex = 32;

		int c = iMapColBeg - 1 - i;
		if (c < 0) c += MAXSIZE;

		m_MapData[HALFSIZE][c].iTex = 0;
		m_MapData[HALFSIZE][c].bCollision = false;
		m_MapData[HALFSIZE + 1][c].iTex = 32;
	}

	for (int i = iMapRowBeg; i < iMapRowEnd; i++)
		for (int j = iMapColBeg; j < iMapColEnd; j++)
		{
			m_MapData[i][j].iTex = rand() % 50 < 49 ? (rand() % 5 != 0 ? 0 : 1) : 2;
			m_MapData[i][j].bCollision = false;
		}

	// Top Bound

	m_MapData[iMapRowBeg][iMapColBeg].iTex = 4;
	m_MapData[iMapRowBeg][iMapColBeg].bCollision = true;

	for (int i = 1; i < cCol - 1; i++)
	{
		m_MapData[iMapRowBeg + 0][iMapColBeg + i].iTex = 5;
		m_MapData[iMapRowBeg + 0][iMapColBeg + i].bCollision = true;
		m_MapData[iMapRowBeg + 1][iMapColBeg + i].iTex = 9;
		m_MapData[iMapRowBeg + 1][iMapColBeg + i].bCollision = true;
		m_MapData[iMapRowBeg + 2][iMapColBeg + i].iTex = 9;
		m_MapData[iMapRowBeg + 2][iMapColBeg + i].bCollision = true;
		m_MapData[iMapRowBeg + 3][iMapColBeg + i].iTex = 31;
	}

	m_MapData[iMapRowBeg][iMapColEnd - 1].iTex = 6;
	m_MapData[iMapRowBeg][iMapColEnd - 1].bCollision = true;

	// Left/Right Bound

	for (int i = iMapRowBeg + 1; i < HALFSIZE - 4; i++)
	{
		m_MapData[i][iMapColBeg].iTex = 8;
		m_MapData[i][iMapColBeg].bCollision = true;
	}

	for (int i = iMapRowBeg + 3; i < HALFSIZE - 1; i++)
	{
		m_MapData[i][iMapColBeg + 1].iTex = 23;
		m_MapData[i][iMapColEnd - 1].iTex = 10;
		m_MapData[i][iMapColEnd - 1].bCollision = true;
	}

	m_MapData[HALFSIZE - 4][iMapColBeg].iTex = 16;
	m_MapData[HALFSIZE - 4][iMapColBeg].bCollision = true;
	m_MapData[HALFSIZE - 3][iMapColBeg].iTex = 20;
	m_MapData[HALFSIZE - 3][iMapColBeg].bCollision = true;
	m_MapData[HALFSIZE - 2][iMapColBeg].iTex = 24;
	m_MapData[HALFSIZE - 2][iMapColBeg].bCollision = true;

	m_MapData[HALFSIZE - 4][iMapColEnd - 1].iTex = 14;
	m_MapData[HALFSIZE - 4][iMapColEnd - 1].bCollision = true;
	m_MapData[HALFSIZE - 3][iMapColEnd - 1].iTex = 22;
	m_MapData[HALFSIZE - 3][iMapColEnd - 1].bCollision = true;
	m_MapData[HALFSIZE - 2][iMapColEnd - 1].iTex = 26;
	m_MapData[HALFSIZE - 2][iMapColEnd - 1].bCollision = true;

	m_MapData[HALFSIZE - 1][iMapColBeg + 0].iTex = 28;
	m_MapData[HALFSIZE - 1][iMapColBeg + 1].iTex = 29;
	m_MapData[HALFSIZE - 1][iMapColEnd - 1].iTex = 30;

	m_MapData[iMapRowBeg + 3][iMapColBeg + 1].iTex = 25;
	m_MapData[iMapRowBeg + 1][iMapColEnd - 1].iTex = 11;
	m_MapData[iMapRowBeg + 1][iMapColEnd - 1].bCollision = true;
	m_MapData[iMapRowBeg + 2][iMapColEnd - 1].iTex = 15;
	m_MapData[iMapRowBeg + 2][iMapColEnd - 1].bCollision = true;

	for (int i = HALFSIZE + 2; i < iMapRowEnd - 4; i++)
	{
		m_MapData[i][iMapColBeg].iTex = 8;
		m_MapData[i][iMapColBeg].bCollision = true;
	}

	m_MapData[HALFSIZE + 4][iMapColBeg + 1].iTex = 27;

	for (int i = HALFSIZE + 5; i < iMapRowEnd - 1; i++)
	{
		m_MapData[i][iMapColBeg + 1].iTex = 23;
	}

	m_MapData[iMapRowEnd - 4][iMapColBeg].iTex = 16;
	m_MapData[iMapRowEnd - 4][iMapColBeg].bCollision = true;
	m_MapData[iMapRowEnd - 3][iMapColBeg].iTex = 20;
	m_MapData[iMapRowEnd - 3][iMapColBeg].bCollision = true;
	m_MapData[iMapRowEnd - 2][iMapColBeg].iTex = 24;
	m_MapData[iMapRowEnd - 2][iMapColBeg].bCollision = true;

	for (int i = HALFSIZE + 4; i < iMapRowEnd - 4; i++)
	{
		m_MapData[i][iMapColEnd - 1].iTex = 10;
		m_MapData[i][iMapColEnd - 1].bCollision = true;
	}

	m_MapData[iMapRowEnd - 4][iMapColEnd - 1].iTex = 14;
	m_MapData[iMapRowEnd - 4][iMapColEnd - 1].bCollision = true;
	m_MapData[iMapRowEnd - 3][iMapColEnd - 1].iTex = 22;
	m_MapData[iMapRowEnd - 3][iMapColEnd - 1].bCollision = true;
	m_MapData[iMapRowEnd - 2][iMapColEnd - 1].iTex = 26;
	m_MapData[iMapRowEnd - 2][iMapColEnd - 1].bCollision = true;

	m_MapData[HALFSIZE + 2][iMapColEnd - 1].iTex = 11;
	m_MapData[HALFSIZE + 2][iMapColEnd - 1].bCollision = true;
	m_MapData[HALFSIZE + 3][iMapColEnd - 1].iTex = 11;
	m_MapData[HALFSIZE + 3][iMapColEnd - 1].bCollision = true;
	m_MapData[HALFSIZE + 4][iMapColEnd - 1].iTex = cRow <= 15 ? 18 : 7;
	m_MapData[HALFSIZE + 4][iMapColEnd - 1].bCollision = true;

	//Bottom Bound

	for (int i = 0; i < cCol; i++)
	{
		m_MapData[iMapRowEnd - 1][iMapColBeg + i].iTex = 32;
		m_MapData[iMapRowEnd - 1][iMapColBeg + i].bCollision = true;
	}

	//Random Holes

	std::vector<int> vecCol;
	int curCol = iMapColBeg + cGap;

	while (curCol <= iMapColEnd - cGap)
	{
		vecCol.push_back(curCol);
		curCol += cGap + rand() % 3;
	}

	for (auto c:vecCol)
	{
		for (int i = 0; i < cHole; i++)
		{
			bool bNewHole = true;
			int cTrial = 0;
			while (bNewHole && cTrial < 3)
			{
				bNewHole = false;

				int row = iMapRowBeg + 3 + rand() % (cRow - 4);

				if (m_MapData[row][c].bCollision)
				{
					bNewHole = true;
					cTrial++;
				}
				else
				{
					if (m_MapData[row - 1][c].iTex == 32 ||
						m_MapData[row - 1][c].iTex == -1)
						m_MapData[row][c].iTex = -1;
					else
						m_MapData[row][c].iTex = 32;

					m_MapData[row][c].bCollision = true;

					if (m_MapData[row + 1][c].iTex == 32)
						m_MapData[row + 1][c].iTex = -1;
				}
			}
		}

		//Ensure each path has at least two blocks' width

		bool bRevise = true;
		for (int i = iMapRowBeg + 3; i < iMapRowEnd - 2; i++)
		{
			if (m_MapData[i][c].bCollision == false)
			{
				if (bRevise && m_MapData[i + 1][c].bCollision == true)
				{
					m_MapData[i + 1][c].iTex = 0;
					m_MapData[i + 1][c].bCollision = false;
					if (m_MapData[i + 2][c].iTex == -1)
						m_MapData[i + 2][c].iTex = 32;
				}
				bRevise = false;
			}
			else
				bRevise = true;
		}
	}

//	for (int i = 0; i < cHole; i++)
//	{
//		bool bNew = true;
//		while (bNew)
//		{
//			bNew = false;
//			int row = iMapRowBeg + 4 + rand() % (cRow - 5);
//			int col = iMapColBeg + 1 + rand() % (cCol - 2);
//			if (IsCollision(row, col))
//				bNew = true;
//			else
//			{
//				if (m_MapData[row - 1][col].iTex == 32)
//					m_MapData[row][col].iTex = -1;
//				else
//					m_MapData[row][col].iTex = 32;
//
//				m_MapData[row][col].bCollision = true;
//				
//				if (m_MapData[row + 1][col].iTex == 32)
//					m_MapData[row + 1][col].iTex = -1;
//			}
//		}
//	}

	//Initialization of VertexBuffer

	SAFE_RELEASE(m_pVertexBuffer);
	m_pd3dDevice->CreateVertexBuffer(4 * MAXSIZE * MAXSIZE * sizeof(TEXVERTEX), 0, TEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	for (int i = 0; i < MAXSIZE; i++)
		for (int j = 0; j < MAXSIZE; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				float left = j * fSpacing;
				float right = (j + 1) * fSpacing;
				if (j >= HALFSIZE)
				{
					left = (j - MAXSIZE)*fSpacing;
					right = (j + 1 - MAXSIZE)*fSpacing;
				}
				float top = i * fSpacing;
				float bottom = (i + 1) * fSpacing;
				RECTRANGE TexRc = m_TexRects[m_MapData[i][j].iTex];
				TEXVERTEX *pVertices;
				m_pVertexBuffer->Lock(4 * (i * MAXSIZE + j) * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
				pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
				pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
				pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
				pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
				m_pVertexBuffer->Unlock();
			}
		}
}

Map::~Map()
{
	DXObject::~DXObject();
}

BOOL Map::Update(double fTime, float fElapsedTime)
{
	return TRUE;
}

BOOL Map::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, m_pTexture);
	for (int i = 0; i < MAXSIZE; i++)
		for (int j = 0; j < MAXSIZE; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
				m_pd3dDevice->SetFVF(TEX_FVF);
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (i * MAXSIZE + j), 2);
			}
		}

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL Map::IsCollision(int row, int col)
{
	if (row < 0 || row >= MAXSIZE || col >= MAXSIZE)
		return TRUE;

	if (col >= 0)
		return m_MapData[row][col].bCollision;
	else
	{
		if (bInterStage)
			return m_MapData[row][col + MAXSIZE].bCollision;
		else
			return true;
	}
}

//
//Player
//

Player::Player()
{
	SetRange(-64, -144, 64, 0);
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(iCharaID), 256, 256);
	Init();
	AddTexRect(  0,   0,  32,  36);				 // 0  -- BW
	AddTexRect(  0,   0,  32,  36);				 // 1  -- D-1
	AddTexRect( 32,   0,  64,  36);				 // 2  -- D-2
	AddTexRect( 64,   0,  96,  36);				 // 3  -- D-3
	AddTexRect( 96,   0, 128,  36);				 // 4  -- D-4
	AddTexRect(128,   0, 160,  36);				 // 5  -- D-5
	AddTexRect(160,   0, 192,  36);				 // 6  -- D-6
	AddTexRect(192,   0, 224,  36);				 // 7  -- D-7
	AddTexRect(224,   0, 256,  36);				 // 8  -- D-8
	AddTexRect(  0,  36,  32,  72);				 // 9  -- U-1
	AddTexRect( 32,  36,  64,  72);				 // 10 -- U-2
	AddTexRect( 64,  36,  96,  72);				 // 11 -- U-3
	AddTexRect( 96,  36, 128,  72);				 // 12 -- U-4
	AddTexRect(128,  36, 160,  72);				 // 13 -- U-5
	AddTexRect(160,  36, 192,  72);				 // 14 -- U-6
	AddTexRect(192,  36, 224,  72);				 // 15 -- U-7
	AddTexRect(224,  36, 256,  72);				 // 16 -- U-8
	AddTexRect(  0,  72,  32, 108);				 // 17 -- L-1
	AddTexRect( 32,  72,  64, 108);				 // 18 -- L-2
	AddTexRect( 64,  72,  96, 108);				 // 19 -- L-3
	AddTexRect( 96,  72, 128, 108);				 // 20 -- L-4
	AddTexRect(128,  72, 160, 108);				 // 21 -- L-5
	AddTexRect(160,  72, 192, 108);				 // 22 -- L-6
	AddTexRect(192,  72, 224, 108);				 // 23 -- L-7
	AddTexRect(224,  72, 256, 108);				 // 24 -- L-8
	AddTexRect(  0, 108,  32, 144);				 // 25 -- R-1
	AddTexRect( 32, 108,  64, 144);				 // 26 -- R-2
	AddTexRect( 64, 108,  96, 144);				 // 27 -- R-3
	AddTexRect( 96, 108, 128, 144);				 // 28 -- R-4
	AddTexRect(128, 108, 160, 144);				 // 29 -- R-5
	AddTexRect(160, 108, 192, 144);				 // 30 -- R-6
	AddTexRect(192, 108, 224, 144);				 // 31 -- R-7
	AddTexRect(224, 108, 256, 144);				 // 32 -- R-8
	AddTexRect(  0, 144,  32, 180);				 // 33 -- E-1
	AddTexRect( 32, 144,  64, 180);				 // 34 -- E-2
	AddTexRect( 64, 144,  96, 180);				 // 35 -- E-3
	AddTexRect( 96, 144, 128, 180);				 // 36 -- E-4
	AddTexRect(128, 144, 160, 180);				 // 37 -- E-5
	AddTexRect(160, 144, 192, 180);				 // 38 -- E-6

	float fSPS = (float)fSPB / 4.0f; //time per step
	AddState(D_STAY, LOOP, (float)fSPB, 1, 2, 1, 2, 8, 2, 1, 2);
	AddState(D_MOVE, LOOP, (float)fSPS, 3, 1, 4, 1);
	AddState(U_STAY, LOOP, (float)fSPB, 9, 10, 9, 10, 16, 10, 9, 10);
	AddState(U_MOVE, LOOP, (float)fSPS, 11, 9, 12, 9);
	AddState(L_STAY, LOOP, (float)fSPB, 17, 18, 17, 18, 24, 18, 17, 18);
	AddState(L_MOVE, LOOP, (float)fSPS, 20, 19);
	AddState(R_STAY, LOOP, (float)fSPB, 25, 26, 25, 26, 32, 26, 25, 26);
	AddState(R_MOVE, LOOP, (float)fSPS, 28, 27);
	AddState(HAPPY, LOOP, (float)fBaseSPB, 37, 38);
	AddState(DEATH, SINGLE, 0.0f, 1);

	SetState(R_STAY);
	SetGrid(HALFSIZE, 0);
	m_UpdateTime = (float)fBegTime - (float)fSPB;

	m_timeMove = 0.0f;
	m_bCrashed = false;

	m_pShadow = new DXObject();
	m_pShadow->SetRange(-64, -16, 64, 16);
	m_pShadow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_SHADOW), 64, 16);
	m_pShadow->Init();
	m_pShadow->AddTexRect(0, 0, 32, 8);
	m_pShadow->AddState(0, SINGLE, 0.0f, 0);
	m_pShadow->SetState(0);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);
}

Player::~Player()
{
	SAFE_DELETE(m_pShadow);
	DXObject::~DXObject();
}

BOOL Player::Update(double fTime, float fElapsedTime)
{
	GetInput();

	static bool bMove = false;
	float fSPM = (float)fSPB;
	if (bInterStage)
	{
		fSPM = 0.4f * (float)fBaseSPB;
		m_UpdateTime += (fRate * 1.6f - 1.0f) * fElapsedTime;
	}

	if (!bGameOver && !bComplete)
	{
		switch (m_iState)
		{
		case D_STAY:
		case U_STAY:
		case L_STAY:
		case R_STAY:
			bMove = false;
			if (IsKeyDown(DIK_UP) || IsKeyDown(DIK_W))
			{
				pSE1->Play(0);
				SetState(U_MOVE);
				if (Access(m_iRow - 1, m_iCol))
					bMove = true;
			}
			else if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
			{
				pSE1->Play(0);
				SetState(D_MOVE);
				if (Access(m_iRow + 1, m_iCol))
					bMove = true;
			}
			else if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
			{
				pSE1->Play(0);
				SetState(L_MOVE);
				if (Access(m_iRow, m_iCol - 1))
					bMove = true;
			}
			else if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
			{
				pSE1->Play(0);
				SetState(R_MOVE);
				if (Access(m_iRow, m_iCol + 1))
					bMove = true;
			}
			m_timeMove = 0.0f;
			break;

		case U_MOVE:
			m_timeMove += fElapsedTime;
			if (bMove)
			{
				if (m_timeMove >= fSPM)
				{
					SetGrid(m_iRow - 1, m_iCol);
					SetState(U_STAY);
					pSE1->Play(0);
				}
				else
					m_ObjPos.y = (m_iRow + 0.5f - m_timeMove / fSPM) * fSpacing;
			}
			else
			{
				if (m_timeMove >= fSPM)
				{
					SetState(U_STAY);
					pSE1->Play(0);
				}
			}
			break;

		case D_MOVE:
			m_timeMove += fElapsedTime;
			if (bMove)
			{
				if (m_timeMove >= fSPM)
				{
					SetGrid(m_iRow + 1, m_iCol);
					SetState(D_STAY);
					pSE1->Play(0);
				}
				else
					m_ObjPos.y = (m_iRow + 0.5f + m_timeMove / fSPM) * fSpacing;
			}
			else
			{
				if (m_timeMove >= fSPM)
				{
					SetState(D_STAY);
					pSE1->Play(0);
				}
			}
			break;

		case L_MOVE:
			m_timeMove += fElapsedTime;
			if (bMove)
			{
				if (m_timeMove >= fSPM)
				{
					SetGrid(m_iRow, m_iCol - 1);
					SetState(L_STAY);
					pSE1->Play(0);
				}
				else
					m_ObjPos.x = (m_iCol + 0.5f - m_timeMove / fSPM) * fSpacing;
			}
			else
			{
				if (m_timeMove >= fSPM)
				{
					SetState(L_STAY);
					pSE1->Play(0);
				}
			}
			break;

		case R_MOVE:
			m_timeMove += fElapsedTime;
			if (bMove)
			{
				if (m_timeMove >= fSPM)
				{
					SetGrid(m_iRow, m_iCol + 1);
					SetState(R_STAY);
					pSE1->Play(0);
				}
				else
					m_ObjPos.x = (m_iCol + 0.5f + m_timeMove / fSPM) * fSpacing;
			}
			else
			{
				if (m_timeMove >= fSPM)
				{
					SetState(R_STAY);
					pSE1->Play(0);
				}
			}
			break;
		}
		m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);
		m_timeDamage = max(0.0f, m_timeDamage - fElapsedTime);
	}

	DXObject::Update(fTime, fElapsedTime);
	return TRUE;
}

BOOL Player::Render(double fTime, float fElapsedTime)
{
	if (m_bCrashed)
		return FALSE;

	m_pShadow->Render(fTime, fElapsedTime);

	if (m_timeDamage > 0.0f)
	{
		int iShow = (int)(4.0f * m_timeDamage / fSPB);
		if (iShow % 2 == 0)
			return TRUE;
	}

	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL Player::SetState(int iState)
{
	if (iState != m_iState)
		return DXObject::SetState(iState);

	return FALSE;
}

BOOL Player::SetGrid(int row, int col)
{
	if (pMap->IsCollision(row, col))
		return FALSE;

	m_iRow = row;
	m_iCol = col;
	SetPos((m_iCol + 0.5f) * fSpacing, (m_iRow + 0.5f) * fSpacing);
	return TRUE;
}

int Player::GetRow(bool bAct)
{
	if (bAct)
		return (int)(m_ObjPos.y / fSpacing) - (m_ObjPos.y < 0.0f ? 1 : 0);
	else
		return m_iRow;
}

int Player::GetCol(bool bAct)
{
	if (bAct)
		return (int)(m_ObjPos.x / fSpacing) - (m_ObjPos.x < 0.0f ? 1 : 0);
	else
		return m_iCol;
}

void Player::Damaged()
{
	if (!IsDebugMode())
		iDamage++;

	m_timeDamage = (float)fSPB;

	if (!m_bCrashed)
	{
		pSE0->Play(1);
	}
}

void Player::Crashed()
{
	if (!m_bCrashed)
	{
		pSE0->Play(2);
		m_bCrashed = true;
	}
}

bool Player::Access(int row, int col)
{
	if (pMap->IsCollision(row, col))
		return false;
	if (NULL != pPunch && pPunch->IsPunched(row, col))
		return false;
	for (auto p : vecEnemy)
	{
		if (row == p->GetRow() && col == p->GetCol() && !p->IsCrashed())
			return false;
	}
	return true;
}

//
//Scanner
//

Scanner::Scanner()
{
	m_pd3dDevice->CreateVertexBuffer(8 * sizeof(COLORVERTEX), 0, COLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	float left = 0.0f;
	float right = 15.0f * fSpacing / 16.0f;
	float top = 0.0f;
	float bottom = 15.0f * fSpacing / 16.0f;

	COLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0] = { left,	bottom,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 0, 120, 0) };
	pVertices[1] = { left,	   top,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 0, 120, 0) };
	pVertices[2] = { right,	bottom,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 0, 120, 0) };
	pVertices[3] = { right,	   top,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 0, 120, 0) };
	pVertices[4] = { left,	bottom,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 120, 0, 0) };
	pVertices[5] = { left,	   top,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 120, 0, 0) };
	pVertices[6] = { right,	bottom,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 120, 0, 0) };
	pVertices[7] = { right,	   top,	0.1f*m_Layer, D3DCOLOR_ARGB(iScanAlpha, 120, 0, 0) };

	m_pVertexBuffer->Unlock();

	m_iRow = pPlayer->GetRow();
	m_iCol = pPlayer->GetCol();
	m_iRange = iScanRange;

	for (int i = -m_iRange; i <= m_iRange; i++)
	{
		int colRange = i > 0 ? m_iRange - i : m_iRange + i;
		for (int j = -colRange; j <= colRange; j++)
		{
			m_vecPoints.push_back({ m_iRow + i, m_iCol + j });
		}
	}
}

Scanner::~Scanner()
{
	DXObject::~DXObject();
}

BOOL Scanner::Update(double fTime, float fElapsedTime)
{
	if (m_iRow != pPlayer->GetRow() || m_iCol != pPlayer->GetCol())
	{
		m_iRow = pPlayer->GetRow();
		m_iCol = pPlayer->GetCol();
		m_vecPoints.clear();

		for (int i = -m_iRange; i <= m_iRange; i++)
		{
			int colRange = i > 0 ? m_iRange - i : m_iRange + i;
			for (int j = -colRange; j <= colRange; j++)
			{
				m_vecPoints.push_back({ m_iRow + i, m_iCol + j });
			}
		}
	}

	return TRUE;
}

BOOL Scanner::Render(double fTime, float fElapsedTime)
{
	if (bGameOver || bComplete)
		return FALSE;

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture(0, NULL);

	for (auto c : m_vecPoints)
	{
		if (!pMap->IsCollision(c.row, c.col))
		{
			D3DXMatrixTranslation(&matWorld, c.col * fSpacing, c.row * fSpacing, 0.0f);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

			int iType = 0;
			if (NULL != pPunch)
			{
				for (auto punch : pPunch->m_vecPunch)
				{
					if (c.row == punch.row && c.col == punch.col)
					{
						iType = 1;
						break;
					}
				}
			}
			m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(COLORVERTEX));
			m_pd3dDevice->SetFVF(COLOR_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * iType, 2);
		}
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Enemy
//

Enemy::Enemy()
{
	SetRange(-64, -144, 64, 0);
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(IDC_MONOKUMA), 256, 256);
	Init();
	AddTexRect( 32, 180,  64, 216);				 // 0  -- BREAK
	AddTexRect(  0,   0,  32,  36);				 // 1  -- D-1
	AddTexRect( 32,   0,  64,  36);				 // 2  -- D-2
	AddTexRect( 64,   0,  96,  36);				 // 3  -- D-3
	AddTexRect( 96,   0, 128,  36);				 // 4  -- D-4
	AddTexRect(128,   0, 160,  36);				 // 5  -- D-5
	AddTexRect(160,   0, 192,  36);				 // 6  -- D-6
	AddTexRect(192,   0, 224,  36);				 // 7  -- D-7
	AddTexRect(224,   0, 256,  36);				 // 8  -- D-8
	AddTexRect(  0,  36,  32,  72);				 // 9  -- U-1
	AddTexRect( 32,  36,  64,  72);				 // 10 -- U-2
	AddTexRect( 64,  36,  96,  72);				 // 11 -- U-3
	AddTexRect( 96,  36, 128,  72);				 // 12 -- U-4
	AddTexRect(128,  36, 160,  72);				 // 13 -- U-5
	AddTexRect(160,  36, 192,  72);				 // 14 -- U-6
	AddTexRect(192,  36, 224,  72);				 // 15 -- U-7
	AddTexRect(224,  36, 256,  72);				 // 16 -- U-8
	AddTexRect(  0,  72,  32, 108);				 // 17 -- L-1
	AddTexRect( 32,  72,  64, 108);				 // 18 -- L-2
	AddTexRect( 64,  72,  96, 108);				 // 19 -- L-3
	AddTexRect( 96,  72, 128, 108);				 // 20 -- L-4
	AddTexRect(128,  72, 160, 108);				 // 21 -- L-5
	AddTexRect(160,  72, 192, 108);				 // 22 -- L-6
	AddTexRect(192,  72, 224, 108);				 // 23 -- L-7
	AddTexRect(224,  72, 256, 108);				 // 24 -- L-8
	AddTexRect(  0, 108,  32, 144);				 // 25 -- R-1
	AddTexRect( 32, 108,  64, 144);				 // 26 -- R-2
	AddTexRect( 64, 108,  96, 144);				 // 27 -- R-3
	AddTexRect( 96, 108, 128, 144);				 // 28 -- R-4
	AddTexRect(128, 108, 160, 144);				 // 29 -- R-5
	AddTexRect(160, 108, 192, 144);				 // 30 -- R-6
	AddTexRect(192, 108, 224, 144);				 // 31 -- R-7
	AddTexRect(224, 108, 256, 144);				 // 32 -- R-8
	AddTexRect(  0, 144,  32, 180);				 // 33 -- E-1
	AddTexRect( 32, 144,  64, 180);				 // 34 -- E-2
	AddTexRect( 64, 144,  96, 180);				 // 35 -- E-3
	AddTexRect( 96, 144, 128, 180);				 // 36 -- E-4
	AddTexRect(128, 144, 160, 180);				 // 37 -- E-5
	AddTexRect(160, 144, 192, 180);				 // 38 -- E-6

	float fSPS = (float)fSPB / 4.0f; //time per step
	AddState(D_STAY, LOOP, (float)fSPB, 1, 2);
	AddState(D_MOVE, LOOP, (float)fSPS, 3, 1, 4, 1);
	AddState(U_STAY, LOOP, (float)fSPB, 9, 10);
	AddState(U_MOVE, LOOP, (float)fSPS, 11, 9, 12, 9);
	AddState(L_STAY, LOOP, (float)fSPB, 17, 18);
	AddState(L_MOVE, LOOP, (float)fSPS, 20, 19);
	AddState(R_STAY, LOOP, (float)fSPB, 25, 26);
	AddState(R_MOVE, LOOP, (float)fSPS, 28, 27);
	AddState(D_OFF, SINGLE, 0.0f, 8);
	AddState(U_OFF, SINGLE, 0.0f, 16);
	AddState(L_OFF, SINGLE, 0.0f, 24);
	AddState(R_OFF, SINGLE, 0.0f, 32);
	AddState(DEATH, SINGLE, 0.0f, 0);
	SetState(L_STAY);
	m_UpdateTime = (float)fBegTime - (float)fSPB;

	bool bSetGrid = true;
	while (bSetGrid)
	{
		bSetGrid = false;
		int row = iMapRowBeg + 3 + rand() % (cRow - 4);
		int col = iMapColBeg + 1 + rand() % (cCol - 2);
		if (!SetGrid(row, col))
			bSetGrid = true;
	}

	m_iLP = 5;
	m_bMove = false;
	m_timeMove = 0.0f;
	m_bCrashed = false;
	m_bObserved = false;
	m_bLevelUp = false;

	m_pShadow = new DXObject();
	m_pShadow->SetRange(-64, -16, 64, 16);
	m_pShadow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_SHADOW), 64, 16);
	m_pShadow->Init();
	m_pShadow->AddTexRect(0, 8, 32, 16);
	m_pShadow->AddState(0, SINGLE, 0.0f, 0);
	m_pShadow->SetState(0);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);
}

Enemy::~Enemy()
{
	SAFE_DELETE(m_pShadow);
	DXObject::~DXObject();
}

BOOL Enemy::Update(double fTime, float fElapsedTime)
{
	if (m_bCrashed)
		return FALSE;

	if (!bGameOver && abs(m_iRow - pPlayer->GetRow()) + abs(m_iCol - pPlayer->GetCol()) <= iScanRange)
		Observed(true);
	else
		Observed(false);

	if (m_bMove)
	{
		m_timeMove += fElapsedTime;
		if (m_timeMove >= 0.0f)
		{
			m_bMove = false;

			if (!m_bObserved)
			{
				bool bSafe[5] = { false,false,false,false,false };
				std::vector<int> vecTarget;
				int iTarget;

				if (Access(m_iRow, m_iCol))
					bSafe[0] = true;
				if (Access(m_iRow, m_iCol - 1))
					bSafe[1] = true;
				if (Access(m_iRow, m_iCol + 1))
					bSafe[2] = true;
				if (Access(m_iRow - 1, m_iCol))
					bSafe[3] = true;
				if (Access(m_iRow + 1, m_iCol))
					bSafe[4] = true;

				for (int i = 0; i < 5; i++)
				{
					if (bSafe[i])
						vecTarget.push_back(i);
				}
				if (vecTarget.size() == 0)
					iTarget = 0;
				else
				{
					iTarget = vecTarget[rand() % (int)vecTarget.size()];
				}

				switch (iTarget)
				{
				case 0:
					m_iTarRow = m_iRow;
					m_iTarCol = m_iCol;
					break;
				case 1:
					m_iTarRow = m_iRow;
					m_iTarCol = m_iCol - 1;
					SetState(L_MOVE);
					break;
				case 2:
					m_iTarRow = m_iRow;
					m_iTarCol = m_iCol + 1;
					SetState(R_MOVE);
					break;
				case 3:
					m_iTarRow = m_iRow - 1;
					m_iTarCol = m_iCol;
					SetState(U_MOVE);
					break;
				case 4:
					m_iTarRow = m_iRow + 1;
					m_iTarCol = m_iCol;
					SetState(D_MOVE);
					break;
				}
			}
		}
	}

	float fSPM = (float)fSPB;
	switch (m_iState)
	{
	case U_MOVE:
		m_timeMove += fElapsedTime;
			if (m_timeMove >= fSPM)
			{
				m_iRow--;
				SetState(U_STAY);
			}
			else
				m_ObjPos.y = (m_iRow + 0.5f - m_timeMove / fSPM) * fSpacing;
		break;

	case D_MOVE:
		m_timeMove += fElapsedTime;
			if (m_timeMove >= fSPM)
			{
				m_iRow++;
				SetState(D_STAY);
			}
			else
				m_ObjPos.y = (m_iRow + 0.5f + m_timeMove / fSPM) * fSpacing;
		break;

	case L_MOVE:
		m_timeMove += fElapsedTime;
			if (m_timeMove >= fSPM)
			{
				m_iCol--;
				SetState(L_STAY);
			}
			else
				m_ObjPos.x = (m_iCol + 0.5f - m_timeMove / fSPM) * fSpacing;
		break;

	case R_MOVE:
		m_timeMove += fElapsedTime;
			if (m_timeMove >= fSPM)
			{
				m_iCol++;
				SetState(R_STAY);
			}
			else
				m_ObjPos.x = (m_iCol + 0.5f + m_timeMove / fSPM) * fSpacing;
		break;
	}
	m_timeDamage = max(0.0f, m_timeDamage - fElapsedTime);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);

	DXObject::Update(fTime, fElapsedTime);
	return TRUE;
}

BOOL Enemy::Render(double fTime, float fElapsedTime)
{
	if (m_timeDamage > 0.0f)
	{
		int iShow = (int)(4.0f * m_timeDamage / fSPB);
		if (iShow % 2 == 0)
			return TRUE;
	}

	m_pShadow->Render(fTime, fElapsedTime);
	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL Enemy::SetGrid(int row, int col)
{
	if (pMap->IsCollision(row, col))
		return FALSE;

	if (NULL != pPunch)
	{
		for (auto c : pPunch->m_vecPunch)
		{
			if (row == c.row && col == c.col)
				return FALSE;
		}
	}

	for (auto p : vecEnemy)
	{
		if (row == p->m_iRow && col == p->m_iCol)
			return FALSE;
	}

	m_iRow = row;
	m_iCol = col;
	SetPos((m_iCol + 0.5f) * fSpacing, (m_iRow + 0.5f) * fSpacing);
	return TRUE;
}

int Enemy::GetRow(bool bAct)
{
	if (bAct)
		return (int)(m_ObjPos.y / fSpacing) - (m_ObjPos.y < 0.0f ? 1 : 0);
	else
		return m_iRow;
}

int Enemy::GetCol(bool bAct)
{
	if (bAct)
		return (int)(m_ObjPos.x / fSpacing) - (m_ObjPos.x < 0.0f ? 1 : 0);
	else
		return m_iCol;
}

void Enemy::Damaged()
{
	m_iLP--;
	if (m_iLP < 0)
		Crashed();
	m_timeDamage = (float)fSPB;
}

void Enemy::Crashed()
{
	if (!m_bCrashed)
	{
		int iType = 0;
		if (m_bLevelUp)
			iType = 1;
		else if (rand() % 100 < (5 + iStage + iLuc / 3))
			iType = 1;

		pItem->AddItem(iType, m_iRow, m_iCol);
		m_bCrashed = true;
		SetState(DEATH);
		m_ObjPos.y += 0.5f * fSpacing;
	}
}

bool Enemy::IsCrashed()
{
	return m_bCrashed;
}

void Enemy::Observed(bool bVal)
{
	m_bObserved = bVal;
	if (m_bObserved)
	{
		switch (m_iState)
		{
		case D_STAY:
			SetState(D_OFF);
			break;
		case U_STAY:
			SetState(U_OFF);
			break;
		case L_STAY:
			SetState(L_OFF);
			break;
		case R_STAY:
			SetState(R_OFF);
			break;
		}
	}
	else
	{
		switch (m_iState)
		{
		case D_OFF:
			SetState(D_STAY);
			break;
		case U_OFF:
			SetState(U_STAY);
			break;
		case L_OFF:
			SetState(L_STAY);
			break;
		case R_OFF:
			SetState(R_STAY);
			break;
		}
	}
}

void Enemy::Move()
{
	if (!m_bCrashed && !m_bObserved)
		m_bMove = true;
	m_timeMove = -(float)fSPB / fFrqPunch;

	m_iTarRow = 0;
	m_iTarCol = 0;
}

void Enemy::LevelUp()
{
	m_bLevelUp = true;
}

bool Enemy::Access(int row, int col)
{
	if (pMap->IsCollision(row, col))
		return false;

	if (col <= iMapColBeg || col >= iMapColEnd - 1)
		return false;

	if (NULL != pPunch)
	{
		for (auto c : pPunch->m_vecPunch)
		{
			if (c.row == row && c.col == col)
				return false;
		}
	}

	for (auto p : vecEnemy)
	{
		if (p->m_iTarRow == row && p->m_iTarCol == col)
			return false;
		if (p->m_iRow == row && p->m_iCol == col)
			return false;
	}

	return true;
}

//
//Monomi
//

Monomi::Monomi()
{
	SetRange(-64, -144, 64, 0);
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(IDC_MONOMI), 256, 256);
	Init();
	AddTexRect( 64, 144,  96, 180);				 // 0 -- E-3
	AddTexRect( 96, 144, 128, 180);				 // 1 -- E-4

	float fSPS = (float)fSPB / 4.0f; //time per step
	AddState(CRY, LOOP, (float)fBaseSPB, 0, 1);
	SetState(CRY);

	m_pShadow = new DXObject();
	m_pShadow->SetRange(-64, -16, 64, 16);
	m_pShadow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_SHADOW), 64, 16);
	m_pShadow->Init();
	m_pShadow->AddTexRect(0, 8, 32, 16);
	m_pShadow->AddState(0, SINGLE, 0.0f, 0);
	m_pShadow->SetState(0);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);
}

Monomi::~Monomi()
{
	SAFE_DELETE(m_pShadow);
	DXObject::~DXObject();
}

BOOL Monomi::Update(double fTime, float fElapsedTime)
{
	DXObject::Update(fTime, fElapsedTime);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);

	return TRUE;
}

BOOL Monomi::Render(double fTime, float fElapsedTime)
{
	m_pShadow->Render(fTime, fElapsedTime);
	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

//
//Item
//

Item::Item()
{
	SetRange(0.0f, -1.5f * fSpacing, fSpacing, 0.5f * fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	Init();
	AddTexRect( 0,  0, 16, 32);
	AddTexRect(16,  0, 32, 32);
	AddTexRect(32,  0, 48, 32);
	AddTexRect(48,  0, 64, 32);
	AddTexRect( 0, 32, 16, 64);
	AddTexRect(16, 32, 32, 64);
	AddTexRect(32, 32, 48, 64);
	AddTexRect(48, 32, 64, 64);

	m_pd3dDevice->CreateVertexBuffer(4 * m_TexRects.size() * sizeof(TEXVERTEX),
		0, TEX_FVF,	D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	for (int i = 0; i < (int)m_TexRects.size(); i++)
	{
		float left = m_ObjRange.left;
		float top = m_ObjRange.top;
		float right = m_ObjRange.right;
		float bottom = m_ObjRange.bottom;
		RECTRANGE TexRc = m_TexRects[i];

		TEXVERTEX *pVertices;
		m_pVertexBuffer->Lock(4 * i * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
		pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
		pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
		pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
		pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
		m_pVertexBuffer->Unlock();
	}

	m_UpdateTime = 0.0f;
	m_iTex = 0;

	for (int i = 0; i < cItem; i++)
	{
		bool bNew = true;
		while (bNew)
		{
			int row = iMapRowBeg + rand() % cRow;
			int col = iMapColBeg + rand() % cCol;
			int iType = rand() % 100 < (5 + iStage + iLuc / 3) ? 1 : 0;
			bNew = !AddItem(iType, row, col);
		}
	}

	m_pShadow = new DXObject();
	m_pShadow->SetRange(-64, -16, 64, 16);
	m_pShadow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_SHADOW), 64, 16);
	m_pShadow->Init();
	m_pShadow->AddTexRect(32, 0, 64, 8);
	m_pShadow->AddState(0, SINGLE, 0.0f, 0);
	m_pShadow->SetState(0);
}

Item::~Item()
{
	SAFE_DELETE(m_pShadow);
	DXObject::~DXObject();
}

BOOL Item::Update(double fTime, float fElapsedTime)
{
	auto iter = m_vecPoints.begin();
	while (iter != m_vecPoints.end())
	{
		if (iter->col == pPlayer->GetCol() && iter->row == pPlayer->GetRow())
		{
			if (iter->iType == 0)
				iHopeFragment++;
			else
				iGoldFragment++;
			iFragment++;
			iPlayerLP++;
			if (iPlayerLP > iMaxLP) iPlayerLP = iMaxLP;
			timeTimer += timeItem;
			pFLInfo->Add(ADDTIME, 3.2f * fSpacing, 16.0f * fSpacing);
			pSE0->Play(0);
			iter = m_vecPoints.erase(iter);
		}
		else 
		{
			bool bDestroy = false;
			for (auto p : vecEnemy)
			{
				if (!p->IsCrashed() && iter->col == p->GetCol() && iter->row == p->GetRow())
				{
					bDestroy = true;
					p->LevelUp();
					iter = m_vecPoints.erase(iter);
					break;
				}
			}
			if (!bDestroy)
				iter++;
		}
	}

	m_UpdateTime += fElapsedTime;
	if (m_UpdateTime >= (float)fBaseSPB / 4)
	{
		m_iTex++;
		if (m_iTex >= 4) m_iTex = 0;
		m_UpdateTime -= (float)fBaseSPB / 4;
	}

	m_pShadow->Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL Item::Render(double fTime, float fElapsedTime)
{
	for (auto c : m_vecPoints)
	{
		m_pShadow->SetPos((c.col + 0.5f) * fSpacing, (c.row + 0.5f) * fSpacing);
		m_pShadow->Render(fTime, fElapsedTime);
	}

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture(0, m_pTexture);

	for (auto c : m_vecPoints)
	{
		D3DXMatrixTranslation(&matWorld, c.col * fSpacing, c.row * fSpacing, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
		m_pd3dDevice->SetFVF(TEX_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (4 * c.iType + m_iTex), 2);
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

bool Item::AddItem(int iType, int row, int col)
{
	if (col <= iMapColBeg || col >= iMapColEnd - 1)
		return FALSE;

	if (pMap->IsCollision(row, col))
		return FALSE;

	for (auto c : m_vecPoints)
	{
		if (row == c.row && col == c.col)
			return FALSE;
	}

	m_vecPoints.push_back({ iType, row, col });
	return TRUE;
}

BOOL Item::Render(int iRow)
{
	for (auto c : m_vecPoints)
	{
		if (c.row == iRow)
		{
			m_pShadow->SetPos((c.col + 0.5f) * fSpacing, (c.row + 0.5f) * fSpacing);
			m_pShadow->Render(0.0f, 0.0f);
		}
	}

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture(0, m_pTexture);

	for (auto c : m_vecPoints)
	{
		if (c.row == iRow)
		{
			D3DXMatrixTranslation(&matWorld, c.col * fSpacing, c.row * fSpacing, 0.0f);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

			m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
			m_pd3dDevice->SetFVF(TEX_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (4 * c.iType + m_iTex), 2);
		}
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Punch
//

Punch::Punch(int cPunch)
{
	SetRange(0.0f, -fSpacing, fSpacing, fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM2), 64, 64);
	Init();
	AddTexRect(0, 32, 16, 64);
	AddState(0, SINGLE, 0.0f, 0);
	SetState(0);
	m_cPunch = cPunch;
	m_fTime = 4 * (float)fSPB / fFrqPunch - (float)fSPB * 0.8f - fPunchTime;
	m_fHeight = 2.0f * fSpacing;

	int rowPlayer = pPlayer->GetRow();
	int colPlayer = pPlayer->GetCol();

	for (int i = 0; i < m_cPunch; i++)
	{
//		bool bNew = true;
//		while (bNew)
//		{
//			int row = rand() % 3 != 0 ? (rowPlayer - 3 + rand() % 7) : (rowPlayer - 6 + rand() % 13);
//			int col = rand() % 3 != 0 ? (colPlayer - 5 + rand() % 11) : (colPlayer - 11 + rand() % 23);
//			bNew = !AddPunch(row, col);
//		}
		int row = rand() % 3 != 0 ? (rowPlayer - 3 + rand() % 7) : (rowPlayer - 6 + rand() % 13);
		int col = rand() % 3 != 0 ? (colPlayer - 5 + rand() % 11) : (colPlayer - 11 + rand() % 23);
		AddPunch(row, col);
	}
}

Punch::~Punch()
{
	DXObject::~DXObject();
}

BOOL Punch::Update(double fTime, float fElapsedTime)
{
	m_fTime -= fElapsedTime;

	if (m_fTime <= -0.6f * fSPB)
	{
		m_fHeight = -6 * (m_fTime + 0.6f * (float)fSPB) / (float)fSPB * fSpacing;
	}
	else
	{
		m_fHeight = 3 * m_fTime / (float)fSPB * fSpacing;

		if (m_fHeight < 0.0f) m_fHeight = 0.0f;
	}

	if (m_fHeight < fSpacing * 0.2f * (1.0f - 0.5f * (float)iAgi /18.0f))
	{
		int colPlayer = pPlayer->GetCol();
		int rowPlayer = pPlayer->GetRow();

		for (auto c : m_vecPunch)
		{
			if (c.col == colPlayer && c.row == rowPlayer)
			{
				if (IsDebugMode())
				{		
					int iTeleport = 1;
					int xTeleport = -1, yTeleport = 0;
					int xDelta = 1, yDelta = -1;
					while (!pPlayer->Access(rowPlayer + yTeleport, colPlayer + xTeleport))
					{
						xTeleport += xDelta;
						yTeleport += yDelta;
						if (yTeleport == iTeleport || yTeleport == -iTeleport)
							yDelta = -yDelta;
						if (xTeleport == iTeleport || xTeleport == -iTeleport)
							xDelta = -xDelta;
						if (xTeleport == -iTeleport)
						{
							iTeleport++;
							xTeleport = -iTeleport;
						}
					}
					pPlayer->SetGrid(rowPlayer + yTeleport, colPlayer + xTeleport);
					break;
				}
				else
				{
					if (!bGameOver)
					{
						bGameOver = true;
						iDeathReason = 2;
						pPlayer->Crashed();
					}
					else
					{
						pPlayer->Crashed();
					}
					iPlayerLP = 0;
					pBlood->Add(c.row, c.col, 1);
					break;
				}
			}
			for (auto p : vecEnemy)
			{
				if (c.col == p->GetCol() && c.row == p->GetRow())
				{
					p->Crashed();
				}
			}
		}
	}

	return TRUE;
}

BOOL Punch::Render(double fTime, float fElapsedTime)
{
	for (int i = 0; i < cRow; i++)
		Render(i);
	return TRUE;
}

BOOL Punch::AddPunch(int row, int col)
{
	if (col <= iMapColBeg || col >= iMapColEnd - 1)
		return FALSE;

	if (pMap->IsCollision(row, col))
		return FALSE;

	if (col == pPlayer->GetCol() && row == pPlayer->GetRow() - 1)
		return FALSE;

	for (auto c : pItem->m_vecPoints)
	{
		if (row == c.row && col == c.col)
			return FALSE;
	}

	for (auto c : m_vecPunch)
	{
		if (row == c.row && col == c.col)
			return FALSE;
	}
	m_vecPunch.push_back({ row, col });
	return TRUE;
}

BOOL Punch::IsPunched(int row, int col)
{
	if (m_fHeight > fSpacing * 0.2f)
		return FALSE;

	for (auto c : m_vecPunch)
	{
		if (row == c.row && col == c.col)
			return TRUE;
	}
	return FALSE;
}

BOOL Punch::Render(int iRow)
{
	if (m_fHeight > 1.5f * fSpacing)
		return FALSE;

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture(0, m_pTexture);

	for (auto c : m_vecPunch)
	{
		if (c.row == iRow)
		{
			D3DXMatrixTranslation(&matWorld, c.col * fSpacing, c.row * fSpacing - m_fHeight, 0.0f);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

			m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
			m_pd3dDevice->SetFVF(TEX_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Prick
//

Prick::Prick()
{
	SetRange(0.0f, -fSpacing * 2.0f / 16.0f, fSpacing, fSpacing * 14.0f / 16.0f);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM2), 64, 64);
	Init();
	AddTexRect( 0,  0, 16, 16);
	AddTexRect(16,  0, 32, 16);
	AddTexRect(32,  0, 48, 16);
	AddTexRect(48,  0, 64, 16);
	AddTexRect( 0, 16, 16, 32);
	AddTexRect(16, 16, 32, 32);
	AddTexRect(32, 16, 48, 32);
	AddTexRect(48, 16, 64, 32);
	AddState(0, NOLOOP, (float)fSPB * 0.1f, 0, 1, 2, 3);
	AddState(1, NOLOOP, (float)fSPB * 0.6f, 3);
	AddState(2, NOLOOP, (float)fSPB * 0.1f, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0);
	AddStateChain(0, 0, 1);
	AddStateChain(1, 0, 2);
	SetState(0);

	int row = pPlayer->GetRow();
	int col = pPlayer->GetCol();
	AddPrick(row, col);
	AddPrick(row - 1, col - 1);
	AddPrick(row - 1, col + 1);
	AddPrick(row + 1, col - 1);
	AddPrick(row + 1, col + 1);
	AddPrick(row + 2, col);
	AddPrick(row - 2, col);
	AddPrick(row, col + 2);
	AddPrick(row, col - 2);
}

Prick::~Prick()
{
	DXObject::~DXObject();
}

BOOL Prick::Update(double fTime, float fElapsedTime)
{
	DXObject::Update(fTime, fElapsedTime);

	if (m_iState == 2)
	{
		for (auto &c : m_vecPrick)
		{
			if (c.bDamage && c.col == pPlayer->GetCol() && c.row == pPlayer->GetRow())
			{
				if (iPlayerLP == 0)
				{
					if (!bGameOver)
					{
						bGameOver = true;
						iDeathReason = 1;
						pPlayer->SetState(DEATH);
					}
				}
				else
				{
					if (!bGameOver && !IsDebugMode())
						iPlayerLP--;
				}

				pPlayer->Damaged();
				pBlood->Add(c.row, c.col);
				c.bDamage = false;
				break;
			}
			for (auto p : vecEnemy)
			{
				if (c.bDamage && c.col == p->GetCol() && c.row == p->GetRow())
				{
					p->Damaged();
					c.bDamage = false;
					break;
				}
			}
		}
	}
	return TRUE;
}

BOOL Prick::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture(0, m_pTexture);
	for (auto c : m_vecPrick)
	{
		D3DXMatrixTranslation(&matWorld, c.col * fSpacing, c.row * fSpacing, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
		m_pd3dDevice->SetFVF(TEX_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL Prick::AddPrick(int row, int col)
{
	if (col <= 0 || col >= iMapColEnd - 1)
		return FALSE;
	if (pMap->IsCollision(row, col))
		return FALSE;
	m_vecPrick.push_back({ row, col });
	return TRUE;
}

//
//Blood
//

Blood::Blood()
{
	SetRange(0.0f, 0.0f, fSpacing, fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM2), 64, 64);
	Init();
	AddTexRect(32, 32, 48, 48);
	AddTexRect(48, 32, 64, 48);
	AddTexRect(48, 48, 64, 64);

	m_pd3dDevice->CreateVertexBuffer(4 * m_TexRects.size() * sizeof(TEXVERTEX), 0, TEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	for (int i = 0; i < (int)m_TexRects.size(); i++)
	{
		float left = m_ObjRange.left;
		float top = m_ObjRange.top;
		float right = m_ObjRange.right;
		float bottom = m_ObjRange.bottom;
		RECTRANGE TexRc = m_TexRects[i];

		TEXVERTEX *pVertices;
		m_pVertexBuffer->Lock(4 * i * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
		pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
		pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
		pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
		pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
		m_pVertexBuffer->Unlock();
	}
}

Blood::~Blood()
{
	DXObject::~DXObject();
}

BOOL Blood::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pd3dDevice->SetTexture(0, m_pTexture);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
	m_pd3dDevice->SetFVF(TEX_FVF);

	for (auto data : m_vecBlood)
	{
		float x = data.col * fSpacing;
		float y = data.row * fSpacing;

		D3DXMatrixTranslation(&matWorld, x, y, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * data.iTex, 2);
	}

	m_pd3dDevice->SetTexture(0, NULL);
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

BOOL Blood::Add(int row, int col, int iType)
{
	if (iType == 0)
	{
		BloodData newBlood;
		newBlood.row = row;
		newBlood.col = col;
		newBlood.iTex = rand() % 2;
		m_vecBlood.push_back(newBlood);
	}
	else
	{
		BloodData newBlood;
		newBlood.row = row;
		newBlood.col = col;
		newBlood.iTex = 2;
		m_vecBlood.push_back(newBlood);
	}

	return TRUE;
}

//
//FLInfo
//

FLInfo::FLInfo()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_fScale = GetSettings().m_fScale;
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);

	m_pArrow = new DXObject();
	m_pArrow->SetRange(-fSpacing / 2, -fSpacing / 2, fSpacing / 2, fSpacing / 2);
	m_pArrow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM2), 64, 64);
	m_pArrow->Init();
	m_pArrow->AddTexRect(16, 32, 32, 48);
	m_pArrow->AddTexRect(16, 48, 32, 64);
	m_pArrow->AddTexRect(32, 48, 48, 64);
	m_pArrow->AddState(0, SINGLE, 0.0f, 0);
	m_pArrow->AddState(1, SINGLE, 0.0f, 1);
	m_pArrow->AddState(2, SINGLE, 0.0f, 2);
	m_pArrow->AddState(3, SINGLE, 0.0f, 1);
	m_pArrow->SetState(3);
	m_pArrow->SetPos(0.0f, 0.0f);
	m_fOffsetArrow = 0.0f;
}

FLInfo::~FLInfo()
{
	SAFE_DELETE(m_pArrow);
	SAFE_RELEASE(m_pFont);
}

void FLInfo::OnResetDevice()
{
	SAFE_RELEASE(m_pFont);
	m_pd3dDevice = DXUTGetD3DDevice();
	m_fScale = GetSettings().m_fScale;
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);
}

void FLInfo::OnLostDevice()
{
	SAFE_RELEASE(m_pFont);
}

BOOL FLInfo::Update(double fTime, float fElapsedTime)
{
	auto iter = m_vecText.begin();
	while (iter != m_vecText.end())
	{
		iter->fTime += fElapsedTime;
		if (iter->fTime >= 2 * fSPB)
			iter = m_vecText.erase(iter);
		else
			iter++;
	}

	//Arrow

	if (!bStageChange && !bInterStage)
	{
		m_fOffsetArrow -= fElapsedTime * 0.05f;
		if (m_fOffsetArrow < 0.0f)
			m_fOffsetArrow = (float)fBaseSPB * 0.05f;

		float x = (iMapColEnd + 0.5f) * fSpacing;
		float y = (HALFSIZE + 0.5f) * fSpacing;
		int iState = 3;

		if (x > vCameraPos.x + fRangeX / 2 - 0.5f * fSpacing)
		{
			x = vCameraPos.x + fRangeX / 2 - 0.5f * fSpacing;
			iState = 2;

			if (y > vCameraPos.y + fRangeY / 2)
			{
				y = vCameraPos.y + fRangeY / 2 - 0.5f * fSpacing;
				iState = 1;
			}

			if (y < vCameraPos.y - fRangeY / 2)
			{
				y = vCameraPos.y - fRangeY / 2 + 0.5f * fSpacing;
				iState = 0;
			}
		}
		else
		{
			if (y > vCameraPos.y + fRangeY / 2)
			{
				y = vCameraPos.y + fRangeY / 2 - 0.5f * fSpacing;
				iState = 1;
			}

			if (y < vCameraPos.y - fRangeY / 2 + fSpacing)
			{
				y = max(y + 0.5f * fSpacing, vCameraPos.y - fRangeY / 2 + 0.5f * fSpacing);
				iState = 0;
			}
		}

		m_pArrow->SetState(iState);

		switch (iState)
		{
		case 0:
			m_pArrow->SetPos(x, y + m_fOffsetArrow);
			break;
		case 1:
			m_pArrow->SetPos(x, y - m_fOffsetArrow);
			break;
		case 2:
			m_pArrow->SetPos(x - m_fOffsetArrow, y);
			break;
		case 3:
			m_pArrow->SetPos(x, y - 0.5f * fSpacing - m_fOffsetArrow);
			break;
		}
	}

	return TRUE;
}

BOOL FLInfo::Render(double fTime, float fElapsedTime)
{
	RECT rcJudge = { 0,0,0,0 };
	DWORD colorJudge = D3DCOLOR_ARGB(255, 255, 255, 255);
	for (auto data : m_vecText)
	{
		int iAlpha = 255 - (int)(255.0f * data.fTime / (2 * fSPB));

		switch (data.iType)
		{
		case ADDTIME:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 255, 255);
			break;
		}

		rcJudge.left = (int)((data.x - 100.0f) * m_fScale);
		rcJudge.right = (int)((data.x + 100.0f) * m_fScale);
		rcJudge.top = (int)((data.y - 20.0f - data.fTime * 0.1f) * m_fScale);
		rcJudge.bottom = (int)((data.y + 20.0f - data.fTime * 0.1f) * m_fScale);

		m_pFont->DrawText(NULL, data.szText, -1, &rcJudge, DT_CENTER | DT_VCENTER, colorJudge);
	}

	if (!bStageChange && !bInterStage)
		m_pArrow->Render(fTime, fElapsedTime);

	return TRUE;
}

void FLInfo::Add(int iType, float x, float y)
{
	TextInfo newText;
	newText = { iType, x, y, 0.0f, L"" };

	switch (iType)
	{
	case ADDTIME:
		swprintf_s(newText.szText, L"+ %ds", (int)timeItem / 1000);
		break;
	}
	m_vecText.push_back(newText);
}

//
//HUD
//

HUD::HUD()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_fScale = GetSettings().m_fScale;
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_pCNFont);
	D3DXCreateFont(m_pd3dDevice, (int)(128.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Courier"), &m_pLFont);
	D3DXCreateFont(m_pd3dDevice, (int)(128.0f * m_fScale), 0, 0, 1000, TRUE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Courier"), &m_pLIFont);

	float fActSpacing = fSpacing * m_fScale;
	m_pHopeFragment = new DXRHWObject();
	m_pHopeFragment->SetRange(-fActSpacing / 2, -fActSpacing, fActSpacing / 2, fActSpacing);
	m_pHopeFragment->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	m_pHopeFragment->Init();
	m_pHopeFragment->AddTexRect( 0, 0, 16, 32);
	m_pHopeFragment->AddTexRect(16, 0, 32, 32);
	m_pHopeFragment->AddTexRect(32, 0, 48, 32);
	m_pHopeFragment->AddTexRect(48, 0, 64, 32);
	m_pHopeFragment->AddState(0, LOOP, (float)fBaseSPB / 4, 0, 1, 2, 3);
	m_pHopeFragment->SetState(0);
	m_pHopeFragment->SetPos(fActSpacing, fActSpacing);

	m_pGoldFragment = new DXRHWObject();
	m_pGoldFragment->SetRange(-fActSpacing / 2, -fActSpacing, fActSpacing / 2, fActSpacing);
	m_pGoldFragment->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	m_pGoldFragment->Init();
	m_pGoldFragment->AddTexRect( 0, 32, 16, 64);
	m_pGoldFragment->AddTexRect(16, 32, 32, 64);
	m_pGoldFragment->AddTexRect(32, 32, 48, 64);
	m_pGoldFragment->AddTexRect(48, 32, 64, 64);
	m_pGoldFragment->AddState(0, LOOP, (float)fBaseSPB / 4, 0, 1, 2, 3);
	m_pGoldFragment->SetState(0);
	m_pGoldFragment->SetPos(4.0f * fActSpacing, fActSpacing);

	m_pConnection = new DXRHWObject();
	m_pConnection->SetRange(-fActSpacing / 2, -fActSpacing, fActSpacing / 2, fActSpacing);
	m_pConnection->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	m_pConnection->Init();
	m_pConnection->AddTexRect( 64,  0,  80, 32);
	m_pConnection->AddTexRect( 80,  0,  96, 32);
	m_pConnection->AddTexRect( 96,  0, 112, 32);
	m_pConnection->AddTexRect(112,  0, 128, 32);
	m_pConnection->AddTexRect( 64, 32,  80, 64);
	m_pConnection->AddTexRect( 80, 32,  96, 64);
	m_pConnection->AddTexRect( 96, 32, 112, 64);
	m_pConnection->AddTexRect(112, 32, 128, 64);
	m_pConnection->AddState(0, LOOP, (float)fBaseSPB / 4, 0, 1, 2, 3, 4, 5, 6, 7);
	m_pConnection->SetState(0);
	m_pConnection->SetPos(7.0f * fActSpacing, fActSpacing);

	m_pBox = new Box();
	m_pBox->Set(960, 512, 255, 240);

	for (int i = 0; i < 6; i++)
		m_bSoundPlay[i] = false;
}

HUD::~HUD()
{
	SAFE_DELETE(m_pHopeFragment);
	SAFE_DELETE(m_pGoldFragment);
	SAFE_DELETE(m_pConnection);
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pCNFont);
	SAFE_RELEASE(m_pLFont);
	SAFE_RELEASE(m_pLIFont);
}

void HUD::OnResetDevice()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pCNFont);
	SAFE_RELEASE(m_pLFont);
	SAFE_RELEASE(m_pLIFont);

	m_pd3dDevice = DXUTGetD3DDevice();
	m_fScale = GetSettings().m_fScale;
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &m_pCNFont);
	D3DXCreateFont(m_pd3dDevice, (int)(128.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Courier"), &m_pLFont);
	D3DXCreateFont(m_pd3dDevice, (int)(128.0f * m_fScale), 0, 0, 1000, TRUE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Courier"), &m_pLIFont);
}

void HUD::OnLostDevice()
{
	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pCNFont);
	SAFE_RELEASE(m_pLFont);
	SAFE_RELEASE(m_pLIFont);
}

BOOL HUD::Update(double fTime, float fElapsedTime)
{
	m_pHopeFragment->Update(fTime, fElapsedTime);
	m_pGoldFragment->Update(fTime, fElapsedTime);
	m_pConnection->Update(fTime, fElapsedTime);
	return TRUE;
}

BOOL HUD::Render(double fTime, float fElapsedTime)
{
	WCHAR szBuffer[MAX_PATH];
	RECT rcText;
	DWORD color;
	int left, top, right, bottom;

	if (!bCount)
	{
		m_pHopeFragment->Render(fTime, fElapsedTime);
		m_pGoldFragment->Render(fTime, fElapsedTime);
		m_pConnection->Render(fTime, fElapsedTime);

		left = (int)(1.5f * fSpacing * m_fScale);
		right = (int)(3.0f * fSpacing * m_fScale);
		top = (int)(0.5f * fSpacing * m_fScale);
		bottom = (int)(1.5f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"X %3d", iHopeFragment);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, WHITE);

		left = (int)(4.5f * fSpacing * m_fScale);
		right = (int)(6.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"X %3d", iGoldFragment);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, WHITE);

		left = (int)(7.5f * fSpacing * m_fScale);
		right = (int)(9.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"X %3d", iPlayerLP);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, WHITE);

		int iMin = (int)timeTimer / 60000;
		int iSec = (int)timeTimer / 1000 - iMin * 60;
		int iMil = (int)timeTimer % 1000 / 10;

		left = (int)(0.5f * fSpacing * m_fScale);
		right = (int)(3.5f * fSpacing * m_fScale);
		top = (int)(15.0f * fSpacing * m_fScale);
		bottom = (int)(18.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"Time: \n%d'%02d\"%02d", iMin, iSec, iMil);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);

		if (!bInterStage && !bStageChange)
		{
			left = (int)(0.5f * fSpacing * m_fScale);
			right = (int)(3.5f * fSpacing * m_fScale);
			top = (int)(13.5f * fSpacing * m_fScale);
			bottom = (int)(15.0f * fSpacing * m_fScale);
			SetRect(&rcText, left, top, right, bottom);
			swprintf_s(szBuffer, L"Stage \n%d / %d", iStage, cStage);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);
		}

		if (bInterStage && !bStageChange)
		{
			if (timeInter >= fBaseSPB)
			{
				left = (int)(0.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				top = (int)(2.5f * fSpacing * m_fScale);
				bottom = (int)(3.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Stage %d Clear", iStage - 1);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (timeInter >= 2 * fBaseSPB)
			{
				int iSec = (int)timeCost / 1000;
				int iMil = (int)timeCost % 1000 / 10;

				left = (int)(0.0f * fSpacing * m_fScale);
				right = (int)(10.0f * fSpacing * m_fScale);
				top = (int)(4.5f * fSpacing * m_fScale);
				bottom = (int)(5.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Cost Time: %d\"%02d", iSec, iMil);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (timeInter >= 3 * fBaseSPB)
			{
				left = (int)(10.0f * fSpacing * m_fScale);
				right = (int)(20.0f * fSpacing * m_fScale);
				top = (int)(4.5f * fSpacing * m_fScale);
				bottom = (int)(5.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Damage Counts: %d", iDamage);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (timeInter >= 4 * fBaseSPB)
			{
				left = (int)(20.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				top = (int)(4.5f * fSpacing * m_fScale);
				bottom = (int)(5.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Fragment: %d", iFragment);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (iStage <= cStage && timeInter >= 5 * fBaseSPB)
			{
				if (timeInter >= 6 * fBaseSPB)
				{
					if (timeBonus > 0.0f)
					{
						timeBonus -= fElapsedTime * 50;
						timeTimer += fElapsedTime * 50;
						if (timeBonus < 0.0f)
						{
							timeBonus = 0.0f;
							timeTimer += timeBonus;
						}
					}
				}

				int iSec = (int)timeBonus / 1000;
				int iMil = (int)timeBonus % 1000 / 10;

				left = (int)(0.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				top = (int)(10.5f * fSpacing * m_fScale);
				bottom = (int)(11.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Bonus Time: %d\"%02d", iSec, iMil);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}
		}

		left = (int)(0.0f * fSpacing * m_fScale);
		right = (int)(30.0f * fSpacing * m_fScale);
		top = (int)(15.5f * fSpacing * m_fScale);
		bottom = (int)(16.5f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);

		if (timeReRange > 0.0f)
		{
			swprintf_s(szBuffer, L"Camera Range: %d", iCameraRange);
			int iAlpha = (int)(timeReRange / 3000.0f * 255.0f);
			DWORD color = D3DCOLOR_ARGB(iAlpha, 255, 255, 255);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
		}

		if (timeReAlpha > 0.0f)
		{
			swprintf_s(szBuffer, L"Scanner Alpha: %d", iScanAlpha);
			int iAlpha = (int)(timeReAlpha / 3000.0f * 255.0f);
			DWORD color = D3DCOLOR_ARGB(iAlpha, 255, 255, 255);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
		}

		if (timeReAdjust > 0.0f)
		{
			swprintf_s(szBuffer, L"Beat Adjust: %d", iAdjust);
			int iAlpha = (int)(timeReAdjust / 3000.0f * 255.0f);
			DWORD color = D3DCOLOR_ARGB(iAlpha, 255, 255, 255);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
		}

		if (bComplete)
		{
			int iAlpha = min(127, (int)(127.0f * timeComplete / 3000.0f));
			RenderAlphaCurtain(iAlpha);
			if (timeComplete > 2000.0f)
			{
				if (!m_bSoundPlay[0])
				{
					pSE0->Play(3);
					m_bSoundPlay[0] = true;
				}
				if (timeComplete <= 2400.0f)
				{
					color = D3DCOLOR_ARGB(255, 255, 255, 127);
					left = (int)((timeComplete / 10.0f - 240.0f) * fSpacing * m_fScale);
					top = (int)(7.0f * fSpacing * m_fScale);
					right = (int)((timeComplete / 10.0f - 210.0f) * fSpacing * m_fScale);
					bottom = (int)(9.0f * fSpacing * m_fScale);
					SetRect(&rcText, left, top, right, bottom);
					swprintf_s(szBuffer, L"Mission Complete");
					m_pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
				}
				else
				{
					int iBlue = max(127, 255 - (int)(128.0f / 300.0f * abs(timeComplete - 2700.0)));
					color = D3DCOLOR_ARGB(255, 255, 255, iBlue);
					left = (int)(0.0f * fSpacing * m_fScale);
					top = (int)(7.0f * fSpacing * m_fScale);
					right = (int)(30.0f * fSpacing * m_fScale);
					bottom = (int)(9.0f * fSpacing * m_fScale);
					SetRect(&rcText, left, top, right, bottom);
					swprintf_s(szBuffer, L"Mission Complete");
					if (timeComplete < 2700.0f)
						m_pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
					else
						m_pLIFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
				}
			}
		}

		if (bGameOver && GAME_PAUSE != GetMode())
		{
			int iAlpha = min(127, (int)(127.0f * timeOver / 3000.0f));
			RenderAlphaCurtain(iAlpha);

			if (timeOver > 1000.0f)
			{
				if (!m_bSoundPlay[0])
				{
					pSE0->Play(4);
					m_bSoundPlay[0] = true;
				}
				int iOverAlpha = min(255, (int)(255.0f * (timeOver - 1000.0f) / 2000.0f));
				color = D3DCOLOR_ARGB(iOverAlpha, 255, 63, 31);
				left = (int)(0.0f * fSpacing * m_fScale);
				top = (int)(7.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				bottom = (int)(9.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Mission Failed");
				m_pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
			}

			if (timeOver > 2000.0f)
			{
				int iReasonAlpha = min(255, (int)(255.0f * (timeOver - 2000.0f) / 1100.0f));
				color = D3DCOLOR_ARGB(iReasonAlpha, 255, 63, 31);
				left = (int)(0.0f * fSpacing * m_fScale);
				top = (int)(9.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				bottom = (int)(10.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				if (iDeathReason == 0)
					swprintf_s(szBuffer, L"Time Over");
				else if (iDeathReason == 1)
					swprintf_s(szBuffer, L"Excessive Hemorrhage");
				else if (iDeathReason == 2)
					swprintf_s(szBuffer, L"Crashed to Death");
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
			}
		}
	}
	else
	{
		if (!IsCurtainOn())
		{
			RenderAlphaCurtain(127);
			m_pBox->Render(fElapsedTime);

			if (m_pBox->IsReady())
			{
				timeCount += fElapsedTime;

				if (timeCount >= 500.0f)
				{
					if (!m_bSoundPlay[1])
					{
						pSE1->Play(1);
						m_bSoundPlay[1] = true;
					}
					SetGridRect(&rcText, 11.5f, 4.5f, 18.5f, 6.0f, fActSpacing);
					swprintf_s(szBuffer, L"总得分");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
					swprintf_s(szBuffer, L"%d", iScore);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
				}

				if (timeCount >= 1000.0f)
				{
					if (!m_bSoundPlay[2])
					{
						pSE1->Play(1);
						m_bSoundPlay[2] = true;
					}
					int iMin = (int)timeCostTotal / 60000;
					int iSec = (int)timeCostTotal / 1000 - iMin * 60;
					int iMil = (int)timeCostTotal % 1000 / 10;

					SetGridRect(&rcText, 11.5f, 6.5f, 18.5f, 7.5f, fActSpacing);
					swprintf_s(szBuffer, L"总计时间");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
					swprintf_s(szBuffer, L"%d'%d\"%d", iMin, iSec, iMil);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
				}

				if (timeCount >= 1500.0f)
				{
					if (!m_bSoundPlay[3])
					{
						pSE1->Play(1);
						m_bSoundPlay[3] = true;
					}
					SetGridRect(&rcText, 11.5f, 8.0f, 18.5f, 9.0f, fActSpacing);
					swprintf_s(szBuffer, L"被击次数");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
					swprintf_s(szBuffer, L"%d", iDamageTotal);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
				}

				if (timeCount >= 2000.0f)
				{
					if (!m_bSoundPlay[4])
					{
						pSE1->Play(1);
						m_bSoundPlay[4] = true;
					}
					SetGridRect(&rcText, 11.5f, 9.5f, 18.5f, 10.5f, fActSpacing);
					swprintf_s(szBuffer, L"希望碎片");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
					IsDebugMode() ?
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iHopeFragment, 0) :
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iHopeFragment - iHopeFragment, iHopeFragment);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
				}

				if (timeCount >= 2500.0f)
				{
					if (!m_bSoundPlay[5])
					{
						pSE1->Play(1);
						m_bSoundPlay[5] = true;
					}
					SetGridRect(&rcText, 11.5f, 11.0f, 18.5f, 12.0f, fActSpacing);
					swprintf_s(szBuffer, L"金色碎片");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
					IsDebugMode() ?
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iGoldFragment, 0) :
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iGoldFragment - iGoldFragment, iGoldFragment);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
				}

				GetInput();

				static bool bUp = false;
				if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_X) || IsKeyDown(DIK_RETURN))
				{
					if (bUp)
					{
						if (timeCount < 4000.0f)
							timeCount = 4000.0f;
						if (timeCount > 4500.0f)
							m_pBox->Close();
					}
					bUp = false;
				}
				else
					bUp = true;
			}

			if (timeCount > 4500.0f && m_pBox->IsClosed())
			{
				if (!IsStoryMode())
					SetCurtain(127, 255, 1500.0f, OpenSelectMenu);
				else
				{
					GetSaveData().m_iStory = 3;
					SetScene(MIDSAVE);
					SetCurtain(127, 255, 1500.0f, GameStart);
				}
			}
		}
	}

	return TRUE;
}

//
//Scene
//

SceneSnake::SceneSnake()
{

}

SceneSnake::~SceneSnake()
{
	End();
}

void SceneSnake::Init()
{
	//
	//Player Character
	//

	iCharaID = IsStoryMode() ? IDC_SAIHARA : GetSaveData().m_iCharaID;
	iStr = GetCharaSkillLevel(iCharaID, 0);
	iAgi = GetCharaSkillLevel(iCharaID, 1);
	iEnd = GetCharaSkillLevel(iCharaID, 2);
	iInt = GetCharaSkillLevel(iCharaID, 3);
	iLuc = GetCharaSkillLevel(iCharaID, 4);

	iScanRange = 2 + iInt / 3;
	iMaxLP = 6 + 2 * iEnd;

	//
	//Parameters decided by Difficulty
	//

	int iDifficulty = GetDifficulty();

	vecMap.clear();
	switch (iDifficulty)
	{
	case 0:
		fRate = 2.0f;
		fFrqPunch = 0.5f;
		timeTotal = 40000.0f;
		timeItem = 5000.0f;
		vecMap.push_back({ 13,13,4,4,3,1,16 });
		vecMap.push_back({ 15,15,4,4,4,1,20 });
		vecMap.push_back({ 15,19,4,4,6,2,24 });
		cStage = 3;
		break;
	case 1:
		fRate = 1.0f;
		fFrqPunch = 0.5f;
		timeTotal = 30000.0f;
		timeItem = 4000.0f;
		vecMap.push_back({ 13,15,4,5,5,2,16 });
		vecMap.push_back({ 15,18,4,7,6,3,20 });
		vecMap.push_back({ 17,22,4,9,8,5,24 });
		vecMap.push_back({ 19,28,3,13,10,8,30 });
		cStage = 4;
		break;
	case 2:
		fRate = 1.0f;
		fFrqPunch = 1.0f;
		timeTotal = 30000.0f;
		timeItem = 4000.0f;
		vecMap.push_back({ 15,20,4,7,6,3,20 });
		vecMap.push_back({ 17,24,3,9,8,5,24 });
		vecMap.push_back({ 19,29,3,13,10,8,28 });
		vecMap.push_back({ 25,36,3,17,10,15,32 });
		cStage = 4;
		break;
	case 3:
		fRate = 0.5f;
		fFrqPunch = 1.0f;
		timeTotal = 40000.0f;
		timeItem = 5000.0f;
		if (IsDebugMode)
			vecMap.push_back({ 30,64,3,24,18,28,32 });
		else
		{
			vecMap.push_back({ 15,24,4,3,18,3,3 });
			vecMap.push_back({ 30,64,3,24,18,28,32 });
			vecMap.push_back({ 15,24,4,3,18,3,3 });
		}
		cStage = 3;
		break;
	}

	if (IsDebugMode())
	{
		cStage = 1;
		timeTotal = 120000.0f;
		timeItem = 10000.0f;
	}

	//
	//Setting Parameters
	//

	fScale = GetSettings().m_fScale;

	SetSnakeCameraRange(GetSettings().m_iSKRange);
	iScanAlpha = GetSettings().m_iSKScanAlpha;
	iAdjust = GetSettings().m_iSKAdjust;
	timeAdjust = (float)iAdjust;

	//
	//Calculated Parameters
	//

	fActSpacing = fSpacing * fScale;

	fBaseSPB = 60000.0 / fBPM;
	fSPB = fBaseSPB * fRate;;

	fPrickTime = -fBegTime;
	fPunchTime = (float)(4 * fSPB / fFrqPunch - fSPB - fBegTime - 10.0f * fBaseSPB);

	//
	//Timer & Counter
	//

	iPlayerLP = iMaxLP;
	iStage = 1;
	LoadMapInfo(iStage - 1);

	timeTimer = timeTotal;
	timeOver = 0.0f;
	timeComplete = 0.0f;
	timeCount = 0.0f;

	bStageChange = false;
	bInterStage = false;
	iDamage = 0;
	iFragment = 0;
	timeCost = 0.0f;

	bWarning = false;
	fWarning = 0.0f;

	//
	//Results
	//

	iDeathReason = 0;
	bGameOver = false;
	bComplete = false;
	bCount = false;

	iScore = 0;
	timeCostTotal = 0.0f;
	iDamageTotal = 0;
	iFragmentTotal = 0;
	iHopeFragment = 0;
	iGoldFragment = 0;

	//
	//Pointers
	//

	pBackground = new Background();

	pMap = new Map();

	pPlayer = new Player();
	pPlayer->SetGrid(HALFSIZE, 0);
	pScanner = new Scanner();

	pItem = new Item();
	pBlood = new Blood();
	for (int i = 0; i < cEnemy; i++)
		vecEnemy.push_back(new Enemy());

	pFLInfo = new FLInfo();
	pHUD = new HUD();

	pBGM = new Sound();
	pBGM->Register(ID_BGM_SNAKE, ID_SUBMIX_BACKGROUND);
	pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_KIBOU, L"MP3", 255);

	pSE0 = new Sound();
	pSE0->Register(ID_SE_SK0, ID_SUBMIX_SOUNDEFFECT);
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_ITEM, L"WAVE");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_PRICK, L"WAVE");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_DAMAGE, L"WAVE");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CLEAR, L"MP3");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_FAIL, L"MP3");

	pSE1 = new Sound();
	pSE1->Register(ID_SE_SK1, ID_SUBMIX_SOUNDEFFECT, TRUE);
	pSE1->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_STEP, L"WAVE");
	pSE1->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SCORE, L"WAVE");


	//
	//Camera
	//

	pPlayer->GetPos(&vPlayerPos);
	fRangeX = GAME_WIDTH * fCameraScale;
	fRangeY = GAME_HEIGHT * fCameraScale;
	vCameraPos.x = max(min(vPlayerPos.x, (iMapColEnd + cBoundCol) * fSpacing - fRangeX / 2), (iMapColBeg - cBoundCol) * fSpacing + fRangeX / 2);
	vCameraPos.y = vPlayerPos.y;

	//Story

	if (IsStoryMode())
	{
		bStoryOver = false;
		bSentOver = false;
		iSent = 0;
		iChar = 0;

		timeChar = 0.0f;
		timeTalk = 0.0f;
		timeStoryOver = 0.0f;

		fSPChar = 60.0f;
		pTalkBox = new Box();
		pTalkBox->Set(960, 868, 832, 84);

		pPrepare = new Sound();
		pPrepare->Register(ID_BGM_STORY, ID_SUBMIX_BACKGROUND);
		pPrepare->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_UNDERGROUND, L"MP3", 255);
		pPrepare->Play(0);

		D3DXCreateFont(DXUTGetD3DDevice(), (int)(44.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
		D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
		D3DXCreateFont(DXUTGetD3DDevice(), (int)(88.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);

		pSEText = new Sound();
		pSEText->Register(ID_SE_TEXT, ID_SUBMIX_SOUNDEFFECT);
		pSEText->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_TEXT, L"WAVE");
	}
	else
	{
		bStoryOver = true;
		pBGM->Play(0);
	}
}

void SceneSnake::End()
{
	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pPrepare);
	SAFE_DELETE(pSEText);
	SAFE_DELETE(pMonomi);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	vecMap.clear();

	SAFE_DELETE(pMap);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pScanner);
	SAFE_DELETE(pItem);
	SAFE_DELETE(pBlood);
	for (auto p : vecEnemy)
		SAFE_DELETE(p);

	vecEnemy.clear();
	SAFE_DELETE(pPunch);
	SAFE_DELETE(pPrick);
	SAFE_DELETE(pHUD);
	SAFE_DELETE(pFLInfo);
	SAFE_DELETE(pBGM);
	SAFE_DELETE(pSE0);
	SAFE_DELETE(pSE1);
}

void SceneSnake::Reset()
{
	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pPrepare);
	SAFE_DELETE(pSEText);
	SAFE_DELETE(pMonomi);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	SAFE_DELETE(pMap);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pScanner);
	SAFE_DELETE(pItem);
	SAFE_DELETE(pBlood);
	for (auto p : vecEnemy)
		SAFE_DELETE(p);

	vecEnemy.clear();
	SAFE_DELETE(pPunch);
	SAFE_DELETE(pPrick);
	SAFE_DELETE(pHUD);
	SAFE_DELETE(pFLInfo);

	pBGM->Stop();

	//
	//Calculated Parameters
	//

	fBaseSPB = 60000.0 / fBPM;
	fSPB = fBaseSPB * fRate;;

	fPrickTime = -fBegTime;
	fPunchTime = (float)(4 * fSPB / fFrqPunch - fSPB - fBegTime - 10.0f * fBaseSPB);

	//
	//Timer & Counter
	//

	iPlayerLP = iMaxLP;
	iStage = 1;
	LoadMapInfo(iStage - 1);

	timeTimer = timeTotal;
	timeOver = 0.0f;
	timeComplete = 0.0f;
	timeCount = 0.0f;

	bStageChange = false;
	bInterStage = false;
	iDamage = 0;
	iFragment = 0;
	timeCost = 0.0f;

	//
	//Results
	//

	iDeathReason = 0;
	bGameOver = false;
	bComplete = false;
	bCount = false;

	iScore = 0;
	timeCostTotal = 0.0f;
	iDamageTotal = 0;
	iFragmentTotal = 0;
	iHopeFragment = 0;
	iGoldFragment = 0;

	//
	//Pointers
	//

	pBackground = new Background();

	pMap = new Map();

	pPlayer = new Player();
	pPlayer->SetGrid(HALFSIZE, 0);
	pScanner = new Scanner();

	pItem = new Item();
	pBlood = new Blood();
	for (int i = 0; i < cEnemy; i++)
		vecEnemy.push_back(new Enemy());

	pFLInfo = new FLInfo();
	pHUD = new HUD();

	pBGM->Play(0);
}

void SceneSnake::OnResetDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	if (IsStoryMode() && !bStoryOver)
	{
		D3DXCreateFont(DXUTGetD3DDevice(), (int)(44.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pFont);
		D3DXCreateFont(DXUTGetD3DDevice(), (int)(32.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pSFont);
		D3DXCreateFont(DXUTGetD3DDevice(), (int)(88.0f * fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, PROOF_QUALITY, 0, TEXT("黑体"), &pLFont);
	}

	if (NULL != pFLInfo)
		pFLInfo->OnResetDevice();

	if (NULL != pHUD)
		pHUD->OnResetDevice();
}

void SceneSnake::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	if (NULL != pFLInfo)
		pFLInfo->OnLostDevice();

	if (NULL != pHUD)
		pHUD->OnLostDevice();
}

void SceneSnake::Update(double fTime, float fElapsedTime)
{
	pBackground->Update(fTime, fElapsedTime);

	GetInput();

	static bool bEscUp = false;
	
	if (!bStoryOver)
	{
		static bool bZUp = false;

		timeTalk += fElapsedTime;

		if (IsKeyDown(DIK_ESCAPE))
		{
			if (bEscUp)
			{
				pSEText->Play(0);
				iSent = cSent;
			}
			bEscUp = false;
		}
		else
			bEscUp = true;

		if (iSent == cSent)
		{
			if (pTalkBox->IsReady())
				pTalkBox->Close();

			if (pTalkBox->IsClosed())
			{
				bStoryOver = true;
			}
		}
		else
		{
			if (pTalkBox->IsReady())
			{
				if (bSentOver)
				{
					if (IsKeyDown(DIK_Z))
					{
						if (bZUp)
						{
							pSEText->Play(0);
							iSent++;
							iChar = 0;
							bSentOver = false;
						}
						bZUp = false;
					}
					else
						bZUp = true;
				}
				else
				{
					timeChar += fElapsedTime;

					if (timeChar >= 0.0f)
					{
						if (szStory[iSent][iChar] != NULL)
							iChar++;
						else
							bSentOver = true;

						timeChar -= fSPChar;
					}

					if (IsKeyDown(DIK_Z))
					{
						if (bZUp)
						{
							bSentOver = true;
							while (szStory[iSent][iChar++] != NULL);
						}
						bZUp = false;
					}
					else
						bZUp = true;
				}
			}
		}
		return;
	}

	if (NULL != pPrepare)
	{
		if (timeStoryOver < 1000.0f)
		{
			timeStoryOver += fElapsedTime;
			pPrepare->SetVol(1.0f - timeStoryOver / 1000.0f);
		}
		else
		{
			pPrepare->End();
			SAFE_DELETE(pPrepare);
			pBGM->Play(0);
		}
		return;
	}

	if (!bInterStage)
	{
		if (!bStageChange)
		{
			if (pPlayer->GetCol(false) >= iMapColEnd)
			{
				{
					int StageScore = iStage * 10000;
					StageScore += iFragment * 500;
					StageScore -= iDamage * 200;
					StageScore -= (int)timeCost / 100;
					if (StageScore < 0)
						StageScore = 0;

					iScore += StageScore;
					timeCostTotal += timeCost;
					iFragmentTotal += iFragment;
					iDamageTotal += iDamage;

					bStageChange = true;
					timeBonus = max(0.0f, (float)timeTotal + (iFragment - iDamage) * 1000.0f);
					iStage++;
				}
			}
		}
	}
	else
	{
		timeInter += fElapsedTime;
		if (!bStageChange)
		{
			if (iStage > cStage)
			{
				if (pPlayer->GetCol(false) >= -cMinCol / 2)
				{
					if (!bComplete)
					{
						bComplete = true;
						pPlayer->SetState(HAPPY);

						if (!IsDebugMode())
						{
							GetSaveData().m_iHopeFragment = min(999, GetSaveData().m_iHopeFragment + iHopeFragment);
							GetSaveData().m_iGoldFragment = min(999, GetSaveData().m_iGoldFragment + iGoldFragment);
							if (iDamageTotal == 0)
								GetSaveData().m_iNoMiss[1][GetDifficulty()] = 1;
							int iMin = (int)timeCostTotal / 60000;
							int iSec = (int)timeCostTotal / 1000 % 60;
							CheckSKScore(GetDifficulty(), iScore, iMin, iSec, iDamageTotal, iFragmentTotal);
							SaveDataUpdated();
						}
					}
				}
			}
			else
			{
				if (pPlayer->GetCol(false) >= 0)
				{
					bStageChange = true;
					iFragment = 0;
					iDamage = 0;
					timeCost = 0.0f;
				}
			}
		}
	}

	if (timeTimer > 0.0f && timeTimer < 10000.0f && !bInterStage && !bGameOver)
	{
		bWarning = true;
		fWarning += 127.0f * fElapsedTime / fBaseSPB;
		if (fWarning >= 254.0f)
			fWarning = 0.0f;
	}
	else
	{
		bWarning = false;
		fWarning = 0.0f;
	}

	if (timeTimer <= 0.0f && !bComplete)
	{
		timeTimer = 0.0f;
		if (!bGameOver)
		{
			bGameOver = true;
			iDeathReason = 0;
		}
	}
	else if (!bStageChange && !bInterStage && !bComplete && !bGameOver)
	{
		timeTimer -= fElapsedTime;
		timeCost += fElapsedTime;
	}

	if (!bStageChange)
	{
		pPlayer->Update(fTime, fElapsedTime);
		pScanner->Update(fTime, fElapsedTime);
		pItem->Update(fTime, fElapsedTime);
		for (auto p : vecEnemy)
			p->Update(fTime, fElapsedTime);
	}

	pFLInfo->Update(fTime, fElapsedTime);
	pHUD->Update(fTime, fElapsedTime);

	//Prick
	fPrickTime += fElapsedTime;
	if (fPrickTime >= timeAdjust)
	{
		SAFE_DELETE(pPrick);
		if (!bStageChange)
			pPrick = new Prick();
		fPrickTime -= 2.0f * (float)fSPB;
	}
	else if (NULL != pPrick)
		pPrick->Update(fTime, fElapsedTime);

	//Punch
	fPunchTime += fElapsedTime;
	if (fPunchTime >= timeAdjust)
	{
		SAFE_DELETE(pPunch);
		if (!bStageChange)
			pPunch = new Punch(cPunch);
		fPunchTime -= 4.0f * (float)fSPB / fFrqPunch;
		for (auto p : vecEnemy)
			p->Move();
	}
	else if (NULL != pPunch)
		pPunch->Update(fTime, fElapsedTime);

	//Monomi
	if (NULL != pMonomi)
		pMonomi->Update(fTime, fElapsedTime);

	//Updates realting to control
	
	//Alpha and Adjust

	static bool bHomeUp = false;
	static bool bEndUp = false;
	static bool bPGUPUp = false;
	static bool bPGDNUp = false;

	timeReAlpha = max(0.0f, timeReAlpha - fElapsedTime);
	timeReAdjust = max(0.0f, timeReAdjust - fElapsedTime);

	if (IsKeyDown(DIK_HOME))
	{
		if (timeReAlpha <= 2950.0f)
			bHomeUp = true;

		if ((bHomeUp))
		{
			GetSettings().m_iSKScanAlpha = min(255, GetSettings().m_iSKScanAlpha + 1);
			SetSnakeScanAlpha(GetSettings().m_iSKScanAlpha);
			SaveSettings();
			timeReAlpha = 3000.0f;
			timeReRange = 0.0f;
			timeReAdjust = 0.0f;
		}
		bHomeUp = false;
	}
	else
		bHomeUp = true;

	if (IsKeyDown(DIK_END))
	{
		if (timeReAlpha <= 2950.0f)
			bEndUp = true;

		if (bEndUp)
		{
			GetSettings().m_iSKScanAlpha = max(0, GetSettings().m_iSKScanAlpha - 1);
			SetSnakeScanAlpha(GetSettings().m_iSKScanAlpha);
			SaveSettings();
			timeReAlpha = 3000.0f;
			timeReRange = 0.0f;
			timeReAdjust = 0.0f;
		}
		bEndUp = false;
	}
	else
		bEndUp = true;

	if (IsKeyDown(DIK_PGUP))
	{
		if (timeReAdjust <= 2950.0f)
			bPGUPUp = true;

		if (bPGUPUp)
		{
			GetSettings().m_iSKAdjust = min(127, GetSettings().m_iSKAdjust + 1);
			SetSnakeAdjust(GetSettings().m_iSKAdjust);
			SaveSettings();
			timeReAdjust = 3000.0f;
			timeReRange = 0.0f;
			timeReAlpha = 0.0f;
		}
		bPGUPUp = false;
	}
	else
		bPGUPUp = true;

	if (IsKeyDown(DIK_PGDN))
	{
		if (timeReAdjust <= 2950.0f)
			bPGDNUp = true;

		if (bPGDNUp)
		{
			GetSettings().m_iSKAdjust = max(-127, GetSettings().m_iSKAdjust - 1);
			SetSnakeAdjust(GetSettings().m_iSKAdjust);
			SaveSettings();
			timeReAdjust = 3000.0f;
			timeReRange = 0.0f;
			timeReAlpha = 0.0f;
		}
		bPGDNUp = false;
	}
	else
		bPGDNUp = true;

	//Camera

	timeReRange = max(0.0f, timeReRange - fElapsedTime);

	if (bInterStage)
	{
		if (bStageChange)
		{
			float x = (iMapColBeg - cBoundCol + cMinCol / 2) * fSpacing;
			if (vCameraPos.x < x)
				vCameraPos.x += fElapsedTime * 1.2f;
			else
			{
				bStageChange = false;
				bInterStage = false;
			}
		}
	}
	else
	{
		if (!bStageChange)
		{
			iCameraRange = GetSettings().m_iSKRange;
			switch (iCameraRange)
			{
			case 1:
				if (fCameraScale > 0.25f)
					fCameraScale = max(0.25f, fCameraScale - 0.001f * fElapsedTime);
				else
				{
					if (IsKeyDown(DIK_DELETE))
					{
						GetSettings().m_iSKRange = 2;
						SaveSettings();
						timeReRange = 3000.0f;
						timeReAlpha = 0.0f;
						timeReAdjust = 0.0f;
					}
				}
				break;
			case 2:
				if (fCameraScale < 0.5f)
					fCameraScale = min(0.5f, fCameraScale + 0.001f * fElapsedTime);
				else if (fCameraScale > 0.5f)
					fCameraScale = max(0.5f, fCameraScale - 0.001f * fElapsedTime);
				else
				{
					if (IsKeyDown(DIK_INSERT))
					{
						GetSettings().m_iSKRange = 1;
						SaveSettings();
						timeReRange = 3000.0f;
						timeReAlpha = 0.0f;
						timeReAdjust = 0.0f;
					}
					if (IsKeyDown(DIK_DELETE))
					{
						GetSettings().m_iSKRange = 3;
						SaveSettings();
						timeReRange = 3000.0f;
						timeReAlpha = 0.0f;
						timeReAdjust = 0.0f;
					}
				}
				break;
			case 3:
				if (fCameraScale < 1.0f)
					fCameraScale = min(1.0f, fCameraScale + 0.001f * fElapsedTime);
				else
				{
					if (IsKeyDown(DIK_INSERT))
					{
						GetSettings().m_iSKRange = 2;
						SaveSettings();
						timeReRange = 3000.0f;
						timeReAlpha = 0.0f;
						timeReAdjust = 0.0f;
					}
				}
				break;
			}

			pPlayer->GetPos(&vPlayerPos);
			fRangeX = GAME_WIDTH * fCameraScale;
			fRangeY = GAME_HEIGHT * fCameraScale;
			vCameraPos.x = max(min(vPlayerPos.x, (iMapColEnd + cBoundCol) * fSpacing - fRangeX / 2), (iMapColBeg - cBoundCol) * fSpacing + fRangeX / 2);
			vCameraPos.y = vPlayerPos.y;
		}
		else
		{
			if (fCameraScale < 1.0f)
			{
				fCameraScale = min(1.0f, fCameraScale + 0.001f * fElapsedTime);

				fRangeX = GAME_WIDTH * fCameraScale;
				fRangeY = GAME_HEIGHT * fCameraScale;
				vCameraPos.x = max(min(vPlayerPos.x, (iMapColEnd + cBoundCol) * fSpacing - fRangeX / 2), (iMapColBeg - cBoundCol) * fSpacing + fRangeX / 2);
				vCameraPos.y = vPlayerPos.y;
			}
			else
			{
				float x = (iMapColEnd + cMinCol / 2) * fSpacing;

				if (vCameraPos.x < x)
					vCameraPos.x += fElapsedTime * 1.2f;
				else
				{
					bStageChange = false;
					bInterStage = true;
					timeInter = 0.0f;

					LoadMapInfo(min(iStage, cStage) - 1);
					SAFE_DELETE(pMap);
					pMap = new Map();
					SAFE_DELETE(pItem);
					pItem = new Item();
					SAFE_DELETE(pBlood);
					pBlood = new Blood();
					for (auto p : vecEnemy)
						SAFE_DELETE(p);
					vecEnemy.clear();
					for (int i = 0; i < cEnemy; i++)
						vecEnemy.push_back(new Enemy());
					pPlayer->SetGrid(HALFSIZE, -cMinCol + 1);
					vCameraPos.x = (-cMinCol / 2 + 1) * fSpacing;

					if (iStage > cStage && IsStoryMode())
					{
						pMonomi = new Monomi();
						pMonomi->SetPos((-cMinCol / 2 + 1.5f) * fSpacing, (HALFSIZE + 0.5f) * fSpacing);
					}
				}
			}
		}
	}

	//GamePause
	if (!bGameOver && !bComplete)
	{
		if (IsKeyDown(DIK_ESCAPE))
		{
			if (bEscUp)
				GamePause();
			bEscUp = false;
		}
		else
			bEscUp = true;
	}

	//GameOver
	if (bGameOver)
	{
		timeOver += fElapsedTime;
		float fVol = 1.0f - timeOver / 3000.0f;
		if (fVol >= 0.0f)
			pBGM->SetVol(fVol);
		else
			pBGM->End();

		static bool bUp = false;
		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_X) || IsKeyDown(DIK_RETURN))
		{
			if (bUp)
			{
				if (timeOver >= 3000.0f)
					GameOver();
			}
			bUp = false;
		}
		else
			bUp = true;
	}

	//Complete
	if (bComplete)
	{
		timeComplete += fElapsedTime;

		float fVol = 1.0f - timeComplete / 3000.0f;
		if (fVol >= 0.0f)
			pBGM->SetVol(fVol);
		else
			pBGM->End();

		static bool bUp = false;
		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_X) || IsKeyDown(DIK_RETURN))
		{
			if (bUp)
			{
				if (timeComplete >= 3000.0f)
					bCount = true;
			}
			bUp = false;
		}
		else
			bUp = true;
	}
}

void SceneSnake::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(vCameraPos.x, vCameraPos.y);
	SetView();
	SetProj(NULL, fRangeX, fRangeY);
	SetViewPort(0.0f, 0.0f, 0.0f, 0.0f);

	pBackground->Render(fTime, fElapsedTime);

	pMap->Render(fTime, fElapsedTime);

	if (!bStageChange && !bInterStage)
	{
		pScanner->Render(fTime, fElapsedTime);
		pBlood->Render(fTime, fElapsedTime);
	}

	for (auto p : vecEnemy)
	{
		if (p->IsCrashed())
			p->Render(fTime, fElapsedTime);
	}
	if (NULL != pPrick)
		pPrick->Render(fTime, fElapsedTime);

	for (int row = iMapRowBeg; row < iMapRowEnd; row++)
	{
		if (NULL != pItem)
			pItem->Render(row);

		for (auto p : vecEnemy)
		{
			if (!p->IsCrashed() && row == p->GetRow())
			p->Render(fTime, fElapsedTime);
		}

		if (row == pPlayer->GetRow())
			pPlayer->Render(fTime, fElapsedTime);

		if (NULL != pPunch)
			pPunch->Render(row);
	}

	if (NULL != pMonomi)
		pMonomi->Render(fTime, fElapsedTime);

	if (!bStoryOver)
	{
		pTalkBox->Render(fElapsedTime);

		if (pTalkBox->IsReady())
		{
			RECT rcTalk;
			SetGridRect(&rcTalk, 1.9f, 12.0f, 28.6f, 15.5f, 64.0f * fScale);
			DrawGridText(pFont, szStory[iSent], iChar, 38, 70.0f * fScale, &rcTalk, BLACK);
			if ((int)(0.1f * timeTalk / fSPChar) % 3 > 0)
			{
				if (bSentOver)
					pSFont->DrawTextW(NULL, L"按Z继续↓", -1, &rcTalk, DT_RIGHT | DT_BOTTOM, BLACK);
			}
			SetGridRect(&rcTalk, 2.0f, 0.5f, 29.5f, 2.0f, 64.0f * fScale);
			pSFont->DrawTextW(NULL, L"按ESC跳过说明", -1, &rcTalk, DT_RIGHT | DT_TOP, WHITE);
		}
		return;
	}

	pFLInfo->Render(fTime, fElapsedTime);
	pHUD->Render(fTime, fElapsedTime);

	if (bWarning)
		RenderAlphaCurtain(127 - (int)abs(fWarning - 127.0f));
}