#include "stdafx.h"
#include "BuffColorBK.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"

using namespace luaaa;

CBuffColorBK gBuffColorBK;

CBuffColorBK::CBuffColorBK() : m_Lua()
{
    this->m_BuffColorBKInfo.clear();
    this->m_BuffColorBK_2Info.clear();
    this->m_BuffColorBK_3Info.clear();
    this->m_BuffColorBK_4Info.clear();
    this->m_BuffColorBK_5Info.clear();
}

CBuffColorBK::~CBuffColorBK()
{
    this->m_BuffColorBKInfo.clear();
    this->m_BuffColorBK_2Info.clear();
    this->m_BuffColorBK_3Info.clear();
    this->m_BuffColorBK_4Info.clear();
    this->m_BuffColorBK_5Info.clear();
}

void SetBuffColorBK(int type, float ColorR, float ColorG, float ColorB, float Size, int Image, int Black)
{
    CUSTOM_BUFF_COLOR_BK info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Size = Size;

    info.Image = Image;

    info.Black = Black;

    gBuffColorBK.m_BuffColorBKInfo[type] = info;
}

void SetBuffColorBK2(int type, float ColorR, float ColorG, float ColorB, float Size, int Image, int Black)
{
    CUSTOM_BUFF_COLOR_BK_2 info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Size = Size;

    info.Image = Image;

    info.Black = Black;

    gBuffColorBK.m_BuffColorBK_2Info[type] = info;
}

void SetBuffColorBK3(int type, float ColorR, float ColorG, float ColorB, float Size, int Image, int Black)
{
    CUSTOM_BUFF_COLOR_BK_3 info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Size = Size;

    info.Image = Image;

    info.Black = Black;

    gBuffColorBK.m_BuffColorBK_3Info[type] = info;
}

void SetBuffColorBK4(int type, float ColorR, float ColorG, float ColorB, float Size, int Image, int Black)
{
    CUSTOM_BUFF_COLOR_BK_4 info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Size = Size;

    info.Image = Image;

    info.Black = Black;

    gBuffColorBK.m_BuffColorBK_4Info[type] = info;
}

void SetBuffColorBK5(int type, float ColorR, float ColorG, float ColorB, float Size, int Image, int Black)
{
    CUSTOM_BUFF_COLOR_BK_5 info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Size = Size;

    info.Image = Image;

    info.Black = Black;

    gBuffColorBK.m_BuffColorBK_5Info[type] = info;
}

void CBuffColorBK::RestartLua()
{
    this->m_BuffColorBKInfo.clear();

    this->m_BuffColorBK_2Info.clear();

    this->m_BuffColorBK_3Info.clear();

    this->m_BuffColorBK_4Info.clear();

    this->m_BuffColorBK_5Info.clear();

    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK", &SetBuffColorBK);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK2", &SetBuffColorBK2);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK3", &SetBuffColorBK3);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK4", &SetBuffColorBK4);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK5", &SetBuffColorBK5);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorBuffBK.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartBuffColorBK", "");
}

void CBuffColorBK::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK", &SetBuffColorBK);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK2", &SetBuffColorBK2);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK3", &SetBuffColorBK3);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK4", &SetBuffColorBK4);

    LuaModule(this->m_Lua.GetState()).fun("SetBuffColorBK5", &SetBuffColorBK5);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorBuffBK.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartBuffColorBK", "");
}

