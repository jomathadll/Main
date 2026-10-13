#include "stdafx.h"
#include "EffectManager.h"
#include "LuaStack.hpp"
using namespace luaaa;

CItemEffectManager gItemEffectManager;

CItemEffectManager::CItemEffectManager() : m_Lua() {
    m_EffectInfo.clear();
  
}

CItemEffectManager::~CItemEffectManager() {
    m_EffectInfo.clear();
    
}

void CItemEffectManager::Init() {
    LuaModule(this->m_Lua.GetState())
        .fun("LoadEffect", &CItemEffectManager::LuaLoadEffect);
     

    m_Lua.DoFile("Data//Custom//Lua//EffectSystem//DropEffectItem.lua");
    m_Lua.Generic_Call("StartLoadEffects", "");
}

void CItemEffectManager::RestartLua() {
    LuaModule(this->m_Lua.GetState())
        .fun("LoadEffect", &CItemEffectManager::LuaLoadEffect);
     

    m_Lua.DoFile("Data//Custom//Lua//EffectSystem//DropEffectItem.lua");
    m_Lua.Generic_Call("StartLoadEffects", "");
}

int CItemEffectManager::LuaLoadEffect(lua_State* L)
{
    int ItemIndex = lua_tointeger(L, 1);
    int colorR = lua_tointeger(L, 2);
    int colorG = lua_tointeger(L, 3);
    int colorB = lua_tointeger(L, 4);
    int effectType = lua_tointeger(L, 5);
    float intensity = lua_isnumber(L, 6) ? lua_tonumber(L, 6) : 1.0f;
    float scale = lua_isnumber(L, 7) ? lua_tonumber(L, 7) : 1.0f;
    float height = lua_isnumber(L, 8) ? lua_tonumber(L, 8) : 0.0f;
    float opacity = lua_isnumber(L, 9) ? lua_tonumber(L, 9) : 1.0f;
    float width = lua_isnumber(L, 10) ? lua_tonumber(L, 10) : 1.0f;
    float fadeIn = lua_isnumber(L, 11) ? lua_tonumber(L, 11) : 0.0f;
    float fadeOut = lua_isnumber(L, 12) ? lua_tonumber(L, 12) : 0.0f;
    bool ascending = lua_isboolean(L, 13) ? lua_toboolean(L, 13) : false;

    ITEM_EFFECT_INFO info;
    info.ItemIndex = ItemIndex;
    info.ColorR = static_cast<float>(colorR) / 255.0f;
    info.ColorG = static_cast<float>(colorG) / 255.0f;
    info.ColorB = static_cast<float>(colorB) / 255.0f;
    info.EffectType = static_cast<BYTE>(effectType);
    info.Intensity = intensity;
    info.Scale = scale;
    info.Height = height;
    info.Opacity = opacity;
    info.Width = width;
    info.FadeInTime = fadeIn;
    info.FadeOutTime = fadeOut;
    info.Ascending = ascending;

    gItemEffectManager.m_EffectInfo[ItemIndex] = info;

    return 0;
}



bool CItemEffectManager::GetEffectInfo(int Type, ITEM_EFFECT_INFO& info)
{
    auto it = m_EffectInfo.find(Type);
    if (it == m_EffectInfo.end())
    {
        return false;
    }
    info = it->second;
    return true;

}