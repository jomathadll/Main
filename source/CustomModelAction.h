#pragma once
#pragma once
#include "Lua.h"
#include "LuaGlobal.h"

struct CUSTOM_MODEL_ACTION
{
	int ItemIndex;
	int ActionStop;
	int ActionRun;
	int ActionSafeZone;
};

class CSetModelAction
{
public:
	CSetModelAction();
	~CSetModelAction();

	void Init();
	void RestartLua();
	int IsSetModelAction(DWORD characterObj);

	std::map<int, CUSTOM_MODEL_ACTION> m_SetModelAction;

private:
	Lua m_Lua;
	LuaGlobal m_LuaGLobal;

};

extern CSetModelAction gSetModelAction;