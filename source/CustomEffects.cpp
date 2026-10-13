#include "stdafx.h"
#include "CustomEffects.h"
#include "LuaObject.h"
#include "ZzzScene.h"

CCustomEffects gCustomEffects;

CCustomEffects::CCustomEffects() : m_Lua()
{

}

CCustomEffects::~CCustomEffects()
{

}

void CCustomEffects::Init()
{
	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\EffectSystem\\CharacterEffectItens.lua", LUA_LOAD_PATH);

	this->m_LuaEffects.RegisterLuaClass(this->m_Lua.GetState());
	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());
	this->m_Lua.DoFile(name);
	RegisterClassObject(this->m_Lua.GetState());

}

void CCustomEffects::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\EffectSystem\\CharacterEffectItens.lua", LUA_LOAD_PATH);
	this->m_LuaEffects.RegisterLuaClass(this->m_Lua.GetState());
	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());
	this->m_Lua.DoFile(name);
	RegisterClassObject(this->m_Lua.GetState());

}


void CCustomEffects::SetInfo(DWORD ObjectStruct, BMD* BMDStruct, int Type, int Level, int PK)
{
	if (SceneFlag == CHARACTER_SCENE)
	{
		return;
	}

	auto o = (OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
	{
		gCustomEffects.m_Lua.Generic_Call("CharacterItensEffect", "jjiii>", ObjectStruct, BMDStruct, Type, Level, PK);
	}
}
