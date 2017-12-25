#include "SceneUtil.h"
#include "SceneShooter.h"
#include "DXAudio.h"
#include "DXInput.h"
#include "DXCamera.h"
#include "Settings.h"
#include "SaveData.h"
#include "GameAudio.h"
#include "CharaData.h"
#include "MiscData.h"
#include "SoundData.h"

using namespace ShooterClass;

namespace ShooterData
{
	//
	//Const Parameters
	//

	const int	cRow = 17;
	const int	cCol = 30;
	const float fSpacing = 64.0f;

	const int	iRowBeg = 4;
	const int	iRowRange = 10;

	const double fBegTime = 476.0;
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

	//
	//Parameters to be Setted / Edited
	//

	//Relating to difficulty

	float	fRate;

	bool	bPoison;
	int		iPoison;
	bool	bDebuff;
	int		iDebuff;
	bool	bAutoDebuff;
	bool	bCombine;

	float	timeTotal;
	float	timeDebuff;
	float	timeRecovery;

	//Relating to Setting

	float	fScale;

	int		iDecorate;
	int		iScanAlpha;
	int		iAdjust;
	float	timeAdjust;

	//
	//Parameters to be Calculated
	//

	float	fActSpacing;
	double	fBaseSPB;
	double	fSPB;
	double	fPreTime;

	float	fExpRange;

	//
	//Timers and Counters
	//

	float	timeSmoke;
	float	timeFire;
	float	timeHidden;
	float	timePoison;
	
	int		iBeat;
	double	timeBeat;
	double	timeTimer;
	int		iFragment;

	float	timeOver;
	float	timeComplete;
	float	timeCount;

	float	timeReDecorate;
	float	timeReAlpha;
	float	timeReAdjust;

	std::vector<int> vecPitcher;

	//Parameters for Game Results

	int iDeathReason;
	bool bGameOver;
	bool bComplete;
	bool bCount;
	int iPerfect;
	int iGood;
	int iBad;
	int iMiss;
	int iCombo;
	int iMaxCombo;
	int iScore;
	int iHopeFragment;
	int iGoldFragment;

	//
	//Pointers to Objects & Sounds
	//

	Map		*pMap = NULL;
	Player	*pPlayer = NULL;
	Beats	*pBeats = NULL;
	Scanner *pScanner = NULL;
	Fire	*pFire = NULL;
	Smoke	*pSmoke = NULL;
	Judge	*pJudge = NULL;
	HUD		*pHUD = NULL;

	Sound	*pBGM = NULL;
	Sound	*pSE0 = NULL;
	Sound	*pSE1 = NULL;

	//
	//Story
	//

	const int cSent = 17;
	LPCWSTR szStory[cSent] =
	{
		L"是不是非常熟悉的场景呢~",
		L"为了确保这些备份不被「抹去」，首先我们要阻止这次裁判的「处刑」。所以虽然有点对不起狛枝的复制品们，但我们要让他们的死亡原因变成出血过多，从而使这场裁判的结论变成狛枝确实是自杀。",
		L"虽然没有办法直接干涉其他人员的行动，但通过我的程序可以让你对大家投掷出的灭火瓶进行一定的调整。",
		L"对普通的（红色）灭火瓶，可以（按Z键）加强它们的灭火能力，从而控制住火势以避免狛枝被烧死。",
		L"对于七海扔出的有毒的（紫色）灭火瓶，可以（按X键）消除毒药成分。\n我对毒药也尽力进行了改写，让毒药的性质不再是立刻致死。在受毒药影响后，可以在一段时间后恢复，但如果在这段时间内再次受到毒药影响则会死亡。",
		L"为了确保及时调整瓶子的效果同时避免我们的行动被这里的其他人观察到，你的动作不能太晚也不能太早。我为你标出了适合对灭火瓶进行干涉的区域，越接近中间的区域效果越好。",
		L"此外，还有一些试图干扰我们的对手存在，他们有可能会扔出金色的灭火瓶来干扰你的节奏，这些灭火瓶会自动消失，如果你主动去干涉它，很可能对之后的节奏造成影响，所以最好的选择是无视它们。",
		L"这些瓶子在消失后还会造成一些负面效果（在简单难度下仅主动干涉的情况下会导致负面效果）。",
		L"负面效果 - 火势扩大\n立刻造成火势的增加，并且持续效果期间无法通过普通灭火瓶减小火势。",
		L"负面效果 - 烟雾遮蔽\n在干涉区造成烟雾效果，会对干涉时机的判断带来干扰。",
		L"负面效果 - 观测干扰\n对扔出的灭火瓶类型的观测进行干扰，对判断采用何种干涉方式造成影响。",
		L"（通常，新的负面效果会取代原本的负面效果，而在欺负人难度下，这些负面效果可以同时存在。）",
		L"如果连续达成完美（PERFECT）或良好（GOOD）的干涉，会增加连击数。\n相反，糟糕（BAD）和错过（MISS）则会使连击数归零。",
		L"每实现十次连击（欺负人难度下为五次），能够获得希望碎片作为奖励，并且随着连击数增加会有逐渐增高的几率升级为金色碎片。",
		L"虽然现在还不知道这些碎片的用处，但多收集一些一定不是坏事。",
		L"最后，因为这些「虚拟世界程序」多少存在着差异，所以在这一幕出现的人员也不尽相同。为了提高效率，这些世界线被整合在了一起，所以无论会出现怎样的人员都请不要惊讶。",
		L"那么，就请努力坚持到自动消防装置正常启动吧！"
	};
	int iSent;
	int iChar;
	float timeChar;
	float timeTalk;
	float fSPChar;
	float timeStoryOver;

	bool bSentOver;
	bool bStoryOver;
	Box *pTalkBox = NULL;

	ID3DXFont *pFont = NULL;
	ID3DXFont *pSFont = NULL;
	ID3DXFont *pLFont = NULL;

	Sound *pPrepare = NULL;
	Sound *pSEText = NULL;
}

using namespace ShooterData;

void SetShooterNanami(bool bOn)
{
	bPoison = bOn;
}

void SetShooterOma(bool bOn)
{
	bDebuff = bOn;
}

void SetShooterDecorate(int iVal)
{
	iDecorate = iVal;
	pMap->~Map();
	SAFE_DELETE(pMap);
	pMap = new Map();
}

void SetShooterScanAlpha(int iVal)
{
	iScanAlpha = iVal;
	pScanner->~Scanner();
	SAFE_DELETE(pScanner);
	pScanner = new Scanner();
}

