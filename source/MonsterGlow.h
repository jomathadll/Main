#pragma once

#include "Lua.h"
#include "LuaBMD.h"
#include "LuaGlobal.h"

class CMonsterGlow 
{
public:
	CMonsterGlow();

	~CMonsterGlow();

	void Init();
	void RestartLua();

	static bool RenderMonsterCustomObjectMesh(OBJECT* pObject, BMD* pModel, bool ExtraMon);

private:
	Lua m_Lua;
	LuaBMD m_LuaBMD;
	LuaGlobal m_LuaGlobal;
};

extern CMonsterGlow gMonsterGlow;
