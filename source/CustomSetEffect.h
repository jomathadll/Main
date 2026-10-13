#pragma once

#include "Lua.h"
#include "LuaBMD.h"
#include "LuaGlobal.h"
#include "LuaObject.h"
#include <unordered_map>

struct EFFECT_15GRADE
{
	int ItemIndex;
	bool NeedFullSet;
	int EffectLevel;
	float Red;
	float Green;
	float Blue;
};

struct SMOKE_EFFECT
{
	int ItemIndex;
	float Red;
	float Green;
	float Blue;
};

class CCustomSetEffect {
public:
	CCustomSetEffect();
	~CCustomSetEffect();
	void CreateEffectSetPlayer(CHARACTER* c, DWORD ObjectStruct, DWORD ItemIndex, int level, int PK);

	void Init();
	void RestartLua();
	bool GetEffect15Grade(CHARACTER* c, int ItemIndex);
	bool GetSmokeEffect(int ItemIndex);
	void GetSmokeColor(int ItemIndex, float* Red, float* Green, float* Blue);

public:
	Lua m_Lua;
	LuaGlobal m_LuaGlobal;
	std::vector<EFFECT_15GRADE> haveEffect15;
	std::unordered_map<int, SMOKE_EFFECT> m_CustomSmokeEffect;
	LuaBMD m_LuaBMD;
};

extern CCustomSetEffect gCCustomSetEffect;