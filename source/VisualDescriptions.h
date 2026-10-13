#pragma once

#include "Lua.h"
#include "LuaGlobal.h"

struct ITEM_HYBRID_NAME
{
	int ItemIndex;
	char Name[50];
};

class cVisuals
{
public:
	void Init();
	void RestartLua();
	void SetDescriptionsItem(int Line, ITEM* itemObject);
	int DescriptionsCall(int Line, ITEM* itemObject);
	char* GetItemHybridName(int ItemIndex);

public:
	int VisualSwitch;
	int VisualOriginalDescription;
	Lua m_Lua;
	LuaGlobal m_LuaGlobal;
	std::map<int, ITEM_HYBRID_NAME> ItemHybridNameList;
};

extern cVisuals Visuals;