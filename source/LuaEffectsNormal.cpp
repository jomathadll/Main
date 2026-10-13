#include "stdafx.h"
#include "LuaEffects.h"
#include "LuaEffectsNormal.h"
#include "LuaStack.hpp"

using namespace luaaa;

void RenderBrightEffects(BMD* b, int Bitmap, int Link, float Scale, vec3_t Light, OBJECT* o, int Black)
{
	vec3_t p, Position;
	Vector(0.f, 0.f, 0.f, p);
	b->TransformPosition(BoneTransform[Link], p, Position, true);
	CreateSprite(Bitmap, Position, Scale, Light, o, 0, Black);
}

void FuncCreateParticleNormal(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, vec3_t Light, DWORD ObjectStruct)
{
	vec3_t Position, p;
	p[0] = 0.f; p[1] = 0.f; p[2] = 0.f;

	auto b = (BMD*)BMDStruct;
	auto o = (OBJECT*)ObjectStruct;

	if (!&ObjectStruct) 
	{
		return;
	}

	if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
	{
		b->TransformPosition(BoneTransform[Link], p, Position);
		CreateParticle(Bitmap, Position, o->Angle, Light, SubType, Scale, o);
	}

}

void CreateSpriteNormal(DWORD BMDStruct, int Bitmap, int Link, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct, int Black)
{
	auto b = (BMD*)BMDStruct;
	auto o = (OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
	{
		vec3_t Light;
		Vector(LightR, LightG, LightB, Light);
		if (Black == 1) {
			float Luminosity = 1.f;
			Vector(Luminosity * 1.f, Luminosity * 1.f, Luminosity * 1.f, Light);
		}
		RenderBrightEffects(b, Bitmap, Link, Scale, Light, o, Black);
	}
}

void CreateParticleNormal(DWORD BMDStruct, int Bitmap, int SubType, int Link, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct)
{
	auto o = (OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
	{
		vec3_t Light;
		Vector(LightR, LightG, LightB, Light);
		FuncCreateParticleNormal(BMDStruct, Bitmap, SubType, Link, Scale, Light, ObjectStruct);
	}
}

CLuaEffectNormal::CLuaEffectNormal()
{
}

CLuaEffectNormal::~CLuaEffectNormal()
{
}

void CLuaEffectNormal::RegisterLuaClass(lua_State * lua)
{
	LuaModule(lua).fun("CreateSprite", &CreateSpriteNormal);
	LuaModule(lua).fun("CreateParticle", &CreateParticleNormal);
}
