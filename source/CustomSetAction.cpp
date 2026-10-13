#pragma once
#include "stdafx.h"
#include "CustomSetAction.h"
#include "LuaStack.hpp"
#include "LuaGlobal.h"
#include "LuaCharacterObject.h"

using namespace luaaa;

CSetAction gSetAction;

CSetAction::CSetAction() : m_Lua()
{
	this->m_SetAction.clear();
}

CSetAction::~CSetAction()
{

}

void SetAction(int ItemIndex, char* Command, int Action) {

	CUSTOM_SET_ACTION info;

	info.ItemIndex = ItemIndex;

	strcpy_s(info.Command, Command);

	info.Action = Action;

	gSetAction.m_SetAction.insert(std::make_pair(info.ItemIndex, info));
}

void CSetAction::Init()
{
	RegisterLuaCharacterObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetAction", &SetAction);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomSetAction.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("CustomSetAction", ">");
}

void CSetAction::RestartLua()
{
	this->m_SetAction.clear();

	RegisterLuaCharacterObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetAction", &SetAction);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomSetAction.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("CustomSetAction", ">");
}

int CSetAction::IsSetAction(DWORD characterObj)
{
	int result = 0;
	this->m_Lua.Generic_Call("CheckSetAction", "j>i", characterObj, &result);

	return result;
}