void SetShooterAdjust(int iVal)
{
	iAdjust = iVal;
	timeAdjust = (float)iAdjust;
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
	AddTexRect( 64,   0,  80,  16);			// 32 -- 1-5
	AddTexRect( 80,   0,  96,  16);			// 33 -- 1-6
	AddTexRect( 96,   0, 112,  16);			// 34 -- 1-7
	AddTexRect(112,   0, 128,  16);			// 35 -- 1-8
	AddTexRect( 64,  16,  80,  32);			// 36 -- 2-5
	AddTexRect( 80,  16,  96,  32);			// 37 -- 2-6
	AddTexRect( 96,  16, 112,  32);			// 38 -- 2-7
	AddTexRect(112,  16, 128,  32);			// 39 -- 2-8
	AddTexRect( 64,  32,  80,  48);			// 40 -- 3-5
	AddTexRect( 80,  32,  96,  48);			// 41 -- 3-6
	AddTexRect( 96,  32, 112,  48);			// 42 -- 3-7
	AddTexRect(112,  32, 128,  48);			// 43 -- 3-8
	AddTexRect( 64,  48,  80,  64);			// 44 -- 4-5
	AddTexRect( 80,  48,  96,  64);			// 45 -- 4-6
	AddTexRect( 96,  48, 112,  64);			// 46 -- 4-7
	AddTexRect(112,  48, 128,  64);			// 47 -- 4-8
	AddTexRect( 64,  64,  80,  80);			// 48 -- 5-5
	AddTexRect( 80,  64,  96,  80);			// 49 -- 5-6
	AddTexRect( 96,  64, 112,  80);			// 50 -- 5-7
	AddTexRect(112,  64, 128,  80);			// 51 -- 5-8
	AddTexRect( 64,  80,  80,  96);			// 52 -- 6-5
	AddTexRect( 80,  80,  96,  96);			// 53 -- 6-6
	AddTexRect( 96,  80, 112,  96);			// 54 -- 6-7
	AddTexRect(112,  80, 128,  96);			// 55 -- 6-8
	AddTexRect( 64,  96,  80, 112);			// 56 -- 7-5
	AddTexRect( 80,  96,  96, 112);			// 57 -- 7-6
	AddTexRect( 96,  96, 112, 112);			// 58 -- 7-7
	AddTexRect(112,  96, 128, 112);			// 59 -- 7-8

	//Base

	int MapData[cRow][cCol];
	for (int i = 0; i < cRow; i++)
		for (int j = 0; j < cCol; j++)
		{
			if (iDecorate == 2)
				MapData[i][j] = rand() % 50 < 49 ? (rand() % 2 == 0 ? 0 : 1) : 2;
			else
				MapData[i][j] = 0;
		}

	MapData[0][3] = 4;
	MapData[0][cCol - 1] = 6;
	MapData[1][3] = 16;
	MapData[2][3] = 20;
	MapData[3][3] = 24;
	MapData[4][3] = 28;
	MapData[cRow - 3][3] = 3;
	MapData[cRow - 2][3] = 12;
	MapData[cRow - 2][cCol - 1] = 14;
	MapData[cRow - 1][3] = 24;
	MapData[cRow - 1][cCol - 1] = 26;

	for (int i = 4; i < cCol - 1; i++)
	{
		if (i != 22 && i != 23)
		{
			MapData[0][i] = 5;
			MapData[1][i] = 9;
			MapData[2][i] = 9;
			MapData[3][i] = 31;
		}

		MapData[cRow - 2][i] = 13;
		MapData[cRow - 1][i] = 9;
	}

	MapData[0][21] = 21;
	MapData[2][22] = 19;
	MapData[3][4] = 25;
	MapData[4][4] = 29;
	MapData[3][21] = 25;
	MapData[3][22] = 29;
	MapData[3][24] = 30;


	MapData[1][cCol - 1] = 11;
	MapData[2][cCol - 1] = 15;
	for (int i = 3; i < cRow - 2; i++)
		MapData[i][cCol - 1] = 10;

	//Curtain

	MapData[1][20] = 32;
	MapData[2][20] = 36;
	MapData[3][20] = 40;
	for (int i = 4; i < 7; i++)
		MapData[i][20] = 33;
	MapData[7][20] = 37;
	MapData[8][20] = 41;

	MapData[3][21] = 25;
	for (int i = 4; i < 9; i++)
		MapData[i][21] = 23;
	MapData[9][21] = 35;

	MapData[9][20] = 34;
	for (int i = 10; i < cRow - 2; i++)
		MapData[i][20] = 38;
	MapData[cRow - 2][20] = 42;

	MapData[11][21] = 27;
	for (int i = 12; i < cRow - 2; i++)
		MapData[i][21] = 23;
	MapData[cRow - 2][21] = 17;

	//Komaeda

	MapData[7][25] = 48;
	MapData[7][26] = 49;
	MapData[8][25] = 52;
	MapData[8][26] = 53;
	MapData[9][25] = 56;
	MapData[9][26] = 57;

	//Other

	if (iDecorate >= 1)
	{
		MapData[4][28] = 39;
		MapData[5][28] = 43;
		MapData[6][28] = 47;
		MapData[7][28] = 54;
		MapData[8][28] = 58;
		MapData[9][28] = 39;
		MapData[10][28] = 43;
		MapData[11][28] = 47;
		MapData[12][28] = 39;
		MapData[13][28] = 43;
		MapData[14][28] = 47;

		MapData[14][7] = 50;
		MapData[14][8] = 51;
		MapData[14][14] = 50;
		MapData[14][15] = 51;
		MapData[13][18] = 50;
		MapData[13][19] = 51;
		MapData[14][18] = 50;
		MapData[14][19] = 51;
	}

	if (iDecorate >= 2)
	{
		MapData[4][5] = 44;
		MapData[4][6] = 45;
		MapData[4][7] = 45;
		MapData[4][8] = 46;
		MapData[5][7] = 44;
		MapData[5][8] = 45;
		MapData[5][9] = 45;
		MapData[5][10] = 45;
		MapData[5][11] = 46;
		MapData[6][10] = 44;
		MapData[6][11] = 45;
		MapData[6][12] = 45;
		MapData[6][13] = 45;
		MapData[6][14] = 45;
		MapData[6][15] = 45;
		MapData[6][16] = 46;
		MapData[7][16] = 44;
		MapData[7][17] = 45;
		MapData[7][18] = 45;
		MapData[7][19] = 46;
	}

	SAFE_RELEASE(m_pVertexBuffer);
	m_pd3dDevice->CreateVertexBuffer(4 * cRow * cCol * sizeof(TEXVERTEX), 0, TEX_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	for (int i = 0; i < cRow; i++)
		for (int j = 0; j < cCol; j++)
		{
			float left = j * fSpacing;
			float right = (j + 1) * fSpacing;
			float top = i * fSpacing;
			float bottom = (i + 1) * fSpacing;
			RECTRANGE TexRc = m_TexRects[MapData[i][j]];
			TEXVERTEX *pVertices;
			m_pVertexBuffer->Lock(4 * (i*cCol + j) * sizeof(TEXVERTEX), 0, (void**)&pVertices, 0);
			pVertices[0] = { left,	bottom,	0.1f*m_Layer,	TexRc.left,		TexRc.bottom };
			pVertices[1] = { left,	   top,	0.1f*m_Layer,	TexRc.left,		TexRc.top };
			pVertices[2] = { right,	bottom,	0.1f*m_Layer,	TexRc.right,	TexRc.bottom };
			pVertices[3] = { right,	   top,	0.1f*m_Layer,	TexRc.right,	TexRc.top };
			m_pVertexBuffer->Unlock();
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
	for (int i = 0; i < cRow; i++)
		for (int j = 0; j < cCol; j++)
		{
			m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
			m_pd3dDevice->SetFVF(TEX_FVF);
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (i*cCol + j), 2);
		}

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Player
//

Player::Player()
{
	SetRange(-64, -144, 64, 0);
	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(iCharaID), 256, 256);
	Init();
	AddTexRect(  0,   0,  32,  36);				 // 0 -- D-1
	AddTexRect( 32,   0,  64,  36);				 // 1 -- D-2
	AddTexRect(  0, 144,  32, 180);				 // 2 -- E-1
	AddTexRect(160, 144, 192, 180);				 // 3 -- E-6
	AddState(D_STAY, LOOP, (float)fBaseSPB, 0, 1);
	AddState(GOOD, NOLOOP, (float)fSPB / 2.0f, 2);
	AddState(PERFECT, NOLOOP, (float)fSPB / 2.0f, 3);
	AddStateChain(GOOD, D_STAY, D_STAY);
	AddStateChain(PERFECT, D_STAY, D_STAY);

	SetState(D_STAY);
	SetPos(23.0f * fSpacing, 2.5f * fSpacing);

	m_pShadow = new DXObject();
	m_pShadow->SetRange(-64, -16, 64, 16);
	m_pShadow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_SHADOW), 64, 16);
	m_pShadow->Init();
	m_pShadow->AddTexRect(0, 0, 32, 8);
	m_pShadow->AddState(0, SINGLE, 0.0f, 0);
	m_pShadow->SetState(0);
}

Player::~Player()
{
	SAFE_DELETE(m_pShadow);
	DXObject::~DXObject();
}

BOOL Player::Update(double fTime, float fElapsedTime)
{
	DXObject::Update(fTime, fElapsedTime);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);
	return TRUE;
}

BOOL Player::Render(double fTime, float fElapsedTime)
{
	m_pShadow->Render(fTime, fElapsedTime);
	DXObject::Render(fTime, fElapsedTime);
	return TRUE;
}

//
//Pitcher
//

Pitcher::Pitcher(int row, int iType)
{
	SetRange(-64, -144, 64, 0);

	int iPitcherID;

	if (iType == 0)
	{
		bool bNew = true;
		while (bNew)
		{
			bNew = false;
			iPitcherID = GetCharaID(rand() % TOTALCHARACOUNT, 0);
			if (iPitcherID < 200 || iPitcherID >= 300)
				bNew = true;
			for (auto ID : vecPitcher)
			{
				if (ID == iPitcherID)
					bNew = true;
			}
			if (iPitcherID == IDC_NANAMI || iPitcherID == IDC_KOMAEDA)
				bNew = true;
		}
	}
	else if (iType == 1)
		iPitcherID = IDC_NANAMI;
	else if (iType == 2)
	{
		iPitcherID = (rand() % 4 == 0) ? IDC_OMA : IDC_MONOKUMA;
	}

	vecPitcher.push_back(iPitcherID);
	if (vecPitcher.size() >= 6)
		vecPitcher.erase(vecPitcher.begin());

	AddTexFromResc(GetCharaDLLHandle(), MAKEINTRESOURCE(iPitcherID), 256, 256);

	Init();
	AddTexRect(64,  72,  96, 108);				 // 0 -- L-3
	AddTexRect(96,  72, 128, 108);				 // 1 -- L-4
	AddTexRect(64, 108,  96, 144);				 // 2 -- R-3
	AddTexRect(96, 108, 128, 144);				 // 3 -- R-4
	AddState(L_MOVE, LOOP, (float)fSPB / 3, 0, 1);
	AddState(R_MOVE, LOOP, (float)fSPB / 3, 2, 3);
	SetState(R_MOVE);
	m_fTime = 0.0f;
	m_iRow = row;
	SetPos(0.0f, fSpacing * (m_iRow + 1.5f));

	m_pShadow = new DXObject();
	m_pShadow->SetRange(-64, -16, 64, 16);
	m_pShadow->AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_SHADOW), 64, 16);
	m_pShadow->Init();
	m_pShadow->AddTexRect(0, 0, 32, 8);
	m_pShadow->AddState(0, SINGLE, 0.0f, 0);
	m_pShadow->SetState(0);
}

