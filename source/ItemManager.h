// ItemManager.h: interface for the CItemManager class.
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Lua.h"

struct CUSTOM_ITEM_INFO
{
	int Index;
	int ItemIndex;
	float ColorR;
	float ColorG;
	float ColorB;
	char ModelName[32];
	char Dir[256];
	bool Type;
};

class CItemManager
{
public:
	CItemManager();
	virtual ~CItemManager();
	void Init();
	BOOL GetCustomItemColor(int Type, vec3_t Light, float Bright);
	int GET_ITEM(int section, int index);
	int GET_ITEM_MODEL(int section, int index);
	void GetItemColor(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon = false);
	void GetItemColor2(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon = false);
	void LoadItem(int ItemIndex);
	//void GetItemColor3(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon = false);
public:
	std::map<int, CUSTOM_ITEM_INFO> m_CustomItemInfo;
private:
	Lua m_Lua;
};

extern CItemManager gItemManager;