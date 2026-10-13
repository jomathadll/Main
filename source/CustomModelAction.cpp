#pragma once
#include "stdafx.h"
#include "CustomModelAction.h"
#include "LuaStack.hpp"
#include "LuaGlobal.h"
#include "LuaCharacterObject.h"

using namespace luaaa;

CSetModelAction gSetModelAction;

CSetModelAction::CSetModelAction() : m_Lua()
{
	this->m_SetModelAction.clear();
}

CSetModelAction::~CSetModelAction()
{

}

void SetModelAction(int ItemIndex, int ActionStop, int ActionRun, int ActionSafeZone) {

	CUSTOM_MODEL_ACTION info;

	info.ItemIndex = ItemIndex;

	info.ActionStop = ActionStop;

	info.ActionRun = ActionRun;

	info.ActionSafeZone = ActionSafeZone;

	gSetModelAction.m_SetModelAction.insert(std::make_pair(info.ItemIndex, info));
}

void CSetModelAction::Init()
{
	RegisterLuaCharacterObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetModelAction", &SetModelAction);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomModelAction.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("CustomModelAction", ">");
}

void CSetModelAction::RestartLua()
{
	this->m_SetModelAction.clear();

	RegisterLuaCharacterObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetModelAction", &SetModelAction);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomModelAction.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("CustomModelAction", ">");
}

int CSetModelAction::IsSetModelAction(DWORD characterObj)
{
	int result = 0;
	this->m_Lua.Generic_Call("CheckSetModelAction", "j>i", characterObj, &result);

	return result;
}
