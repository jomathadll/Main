#include "stdafx.h"
#include "CustomMeshGlow.h"
#include "LuaStack.hpp"

CCustomMeshGlow gCustomMeshGlow;

CCustomMeshGlow::CCustomMeshGlow() : m_Lua()
{
	this->m_MeshList.clear();
}

CCustomMeshGlow::~CCustomMeshGlow()
{

}

void AddItemMesh(int ItemIndex, int Mesh)
{
	ItemIndex += MODEL_ITEM;

	MESH_GLOW info;

	info.ItemIndex = ItemIndex;
	info.Mesh = Mesh;

	gCustomMeshGlow.m_MeshList.push_back(info);
}

void CCustomMeshGlow::Init()
{
	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadMeshGlow", &AddItemMesh);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomMeshGlow.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadMeshGlow", ">");
}

void CCustomMeshGlow::RestartLua()
{
	this->m_MeshList.clear();

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadMeshGlow", &AddItemMesh);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomMeshGlow.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadMeshGlow", ">");
}