#pragma once
#include "stdafx.h"
#include "LuaGlobal.h"

struct CUSTOM_TITLE_ITEM
{
    int Bold;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
    char Title[256];
    char Command[32];
    int Align;
    bool DynamicTitle;
};

class CCustomTitleItem
{
public:
    CCustomTitleItem();
    ~CCustomTitleItem();

    void Init();
    void RestartLua();
    int IsCustomTitleItem(DWORD characterObj);
    std::string GetDinamycTitle(DWORD characterObj, int titleIndex);

private:
    Lua m_Lua;
    LuaGlobal m_LuaGlobal;

public:
    std::map<int, CUSTOM_TITLE_ITEM> m_CustomTitleItem;
};

extern CCustomTitleItem gCustomTitleItem;