#include "stdafx.h"
#include "CharacterList.h"
#include "LuaStack.hpp"
#include "Protect.h"
#include "UIMng.h"
#include "ZzzInterface.h"

using namespace luaaa;

CCharacterList gCharacterList;

extern bool LogOut;

CCharacterList::CCharacterList() : m_Lua()
{

}

CCharacterList::~CCharacterList()
{

}

int CheckCreateCharacterWindow() 
{
	CUIMng& rUIMng = CUIMng::Instance();

	return rUIMng.m_CharMakeWin.IsShow();
}

int CheckCharacterListActionVisible()
{
	return (LogOut || IsQuitGameNoticeActive()) ? 0 : 1;
}

void SetMaxCharacter(int value) 
{
	gCharacterList.MaxCharacters = value;
}

void SetPetSize(float MountSize, float FlyingSize, float FlyingHeight) 
{
	gCharacterList.MountAddSize = MountSize;
	gCharacterList.FlyingAddSize = FlyingSize;
	gCharacterList.FlyingAddHeight = FlyingHeight;
}

void CCharacterList::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetPetSize", &SetPetSize);

	LuaModule(this->m_Lua.GetState()).fun("SetMaxCharacter", &SetMaxCharacter);

	LuaModule(this->m_Lua.GetState()).fun("CheckCreateCharacterWindow", &CheckCreateCharacterWindow);

	LuaModule(this->m_Lua.GetState()).fun("CheckCharacterListActionVisible", &CheckCharacterListActionVisible);

	this->m_LuaCharacter.RegisterLuaCharacter(this->m_Lua.GetState());

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	this->m_LuaLoadImage.SetFunctions(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CharacterList.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

void CCharacterList::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	LuaModule(this->m_Lua.GetState()).fun("SetPetSize", &SetPetSize);

	LuaModule(this->m_Lua.GetState()).fun("SetMaxCharacter", &SetMaxCharacter);

	LuaModule(this->m_Lua.GetState()).fun("CheckCreateCharacterWindow", &CheckCreateCharacterWindow);

	LuaModule(this->m_Lua.GetState()).fun("CheckCharacterListActionVisible", &CheckCharacterListActionVisible);

	this->m_LuaCharacter.RegisterLuaCharacter(this->m_Lua.GetState());

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	this->m_LuaLoadImage.SetFunctions(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CharacterList.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}


void CCharacterList::OpenCharacterSceneData()
{
	gCharacterList.m_Lua.Generic_Call("StartLoadImages", ">");
}

void CCharacterList::MoveCharacterList()
{
	gCharacterList.m_Lua.Generic_Call("UpdateProc", "i>", gCharacterList.MaxCharactersAccount);
}

void CCharacterList::RenderCharacterList()
{
	gCharacterList.m_Lua.Generic_Call("RenderProc", "i>", gCharacterList.MaxCharactersAccount);
}

void CCharacterList::SetCharacterPosition(int slot)
{
	gCharacterList.m_Lua.Generic_Call("SetCharacterPosition", "i>", slot);
}
