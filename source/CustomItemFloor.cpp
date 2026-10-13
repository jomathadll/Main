#include "stdafx.h"
#include "CustomItemFloor.h"
#include "LuaStack.hpp"
#include "ZzzObject.h"

using namespace luaaa;

CCustomItemFloor gCustomItemFloor;

CCustomItemFloor::CCustomItemFloor() : m_Lua()
{
	this->m_CustomItemFloorInfo.clear();
}

CCustomItemFloor::~CCustomItemFloor()
{
	this->m_CustomItemFloorInfo.clear();
}

void SetItemFloor(int ItemIndex, float AngleX, float AngleY, float AngleZ, float Size) 
{
	CUSTOM_ITEM_FLOOR_INFO info;

	info.ItemIndex = ItemIndex + MODEL_ITEM;

	info.AngleX = AngleX;

	info.AngleY = AngleY;

	info.AngleZ = AngleZ;

	info.Size = Size;

	gCustomItemFloor.m_CustomItemFloorInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CCustomItemFloor::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetItemFloor", &SetItemFloor);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomItemFloor.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadItemFloor", ">");
}

void CCustomItemFloor::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	gCustomItemFloor.m_CustomItemFloorInfo.clear();

	LuaModule(this->m_Lua.GetState()).fun("SetItemFloor", &SetItemFloor);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomItemFloor.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadItemFloor", ">");
}

CUSTOM_ITEM_FLOOR_INFO * CCustomItemFloor::getItem(int ItemIndex)
{
	std::map<int, CUSTOM_ITEM_FLOOR_INFO>::iterator it = this->m_CustomItemFloorInfo.find(ItemIndex);

	if (it == this->m_CustomItemFloorInfo.end()) 
	{
		return 0;
	}

	return &it->second;
}

void CCustomItemFloor::AngleItem(OBJECT * object)
{
	auto CheckItem = gCustomItemFloor.getItem(object->Type);

	if (CheckItem) 
	{
		Vector(CheckItem->AngleX, CheckItem->AngleY, CheckItem->AngleZ, object->Angle);

		object->Scale = CheckItem->Size;
	}
	else 
	{
		ItemAngle(object);
	}
}
