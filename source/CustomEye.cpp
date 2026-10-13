#include "stdafx.h"
#include "CustomEye.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"

using namespace luaaa;

CCustomEye gCustomEye;

CCustomEye::CCustomEye() : m_Lua()
{
    this->m_CustomEyeInfo.clear();
}

CCustomEye::~CCustomEye()
{
    this->m_CustomEyeInfo.clear();
}

void SetCustomEye(int ItemIndex, float ColorR, float ColorG, float ColorB, int Bone, int Eye)
{
    CUSTOM_COLOR_EYE info;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.Bone = Bone;

    info.Eye = Eye;

    gCustomEye.m_CustomEyeInfo[ItemIndex] = info;
}

void CCustomEye::RestartLua()
{
    this->m_CustomEyeInfo.clear();

    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCustomEye", &SetCustomEye);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorEffectEye.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCustomEye", "");
}

void CCustomEye::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCustomEye", &SetCustomEye);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorEffectEye.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCustomEye", "");
}

void CCustomEye::GetEyeColor(int ItemIndex, vec3_t light)
{
    std::map<int, CUSTOM_COLOR_EYE>::iterator it = this->m_CustomEyeInfo.find(ItemIndex);

    if (it == this->m_CustomEyeInfo.end())
    {
        Vector(1.f, 1.f, 1.f, light);
        return;
    }

    Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
    return;
}

int CCustomEye::GetEyeBone(int ItemIndex)
{
    std::map<int, CUSTOM_COLOR_EYE>::iterator it = this->m_CustomEyeInfo.find(ItemIndex);

    if (it == this->m_CustomEyeInfo.end())
    {
        return 0;
    }

    return it->second.Bone;
}

int CCustomEye::GetEyeType(int ItemIndex)
{
    std::map<int, CUSTOM_COLOR_EYE>::iterator it = this->m_CustomEyeInfo.find(ItemIndex);

    if (it == this->m_CustomEyeInfo.end())
    {
        return 0;
    }

    if (it->second.Eye == 1)
    {
        return 1;
    }
    else if (it->second.Eye == 2)
    {
        return 2;
    }
    else if (it->second.Eye == 3)
    {
        return 3;
    }
    else if (it->second.Eye == 4)
    {
        return 4;
    }
    else if (it->second.Eye == 5)
    {
        return 5;
    }
    else if (it->second.Eye == 6)
    {
        return 6;
    }

    return 0;
}

int CCustomEye::IsCustomEye(DWORD characterObj)
{
    int result = 0;
    this->m_Lua.Generic_Call("CheckColorEye", "j>i", characterObj, &result);

    return result;
}