void CBuffColorBK::GetBuffColorBK(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK>::iterator it = this->m_BuffColorBKInfo.find(type);

    if (it == this->m_BuffColorBKInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

float CBuffColorBK::GetBuffSize(int type)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK>::iterator it = this->m_BuffColorBKInfo.find(type);

    if (it == this->m_BuffColorBKInfo.end())
    {
        return 60.f;
    }

    return it->second.Size;
}

int CBuffColorBK::GetBuffImage(int type)
{
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK>::iterator it = this->m_BuffColorBKInfo.find(type);

    if (it == this->m_BuffColorBKInfo.end())
    {
        return -1;
    }

    return it->second.Image;
}

bool CBuffColorBK::GetBuffBlack(int type)
{
    auto it = this->m_BuffColorBKInfo.find(type);

    if (it == this->m_BuffColorBKInfo.end())
    {
        return false;
    }

    return it->second.Black;
}

void CBuffColorBK::GetBuffColorBK2(int type, vec3_t light)
{
    auto it = this->m_BuffColorBK_2Info.find(type);

    if (it == this->m_BuffColorBK_2Info.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
}

float CBuffColorBK::GetBuffSize2(int type)
{
    auto it = this->m_BuffColorBK_2Info.find(type);

    if (it == this->m_BuffColorBK_2Info.end())
    {
        return 40.f;
    }

    return it->second.Size;
}

int CBuffColorBK::GetBuffImage2(int type)
{
    auto it = this->m_BuffColorBK_2Info.find(type);

    if (it == this->m_BuffColorBK_2Info.end())
    {
        return -1;
    }

    return it->second.Image;
}

bool CBuffColorBK::GetBuffBlack2(int type)
{
    auto it = this->m_BuffColorBK_2Info.find(type);

    if (it == this->m_BuffColorBK_2Info.end())
    {
        return false;
    }

    return it->second.Black;
}

void CBuffColorBK::GetBuffColorBK3(int type, vec3_t light)
{
    auto it = this->m_BuffColorBK_3Info.find(type);

    if (it == this->m_BuffColorBK_3Info.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
}

float CBuffColorBK::GetBuffSize3(int type)
{
    auto it = this->m_BuffColorBK_3Info.find(type);

    if (it == this->m_BuffColorBK_3Info.end())
    {
        return 40.f;
    }

    return it->second.Size;
}

int CBuffColorBK::GetBuffImage3(int type)
{
    auto it = this->m_BuffColorBK_3Info.find(type);

    if (it == this->m_BuffColorBK_3Info.end())
    {
        return -1;
    }

    return it->second.Image;
}

bool CBuffColorBK::GetBuffBlack3(int type)
{
    auto it = this->m_BuffColorBK_3Info.find(type);

    if (it == this->m_BuffColorBK_3Info.end())
    {
        return false;
    }

    return it->second.Black;
}

void CBuffColorBK::GetBuffColorBK4(int type, vec3_t light)
{
    auto it = this->m_BuffColorBK_4Info.find(type);

    if (it == this->m_BuffColorBK_4Info.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
}

float CBuffColorBK::GetBuffSize4(int type)
{
    auto it = this->m_BuffColorBK_4Info.find(type);

    if (it == this->m_BuffColorBK_4Info.end())
    {
        return 0.f;
    }

    return it->second.Size;
}

int CBuffColorBK::GetBuffImage4(int type)
{
    auto it = this->m_BuffColorBK_4Info.find(type);

    if (it == this->m_BuffColorBK_4Info.end())
    {
        return -1;
    }

    return it->second.Image;
}

bool CBuffColorBK::GetBuffBlack4(int type)
{
    auto it = this->m_BuffColorBK_4Info.find(type);

    if (it == this->m_BuffColorBK_4Info.end())
    {
        return false;
    }

    return it->second.Black;
}

void CBuffColorBK::GetBuffColorBK5(int type, vec3_t light)
{
    auto it = this->m_BuffColorBK_5Info.find(type);

    if (it == this->m_BuffColorBK_5Info.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
}

float CBuffColorBK::GetBuffSize5(int type)
{
    auto it = this->m_BuffColorBK_5Info.find(type);

    if (it == this->m_BuffColorBK_5Info.end())
    {
        return 40.f;
    }

    return it->second.Size;
}

int CBuffColorBK::GetBuffImage5(int type)
{
    auto it = this->m_BuffColorBK_5Info.find(type);

    if (it == this->m_BuffColorBK_5Info.end())
    {
        return -1;
    }

    return it->second.Image;
}

bool CBuffColorBK::GetBuffBlack5(int type)
{
    auto it = this->m_BuffColorBK_5Info.find(type);

    if (it == this->m_BuffColorBK_5Info.end())
    {
        return false;
    }

    return it->second.Black;
}

int CBuffColorBK::IsBuffColorBK(DWORD characterObj)
{
    int result = 0;
    this->m_Lua.Generic_Call("CheckColorBuffBK", "j>i", characterObj , &result);

    return result;
}