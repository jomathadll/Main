#pragma once
#pragma once
#pragma once

#include "Lua.h"
#include "LuaInterface.h"

struct CUSTOM_DEATH_STAB_COLOR
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int FloorSize;
};

class CDeathStabColor
{
public:
    CDeathStabColor();
    ~CDeathStabColor();

    void Init();
    void RestartLua();
    void GetDeathStabColor(int type, vec3_t light);
    int GetDeathStabImage(int type);
    float GetDeathStabSize(int type);
    int IsDeathStabColor(DWORD characterObj);
    void LoadImageDeathStab();

private:
    Lua m_Lua;
    LuaInterface m_LuaInterface;

public:
    std::map<int, CUSTOM_DEATH_STAB_COLOR> m_DeathStabColorInfo;
};

extern CDeathStabColor gDeathStabColor;