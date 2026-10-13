#include "stdafx.h"
#include "MonsterItem.h"
#include "LuaStack.hpp"
#include "LuaGlobal.h"

using namespace luaaa;

CCustomMonsterItem gCustomMonsterItem;

CCustomMonsterItem::CCustomMonsterItem() : m_Lua()
{
	this->m_CustomMonsterItem.clear();
}

CCustomMonsterItem::~CCustomMonsterItem()
{

}

void AddCustomMonsterItem(int Index, char* Name, int Class, float Size, int WeaponR, int WeaponL, int Helm, int Armor, int Pants, int Gloves, int Boots, int 
	Wings, int Helper, int Level, float Edge, int Action, bool Shadow, int Kind) {

	CUSTOM_MONSTER_ITEM info;

	info.Index = Index;

	strcpy_s(info.Name, Name);

	info.Class = Class;

	info.Size = Size;

	info.WeaponR = WeaponR;

	info.WeaponL = WeaponL;

	info.Helm = Helm;

	info.Armor = Armor;

	info.Pants = Pants;

	info.Gloves = Gloves;

	info.Boots = Boots;

	info.Wings = Wings;

	info.Helper = Helper;

	info.Level = Level;

	info.Edge = Edge;

	info.Action = Action;

	info.Kind = Kind;

	info.Shadow = Shadow;

	gCustomMonsterItem.m_CustomMonsterItem.insert(std::make_pair(info.Index, info));
}

void CCustomMonsterItem::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("AddCustomMonsterItem", &AddCustomMonsterItem);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterItem.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartCustomMonsterItem", ">");
}

void CCustomMonsterItem::RestartLua()
{
	this->m_CustomMonsterItem.clear();

	LuaModule(this->m_Lua.GetState()).fun("AddCustomMonsterItem", &AddCustomMonsterItem);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterItem.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartCustomMonsterItem", ">");
}