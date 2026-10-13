#pragma once
#pragma once
#include "Lua.h"
#include "LuaGlobal.h"

struct CUSTOM_MONSTER_ITEM
{
	int Index;
	char Name[32];
	int Class;
	float Size;
	int WeaponR;
	int WeaponL;
	int Helm;
	int Armor;
	int Pants;
	int Gloves;
	int Boots;
	int Wings;
	int Helper;
	int Level;
	float Edge;
	int Action;
	bool Shadow;
	int Kind;
};

class CCustomMonsterItem
{
public:
	CCustomMonsterItem();
	~CCustomMonsterItem();

	void Init();
	void RestartLua();

	std::map<int, CUSTOM_MONSTER_ITEM> m_CustomMonsterItem;

private:
	Lua m_Lua;
	LuaGlobal m_LuaGLobal;

};

extern CCustomMonsterItem gCustomMonsterItem;