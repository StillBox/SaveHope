#include "SceneUpstairs.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "SaveData.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace UpstairsClass;

namespace UpstairsData
{
	//
	//Const Parameters
	//

	static float fSpacing = 64.0f;
	const double fBPM = 128.01;

	//
	//Player Character
	//

	int		iCharaID;
	int		iStr;
	int		iAgi;
	int		iEnd;
	int		iInt;
	int		iLuc;

	float	timeReTurn;
	float	timeMaxDash;
	int		iMaxLP;
	int		cMaxJump;

	//
	//Setted / Edited
	//

	float	fScale;

	int		iCameraRange;
	int		cStage;
	int		cFloor;
	int		cLevelUp;
	float	timeBuff;

	//Map

	std::vector<StageInfo> vecStage;

	int cRow;
	int cCol;

	int iTrapBegFloor;
	int iEnemyBegFloor;
	int iFlyEnemyBegFloor;

	//
	//Calculated
	//

	float	fActSpacing;
	double	fBaseSPB;

	//
	//Timer and Counter
	//

	double	timeTimer;
	float	timeOver;
	float	timeComplete;
	float	timeChaEnd;
	float	timeCount;

	float	timeTurn;
	float	timeDash;

	float	timeInfTurn;
	float	timeInfDash;
	float	timeInfJump;
	float	timeSuper;

	int		iBuild;
	int		iJump;
	int		iPlayerLP;
	int		iPlayerBlock;

	int		iStage;
	int		cRollMap;
	int		iFloor;
	int		iLastFloor;
	
	float	timeReRange;

	//
	//Camera
	//

	D3DXVECTOR2 vPlayerPos;
	D3DXVECTOR2	vCameraPos;
	float	fCameraScale;
	float	fCameraAngle;
	float	fRangeX, fRangeY;
	float	fCameraRadius;

	//
	//Parameters for Game Results
	//

	int		iDeathReason;
	bool	bGameOver;
	bool	bComplete;
	bool	bChaEnd;
	bool	bCount;

	int		iScore;
	float	timeCostTotal;
	int		iDamageTotal;
	int		iFragmentTotal;

	int		iMaxFloor;
	int		iHopeFragment;
	int		iGoldFragment;

	//Stage

	bool	bStageChange;
	bool	bInterStage;
	float	timeInter;

	float	timeCost;
	int		iFragment;
	int		iDamage;

	//
	//Pointers to Objects & Sounds
	//

	Background	*pBackground;
	Map *pMap = NULL;
	Player *pPlayer = NULL;
	std::vector<Enemy*> vecEnemy;
	std::vector<FlyEnemy*> vecFlyEnemy;
	Prick *pPrick = NULL;
	Item *pItem = NULL;
	Fragment *pFragment = NULL;

	int cBlue = 0;
	int cGold = 0;
	HUD *pHUD = NULL;

	Sound *pBGM = NULL;
	Sound *pSE0 = NULL;
	Sound *pSE1 = NULL;
	Sound *pSE2 = NULL;

	//
	//Story
	//

	const int cSent = 11;
	LPCWSTR szStory[cSent] =
	{
		L"这里就是电力控制系统的虚拟控制塔了，控制核心在塔最顶层的房间里，需要向上攀登相当的距离才能到达。",
		L"根据我的检测，盾子似乎并不只是在这里布置了陷阱和守卫而已，还通过环境干扰使你自身的行动受到了限制。",
		L"从进入控制塔的瞬间，你就无法停止前进。\n如果不掌握好跳跃（Z键）或者转身（←→键）的时机，随时都可能受到黑白熊或者陷阱的伤害。",
		L"虽然无法停止前进，但我试图让你获得了冲刺（X键）的能力，应该多少能帮你更灵活地进行一些走位。",
		L"可惜由于干扰太重，转身和冲刺的能力都受到了限制，需要充能才能恢复使用次数或持续时间。",
		L"虽然这里的黑白熊无法被破坏，但如同大多数游戏一样，它们的头部是弱点，你可以踩到他们头上来获得额外的跳跃机会进行更高的跳跃。",
		L"似乎为了减少消耗，盾子在布置了陷阱的位置几乎没有进行太大的环境干扰，我会尽力在那些位置提供一些道具，获得道具后可以在一段时间内无限制地使用对应的能力。",
		L"兔美去救七海前也给我留下了一些更高权限的道具，这些道具则可以让你无限制地使用所有的运动能力。",
		L"此外，还有一种道具可以让你在当前所处的高度处（按↓键）设置一层木质平台，虽然它们很脆弱易坏，但应该足以帮助你解决一些险境了。",
		L"最后，还请注意不要掉落太大的高度，盾子似乎在你的下方设置了足够直接杀死你的大量长枪……就像对她的姐姐一样……还真是恶趣味呢。",
		L"这是最后的战斗了，就请相信着在另一处努力的兔美，相信着在「新世界程序」中努力的大家，为了不辜负努力到现在的七海，不断地向上攀登吧。"
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

	ID3DXFont *pFont = NULL;
	ID3DXFont *pSFont = NULL;
	ID3DXFont *pLFont = NULL;

	Sound *pPrepare = NULL;
	Sound *pSEText = NULL;
}

using namespace UpstairsData;

void SetUpstairsCameraRange(int iVal)
{
	iCameraRange = iVal;
	switch (iCameraRange)
	{
	case 1:
		fCameraScale = 1.0f;
		break;
	case 2:
		fCameraScale = 2.0f;
		break;
	}
}

void LoadStageInfo(int index)
{
	if (index == -1)
	{
		cRow = 16;
		cCol = 48;
	}
	else
	{
		cRow = 54;
		cCol = vecStage[index].cWidth;
		cFloor = vecStage[index].cFloor;

		iTrapBegFloor = vecStage[index].iTrapBegFloor;
		iEnemyBegFloor = vecStage[index].iEnemyBegFloor;
		iFlyEnemyBegFloor = vecStage[index].iFlyEnemyBegFloor;
	}
	
	cRollMap = 0;
}

void UpstairsClass::RollScene()
{
	int iRollFloor = cRollMap / 18;

	pPrick->Roll();
	pItem->Roll();
	pFragment->Roll();

	for (auto p : vecEnemy)
		p->Roll();
	for (auto p : vecFlyEnemy)
		p->Roll();

	if (cRollMap > cFloor * 18)
	{
		if (cRollMap == cFloor * 18 + 15)
			pMap->Roll(CEILING);
		else if (cRollMap == cFloor * 18 + 6)
			pMap->Roll(TARGET);
		else
			pMap->Roll();
	}
	else
	{
		if (cRollMap % 18 == 6)
		{
			pMap->Roll(NORMAL_DSTAIR);

			pFragment->Add(cRow + 2, rand() % (cCol - 10) + 5);
			if (!IsInfiniteMode())
			{
				if (cCol > 60)
					pFragment->Add(cRow + 2, rand() % (cCol - 10) + 5);
			}
			for (int i = 0; i < iRollFloor / (3 * cLevelUp); i++)
				pFragment->Add(cRow + 2, rand() % (cCol - 10) + 5);

			if (iRollFloor >= iFlyEnemyBegFloor)
			{
				int cEnemy = min((iRollFloor - iFlyEnemyBegFloor) / (2 * cLevelUp) + 1, cCol / 20 - 1);
				for (int i = 0; i < cEnemy; i++)
				{
					if (rand() % iRollFloor != 0)
					{
						bool bEnemy = true;
						int cTrial = 0;
						int colEnemy;

						while (bEnemy && cTrial < 32)
						{
							bEnemy = false;
							cTrial++;
							colEnemy = 4 + rand() % (cCol - 8);

							if (pMap->m_MapData[cRow - 1][colEnemy].bCollision)
								bEnemy = true;

							int cFloorWidth = 1;
							int iLeft = 1, iRight = 1;
							while (!pMap->m_MapData[cRow - 1][colEnemy - iLeft].bCollision)
							{
								iLeft++;
								cFloorWidth++;
							}
							while (!pMap->m_MapData[cRow - 1][colEnemy + iRight].bCollision)
							{
								iRight++;
								cFloorWidth++;
							}
							if (iLeft <=2 || iRight <= 2 || cFloorWidth <= 9)
								bEnemy = true;
						}

						if (!bEnemy)
						{
							FlyEnemy *pNewEnemy = new FlyEnemy();
							pNewEnemy->SetPos(colEnemy * fSpacing, -cRow * fSpacing);
							vecFlyEnemy.push_back(pNewEnemy);
						}
					}
				}
			}
		}
		else if (cRollMap % 18 == 12)
		{
			pMap->Roll(NORMAL_USTAIR);

			pFragment->Add(cRow + 2, rand() % (cCol - 10) + 5);
			if (!IsInfiniteMode())
			{
				if (cCol > 60)
					pFragment->Add(cRow + 2, rand() % (cCol - 10) + 5);
			}
			for (int i = 0; i < iRollFloor / (3 * cLevelUp); i++)
				pFragment->Add(cRow + 2, rand() % (cCol - 10) + 5);

			if (iRollFloor >= iFlyEnemyBegFloor)
			{
				int cEnemy = min((iRollFloor - iFlyEnemyBegFloor) / (2 * cLevelUp) + 1, cCol / 20 - 1);
				for (int i = 0; i < cEnemy; i++)
				{
					if (rand() % iRollFloor != 0)
					{
						bool bEnemy = true;
						int cTrial = 0;
						int colEnemy;

						while (bEnemy && cTrial < 32)
						{
							bEnemy = false;
							cTrial++;
							colEnemy = 4 + rand() % (cCol - 8);

							if (pMap->m_MapData[cRow - 1][colEnemy].bCollision)
								bEnemy = true;

							int cFloorWidth = 1;
							int iLeft = 1, iRight = 1;
							while (!pMap->m_MapData[cRow - 1][colEnemy - iLeft].bCollision)
							{
								iLeft++;
								cFloorWidth++;
							}
							while (!pMap->m_MapData[cRow - 1][colEnemy + iRight].bCollision)
							{
								iRight++;
								cFloorWidth++;
							}
							if (iLeft <= 2 || iRight <= 2 || cFloorWidth <= 9)
								bEnemy = true;
						}

						if (!bEnemy)
						{
							FlyEnemy *pNewEnemy = new FlyEnemy();
							pNewEnemy->SetPos(colEnemy * fSpacing, -cRow * fSpacing);
							vecFlyEnemy.push_back(pNewEnemy);
						}
					}
				}
			}
		}
		else if (cRollMap % 18 == 0)
		{
			pMap->Roll(NORMAL_FLOOR);

			if (iRollFloor >= iTrapBegFloor)
			{
				int cPrick = min((iRollFloor - iTrapBegFloor) / cLevelUp + 1, cCol / 20 - 1);
				for (int i = 0; i < cPrick; i++)
				{
					if (rand() % iRollFloor != 0)
					{
						bool bPrick = true;
						int cTrial = 0;
						int colLeft, colRight;

						while (bPrick && cTrial < 32)
						{
							bPrick = false;
							cTrial++;
							colLeft = 1 + rand() % (cCol - 2);
							colRight = colLeft + (3 + rand() % 4) * 2;

							if (colRight >= cCol - 1)
								bPrick = true;

							for (int col = colLeft; col < colRight; col++)
							{
								if (pMap->m_MapData[cRow - 1][col].bCollision == false)
									bPrick = true;

								for (auto data : pPrick->m_vecPrick)
									if (data.col == col && data.row == cRow)
										bPrick = true;

								if (bPrick) break;
							}
						}

						if (!bPrick)
						{
							for (int col = colLeft; col < colRight; col++)
							{
								pPrick->AddPrick(cRow, col);
							}

							if (rand() % 100 < 14 + iLuc * 2)
							{
								int iType = 0;
								int iRate[5] = { 10,10,10,10 + iLuc / 2,10 + iLuc };
								int iRnd = rand() % (iRate[0] + iRate[1] + iRate[2] + iRate[3] + iRate[4]);
								for (int j = 0; j < 5; j++)
								{
									if (0 > (iRnd -= iRate[j]))
									{
										iType = j;
										break;
									}
								}

								pItem->Add(iType, cRow + 2, (colLeft + colRight) / 2);
							}
						}
					}
				}
			}

			if (iRollFloor >= iEnemyBegFloor)
			{
				int cEnemy = min((iRollFloor - iEnemyBegFloor) * 2 / (3 * cLevelUp) + 1, cCol / 20 - 1);
				for (int i = 0; i < cEnemy; i++)
				{
					if (rand() % iRollFloor != 0)
					{
						bool bEnemy = true;
						int cTrial = 0;
						int colEnemy;

						while (bEnemy && cTrial < 32)
						{
							bEnemy = false;
							cTrial++;
							colEnemy = 4 + rand() % (cCol - 8);

							if (!pMap->m_MapData[cRow - 1][colEnemy].bCollision)
								bEnemy = true;

							int cFloorWidth = 1;
							int iLeft = 1, iRight = 1;
							while (pMap->m_MapData[cRow - 1][colEnemy - iLeft].bCollision)
							{
								iLeft++;
								cFloorWidth++;
							}
							while (pMap->m_MapData[cRow - 1][colEnemy + iRight].bCollision)
							{
								iRight++;
								cFloorWidth++;
							}
							if (cFloorWidth <= 7)
								bEnemy = true;
						}

						if (!bEnemy)
						{
							Enemy *pNewEnemy = new Enemy();
							pNewEnemy->SetPos(colEnemy * fSpacing, -cRow * fSpacing);
							vecEnemy.push_back(pNewEnemy);
						}
					}
				}
			}
		}
		else
			pMap->Roll();
	}

	pPlayer->Roll();
	pPlayer->GetPos(&vPlayerPos);
	vCameraPos.y += fSpacing;
	cRollMap++;
}

//////////////////////////////////////////
//Map
//////////////////////////////////////////

Map::Map()
{
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_MAP), 128, 128);

	AddTexRect( 64, 112,  72, 120);				 //  0  -- back
	AddTexRect( 72, 112,  80, 120);				 //  1  -- back
	AddTexRect( 80, 112,  88, 120);				 //  2  -- back
	AddTexRect( 88, 112,  96, 120);				 //  3  -- back
	AddTexRect( 64, 120,  72, 128);				 //  4  -- plat
	AddTexRect( 72, 120,  80, 128);				 //  5  -- plat
	AddTexRect( 80, 120,  88, 128);				 //  6  -- plat
	AddTexRect( 88, 120,  96, 128);				 //  7  -- plat

	AddTexRect( 96, 112, 104, 120);				 //  8  -- 8-1-1
	AddTexRect(104, 112, 112, 120);				 //  9  -- 8-1-2
	AddTexRect(112, 112, 120, 120);				 // 10  -- 8-1-3
	AddTexRect(120, 112, 128, 120);				 // 11  -- 8-1-4
	AddTexRect( 96, 120, 104, 128);				 // 12  -- 8-2-1
	AddTexRect(104, 120, 112, 128);				 // 13  -- 8-2-2
	AddTexRect(112, 120, 120, 128);				 // 14  -- 8-2-3
	AddTexRect(120, 120, 128, 128);				 // 15  -- 8-2-4

	AddTexRect(112,  96, 120, 104);				 // 16  -- bottom
	AddTexRect(120,  96, 128, 104);				 // 17  -- bottom
	AddTexRect(112, 104, 120, 112);				 // 18  -- bottom
	AddTexRect(120, 104, 128, 112);				 // 19  -- bottom

	if (bInterStage)
	{
		for (int i = 0; i < cRow; i++)
		{
			for (int j = 0; j < cCol; j++)
			{
				m_MapData[i][j].iTex = 0;
				m_MapData[i][j].bCollision = false;

				if (i == 5 && j %12 == 6)
					m_MapData[i][j].iTex = 3;
			}
		}

		for (int i = 0; i < cCol; i++)
		{
			m_MapData[0][i].iTex = rand() % 4 + 4;
			m_MapData[0][i].bCollision = true;
			m_MapData[cRow - 1][i].iTex = rand() % 4 + 4;
			m_MapData[cRow - 1][i].bCollision = true;
		}

		for (int i = 0; i < cRow; i++)
		{
			m_MapData[i][0].iTex = rand() % 4 + 4;
			m_MapData[i][0].bCollision = true;
			m_MapData[i][cCol - 1].iTex = rand() % 4 + 4;
			m_MapData[i][cCol - 1].bCollision = true;
		}

		for (int i = 1; i < 5; i++)
		{
			m_MapData[i][5].iTex = 12;
			m_MapData[i][6].iTex = -1;
			m_MapData[i][7].iTex = -1;
			m_MapData[i][8].iTex = -1;
			m_MapData[i][9].iTex = 14;

			m_MapData[i][cCol - 10].iTex = 12;
			m_MapData[i][cCol - 9].iTex = -1;
			m_MapData[i][cCol - 8].iTex = -1;
			m_MapData[i][cCol - 7].iTex = -1;
			m_MapData[i][cCol - 6].iTex = 14;
		}

		m_MapData[5][5].iTex = 8;
		m_MapData[5][6].iTex = 9;
		m_MapData[5][7].iTex = 9;
		m_MapData[5][8].iTex = 9;
		m_MapData[5][9].iTex = 10;
		m_MapData[5][cCol - 10].iTex = 8;
		m_MapData[5][cCol - 9].iTex = 9;
		m_MapData[5][cCol - 8].iTex = 9;
		m_MapData[5][cCol - 7].iTex = 9;
		m_MapData[5][cCol - 6].iTex = 10;
	}
	else
	{
		for (int i = 0; i < cRow; i++)
		{
			int cl = rand() % 10;
			for (int j = 0; j < cCol; j++)
			{
				int c = rand() % 1000;
				if (i % 18 == 3 && j % 15 == cl)
					m_MapData[i][j].iTex = 3;
				else
					m_MapData[i][j].iTex = (c < 995) ? 0 : (c < 997) ? 1 : 2;

				m_MapData[i][j].bCollision = false;
			}
		}

		for (int i = 0; i < cCol; i++)
		{
			m_MapData[0][i].iTex = rand() % 4 + 4;
			m_MapData[0][i].bCollision = true;
		}

		for (int i = 0; i < cRow; i++)
		{
			m_MapData[i][0].iTex = rand() % 4 + 4;
			m_MapData[i][0].bCollision = true;
			m_MapData[i][cCol - 1].iTex = rand() % 4 + 4;
			m_MapData[i][cCol - 1].bCollision = true;
		}

		for (int row = 12; row < cRow; row += 18)
		{
			for (int col = 1; col < cCol - 1; col++)
			{
				m_MapData[row + 6][col].iTex = rand() % 4 + 4;
				m_MapData[row + 6][col].bCollision = true;
			}

			int cStair = cCol / 40;
			int cPartCol = cCol / cStair;
			for (int i = 0; i < cStair; i++)
			{
				int beg = (row == 12 && i == 0) ? 32 :
					cPartCol * i + 6 + rand() % (cPartCol - 20);
				int end = beg + 8;
				int iDir = (row == 12 && i == 0) ? -1 : rand() % 2 * 2 - 1;

				for (int col = beg + 1; col < end - 1; col++)
				{
					m_MapData[row][col].iTex = rand() % 4 + 4;
					m_MapData[row][col].bCollision = true;
					m_MapData[row - 6][col + 7 * iDir].iTex = rand() % 4 + 4;
					m_MapData[row - 6][col + 7 * iDir].bCollision = true;
				}
				for (int col = beg; col < end; col++)
				{
					m_MapData[row + 6][col].iTex = 0;
					m_MapData[row + 6][col].bCollision = false;
				}
			}
		}

		//Door

		for (int i = 1; i < 5; i++)
		{
			m_MapData[i][5].iTex = 12;
			m_MapData[i][6].iTex = -1;
			m_MapData[i][7].iTex = -1;
			m_MapData[i][8].iTex = -1;
			m_MapData[i][9].iTex = 14;
		}

		m_MapData[5][5].iTex = 8;
		m_MapData[5][6].iTex = 9;
		m_MapData[5][7].iTex = 9;
		m_MapData[5][8].iTex = 9;
		m_MapData[5][9].iTex = 10;
		
		cRollMap = cRow;

		for (int floor = iEnemyBegFloor; floor < 3; floor++)
		{
			int cEnemy = min((floor - iEnemyBegFloor) * 2 / (3 * cLevelUp) + 1, cCol / 20 - 1);
			for (int i = 0; i < cEnemy; i++)
			{
				if (rand() % (floor + 3) != 0)
				{
					bool bEnemy = true;
					int cTrial = 0;
					int colEnemy;

					while (bEnemy && cTrial < 32)
					{
						bEnemy = false;
						cTrial++;
						colEnemy = 4 + rand() % (cCol - 8);

						if (!m_MapData[18 * floor][colEnemy].bCollision)
							bEnemy = true;

						int cFloorWidth = 1;
						int iLeft = 1, iRight = 1;
						while (m_MapData[18 * floor][colEnemy - iLeft].bCollision)
						{
							iLeft++;
							cFloorWidth++;
						}
						while (m_MapData[18 * floor][colEnemy + iRight].bCollision)
						{
							iRight++;
							cFloorWidth++;
						}
						if (cFloorWidth <= 7)
							bEnemy = true;
					}

					if (!bEnemy)
					{
						Enemy *pNewEnemy = new Enemy();
						pNewEnemy->SetPos(colEnemy * fSpacing, -(18 * floor + 1) * fSpacing);
						vecEnemy.push_back(pNewEnemy);
					}
				}
			}
		}

		for (int floor = iFlyEnemyBegFloor; floor < 3; floor++)
		{
			int cEnemy = min((floor - iFlyEnemyBegFloor) / (2 * cLevelUp) + 1, cCol / 20 - 1);
			for (int i = 0; i < cEnemy; i++)
			{
				if (rand() % (floor + 3) != 0)
				{
					bool bEnemy = true;
					int cTrial = 0;
					int colEnemy;

					while (bEnemy && cTrial < 32)
					{
						bEnemy = false;
						cTrial++;
						colEnemy = 4 + rand() % (cCol - 8);

						if (m_MapData[18 * floor - 12][colEnemy].bCollision)
							bEnemy = true;

						int cFloorWidth = 1;
						int iLeft = 1, iRight = 1;
						while (!m_MapData[18 * floor - 12][colEnemy - iLeft].bCollision)
						{
							iLeft++;
							cFloorWidth++;
						}
						while (!m_MapData[18 * floor - 12][colEnemy + iRight].bCollision)
						{
							iRight++;
							cFloorWidth++;
						}
						if (iLeft <= 2 || iRight <= 2 || cFloorWidth <= 9)
							bEnemy = true;
					}

					if (!bEnemy)
					{
						FlyEnemy *pNewEnemy = new FlyEnemy();
						pNewEnemy->SetPos(colEnemy * fSpacing, -(18 * floor - 11) * fSpacing);
						vecFlyEnemy.push_back(pNewEnemy);
					}
				}

				if (rand() % (floor + 3) != 0)
				{
					bool bEnemy = true;
					int cTrial = 0;
					int colEnemy;

					while (bEnemy && cTrial < 32)
					{
						bEnemy = false;
						cTrial++;
						colEnemy = 4 + rand() % (cCol - 8);

						if (m_MapData[18 * floor - 6][colEnemy].bCollision)
							bEnemy = true;

						int cFloorWidth = 1;
						int iLeft = 1, iRight = 1;
						while (!m_MapData[18 * floor - 6][colEnemy - iLeft].bCollision)
						{
							iLeft++;
							cFloorWidth++;
						}
						while (!m_MapData[18 * floor - 6][colEnemy + iRight].bCollision)
						{
							iRight++;
							cFloorWidth++;
						}
						if (iLeft <= 2 || iRight <= 2 || cFloorWidth <= 9)
							bEnemy = true;
					}

					if (!bEnemy)
					{
						FlyEnemy *pNewEnemy = new FlyEnemy();
						pNewEnemy->SetPos(colEnemy * fSpacing, -(18 * floor - 5) * fSpacing);
						vecFlyEnemy.push_back(pNewEnemy);
					}
				}
			}
		}
	}

	SAFE_RELEASE(m_pVertexBuffer);
	m_pd3dDevice->CreateVertexBuffer(4 * cRow * cCol * sizeof(TEXVERTEX), 0, TEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	for (int i = 0; i < cRow; i++)
		for (int j = 0; j < cCol; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				float left = j * fSpacing;
				float right = (j + 1) * fSpacing;
				float top = -(i + 1) * fSpacing;
				float bottom = -i * fSpacing;
				RECTRANGE TexRc = m_TexRects[m_MapData[i][j].iTex];
				TEXVERTEX *pVertices;
				m_pVertexBuffer->Lock(4 * (i*cCol + j) * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
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
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXVECTOR3 vCameraPos;
	GetCameraPos(&vCameraPos);
	int iRowBeg = max(0, (int)(-vCameraPos.y / fSpacing) - 18);
	int iRowEnd = min(cRow, (int)(-vCameraPos.y / fSpacing) + 18);

	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);

	if (fCameraAngle != 0.0f)
	{
		D3DXMATRIX matAngle, matCamera, matReturn;
		D3DXMatrixRotationY(&matAngle, fCameraAngle);
		D3DXMatrixTranslation(&matCamera, -vCameraPos.x, 0.0f, 0.0f);
		D3DXMatrixTranslation(&matReturn, vCameraPos.x, 0.0f, 0.0f);
		matWorld = matCamera * matAngle * matReturn;
	}
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, m_pTexture);
	for (int i = iRowBeg; i < iRowEnd; i++)
		for (int j = 0; j < cCol; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
				m_pd3dDevice->SetFVF(TEX_FVF);
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (i*cCol + j), 2);
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
	if (row > cRow - 1 || col < 0 || col > cCol - 1)
		return TRUE;
	else if (row < 0)
		return FALSE;
	else
		return m_MapData[row][col].bCollision;
}