Pitcher::~Pitcher()
{
	SAFE_DELETE(m_pShadow);
	DXObject::~DXObject();
}

BOOL Pitcher::Update(double fTime, float fElapsedTime)
{
	m_fTime += fElapsedTime;

	if (m_fTime <= 2 * fSPB)
	{
		m_ObjPos.x += fElapsedTime * 1.2f * fSpacing / (float)fSPB;
	}
	else
	{
		if (L_MOVE != m_iState) SetState(L_MOVE);
		m_ObjPos.x -= fElapsedTime * 1.2f * fSpacing / (float)fSPB;
	}

	DXObject::Update(fTime, fElapsedTime);
	m_pShadow->SetPos(m_ObjPos.x, m_ObjPos.y);

	return TRUE;
}

BOOL Pitcher::Render(double fTime, float fElapsedTime)
{
	m_pShadow->Render(fTime, fElapsedTime);
	DXObject::Render(fTime, fElapsedTime);
	return TRUE;
}

//
//Beat
//

Beats::Beats()
{
	SetRange(-fSpacing / 2, -fSpacing / 4, fSpacing / 2, fSpacing / 4);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM1), 64, 64);
	AddTexRect(  0,   0,  16,   8);
	AddTexRect(  0,   8,  16,  16);
	AddTexRect( 16,   0,  32,   8);
	AddTexRect( 16,   8,  32,  16);
	AddTexRect( 32,   0,  48,   8);
	AddTexRect( 32,   8,  48,  16);
	AddTexRect( 48,   0,  64,   8);
	AddTexRect( 48,   8,  64,  16);

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

	m_fTime = 0.0f;
}

Beats::~Beats()
{
	DXObject::~DXObject();
}

BOOL Beats::Update(double fTime, float fElapsedTime)
{
	GetInput();
	m_fTime += fElapsedTime;

	for (auto &data : m_vecBeats)
	{
		if (m_fTime >= data.fBegTime + timeAdjust - 2 * fSPB && data.bIn)
		{
			if (data.iTex == 2)
				m_vecPitcher.push_back(new Pitcher(data.iRow, 1));
			else if (data.iTex == 4)
				m_vecPitcher.push_back(new Pitcher(data.iRow, 2));
			else
				m_vecPitcher.push_back(new Pitcher(data.iRow));

			data.bIn = false;
		}
		if (m_fTime >= data.fBegTime + timeAdjust)
		{
			if (data.bPitch)
			{
				data.bPitch = false;
				data.bActive = true;
			}
			data.xPos = (3.0f + 20.0f * data.iRate * (float)(m_fTime - data.fBegTime - timeAdjust) / (float)fPreTime) * fSpacing;
		}
		if (IsDebugMode() && IsKeyDown(DIK_SPACE))
		{
			if (m_fTime >= data.fBeatTime + timeAdjust + data.fLastTime)
			{
				data.bActive = false;
				pJudge->Add(PERFECT, data.xPos, data.yPos - fSpacing / 2);
			}
		}
		else
		{
			if (m_fTime >= data.fBeatTime + timeAdjust + data.fLastTime)
			{
				if (data.iTex == 4)
				{
					data.bActive = false;
					pJudge->Add(PERFECT, data.xPos, data.yPos - fSpacing / 2);
					if (bAutoDebuff)
					{
						if (rand() % 100 < iLuc)
						{
							pJudge->Add(NOEFFECT, data.xPos, data.yPos);
						}
						else
						{
							switch (rand() % 3)
							{
							case 0:
								pJudge->Add(HIDDEN, data.xPos, data.yPos);
								timeHidden = timeDebuff;
								if (!bCombine)
								{
									timeSmoke = 0.0f;
									timeFire = 0.0f;
								}
								break;
							case 1:
								pJudge->Add(FIRE, data.xPos, data.yPos);
								pFire->Set(10);
								timeFire = timeDebuff;
								if (!bCombine)
								{
									timeHidden = 0.0f;
									timeSmoke = 0.0f;
								}
								break;
							case 2:
								pJudge->Add(SMOKE, data.xPos, data.yPos);
								if (!pSmoke->IsOn()) pSmoke->Open();
								timeSmoke = timeDebuff;
								if (!bCombine)
								{
									timeHidden = 0.0f;
									timeFire = 0.0f;
								}
								break;
							}
						}
					}
				}
			}
			if (m_fTime >= data.fBeatTime + timeAdjust + data.fLastTime + fSPB * 0.9f)
			{
				data.bActive = false;
				if (data.iTex == 0)
				{
					pJudge->Add(MISS, data.xPos, data.yPos);
					pFire->Add();
				}
				else if (data.iTex == 2)
				{
					pJudge->Add(MISS, data.xPos, data.yPos);
					if (rand() % 100 < iLuc)
					{
						pJudge->Add(NOEFFECT, 26.0f * fSpacing, 8.0f * fSpacing);
					}
					else
					{
						pJudge->Add(POISON, 26.0f * fSpacing, 8.0f * fSpacing);
						if (timePoison > 0.0f)
						{
							if (!bGameOver)
							{
								bGameOver = true;
								iDeathReason = 1;
							}
						}
						else
						{
							timePoison = timeRecovery;
						}
					}
				}
			}
		}
	}

	auto iter = m_vecPitcher.begin();
	while (iter != m_vecPitcher.end())
	{
		(*iter)->Update(fTime, fElapsedTime);
		D3DXVECTOR2 vPos;
		(*iter)->GetPos(&vPos);
		if (vPos.x < 0.0f)
		{
			SAFE_DELETE(*iter);
			iter = m_vecPitcher.erase(iter);
		}
		else
			iter++;
	}

	static bool bZUp = true;
	static bool bXUp = true;
	
	if (!bGameOver)
	{
		if (IsKeyDown(DIK_Z) || IsKeyDown(DIK_J))
		{
			if (bZUp)
			{
				if (m_vecBeats.size() > 0)
				{
					auto &data = m_vecBeats[0];
					if (data.bActive)
					{
						if (data.iTex == 0)
						{
							if (data.xPos > (22.5f * fSpacing - fExpRange) && 
								data.xPos < (23.5f * fSpacing + fExpRange))
							{
								pPlayer->SetState(PERFECT);
								pJudge->Add(PERFECT, data.xPos, data.yPos);
								pFire->Del();
							}
							else if (data.xPos > (21.0f * fSpacing - 0.5f * fExpRange) && 
									 data.xPos < (25.0f * fSpacing + 0.5f * fExpRange))
							{
								pPlayer->SetState(GOOD);
								pJudge->Add(GOOD, data.xPos, data.yPos);
								pFire->Del();
							}
							else if (data.xPos > 19.0f * fSpacing && data.xPos < 27.0f * fSpacing)
							{
								pJudge->Add(BAD, data.xPos, data.yPos);
							}
							else
							{
								pJudge->Add(MISS, data.xPos, data.yPos);
								pFire->Add();
							}
						}
						else if (data.iTex == 2)
						{
							pJudge->Add(MISS, data.xPos, data.yPos);
							if (rand() % 100 < iLuc)
							{
								pJudge->Add(NOEFFECT, 26.0f * fSpacing, 8.0f * fSpacing);
							}
							else
							{
								pJudge->Add(POISON, 26.0f * fSpacing, 8.0f * fSpacing);
								if (timePoison > 0.0f)
								{
									if (!bGameOver)
									{
										bGameOver = true;
										iDeathReason = 1;
									}
								}
								else
								{
									timePoison = timeRecovery;
								}
							}
						}
						else if (data.iTex == 4)
						{
							if (rand() % 100 < iLuc)
							{
								pJudge->Add(NOEFFECT, data.xPos, data.yPos);
							}
							else
							{
								switch (rand() % 3)
								{
								case 0:
									pJudge->Add(HIDDEN, data.xPos, data.yPos);
									timeHidden = timeDebuff;
									if (!bCombine)
									{
										timeSmoke = 0.0f;
										timeFire = 0.0f;
									}
									break;
								case 1:
									pJudge->Add(FIRE, data.xPos, data.yPos);
									pFire->Set(10);
									timeFire = timeDebuff;
									if (!bCombine)
									{
										timeHidden = 0.0f;
										timeSmoke = 0.0f;
									}
									break;
								case 2:
									pJudge->Add(SMOKE, data.xPos, data.yPos);
									if (!pSmoke->IsOn()) pSmoke->Open();
									timeSmoke = timeDebuff;
									if (!bCombine)
									{
										timeHidden = 0.0f;
										timeFire = 0.0f;
									}
									break;
								}
							}
						}
						data.bActive = false;
					}
				}
				bZUp = false;
			}
		}
		else
			bZUp = true;

		if (IsKeyDown(DIK_X) || IsKeyDown(DIK_K))
		{
			if (bXUp)
			{
				if (m_vecBeats.size() > 0)
				{
					auto &data = m_vecBeats[0];
					if (data.bActive)
					{
						if (data.iTex == 2)
						{
							if (data.xPos > 22.5f * fSpacing && data.xPos < 23.5f * fSpacing)
							{
								pPlayer->SetState(PERFECT);
								pJudge->Add(PERFECT, data.xPos, data.yPos);
							}
							else if (data.xPos > 21.0f * fSpacing && data.xPos < 25.0f * fSpacing)
							{
								pPlayer->SetState(GOOD);
								pJudge->Add(GOOD, data.xPos, data.yPos);
							}
							else if (data.xPos > 19.0f * fSpacing && data.xPos < 27.0f * fSpacing)
							{
								pJudge->Add(BAD, data.xPos, data.yPos);
							}
							else
							{
								pJudge->Add(MISS, data.xPos, data.yPos);
								if (rand() % 100 < iLuc)
								{
									pJudge->Add(NOEFFECT, 26.0f * fSpacing, 8.0f * fSpacing);
								}
								else
								{
									pJudge->Add(POISON, 26.0f * fSpacing, 8.0f * fSpacing);
									if (timePoison > 0.0f)
									{
										if (!bGameOver)
										{
											bGameOver = true;
											iDeathReason = 1;
										}
									}
									else
									{
										timePoison = timeRecovery;
									}
								}
							}
						}
						else if (data.iTex == 0)
						{
							pJudge->Add(MISS, data.xPos, data.yPos);
							pFire->Add();
						}
						else if (data.iTex == 4)
						{
							if (rand() % 100 < iLuc)
							{
								pJudge->Add(NOEFFECT, data.xPos, data.yPos);
							}
							else
							{
								switch (rand() % 3)
								{
								case 0:
									pJudge->Add(HIDDEN, data.xPos, data.yPos);
									timeHidden = timeDebuff;
									if (!bCombine)
									{
										timeSmoke = 0.0f;
										timeFire = 0.0f;
									}
									break;
								case 1:
									pJudge->Add(FIRE, data.xPos, data.yPos);
									pFire->Set(10);
									timeFire = timeDebuff;
									if (!bCombine)
									{
										timeHidden = 0.0f;
										timeSmoke = 0.0f;
									}
									break;
								case 2:
									pJudge->Add(SMOKE, data.xPos, data.yPos);
									if (!pSmoke->IsOn()) pSmoke->Open();
									timeSmoke = timeDebuff;
									if (!bCombine)
									{
										timeHidden = 0.0f;
										timeFire = 0.0f;
									}
									break;
								}
							}
						}
						data.bActive = false;
					}
				}
				bXUp = false;
			}
		}
		else
			bXUp = true;
	}

	auto itBeat = m_vecBeats.begin();
	while (itBeat != m_vecBeats.end())
	{
		if (!itBeat->bPitch && !itBeat->bActive)
			itBeat = m_vecBeats.erase(itBeat);
		else
			itBeat++;
	}

	return TRUE;
}

