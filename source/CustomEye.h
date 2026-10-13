#pragma once
#pragma once
#pragma once

#include "Lua.h"

struct CUSTOM_COLOR_EYE
{
    int ItemIndex;
    float ColorR;
    float ColorG;
    float ColorB;
    int Bone;
    int Eye;
};

class CCustomEye
{
public:
    CCustomEye();
    ~CCustomEye();

    void Init();
    void RestartLua();
    void GetEyeColor(int type, vec3_t light);
    int GetEyeBone(int type);
    int GetEyeType(int type);
    int IsCustomEye(DWORD characterObj);

private:
    Lua m_Lua;

public:
    std::map<int, CUSTOM_COLOR_EYE> m_CustomEyeInfo;
};

extern CCustomEye gCustomEye;