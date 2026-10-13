#pragma once
#pragma once

#include "Lua.h"
#include <unordered_map>

struct CUSTOM_BUFF_COLOR_BK
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int Black;
};

struct CUSTOM_BUFF_COLOR_BK_2
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int Black;
};

struct CUSTOM_BUFF_COLOR_BK_3
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int Black;
};

struct CUSTOM_BUFF_COLOR_BK_4
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int Black;
};

struct CUSTOM_BUFF_COLOR_BK_5
{
    float ColorR;
    float ColorG;
    float ColorB;
    float Size;
    int Image;
    int Black;
};

class CBuffColorBK
{
public:
    CBuffColorBK();
    ~CBuffColorBK();

    void Init();
    void RestartLua();
    void GetBuffColorBK(int type, vec3_t light);
    float GetBuffSize(int type);
    int GetBuffImage(int type);
    bool GetBuffBlack(int type);

    void GetBuffColorBK2(int type, vec3_t light);
    float GetBuffSize2(int type);
    int GetBuffImage2(int type);
    bool GetBuffBlack2(int type);

    void GetBuffColorBK3(int type, vec3_t light);
    float GetBuffSize3(int type);
    int GetBuffImage3(int type);
    bool GetBuffBlack3(int type);

    void GetBuffColorBK4(int type, vec3_t light);
    float GetBuffSize4(int type);
    int GetBuffImage4(int type);
    bool GetBuffBlack4(int type);

    void GetBuffColorBK5(int type, vec3_t light);
    float GetBuffSize5(int type);
    int GetBuffImage5(int type);
    bool GetBuffBlack5(int type);

    int IsBuffColorBK(DWORD characterObj);

private:
    Lua m_Lua;

public:
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK>   m_BuffColorBKInfo;
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK_2> m_BuffColorBK_2Info;
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK_3> m_BuffColorBK_3Info;
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK_4> m_BuffColorBK_4Info;
    std::unordered_map<int, CUSTOM_BUFF_COLOR_BK_5> m_BuffColorBK_5Info;
};

extern CBuffColorBK gBuffColorBK;