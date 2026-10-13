#include "stdafx.h"
#include "CustomJewelStack.h"
#include "LuaStack.hpp"

using namespace luaaa;

CJewelStack gJewelStack;

CJewelStack::CJewelStack() : m_Lua()
{
	this->m_JewelList.clear();
}

void SetJewelStack(int ItemIndex) 
{
	gJewelStack.m_JewelList.push_back(ItemIndex);
}

void CJewelStack::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetJewelStack", &SetJewelStack);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomJewelStack.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadJewelStack", ">");
}

bool CJewelStack::CheckIsStack(int ItemIndex)
{
	for (auto& item : this->m_JewelList) {
		if (item == ItemIndex) {
			return true;
		}
	}

	return false;
}