BOOL Beats::Render(double fTime, float fElapsedTime)
{
	for (int row = iRowBeg; row < iRowBeg + iRowRange; row++)
	{
		for (auto p : m_vecPitcher)
		{
			if (row == p->m_iRow)
				p->Render(fTime, fElapsedTime);
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

	for (auto &data : m_vecBeats)
	{
		if (data.bActive)
		{
			D3DXMATRIX matWorld;
			D3DXMatrixTranslation(&matWorld, data.xPos, data.yPos, 0.0f);

			m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
			m_pd3dDevice->SetTexture(0, m_pTexture);
			m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(TEXVERTEX));
			m_pd3dDevice->SetFVF(TEX_FVF);

			if (timeHidden > 0.0f && (int)(2.0f * m_fTime / fBaseSPB) % 2 == 1)
			{
				if (data.xPos > 19.0f * fSpacing && data.xPos < 27.0f * fSpacing)
					m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 28, 2);
				else
					m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 24, 2);
			}
			else
			{
				if (data.xPos > 19.0f * fSpacing && data.xPos < 27.0f * fSpacing)
					m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * (data.iTex + 1), 2);
				else
					m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * data.iTex, 2);
			}

			D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		}
	}

	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

void Beats::AddBeat(int iBeat, int iLastBeat, int iRateSpeed)
{
	BeatData newBeat;

	static int iLastPoison = iPoison;
	static int iLastDebuff = iDebuff;

	newBeat.iTex = 0;

	if (bPoison && iLastPoison > iPoison && rand() % 2 == 0)
		newBeat.iTex = 2;
	else if (bDebuff && iLastDebuff > (iDebuff + iStr / 6) && rand() % 20 < (10 - iStr / 3))
		newBeat.iTex = 4;

	switch (newBeat.iTex)
	{
	case 0:
		iLastPoison++;
		iLastDebuff++;
		break;
	case 2:
		iLastPoison = 0;
		iLastDebuff++;
		break;
	case 4:
		iLastPoison++;
		iLastDebuff = 0;
		break;
	}

	newBeat.iRate = iRateSpeed;

	bool bNew = true;
	while (bNew)
	{
		bNew = false;
		newBeat.iRow = iRowBeg + rand() % iRowRange;
		auto iter = m_vecBeats.end() - min(m_vecBeats.size(),5);
		while (iter != m_vecBeats.end())
		{
			if (iter->iRow == newBeat.iRow)
			{
				bNew = true;
				break;
			}
			iter++;
		}
	}

	newBeat.fBeatTime = fBegTime + iBeat * fSPB;
	newBeat.fBegTime = newBeat.fBeatTime - fPreTime;
	newBeat.fLastTime = iLastBeat * 60000.0f / fBPM;
	newBeat.xPos = 0.0f;
	newBeat.yPos = fSpacing * (newBeat.iRow + 0.5f);
	newBeat.bIn = true;
	newBeat.bPitch = true;
	newBeat.bActive = false;

	m_vecBeats.push_back(newBeat);
}

//
//Scanner
//

Scanner::Scanner()
{
	m_pd3dDevice->CreateVertexBuffer(32 * iRowRange * sizeof(COLORVERTEX), 0, COLOR_FVF,
		D3DPOOL_MANAGED, &m_pVertexBuffer, NULL);

	COLORVERTEX *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < iRowRange; i++)
	{
		int row = iRowBeg + i;
		for (int col = 19; col < 27; col++)
		{

			int index = 4 * (8 * i + col - 19);
			float left   = col * fSpacing;
			float right  = col * fSpacing + fSpacing * 15.0f / 16.0f;
			float top    = row * fSpacing;
			float bottom = row * fSpacing + fSpacing * 15.0f / 16.0f;
			DWORD color = (col == 22 || col == 23) ? D3DCOLOR_ARGB(iScanAlpha, 0, 0, 120) :
						  (col == 21 || col == 24) ? D3DCOLOR_ARGB(iScanAlpha, 0, 120, 0) : 
													 D3DCOLOR_ARGB(iScanAlpha, 120, 120, 0);

			pVertices[index    ] = { left,	bottom,	0.1f * m_Layer, color };
			pVertices[index + 1] = { left,	   top,	0.1f * m_Layer, color };
			pVertices[index + 2] = { right,	bottom,	0.1f * m_Layer, color };
			pVertices[index + 3] = { right,	   top,	0.1f * m_Layer, color };
		}
	}
	m_pVertexBuffer->Unlock();
}

Scanner::~Scanner()
{
	DXObject::~DXObject();
}

BOOL Scanner::Render(double fTime, float fElapsedTime)
{
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;

	m_pd3dDevice->SetTexture(0, NULL);

	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(COLORVERTEX));
	m_pd3dDevice->SetFVF(COLOR_FVF);

	for (int i = 0; i < 88; i++)
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * i, 2);

	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Fire
//

Fire::Fire()
{
	SetRange(0, 0, fSpacing, fSpacing);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM1), 64, 64);
	AddTexRect( 0, 16, 16, 32);
	AddTexRect(16, 16, 32, 32);
	AddTexRect(32, 16, 48, 32);
	AddTexRect(48, 16, 64, 32);

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

	m_iMinFire = 0;
	m_cFire = 0;
	m_fTime = 0.0f;

	for (int i = 0; i < 12; i++)
	{
		m_arrLifeFire[i] = { 14 - i, 27, rand() % 4, 0.0f };
	}

	m_arrMapFire[0] = { 7, 28, rand() % 4, 0.0f };
	m_arrMapFire[1] = { 4, 28, rand() % 4, 0.0f };
	m_arrMapFire[2] = { 9, 28, rand() % 4, 0.0f };
	m_arrMapFire[3] = { 12, 28, rand() % 4, 0.0f };

	for (int i = 0; i < m_cFire; i++)
	{
		bool bNew = true;
		FireData newFire;
		while (bNew)
		{
			bNew = false;
			newFire = { 14 - rand() % 11, 20, rand() % 4, 0.0f };
			for (auto data : m_vecRndFire)
			{
				if (newFire.row == data.row)
					bNew = true;
			}
		}
		m_vecRndFire.push_back(newFire);
	}
}

