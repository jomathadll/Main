#pragma once

#include "Lua.h"

class CLoadImages {
public:
	CLoadImages();
	~CLoadImages();

	void Init();
	void Unload();
	void RestartLua();
	void SetFunctions(lua_State* lua);

	int luaStartImage = BITMAP_AUTOLOAD_START;

private:
	Lua m_Lua;
	bool m_fisrtCall;
};

extern CLoadImages gLoadImages;