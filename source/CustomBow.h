#pragma once

#include "Lua.h"

struct CUSTOM_BOW_INFO 
{
	int ItemIndex;
	int Type;
	int Skill;
	int EffectLevel;
	float ColorR;
	float ColorG;
	float ColorB;
};

class CCustomBow 
{
public:
	CCustomBow();
	~CCustomBow();

	void Init();
	void RestartLua();
	bool CheckIsBow(int ItemIndex);
	bool CheckIsCrossBow(int ItemIndex);
	int GetSkillNumber(int ItemIndex);
	int GetEffectLevel(int ItemIndex);
	void GetEffectColor(int ItemIndex, vec3_t light);

private:
	Lua m_Lua;

public:
	std::map<int, CUSTOM_BOW_INFO> m_CustomBowInfo;
};

extern CCustomBow gCustomBow;