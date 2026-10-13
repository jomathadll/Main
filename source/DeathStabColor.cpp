#include "stdafx.h"
#include "DeathStabColor.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"
#include "LoadImages.h"

using namespace luaaa;

CDeathStabColor gDeathStabColor;

CDeathStabColor::CDeathStabColor() : m_Lua()
{
    this->m_DeathStabColorInfo.clear();
}

CDeathStabColor::~CDeathStabColor()
{
    this->m_DeathStabColorInfo.clear();
}

void SetDeathStabColor(int type, float ColorR, float ColorG, float ColorB, int Image, float Size)
{
    CUSTOM_DEATH_STAB_COLOR info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Image = Image;

    info.Size = Size;

    gDeathStabColor.m_DeathStabColorInfo[type] = info;
}

void CDeathStabColor::RestartLua()
{
    this->m_DeathStabColorInfo.clear();

    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetDeathStabColor", &SetDeathStabColor);

    this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

    gLoadImages.SetFunctions(this->m_Lua.GetState());

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorDeathStab.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartDeathStabColor", "");
}

void CDeathStabColor::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetDeathStabColor", &SetDeathStabColor);

    this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

    gLoadImages.SetFunctions(this->m_Lua.GetState());

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorDeathStab.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartDeathStabColor", "");
}

void CDeathStabColor::GetDeathStabColor(int type, vec3_t light)
{
    std::map<int, CUSTOM_DEATH_STAB_COLOR>::iterator it = this->m_DeathStabColorInfo.find(type);

    if (it == this->m_DeathStabColorInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

int CDeathStabColor::GetDeathStabImage(int type)
{
    std::map<int, CUSTOM_DEATH_STAB_COLOR>::iterator it = this->m_DeathStabColorInfo.find(type);

    if (it == this->m_DeathStabColorInfo.end())
    {
        return -1;
    }

    return it->second.Image;
}

float CDeathStabColor::GetDeathStabSize(int type)
{
    std::map<int, CUSTOM_DEATH_STAB_COLOR>::iterator it = this->m_DeathStabColorInfo.find(type);

    if (it == this->m_DeathStabColorInfo.end())
    {
        return 40.0f;
    }

    return it->second.Size;
}

int CDeathStabColor::IsDeathStabColor(DWORD characterObj)
{
    int result = 0;
    this->m_Lua.Generic_Call("CheckColorDeathStab", "j>i", characterObj, &result);

    return result;
}

void CDeathStabColor::LoadImageDeathStab()
{
    this->m_Lua.Generic_Call("StartLoadImages", ">");
}