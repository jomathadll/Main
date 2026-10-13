// ItemManager.cpp: implementation of the CItemManager class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemManager.h"
#include "LuaStack.hpp"
#include "ZzzObject.h"
#include "HelperSystem.h"
#include "DarkSpirit.h"
#include "LoadData.h"

using namespace luaaa;

CItemManager gItemManager;

CItemManager::CItemManager() : m_Lua() // OK
{
	this->m_CustomItemInfo.clear();
}

CItemManager::~CItemManager() // OK
{
	this->m_CustomItemInfo.clear();
}

void SetLoadItem(int ItemIndex, int colorR, int colorG, int ColorB, char* Dir, char* ModelName, int type)
{
	CUSTOM_ITEM_INFO info;

	info.ItemIndex = ItemIndex;

	info.ColorR = static_cast<float>(static_cast<float>(colorR) / static_cast<float>(255));

	info.ColorG = static_cast<float>(static_cast<float>(colorG) / static_cast<float>(255));

	info.ColorB = static_cast<float>(static_cast<float>(ColorB) / static_cast<float>(255));

	strcpy_s(info.ModelName, ModelName);

	strcpy_s(info.Dir, Dir);

	info.Type = type;

	gItemManager.m_CustomItemInfo.insert(std::make_pair(info.ItemIndex, info));
}

void CItemManager::Init() // OK
{
	LuaModule(this->m_Lua.GetState()).fun("LoadItem", &SetLoadItem);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\LoadItens.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadItens", "");
}

BOOL CItemManager::GetCustomItemColor(int Type, vec3_t Light, float Bright) // OK
{
	std::map<int, CUSTOM_ITEM_INFO>::iterator it = gItemManager.m_CustomItemInfo.find(Type);

	if (it == gItemManager.m_CustomItemInfo.end()) 
	{
		if (gHelperSystem.GetHelper(Type) != 0 || gDarkSpirit.checkIsDarkSpirit(Type - MODEL_ITEM)) {
			Vector(0, 0, 0, Light);
			return 1;
		}
		return 0;
	}

	auto lpInfo = &(*it).second; 


	Vector(Bright * lpInfo->ColorR, Bright * lpInfo->ColorG, Bright * lpInfo->ColorB, Light);

	return 1;
}

int CItemManager::GET_ITEM(int section, int index)
{
	return ((section * 512) + index);
}

int CItemManager::GET_ITEM_MODEL(int section, int index)
{
	return (((section * 512) + index) + MODEL_ITEM);
}

void CItemManager::GetItemColor(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon)
{
	if (this->GetCustomItemColor(Type, Light, Bright) == 0)
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}

void CItemManager::GetItemColor2(int Type, float Alpha, float Bright, vec3_t Light, bool ExtraMon)
{
	if (this->GetCustomItemColor(Type, Light, Bright) == 0)
	{
		PartObjectColor2(Type, Alpha, Bright, Light, ExtraMon);
	}
}

void CItemManager::LoadItem(int ItemIndex)
{
	//Novos Itens
	for (std::map<int, CUSTOM_ITEM_INFO>::iterator it = this->m_CustomItemInfo.begin(); it != this->m_CustomItemInfo.end(); it++)
	{
		auto ItemInfo = &(*it).second;

		if (ItemInfo->ItemIndex == ItemIndex) {
			if (ItemInfo->Type == 0)
			{
				gLoadData.AccessModel(ItemInfo->ItemIndex, ItemInfo->Dir, ItemInfo->ModelName);
				gLoadData.OpenTexture(ItemInfo->ItemIndex, ItemInfo->Dir);

			}
			else
			{
				gLoadData.AccessModelCustom(ItemInfo->ItemIndex, ItemInfo->Dir, ItemInfo->ModelName);
				gLoadData.OpenTextureCustom(ItemInfo->ItemIndex, ItemInfo->Dir);

			}
			break;
		}		
	}
}