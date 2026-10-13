#include "stdafx.h"
#include "Descriptions.h"
#include "LuaStack.hpp"
#include "LuaUser.h"
#include "LuaItemObject.h"
#include "UIControls.h"
#include "ZzzInventory.h"
#include "DrawBarItem.h"

using namespace luaaa;

CDescriptions gDescriptions;

CDescriptions::CDescriptions() : m_Lua()
{
	this->m_ColorFull.clear();
}

CDescriptions::~CDescriptions()
{

}
void CDescriptions::SetDescriptionsItem(int Line, ITEM* itemObject)
{
	this->DescriptionsCall(Line, itemObject);
}

int CDescriptions::DescriptionsCall(int Line, ITEM* itemObject)
{
	if (itemObject == 0) 
	{
		return Line;
	}

	gDrawBarItem.GetItemBorderColor(itemObject->Type, ((int)itemObject->Level / 8 & 15));

	gDescriptions.m_Lua.Generic_Call("SetDescriptions", "iij>i", Line, itemObject->Type, itemObject, &Line);
	return Line;
}

void CreateDescription(int Line, char* text, int color, int bold) 
{
	wsprintf(TextList[Line], text);
	TextListColor[Line] = color;
	TextBold[Line] = bold;
}

void AddNewColorBackGroundDescription(int Index, int fR, int fG, int fB, int fA, int bR, int bG, int bB, int bA) 
{
	NEW_COLOR_FULL color;
	color.fR = fR;
	color.fG = fG;
	color.fB = fB;
	color.fA = fA;
	color.bR = bR;
	color.bG = bG;
	color.bB = bB;
	color.bA = bA;
	gDescriptions.m_ColorFull.insert(std::make_pair(Index, color));
}

void CDescriptions::SetNewColorBackGroundDescription(int colorID)
{
	if (gDescriptions.m_ColorFull.count(colorID) > 0) 
	{
		g_pRenderText->SetBgColor(gDescriptions.m_ColorFull[colorID].bR, gDescriptions.m_ColorFull[colorID].bG, gDescriptions.m_ColorFull[colorID].bB, gDescriptions.m_ColorFull[colorID].bA);
		g_pRenderText->SetTextColor(gDescriptions.m_ColorFull[colorID].fR, gDescriptions.m_ColorFull[colorID].fG, gDescriptions.m_ColorFull[colorID].fB, gDescriptions.m_ColorFull[colorID].fA);
	}
}

void CDescriptions::Init()
{
	RegisterLuaItemObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("createDescription", &CreateDescription);
	LuaModule(this->m_Lua.GetState()).fun("AddNewColorDescription", &AddNewColorBackGroundDescription);

	this->m_LuaUser.RegisterLuaUser(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\Descriptions.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	gDescriptions.m_Lua.Generic_Call("SetNewColorDescription", "i>");
}

void CDescriptions::RestartLua()
{
	this->m_Lua.CloseLua();
	this->m_Lua.RegisterLua();

	this->m_ColorFull.clear();
	RegisterLuaItemObject(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("createDescription", &CreateDescription);
	LuaModule(this->m_Lua.GetState()).fun("AddNewColorDescription", &AddNewColorBackGroundDescription);

	this->m_LuaUser.RegisterLuaUser(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\Descriptions.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	gDescriptions.m_Lua.Generic_Call("SetNewColorDescription", "i>");
}