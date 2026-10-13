#include "stdafx.h"
#include "RuneColorEffect.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"
#include "GMBattleCastle.h"
#include "LuaObject.h"
#include "ZzzLodTerrain.h"
#include "ZzzScene.h"

using namespace luaaa;

CRuneColorEffect gRuneColorEffect;

CRuneColorEffect::CRuneColorEffect() : m_Lua()
{

}

CRuneColorEffect::~CRuneColorEffect()
{

}

void LuaRenderAurora(int Type, int RenderType, float size, float r, float g, float b, bool render, DWORD ObjectStruct)
{
    auto o = &*(OBJECT*)ObjectStruct;
    if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
    {
        vec3_t Light;
        Vector(r, g, b, Light);

        if (render)
        {
            float fLumi = sinf(WorldTime * 0.0015f) * 0.3f + 0.5f;
            Vector(fLumi * Light[0], fLumi * Light[1], fLumi * Light[2], Light);
        }

        RenderAurora(Type, RenderType, o->Position[0], o->Position[1], size, size, Light);
    }
}

void LuaRenderAurora2(int Type, int RenderType, float size, float r, float g, float b, float speed, float alpha, bool render, DWORD ObjectStruct)
{
    auto o = &*(OBJECT*)ObjectStruct;
    if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
    {
        vec3_t Light;
        Vector(r, g, b, Light);

        if (render)
        {
            float fLumi = sinf(WorldTime * 0.0015f) * 0.3f + 0.5f;
            Vector(fLumi * Light[0], fLumi * Light[1], fLumi * Light[2], Light);
        }

        RenderAurora2(Type, RenderType, o->Position[0], o->Position[1], size, size, Light, WorldTime * speed, alpha);
    }
}

void LuaRenderTerrain(int Type, int RenderType, float size, float r, float g, float b, float speed, float alpha, float height, bool render, DWORD ObjectStruct)
{
    auto o = &*(OBJECT*)ObjectStruct;
    if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
    {
        vec3_t Light;
        Vector(r * alpha, g * alpha, b * alpha, Light);

        if (render)
        {
            float fLumi = sinf(WorldTime * 0.0015f) * 0.3f + 0.5f;
            Vector(fLumi * Light[0], fLumi * Light[1], fLumi * Light[2], Light);
        }

        RenderTerrainAlphaBitmap(Type, o->Position[0], o->Position[1], size, size, Light, WorldTime * speed, alpha, height);
    }
}

void CRuneColorEffect::RestartLua()
{
    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    RegisterClassObject(this->m_Lua.GetState());

    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("RenderAurora", &LuaRenderAurora);

    LuaModule(this->m_Lua.GetState()).fun("RenderAurora2", &LuaRenderAurora2);

    LuaModule(this->m_Lua.GetState()).fun("RenderTerrain", &LuaRenderTerrain);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorRuneEffect.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);
}

void CRuneColorEffect::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    RegisterClassObject(this->m_Lua.GetState());

    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("RenderAurora", &LuaRenderAurora);

    LuaModule(this->m_Lua.GetState()).fun("RenderAurora2", &LuaRenderAurora2);

    LuaModule(this->m_Lua.GetState()).fun("RenderTerrain", &LuaRenderTerrain);

    char name[MAX_PATH] = { 0 };

    sprintf_s(name, "%s\\SkillColorEffect\\ColorRuneEffect.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);
}

void CRuneColorEffect::IsRuneEffect(DWORD characterObj, DWORD ObjectStruct)
{
    if (SceneFlag == CHARACTER_SCENE)
    {
        return;
    }

    this->m_Lua.Generic_Call("StartRuneEffect", "jj>", characterObj, ObjectStruct);
}
