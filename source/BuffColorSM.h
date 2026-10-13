#pragma once
#pragma once
#pragma once

#include "Lua.h"
#include <unordered_map>

struct CUSTOM_BUFF_COLOR_SM
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int Black;
};

class CBuffColorSM
{
public:
    CBuffColorSM();
    ~CBuffColorSM();

    void Init();
    void RestartLua();
    void GetBuffColorSM(int type, vec3_t light);
    float GetBuffSize(int type);
    int GetBuffImage(int type);
    bool GetBuffBlack(int type);
    int IsBuffColorSM(DWORD characterObj);

private:
    Lua m_Lua;

public:
    std::unordered_map<int, CUSTOM_BUFF_COLOR_SM> m_BuffColorSMInfo;
};

extern CBuffColorSM gBuffColorSM;