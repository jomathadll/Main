#include "stdafx.h"
#include "MonsterName.h"
#include "LuaStack.hpp"

using namespace luaaa;

CMonsterName gMonsterName;

CMonsterName::CMonsterName() : m_Lua()
{
	this->m_CustomMonsterNameInfo.clear();
}

CMonsterName::~CMonsterName()
{
}

void SetCustomMonsterName(int Class, int Map, int X, int Y, std::string namePor, std::string nameEng, std::string nameSpn)
{
	CUSTOM_MONSTER_NAME_INFO info;

	info.Class = Class;

	info.Map = Map;

	info.X = X;

	info.Y = Y;

	info.namePor = namePor;
	info.nameEng = nameEng;
	info.nameSpn = nameSpn;

	gMonsterName.m_CustomMonsterNameInfo.push_back(info);
}

void CMonsterName::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetCustomMonsterName", SetCustomMonsterName);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterName.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

void CMonsterName::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	gMonsterName.m_CustomMonsterNameInfo.clear();

	LuaModule(this->m_Lua.GetState()).fun("SetCustomMonsterName", SetCustomMonsterName);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterName.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

CUSTOM_MONSTER_NAME_INFO* CMonsterName::getMonster(int Class, int Map, int X, int Y)
{
	for (auto& monster : this->m_CustomMonsterNameInfo)
	{
		if (monster.Class != Class) {
			continue;
		}

		if (monster.Map != Map) {
			continue;
		}

		if ((monster.X == -1 || monster.X == X) && (monster.Y == -1 || monster.Y == Y)) {
			return &monster;
		}
	}

	return nullptr;
}

void CMonsterName::SetMonsterAttribute(CHARACTER* ObjectMonster, int MonsterID, int PositionX, int PositionY)
{
	auto monster = gMonsterName.getMonster(MonsterID, gMapManager.WorldActive, PositionX, PositionY);

	if (monster)
	{
		if (_strcmpi(g_aszMLSelection, "Por") == 0)
		{
			sprintf_s(ObjectMonster->ID, monster->namePor.c_str());
		}
		else if (_strcmpi(g_aszMLSelection, "Eng") == 0)
		{
			sprintf_s(ObjectMonster->ID, monster->nameEng.c_str());
		}
		else if (_strcmpi(g_aszMLSelection, "Spn") == 0)
		{
			sprintf_s(ObjectMonster->ID, monster->nameSpn.c_str());
		}
		else
		{
			sprintf_s(ObjectMonster->ID, monster->nameEng.c_str());
		}
	}
}
