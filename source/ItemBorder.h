#pragma once

#include "Lua.h"
#include "LuaInterface.h"
#include "LuaGlobal.h"
#include <map>
#include <vector>

struct ITEM_BORDER_EQUIPED
{
    int ItemIndex;
    int image;
    float PosX;
    float PosY;
    float Width;
    float Height;
    float FrameX;
    float FrameY;
    float Alpha;
    float Speed;
    bool FadeEffect;       // Se verdadeiro, habilita o efeito de fade
    float FadeSpeed;       // Velocidade do fade
    float MaxAlpha;        // Alpha máximo a ser alcançado
    float MinAlpha;        // Alpha mínimo
    bool FadeDirection;    // Direção do fade (true = aumentando, false = diminuindo)
};

class cItemBorder
{
public:
    cItemBorder();
    ~cItemBorder();
    void Init();
    void RestartLua();
    void UpdateFadeEffect();
    void RenderItemBorders(int ItemIndex);  // NOVO MÉTODO ADICIONADO

private:
    Lua m_Lua;
    LuaInterface m_LuaInterface;
    LuaGlobal m_LuaGlobal;

public:
    std::map<int, std::vector<ITEM_BORDER_EQUIPED>> m_ItemBorderEquiped;
};

extern cItemBorder gItemBorder;