BOOL Map::Roll(int iType)
{
	//Roll Existing Rows

	for (int i = 0; i < cRow - 1; i++)
	{
		for (int j = 0; j < cCol; j++)
		{
			m_MapData[i][j] = m_MapData[i + 1][j];
		}
	}

	for (int i = 1; i < cCol - 1; i++)
	{
		m_MapData[0][i].iTex = 18 + rand() % 2;
		m_MapData[1][i].iTex = 16 + rand() % 2;
		m_MapData[0][i].bCollision = false;
		m_MapData[1][i].bCollision = false;
	}

	//Add New Row

	static int iColBeg[4];
	static int iColEnd[4];

	int cStair = cCol / 40;
	int cPartCol = cCol / cStair;

	m_MapData[cRow - 1][0].iTex = rand() % 4 + 4;
	m_MapData[cRow - 1][0].bCollision = true;
	m_MapData[cRow - 1][cCol - 1].iTex = rand() % 4 + 4;
	m_MapData[cRow - 1][cCol - 1].bCollision = true;

	switch (iType)
	{
	case NORMAL_SPACE:
	{
		int cl = rand() % 10;
		for (int i = 1; i < cCol - 1; i++)
		{
			int c = rand() % 1000;
			if (cRollMap % 18 == 3 && i % 15 == cl)
				m_MapData[cRow - 1][i].iTex = 3;
			else
				m_MapData[cRow - 1][i].iTex = (c < 995) ? 0 : (c < 997) ? 1 : 2;
			m_MapData[cRow - 1][i].bCollision = false;
		}

		if (cRollMap > cFloor * 18 + 6 && cRollMap < cFloor * 18 + 11)
		{
			m_MapData[cRow - 1][cCol - 10].iTex = 12;
			m_MapData[cRow - 1][cCol - 9].iTex = -1;
			m_MapData[cRow - 1][cCol - 8].iTex = -1;
			m_MapData[cRow - 1][cCol - 7].iTex = -1;
			m_MapData[cRow - 1][cCol - 6].iTex = 14;
		}

		if (cRollMap == cFloor * 18 + 11)
		{
			m_MapData[cRow - 1][cCol - 10].iTex = 8;
			m_MapData[cRow - 1][cCol - 9].iTex = 9;
			m_MapData[cRow - 1][cCol - 8].iTex = 9;
			m_MapData[cRow - 1][cCol - 7].iTex = 9;
			m_MapData[cRow - 1][cCol - 6].iTex = 10;
		}
	}	break;

	case NORMAL_DSTAIR:
	{
		for (int i = 0; i < cStair; i++)
		{
			iColBeg[i] = cPartCol * i + 6 + rand() % (cPartCol - 20);
			iColEnd[i] = iColBeg[i] + 6;
			int iDir = rand() % 2 * 2 - 1;

			for (int col = iColBeg[i]; col < iColEnd[i]; col++)
			{
				m_MapData[cRow - 1][col + 7 * iDir].iTex = rand() % 4 + 4;
				m_MapData[cRow - 1][col + 7 * iDir].bCollision = true;
			}
		}
	}	break;

	case NORMAL_USTAIR:
	{
		for (int i = 0; i < cStair; i++)
		{
			for (int col = iColBeg[i]; col < iColEnd[i]; col++)
			{
				m_MapData[cRow - 1][col].iTex = rand() % 4 + 4;
				m_MapData[cRow - 1][col].bCollision = true;
			}
		}
	}	break;

	case NORMAL_FLOOR:
	{
		for (int col = 0; col < cCol; col++)
		{
			m_MapData[cRow - 1][col].iTex = rand() % 4 + 4;
			m_MapData[cRow - 1][col].bCollision = true;
		}

		for (int col = 1; col < cCol - 1; col++)
		{
			if (m_MapData[cRow - 7][col].bCollision == true)
			{
				m_MapData[cRow - 1][col - 1].iTex = 0;
				m_MapData[cRow - 1][col - 1].bCollision = false;
				m_MapData[cRow - 1][col].iTex = 0;
				m_MapData[cRow - 1][col].bCollision = false;
				m_MapData[cRow - 1][col + 1].iTex = 0;
				m_MapData[cRow - 1][col + 1].bCollision = false;
			}
		}

		//Add Additional Holes
		
		int iRollFloor = cRollMap / 18;
		int cHole = min(iRollFloor / cLevelUp, cCol / 20 - cStair);

		for (int i = 0; i < cHole; i++)
		{
			int colBeg;
			int colEnd;
			bool bNewHole = true;
			while (bNewHole)
			{
				bNewHole = false;
				int cColHole = 5 + rand() % 6;
				colBeg = rand() % (cCol - cColHole - 2) + 1;
				colEnd = colBeg + cColHole;
				for (int col = max(1, colBeg - 2); col < min(cCol - 2, colEnd + 3); col++)
				{
					if (m_MapData[cRow - 1][col].bCollision == false)
					{
						bNewHole = true;
						break;
					}
				}
			}

			for (int col = colBeg; col < colEnd; col++)
			{
				m_MapData[cRow - 1][col].iTex = 0;
				m_MapData[cRow - 1][col].bCollision = false;
			}
		}

	}	break;

	case TARGET:
	{
		for (int i = cCol - 14; i < cCol - 1; i++)
		{
			m_MapData[cRow - 1][i].iTex = rand() % 4 + 4;
			m_MapData[cRow - 1][i].bCollision = true;
		}
	}	break;

	case CEILING:
	{
		for (int i = 1; i < cCol - 1; i++)
		{
			m_MapData[cRow - 1][i].iTex = rand() % 4 + 4;
			m_MapData[cRow - 1][i].bCollision = true;
		}
	}	break;
	}
	
	//Update Vertex Buffer

	for (int i = 0; i < cRow; i++)
		for (int j = 0; j < cCol; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				float left = j * fSpacing;
				float right = (j + 1) * fSpacing;
				float top = -(i + 1) * fSpacing;
				float bottom = -i * fSpacing;
				RECTRANGE TexRc = m_TexRects[m_MapData[i][j].iTex];
				TEXVERTEX *pVertices;
				m_pVertexBuffer->Lock(4 * (i*cCol + j) * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
				pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
				pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
				pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
				pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
				m_pVertexBuffer->Unlock();
			}
		}

	return TRUE;
}