Fire::~Fire()
{
	DXObject::~DXObject();
}

BOOL Fire::Update(double fTime, float fElapsedTime)
{
	m_fTime += fElapsedTime;

	if (m_fTime >= 0.25f * (float)fBaseSPB)
	{
		for (int i = 0; i < min(m_cFire, 12); i++)
			m_arrLifeFire[i].iTex = (m_arrLifeFire[i].iTex + 1) % 4;

		for (int i = 0; i < 4; i++)
			m_arrMapFire[i].iTex = (m_arrMapFire[i].iTex + 1) % 4;

		for (auto &data : m_vecRndFire)
			data.iTex = (data.iTex + 1) % 4;

		m_fTime -= 0.25f * (float)fSPB;
	}

	return TRUE;
}

BOOL Fire::Render(double fTime, float fElapsedTime)
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

	for (int i = 0; i < min(m_cFire, 12); i++)
	{
		float x = m_arrLifeFire[i].col * fSpacing;
		float y = m_arrLifeFire[i].row * fSpacing;

		D3DXMatrixTranslation(&matWorld, x, y, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * m_arrLifeFire[i].iTex, 2);
	}

	for (int i = 0; i < 4; i++)
	{
		float x = m_arrMapFire[i].col * fSpacing;
		float y = m_arrMapFire[i].row * fSpacing;

		D3DXMatrixTranslation(&matWorld, x, y, 0.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * m_arrMapFire[i].iTex, 2);
	}

	for (auto data : m_vecRndFire)
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

void Fire::Add()
{
	m_cFire++;
	
	if (m_cFire <= 12)
	{
		bool bNew = true;
		FireData newFire;
		while (bNew)
		{
			bNew = false;
			newFire = { 14 - rand() % 12, 20, rand() % 4, 0.0f };
			for (auto data : m_vecRndFire)
			{
				if (newFire.row == data.row)
					bNew = true;
			}
		}
		m_vecRndFire.push_back(newFire);
	}
	else
	{
		for (int i = 0; i < min(5, 72 - m_cFire); i++)
		{
			bool bNew = true;
			FireData newFire;
			while (bNew)
			{
				bNew = false;
				newFire = { 14 - rand() % 12, 21 + rand() % 6, rand() % 4, 0.0f };
				for (auto data : m_vecRndFire)
				{
					if (newFire.row == data.row && newFire.col == data.col)
						bNew = true;
				}
			}
			m_cFire++;
			m_vecRndFire.push_back(newFire);

			if (!bGameOver)
			{
				bGameOver = true;
				iDeathReason = 0;
			}
		}
	}
}

void Fire::Del()
{
	if (m_cFire > m_iMinFire)
	{
		m_cFire--;
		auto iter = m_vecRndFire.begin();
		int cFire = m_vecRndFire.size();
		m_vecRndFire.erase(iter + rand() % cFire);
	}
}

void Fire::Set(int cFire)
{
	m_iMinFire = cFire;
	if (cFire > m_cFire)
	{
		for (int i = m_cFire; i < cFire; i++)
			Add();
	}
}


//
//Smoke
//

Smoke::Smoke()
{
	SetRange(0, -fSpacing / 2, fSpacing, fSpacing / 2);
	AddTexFromResc(GetMiscDLLHandle(), MAKEINTRESOURCE(IDR_ITEM1), 64, 64);
	AddTexRect(0, 32, 16, 48);
	AddTexRect(16, 32, 32, 48);
	AddTexRect(32, 32, 48, 48);
	AddTexRect(48, 32, 64, 48);

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

	m_iColBeg = 19;
	m_iColEnd = 26;
	m_iRowBeg = 4;
	m_iRowEnd = 15;
	m_cSmoke = maxSmoke;

	m_bOn = false;
}

Smoke::~Smoke()
{
	DXObject::~DXObject();
}

void Smoke::Open(int iColBeg, int iColEnd, int cSmoke)
{
	m_bOn = true;

	m_iColBeg = iColBeg;
	m_iColEnd = iColEnd;
	m_iRowBeg = 4;
	m_iRowEnd = 15;
	m_cSmoke = min(cSmoke, maxSmoke);

	while ((int)m_vecSmoke.size() < m_cSmoke)
	{
		SmokeData newSmoke;
		newSmoke.x = (m_iColBeg + (m_iColEnd - m_iColBeg) * (rand() % 100) / 100.0f) * fSpacing;
		newSmoke.y = (m_iRowBeg + (m_iRowEnd - m_iRowBeg) * (rand() % 100) / 100.0f) * fSpacing;
		newSmoke.fSpeed = 0.02f + 0.02f * (rand() % 1000) / 1000.0f;
		newSmoke.fTime = -8.0f * (float)fBaseSPB * (rand() % 100) / 100.0f;
		m_vecSmoke.push_back(newSmoke);
	}
}

void Smoke::Close()
{
	m_bOn = false;

	auto iter = m_vecSmoke.begin();
	while (iter != m_vecSmoke.end())
	{
		if (iter->fTime <= 0.0f)
			iter = m_vecSmoke.erase(iter);
		else
			iter++;
	}
}

BOOL Smoke::Update(double fTime, float fElapsedTime)
{
	auto iter = m_vecSmoke.begin();
	while (iter != m_vecSmoke.end())
	{
		iter->fTime += fElapsedTime;
		if (iter->fTime > 4 * fBaseSPB)
			iter = m_vecSmoke.erase(iter);
		else
			iter++;
	}

	if (m_bOn)
	{
		while ((int)m_vecSmoke.size() < m_cSmoke)
		{
			SmokeData newSmoke;
			newSmoke.x = (m_iColBeg + (m_iColEnd - m_iColBeg) * (rand() % 100) / 100.0f) * fSpacing;
			newSmoke.y = (m_iRowBeg + (m_iRowEnd - m_iRowBeg) * (rand() % 100) / 100.0f) * fSpacing;
			newSmoke.fSpeed = 0.02f + 0.02f * (rand() % 1000) / 1000.0f;
			newSmoke.fTime = -8.0f * (float)fBaseSPB * (rand() % 100) / 100.0f;
			m_vecSmoke.push_back(newSmoke);
		}
	}

	return TRUE;
}

BOOL Smoke::Render(double fTime, float fElapsedTime)
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

	for (auto smoke : m_vecSmoke)
	{
		if (smoke.fTime >= 0.0f)
		{
			int iTex = (int)(smoke.fTime / fBaseSPB);

			if (iTex < 4)
			{
				float x = smoke.x;
				float y = smoke.y - smoke.fSpeed * smoke.fTime;

				D3DXMatrixTranslation(&matWorld, x, y, 0.0f);
				m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * iTex, 2);
			}
		}
	}

	m_pd3dDevice->SetTexture(0, NULL);
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return TRUE;
}

//
//Judge
//

Judge::Judge()
{
	m_pd3dDevice = DXUTGetD3DDevice();
	m_fScale = GetSettings().m_fScale;
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);
}

Judge::~Judge()
{
	SAFE_RELEASE(m_pFont);
}

void Judge::OnResetDevice()
{
	SAFE_RELEASE(m_pFont);
	m_pd3dDevice = DXUTGetD3DDevice();
	m_fScale = GetSettings().m_fScale;
	D3DXCreateFont(m_pd3dDevice, (int)(40.0f * m_fScale), 0, 0, 1000, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, TEXT("Small Fonts"), &m_pFont);
}

void Judge::OnLostDevice()
{
	SAFE_RELEASE(m_pFont);
}

BOOL Judge::Update(double fTime, float fElapsedTime)
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

	return TRUE;
}

BOOL Judge::Render(double fTime, float fElapsedTime)
{
	RECT rcJudge = { 0,0,0,0 };
	DWORD colorJudge = D3DCOLOR_ARGB(255,255,255,255);
	for (auto data : m_vecText)
	{
		int iAlpha = 255 - (int)(255.0f * data.fTime / (2 * fSPB));

		switch (data.iType)
		{
		case PERFECT:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 255, 0);
			break;
		case GOOD:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 0, 255, 0);
			break;
		case BAD:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 255, 255);
			break;
		case MISS:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 0, 0);
			break;
		case POISON:
			colorJudge = D3DCOLOR_ARGB(255, 0, 127, 0);
			break;
		case SMOKE:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 127, 127, 127);
			break;
		case FIRE:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 80, 40);
			break;
		case HIDDEN:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 127, 127, 127);
			break;
		case COMBO:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 255, 0);
			break;
		case NOEFFECT:
			colorJudge = D3DCOLOR_ARGB(iAlpha, 255, 255, 127);
			break;
		}

		rcJudge.left = (int)((data.x - 100.0f) * m_fScale);
		rcJudge.right = (int)((data.x + 100.0f) * m_fScale);
		rcJudge.top = (int)((data.y - 20.0f - data.fTime * 0.1f) * m_fScale);
		rcJudge.bottom = (int)((data.y + 20.0f - data.fTime * 0.1f) * m_fScale);

		m_pFont->DrawText(NULL, data.szText, -1, &rcJudge, DT_CENTER | DT_VCENTER, colorJudge);
	}

	return TRUE;
}

