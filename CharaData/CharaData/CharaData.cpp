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
	L"�������� / Zero",								//0
	L"�������ƣ�ϣ����ѧ԰�;���������",			//1
	L"������������2���ټ�����ѧ԰",					//2
	L"��������3 - The End of ϣ��֮��ѧ԰ -",		//3
	L"���Ծ�����Ů",								//4
	L"��������2.5����֦�M����������ƻ���",			//5
	L"�¡���������V3����ҵĻ����ɱ��ѧ��"			//6
};

std::vector<CharaData> vecChara =
{
	{ 100, L"��ľ ��",			L"�ʥ������ޥ���",			L"����У���ġ����ˡ�",				L"2��5��",		L"A��",		L"ϣ��֮��ѧ԰��78����\nδ�����س�Ա",	160,	52,		75,		{ 0,1,2,3,4,5 }	},
	{ 199, L"��֮�� ����",		L"���Υ��ޡ������",		L"����У���ġ�������",				L"12��24��",	L"AB��",	L"ϣ��֮��ѧ԰��78����",				169,	44,		90,		{ 0,1,2,3,4}	},
	{ 200, L"���� ��",			L"�ҥʥ����ϥ���",			L"����У���ġ���������",			L"1��1��",		L"A��",		L"ϣ��֮��ѧ԰��77��\nԤ��ѧ��ѧ��",	179,	67,		91,		{ 2,3,5 }	},
	{ 201, L"�ߺ� ǧ��",		L"�ʥʥߡ�������",			L"����У���ġ���Ϸ��ҡ�",			L"3��14��",		L"O��",		L"ϣ��֮��ѧ԰��77����",				160,	46,		88,		{ 2,3 }		},
	{ 202, L"��֦ �M��",		L"���ޥ������ʥ���",		L"����У���ġ����ˡ�",				L"4��28��",		L"O��",		L"ϣ��֮��ѧ԰��77����",				180,	65,		84,		{ 2,3,4,5 }	},
	{ 203, L"������",			L"������",					L"����У���ġ���թʦ��",			L"??��??��",	L"?��",		L"ϣ��֮��ѧ԰��77����",				185,	130,	128,	{ 2,3,5 }	},
	{ 204, L"���� ������",		L"���ʥ����������",		L"����У���ġ�����ίԱ��",			L"12��14��",	L"B��",		L"ϣ��֮��ѧ԰��77����",				182,	74,		93,		{ 2,3,5 }	},
	{ 205, L"������ ��һ",		L"����������������",		L"����У���ġ���еʦ��",			L"6��29��",		L"A��",		L"ϣ��֮��ѧ԰��77����",				172,	64,		86,		{ 2,3,5 }	},
	{ 206, L"���� �Ի�",		L"�ϥʥ�顡�ƥ�ƥ�",		L"����У���ġ������ˡ�",			L"9��2��",		L"A��",		L"ϣ��֮��ѧ԰��77����",				133,	69,		88,		{ 2,3,5 }	},
	{ 207, L"���� è��",		L"�˥������ϥ���",			L"����У���ġ����ž���",			L"2��22��",		L"A��",		L"ϣ��֮��ѧ԰��77����",				198,	122,	122,	{ 2,3,5 }	},
	{ 208, L"��ͷ�� ����",		L"������奦���ե�ҥ�",	L"����У���ġ�������",				L"8��16��",		L"AB��",	L"ϣ��֮��ѧ԰��77����",				157,	43,		73,		{ 2,3,5 }	},
	{ 209, L"���� ����",		L"����ꡡ������",			L"����У���ġ����ѡ�֡�",			L"7��15��",		L"B��",		L"ϣ��֮��ѧ԰��77����",				176,	56,		93,		{ 2,3,5 }	},
	{ 210, L"����櫡�������",	L"���˥����ͥ���-�ޥ����", L"����У���ġ���Ů��",				L"10��13��",	L"A��",		L"ϣ��֮��ѧ԰��77����",				174,	50,		83,		{ 2,3,5 }	},
	{ 211, L"СȪ ����",		L"�������ߡ��ޥҥ�",		L"����У���ġ���Ӱʦ��",			L"4��24��",		L"A��",		L"ϣ��֮��ѧ԰��77����",				165,	46,		77,		{ 2,3,5 }	},
	{ 212, L"��ľ �۸�",		L"�ĥߥ����ߥ���",			L"����У���ġ�����ίԱ��",			L"5��12��",		L"A��",		L"ϣ��֮��ѧ԰��77����",				165,	57,		89,		{ 2,3,5 }	},
	{ 213, L"���� Ψ��",		L"�ߥ��������֥�",			L"����У���ġ���������",			L"11��27��",	L"AB��",	L"ϣ��֮��ѧ԰��77����",				164,	42,		76,		{ 2,3,5 }	},
	{ 214, L"��԰�� �ռ���",	L"�������󥸡��ҥ襳",		L"����У���ġ��ձ��赸�ҡ�",		L"3��9��",		L"B��",		L"ϣ��֮��ѧ԰��77����",				130,	31,		64,		{ 2,3,5 }	},
	{ 215, L"�߹�ɽ ����",		L"�ڥ���ޡ��ڥ�",			L"����У���ġ������ҡ�",			L"6��30��",		L"O��",		L"ϣ��֮��ѧ԰��77����",				172,	51,		85,		{ 2,3,5 }	},
	{ 300, L"��ԭ ��һ",		L"�����ϥ顡���奦����",	L"����У���ġ���̽(��)��",			L"9��7��",		L"AB��",	L"����ѧ԰ѧ��",						171,	58,		80,		{ 6 }		},
	{ 301, L"���� С��",		L"�����ޡ�������",			L"����У���ġ���ͳ(��)��",			L"6��21��",		L"A��",		L"����ѧ԰ѧ��",						156,	44,		70,		{ 6 }		},
	{ 900, L"�ڰ���",			L"��Υ���",				L"ϣ��֮��ѧ԰У��(�Գ�)",			L"??��??��",	L"?��",		L"��Ϊ����ѧ԰������(�Գ�)",			0,		0,		0,		{ 1,2,3,4,6 } },
	{ 901, L"����",				L"������",					L"ϣ��֮��ѧ԰��ʦ(�Գ�)",			L"??��??��",	L"?��",		L"δ����������������",					0,		0,		0,		{ 2 }		},
	{ 902, L"Īŵ��",			L"��Υ�",					L"ϣ��֮��ѧ԰��ʦ(�����)",		L"??��??��",	L"?��",		L"δ����������������",					0,		0,		0,		{ 2 }		},
	{ 903, L"Alter Ego",		L"���륿�`������",			L"�˹�����",						L"??��??��",	L"?��",		L"δ�����س�Ա",						0,		0,		0,		{ 1,2,4 } },
	{ 942, L"����",				L"�ϥ�",					L"����У���ġ�������",				L"??��??��",	L"?��",		L"����ϣ����ԮĻ�����",				0,		0,		0,		{ }	},
	{ 998, L"���� ��",			L"�ҥʥ����ϥ���",			L"����У���ġ�ϣ����",				L"1��1��",		L"A��",		L"ϣ��֮��ѧ԰��77��\nԤ��ѧ��ѧ��",	179,	67,		91,		{ 2,3,5 }	},
	{ 999, L"���� ����",		L"���९�顡������",		L"����У���ġ�ϣ����",				L"1��1��",		L"A��",		L"�˹�ϣ��",							179,	67,		91,		{ 2,3,4,5 }	}
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
			WCHAR szBuffer[MAX_PATH] = L"�ǳ���Ʒ";
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