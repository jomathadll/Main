#pragma once
#include "Lua.h"
#include "LuaGlobal.h"

struct RING_SKIN_MODEL
{
	int Index;
	char ModelName[32];
	char Dir[256];
	bool Type;
	float Size;
	int CapeFront;
	int CapeBack;
};

class CRingTransform
{
public:
	CRingTransform();
	~CRingTransform();

	void Init();
	void RestartLua();
	int CRingTransformCapeFront(int Index);
	int CRingTransformCapeBack(int Index);
	float CRingTransformSize(int Index);

	std::map<int, RING_SKIN_MODEL> m_RingSkinModel;

private:
	Lua m_Lua;
	LuaGlobal m_LuaGLobal;

};

extern CRingTransform gRingTransform;