void Judge::Add(int iType, float x, float y)
{
	TextInfo newText;
	newText = { iType, x, y, 0.0f, L"" };

	switch (iType)
	{
	case PERFECT:
		wcscpy_s(newText.szText, L"Perfect!");
		break;
	case GOOD:
		wcscpy_s(newText.szText, L"Good!");
		break;
	case BAD:
		wcscpy_s(newText.szText, L"Bad");
		break;
	case MISS:
		wcscpy_s(newText.szText, L"Miss");
		break;
	case POISON:
		wcscpy_s(newText.szText, L"Poisoned");
		break;
	case SMOKE:
		wcscpy_s(newText.szText, L"Smoke!");
		break;
	case FIRE:
		wcscpy_s(newText.szText, L"Fire!");
		break;
	case HIDDEN:
		wcscpy_s(newText.szText, L"Hidden!");
		break;
	case NOEFFECT:
		wcscpy_s(newText.szText, L"No Effect!");
		break;
	}

	m_vecText.push_back(newText);

	if (!bGameOver)
	{
		int iBase = 0;
		switch (iType)
		{
		case PERFECT:
			pSE0->Play(0);
			iBase = 200;
			iCombo++;
			iPerfect++;
			if (iCombo > 1)
			{
				TextInfo comboText = { COMBO, 23.0f * fSpacing, 3.0f * fSpacing, 0.0f , L"" };
				swprintf_s(comboText.szText, L"Combo x %d", iCombo);
				m_vecText.push_back(comboText);
			}
			break;

		case GOOD:
			pSE0->Play(1);
			iBase = 100;
			iCombo++;
			iGood++;
			if (iCombo > 1)
			{
				TextInfo comboText = { COMBO, 23.0f * fSpacing, 3.0f * fSpacing, 0.0f , L"" };
				swprintf_s(comboText.szText, L"Combo x %d", iCombo);
				m_vecText.push_back(comboText);
			}
			break;
		case BAD:
			iBase = 50;
			iCombo = 0;
			pSE0->Play(2);
			iBad++;
			break;
		case MISS:
			iCombo = 0;
			iMiss++;
			break;
		}

		if (iCombo > iMaxCombo)
			iMaxCombo = iCombo;
		if (iCombo == 0)
			iFragment = 0;

		if (iCombo / (GetDifficulty() == 3 ? 5 : 10) > iFragment)
		{
			iFragment++;
			if (rand() % 100 < (iCombo / 50 + iLuc / 3))
				iGoldFragment++;
			else
				iHopeFragment++;
		}

		int iMul = iCombo / 10 + 1;
		iScore += (iBase + iCombo) * iMul;
	}
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
	m_pGoldFragment->SetPos(fActSpacing, 2.5f * fActSpacing);

	m_pBox = new Box();
	m_pBox->Set(GAME_WIDTH / 2, GAME_HEIGHT / 2, 255, 280);

	for (int i = 0; i < 9; i++)
		m_bSoundPlay[i] = false;
}

HUD::~HUD()
{
	SAFE_DELETE(m_pHopeFragment);
	SAFE_DELETE(m_pGoldFragment);
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

		left = (int)(fSpacing * m_fScale);
		top = (int)(0.5f * fSpacing * m_fScale);
		right = (int)(3.0f * fSpacing * m_fScale);
		bottom = (int)(1.5f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"X %3d", iHopeFragment);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);

		top = (int)(2.0f * fSpacing * m_fScale);
		bottom = (int)(3.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"X %3d", iGoldFragment);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_CENTER | DT_VCENTER, WHITE);

		int iMin = (int)timeTimer / 60000;
		int iSec = (int)timeTimer / 1000 - iMin * 60;
		int iMil = (int)timeTimer % 1000 / 10;

		left = (int)(0.5f * fSpacing * m_fScale);
		top = (int)(13.9f * fSpacing * m_fScale);
		right = (int)(3.5f * fSpacing * m_fScale);
		bottom = (int)(17.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"Time: \n%d'%02d\"%02d", iMin, iSec, iMil);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);

		left = (int)(0.5f * fSpacing * m_fScale);
		top = (int)(15.3f * fSpacing * m_fScale);
		right = (int)(3.5f * fSpacing * m_fScale);
		bottom = (int)(17.0f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);
		swprintf_s(szBuffer, L"Score: \n%d", iScore);
		m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, WHITE);

		top = (int)(14.1f * fSpacing * m_fScale);
		bottom = (int)(17.0f * fSpacing * m_fScale);

		if (timeHidden > 0.0f)
		{
			int iSec = (int)timeHidden / 1000;
			int iMil = (int)timeHidden % 1000 / 10;
			color = D3DCOLOR_ARGB(255, 191, 191, 191);

			left = (int)(16.5f * fSpacing * m_fScale);
			right = (int)(19.5f * fSpacing * m_fScale);
			SetRect(&rcText, left, top, right, bottom);
			swprintf_s(szBuffer, L"Hidden: \n%d\"%02d", iSec, iMil);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, color);
		}

		if (timeSmoke > 0.0f)
		{
			int iSec = (int)timeSmoke / 1000;
			int iMil = (int)timeSmoke % 1000 / 10;
			color = D3DCOLOR_ARGB(255, 191, 191, 191);

			left = (int)(21.0f * fSpacing * m_fScale);
			right = (int)(24.0f * fSpacing * m_fScale);
			SetRect(&rcText, left, top, right, bottom);
			swprintf_s(szBuffer, L"Smoke: \n%d\"%02d", iSec, iMil);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, color);
		}

		if (timeFire > 0.0f)
		{
			int iSec = (int)timeFire / 1000;
			int iMil = (int)timeFire % 1000 / 10;
			color = D3DCOLOR_ARGB(255, 255, 80, 40);

			left = (int)(25.5f * fSpacing * m_fScale);
			right = (int)(28.5f * fSpacing * m_fScale);
			SetRect(&rcText, left, top, right, bottom);
			swprintf_s(szBuffer, L"Fire: \n%d\"%02d", iSec, iMil);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, color);
		}

		if (timePoison > 0.0f)
		{
			int iSec = (int)timePoison / 1000;
			int iMil = (int)timePoison % 1000 / 10;
			color = D3DCOLOR_ARGB(255, 80, 255, 40);

			top = (int)(1.4f * fSpacing * m_fScale);
			bottom = (int)(5.0f * fSpacing * m_fScale);
			left = (int)(25.1f * fSpacing * m_fScale);
			right = (int)(28.5f * fSpacing * m_fScale);
			SetRect(&rcText, left, top, right, bottom);
			swprintf_s(szBuffer, L"Poison: \n%d\"%02d", iSec, iMil);
			m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, color);
		}
		else if (timePoison > -2.4f * fBaseSPB)
		{
			if ((int)(2.0f * timePoison / fBaseSPB) % 2 == 0)
			{
				color = D3DCOLOR_ARGB(255, 255, 255, 127);

				top = (int)(1.7f * fSpacing * m_fScale);
				bottom = (int)(5.0f * fSpacing * m_fScale);
				left = (int)(24.8f * fSpacing * m_fScale);
				right = (int)(28.5f * fSpacing * m_fScale);
				SetRect(&rcText, left, top, right, bottom);
				swprintf_s(szBuffer, L"Recovered");
				m_pFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_TOP, color);
			}
		}
		
		left = (int)(0.0f * fSpacing * m_fScale);
		right = (int)(30.0f * fSpacing * m_fScale);
		top = (int)(15.5f * fSpacing * m_fScale);
		bottom = (int)(16.5f * fSpacing * m_fScale);
		SetRect(&rcText, left, top, right, bottom);

		if (timeReDecorate > 0.0f)
		{
			swprintf_s(szBuffer, L"Map Detail: %d", iDecorate);
			int iAlpha = (int)(timeReDecorate / 3000.0f * 255.0f);
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
					swprintf_s(szBuffer, L"Komaeda is Killed by Fire");
				else if (iDeathReason == 1)
					swprintf_s(szBuffer, L"Komaeda is Killed by Poison");
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
						pSE1->Play(0);
						m_bSoundPlay[1] = true;
					}
					SetGridRect(&rcText, 11.5f, 4.0f, 18.5f, 5.0f, fActSpacing);
					swprintf_s(szBuffer, L"总得分");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					swprintf_s(szBuffer, L"%d", iScore);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}

				if (timeCount >= 1000.0f)
				{
					if (!m_bSoundPlay[2])
					{
						pSE1->Play(0);
						m_bSoundPlay[2] = true;
					}
					SetGridRect(&rcText, 11.5f, 5.0f, 18.5f, 6.0f, fActSpacing);
					swprintf_s(szBuffer, L"最高连击");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					swprintf_s(szBuffer, L"%d", iMaxCombo);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}

				if (timeCount >= 1500.0f)
				{
					if (!m_bSoundPlay[3])
					{
						pSE1->Play(0);
						m_bSoundPlay[3] = true;
					}
					SetGridRect(&rcText, 11.5f, 6.0f, 18.5f, 7.0f, fActSpacing);
					swprintf_s(szBuffer, L"希望碎片");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					IsDebugMode() ?
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iHopeFragment, 0) :
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iHopeFragment - iHopeFragment, iHopeFragment);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}

				if (timeCount >= 2000.0f)
				{
					if (!m_bSoundPlay[4])
					{
						pSE1->Play(0);
						m_bSoundPlay[4] = true;
					}
					SetGridRect(&rcText, 11.5f, 7.0f, 18.5f, 8.0f, fActSpacing);
					swprintf_s(szBuffer, L"金色碎片");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					IsDebugMode() ?
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iGoldFragment, 0) :
						swprintf_s(szBuffer, L"%d + %d", GetSaveData().m_iGoldFragment - iGoldFragment, iGoldFragment);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}

				if (timeCount >= 2500.0f)
				{
					if (!m_bSoundPlay[5])
					{
						pSE1->Play(0);
						m_bSoundPlay[5] = true;
					}
					SetGridRect(&rcText, 11.5f, 9.0f, 18.5f, 10.0f, fActSpacing);
					swprintf_s(szBuffer, L"Perfect");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					swprintf_s(szBuffer, L"%d", iPerfect);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}
				if (timeCount >= 3000.0f)
				{
					if (!m_bSoundPlay[6])
					{
						pSE1->Play(0);
						m_bSoundPlay[6] = true;
					}
					SetGridRect(&rcText, 11.5f, 10.0f, 18.5f, 11.0f, fActSpacing);
					swprintf_s(szBuffer, L"Good");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					swprintf_s(szBuffer, L"%d", iGood);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}
				if (timeCount >= 3500.0f)
				{
					if (!m_bSoundPlay[7])
					{
						pSE1->Play(0);
						m_bSoundPlay[7] = true;
					}
					SetGridRect(&rcText, 11.5f, 11.0f, 18.5f, 12.0f, fActSpacing);
					swprintf_s(szBuffer, L"Bad");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					swprintf_s(szBuffer, L"%d", iBad);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
				}
				if (timeCount >= 4000.0f)
				{
					if (!m_bSoundPlay[8])
					{
						pSE1->Play(0);
						m_bSoundPlay[8] = true;
					}
					SetGridRect(&rcText, 11.5f, 12.0f, 18.5f, 13.0f, fActSpacing);
					swprintf_s(szBuffer, L"Miss");
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_LEFT | DT_VCENTER, BLACK);
					swprintf_s(szBuffer, L"%d", iMiss);
					m_pCNFont->DrawText(NULL, szBuffer, -1, &rcText, DT_RIGHT | DT_VCENTER, BLACK);
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
					GetSaveData().m_iStory = 2;
					SetScene(MIDSAVE);
					SetCurtain(127, 255, 1500.0f, GameStart);
				}
			}
		}
	}

	return TRUE;
}

