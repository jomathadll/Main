#pragma once
#pragma once
#pragma once

#include "Lua.h"
#include "LuaGlobal.h"

class CRuneColorEffect
{
public:
    CRuneColorEffect();
    ~CRuneColorEffect();

    void Init();
    void RestartLua();
    void IsRuneEffect(DWORD characterObj, DWORD ObjectStruct);

private:
    Lua m_Lua;
    LuaGlobal m_LuaGlobal;
};

extern CRuneColorEffect gRuneColorEffect;