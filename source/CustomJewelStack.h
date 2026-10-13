#pragma once
#include "Lua.h"

class CJewelStack
{
public:
	CJewelStack();
	~CJewelStack() = default;

	void Init();
	bool CheckIsStack(int ItemIndex);

	std::vector<int> m_JewelList;

private:
	Lua m_Lua;

};

extern CJewelStack gJewelStack;