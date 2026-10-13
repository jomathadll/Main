#include "stdafx.h"
#include "RenderModel.h"
#include "LuaBMD.h"
#include "LuaObject.h"
#include "LuaItemObject.h"
#include "LoadImages.h"

CRenderModel gRenderModel;

CRenderModel::CRenderModel() : m_Lua()
{

}

CRenderModel::~CRenderModel()
{

}

int CRenderModel::RenderModelBodyLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct)
{
	int retorno = 0;
	
	gRenderModel.m_Lua.Generic_Call("RenderModelBody", "iii>i", BMDStruct, ObjectStruct, ItemIndex, &retorno);
	

	return retorno;
}

int CRenderModel::RenderModelBodyColorLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct)
{
	int retorno = 0;

	gRenderModel.m_Lua.Generic_Call("RenderModelBodyColor", "iii>i", BMDStruct, ObjectStruct, ItemIndex, &retorno);

	return retorno;
}

int CRenderModel::RenderModelBodyColor2Lua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct)
{
	int retorno = 0;

	gRenderModel.m_Lua.Generic_Call("RenderModelBodyColor2", "iii>i", BMDStruct, ObjectStruct, ItemIndex, &retorno);

	return retorno;
}

void CRenderModel::Init()
{
	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\RenderModel.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaUser.RegisterLuaUser(this->m_Lua.GetState());

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	gLoadImages.SetFunctions(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	RegisterLuaItemObject(this->m_Lua.GetState());
}

void CRenderModel::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\RenderModel.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaUser.RegisterLuaUser(this->m_Lua.GetState());
	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	RegisterLuaItemObject(this->m_Lua.GetState());
}

void CRenderModel::LoadImageRenderModel()
{
	this->m_Lua.Generic_Call("StartLoadImages", ">");
}