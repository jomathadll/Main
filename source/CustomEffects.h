#pragma once

#include "Lua.h"
#include "LuaEffectsNormal.h"
#include "LuaBMD.h"
#include "LuaGlobal.h"

class CCustomEffects {
public:
	CCustomEffects();
	~CCustomEffects();

	void Init();
	void RestartLua();
	void SetInfo(DWORD ObjectStruct, BMD* BMDStruct, int Type, int Level, int PK);

private:
	Lua m_Lua;
	CLuaEffectNormal m_LuaEffects;
	LuaBMD m_LuaBMD;
};

extern CCustomEffects gCustomEffects;