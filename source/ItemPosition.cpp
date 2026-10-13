#include "stdafx.h"
#include "ItemPosition.h"
#include "LuaStack.hpp"
#include "ItemSize.h"

using namespace luaaa;

CCustomItemPosition gCustomItemPosition;

CCustomItemPosition::CCustomItemPosition() : m_Lua()
{
	this->m_CustomItemPositionInfo.clear();
}

CCustomItemPosition::~CCustomItemPosition()
{
	this->m_CustomItemPositionInfo.clear();
}

void SetItemPosition(int ItemIndex, float PosX, float PosY, float AngleX, float AngleY, float AngleZ) 
{
	CUSTOM_ITEM_POSITION_INFO info;

	info.ItemIndex = ItemIndex;

	info.PosX = PosX;

	info.PosY = PosY;

	info.AngleX = AngleX;

	info.AngleY = AngleY;

	info.AngleZ = AngleZ;

	gCustomItemPosition.m_CustomItemPositionInfo.insert(std::make_pair(info.ItemIndex, info));
}

void SetItemSize(int ItemIndex, float SizeInventory) {
	CUSTOM_ITEM_SIZE_INFO info;

	info.ItemIndex = ItemIndex;

	info.SizeInventory = SizeInventory;

	gCustomItemSize.m_CustomItemSizeInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CCustomItemPosition::GetPositionItem(int ItemIndex, float* PosX, float* PosY, float* AngleX, float* AngleY, float* AngleZ)
{
	auto itemPos = gCustomItemPosition.getItemPosition(ItemIndex);

	if (itemPos) 
	{
		(*PosX) = itemPos->PosX;
		(*PosY) = itemPos->PosY;
		(*AngleX) = itemPos->AngleX;
		(*AngleY) = itemPos->AngleY;
		(*AngleZ) = itemPos->AngleZ;
	}
}

bool CCustomItemPosition::CheckCustomItemPosition(int ItemIndex)
{
	std::map<int, CUSTOM_ITEM_POSITION_INFO>::iterator it = this->m_CustomItemPositionInfo.find(ItemIndex);

	if (it == this->m_CustomItemPositionInfo.end()) 
	{
		return 0;
	}

	return 1;
}

CUSTOM_ITEM_POSITION_INFO * CCustomItemPosition::getItemPosition(int ItemIndex)
{
	std::map<int, CUSTOM_ITEM_POSITION_INFO>::iterator it = this->m_CustomItemPositionInfo.find(ItemIndex);

	if (it == this->m_CustomItemPositionInfo.end()) 
	{
		return 0;
	}

	return &it->second;
}

void CCustomItemPosition::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetItemPosition", &SetItemPosition);

	LuaModule(this->m_Lua.GetState()).fun("SetItemSize", &SetItemSize);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomItemPosition.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadItemPosition", "");
}

void CCustomItemPosition::RestartLua()
{
	gCustomItemPosition.m_CustomItemPositionInfo.clear();

	gCustomItemSize.m_CustomItemSizeInfo.clear();

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	LuaModule(this->m_Lua.GetState()).fun("SetItemPosition", &SetItemPosition);

	LuaModule(this->m_Lua.GetState()).fun("SetItemSize", &SetItemSize);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomItemPosition.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadItemPosition", "");
}

