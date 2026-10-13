#include "stdafx.h"
#include "ItemBorder.h"
#include "LuaStack.hpp"
#include <GL/gl.h>  // Inclua as bibliotecas gráficas que você usa

using namespace luaaa;

cItemBorder gItemBorder;

cItemBorder::cItemBorder() : m_Lua()
{
    m_ItemBorderEquiped.clear();
}

cItemBorder::~cItemBorder()
{
}

void SetItemBorderEquiped(int ItemIndex, int image, float PosX, float PosY, float Width, float Height,
    float FrameX, float FrameY, float Alpha, float Speed,
    bool FadeEffect, float FadeSpeed, float MaxAlpha, float MinAlpha)
{
    ITEM_BORDER_EQUIPED info;

    info.ItemIndex = ItemIndex;
    info.image = image;
    info.PosX = PosX;
    info.PosY = PosY;
    info.Width = Width;
    info.Height = Height;
    info.FrameX = FrameX;
    info.FrameY = FrameY;
    info.Alpha = Alpha;
    info.Speed = Speed;
    info.FadeEffect = FadeEffect;
    info.FadeSpeed = FadeSpeed;
    info.MaxAlpha = MaxAlpha;
    info.MinAlpha = MinAlpha;
    info.FadeDirection = true;

    gItemBorder.m_ItemBorderEquiped[ItemIndex].push_back(info);
}

void cItemBorder::UpdateFadeEffect()
{
    static DWORD lastTick = GetTickCount();
    DWORD currentTick = GetTickCount();
    float deltaTime = (currentTick - lastTick) / 1000.0f;
    lastTick = currentTick;

    for (auto& itemPair : m_ItemBorderEquiped)
    {
        for (auto& item : itemPair.second)
        {
            if (item.FadeEffect)
            {
                float fadeAmount = item.FadeSpeed * deltaTime;

                if (item.FadeDirection)
                {
                    item.Alpha += fadeAmount;
                    if (item.Alpha >= item.MaxAlpha)
                    {
                        item.Alpha = item.MaxAlpha;
                        item.FadeDirection = false;
                    }
                }
                else
                {
                    item.Alpha -= fadeAmount;
                    if (item.Alpha <= item.MinAlpha)
                    {
                        item.Alpha = item.MinAlpha;
                        item.FadeDirection = true;
                    }
                }

                item.Alpha = max(item.MinAlpha, min(item.MaxAlpha, item.Alpha));
            }
        }
    }
}

// NOVA FUNÇÃO ADICIONADA
void cItemBorder::RenderItemBorders(int ItemIndex)
{
    auto it = m_ItemBorderEquiped.find(ItemIndex);
    if (it != m_ItemBorderEquiped.end())
    {
        for (auto& item : it->second)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(0.50f, 0.50f, 0.50f, item.Alpha);  

            glDisable(GL_BLEND);
        }
    }
}

void cItemBorder::Init()
{
    this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());
    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetItemBorderEquiped", &SetItemBorderEquiped);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Configs\\CustomItemEquippedBorder.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);
    this->m_Lua.Generic_Call("StartItemEquippedBorder", "");
}

void cItemBorder::RestartLua()
{
    m_ItemBorderEquiped.clear();
    this->m_Lua.CloseLua();
    this->m_Lua.RegisterLua();

    this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());
    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetItemBorderEquiped", &SetItemBorderEquiped);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Configs\\CustomItemEquippedBorder.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);
    this->m_Lua.Generic_Call("StartItemEquippedBorder", "");
}