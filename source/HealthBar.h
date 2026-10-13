#pragma once

#include "Lua.h"

#define MAX_MAIN_VIEWPORT 400

extern int SelectedCharacter;

struct NEW_HEALTH_BAR
{
	WORD index;
	BYTE type;
	BYTE rate;
	DWORD life;
	DWORD maxLife;
};

class CHealthBar {
public:
	CHealthBar();
	~CHealthBar();

	void Init();

	void ClearNewHealthBar();
	void InsertNewHealthBar(WORD index, BYTE type, BYTE rate, DWORD life, DWORD maxLife);
	NEW_HEALTH_BAR* GetNewHealthBar(WORD index, BYTE type);
	static void DrawNewHealthBar();

private:
	NEW_HEALTH_BAR gNewHealthBar[MAX_MAIN_VIEWPORT];
};

extern CHealthBar gHealthBar;
