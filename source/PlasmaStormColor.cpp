#include "stdafx.h"
#include "PlasmaStormColor.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"

using namespace luaaa;

CPlasmaStormColor gPlasmaStormColor;

CPlasmaStormColor::CPlasmaStormColor() : m_Lua()
{
    this->m_PlasmaStormColorInfo.clear();
    this->m_PlasmaStormColorRuneInfo.clear();
    this->m_PlasmaStormColorCastingInfo.clear();
}

CPlasmaStormColor::~CPlasmaStormColor()
{
    this->m_PlasmaStormColorInfo.clear();
    this->m_PlasmaStormColorRuneInfo.clear();
    this->m_PlasmaStormColorCastingInfo.clear();
}

//PLASMA
void SetPlasmaStormColor(int type, int Black, float ColorR, float ColorG, float ColorB, int Black2, float ColorR2, float ColorG2, float ColorB2)
{
    CUSTOM_PLASMA_STORM_COLOR info;

    info.Black = Black;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Black2 = Black2;

    info.ColorR2 = ColorR2;

    info.ColorG2 = ColorG2;

    info.ColorB2 = ColorB2;

    gPlasmaStormColor.m_PlasmaStormColorInfo[type] = info;
}

void CPlasmaStormColor::GetPlasmaStormColor(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR>::iterator it = this->m_PlasmaStormColorInfo.find(type);

    if (it == this->m_PlasmaStormColorInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

bool CPlasmaStormColor::GetPlasmaStormBlack(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR>::iterator it = this->m_PlasmaStormColorInfo.find(type);

    if (it == this->m_PlasmaStormColorInfo.end())
    {
        return 0;
    }

    return it->second.Black;
}

void CPlasmaStormColor::GetPlasmaStormColor2(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR>::iterator it = this->m_PlasmaStormColorInfo.find(type);

    if (it == this->m_PlasmaStormColorInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR2, it->second.ColorG2, it->second.ColorB2, light);
    return;
}

bool CPlasmaStormColor::GetPlasmaStormBlack2(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR>::iterator it = this->m_PlasmaStormColorInfo.find(type);

    if (it == this->m_PlasmaStormColorInfo.end())
    {
        return 0;
    }

    return it->second.Black2;
}

//RUNE
void SetPlasmaStormColorRune(int type, bool Enabled, int Black, float ColorR, float ColorG, float ColorB, int Black2, float ColorR2, float ColorG2, float ColorB2, int Target)
{
    CUSTOM_PLASMA_STORM_COLOR_RUNE info;

    info.Enabled = Enabled;

    info.Black = Black;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Black2 = Black2;

    info.ColorR2 = ColorR2;

    info.ColorG2 = ColorG2;

    info.ColorB2 = ColorB2;

    info.Target = Target;

    gPlasmaStormColor.m_PlasmaStormColorRuneInfo[type] = info;
}

void CPlasmaStormColor::GetPlasmaStormRune(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE>::iterator it = this->m_PlasmaStormColorRuneInfo.find(type);

    if (it == this->m_PlasmaStormColorRuneInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

bool CPlasmaStormColor::GetPlasmaStormRuneBlack(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE>::iterator it = this->m_PlasmaStormColorRuneInfo.find(type);

    if (it == this->m_PlasmaStormColorRuneInfo.end())
    {
        return 0;
    }

    return it->second.Black;
}

void CPlasmaStormColor::GetPlasmaStormRune2(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE>::iterator it = this->m_PlasmaStormColorRuneInfo.find(type);

    if (it == this->m_PlasmaStormColorRuneInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR2, it->second.ColorG2, it->second.ColorB2, light);
    return;
}

bool CPlasmaStormColor::GetPlasmaStormRuneBlack2(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE>::iterator it = this->m_PlasmaStormColorRuneInfo.find(type);

    if (it == this->m_PlasmaStormColorRuneInfo.end())
    {
        return 0;
    }

    return it->second.Black2;
}

bool CPlasmaStormColor::GetPlasmaStormRuneEnabled(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE>::iterator it = this->m_PlasmaStormColorRuneInfo.find(type);

    if (it == this->m_PlasmaStormColorRuneInfo.end())
    {
        return false;
    }

    return true;
}

int CPlasmaStormColor::GetPlasmaStormRuneTarget(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE>::iterator it = this->m_PlasmaStormColorRuneInfo.find(type);

    if (it == this->m_PlasmaStormColorRuneInfo.end())
    {
        return 0;
    }

    if (it->second.Target == 1)
    {
        return 1;
    }
    else if (it->second.Target == 2)
    {
        return 2;
    }

    return 0;
}

//CASTING
void SetPlasmaStormColorCasting(int type, bool Enabled, int Black, float ColorR, float ColorG, float ColorB, int Shadow, float ColorR2, float ColorG2, float ColorB2, float ColorA2, int Target)
{
    CUSTOM_PLASMA_STORM_COLOR_CASTING info;

    info.Enabled = Enabled;

    info.Black = Black;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Shadow = Shadow;

    info.ColorR2 = ColorR2;

    info.ColorG2 = ColorG2;

    info.ColorB2 = ColorB2;

    info.ColorA2 = ColorA2;

    info.Target = Target;

    gPlasmaStormColor.m_PlasmaStormColorCastingInfo[type] = info;
}

void CPlasmaStormColor::GetPlasmaStormCasting(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

bool CPlasmaStormColor::GetPlasmaStormCastingBlack(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        return 0;
    }

    return it->second.Black;
}

bool CPlasmaStormColor::GetPlasmaStormCastingEnabled(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        return false;
    }

    return true;
}

int CPlasmaStormColor::GetPlasmaStormCastingTarget(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        return 0;
    }

    if (it->second.Target == 1)
    {
        return 1;
    }
    else if (it->second.Target == 2)
    {
        return 2;
    }

    return 0;
}

void CPlasmaStormColor::GetPlasmaStormCastingShadowColor(int type, vec3_t light)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR2, it->second.ColorG2, it->second.ColorB2, light);
    return;
}