///////////////////////////////////
//
//Scene
//
///////////////////////////////////

SceneShooter::SceneShooter()
{
	pMap = NULL;
	pPlayer = NULL;
	pBeats = NULL;
	pScanner = NULL;
	pFire = NULL;
	pSmoke = NULL;
	pJudge = NULL;
	pHUD = NULL;

	pBGM = NULL;
	pFire = NULL;
	pSmoke = NULL;
	pJudge = NULL;

	//Story

	pTalkBox = NULL;
	pFont = NULL;
	pSFont = NULL;
	pLFont = NULL;
	pPrepare = NULL;
}

SceneShooter::~SceneShooter()
{
	End();
}

void SceneShooter::Init()
{
	//
	//Player Character
	//

	iCharaID = IsStoryMode() ? IDC_HINATA : GetSaveData().m_iCharaID;
	iStr = GetCharaSkillLevel(iCharaID, 0);
	iAgi = GetCharaSkillLevel(iCharaID, 1);
	iEnd = GetCharaSkillLevel(iCharaID, 2);
	iInt = GetCharaSkillLevel(iCharaID, 3);
	iLuc = GetCharaSkillLevel(iCharaID, 4);

	//
	//Parameters decided by Difficulty
	//

	bPoison = true;
	bDebuff = true;

	int iDifficulty = GetDifficulty();
	switch (iDifficulty)
	{
	case 0:
		fRate = 2.0f;
		iPoison = 10;
		iDebuff = 20;
		timeDebuff = 5000.0f;
		timeRecovery = 20000.0f;
		timeTotal = 100000.0f;
		bAutoDebuff = false;
		bCombine = false;	
		break;

	case 1:
		fRate = 1.0f;
		iPoison = 8;
		iDebuff = 16;
		timeDebuff = 10000.0f;
		timeRecovery = 20000.0f;
		timeTotal = 120000.0f;
		bAutoDebuff = true;
		bCombine = false;
		break;

	case 2:
		fRate = 0.5f;
		iPoison = 6;
		iDebuff = 12;
		timeDebuff = 16000.0f;
		timeRecovery = 24000.0f;
		timeTotal = 120000.0f;
		bAutoDebuff = true;
		bCombine = false;
		break;

	case 3:
		fRate = 0.5f;
		iPoison = 4;
		iDebuff = 8;
		timeDebuff = 24000.0f;
		timeRecovery = 30000.0f;
		timeTotal = 150000.0f;
		bAutoDebuff = true;
		bCombine = true;
		break;
	}

	if (IsDebugMode())
	{
		timeTotal = 80000.0f;
		bDebuff = false;
	}

	timeDebuff *= (1.0f - (float)iInt / 36.0f);
	timeRecovery *= (1.0f - (float)iEnd / 36.0f);

	//
	//Setting Paramters
	//

	fScale = GetSettings().m_fScale;

	iDecorate = GetSettings().m_iSTDecorate;
	iScanAlpha = GetSettings().m_iSTScanAlpha;
	iAdjust = GetSettings().m_iSTAdjust;
	timeAdjust = (float)iAdjust;
	
	//
	//Caluculated Parameters
	//

	fActSpacing = fSpacing * fScale;

	fBaseSPB = 60000.0 / fBPM;
	fSPB = fBaseSPB * fRate;
	fPreTime = 4 * fSPB;

	fExpRange = 0.5f * iAgi / 18.0f * fSpacing;
	
	//
	//Timer & Counters & Results
	//

	timeSmoke = 0.0f;
	timeHidden = 0.0f;
	timeFire = 0.0f;
	timePoison = -2.5f * (float)fBaseSPB;
	
	iBeat = (int)(2 * fBaseSPB / fSPB) + 4;
	timeBeat = fSPB * fRate;
	timeTimer = timeTotal;
	iFragment = 0;

	timeOver = 0.0f;
	timeComplete = 0.0f;
	timeCount = 0.0f;
	
	vecPitcher.clear();

	//Result

	iDeathReason = 0;
	bGameOver = false;
	bComplete = false;
	bCount = false;
	iPerfect = 0;
	iGood = 0;
	iBad = 0;
	iMiss = 0;
	iCombo = 0;
	iMaxCombo = 0;
	iScore = 0;
	iHopeFragment = 0;
	iGoldFragment = 0;
	
	pMap = new Map();
	pPlayer = new Player();	
	pBeats = new Beats();	
	pScanner = new Scanner();
	pFire = new Fire();
	pSmoke = new Smoke();
	pJudge = new Judge();
	pHUD = new HUD();
	
	pBGM = new Sound();
	pBGM->Register(ID_BGM_SHOOTER, ID_SUBMIX_BACKGROUND);
	pBGM->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_EKOROSIA, L"MP3", 255);
	
	pSE0 = new Sound();
	pSE0->Register(ID_SE_ST0, ID_SUBMIX_SOUNDEFFECT);
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_BEAT01, L"WAVE");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_BEAT02, L"WAVE");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_BEAT03, L"WAVE");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_CLEAR, L"MP3");
	pSE0->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_FAIL, L"MP3");

	pSE1 = new Sound();
	pSE1->Register(ID_SE_ST1, ID_SUBMIX_SOUNDEFFECT, TRUE);
	pSE1->AddSoundFromRsrc(GetSoundDLLHandle(), IDR_SCORE, L"WAVE");
	
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

