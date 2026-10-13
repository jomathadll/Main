#include "stdafx.h"
#include "CustomRingTransform.h"
#include "LuaStack.hpp"
#include "LuaGlobal.h"

using namespace luaaa;

CRingTransform gRingTransform;

CRingTransform::CRingTransform() : m_Lua()
{
	this->m_RingSkinModel.clear();
}

CRingTransform::~CRingTransform()
{

}

void AddModels(int Index, char* Dir, char* ModelName, int type, float Size, int CapeFront, int CapeBack) {

	Index += MODEL_NEW_TRANSFORM;

	RING_SKIN_MODEL info;

	info.Index = Index;

	strcpy_s(info.ModelName, ModelName);

	strcpy_s(info.Dir, Dir);

	info.Type = type;

	info.Size = Size;

	info.CapeFront = CapeFront;

	info.CapeBack = CapeBack;

	gRingTransform.m_RingSkinModel.insert(std::make_pair(info.Index, info));
}

void CRingTransform::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("AddLoadModel", &AddModels);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomRingTransformation.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadSkins", ">");
}

void CRingTransform::RestartLua()
{
	this->m_RingSkinModel.clear();

	LuaModule(this->m_Lua.GetState()).fun("AddLoadModel", &AddModels);

	this->m_LuaGLobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CustomRingTransformation.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadSkins", ">");
}

float CRingTransform::CRingTransformSize(int Index)
{
	std::map<int, RING_SKIN_MODEL>::iterator it = this->m_RingSkinModel.find(Index);

	if (it == this->m_RingSkinModel.end())
	{
		return 1.f;
	}

	return it->second.Size;
}

int CRingTransform::CRingTransformCapeFront(int Index)
{
	std::map<int, RING_SKIN_MODEL>::iterator it = this->m_RingSkinModel.find(Index);

	if (it == this->m_RingSkinModel.end())
	{
		return -1;
	}

	return it->second.CapeFront;
}

int CRingTransform::CRingTransformCapeBack(int Index)
{
	std::map<int, RING_SKIN_MODEL>::iterator it = this->m_RingSkinModel.find(Index);

	if (it == this->m_RingSkinModel.end())
	{
		return -1;
	}

	return it->second.CapeBack;
}