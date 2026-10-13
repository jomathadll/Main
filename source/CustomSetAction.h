#pragma once
#include "Lua.h"
#include "LuaGlobal.h"

struct CUSTOM_SET_ACTION
{
	int ItemIndex;
	char Command[32];
	int Action;
};

class CSetAction
{
public:
	CSetAction();
	~CSetAction();

	void Init();
	void RestartLua();
	int IsSetAction(DWORD characterObj);

	std::map<int, CUSTOM_SET_ACTION> m_SetAction;

private:
	Lua m_Lua;
	LuaGlobal m_LuaGLobal;

};

extern CSetAction gSetAction;