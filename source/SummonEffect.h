#pragma once
#pragma once
#pragma once

#include "Lua.h"
#include "LuaGlobal.h"

struct CUSTOM_SUMMON_EFFECT
{
    int ItemIndex;
    int Model;
    int Type;
    float ColorR;
    float ColorG;
    float ColorB;
    bool SafeZone;
    float Speed;

};

struct CUSTOM_SUMMON_EFFECT_WRIST
{
    int ItemIndex;
    int Type;
    float ColorR;
    float ColorG;
    float ColorB;
    int Bone;

};

class CSummonEffect
{
public:
    CSummonEffect();
    ~CSummonEffect();

    void Init();
    void RestartLua();
    void MoveSummonEffect(CHARACTER* pCharacter, int ItemIndex);
    void CreateSummonEffect(CHARACTER* pCharacter, int ItemIndex);
    void RemoveSummonEffect(CHARACTER* pCharacter);
    void MoveSummonEffect_Wrist(CHARACTER* pCharacter, int ItemIndex);
    void CreateSummonEffect_Wrist(CHARACTER* pCharacter, int ItemIndex);
    void RemoveSummonEffect_Wrist(CHARACTER* pCharacter);
    int IsSummonEffect(DWORD characterObj);
    int IsSummonEffect_Wrist(DWORD characterObj);
    bool SummonEffectSafezone(int ItemIndex);
    float SummonEffectSpeed(int ItemIndex);
    void SummonEffectColor(int ItemIndex, vec3_t light);
    int SummonEffectType(int ItemIndex);
    int SummonEffectModel(int ItemIndex);
    void RemoveSummonEffectsAll(CHARACTER* pCharacter);
    void SummonEffectColor_Wrist(int ItemIndex, vec3_t light);
    int SummonEffectType_Wrist(int ItemIndex);
    int SummonEffectBone_Wrist(int ItemIndex);

public:
    Lua m_Lua;
    LuaGlobal m_LuaGlobal;
    std::map<SHORT, BYTE> m_SummonEffectRandom;
    std::map<int, CUSTOM_SUMMON_EFFECT> m_SummonEffectInfo;
    std::map<int, CUSTOM_SUMMON_EFFECT_WRIST> m_SummonEffectWristInfo;
};

extern CSummonEffect gSummonEffect;