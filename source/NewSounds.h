#pragma once
#include "Lua.h"

class CNewSounds
{
public:
	CNewSounds() : m_Lua() {};
	~CNewSounds() = default;

	void Init();

private:
	Lua m_Lua;
};

extern CNewSounds gNewSounds;