int CPlasmaStormColor::GetPlasmaStormCastingShadow(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        return 0;
    }

    if (it->second.Shadow == 1)
    {
        return 1;
    }
    else if (it->second.Shadow == 2)
    {
        return 2;
    }

    return 0;
}

float CPlasmaStormColor::GetPlasmaStormCastingShadowAlpha(int type)
{
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING>::iterator it = this->m_PlasmaStormColorCastingInfo.find(type);

    if (it == this->m_PlasmaStormColorCastingInfo.end())
    {
        return 1.f;
    }

    return it->second.ColorA2;
}

void CPlasmaStormColor::RestartLua()
{
    this->m_PlasmaStormColorInfo.clear();

    this->m_PlasmaStormColorRuneInfo.clear();

    this->m_PlasmaStormColorCastingInfo.clear();

    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetPlasmaStormColor", &SetPlasmaStormColor);

    LuaModule(this->m_Lua.GetState()).fun("SetPlasmaStormColorRune", &SetPlasmaStormColorRune);

    LuaModule(this->m_Lua.GetState()).fun("SetPlasmaStormColorCasting", &SetPlasmaStormColorCasting);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorPlasmaStorm.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartPlasmaStormColor", "");
}

void CPlasmaStormColor::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetPlasmaStormColor", &SetPlasmaStormColor);

    LuaModule(this->m_Lua.GetState()).fun("SetPlasmaStormColorRune", &SetPlasmaStormColorRune);

    LuaModule(this->m_Lua.GetState()).fun("SetPlasmaStormColorCasting", &SetPlasmaStormColorCasting);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorPlasmaStorm.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartPlasmaStormColor", "");
}

int CPlasmaStormColor::IsPlasmaStormColor(DWORD characterObj)
{
    int result = 0;
    this->m_Lua.Generic_Call("CheckColorPlasmaStorm", "j>i", characterObj, &result);

    return result;
}