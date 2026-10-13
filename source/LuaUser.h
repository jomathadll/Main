#pragma once

#include "Lua.h"

class CLuaUser {
public:
	CLuaUser() = default;
	~CLuaUser() = default;

	void RegisterLuaUser(lua_State* lua);

};