#include "stdafx.h"
#include "NewSounds.h"
#include "LuaStack.hpp"
#include "DSPlaySound.h"

using namespace luaaa;

CNewSounds gNewSounds;

void LoadNewSound(int id, char* path, int channel, int enable3dsound) {
	LoadWaveFile(id, path, channel, enable3dsound);
}

void CNewSounds::Init() {
	LuaModule(this->m_Lua.GetState()).fun("LoadSound", &LoadNewSound);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomNewSounds.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadSounds", "");
}