BOOL Map::Build(int row)
{
	if (bComplete || bGameOver || bChaEnd)
		return FALSE;

	for (int i = 1; i < cCol - 1; i++)
	{
		if (m_MapData[row][i].bCollision == false || m_MapData[row][i].iTex == 15)
		{
			bool bBuild = true;
			
			for (auto p : vecEnemy)
			{
				if (p->IsIn(row, i))
				{
					bBuild = false;
					break;
				}
			}

			if (bBuild)
			{
				for (auto p : vecFlyEnemy)
				{
					if (p->IsIn(row, i))
					{
						bBuild = false;
						break;
					}
				}
			}

			if (bBuild)
			{
				m_MapData[row][i].iTex = 11;
				m_MapData[row][i].bCollision = true;
			}
		}
	}

	for (int i = 0; i < row + 1; i++)
		for (int j = 0; j < cCol; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				float left = j * fSpacing;
				float right = (j + 1) * fSpacing;
				float top = -(i + 1) * fSpacing;
				float bottom = -i * fSpacing;
				RECTRANGE TexRc = m_TexRects[m_MapData[i][j].iTex];
				TEXVERTEX *pVertices;
				m_pVertexBuffer->Lock(4 * (i*cCol + j) * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
				pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
				pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
				pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
				pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
				m_pVertexBuffer->Unlock();
			}
		}

	return TRUE;
}

BOOL Map::Break(int row, int col, int iDir)
{
	if (m_MapData[row][col].iTex == 11 && iDir == 0)
		m_MapData[row][col].iTex = 15;
	else if (m_MapData[row][col].iTex == 15 && iDir == 1)
	{
		m_MapData[row][col].iTex = 0;
		m_MapData[row][col].bCollision = false;
	}
	else
		return FALSE;

	for (int i = 0; i < row + 1; i++)
		for (int j = 0; j < cCol; j++)
		{
			if (m_MapData[i][j].iTex >= 0)
			{
				float left = j * fSpacing;
				float right = (j + 1) * fSpacing;
				float top = -(i + 1) * fSpacing;
				float bottom = -i * fSpacing;
				RECTRANGE TexRc = m_TexRects[m_MapData[i][j].iTex];
				TEXVERTEX *pVertices;
				m_pVertexBuffer->Lock(4 * (i*cCol + j) * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
				pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
				pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
				pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
				pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
				m_pVertexBuffer->Unlock();
			}
		}

	return TRUE;
}


//////////////////////////////////////////
//Player
//////////////////////////////////////////


Player::Player()
{
	SetRange(-128, -288, 128, 0);
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

	float fSPB = (float)fBaseSPB;
	float fSPS = (float)fBaseSPB / 4.0f; //time per step

	AddState(D_STAY, LOOP, fSPB, 1, 2, 1, 2, 8, 2, 1, 2);
	AddState(D_MOVE, LOOP, fSPS, 3, 1, 4, 1);
	AddState(U_STAY, LOOP, fSPB, 9, 10, 9, 10, 16, 10, 9, 10);
	AddState(U_MOVE, LOOP, fSPS, 11, 9, 12, 9);
	AddState(L_STAY, LOOP, fSPB, 17, 18, 17, 18, 24, 18, 17, 18);
	AddState(L_MOVE, LOOP, fSPS, 20, 19);
	AddState(R_STAY, LOOP, fSPB, 25, 26, 25, 26, 32, 26, 25, 26);
	AddState(R_MOVE, LOOP, fSPS, 28, 27);
	AddState(L_JUMP, SINGLE, 0.0f, 20);
	AddState(R_JUMP, SINGLE, 0.0f, 28);
	AddState(L_DASH, SINGLE, 0.0f, 20);
	AddState(R_DASH, SINGLE, 0.0f, 28);
	AddState(DEATH, LOOP, (float)fBaseSPB / 2.0f, 35, 36);
	AddState(HAPPY, LOOP, (float)fBaseSPB / 2.0f, 37, 38);

	m_Bound = RECTRANGE(-80.0f, -224.0f, 72.0f, 0.0f);
	SetPos(7.5f * fSpacing, -fSpacing); 

	m_fSpeed = 0.7f;
	m_fJumpSpeed = 2.8f;
	m_fGravity = 0.0088f;
	SetState(R_STAY, 0.0f, 0.0f);

	m_bTarget = false;
	m_bFall = false;
	m_timeDamage = 0.0f;

	m_pGhost = new DXObject();
	m_pGhost->SetRange(-128, -288, 128, 0);
	m_pGhost->AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(iCharaID), 256, 256);
	m_pGhost->Init();
	m_pGhost->AddTexRect(128,  72, 160, 108);				 // 0 -- L-5
	m_pGhost->AddTexRect(160,  72, 192, 108);				 // 1 -- L-6
	m_pGhost->AddTexRect(192,  72, 224, 108);				 // 2 -- L-7
	m_pGhost->AddTexRect(128, 108, 160, 144);				 // 3 -- R-5
	m_pGhost->AddTexRect(160, 108, 192, 144);				 // 4 -- R-6
	m_pGhost->AddTexRect(192, 108, 224, 144);				 // 5 -- R-7
	m_pGhost->AddState(0, SINGLE, 0.0f, 0);
	m_pGhost->AddState(1, SINGLE, 0.0f, 1);
	m_pGhost->AddState(2, SINGLE, 0.0f, 2);
	m_pGhost->AddState(3, SINGLE, 0.0f, 3);
	m_pGhost->AddState(4, SINGLE, 0.0f, 4);
	m_pGhost->AddState(5, SINGLE, 0.0f, 5);
}

Player::~Player()
{
	m_pGhost->~DXObject();
	SAFE_DELETE(m_pGhost);
	DXObject::~DXObject();
}

