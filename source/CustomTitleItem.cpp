#include "stdafx.h"
#include "Lua.h"
#include "LuaStack.hpp"
#include "CustomTitleItem.h"
#include "LuaCharacterObject.h"
#include "LuaCharacterObject.h"

using namespace luaaa;

CCustomTitleItem gCustomTitleItem;

CCustomTitleItem::CCustomTitleItem() : m_Lua()
{
    this->m_CustomTitleItem.clear();
}

CCustomTitleItem::~CCustomTitleItem()
{
    this->m_CustomTitleItem.clear();
}

void SetCustomTitleItem(int Index, int Bold, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA, char* Title, char* Command, int Align, int DynamicTitle)
{
    CUSTOM_TITLE_ITEM info;

    info.Bold = Bold;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    info.DynamicTitle = DynamicTitle;

    strcpy_s(info.Title, Title);

    strcpy_s(info.Command, Command);

    info.Align = Align;

    gCustomTitleItem.m_CustomTitleItem.insert(std::make_pair(Index, info));
}

void CCustomTitleItem::RestartLua()
{
    this->m_CustomTitleItem.clear();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCustomTitleItem", &SetCustomTitleItem);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Player\\CustomTitleItem.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCustomTitleItem", "");
}

void CCustomTitleItem::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCustomTitleItem", &SetCustomTitleItem);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Player\\CustomTitleItem.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCustomTitleItem", "");
}

int CCustomTitleItem::IsCustomTitleItem(DWORD characterObj)
{
    int result = -1;
    this->m_Lua.Generic_Call("CheckCustomTitleItem", "j>i", characterObj, &result);

    return result;
}

std::string CCustomTitleItem::GetDinamycTitle(DWORD characterObj, int titleIndex)
{
    std::string result;

    this->m_Lua.Generic_Call("GetDynamicTitle", "ji>s", characterObj, titleIndex, &result);

    return result;
}
