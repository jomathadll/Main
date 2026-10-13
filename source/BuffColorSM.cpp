#include "stdafx.h"
#include "BuffColorSM.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"

using namespace luaaa;

CBuffColorSM gBuffColorSM;

CBuffColorSM::CBuffColorSM() : m_Lua()
{
    this->m_BuffColorSMInfo.clear();
}

CBuffColorSM::~CBuffColorSM()
{
    this->m_BuffColorSMInfo.clear();
}

void SetBuffColorSM(int type, float ColorR, float ColorG, float ColorB, float Size, int Image, int Black)
{
    CUSTOM_BUFF_COLOR_SM info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Size = Size;

    info.Image = Image;

    info.Black = Black;

    gBuffColorSM.m_BuffColorSMInfo[type] = info;
}

void CBuffColorSM::RestartLua()
{
    this->m_BuffColorSMInfo.clear();

    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorSM", &SetBuffColorSM);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorBuffSM.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartBuffColorSM", "");
}

void CBuffColorSM::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorSM", &SetBuffColorSM);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorBuffSM.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartBuffColorSM", "");
}

void CBuffColorSM::GetBuffColorSM(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_SM>::iterator it = this->m_BuffColorSMInfo.find(type);

    if (it == this->m_BuffColorSMInfo.end())
    {
        Vector(1.0f, 1.0f, 1.0f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

float CBuffColorSM::GetBuffSize(int type)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_SM>::iterator it = this->m_BuffColorSMInfo.find(type);

    if (it == this->m_BuffColorSMInfo.end())
    {
        return 20.0f;
    }

    return it->second.Size;
}

int CBuffColorSM::GetBuffImage(int type)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_SM>::iterator it = this->m_BuffColorSMInfo.find(type);

    if (it == this->m_BuffColorSMInfo.end())
    {
        return -1;
    }

    return it->second.Image;
}

bool CBuffColorSM::GetBuffBlack(int type)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_SM>::iterator it = this->m_BuffColorSMInfo.find(type);

    if (it == this->m_BuffColorSMInfo.end())
    {
        return 0;
    }

    return it->second.Black;
}

int CBuffColorSM::IsBuffColorSM(DWORD characterObj)
{
    int result = 0;
    this->m_Lua.Generic_Call("CheckColorBuffSM", "j>i", characterObj, &result);

    return result;
}