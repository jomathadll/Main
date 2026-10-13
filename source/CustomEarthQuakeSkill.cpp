#include "stdafx.h"
#include "CustomEarthQuakeSkill.h"
#include "HelperSystem.h"
#include "LuaStack.hpp"

using namespace luaaa;

CCustomEarthQuakeSkill gCustomEarthQuakeSkill;

namespace
{
	void SetCustomEarthquake(int itemIndex, int effectCode, int colorR, int colorG, int colorB, int effectSize, int skillPrimary, int skillSecondary)
	{
		CUSTOM_EARTHQUAKE_EFFECT info;
		info.EffectCode = effectCode;
		info.ColorR = (colorR < 0) ? -1.0f : (float)colorR / 255.0f;
		info.ColorG = (colorG < 0) ? -1.0f : (float)colorG / 255.0f;
		info.ColorB = (colorB < 0) ? -1.0f : (float)colorB / 255.0f;
		info.EffectSize = effectSize;
		info.SkillPrimary = skillPrimary;
		info.SkillSecondary = skillSecondary;

		gCustomEarthQuakeSkill.m_Effects[itemIndex].push_back(info);
	}
}

CCustomEarthQuakeSkill::CCustomEarthQuakeSkill() : m_Lua()
{
	this->m_Effects.clear();
}

CCustomEarthQuakeSkill::~CCustomEarthQuakeSkill()
{
	this->m_Effects.clear();
}

void CCustomEarthQuakeSkill::Load()
{
	LuaModule(this->m_Lua.GetState()).fun("SetCustomEarthquake", &SetCustomEarthquake);

	char path[MAX_PATH] = { 0 };
	sprintf_s(path, "%s\\Configs\\CustomEarthquakeSkill.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(path);
	this->m_Lua.Generic_Call("StartLoadEarthquake", "");
}

void CCustomEarthQuakeSkill::Init()
{
	this->Load();
}

void CCustomEarthQuakeSkill::RestartLua()
{
	this->m_Lua.CloseLua();
	this->m_Lua.RegisterLua();
	this->m_Effects.clear();
	this->Load();
}

const std::vector<CUSTOM_EARTHQUAKE_EFFECT>* CCustomEarthQuakeSkill::GetEffects(int modelIndex)
{
	int itemIndex = gHelperSystem.GetModelComplete(modelIndex);
	std::map<int, std::vector<CUSTOM_EARTHQUAKE_EFFECT> >::iterator it = this->m_Effects.find(itemIndex);

	if(it == this->m_Effects.end())
	{
		return 0;
	}

	return &it->second;
}
