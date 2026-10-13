#pragma once

#include "Lua.h"
#include <map>
#include <string>

struct ITEM_EFFECT_INFO {
    int ItemIndex;
    float ColorR;
    float ColorG;
    float ColorB;
    BYTE EffectType;
    float Intensity;
    float Scale;
    float Height;
    float Opacity;       // Novo: controle de transparência (0.0 a 1.0)
    float Width;         // Novo: largura do efeito
    float FadeInTime;    // Novo: tempo para aparecer (segundos)
    float FadeOutTime;   // Novo: tempo para desaparecer (segundos)
    bool Ascending;      // Novo: se o efeito deve subir
};


class CItemEffectManager {
public:
    CItemEffectManager();
    virtual ~CItemEffectManager();
    void Init();

    void RestartLua();


    static int LuaLoadEffect(lua_State* L);
    bool GetEffectInfo(int Type, ITEM_EFFECT_INFO& info);
private:
    std::map<int, ITEM_EFFECT_INFO> m_EffectInfo;
    
    Lua m_Lua;
};

extern CItemEffectManager gItemEffectManager;