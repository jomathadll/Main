#include "stdafx.h"
#include "CustomSetEffect.h"
#include "LuaStack.hpp"
#include "ZzzCharacter.h"
#include "ZzzScene.h"

using namespace luaaa;

CCustomSetEffect gCCustomSetEffect;

CCustomSetEffect::CCustomSetEffect() : m_Lua()
{
	this->haveEffect15.clear();
	this->m_CustomSmokeEffect.clear();
}

CCustomSetEffect::~CCustomSetEffect()
{

}

void CCustomSetEffect::CreateEffectSetPlayer(CHARACTER* c, DWORD ObjectStruct, DWORD ItemIndex, int level, int PK)
{
	if (SceneFlag == CHARACTER_SCENE)
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		BMD* BMDStruct = &Models[ItemIndex];

		int index = (ItemIndex - MODEL_ITEM) % 512;

		if (BMDStruct)
		{
			int full = 1;

			for (int i = BODYPART_HELM; i <= BODYPART_BOOTS; i++) {
				if ((c->BodyPart[i].Type - MODEL_ITEM) % 512 != index) {
					full = 0;
				}
			}

			gCCustomSetEffect.m_Lua.Generic_Call("CreateEffectSetPlayer", "jjiiii>", BMDStruct, ObjectStruct, ItemIndex, level, full, PK);
		}
	}
}

void AddEffect15Grade(int ItemIndex, int needfullset, int EffectLevel, float Red, float Green, float Blue)
{
	EFFECT_15GRADE info;
	info.ItemIndex = ItemIndex;
	info.NeedFullSet = needfullset;
	info.EffectLevel = EffectLevel;
	info.Red = Red;
	info.Green = Green;
	info.Blue = Blue;

	gCCustomSetEffect.haveEffect15.push_back(info);
}

void AddSmokeEffect(int ItemIndex, float Red, float Green, float Blue)
{
	int fullIndex = MODEL_BOOTS+ItemIndex;

	SMOKE_EFFECT info;

	info.ItemIndex = ItemIndex;
	info.Red = Red;
	info.Green = Green;
	info.Blue = Blue;

	gCCustomSetEffect.m_CustomSmokeEffect[fullIndex] = info;
}

void CreateSetEffectSprite(DWORD BMDStruct, DWORD ObjectStruct, int BonePlayer, int EffectID, float Scale, float LightR, float LightG, float LightB, float PosX, float PosY, float PosZ, int Black) 
{
	if (SceneFlag == CHARACTER_SCENE)
	{
		return;
	}

	if (BMDStruct == 0 || ObjectStruct == 0) 
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		auto b = (BMD*)BMDStruct;

		if (Object != nullptr)
		{
			vec3_t p, Position;
			vec3_t Light;

			Vector(LightR, LightG, LightB, Light);
			Vector(PosX, PosY, PosZ, p);

			vec34_t* BoneTransform = &*(vec34_t*)(Object->BoneTransform[BonePlayer]);

			if (BoneTransform != 0)
			{
				b->TransformPosition(BoneTransform[0], p, Position, 1);

				if (Black == 1)
				{
					float Luminosity = 1.f;
					Vector(Luminosity * 1.f, Luminosity * 1.f, Luminosity * 1.f, Light);
				}

				CreateSprite(EffectID, Position, Scale, Light, (OBJECT*)ObjectStruct, 0.0, Black);
			}
		}
	}
}

void CreateSetEffectParticle(DWORD BMDStruct, DWORD ObjectStruct, int BonePlayer, int EffectID, int EffectLv, float Scale, float LightR, float LightG, float LightB, float PosX, float PosY, float PosZ)
{
	if (SceneFlag == CHARACTER_SCENE)
	{
		return;
	}

	if (BMDStruct == 0 || ObjectStruct == 0) 
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		auto b = (BMD*)BMDStruct;

		if (Object != nullptr)
		{
			vec3_t p, Position;
			vec3_t Light;

			Vector(LightR, LightG, LightB, Light);
			Vector(PosX, PosY, PosZ, p);

			vec34_t* BoneTransform = &*(vec34_t*)(Object->BoneTransform[BonePlayer]);

			if (BoneTransform != 0)
			{
				b->TransformPosition(BoneTransform[0], p, Position, 1);

				CreateParticle(EffectID, Position, Object->Angle, Light, EffectLv, Scale, (OBJECT*)ObjectStruct);
			}
		}
	}
}

