#include "stdafx.h"
#include "Lua.h"
#include "LuaStack.hpp"
#include "Charactername.h"
#include "LuaCharacterObject.h"

using namespace luaaa;

CCharactername gCharactername;

CCharactername::CCharactername() : m_Lua()
{
    this->m_Charactername.clear();
}

CCharactername::~CCharactername()
{
    this->m_Charactername.clear();
}

void SetCharactername(char* Name, char* Charactername, char* Uncharactername)
{
    CHARACTER_NAME info;

    strcpy_s(info.Name, Name);

    strcpy_s(info.Charactername, Charactername);

    strcpy_s(info.Uncharactername, Uncharactername);

    gCharactername.m_Charactername[Name] = info;
}

void CCharactername::RestartLua()
{
    this->m_Charactername.clear();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCharactername", &SetCharactername);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Player\\Charactername.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCharactername", "");
}

void CCharactername::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCharactername", &SetCharactername);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Player\\Charactername.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCharactername", "");
}

