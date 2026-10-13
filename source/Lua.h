#pragma once

extern "C" {
#include <luajit\lua.h>
#include <luajit\lua.hpp>
#include <luajit\lauxlib.h>
}


class Lua {
public:
	Lua();
	virtual ~Lua();

	void RegisterLua();
	void CloseLua();
	lua_State* GetState();
	void DoFile(const char * szFileName);
	bool Generic_Call(const char* func, const char* sig, ...);

public:
	DWORD lastGcRun;
private:
	lua_State* m_luaState;

private:
	//CCriticalSection m_critical;
	//LuaMemory m_LuaMemory;
};