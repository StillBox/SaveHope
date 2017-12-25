#include <Windows.h>
#include <vector>
#include "CharaData.h"
#include "resource.h"

#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

//Get Character Count of each Group
EXPORT int GetCharaCount(int iGroup);

//Get Character Information
EXPORT int GetCharaID(int iChara, int iGroup);

EXPORT int GetCharaHeight(int iChara, int iGroup = -1);
EXPORT int GetCharaWeight(int iChara, int iGroup = -1);
EXPORT int GetCharaBust(int iChara, int iGroup = -1);

EXPORT bool GetCharaName(LPWSTR szDest, int iChara, int iGroup = -1);
EXPORT bool GetCharaKata(LPWSTR szDest, int iChara, int iGroup = -1);
EXPORT bool GetCharaTitle(LPWSTR szDest, int iChara, int iGroup = -1);
EXPORT bool GetCharaBirth(LPWSTR szDest, int iChara, int iGroup = -1);
EXPORT bool GetCharaBlood(LPWSTR szDest, int iChara, int iGroup = -1);
EXPORT bool GetCharaStage(LPWSTR szDest, int iChara, int iGroup = -1);
EXPORT bool GetCharaDegree(LPWSTR szDest, int iChara, int iGroup = -1);

struct CharaData
{
	int ID;
	LPWSTR szName;
	LPWSTR szKata;
	LPWSTR szTitle;
	LPWSTR szBirth;
	LPWSTR szBlood;
	LPWSTR szDegree;
	int iHeight;
	int iWeight;
	int iBust;
	std::vector<int> iStage;
};

LPWSTR szStage[7] =
{
	L"弹丸论破 / Zero",								//0
	L"弹丸论破：希望的学园和绝望高中生",			//1
	L"超级弹丸论破2：再见绝望学园",					//2
	L"弹丸论破3 - The End of 希望之峰学园 -",		//3
	L"绝对绝望少女",								//4
	L"弹丸论破2.5：狛枝凪斗与世界的破坏者",			//5
	L"新·弹丸论破V3：大家的互相残杀新学期"			//6
};

std::vector<CharaData> vecChara =
{
	{ 100, L"苗木 诚",			L"ナエギ　マコト",			L"超高校级的「幸运」",				L"2月5日",		L"A型",		L"希望之峰学园第78期生\n未来机关成员",	160,	52,		75,		{ 0,1,2,3,4,5 }	},
	{ 199, L"江之岛 盾子",		L"エノシマ　ジュンコ",		L"超高校级的「绝望」",				L"12月24日",	L"AB型",	L"希望之峰学园第78期生",				169,	44,		90,		{ 0,1,2,3,4}	},
	{ 200, L"日向 创",			L"ヒナタ　ハジメ",			L"超高校级的「？？？」",			L"1月1日",		L"A型",		L"希望之峰学园第77期\n预备学科学生",	179,	67,		91,		{ 2,3,5 }	},
	{ 201, L"七海 千秋",		L"ナナミ　チアキ",			L"超高校级的「游戏玩家」",			L"3月14日",		L"O型",		L"希望之峰学园第77期生",				160,	46,		88,		{ 2,3 }		},
	{ 202, L"狛枝 凪斗",		L"コマエダ　ナギト",		L"超高校级的「幸运」",				L"4月28日",		L"O型",		L"希望之峰学园第77期生",				180,	65,		84,		{ 2,3,4,5 }	},
	{ 203, L"？？？",			L"？？？",					L"超高校级的「欺诈师」",			L"??月??日",	L"?型",		L"希望之峰学园第77期生",				185,	130,	128,	{ 2,3,5 }	},
	{ 204, L"田中 眼蛇梦",		L"タナカ　ガンダム",		L"超高校级的「饲育委员」",			L"12月14日",	L"B型",		L"希望之峰学园第77期生",				182,	74,		93,		{ 2,3,5 }	},
	{ 205, L"左右田 和一",		L"ソウダ　カズイチ",		L"超高校级的「机械师」",			L"6月29日",		L"A型",		L"希望之峰学园第77期生",				172,	64,		86,		{ 2,3,5 }	},
	{ 206, L"花村 辉辉",		L"ハナムラ　テルテル",		L"超高校级的「料理人」",			L"9月2日",		L"A型",		L"希望之峰学园第77期生",				133,	69,		88,		{ 2,3,5 }	},
	{ 207, L"二大 猫丸",		L"ニダイ　ハジメ",			L"超高校级的「社团经理」",			L"2月22日",		L"A型",		L"希望之峰学园第77期生",				198,	122,	122,	{ 2,3,5 }	},
	{ 208, L"九头龙 冬彦",		L"クズリュウ　フユヒコ",	L"超高校级的「极道」",				L"8月16日",		L"AB型",	L"希望之峰学园第77期生",				157,	43,		73,		{ 2,3,5 }	},
	{ 209, L"终里 赤音",		L"オワリ　アカネ",			L"超高校级的「体操选手」",			L"7月15日",		L"B型",		L"希望之峰学园第77期生",				176,	56,		93,		{ 2,3,5 }	},
	{ 210, L"索尼娅·内瓦曼",	L"ソニア·ネヴァ-マィンド", L"超高校级的「王女」",				L"10月13日",	L"A型",		L"希望之峰学园第77期生",				174,	50,		83,		{ 2,3,5 }	},
	{ 211, L"小泉 真昼",		L"コイズミ　マヒル",		L"超高校级的「摄影师」",			L"4月24日",		L"A型",		L"希望之峰学园第77期生",				165,	46,		77,		{ 2,3,5 }	},
	{ 212, L"罪木 蜜柑",		L"ツミキ　ミカン",			L"超高校级的「保健委员」",			L"5月12日",		L"A型",		L"希望之峰学园第77期生",				165,	57,		89,		{ 2,3,5 }	},
	{ 213, L"澪田 唯吹",		L"ミオダ　イブキ",			L"超高校级的「轻音部」",			L"11月27日",	L"AB型",	L"希望之峰学园第77期生",				164,	42,		76,		{ 2,3,5 }	},
	{ 214, L"西园寺 日寄子",	L"サイオンジ　ヒヨコ",		L"超高校级的「日本舞蹈家」",		L"3月9日",		L"B型",		L"希望之峰学园第77期生",				130,	31,		64,		{ 2,3,5 }	},
	{ 215, L"边谷山 佩子",		L"ペコヤマ　ペコ",			L"超高校级的「剑道家」",			L"6月30日",		L"O型",		L"希望之峰学园第77期生",				172,	51,		85,		{ 2,3,5 }	},
	{ 300, L"最原 终一",		L"サイハラ　シュウイチ",	L"超高校级的「侦探(？)」",			L"9月7日",		L"AB型",	L"才囚学园学生",						171,	58,		80,		{ 6 }		},
	{ 301, L"王马 小吉",		L"オウマ　コキチ",			L"超高校级的「总统(？)」",			L"6月21日",		L"A型",		L"才囚学园学生",						156,	44,		70,		{ 6 }		},
	{ 900, L"黑白熊",			L"モノクマ",				L"希望之峰学园校长(自称)",			L"??月??日",	L"?型",		L"亦为才囚学园管理人(自称)",			0,		0,		0,		{ 1,2,3,4,6 } },
	{ 901, L"兔美",				L"ウサミ",					L"希望之峰学园教师(自称)",			L"??月??日",	L"?型",		L"未来机关所属监视者",					0,		0,		0,		{ 2 }		},
	{ 902, L"莫诺美",			L"モノミ",					L"希望之峰学园教师(改造后)",		L"??月??日",	L"?型",		L"未来机关所属监视者",					0,		0,		0,		{ 2 }		},
	{ 903, L"Alter Ego",		L"アルター　エゴ",			L"人工智能",						L"??月??日",	L"?型",		L"未来机关成员",						0,		0,		0,		{ 1,2,4 } },
	{ 942, L"箱子",				L"ハコ",					L"超高校级的「懒癌」",				L"??月??日",	L"?型",		L"绝对希望救援幕后黑手",				0,		0,		0,		{ }	},
	{ 998, L"日向 创",			L"ヒナタ　ハジメ",			L"超高校级的「希望」",				L"1月1日",		L"A型",		L"希望之峰学园第77期\n预备学科学生",	179,	67,		91,		{ 2,3,5 }	},
	{ 999, L"神座 出流",		L"カムクラ　イズル",		L"超高校级的「希望」",				L"1月1日",		L"A型",		L"人工希望",							179,	67,		91,		{ 2,3,4,5 }	}
};