BOOL Player::Update(double fTime, float fElapsedTime)
{
	GetInput();

	static bool bSpaceUp = false;
	static bool bCrush = false;

	float xNew = m_ObjPos.x;
	float yNew = m_ObjPos.y;
	float vNew = m_v / m_fJumpSpeed;

	xNew += m_u * fElapsedTime;

	if (timeTurn < timeReTurn)
		timeTurn += fElapsedTime;
	if (timeTurn > timeReTurn)
		timeTurn = timeReTurn;

	if (L_DASH == m_iState || R_DASH == m_iState)
	{
		if (timeDash > 0.0f)
			timeDash -= fElapsedTime;
		if (timeDash < 0.0f)
			timeDash = 0.0f;
	}
	else
	{
		if (timeDash < timeMaxDash)
			timeDash += 0.5f * fElapsedTime;
		if (timeDash > timeMaxDash)
			timeDash = timeMaxDash;
	}

	switch (m_iState)
	{
	case L_MOVE:
		if (CheckLeft(&xNew, &yNew))
		{
			SetState(R_MOVE, 1.0f, 0.0f);
		}

		if (IsKeyDown(DIK_RIGHT) || IsKeyDown(DIK_D))
		{
			if (timeTurn >= 0.5f * timeReTurn)
			{
				SetState(R_MOVE, 1.0f, 0.0f);
				timeTurn -= 0.5f * timeReTurn;
			}
		}

		if (CheckLeftEnemy(&xNew, &yNew))
		{
			SetState(R_MOVE, 1.0f, 0.0f);
			Damage();
		}

		if (!CheckBottom(&xNew, &yNew))
		{
			SetState(L_JUMP, -1.0f, 0.0f);
			iJump++;
		}

		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_J))
		{
			bSpaceUp = false;
			SetState(L_JUMP, -1.0f, -1.0f);
			iJump++;
			pSE2->Play(0);
		}

		if (IsKeyDown(DIK_X) || IsKeyDown(DIK_K))
		{
			if (!bCrush)
			{
				SetState(L_DASH, -3.0f, 0.0f);
				pSE1->Play(0);
			}
		}
		else
			bCrush = false;

		break;

	case R_MOVE:
		if (CheckRight(&xNew, &yNew))
		{
			SetState(L_MOVE, -1.0f, 0.0f);
		}

		if (IsKeyDown(DIK_LEFT) || IsKeyDown(DIK_A))
		{
			if (timeTurn >= 0.5f * timeReTurn)
			{
				SetState(L_MOVE, -1.0f, 0.0f);
				timeTurn -= 0.5f * timeReTurn;
			}
		}

		if (CheckRightEnemy(&xNew, &yNew))
		{
			SetState(L_MOVE, -1.0f, 0.0f);
			Damage();
		}

		if (!CheckBottom(&xNew, &yNew))
		{
			SetState(R_JUMP, 1.0f, 0.0f);
			iJump++;
		}

		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_J))
		{
			bSpaceUp = false;
			SetState(R_JUMP, 1.0f, -1.0f);
			iJump++;
			pSE2->Play(0);
		}

		if (IsKeyDown(DIK_X) || IsKeyDown(DIK_K))
		{
			if (!bCrush)
			{
				SetState(R_DASH, 3.0f, 0.0f);
				pSE1->Play(0);
			}
		}
		else
			bCrush = false;

		break;

	case L_JUMP:
		if (CheckLeft(&xNew, &yNew))
		{
			SetState(R_JUMP, 1.0f, m_v / m_fJumpSpeed);
		}

		if (CheckLeftEnemy(&xNew, &yNew))
		{
			SetState(R_JUMP, 1.0f, m_v / m_fJumpSpeed);
			Damage();
		}

		if (IsKeyDown(DIK_X) || IsKeyDown(DIK_K))
		{
			if (!bCrush)
			{
				SetState(L_DASH, -3.0f, 0.0f);
				pSE1->Play(0);
			}
		}
		else
			bCrush = false;

		break;

	case R_JUMP:
		if (CheckRight(&xNew, &yNew))
		{
			SetState(L_JUMP, -1.0f, m_v / m_fJumpSpeed);
		}

		if (CheckRightEnemy(&xNew, &yNew))
		{
			SetState(L_JUMP, -1.0f, m_v / m_fJumpSpeed);
			Damage();
		}

		if (IsKeyDown(DIK_X) || IsKeyDown(DIK_K))
		{
			if (!bCrush)
			{
				SetState(R_DASH, 3.0f, 0.0f);
				pSE1->Play(0);
			}
		}
		else
			bCrush = false;

		break;
		
	case L_DASH:
		if (CheckLeft(&xNew, &yNew))
		{
			SetState(R_JUMP, 1.0f, 0.0f);
			bCrush = true;
		}

		if (CheckLeftEnemy(&xNew, &yNew))
		{
			SetState(L_JUMP, -1.0f, 0.0f);
			Damage();
		}

		if (m_timeDamage >= 0.9f * fBaseSPB)
		{
			SetState(L_JUMP, -1.0f, 0.0f);
			bCrush = true;
		}

		if (!IsKeyDown(DIK_X) && !IsKeyDown(DIK_K))
		{
			SetState(L_JUMP, -1.0f, 0.0f);
		}

		if (timeDash <= 0.0f)
		{
			SetState(L_JUMP, -1.0f, 0.0f);
			bCrush = true;
		}

		m_vDashEnd = m_ObjPos;

		break;

	case R_DASH:
		if (CheckRight(&xNew, &yNew))
		{
			SetState(L_JUMP, -1.0f, 0.0f);
			bCrush = true;
		}

		if (CheckRightEnemy(&xNew, &yNew))
		{
			SetState(L_JUMP, -1.0f, 0.0f);
			Damage();
		}

		if (m_timeDamage >= 0.9f * fBaseSPB)
		{
			SetState(R_JUMP, 1.0f, 0.0f);
			bCrush = true;
		}

		if (!IsKeyDown(DIK_X) && !IsKeyDown(DIK_K))
		{
			SetState(R_JUMP, 1.0f, 0.0f);
		}

		if (timeDash <= 0.0f)
		{
			SetState(R_JUMP, 1.0f, 0.0f);
			bCrush = true;
		}

		m_vDashEnd = m_ObjPos;

		break;	
	}

	yNew += m_v * fElapsedTime;
	if (L_JUMP == m_iState || R_JUMP == m_iState || DEATH == m_iState)
		m_v = min(m_fJumpSpeed, m_v + fElapsedTime * m_fGravity);

	switch (m_iState)
	{
	case L_JUMP:
		if (CheckTop(&xNew, &yNew))
		{
			m_v = 0.0f;
		}

		if (CheckTopEnemy(&xNew, &yNew))
		{
			m_v = 0.0f;
			Damage();
		}

		if (CheckBottom(&xNew, &yNew))
		{
			if (m_v > 0.0f)
			{
				SetState(L_MOVE, -1.0f, 0.0f);
				iJump = 0;
			}
		}

		if (CheckBottomEnemy(&xNew, &yNew, &vNew))
		{
			SetState(L_JUMP, -1.0f, vNew);
			iJump = 0;
			pSE2->Play(0);
		}

		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_J))
		{
			if (bSpaceUp && iJump < cMaxJump)
			{
				m_v = -m_fJumpSpeed;
				iJump++;
				pSE2->Play(0);
			}
			bSpaceUp = false;
		}
		else
			bSpaceUp = true;
		break;

	case R_JUMP:
		if (CheckTop(&xNew, &yNew))
		{
			m_v = 0.0f;
		}

		if (CheckTopEnemy(&xNew, &yNew))
		{
			m_v = 0.0f;
			Damage();
		}

		if (CheckBottom(&xNew, &yNew))
		{
			if (m_v > 0.0f)
			{
				SetState(R_MOVE, 1.0f, 0.0f);
				iJump = 0;
			}
		}

		if (CheckBottomEnemy(&xNew, &yNew, &vNew))
		{
			SetState(R_JUMP, 1.0f, vNew);
			iJump = 0;
			pSE2->Play(0);
		}

		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_J))
		{
			if (bSpaceUp && iJump < cMaxJump)
			{
				m_v = -m_fJumpSpeed;
				iJump++;
				pSE2->Play(0);
			}
			bSpaceUp = false;
		}
		else
			bSpaceUp = true;
		break;
	}

	m_ObjPos.x = xNew;
	m_ObjPos.y = yNew;

	//Target & Game Over

	if (bInterStage)
	{
		if (m_ObjPos.x >= (cCol - 7.6f) * fSpacing &&
			m_ObjPos.x <= (cCol - 7.4f) * fSpacing &&
			m_ObjPos.y > -3 * fSpacing &&
			m_ObjPos.y < -fSpacing)			
			SetState(U_MOVE, 0.0f, 0.5f);


		if (m_ObjPos.x >= (cCol - 7.6f) * fSpacing &&
			m_ObjPos.x <= (cCol - 7.4f) * fSpacing &&
			m_ObjPos.y >= -fSpacing &&
			m_ObjPos.y <= 0.0f)
		{
			if (!m_bTarget)
			{
				SetState(U_MOVE, 0.0f, 0.0f);
				m_bTarget = true;
			}
		}
	}
	else
	{
		if (cRollMap > cRow)
		{
			if (m_ObjPos.y >= -fSpacing)
			{
				if (!m_bFall && !bGameOver && !bChaEnd)
				{
					pSE0->Play(1);
					m_bFall = true;
					SetState(DEATH, 0.0f, -0.6f);
				}
			}
		}

		if (m_ObjPos.x >= (cCol - 7.6f) * fSpacing &&
			m_ObjPos.x <= (cCol - 7.4f) * fSpacing &&
			m_ObjPos.y > -(cFloor * 18 + 6 - cRollMap + cRow + 3) * fSpacing &&
			m_ObjPos.y < -(cFloor * 18 + 6 - cRollMap + cRow + 1) * fSpacing)
			SetState(U_MOVE, 0.0f, 0.5f);

		if (m_ObjPos.x >= (cCol - 7.6f) * fSpacing &&
			m_ObjPos.x <= (cCol - 7.4f) * fSpacing &&
			m_ObjPos.y >= -(cFloor * 18 + 6 - cRollMap + cRow + 1) * fSpacing &&
			m_ObjPos.y <= -(cFloor * 18 + 6 - cRollMap + cRow) * fSpacing)
		{
			if (!m_bTarget)
			{
				SetState(U_MOVE, 0.0f, 0.0f);
				m_bTarget = true;
			}
		}
	}

	int row = -(int)(m_ObjPos.y / fSpacing) - 1;

	//Updates Realating to Wood Builds

	static bool bDownUp = false;

	if (IsKeyDown(DIK_DOWN) || IsKeyDown(DIK_S))
	{
		if (bDownUp)
		{
			if (iBuild > 0)
			{
				pMap->Build(row);
				if (!IsDebugMode())
					iBuild--;
			}
		}
		bDownUp = false;
	}
	else
		bDownUp = true;

	if (m_iState == R_MOVE)
	{
		int iFwdCol = (int)((m_ObjPos.x + m_Bound.right) / fSpacing - 0.9f);
		int iAftCol = (int)((m_ObjPos.x + m_Bound.left) / fSpacing);

		for (int i = iFwdCol; i > iAftCol; i--)
		{
			bool bBreak = true;
			for (auto col : m_vecBreak)
			{
				if (i == col)
				{
					bBreak = false;
					break;
				}
			}

			if (bBreak)
			{
				bBreak = pMap->Break(row, i, 0);
			}

			if (bBreak)
				m_vecBreak.push_back(i);
		}

		bool bBreak = true;
		for (auto col : m_vecBreak)
		{
			if (iAftCol == col)
			{
				bBreak = false;
				break;
			}
		}

		if (bBreak)
			bBreak = pMap->Break(row, iAftCol, 1);
	}
	
	if (m_iState == L_MOVE)
	{
		int iFwdCol = (int)((m_ObjPos.x + m_Bound.left) / fSpacing + 0.9f);
		int iAftCol = (int)((m_ObjPos.x + m_Bound.right) / fSpacing);

		for (int i = iFwdCol; i < iAftCol; i++)
		{
			bool bBreak = true;
			for (auto col : m_vecBreak)
			{
				if (i == col)
				{
					bBreak = false;
					break;
				}
			}

			if (bBreak)
				bBreak = pMap->Break(row, i, 0);

			if (bBreak)
				m_vecBreak.push_back(i);
		}

		bool bBreak = true;
		for (auto col : m_vecBreak)
		{
			if (iAftCol == col)
			{
				bBreak = false;
				break;
			}
		}

		if (bBreak)
			bBreak = pMap->Break(row, iAftCol, 1);
	}

	m_timeDamage = max(0.0f, m_timeDamage - fElapsedTime);

	DXObject::Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL Player::Render(double fTime, float fElapsedTime)
{
	if (m_timeDamage > 0.0f && (int)(5 * m_timeDamage / fBaseSPB) % 2 == 0)
		return FALSE;

	if (L_DASH == m_iState)
	{
		float x[3];
		float y = m_vDashEnd.y;
		if (m_vDashStart.x - m_vDashEnd.x < 6 * fSpacing)
		{
			x[2] = m_vDashStart.x;
			x[1] = (2 * m_vDashStart.x + m_vDashEnd.x) / 3;
			x[0] = (m_vDashStart.x + 2 * m_vDashEnd.x) / 3;
		}
		else
		{
			x[2] = m_vDashEnd.x + 6 * fSpacing;
			x[1] = m_vDashEnd.x + 4 * fSpacing;
			x[0] = m_vDashEnd.x + 2 * fSpacing;
		}

		for (int i = 2; i >= 0; i--)
		{
			m_pGhost->SetPos(x[i], y);
			m_pGhost->SetState(i);
			m_pGhost->Render(fTime, fElapsedTime);
		}
	}

	if (R_DASH == m_iState)
	{
		float x[3];
		float y = m_vDashEnd.y;
		if (m_vDashEnd.x - m_vDashStart.x < 6 * fSpacing)
		{
			x[2] = m_vDashStart.x;
			x[1] = (2 * m_vDashStart.x + m_vDashEnd.x) / 3;
			x[0] = (m_vDashStart.x + 2 * m_vDashEnd.x) / 3;
		}
		else
		{
			x[2] = m_vDashEnd.x - 6 * fSpacing;
			x[1] = m_vDashEnd.x - 4 * fSpacing;
			x[0] = m_vDashEnd.x - 2 * fSpacing;
		}

		for (int i = 2; i >= 0; i--)
		{
			m_pGhost->SetPos(x[i], y);
			m_pGhost->SetState(3 + i);
			m_pGhost->Render(fTime, fElapsedTime);
		}
	}
	
	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL Player::CheckLeft(float *x, float *y)
{
	int iLeft = (int)((*x + m_Bound.left) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top + EPSILON) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom - EPSILON) / fSpacing);
	for (int i = iBottom; i <= iTop; i++)
	{
		if (pMap->IsCollision(i, iLeft))
		{
			*x = (iLeft + 1) * fSpacing - m_Bound.left;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Player::CheckRight(float *x, float *y)
{
	int iRight = (int)((*x + m_Bound.right) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top + EPSILON) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom - EPSILON) / fSpacing);
	for (int i = iBottom; i <= iTop; i++)
	{
		if (pMap->IsCollision(i, iRight))
		{
			*x = iRight * fSpacing - m_Bound.right - EPSILON;
			return TRUE;
		}
	}
	return FALSE;
}
BOOL Player::CheckTop(float *x, float *y)
{
	int iLeft = (int)((*x + m_Bound.left) / fSpacing);
	int iRight = (int)((*x + m_Bound.right) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top) / fSpacing);
	for (int i = iLeft; i <= iRight; i++)
	{
		if (pMap->IsCollision(iTop, i))
		{
			*y = -iTop * fSpacing - m_Bound.top + EPSILON;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Player::CheckBottom(float *x, float *y)
{
	int iLeft = (int)((*x + m_Bound.left) / fSpacing);
	int iRight = (int)((*x + m_Bound.right) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom + EPSILON) / fSpacing);
	for (int i = iLeft; i <= iRight; i++)
	{
		if (pMap->IsCollision(iBottom, i))
		{
			*y = -(iBottom + 1) * fSpacing - m_Bound.bottom;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Player::CheckLeftEnemy(float *x, float *y)
{
	for (auto p : vecEnemy)
	{
		if (*x > p->m_ObjPos.x &&
			*y + m_Bound.top < p->m_ObjPos.y + p->m_Bound.bottom &&
			*y + m_Bound.bottom > p->m_ObjPos.y + p->m_Bound.top)
		{
			float xCollision = p->m_ObjPos.x + p->m_Bound.right - m_Bound.left;
			if (*x <= xCollision)
			{
				p->SetState(L_MOVE, -1.0f);
				*x = xCollision;
				return TRUE;
			}
		}
	}

	for (auto p : vecFlyEnemy)
	{
		if (*x > p->m_ObjPos.x &&
			*y + m_Bound.top < p->m_ObjPos.y + p->m_Bound.bottom &&
			*y + m_Bound.bottom > p->m_ObjPos.y + p->m_Bound.top)
		{
			float xCollision = p->m_ObjPos.x + p->m_Bound.right - m_Bound.left;
			if (*x <= xCollision)
			{
				p->SetState(L_MOVE, -1.0f);
				*x = xCollision;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL Player::CheckRightEnemy(float *x, float *y)
{
	for (auto p : vecEnemy)
	{
		if (*x < p->m_ObjPos.x &&
			*y + m_Bound.top < p->m_ObjPos.y + p->m_Bound.bottom &&
			*y + m_Bound.bottom > p->m_ObjPos.y + p->m_Bound.top)
		{
			float xCollision = p->m_ObjPos.x + p->m_Bound.left - m_Bound.right;
			if (*x >= xCollision)
			{
				p->SetState(R_MOVE, 1.0f);
				*x = xCollision;
				return TRUE;
			}
		}
	}

	for (auto p : vecFlyEnemy)
	{
		if (*x < p->m_ObjPos.x &&
			*y + m_Bound.top < p->m_ObjPos.y + p->m_Bound.bottom &&
			*y + m_Bound.bottom > p->m_ObjPos.y + p->m_Bound.top)
		{
			float xCollision = p->m_ObjPos.x + p->m_Bound.left - m_Bound.right;
			if (*x >= xCollision)
			{
				p->SetState(R_MOVE, 1.0f);
				*x = xCollision;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL Player::CheckTopEnemy(float *x, float *y)
{
	for (auto p : vecEnemy)
	{
		if (*y > p->m_ObjPos.y + p->m_Bound.bottom  &&
			*x + m_Bound.right > p->m_ObjPos.x + p->m_Bound.left &&
			*x + m_Bound.left < p->m_ObjPos.x + p->m_Bound.right)
		{
			float yCollision = p->m_ObjPos.y + p->m_Bound.bottom - m_Bound.top;
			if (*y <= yCollision)
			{
				*y = yCollision + EPSILON;
				return TRUE;
			}
		}
	}

	for (auto p : vecFlyEnemy)
	{
		if (*y > p->m_ObjPos.y + p->m_Bound.bottom &&
			*x + m_Bound.right > p->m_ObjPos.x + p->m_Bound.left &&
			*x + m_Bound.left < p->m_ObjPos.x + p->m_Bound.right)
		{
			float yCollision = p->m_ObjPos.y + p->m_Bound.bottom - m_Bound.top;
			if (*y <= yCollision)
			{
				*y = yCollision + EPSILON;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL Player::CheckBottomEnemy(float *x, float *y, float*v)
{
	for (auto p : vecEnemy)
	{
		if (*y + m_Bound.top < p->m_ObjPos.y + p->m_Bound.top &&
			*x + m_Bound.right > p->m_ObjPos.x + p->m_Bound.left &&
			*x + m_Bound.left < p->m_ObjPos.x + p->m_Bound.right)
		{
			float yCollision = p->m_ObjPos.y + p->m_Bound.top - m_Bound.bottom;
			if (*y >= yCollision)
			{
				*y = yCollision;
				*v = -0.71f;
				p->Damage();
				return TRUE;
			}
		}
	}

	for (auto p : vecFlyEnemy)
	{
		if (*y + m_Bound.top < p->m_ObjPos.y + p->m_Bound.top &&
			*x + m_Bound.right > p->m_ObjPos.x + p->m_Bound.left &&
			*x + m_Bound.left < p->m_ObjPos.x + p->m_Bound.right)
		{
			float yCollision = p->m_ObjPos.y + p->m_Bound.top - m_Bound.bottom;
			if (*y >= yCollision)
			{
				*y = yCollision;
				*v = -1.0f;
				p->Damage();
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL Player::Roll()
{
	m_ObjPos.y += fSpacing;
	return TRUE;
}

BOOL Player::SetState(int iState, float u, float v)
{
	if (iState != m_iState)
	{
		DXObject::SetState(iState);
		m_vecBreak.clear();
		if (L_DASH == iState || R_DASH == iState)
		{
			m_vDashStart = m_ObjPos;
			m_vDashEnd = m_ObjPos;
		}
	}

	m_u = u * m_fSpeed;
	m_v = v * m_fJumpSpeed;

	return TRUE;
}

bool Player::Damage()
{
	if (m_timeDamage <= 0.0f)
	{
		pSE0->Play(1);
		if (IsDebugMode())
		{
			m_timeDamage = (float)fBaseSPB;
			return true;
		}
		else
		{
			if (iPlayerLP > 0)
			{
				m_timeDamage = (float)fBaseSPB;
				iPlayerLP--;
				iDamage++;
			}
			else
			{
				if (IsInfiniteMode())
				{
					if (!bChaEnd)
					{
						bChaEnd = true;
						SetState(DEATH, 0.0f, -0.6f);
						iDeathReason = 1;
					}
				}
				else
				{
					if (!bGameOver)
					{
						bGameOver = true;
						SetState(DEATH, 0.0f, -0.6f);
						iDeathReason = 1;
					}
				}
			}
			return true;
		}
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////
//Enemy
//////////////////////////////////////////

Enemy::Enemy()
{
	SetRange(-128, -288, 128, 0);
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(IDC_MONOKUMA), 256, 256);
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

	float fSPB = (float)fBaseSPB;
	float fSPS = (float)fBaseSPB / 4.0f; //time per step

	AddState(D_STAY, LOOP, fSPB, 1, 2, 1, 2, 8, 2, 1, 2);
	AddState(D_MOVE, LOOP, fSPS, 3, 1, 4, 1);
	AddState(U_STAY, LOOP, fSPB, 9, 10, 9, 10, 16, 10, 9, 10);
	AddState(U_MOVE, LOOP, fSPS, 11, 9, 12, 9);
	AddState(L_STAY, LOOP, fSPB, 17, 18, 17, 18, 24, 18, 17, 18);
	AddState(L_MOVE, LOOP, fSPS, 20, 19);
	AddState(R_STAY, LOOP, fSPB, 25, 26, 25, 26, 32, 26, 25, 26);
	AddState(R_MOVE, LOOP, fSPS, 28, 27);
	AddState(L_JUMP, SINGLE, 0.0f, 20);
	AddState(R_JUMP, SINGLE, 0.0f, 28);
	AddState(L_DASH, SINGLE, 0.0f, 20);
	AddState(R_DASH, SINGLE, 0.0f, 28);
	AddState(DEATH, LOOP, (float)fBaseSPB / 2.0f, 35, 36);
	AddState(HAPPY, LOOP, (float)fBaseSPB / 2.0f, 37, 38);

	m_Bound = RECTRANGE(-80.0f, -224.0f, 72.0f, 0.0f);
	SetPos(7.5f * fSpacing, -fSpacing);

	m_fSpeed = 0.4f;
	SetState(R_MOVE, 1.0f);
	m_timeDamage = 0.0f;
}

Enemy::~Enemy()
{
	DXObject::~DXObject();
}

BOOL Enemy::Update(double fTime, float fElapsedTime)
{
	float xNew = m_ObjPos.x;
	float yNew = m_ObjPos.y;

	xNew += m_u * fElapsedTime;

	switch (m_iState)
	{
	case L_MOVE:
		if (CheckLeft(&xNew, &yNew))
		{
			SetState(R_MOVE, 1.0f);
		}
		break;

	case R_MOVE:
		if (CheckRight(&xNew, &yNew))
		{
			SetState(L_MOVE, -1.0f);
		}
		break;
	}

	m_ObjPos.x = xNew;

	m_timeDamage = max(0.0f, m_timeDamage - fElapsedTime);

	DXObject::Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL Enemy::Render(double fTime, float fElapsedTime)
{
	if (m_timeDamage > 0.0f && (int)(5 * m_timeDamage / fBaseSPB) % 2 == 0)
		return FALSE;

	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL Enemy::CheckLeft(float *x, float *y)
{
	int iLeft = (int)((*x + m_Bound.left) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top + EPSILON) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom - EPSILON) / fSpacing);

	bool bCrush = false;

	for (int i = iBottom; i <= iTop; i++)
	{
		if (pMap->IsCollision(i, iLeft))
		{
			*x = (iLeft + 1) * fSpacing - m_Bound.left;
			if (!pMap->Break(i, iLeft, 0))
				pMap->Break(i, iLeft, 1);
			bCrush = true;
		}
	}

	for (auto p : vecEnemy)
	{
		if (p->m_ObjPos.y == m_ObjPos.y && p->m_ObjPos.x < m_ObjPos.x)
		{
			if (p->m_ObjPos.x + m_Bound.right >= m_ObjPos.x + m_Bound.left)
				bCrush = true;
		}
	}

	if (!pMap->IsCollision(iBottom - 1, iLeft))
		return TRUE;
	
	return bCrush;
}

BOOL Enemy::CheckRight(float *x, float *y)
{
	int iRight = (int)((*x + m_Bound.right) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top + EPSILON) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom - EPSILON) / fSpacing);

	bool bCrush = false;

	for (int i = iBottom; i <= iTop; i++)
	{
		if (pMap->IsCollision(i, iRight))
		{
			*x = iRight * fSpacing - m_Bound.right - EPSILON;
			if (!pMap->Break(i, iRight, 0))
				pMap->Break(i, iRight, 1);
			bCrush = true;
		}
	}

	for (auto p : vecEnemy)
	{
		if (p->m_ObjPos.y == m_ObjPos.y && p->m_ObjPos.x > m_ObjPos.x)
		{
			if (p->m_ObjPos.x + m_Bound.left <= m_ObjPos.x + m_Bound.right)
				bCrush = true;
		}
	}

	if (!pMap->IsCollision(iBottom - 1, iRight))
		return TRUE;

	return bCrush;
}

BOOL Enemy::Roll()
{
	m_ObjPos.y += fSpacing;

	return TRUE;
}

BOOL Enemy::SetState(int iState, float u)
{
	if (iState != m_iState)
	{
		DXObject::SetState(iState);
	}

	m_u = u * m_fSpeed;

	return TRUE;
}

bool Enemy::Damage()
{
	if (m_timeDamage <= 0.0f)
	{
		m_timeDamage = (float)fBaseSPB;
		return true;
	}
	else
		return false;
}

bool Enemy::IsDestroy()
{
	if (cRollMap > cRow && m_ObjPos.y >= -2 * fSpacing)
		return true;

	return false;
}

bool Enemy::IsIn(int row, int col)
{
	if (m_ObjPos.x + m_Bound.left < (col + 1) * fSpacing &&
		m_ObjPos.x + m_Bound.right > col * fSpacing &&
		m_ObjPos.y + m_Bound.top < -row * fSpacing &&
		m_ObjPos.y + m_Bound.bottom > -(row + 1) * fSpacing)
		return true;
	else
		return false;
}

//////////////////////////////////////////
//FlyEnemy
//////////////////////////////////////////

FlyEnemy::FlyEnemy()
{
	SetRange(-128, -32, 128, 96);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	Init();
	AddTexRect( 0,  96, 32, 112);				 // 0  -- R-1
	AddTexRect( 0, 112, 32, 128);				 // 1  -- R-2
	AddTexRect(32,  96, 64, 112);				 // 2  -- L-1
	AddTexRect(32, 112, 64, 128);				 // 3  -- L-2
	AddState(L_MOVE, LOOP, (float)fBaseSPB / 4.0f, 0, 1);
	AddState(R_MOVE, LOOP, (float)fBaseSPB / 4.0f, 2, 3);

	m_Bound = RECTRANGE(-120.0f, 0.0f, 120.0f, 64.0f);
	SetPos(7.5f * fSpacing, -fSpacing);

	m_fSpeed = 0.4f;
	SetState(R_MOVE, 1.0f);
	m_timeDamage = 0.0f;
}

FlyEnemy::~FlyEnemy()
{
	DXObject::~DXObject();
}

BOOL FlyEnemy::Update(double fTime, float fElapsedTime)
{
	float xNew = m_ObjPos.x;
	float yNew = m_ObjPos.y;

	xNew += m_u * fElapsedTime;

	switch (m_iState)
	{
	case L_MOVE:
		if (CheckLeft(&xNew, &yNew))
		{
			SetState(R_MOVE, 1.0f);
		}
		break;

	case R_MOVE:
		if (CheckRight(&xNew, &yNew))
		{
			SetState(L_MOVE, -1.0f);
		}
		break;
	}

	m_ObjPos.x = xNew;

	m_timeDamage = max(0.0f, m_timeDamage - fElapsedTime);

	DXObject::Update(fTime, fElapsedTime);

	return TRUE;
}

BOOL FlyEnemy::Render(double fTime, float fElapsedTime)
{
	if (m_timeDamage > 0.0f && (int)(5 * m_timeDamage / fBaseSPB) % 2 == 0)
		return FALSE;

	DXObject::Render(fTime, fElapsedTime);

	return TRUE;
}

BOOL FlyEnemy::CheckLeft(float *x, float *y)
{
	int iLeft = (int)((*x + m_Bound.left) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top + EPSILON) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom - EPSILON) / fSpacing);

	bool bCrush = false;

	for (int i = iBottom; i <= iTop; i++)
	{
		if (pMap->IsCollision(i, iLeft))
		{
			*x = (iLeft + 1) * fSpacing - m_Bound.left;
			if (!pMap->Break(i, iLeft, 0))
				pMap->Break(i, iLeft, 1);
			bCrush = true;
		}
	}

	for (auto p : vecFlyEnemy)
	{
		if (p->m_ObjPos.y == m_ObjPos.y && p->m_ObjPos.x < m_ObjPos.x)
		{
			if (p->m_ObjPos.x + m_Bound.right >= m_ObjPos.x + m_Bound.left)
				bCrush = true;
		}
	}

	return bCrush;
}

BOOL FlyEnemy::CheckRight(float *x, float *y)
{
	int iRight = (int)((*x + m_Bound.right) / fSpacing);
	int iTop = -(int)((*y + m_Bound.top + EPSILON) / fSpacing);
	int iBottom = -(int)((*y + m_Bound.bottom - EPSILON) / fSpacing);

	bool bCrush = false;

	for (int i = iBottom; i <= iTop; i++)
	{
		if (pMap->IsCollision(i, iRight))
		{
			*x = iRight * fSpacing - m_Bound.right - EPSILON;
			if (!pMap->Break(i, iRight, 0))
				pMap->Break(i, iRight, 1);
			bCrush = true;
		}
	}

	for (auto p : vecFlyEnemy)
	{
		if (p->m_ObjPos.y == m_ObjPos.y && p->m_ObjPos.x > m_ObjPos.x)
		{
			if (p->m_ObjPos.x + m_Bound.left <= m_ObjPos.x + m_Bound.right)
				bCrush = true;
		}
	}

	return bCrush;
}

BOOL FlyEnemy::Roll()
{
	m_ObjPos.y += fSpacing;

	return TRUE;
}

BOOL FlyEnemy::SetState(int iState, float u)
{
	if (iState != m_iState)
	{
		DXObject::SetState(iState);
	}

	m_u = u * m_fSpeed;

	return TRUE;
}

bool FlyEnemy::Damage()
{
	if (m_timeDamage <= 0.0f)
	{
		m_timeDamage = (float)fBaseSPB;
		return true;
	}
	else
		return false;
}

bool FlyEnemy::IsDestroy()
{
	if (cRollMap > cRow && m_ObjPos.y >= -2 * fSpacing)
		return true;

	return false;
}

bool FlyEnemy::IsIn(int row, int col)
{
	if (m_ObjPos.x + m_Bound.left < (col + 1) * fSpacing &&
		m_ObjPos.x + m_Bound.right > col * fSpacing &&
		m_ObjPos.y + m_Bound.top < -row * fSpacing &&
		m_ObjPos.y + m_Bound.bottom > -(row + 1) * fSpacing)
		return true;
	else
		return false;
}

//////////////////////////////////////////
//Prick
//////////////////////////////////////////

Prick::Prick()
{
	SetRange(0.0f, -2 * fSpacing, fSpacing, 0);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	Init();
	AddTexRect(64, 64, 72, 80);
	AddTexRect(72, 64, 80, 80);
	AddTexRect(80, 64, 88, 80);
	AddTexRect(88, 64, 96, 80);
	AddTexRect(64, 80, 72, 96);
	AddTexRect(72, 80, 80, 96);
	AddTexRect(80, 80, 88, 96);
	AddTexRect(88, 80, 96, 96);
	AddState(0, NOLOOP, (float)fBaseSPB * 2, 0);
	AddState(1, NOLOOP, (float)fBaseSPB * 0.2f, 1, 2, 3);
	AddState(2, NOLOOP, (float)fBaseSPB * 0.1f, 4, 5, 6, 7, 7, 6, 5, 4);
	AddState(3, NOLOOP, (float)fBaseSPB * 0.2f, 3, 2, 1);
	AddStateChain(0, 3, 1);
	AddStateChain(1, 0, 2);
	AddStateChain(2, 1, 3);
	AddStateChain(3, 2, 0);
	SetState(0);

	for (int floor = iTrapBegFloor; floor < 3; floor++)
	{
		int cPrick = min((floor - iTrapBegFloor) / cLevelUp + 1, cCol / 20 - 1);
		for (int i = 0; i < cPrick; i++)
		{
			if (rand() % (floor + 3) != 0)
			{
				bool bPrick = true;
				int cTrial = 0;
				int colLeft, colRight;

				while (bPrick && cTrial < 32)
				{
					bPrick = false;
					cTrial++;
					colLeft = 1 + rand() % (cCol - 2);
					colRight = colLeft + (3 + rand() % 4) * 2;

					if (colRight >= cCol - 1)
						bPrick = true;

					for (int col = colLeft; col < colRight; col++)
					{
						if (!pMap->IsCollision(18 * floor, col))
							bPrick = true;

						for (auto data : m_vecPrick)
							if (data.col == col && data.row == 18 * floor + 1)
								bPrick = true;

						if (bPrick) break;
					}
				}

				if (!bPrick)
				{
					for (int col = colLeft; col < colRight; col++)
					{
						AddPrick(18 * floor + 1, col);
					}

					if (rand() % 100 < 14 + iLuc * 2)
					{
						int iType = 0;
						int iRate[5] = { 10,10,10,10 + iLuc / 2,10 + iLuc };
						int iRnd = rand() % (iRate[0] + iRate[1] + iRate[2] + iRate[3] + iRate[4]);
						for (int j = 0; j < 5; j++)
						{
							if (0 >(iRnd -= iRate[j]))
							{
								iType = j;
								break;
							}
						}
						pItem->Add(iType, 18 * floor + 3, (colLeft + colRight) / 2);
					}
				}
			}
		}
	}
}

Prick::~Prick()
{
	DXObject::~DXObject();
}

BOOL Prick::Update(double fTime, float fElapsedTime)
{
	DXObject::Update(fTime, fElapsedTime);

	auto iter = m_vecPrick.begin();
	while (iter != m_vecPrick.end())
	{
		if (m_iState == 2)
		{
			if (vPlayerPos.x >= iter->col * fSpacing &&
				vPlayerPos.x <= (iter->col + 1) * fSpacing &&
				vPlayerPos.y >= -(iter->row + 1) * fSpacing &&
				vPlayerPos.y <= -iter->row * fSpacing &&
				!bGameOver && !bChaEnd)
				pPlayer->Damage();
		}

		if (iter->row <= 2)
			iter = m_vecPrick.erase(iter);
		else
			iter++;
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
		D3DXMatrixTranslation(&matWorld, c.col * fSpacing, -c.row * fSpacing, 0.0f);
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
	PrickData newPrick;
	newPrick.row = row;
	newPrick.col = col;
	m_vecPrick.push_back(newPrick);
	return TRUE;
}

BOOL Prick::Roll()
{
	auto iter = m_vecPrick.begin();
	while (iter != m_vecPrick.end())
	{
		if (iter->row <= 1)
		{
			iter = m_vecPrick.erase(iter);
		}
		else
		{
			iter->row--;
			iter++;
		}
	}

	return TRUE;
}

//////////////////////////////////////////
//Item
//////////////////////////////////////////

Item::Item()
{
	SetRange(-2 * fSpacing, -2 * fSpacing, 2 * fSpacing, 2 * fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	Init();
	AddTexRect( 0,  0,  32,  32); // 0 - Dash
	AddTexRect(32,  0,  64,  32); // 1 - Turn
	AddTexRect(64,  0,  96,  32); // 2 - DJump
	AddTexRect(96,  0, 128,  32); // 3 - Build
	AddTexRect(96, 64, 128,  96); // 4 - Usami
	AddTexRect(64, 96,  96, 128); // 5 - Light
	AddTexRect(96, 96, 128, 128); // 6 - Light

	m_pd3dDevice->CreateVertexBuffer(4 * m_TexRects.size() * sizeof(TEXVERTEX),
		0, TEX_FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

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
}

Item::~Item()
{
	DXObject::~DXObject();
}

BOOL Item::Update(double fTime, float fElapsedTime)
{
	auto iter = m_vecPoints.begin();
	while (iter != m_vecPoints.end())
	{
		if (vPlayerPos.x >= (iter->col - 2) * fSpacing &&
			vPlayerPos.x <= (iter->col + 2) * fSpacing &&
			vPlayerPos.y >= (-iter->row - 1) *fSpacing &&
			vPlayerPos.y <= (-iter->row + 2) * fSpacing &&
			!bGameOver && !bChaEnd)
		{
			pSE0->Play(0);

			switch (iter->iType)
			{
			case 0:
				timeInfDash = timeBuff;
				break;
			case 1:
				timeInfTurn = timeBuff;
				break;
			case 2:
				timeInfJump = timeBuff;
				break;
			case 3:
				iBuild++;
				break;
			case 4:
				timeInfDash = timeBuff;
				timeInfTurn = timeBuff;
				timeInfJump = timeBuff;
				timeSuper = timeBuff;
				break;
			}

			iter = m_vecPoints.erase(iter);
		}
		else if (iter->row <= 1)
			iter = m_vecPoints.erase(iter);
		else
			iter++;
	}

	m_UpdateTime += fElapsedTime;
	if (m_UpdateTime >= 4 * (float)fBaseSPB)
		m_UpdateTime -= 4 * (float)fBaseSPB;

	return TRUE;
}

BOOL Item::Render(double fTime, float fElapsedTime)
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

	for (auto c : m_vecPoints)
	{
		float fOffset = (float)(abs(m_UpdateTime - 2 * fBaseSPB) - fBaseSPB) * 0.02f;

		D3DXMatrixTranslation(&matWorld, c.col * fSpacing, -c.row * fSpacing + fOffset, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
		m_pd3dDevice->SetFVF(TEX_FVF);
		if ((int)(8 * m_UpdateTime / fBaseSPB) % 2 == 0)
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
		else
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 24, 2);

		D3DXMatrixTranslation(&matWorld, c.col * fSpacing, -c.row * fSpacing + 2 * fOffset, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
		m_pd3dDevice->SetFVF(TEX_FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * c.iType, 2);
	}

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

bool Item::Add(int iType, int row, int col)
{
	ItemData newItem;
	newItem.row = row;
	newItem.col = col;
	newItem.iType = iType;
	m_vecPoints.push_back(newItem);

	return true;
}

BOOL Item::Roll()
{
	for (auto &data : m_vecPoints)
		data.row--;

	return TRUE;
}

//////////////////////////////////////////
//Fragment
//////////////////////////////////////////

Fragment::Fragment()
{
	SetRange(-fSpacing, -2 * fSpacing, fSpacing, 2 * fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM0), 128, 128);
	Init();
	AddTexRect(0, 0, 16, 32);
	AddTexRect(16, 0, 32, 32);
	AddTexRect(32, 0, 48, 32);
	AddTexRect(48, 0, 64, 32);
	AddTexRect(0, 32, 16, 64);
	AddTexRect(16, 32, 32, 64);
	AddTexRect(32, 32, 48, 64);
	AddTexRect(48, 32, 64, 64);

	m_pd3dDevice->CreateVertexBuffer(4 * m_TexRects.size() * sizeof(TEXVERTEX),
		0, TEX_FVF, D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

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

	m_iTex = 0;
	m_UpdateTime = 0.0f;

	if (!bInterStage)
	{
		for (int floor = 6; floor < cRow; floor += 18)
		{
			Add(floor + 3, rand() % (cCol - 10) + 5);
		}
		for (int floor = 12; floor < cRow; floor += 18)
		{
			Add(floor + 3, rand() % (cCol - 10) + 5);
		}
	}
}

Fragment::~Fragment()
{
	DXObject::~DXObject();
}

BOOL Fragment::Update(double fTime, float fElapsedTime)
{
	auto iter = m_vecPoints.begin();
	while (iter != m_vecPoints.end())
	{
		if (vPlayerPos.x >= (iter->col - 1.5f) * fSpacing &&
			vPlayerPos.x <= (iter->col + 1.5f) * fSpacing &&
			vPlayerPos.y >= (-iter->row - 1) * fSpacing &&
			vPlayerPos.y <= (-iter->row + 5) * fSpacing &&
			!bGameOver && !bChaEnd)
		{
			if (iter->iType == 0)
				iHopeFragment++;
			else
				iGoldFragment++;
			iFragment++;

			if (iPlayerLP < iMaxLP)
				iPlayerLP++;

			pSE0->Play(0);
			iter = m_vecPoints.erase(iter);
		}
		else if (iter->row<= 1)
			iter = m_vecPoints.erase(iter);
		else
			iter++;
	}

	m_UpdateTime += fElapsedTime;
	if (m_UpdateTime >= (float)fBaseSPB / 4)
	{
		m_iTex++;
		if (m_iTex >= 4) m_iTex = 0;
		m_UpdateTime -= (float)fBaseSPB / 4;
	}

	return TRUE;
}

BOOL Fragment::Render(double fTime, float fElapsedTime)
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

	for (auto c : m_vecPoints)
	{
		D3DXMatrixTranslation(&matWorld, c.col * fSpacing, -c.row * fSpacing, 0.0f);
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

bool Fragment::Add(int row, int col)
{
	int iType = 0;
	if ((rand() % 100 < (5 + iStage + iLuc / 3)))
		iType = 1;

	for (auto &point : m_vecPoints)
	{
		if (row == point.row && col == point.col)
		{
			point.iType = 1;
			return true;
		}
	}

	m_vecPoints.push_back({ iType, row, col });
	return true;
}

BOOL Fragment::Roll()
{
	for (auto &data : m_vecPoints)
		data.row--;

	return TRUE;
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
	m_pHopeFragment->AddTexRect(0, 0, 16, 32);
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
	m_pGoldFragment->AddTexRect(0, 32, 16, 64);
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
	m_pConnection->AddTexRect(64, 0, 80, 32);
	m_pConnection->AddTexRect(80, 0, 96, 32);
	m_pConnection->AddTexRect(96, 0, 112, 32);
	m_pConnection->AddTexRect(112, 0, 128, 32);
	m_pConnection->AddTexRect(64, 32, 80, 64);
	m_pConnection->AddTexRect(80, 32, 96, 64);
	m_pConnection->AddTexRect(96, 32, 112, 64);
	m_pConnection->AddTexRect(112, 32, 128, 64);
	m_pConnection->AddState(0, LOOP, (float)fBaseSPB / 4, 0, 1, 2, 3, 4, 5, 6, 7);
	m_pConnection->SetState(0);
	m_pConnection->SetPos(7.0f * fActSpacing, fActSpacing);

	m_pDash = new DXRHWObject();
	m_pDash->SetRange(-fActSpacing, -fActSpacing, fActSpacing, fActSpacing);
	m_pDash->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	m_pDash->Init();
	m_pDash->AddTexRect(0, 0, 32, 32);
	m_pDash->AddState(0, SINGLE, 0.0f, 0);
	m_pDash->SetState(0);
	m_pDash->SetPos(15 * fActSpacing, fActSpacing);

	m_pTurn = new DXRHWObject();
	m_pTurn->SetRange(-fActSpacing, -fActSpacing, fActSpacing, fActSpacing);
	m_pTurn->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	m_pTurn->Init();
	m_pTurn->AddTexRect(32, 0, 64, 32);
	m_pTurn->AddState(0, SINGLE, 0.0f, 0);
	m_pTurn->SetState(0);
	m_pTurn->SetPos(19 * fActSpacing, fActSpacing);

	m_pDJump = new DXRHWObject();
	m_pDJump->SetRange(-fActSpacing, -fActSpacing, fActSpacing, fActSpacing);
	m_pDJump->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	m_pDJump->Init();
	m_pDJump->AddTexRect(64, 0, 96, 32);
	m_pDJump->AddState(0, SINGLE, 0.0f, 0);
	m_pDJump->SetState(0);
	m_pDJump->SetPos(23 * fActSpacing, fActSpacing);

	m_pBuild = new DXRHWObject();
	m_pBuild->SetRange(-fActSpacing, -fActSpacing, fActSpacing, fActSpacing);
	m_pBuild->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	m_pBuild->Init();
	m_pBuild->AddTexRect(96, 0, 128, 32);
	m_pBuild->AddState(0, SINGLE, 0.0f, 0);
	m_pBuild->SetState(0);
	m_pBuild->SetPos(27 * fActSpacing, fActSpacing);

	m_pCharge = new DXRHWObject();
	m_pCharge->SetRange(-fActSpacing, -fActSpacing / 2, fActSpacing, fActSpacing / 2);
	m_pCharge->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM3), 128, 128);
	m_pCharge->Init();
	m_pCharge->AddTexRect( 0, 32,  32, 48);
	m_pCharge->AddTexRect(32, 32,  64, 48);
	m_pCharge->AddTexRect(64, 32,  96, 48);
	m_pCharge->AddTexRect(96, 32, 128, 48);
	m_pCharge->AddTexRect( 0, 48,  32, 64);
	m_pCharge->AddTexRect(32, 48,  64, 64);
	m_pCharge->AddTexRect(64, 48,  96, 64);
	m_pCharge->AddTexRect(96, 48, 128, 64);
	m_pCharge->AddTexRect( 0, 64,  32, 80);
	m_pCharge->AddTexRect(32, 64,  64, 80);
	m_pCharge->AddTexRect( 0, 80,  32, 96);
	m_pCharge->AddTexRect(32, 80,  64, 96);
	m_pCharge->AddState(0, SINGLE, 0.0f, 0);
	m_pCharge->AddState(1, SINGLE, 0.0f, 1);
	m_pCharge->AddState(2, SINGLE, 0.0f, 2);
	m_pCharge->AddState(3, SINGLE, 0.0f, 3);
	m_pCharge->AddState(4, SINGLE, 0.0f, 4);
	m_pCharge->AddState(5, SINGLE, 0.0f, 5);
	m_pCharge->AddState(6, SINGLE, 0.0f, 6);
	m_pCharge->AddState(7, SINGLE, 0.0f, 7);
	m_pCharge->AddState(8, SINGLE, 0.0f, 8);
	m_pCharge->AddState(9, SINGLE, 0.0f, 9);
	m_pCharge->AddState(10, SINGLE, 0.0f, 10);
	m_pCharge->AddState(11, SINGLE, 0.0f, 11);
	m_pCharge->SetState(0);

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
	float fActSpacing = fSpacing * m_fScale;

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
		
		m_pDash->Render(fTime, fElapsedTime);
		m_pTurn->Render(fTime, fElapsedTime);
		m_pDJump->Render(fTime, fElapsedTime);
		m_pBuild->Render(fTime, fElapsedTime);

		int iCharge;

		iCharge = max(0, min(10, (int)(10.0f * timeDash / timeMaxDash)));
		if (timeInfDash > 0.0f)
		{
			if ((int)(8.0f * timeInfDash / fBaseSPB) % 2 == 0)
				iCharge = 11;
			else
				iCharge = 10;
		}
		m_pCharge->SetPos(16.8f * fActSpacing, fActSpacing);
		m_pCharge->SetState(iCharge);
		m_pCharge->Render(fTime, fElapsedTime);
		
		iCharge = max(0, min(10, (int)(timeReTurn == 0.0f ? 10 : (int)(10.0f * timeTurn / timeReTurn))));
		if (timeInfTurn > 0.0f)
		{
			if ((int)(8.0f * timeInfTurn / fBaseSPB) % 2 == 0)
				iCharge = 11;
			else
				iCharge = 10;
		}
		m_pCharge->SetPos(20.8f * fActSpacing, fActSpacing);
		m_pCharge->SetState(iCharge);
		m_pCharge->Render(fTime, fElapsedTime);
		
		iCharge = max(0, min(10, (int)(10.0f - 10.0f * iJump / cMaxJump)));
		if (timeInfJump > 0.0f)
		{
			if ((int)(8.0f * timeInfJump / fBaseSPB) % 2 == 0)
				iCharge = 11;
			else
				iCharge = 10;
		}
		m_pCharge->SetPos(24.8f * fActSpacing, fActSpacing);
		m_pCharge->SetState(iCharge);
		m_pCharge->Render(fTime, fElapsedTime);

		left = (int)(28.0f * fSpacing * m_fScale);
		right = (int)(30.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"X %3d", iBuild);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, WHITE);

		if (!bGameOver && !bChaEnd && !bInterStage)
		{
			if (IsInfiniteMode())
			{
				left = (int)(0.5f * fSpacing * m_fScale);
				right = (int)(3.5f * fSpacing * m_fScale);
				top = (int)(13.5f * fSpacing * m_fScale);
				bottom = (int)(18.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Floor:\n%d / ∞", iFloor);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);

				int iSec = (int)timeCost / 1000;
				int iMil = (int)timeCost % 1000 / 10;

				left = (int)(0.5f * fSpacing * m_fScale);
				right = (int)(3.5f * fSpacing * m_fScale);
				top = (int)(15.0f * fSpacing * m_fScale);
				bottom = (int)(18.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Time:\n%d\"%02d", iSec, iMil);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);
			}
			else
			{
				left = (int)(0.5f * fSpacing * m_fScale);
				right = (int)(3.5f * fSpacing * m_fScale);
				top = (int)(12.0f * fSpacing * m_fScale);
				bottom = (int)(15.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Stage \n%d / %d", iStage, cStage);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);

				left = (int)(0.5f * fSpacing * m_fScale);
				right = (int)(3.5f * fSpacing * m_fScale);
				top = (int)(13.5f * fSpacing * m_fScale);
				bottom = (int)(18.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Floor:\n%d / %d", iFloor, cFloor);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);

				int iSec = (int)timeCost / 1000;
				int iMil = (int)timeCost % 1000 / 10;

				left = (int)(0.5f * fSpacing * m_fScale);
				right = (int)(3.5f * fSpacing * m_fScale);
				top = (int)(15.0f * fSpacing * m_fScale);
				bottom = (int)(18.0f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Time:\n%d\"%02d", iSec, iMil);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);
			}
		}

		if (bInterStage && !bStageChange)
		{
			if (timeInter >= 1.0f * fBaseSPB)
			{
				left = (int)(0.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				top = (int)(2.5f * fSpacing * m_fScale);
				bottom = (int)(3.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Stage %d Clear", iStage);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (timeInter >= 2.0f * fBaseSPB)
			{
				int iSec = (int)timeCost / 1000;
				int iMil = (int)timeCost % 1000 / 10;

				left = (int)(0.0f * fSpacing * m_fScale);
				right = (int)(10.0f * fSpacing * m_fScale);
				top = (int)(13.5f * fSpacing * m_fScale);
				bottom = (int)(14.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Cost Time: %d\"%02d", iSec, iMil);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (timeInter >= 3.0f * fBaseSPB)
			{
				left = (int)(10.0f * fSpacing * m_fScale);
				right = (int)(20.0f * fSpacing * m_fScale);
				top = (int)(13.5f * fSpacing * m_fScale);
				bottom = (int)(14.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Damage Counts: %d", iDamage);
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);
			}

			if (timeInter >= 4.0f * fBaseSPB)
			{
				int iSec = (int)timeCost / 1000;
				int iMil = (int)timeCost % 1000 / 10;

				left = (int)(20.0f * fSpacing * m_fScale);
				right = (int)(30.0f * fSpacing * m_fScale);
				top = (int)(13.5f * fSpacing * m_fScale);
				bottom = (int)(14.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Fragment: %d", iFragment);
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

		if (bComplete)
		{
			int iAlpha = min(127, (int)(127.0f * timeComplete / 3000.0f));
			RenderAlphaCurtain(iAlpha);
			if (timeComplete > 2000.0f)
			{
				if (!m_bSoundPlay[0])
				{
					pSE0->Play(2);
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

		if (bChaEnd && GAME_PAUSE != GetMode())
		{
			int iAlpha = min(127, (int)(127.0f * timeChaEnd / 3000.0f));
			RenderAlphaCurtain(iAlpha);
			if (timeChaEnd > 2000.0f)
			{
				if (!m_bSoundPlay[0])
				{
					pSE0->Play(2);
					m_bSoundPlay[0] = true;
				}
				if (timeChaEnd <= 2400.0f)
				{
					color = D3DCOLOR_ARGB(255, 255, 255, 127);
					left = (int)((timeChaEnd / 10.0f - 240.0f) * fSpacing * m_fScale);
					top = (int)(7.0f * fSpacing * m_fScale);
					right = (int)((timeChaEnd / 10.0f - 210.0f) * fSpacing * m_fScale);
					bottom = (int)(9.0f * fSpacing * m_fScale);
					SetRect(&rcText, left, top, right, bottom);
					swprintf_s(szBuffer, L"Challenge End");
					m_pLFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, color);
				}
				else
				{
					int iBlue = max(127, 255 - (int)(128.0f / 300.0f * abs(timeChaEnd - 2700.0)));
					color = D3DCOLOR_ARGB(255, 255, 255, iBlue);
					left = (int)(0.0f * fSpacing * m_fScale);
					top = (int)(7.0f * fSpacing * m_fScale);
					right = (int)(30.0f * fSpacing * m_fScale);
					bottom = (int)(9.0f * fSpacing * m_fScale);
					SetRect(&rcText, left, top, right, bottom);
					swprintf_s(szBuffer, L"Challenge End");
					if (timeChaEnd < 2700.0f)
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
					pSE0->Play(3);
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
					swprintf_s(szBuffer, L"Killed by Lances");
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

				if (IsInfiniteMode())
				{
					if (timeCount >= 500.0f)
					{
						if (!m_bSoundPlay[1])
						{
							pSE1->Play(1);
							m_bSoundPlay[1] = true;
						}
						SetGridRect(&rcText, 11.5f, 4.5f, 18.5f, 6.0f, fActSpacing);
						swprintf_s(szBuffer, L"最高层数");
						m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, BLACK);
						swprintf_s(szBuffer, L"%d", iMaxFloor);
						m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
					}

					if (timeCount >= 1000.0f)
					{
						if (!m_bSoundPlay[2])
						{
							pSE1->Play(1);
							m_bSoundPlay[2] = true;
						}
						int iMin = (int)timeCost / 60000;
						int iSec = (int)timeCost / 1000 - iMin * 60;
						int iMil = (int)timeCost % 1000 / 10;

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
						swprintf_s(szBuffer, L"%d", iDamage);
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
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iGoldFragment - iGoldFragment, iGoldFragment);
						m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_TOP, BLACK);
					}

				}
				else
				{
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
				{
					if (IsInfiniteMode())
					{
						if (GAME_PAUSE != GetMode())
							GameOver();
					}
					else
						SetCurtain(127, 255, 1500.0f, OpenSelectMenu);

				}
				else
				{
					GetSaveData().m_iStory = GetDifficulty() >= 2 ? 5 : 4;
					SetScene(STORY);
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

SceneUpstairs::SceneUpstairs()
{

}

SceneUpstairs::~SceneUpstairs()
{
	End();
}

void SceneUpstairs::Init()
{
	//
	//Player Character
	//

	iCharaID = IsStoryMode() ? IDC_NAEGI : GetSaveData().m_iCharaID;
	iStr = GetCharaSkillLevel(iCharaID, 0);
	iAgi = GetCharaSkillLevel(iCharaID, 1);
	iEnd = GetCharaSkillLevel(iCharaID, 2);
	iInt = GetCharaSkillLevel(iCharaID, 3);
	iLuc = GetCharaSkillLevel(iCharaID, 4);

	//InfiniteMode

	if (IsInfiniteMode())
	{
		iStr = 18;
		iAgi = 6;
		iEnd = 2;
		iInt = 12;
	}

	timeReTurn = 4000.0f * (1.0f - (float)iStr / 18.0f);
	timeMaxDash = 300.0f + iAgi * 150.0f;
	iMaxLP = 6 + 2 * iEnd;
	cMaxJump = 1 + iInt / 6;

	//
	//Difficulty
	//

	timeBuff = 8000.0f;

	int iDifficulty = GetDifficulty();
	vecStage.clear();
	switch (iDifficulty)
	{
	case 0:
		vecStage.push_back({ 3, 48, 2, 3, 4 });
		vecStage.push_back({ 4, 48, 2, 3, 4 });
		vecStage.push_back({ 5, 48, 1, 2, 4 });
		cStage = 3;
		cLevelUp = 4;
		break;
	case 1:
		vecStage.push_back({ 3, 60, 2, 3, 255 });
		vecStage.push_back({ 5, 60, 1, 2, 255 });
		vecStage.push_back({ 7, 60, 1, 2, 5 });
		vecStage.push_back({ 9, 60, 1, 1, 3 });
		cStage = 4;
		cLevelUp = 4;
		break;
	case 2:
		vecStage.push_back({ 5, 60, 1, 2, 5 });
		vecStage.push_back({ 7, 80, 1, 1, 5 });
		vecStage.push_back({ 11, 80, 1, 1, 4 });
		vecStage.push_back({ 18, 80, 1, 1, 3 });
		cStage = 4;
		cLevelUp = 3;
		break;
	case 3:
		vecStage.push_back({ 81, 120, 1, 1, 3 });
		cStage = 1;
		cLevelUp = 9;
		break;
	}

	//Debug Mode

	if (IsDebugMode())
	{
		cStage = 1;
	}

	//Infinite Mode

	if (IsInfiniteMode())
	{
		vecStage.clear();
		vecStage.push_back({ 65535, 120, 1,1,6 });
		cLevelUp = 12;
	}

	//Setting

	fScale = GetSettings().m_fScale;
	iCameraRange = GetSettings().m_iUSRange;
	SetUpstairsCameraRange(iCameraRange);

	//Calculated

	fActSpacing = fSpacing * fScale;
	fBaseSPB = 60000.0 / fBPM;

	//Timer and Counter

	timeTimer = 0.0f;
	timeOver = 0.0f;
	timeComplete = 0.0f;
	timeChaEnd = 0.0f;
	timeCount = 0.0f;

	iStage = 1;
	LoadStageInfo(iStage - 1);
	iBuild = 3;
	iJump = 0;
	iPlayerLP = iMaxLP;
	timeDash = timeMaxDash;
	timeTurn = timeReTurn;

	iDeathReason = 1;
	bGameOver = false;
	bComplete = false;
	bChaEnd = false;
	bCount = false;

	iScore = 0;
	timeCostTotal = 0.0f;
	iDamageTotal = 0;
	iFragmentTotal = 0;

	iMaxFloor = 0;
	iHopeFragment = 0;
	iGoldFragment = 0;

	bStageChange = false;
	bInterStage = false;
	timeInter = 0.0f;

	iFragment = 0;
	timeCost = 0.0f;
	iDamage = 0;

	//Camera

	fRangeX = GAME_WIDTH * fCameraScale;
	fRangeY = GAME_HEIGHT * fCameraScale;
	fCameraAngle = 0.0f;

	// Pointers

	pBackground = new Background();
	if (IsInfiniteMode())
		pBackground->Reverse(true);

	pPlayer = new Player();

	pMap = new Map();

	pItem = new Item();
	pFragment = new Fragment();
	pPrick = new Prick();

	pHUD = new HUD();

	pBGM = new Sound();
	pBGM->Register(ID_BGM_UPSTAIRS, ID_SUBMIX_BACKGROUND);
	pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_BOX16, L"MP3", 255);

	pSE0 = new Sound();
	pSE0->Register(ID_SE_UP0, ID_SUBMIX_SOUNDEFFECT);
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_ITEM, L"WAVE");//0
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_PRICK, L"WAVE");//1
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CLEAR, L"MP3");//2
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_FAIL, L"MP3");//3

	pSE1 = new Sound();
	pSE1->Register(ID_SE_UP1, ID_SUBMIX_SOUNDEFFECT, TRUE);
	pSE1->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_DASH, L"WAVE");//0
	pSE1->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SCORE, L"WAVE");//1

	pSE2 = new Sound();
	pSE2->Register(ID_SE_UP2, ID_SUBMIX_SOUNDEFFECT, TRUE);
	pSE2->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_STEP, L"WAVE");//0

	//Camera
	
	pPlayer->GetPos(&vPlayerPos);
	fRangeX = GAME_WIDTH * fCameraScale;
	fRangeY = GAME_HEIGHT * fCameraScale;

	vCameraPos.x = max(min(vPlayerPos.x, cCol * fSpacing - fRangeX * 2 / 5), fRangeX * 2 / 5);
	vCameraPos.y = max(min(vCameraPos.y, -fRangeY / 2),
		(-cRow + max(0, cRollMap - 18 * cFloor - 16)) * fSpacing + fRangeY / 2);
	
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
		pPlayer->SetState(R_MOVE, 1.0f, 0.0f);
	}
}

void SceneUpstairs::End()
{
	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pPrepare);
	SAFE_DELETE(pSEText);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pMap);
	SAFE_DELETE(pItem);

	for (auto p : vecEnemy)
	{
		SAFE_DELETE(p);
	}
	vecEnemy.clear();

	for (auto p : vecFlyEnemy)
	{
		SAFE_DELETE(p);
	}
	vecFlyEnemy.clear();

	vecStage.clear();

	SAFE_DELETE(pPrick);
	SAFE_DELETE(pHUD);
	SAFE_DELETE(pBGM);
	SAFE_DELETE(pSE0);
	SAFE_DELETE(pSE1);
	SAFE_DELETE(pSE2);
}

void SceneUpstairs::Reset()
{
	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pPrepare);
	SAFE_DELETE(pSEText);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pMap);
	SAFE_DELETE(pItem);

	for (auto p : vecEnemy)
	{
		SAFE_DELETE(p);
	}
	vecEnemy.clear();

	for (auto p : vecFlyEnemy)
	{
		SAFE_DELETE(p);
	}
	vecFlyEnemy.clear();

	SAFE_DELETE(pPrick);
	SAFE_DELETE(pHUD);

	pBGM->Stop();
	
	//Timer and Counter
	
	timeTimer = 0.0f;
	timeOver = 0.0f;
	timeComplete = 0.0f;
	timeChaEnd = 0.0f;
	timeCount = 0.0f;

	iStage = 1;
	LoadStageInfo(iStage - 1);
	iBuild = 3;
	iJump = 0;
	iPlayerLP = iMaxLP;
	timeDash = timeMaxDash;
	timeTurn = timeReTurn;

	iDeathReason = 1;
	bGameOver = false;
	bComplete = false;
	bChaEnd = false;
	bCount = false;

	iScore = 0;
	timeCostTotal = 0.0f;
	iDamageTotal = 0;
	iFragmentTotal = 0;

	iMaxFloor = 0;
	iHopeFragment = 0;
	iGoldFragment = 0;

	bStageChange = false;
	bInterStage = false;
	timeInter = 0.0f;

	iFragment = 0;
	timeCost = 0.0f;
	iDamage = 0;

	//Camera

	fRangeX = GAME_WIDTH * fCameraScale;
	fRangeY = GAME_HEIGHT * fCameraScale;
	fCameraAngle = 0.0f;

	// Pointers

	pBackground = new Background();

	pPlayer = new Player();
	pPlayer->SetState(R_MOVE, 1.0f, 0.0f);

	pMap = new Map();

	pItem = new Item();
	pFragment = new Fragment();
	pPrick = new Prick();

	pHUD = new HUD();

	pBGM->Play(0);
}

void SceneUpstairs::OnResetDevice()
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

	if (NULL != pHUD)
		pHUD->OnResetDevice();
}

void SceneUpstairs::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	if (NULL != pHUD)
		pHUD->OnLostDevice();
}

void SceneUpstairs::Update(double fTime, float fElapsedTime)
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
			pPlayer->SetState(R_MOVE, 1.0f, 0.0f);
		}
		return;
	}

	//Timer and Counter

	timeTimer += fElapsedTime;

	if (timeInfDash > 0.0f)
	{
		timeInfDash -= fElapsedTime;
		timeDash = timeMaxDash;
	}
	if (timeInfDash < 0.0f)
		timeInfDash = 0.0f;

	if (timeInfTurn > 0.0f)
	{
		timeInfTurn -= fElapsedTime;
		timeTurn = timeReTurn;
	}
	if (timeInfTurn < 0.0f)
		timeInfTurn = 0.0f;

	if (timeInfJump > 0.0f)
	{
		timeInfJump -= fElapsedTime;
		iJump = 0;
	}
	if (timeInfJump < 0.0f)
		timeInfJump = 0.0f;

	if (timeSuper > 0.0f)
	{
		timeSuper -= fElapsedTime;
		timeDash = timeMaxDash;
		timeTurn = timeReTurn;
		iJump = 0;
	}

	if (IsDebugMode())
	{
		if (timeSuper <= 0.0f)
			timeSuper += timeBuff;
	}
	else
	{
		if (timeSuper < 0.0f)
			timeSuper = 0.0f;
	}

	//Player State

	pPlayer->Update(fTime, fElapsedTime);
	pPlayer->GetPos(&vPlayerPos);
	if (vPlayerPos.y < -30.0f * fSpacing)
		RollScene();
	iPlayerBlock = cRollMap - cRow - 1 - (int)(vPlayerPos.y / fSpacing);
	iFloor = iPlayerBlock / 18;
	iMaxFloor = max(iMaxFloor, iFloor);

	if (!bInterStage)
	{
		if (fCameraAngle == 0.0f && !bGameOver && !bChaEnd)
			timeCost += fElapsedTime;

		if (!bStageChange)
		{
			if (pPlayer->IsTarget())
			{
				{
					bStageChange = true;
					fCameraRadius = vPlayerPos.x - vCameraPos.x;

					for (auto p : vecEnemy)
					{
						SAFE_DELETE(p);
					}
					vecEnemy.clear();

					for (auto p : vecFlyEnemy)
					{
						SAFE_DELETE(p);
					}
					vecFlyEnemy.clear();
				}
			}
		}

		if (pPlayer->IsFall())
		{
			if (IsInfiniteMode())
			{
				if (!bChaEnd)
				{
					bChaEnd = true;
					iDeathReason = 2;
					timeOver = 0.0f;
				}
			}
			else
			{
				if (!bGameOver)
				{
					bGameOver = true;
					iDeathReason = 2;
					timeOver = 0.0f;
				}
			}
		}
	}
	else
	{
		timeInter += fElapsedTime;
		if (!bStageChange)
		{
			if (iStage >= cStage)
			{
				if (pPlayer->IsTarget())
				{
					if (!bComplete)
					{
						timeCostTotal += timeCost;
						iDamageTotal += iDamage;

						int StageScore = iStage * 5000;
						StageScore += iFragment * 500;
						StageScore -= iDamage * 200;
						StageScore -= (int)timeCost / 100;
						if (StageScore < 0)
							StageScore = 0;
						StageScore += iStage * 5000;
						iScore += StageScore;

						bComplete = true;
						pPlayer->SetState(HAPPY);


						if (!IsDebugMode())
						{
							GetSaveData().m_iHopeFragment = min(999, GetSaveData().m_iHopeFragment + iHopeFragment);
							GetSaveData().m_iGoldFragment = min(999, GetSaveData().m_iGoldFragment + iGoldFragment);

							if (iDamageTotal == 0)
								GetSaveData().m_iNoMiss[2][GetDifficulty()] = 1;

							int iMin = (int)timeCostTotal / 60000;
							int iSec = (int)timeCostTotal / 1000 % 60;
							CheckUSScore(GetDifficulty(), iScore, iMin, iSec, iDamageTotal, iFragmentTotal);

							SaveDataUpdated();
						}
					}
				}
			}
			else
			{
				if (pPlayer->IsTarget())
				{
					timeCostTotal += timeCost;
					iDamageTotal += iDamage;

					int StageScore = iStage * 5000;
					StageScore += iFragment * 500;
					StageScore -= iDamage * 200;
					StageScore -= (int)timeCost / 100;
					if (StageScore < 0)
						StageScore = 0;
					StageScore += iStage * 5000;
					iScore += StageScore;

					bStageChange = true;
					iFragment = 0;
					iDamage = 0;
					timeCost = 0.0f;
					iStage++;
				}
			}
		}
	}

	if (!bInterStage && !bStageChange)
	{
		pItem->Update(fTime, fElapsedTime);
		pFragment->Update(fTime, fElapsedTime);
		pPrick->Update(fTime, fElapsedTime);

		{
			auto iter = vecEnemy.begin();
			while (iter != vecEnemy.end())
			{
				(*iter)->Update(fTime, fElapsedTime);
				if ((*iter)->IsDestroy())
				{
					(*iter)->~Enemy();
					SAFE_DELETE(*iter);
					iter = vecEnemy.erase(iter);
				}
				else
					iter++;
			}
		}

		{
			auto iter = vecFlyEnemy.begin();
			while (iter != vecFlyEnemy.end())
			{
				(*iter)->Update(fTime, fElapsedTime);
				if ((*iter)->IsDestroy())
				{
					(*iter)->~FlyEnemy();
					SAFE_DELETE(*iter);
					iter = vecFlyEnemy.erase(iter);
				}
				else
					iter++;
			}
		}
	}

	//Other

	pHUD->Update(fTime, fElapsedTime);

	//Camera

	if (bInterStage)
	{
		if (bStageChange)
		{
			if (fCameraAngle < D3DX_PI / 2)
			{
				fCameraAngle += fElapsedTime * 0.002f;

				pPlayer->SetPos(vCameraPos.x + fCameraRadius * cos(fCameraAngle), vPlayerPos.y);
				pPlayer->SetYaw(fCameraAngle);
			}
			else
			{
				bStageChange = false;
				bInterStage = false;

				fCameraAngle = -D3DX_PI / 2;

				LoadStageInfo(iStage -1);

				pMap->~Map();
				SAFE_DELETE(pMap);
				pMap = new Map();

				pItem = new Item();
				pFragment = new Fragment();
				pPrick = new Prick();

				pPlayer->~Player();
				SAFE_DELETE(pPlayer);
				pPlayer = new Player;
				pPlayer->SetState(D_MOVE);

				vCameraPos.y = -GAME_HEIGHT;
			}
		}
		else
		{
			if (fCameraAngle < 0.0f)
			{
				fCameraAngle += fElapsedTime * 0.002f;
				if (fCameraAngle >= 0.0f)
				{
					fCameraAngle = 0.0f;
					pPlayer->SetState(R_MOVE, 1.0f, 0.0f);
				}
				pPlayer->SetPos(vCameraPos.x - fCameraRadius * cos(fCameraAngle), vPlayerPos.y);
				pPlayer->SetYaw(fCameraAngle);
			}
		}
	}
	else
	{
		if (!bStageChange)
		{
			if (fCameraAngle < 0.0f)
			{
				fCameraAngle += fElapsedTime * 0.002f;
				if (fCameraAngle >= 0.0f)
				{
					fCameraAngle = 0.0f;
					pPlayer->SetState(R_MOVE, 1.0f, 0.0f);
				}
				pPlayer->SetPos(vCameraPos.x - fCameraRadius * cos(fCameraAngle), vPlayerPos.y);
				pPlayer->SetYaw(fCameraAngle);
			}
			else
			{
				iCameraRange = GetSettings().m_iUSRange;
				switch (iCameraRange)
				{
				case 1:
					if (fCameraScale > 1.0f)
						fCameraScale = max(1.0f, fCameraScale - 0.002f * fElapsedTime);
					else
					{
						if (IsKeyDown(DIK_DELETE))
						{
							GetSettings().m_iUSRange = 2;
							SaveSettings();
							timeReRange = 3000.0f;
						}
					}
					break;
				case 2:
					if (fCameraScale < 2.0f)
						fCameraScale = min(2.0f, fCameraScale + 0.002f * fElapsedTime);
					else
					{
						if (IsKeyDown(DIK_INSERT))
						{
							GetSettings().m_iUSRange = 1;
							SaveSettings();
							timeReRange = 3000.0f;
						}
					}
					break;
				}

				pPlayer->GetPos(&vPlayerPos);
				fRangeX = GAME_WIDTH * fCameraScale;
				fRangeY = GAME_HEIGHT * fCameraScale;

				vCameraPos.x = max(min(vPlayerPos.x, cCol * fSpacing - fRangeX * 2 / 5), fRangeX * 2 / 5);

				if (L_MOVE == pPlayer->GetState() || R_MOVE == pPlayer->GetState())
					vCameraPos.y -= 1.2f * fElapsedTime;

				if (vPlayerPos.y < vCameraPos.y)
					vCameraPos.y = vPlayerPos.y;
				else if (vPlayerPos.y > vCameraPos.y + fRangeY / 4)
					vCameraPos.y = vPlayerPos.y - fRangeY / 4;

				vCameraPos.y = max(min(vCameraPos.y, -fRangeY / 2),
					(-cRow + max(0, cRollMap - 18 * cFloor - 16)) * fSpacing + fRangeY / 2);
			}
		}
		else
		{
			if (fCameraScale < 2.0f)
			{
				fCameraScale = min(2.0f, fCameraScale + 0.002f * fElapsedTime);
				if (fCameraScale == 2.0f)
					fCameraRadius = vPlayerPos.x - vCameraPos.x;

				fRangeX = GAME_WIDTH * fCameraScale;
				fRangeY = GAME_HEIGHT * fCameraScale;
				vCameraPos.x = max(min(vPlayerPos.x, cCol * fSpacing - fRangeX * 2 / 5), fRangeX * 2 / 5);
				vCameraPos.y = max(min(vCameraPos.y, -fRangeY / 2),
					(-cRow + max(0, cRollMap - 18 * cFloor - 16)) * fSpacing + fRangeY / 2);
			}
			else
			{
				if (fCameraAngle < D3DX_PI / 2)
				{
					fCameraAngle += fElapsedTime * 0.002f;
					if (fCameraAngle > D3DX_PI / 2)
						fCameraAngle = D3DX_PI / 2;

					pPlayer->SetPos(vCameraPos.x + fCameraRadius * cos(fCameraAngle), vPlayerPos.y);
					pPlayer->SetYaw(fCameraAngle);
				}
				else
				{
					bStageChange = false;
					bInterStage = true;
					timeInter = 0.0f;
					fCameraAngle = -D3DX_PI / 2;

					LoadStageInfo(-1);

					pMap->~Map();
					SAFE_DELETE(pMap);
					pMap = new Map();

					pPrick->~Prick();
					SAFE_DELETE(pPrick);

					pItem->~Item();
					SAFE_DELETE(pItem);

					pFragment->~Fragment();
					SAFE_DELETE(pFragment);

					pPlayer->~Player();
					SAFE_DELETE(pPlayer);
					pPlayer = new Player;
					pPlayer->SetState(D_MOVE);
					pPlayer->GetPos(&vPlayerPos);
					
					vCameraPos.x = cCol * fSpacing / 2;
					vCameraPos.y = -cRow * fSpacing / 2;
					fCameraRadius = vCameraPos.x - vPlayerPos.x;
				}
			}
		}
	}

	//Game Control

	//GamePause
	if (!bGameOver && !bComplete && !bChaEnd)
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

	//Challenge End
	if (bChaEnd)
	{
		if (timeChaEnd == 0.0f)
		{
			GetSaveData().m_iHopeFragment = min(999, GetSaveData().m_iHopeFragment + iHopeFragment);
			GetSaveData().m_iGoldFragment = min(999, GetSaveData().m_iGoldFragment + iGoldFragment);

			int iMin = (int)timeCost / 60000;
			int iSec = (int)timeCost / 1000 % 60;
			CheckInfUSScore(iMaxFloor, iMin, iSec, iDamage, iFragment);

			SaveDataUpdated();
		}

		timeChaEnd += fElapsedTime;

		float fVol = 1.0f - timeChaEnd / 3000.0f;
		if (fVol >= 0.0f)
			pBGM->SetVol(fVol);
		else
			pBGM->End();

		static bool bUp = false;
		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_X) || IsKeyDown(DIK_RETURN))
		{
			if (bUp)
			{
				if (timeChaEnd >= 3000.0f)
					bCount = true;
			}
			bUp = false;
		}
		else
			bUp = true;
	}
}

void SceneUpstairs::Render(double fTime, float fElapsedTime)
{
	SetCameraPos(vCameraPos.x, vCameraPos.y);
	SetView();
	SetProj(NULL, fRangeX, fRangeY);
	SetViewPort();

	pBackground->Render(fTime, fElapsedTime);
	pMap->Render(fTime, fElapsedTime);

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

	if (!bInterStage && !bStageChange && fCameraAngle == 0.0f)
	{
		pItem->Render(fTime, fElapsedTime);
		pFragment->Render(fTime, fElapsedTime);
		for (auto p : vecEnemy)
			p->Render(fTime, fElapsedTime);
		for (auto p : vecFlyEnemy)
			p->Render(fTime, fElapsedTime);
	}

	pPlayer->Render(fTime, fElapsedTime);

	if (!bInterStage && !bStageChange && fCameraAngle == 0.0f)
	{
		pPrick->Render(fTime, fElapsedTime);
	}

	pHUD->Render(fTime, fElapsedTime);
}