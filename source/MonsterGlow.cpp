#include "stdafx.h"
#include "MonsterGlow.h"
//#include "CustomMonsterEffect.h"
#include "LuaStack.hpp"
#include "GMAida.h"
#include "LuaObject.h"

using namespace luaaa;

CMonsterGlow gMonsterGlow;

CMonsterGlow::CMonsterGlow() : m_Lua()
{
}

CMonsterGlow::~CMonsterGlow()
{
}

void CMonsterGlow::Init()
{
	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterGlow.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

void CMonsterGlow::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterGlow.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

bool CMonsterGlow::RenderMonsterCustomObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon)
{
	int retorno = 0;

	if (pObject != 0 && pModel != 0 && pObject->Type >= MODEL_MONSTER01 && pObject->Type < MODEL_MONSTER_END)
	{
		int MonsterID = pObject->Type - MODEL_MONSTER01;

		gMonsterGlow.m_Lua.Generic_Call("RenderMonster", "iii>i", (DWORD)pModel, (DWORD)pObject, MonsterID, &retorno);
	}

	if (retorno)
		return true;
	else
		return M33Aida::RenderAidaMonsterObjectMesh(pObject, pModel, ExtraMon);	
}
