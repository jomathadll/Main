#include "stdafx.h"
#include "CustomBow.h"
#include "LuaStack.hpp"

using namespace luaaa;

CCustomBow gCustomBow;

CCustomBow::CCustomBow() : m_Lua()
{
	this->m_CustomBowInfo.clear();
}

CCustomBow::~CCustomBow()
{
	this->m_CustomBowInfo.clear();
}

void SetBowCross(int ItemIndex, int Type, int Skill, int EffectLevel, float ColorR, float ColorG, float ColorB)
{
	CUSTOM_BOW_INFO info;

	info.ItemIndex = ItemIndex;

	info.Type = Type;

	info.Skill = Skill;

	info.EffectLevel = EffectLevel;

	info.ColorR = ColorR;

	info.ColorG = ColorG;

	info.ColorB = ColorB;

	gCustomBow.m_CustomBowInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CCustomBow::RestartLua()
{
	this->m_CustomBowInfo.clear();

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	LuaModule(this->m_Lua.GetState()).fun("SetBowCross", &SetBowCross);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomBowCross.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadBowCross", "");
}

void CCustomBow::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetBowCross", &SetBowCross);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomBowCross.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadBowCross", "");
}

bool CCustomBow::CheckIsBow(int ItemIndex)
{
	std::map<int, CUSTOM_BOW_INFO>::iterator it = this->m_CustomBowInfo.find(ItemIndex);

	if (it == this->m_CustomBowInfo.end()) 
	{
		return 0;
	}

	return (it->second.Type == 1 ? 1 : 0);
}

bool CCustomBow::CheckIsCrossBow(int ItemIndex)
{
	std::map<int, CUSTOM_BOW_INFO>::iterator it = this->m_CustomBowInfo.find(ItemIndex);

	if (it == this->m_CustomBowInfo.end()) 
	{
		return 0;
	}

	return (it->second.Type == 2 ? 1 : 0);
}

int CCustomBow::GetSkillNumber(int ItemIndex)
{
	std::map<int, CUSTOM_BOW_INFO>::iterator it = this->m_CustomBowInfo.find(ItemIndex);

	if (it == this->m_CustomBowInfo.end()) 
	{
		return 0;
	}

	return it->second.Skill;
}

int CCustomBow::GetEffectLevel(int ItemIndex)
{
	std::map<int, CUSTOM_BOW_INFO>::iterator it = this->m_CustomBowInfo.find(ItemIndex);

	if (it == this->m_CustomBowInfo.end())
	{
		return 0;
	}

	return it->second.EffectLevel;
}

void CCustomBow::GetEffectColor(int ItemIndex, vec3_t light)
{
	std::map<int, CUSTOM_BOW_INFO>::iterator it = this->m_CustomBowInfo.find(ItemIndex);

	if (it == this->m_CustomBowInfo.end())
	{
		Vector(1.f, 1.f, 1.f, light);
		return;
	}

	Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
	return;
}