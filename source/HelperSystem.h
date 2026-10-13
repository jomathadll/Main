#pragma once

#include "Lua.h"
#include "LuaBMD.h"
#include "LuaEffects.h"
#include "LuaGlobal.h"

struct HELPER_EFFECT_INFO {
	int EffectType;
	int EffectIndex;
	int EffectLevel;
	int Skeleton;
	float Size;
	float LightR;
	float LightG;
	float LightB;
	float PosX;
	float PosY;
	float PosZ;
	int randTime;
	int Black;
};

struct HELPER_EFFECT_MAIN {
	std::vector<HELPER_EFFECT_INFO> m_Effect;
};

class HELPER_INFO {
public:
	HELPER_INFO() {
		this->ItemIndex = -1;
	};

	~HELPER_INFO() {

	};

	int ItemIndex;
	int Type;
	int Movement;
	int DefenseSkill;
	int SubType;
	float HeightFloor;
	float Size;
	float SizeCharList;
	char Model[32];
	char ModelDir[256];
	char ObjectModel[32];
	char ObjectModelDir[256];
	int ModelID;
	int Miniature;
	float SizeMiniature;
	float VelocityMiniature;
	bool FileType;
};

class CHelperSystem
{
public:
	CHelperSystem();
	~CHelperSystem();

	void Init();
	void RestartLua();
	int GetHelperModel(int ItemIndex);
	bool CheckIsHelper(int ItemIndex);
	bool CheckHelperType(int ItemIndex, int value);
	int GetHelperType(int ItemIndex);
	int GetHelperSubType(int ItemIndex);
	bool CheckHelperDefenseSkill(int ItemIndex);
	int GetModelComplete(int Index);
	HELPER_INFO* GetHelper(int ItemIndex);
	bool GetCanUseShield(int ItemIndex);

	void InvCreateEquippingEffect(DWORD ItemIndex);
	int CheckIsFenrirOrDino(int ItemIndex);
public:
	std::map<int, HELPER_INFO> m_HelperInfo;
	std::map<int, HELPER_EFFECT_MAIN> m_HelperEffectInfo;
	std::map<int, HELPER_EFFECT_MAIN> m_HelperMiniatureEffectInfo;

public:
	Lua m_Lua;
	LuaBMD m_LuaBMD;
	CLuaEffects m_LuaEffects;
	LuaGlobal m_LuaGlobal;
};

extern CHelperSystem gHelperSystem;