void CreateSetEffectSkill(DWORD BMDStruct, DWORD ObjectStruct, int BonePlayer, int EffectID, int EffectLv, float Scale, float LightR, float LightG, float LightB, float PosX, float PosY, float PosZ)
{
	if (SceneFlag == CHARACTER_SCENE)
	{
		return;
	}

	if (BMDStruct == 0 || ObjectStruct == 0)
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		auto b = (BMD*)BMDStruct;

		if (Object != nullptr)
		{
			vec3_t p, Position;
			vec3_t Light;

			Vector(LightR, LightG, LightB, Light);
			Vector(PosX, PosY, PosZ, p);

			vec34_t* BoneTransform = &*(vec34_t*)(Object->BoneTransform[BonePlayer]);

			if (BoneTransform != 0)
			{
				b->TransformPosition(BoneTransform[0], p, Position, 1);

				CreateParticle(EffectID, Position, Object->Angle, Light, EffectLv, Scale, (OBJECT*)ObjectStruct);

				CreateEffect(EffectID, Position, Object->Angle, Light, EffectLv, Object, -1, 0, 0, 0, 0.0, -1);
			}
		}
	}
}

void CCustomSetEffect::Init()
{
	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\EffectSystem\\CharacterSetEffect.lua", LUA_LOAD_PATH);

	LuaModule(this->m_Lua.GetState()).fun("AddEffect15", &AddEffect15Grade);

	LuaModule(this->m_Lua.GetState()).fun("AddSmokeEffect", &AddSmokeEffect);

	LuaModule(this->m_Lua.GetState()).fun("CreateSprite", &CreateSetEffectSprite);

	LuaModule(this->m_Lua.GetState()).fun("CreateParticle", &CreateSetEffectParticle);

	LuaModule(this->m_Lua.GetState()).fun("CreateSkill", &CreateSetEffectSkill);

	RegisterClassObject(this->m_Lua.GetState());

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("LoadEffectGrade", ">");

	this->m_Lua.Generic_Call("LoadSmokeEffect", ">");
}

void CCustomSetEffect::RestartLua()
{
	this->haveEffect15.clear();

	this->m_CustomSmokeEffect.clear();

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\EffectSystem\\CharacterSetEffect.lua", LUA_LOAD_PATH);

	LuaModule(this->m_Lua.GetState()).fun("AddEffect15", &AddEffect15Grade);

	LuaModule(this->m_Lua.GetState()).fun("AddSmokeEffect", &AddSmokeEffect);

	LuaModule(this->m_Lua.GetState()).fun("CreateSprite", &CreateSetEffectSprite);

	LuaModule(this->m_Lua.GetState()).fun("CreateParticle", &CreateSetEffectParticle);

	LuaModule(this->m_Lua.GetState()).fun("CreateSkill", &CreateSetEffectSkill);

	RegisterClassObject(this->m_Lua.GetState());

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("LoadEffectGrade", ">");

	this->m_Lua.Generic_Call("LoadSmokeEffect", ">");
}

bool CCustomSetEffect::GetEffect15Grade(CHARACTER* c, int ItemIndex)
{
	int full = 1;

	for (int i = BODYPART_HELM; i <= BODYPART_BOOTS; i++) {
		if ((c->BodyPart[i].Type - MODEL_ITEM) % 512 != ItemIndex % 512) {
			full = 0;
		}
	}

	for (auto it = haveEffect15.begin(); it != haveEffect15.end(); ++it) 
	{
		if (it->ItemIndex == ItemIndex) {
			if (it->NeedFullSet == 1 && full == 0) {
				return false;
			}

			return true;
		}
	}

	return false;
}

bool CCustomSetEffect::GetSmokeEffect(int ItemIndex) // OK
{
	auto it = this->m_CustomSmokeEffect.find(ItemIndex);

	if (it != this->m_CustomSmokeEffect.end())
	{
		return true;
	}

	return false;
}

void CCustomSetEffect::GetSmokeColor(int ItemIndex, float* Red, float* Green, float* Blue) // OK
{
	auto it = this->m_CustomSmokeEffect.find(ItemIndex);

	if (it != this->m_CustomSmokeEffect.end())
	{
		*(Red) = it->second.Red;
		*(Green) = it->second.Green;
		*(Blue) = it->second.Blue;
	}
}