void SceneShooter::End()
{
	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pPrepare);
	SAFE_DELETE(pSEText);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	vecPitcher.clear();

	SAFE_DELETE(pHUD);
	SAFE_DELETE(pJudge);
	SAFE_DELETE(pSmoke);
	SAFE_DELETE(pFire);
	SAFE_DELETE(pScanner);
	SAFE_DELETE(pBeats);
	SAFE_DELETE(pPlayer);
	SAFE_DELETE(pMap);
	SAFE_DELETE(pBGM);
	SAFE_DELETE(pSE0);
	SAFE_DELETE(pSE1);
}

void SceneShooter::Reset()
{
	SAFE_DELETE(pTalkBox);
	SAFE_DELETE(pPrepare);
	SAFE_DELETE(pSEText);
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	vecPitcher.clear();

	SAFE_DELETE(pHUD);
	SAFE_DELETE(pJudge);
	SAFE_DELETE(pSmoke);
	SAFE_DELETE(pFire);
	SAFE_DELETE(pScanner);
	SAFE_DELETE(pBeats);
	SAFE_DELETE(pPlayer);

	pBGM->Stop();

	//
	//Timer & Counters & Results
	//

	timeSmoke = 0.0f;
	timeHidden = 0.0f;
	timeFire = 0.0f;
	timePoison = -2.5f * (float)fBaseSPB;

	iBeat = (int)(2 * fBaseSPB / fSPB) + 4;
	timeBeat = fSPB * fRate;
	timeTimer = timeTotal;
	iFragment = 0;

	timeOver = 0.0f;
	timeComplete = 0.0f;
	timeCount = 0.0f;

	vecPitcher.clear();

	//Result

	iDeathReason = 0;
	bGameOver = false;
	bComplete = false;
	bCount = false;
	iPerfect = 0;
	iGood = 0;
	iBad = 0;
	iMiss = 0;
	iCombo = 0;
	iMaxCombo = 0;
	iScore = 0;
	iHopeFragment = 0;
	iGoldFragment = 0;

	//Pointers

	pPlayer = new Player();
	pBeats = new Beats();
	pScanner = new Scanner();
	pFire = new Fire();
	pSmoke = new Smoke();
	pJudge = new Judge();
	pHUD = new HUD();

	pBGM->Play(0);
}

void SceneShooter::OnResetDevice()
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

	if (NULL != pJudge)
		pJudge->OnResetDevice();

	if (NULL != pHUD)
		pHUD->OnResetDevice();
}

void SceneShooter::OnLostDevice()
{
	SAFE_RELEASE(pFont);
	SAFE_RELEASE(pSFont);
	SAFE_RELEASE(pLFont);

	if (NULL != pJudge)
		pJudge->OnLostDevice();

	if (NULL != pHUD)
		pHUD->OnLostDevice();
}

void SceneShooter::Update(double fTime, float fElapsedTime)
{
	GetInput();

	static bool bEscUp = false;
	static bool bInsUp = false;
	static bool bDelUp = false;
	static bool bHomeUp = false;
	static bool bEndUp = false;
	static bool bPGUPUp = false;
	static bool bPGDNUp = false;

	pPlayer->Update(fTime, fElapsedTime);

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

	timeBeat += fElapsedTime;
	timeTimer -= fElapsedTime;

	if (timeTimer <= 0.0f)
	{
		timeTimer = 0.0f;
		if (!bGameOver && !bComplete)
		{
			bComplete = true;

			if (!IsDebugMode())
			{
				GetSaveData().m_iHopeFragment = min(999, GetSaveData().m_iHopeFragment + iHopeFragment);
				GetSaveData().m_iGoldFragment = min(999, GetSaveData().m_iGoldFragment + iGoldFragment);
				if (iBad + iMiss == 0)
					GetSaveData().m_iNoMiss[0][GetDifficulty()] = 1;
				CheckSTScore(GetDifficulty(), iScore, iMaxCombo, iPerfect, iGood, iBad, iMiss);
				SaveDataUpdated();
			}
		}
	}

	if (iBeat < (int)((timeTotal - fBegTime) / fSPB))
	{
		while (timeBeat >= fSPB)
		{
			pBeats->AddBeat(iBeat++);
			timeBeat -= fSPB;
		}
	}

	if (timeHidden <= 0.0f)
		timeHidden = 0.0f;
	else
		timeHidden -= fElapsedTime;

	if (timeSmoke <= 0.0f)
	{
		timeSmoke = 0.0f;
		pSmoke->Close();
	}
	else
		timeSmoke -= fElapsedTime;

	if (timeFire <= 0.0f)
	{
		timeFire = 0.0f;
		pFire->Set(0);
	}
	else
		timeFire -= fElapsedTime;

	if (timePoison <= -2.5f * (float)fBaseSPB)
	{
		timePoison = -2.5f * (float)fBaseSPB;
	}
	else
	{
		if (!bGameOver)
			timePoison -= fElapsedTime;
	}

	pBeats->Update(fTime, fElapsedTime);
	pFire->Update(fTime, fElapsedTime);
	pSmoke->Update(fTime, fElapsedTime);
	pJudge->Update(fTime, fElapsedTime);
	pHUD->Update(fTime, fElapsedTime);

	//Decorate, Alpha and Adjust
	
	timeReDecorate = max(0.0f, timeReDecorate - fElapsedTime);
	timeReAlpha = max(0.0f, timeReAlpha - fElapsedTime);
	timeReAdjust = max(0.0f, timeReAdjust - fElapsedTime);

	if (IsKeyDown(DIK_INSERT))
	{
		if (timeReDecorate <= 2500.0f)
			bInsUp = true;

		if ((bInsUp))
		{
			if (GetSettings().m_iSTDecorate < 2)
			{
				GetSettings().m_iSTDecorate++;
				SetShooterDecorate(GetSettings().m_iSTDecorate);
				SaveSettings();
				timeReDecorate = 3000.0f;
				timeReAlpha = 0.0f;
				timeReAdjust = 0.0f;
			}
		}
		bInsUp = false;
	}
	else
		bInsUp = true;

	if (IsKeyDown(DIK_DELETE))
	{
		if (timeReDecorate <= 2500.0f)
			bDelUp = true;

		if ((bDelUp))
		{
			if (GetSettings().m_iSTDecorate > 0)
			{
				GetSettings().m_iSTDecorate--;
				SetShooterDecorate(GetSettings().m_iSTDecorate);
				SaveSettings();
				timeReDecorate = 3000.0f;
				timeReAlpha = 0.0f;
				timeReAdjust = 0.0f;
			}
		}
		bDelUp = false;
	}
	else
		bDelUp = true;

	if (IsKeyDown(DIK_HOME))
	{
		if (timeReAlpha <= 2950.0f)
			bHomeUp = true;

		if ((bHomeUp))
		{
			GetSettings().m_iSTScanAlpha = min(255, GetSettings().m_iSTScanAlpha + 1);
			SetShooterScanAlpha(GetSettings().m_iSTScanAlpha);
			SaveSettings();
			timeReAlpha = 3000.0f;
			timeReDecorate = 0.0f;
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
			GetSettings().m_iSTScanAlpha = max(0, GetSettings().m_iSTScanAlpha - 1);
			SetShooterScanAlpha(GetSettings().m_iSTScanAlpha);
			SaveSettings();
			timeReAlpha = 3000.0f;
			timeReDecorate = 0.0f;
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
			GetSettings().m_iSTAdjust = min(127, GetSettings().m_iSTAdjust + 1);
			SetShooterAdjust(GetSettings().m_iSTAdjust);
			SaveSettings();
			timeReAdjust = 3000.0f;
			timeReDecorate = 0.0f;
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
			GetSettings().m_iSTAdjust = max(-127, GetSettings().m_iSTAdjust - 1);
			SetShooterAdjust(GetSettings().m_iSTAdjust);
			SaveSettings();
			timeReAdjust = 3000.0f;
			timeReDecorate = 0.0f;
			timeReAlpha = 0.0f;
		}
		bPGDNUp = false;
	}
	else
		bPGDNUp = true;


	//Game Control
	
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

void SceneShooter::Render(double fTime, float fElapsedTime)
{
	float fCameraX = GAME_WIDTH / 2;
	float fCameraY = GAME_HEIGHT / 2;
	SetCameraPos(fCameraX, fCameraY);
	SetView();
	SetProj(NULL, GAME_WIDTH, GAME_HEIGHT);
	SetViewPort(0.0f, 0.0f, 0.0f, 0.0f);
	pMap->Render(fTime, fElapsedTime);
	pPlayer->Render(fTime, fElapsedTime);
	pScanner->Render(fTime, fElapsedTime);
	pFire->Render(fTime, fElapsedTime);

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

	pBeats->Render(fTime, fElapsedTime);
	pSmoke->Render(fTime, fElapsedTime);
	pJudge->Render(fTime, fElapsedTime);
	pHUD->Render(fTime, fElapsedTime);
}