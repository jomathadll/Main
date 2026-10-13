#pragma once
#pragma once
#pragma once
#pragma once

#include "Lua.h"
#include <unordered_map>

struct CUSTOM_PLASMA_STORM_COLOR
{
    int Black;
    float ColorR;
    float ColorG;
    float ColorB;
    int Black2;
    float ColorR2;
    float ColorG2;
    float ColorB2;
};

struct CUSTOM_PLASMA_STORM_COLOR_RUNE
{
    bool Enabled;
    int Black;
    float ColorR;
    float ColorG;
    float ColorB;
    int Black2;
    float ColorR2;
    float ColorG2;
    float ColorB2;
    int Target;
};

struct CUSTOM_PLASMA_STORM_COLOR_CASTING
{
    bool Enabled;
    int Black;
    float ColorR;
    float ColorG;
    float ColorB;
    int Shadow;
    float ColorR2;
    float ColorG2;
    float ColorB2;
    float ColorA2;
    int Target;
};

class CPlasmaStormColor
{
public:
    CPlasmaStormColor();
    ~CPlasmaStormColor();

    void Init();
    void RestartLua();
    void GetPlasmaStormColor(int type, vec3_t light);
    bool GetPlasmaStormBlack(int type);
    void GetPlasmaStormColor2(int type, vec3_t light);
    bool GetPlasmaStormBlack2(int type);

    void GetPlasmaStormRune(int type, vec3_t light);
    bool GetPlasmaStormRuneEnabled(int type);
    bool GetPlasmaStormRuneBlack(int type);
    void GetPlasmaStormRune2(int type, vec3_t light);
    bool GetPlasmaStormRuneBlack2(int type);
    int GetPlasmaStormRuneTarget(int type);

    void GetPlasmaStormCasting(int type, vec3_t light);
    void GetPlasmaStormCastingShadowColor(int type, vec3_t light);
    bool GetPlasmaStormCastingEnabled(int type);
    bool GetPlasmaStormCastingBlack(int type);
    int GetPlasmaStormCastingTarget(int type);
    int GetPlasmaStormCastingShadow(int type);
    float GetPlasmaStormCastingShadowAlpha(int type);

    int IsPlasmaStormColor(DWORD characterObj);

private:
    Lua m_Lua;

public:
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR> m_PlasmaStormColorInfo;
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_RUNE> m_PlasmaStormColorRuneInfo;
    std::unordered_map<int, CUSTOM_PLASMA_STORM_COLOR_CASTING> m_PlasmaStormColorCastingInfo;
};

extern CPlasmaStormColor gPlasmaStormColor;