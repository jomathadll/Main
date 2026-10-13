#pragma once

#include "Lua.h"
#include <map>
#include <vector>

struct CUSTOM_EARTHQUAKE_EFFECT
{
	int EffectCode;
	float ColorR;
	float ColorG;
	float ColorB;
	int EffectSize;
	int SkillPrimary;
	int SkillSecondary;
};

class CCustomEarthQuakeSkill
{
public:
	CCustomEarthQuakeSkill();
	~CCustomEarthQuakeSkill();

	void Init();
	void RestartLua();
	const std::vector<CUSTOM_EARTHQUAKE_EFFECT>* GetEffects(int modelIndex);

private:
	void Load();

private:
	Lua m_Lua;

public:
	std::map<int, std::vector<CUSTOM_EARTHQUAKE_EFFECT> > m_Effects;
};

extern CCustomEarthQuakeSkill gCustomEarthQuakeSkill;
