#include "stdafx.h"
#include "CustomItemEdge.h"
#include "LuaStack.hpp"
#include "LuaObject.h"
#include "LuaCharacterObject.h"

using namespace luaaa;

CItemEdge gItemEdge;

CItemEdge::CItemEdge() : m_Lua()
{

}

void CItemEdge::Init()
{
	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomItemEdge.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	RegisterLuaCharacterObject(this->m_Lua.GetState());
}

void CItemEdge::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomItemEdge.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	RegisterLuaCharacterObject(this->m_Lua.GetState());
}

bool CItemEdge::RenderItemEdge(BMD* b, OBJECT* o, int ItemIndex)
{
	int retorno = 0;

	this->m_Lua.Generic_Call("RenderItemEdge", "iii>i", (DWORD)b, (DWORD)o, ItemIndex, &retorno);

	return retorno;
}

