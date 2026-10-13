#include "stdafx.h"
#include "HelperSystem.h"
#include "ZzzCharacter.h"
#include "ZzzLodTerrain.h"
#include "ZzzEffect.h"
#include "HelperManager.h"
#include "MapManager.h"
#include "LuaStack.hpp"
#include "LuaObject.h"

using namespace luaaa;

CHelperSystem gHelperSystem;

CHelperSystem::CHelperSystem() : m_Lua()
{
	this->m_HelperInfo.clear();
}

CHelperSystem::~CHelperSystem()
{

}

void SetHelper(int ItemIndex, int Type, int SubType, int Movement, int DefenseSkill, float Height, float Size, float SizeCharList, int miniature, float sizeMiniature, float velocityMiniature, std::string ModelDir, std::string Model, std::string ObjectModelDir, std::string ObjectModel, int FileType)
{
	auto it = gHelperSystem.m_HelperInfo.find(ItemIndex);

	if (it != gHelperSystem.m_HelperInfo.end()) {
		return;
	}
	HELPER_INFO info;

	info.ItemIndex = ItemIndex;

	info.Type = (Type == 1 ? 2 : Type == 2 ? 4 : Type == 3 ? 8 : Type == 4 ? 16 : Type == 5 ? 32 : Type);

	info.DefenseSkill = DefenseSkill;

	info.SubType = SubType;

	info.Movement = Movement;

	info.HeightFloor = Height;

	info.Size = Size;

	info.SizeCharList = SizeCharList;

	info.Miniature = miniature;

	info.SizeMiniature = sizeMiniature;

	info.VelocityMiniature = velocityMiniature;

	strcpy_s(info.Model, Model.c_str());

	strcpy_s(info.ModelDir, ModelDir.c_str());

	strcpy_s(info.ObjectModel, ObjectModel.c_str());

	strcpy_s(info.ObjectModelDir, ObjectModelDir.c_str());

	info.FileType = FileType;

	gHelperSystem.m_HelperInfo.insert(std::make_pair(ItemIndex, info));
};

int CheckModelById(int ItemIndex)
{
	return gHelperSystem.GetHelperModel(ItemIndex);
}

bool CHelperSystem::CheckIsHelper(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (ItemIndex == 15)	//	Fix Objects Devias
		{
			return 0;
		}

		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) 
		{
			return 1;
		}
	}

	return 0;
}

bool CHelperSystem::CheckHelperType(int ItemIndex, int value)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) {
			return (it->second.Type & value) == value;
		}
	}

	return 0;
}

int CHelperSystem::GetHelperType(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) {
			return it->second.Type;
		}
	}

	return 0;
}

int CHelperSystem::GetHelperSubType(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) {
			return it->second.SubType;
		}
	}

	return 0;
}

int CHelperSystem::GetHelperModel(int ItemIndex)
{
	std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.find(ItemIndex);

	if (it == this->m_HelperInfo.end()) {
		return 0;
	}
	
	return it->second.ModelID;
}

HELPER_INFO* CHelperSystem::GetHelper(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex)
		{
			return &it->second;
		}
	}

	return 0;
}

void CHelperSystem::InvCreateEquippingEffect(DWORD ItemIndex)
{
	if (gMapManager.InChaosCastle() == true) return;

	auto pHeroObject = Hero;

	gHelperManager.CreateHelper(ItemIndex, gHelperSystem.GetHelperModel(pHeroObject->Helper.Type), pHeroObject->Object.Position, pHeroObject, 0, gHelperSystem.GetHelperSubType(pHeroObject->Helper.Type));

	if (!Hero->SafeZone && gHelperSystem.GetHelperType(pHeroObject->Helper.Type) > 0)
	{
		CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Object.Position, pHeroObject->Object.Angle, pHeroObject->Object.Light, 1, &pHeroObject->Object);
	}
}

int CHelperSystem::CheckIsFenrirOrDino(int ItemIndex)
{
	if (gHelperSystem.CheckHelperType(ItemIndex, 2) == 1 || gHelperSystem.CheckHelperType(ItemIndex, 4) == 1)
	{
		return 1;
	}

	return 0;
}

bool CHelperSystem::GetCanUseShield(int ItemIndex) {
	if (CharacterMachine->Equipment[8].Type-MODEL_ITEM != ItemIndex) {
		ItemIndex = CharacterMachine->Equipment[8].Type - MODEL_ITEM;
	}

	auto helper = this->GetHelper(ItemIndex);

	if (helper == 0) {
		return 1;
	}

	if (helper->Type == 0) {
		return 1;
	}

	return gHelperSystem.CheckHelperDefenseSkill(ItemIndex);
}

bool CHelperSystem::CheckHelperDefenseSkill(int ItemIndex)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == ItemIndex || it->second.ModelID == ItemIndex) {
			if (it->second.DefenseSkill == 0) {
				return 0;
			}
			return 1;
		}
	}

	return 0;
}

int CHelperSystem::GetModelComplete(int Index)
{
	for (std::map<int, HELPER_INFO>::iterator it = this->m_HelperInfo.begin(); it != this->m_HelperInfo.end(); ++it)
	{
		if (it->second.ItemIndex == Index || it->second.ModelID == Index) {
			return it->second.ItemIndex;
		}
	}

	return 0;
}


void CHelperSystem::Init()
{
	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	this->m_LuaEffects.RegisterLuaClass(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetHelper", &SetHelper);

	LuaModule(this->m_Lua.GetState()).fun("CheckModelById", &CheckModelById);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CharacterHelper.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("LoadHelpers", ">");
}

void CHelperSystem::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	this->m_LuaEffects.RegisterLuaClass(this->m_Lua.GetState());

	gHelperSystem.m_HelperEffectInfo.clear();
	gHelperSystem.m_HelperMiniatureEffectInfo.clear();

	LuaModule(this->m_Lua.GetState()).fun("SetHelper", &SetHelper);

	LuaModule(this->m_Lua.GetState()).fun("CheckModelById", &CheckModelById);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\CharacterHelper.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}