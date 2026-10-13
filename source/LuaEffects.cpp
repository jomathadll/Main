#include "stdafx.h"
#include "LuaEffects.h"
#include "LuaStack.hpp"
#include "ZzzBMD.h"

using namespace luaaa;

void FuncCreateParticle(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, vec3_t Light, DWORD ObjectStruct) 
{
	if (BMDStruct == 0 || ObjectStruct == 0) 
	{
		return;
	}

	vec3_t Position, p;

	p[0] = 0.f; p[1] = 0.f; p[2] = 0.f;

	auto Object = &*(OBJECT*)ObjectStruct;

	BMD* b = &*(BMD*)BMDStruct;
	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		if (Object != nullptr)
		{
			if (Object->Type >= gItemManager.GET_ITEM_MODEL(13, 150) && Object->Type <= gItemManager.GET_ITEM_MODEL(13, 500) || Object->Type >= gItemManager.GET_ITEM_MODEL(14, 150) && Object->Type <= gItemManager.GET_ITEM_MODEL(16, 0))
			{
				b->TransformPosition(BoneTransform[Link], p, Position, 0);
			}
			else if (Object->Type >= gItemManager.GET_ITEM_MODEL(16, 250) && Object->Type <= gItemManager.GET_ITEM_MODEL(17, 0))
			{
				b->TransformPosition(Object->BoneTransform[Link], p, Position, 0);
			}
			else
			{
				b->TransformPosition(BoneTransform[Link], p, Position, 1);
			}
		}
		else
		{
			b->TransformPosition(BoneTransform[Link], p, Position, 1);
		}

		CreateParticle(Bitmap, Position, Object->Angle, Light, SubType, Scale, (OBJECT*)ObjectStruct);
	}
}

void FuncCreateSprite(DWORD BMDStruct, int Bitmap, int Link, float Scale, vec3_t Light, DWORD ObjectStruct, int Black) 
{
	if (BMDStruct == 0 || ObjectStruct == 0) 
	{
		return;
	}

	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		vec3_t Position, p;

		p[0] = 0.f; p[1] = 0.f; p[2] = 0.f;

		BMD* b = &*(BMD*)BMDStruct;

		if (Object != nullptr)
		{
			if (Object->Type >= gItemManager.GET_ITEM_MODEL(13, 150) && Object->Type <= gItemManager.GET_ITEM_MODEL(13, 500) || Object->Type >= gItemManager.GET_ITEM_MODEL(14, 150) && Object->Type <= gItemManager.GET_ITEM_MODEL(16, 0))
			{
				b->TransformPosition(BoneTransform[Link], p, Position, 0);
			}
			else if (Object->Type >= gItemManager.GET_ITEM_MODEL(16, 250) && Object->Type <= gItemManager.GET_ITEM_MODEL(17, 0))
			{
				b->TransformPosition(Object->BoneTransform[Link], p, Position, 0);
			}
			else
			{
				b->TransformPosition(BoneTransform[Link], p, Position, 1);
			}
		}
		else
		{
			b->TransformPosition(BoneTransform[Link], p, Position, 1);
		}

		CreateSprite(Bitmap, Position, Scale, Light, (OBJECT*)ObjectStruct, 0.0, Black);
	}
}

void CreateSprites(DWORD BMDStruct, int Bitmap, int Link, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct, int Black) 
{
	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);

		if (Black == 1) {
			float Luminosity = 1.f;
			Vector(Luminosity * 1.f, Luminosity * 1.f, Luminosity * 1.f, Light);
		}

		FuncCreateSprite(BMDStruct, Bitmap, Link, Scale, Light, ObjectStruct, Black);
	}
}

void CreateParticles(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct) 
{
	auto Object = &*(OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(Object, eBuff_Cloaking)) //HIDE
	{
		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);

		FuncCreateParticle(BMDStruct, Bitmap, SubType, Link, Scale, Light, ObjectStruct);
	}
}

CLuaEffects::CLuaEffects()
{
}

CLuaEffects::~CLuaEffects()
{
}

void CLuaEffects::RegisterLuaClass(lua_State * lua)
{
	LuaModule(lua).fun("CreateSprite", &CreateSprites);
	LuaModule(lua).fun("CreateParticle", &CreateParticles);
}