typedef std::vector<int> CharaGroup;
std::vector<CharaGroup> vecGroup =
{
	{
		100,199,
		200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
		300,301,
		900,901,902,903,942,998,999
	},
	{
		200,100,300,
		201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,
		301,
		998,999
	}
};

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	return TRUE;
}

//Get Character Count of each Group
EXPORT int GetCharaCount(int iGroup)
{
	if (iGroup >= (int)vecGroup.size())
		return 0;

	return vecGroup[iGroup].size();
}

//Get Character Information
EXPORT int GetCharaID(int iChara, int iGroup)
{
	if (iGroup >= (int)vecGroup.size())
		return 0;

	if (iChara >= (int)vecGroup[iGroup].size())
		return 0;

	return vecGroup[iGroup][iChara];
}

EXPORT int GetCharaHeight(int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
			return data.iHeight;
	}
	return 0;
}

EXPORT int GetCharaWeight(int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
			return data.iWeight;
	}
	return 0;
}

EXPORT int GetCharaBust(int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
			return data.iBust;
	}
	return 0;
}

EXPORT bool GetCharaName(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			wcscpy_s(szDest, MAX_PATH, data.szName);
			return true;
		}
	}
	return false;
}

EXPORT bool GetCharaKata(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			wcscpy_s(szDest, MAX_PATH, data.szKata);
			return true;
		}
	}
	return false;
}

EXPORT bool GetCharaTitle(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			wcscpy_s(szDest, MAX_PATH, data.szTitle);
			return true;
		}
	}
	return false;
}

EXPORT bool GetCharaBirth(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			wcscpy_s(szDest, MAX_PATH, data.szBirth);
			return true;
		}
	}
	return false;
}

EXPORT bool GetCharaBlood(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			wcscpy_s(szDest, MAX_PATH, data.szBlood);
			return true;
		}
	}
	return false;
}


EXPORT bool GetCharaStage(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			WCHAR szBuffer[MAX_PATH] = L"登场作品";
			for (auto index : data.iStage)
			{
				wcscat_s(szBuffer, L"\n\n");
				wcscat_s(szBuffer, szStage[index]);
			}
			wcscpy_s(szDest, MAX_PATH, szBuffer);
			return true;
		}
	}
	return false;
}

EXPORT bool GetCharaDegree(LPWSTR szDest, int iChara, int iGroup)
{
	int ID = (-1 == iGroup) ? iChara : GetCharaID(iChara, iGroup);
	for (auto data : vecChara)
	{
		if (ID == data.ID)
		{
			wcscpy_s(szDest, MAX_PATH, data.szDegree);
			return true;
		}
	}
	return false;
}