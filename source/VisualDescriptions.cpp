#include "stdafx.h"
#include "VisualDescriptions.h"
#include "LuaStack.hpp"
#include "ZzzInventory.h"
#include "DrawBarItem.h"

using namespace luaaa;

cVisuals Visuals;

void CreateVisualDescription(int Line, char* text, int color, int bold) 
{
	wsprintf(TextList[Line], text);
	TextListColor[Line] = color;
	TextBold[Line] = bold;
}

void setVisualInventory(int switchVisual, int originalDescription) 
{
	Visuals.VisualSwitch = switchVisual;

	Visuals.VisualOriginalDescription = originalDescription;
}

void LuaSetItemHybridName(int ItemIndex, char* itemName) {
	ITEM_HYBRID_NAME info;

	info.ItemIndex = ItemIndex;

	sprintf_s(info.Name, "%s", itemName);

	Visuals.ItemHybridNameList.insert(std::make_pair(ItemIndex, info));
}


void cVisuals::Init()
{
	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("createDescription", &CreateVisualDescription);

	LuaModule(this->m_Lua.GetState()).fun("setVisualInventory", &setVisualInventory);

	LuaModule(this->m_Lua.GetState()).fun("SetItemHybridName", &LuaSetItemHybridName);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\DescriptionsItemsVisual.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("RegisterItemHybridName", "");
}

void cVisuals::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("createDescription", &CreateVisualDescription);

	LuaModule(this->m_Lua.GetState()).fun("setVisualInventory", &setVisualInventory);

	LuaModule(this->m_Lua.GetState()).fun("SetItemHybridName", &LuaSetItemHybridName);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\DescriptionsItemsVisual.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("RegisterItemHybridName", "");
}

void cVisuals::SetDescriptionsItem(int Line, ITEM* itemObject)
{
	this->DescriptionsCall(Line, itemObject);
}

int cVisuals::DescriptionsCall(int Line, ITEM* itemObject)
{
	if (itemObject == 0)
	{
		return Line;
	}

	for (int i = 0; i < 50; ++i) 
	{
		memset(TextList[i], 0, 100);
		TextListColor[i] = 0;
		TextBold[i] = 0;
	}

	gDrawBarItem.GetItemBorderColor(itemObject->Type, ((int)itemObject->Level / 8 & 15));

	Visuals.m_Lua.Generic_Call("RenderDescriptionVisualItem", "i>i", itemObject->Type, &Line);
	return Line;
}

char* cVisuals::GetItemHybridName(int ItemIndex) {
	auto it = this->ItemHybridNameList.find(ItemIndex);

	if (it != this->ItemHybridNameList.end())
	{
		return it->second.Name;
	}

	auto itemAtt = &ItemAttribute[ItemIndex];

	if (itemAtt != nullptr) {
		ITEM_HYBRID_NAME info;

		info.ItemIndex = ItemIndex;

		if (itemAtt->m_byItemSlot >= 240 && itemAtt->m_byItemSlot <= 248) {
			sprintf_s(info.Name, "%s", itemAtt->Name);
		}
		else {
			sprintf_s(info.Name, "%s [V]", itemAtt->Name);
		}

		this->ItemHybridNameList.insert(std::make_pair(ItemIndex, info));
	}

	return this->GetItemHybridName(ItemIndex);
}