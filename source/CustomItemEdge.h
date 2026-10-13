#pragma once

#include "Lua.h"
#include "LuaBMD.h"
#include "LuaInterface.h"

class CItemEdge
{
public:
	CItemEdge();
	~CItemEdge() = default;

	void Init();
	void RestartLua();
	bool RenderItemEdge(BMD *b, OBJECT *o, int ItemIndex);

	Lua m_Lua;
private:
	LuaBMD m_LuaBMD;
	LuaInterface m_LuaInterface;
};
extern CItemEdge gItemEdge;