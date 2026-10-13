#include "stdafx.h"
#include "SummonEffect.h"
#include "LuaStack.hpp"
#include "LuaCharacterObject.h"
#include "LuaObject.h"
#include "SummonSystem.h"
#include "zzzEffect.h"
#include "wsclientinline.h"
#include "ZzzAI.h"
#include "MapManager.h"
#include "SkillManager.h"

using namespace luaaa;

CSummonEffect gSummonEffect;

CSummonEffect::CSummonEffect() : m_Lua()
{

}

CSummonEffect::~CSummonEffect()
{
    m_SummonEffectRandom.clear();
}

void SetSummonEffect(int ItemIndex, int Model, int Type, float ColorR, float ColorG, float ColorB, bool SafeZone, float Speed)
{
	CUSTOM_SUMMON_EFFECT info;

	info.ItemIndex = ItemIndex;

	info.Model = Model;

	info.Type = Type;

	info.ColorR = ColorR;

	info.ColorG = ColorG;

	info.ColorB = ColorB;

	info.SafeZone = SafeZone;

	info.Speed = Speed;

	gSummonEffect.m_SummonEffectInfo[ItemIndex] = info;
}

void SetSummonEffect_Wrist(int ItemIndex, int Type, float ColorR, float ColorG, float ColorB, int Bone)
{
	CUSTOM_SUMMON_EFFECT_WRIST info;

	info.ItemIndex = ItemIndex;

	info.Type = Type;

	info.ColorR = ColorR;

	info.ColorG = ColorG;

	info.ColorB = ColorB;

	info.Bone = Bone;

	gSummonEffect.m_SummonEffectWristInfo[ItemIndex] = info;
}

void CSummonEffect::MoveSummonEffect(CHARACTER* pCharacter, int ItemIndex)
{
	OBJECT* pObject = &pCharacter->Object;

	if (!g_isCharacterBuff(pObject, eBuff_Cloaking)) //HIDE
	{
		int Index = gSummonEffect.IsSummonEffect((DWORD)pCharacter);

		if (ItemIndex == Index)
		{
			CreateSummonEffect(pCharacter, ItemIndex);
		}
		else
		{
			RemoveSummonEffect(pCharacter);
		}
	}
}

void CSummonEffect::CreateSummonEffect(CHARACTER* pCharacter, int ItemIndex)
{
	OBJECT* pObject = &pCharacter->Object;

	if (!g_isCharacterBuff(pObject, eBuff_Cloaking)) //HIDE
	{
		vec3_t vLight;
		gSummonEffect.SummonEffectColor(ItemIndex, vLight);
		//Vector(1.f, 1.f, 1.f, vLight);

		BYTE byRandom;
		std::map<SHORT, BYTE>::iterator iter = m_SummonEffectRandom.find(pCharacter->Key);
		if (iter == m_SummonEffectRandom.end())
		{
			byRandom = rand() % 256;
			m_SummonEffectRandom.insert(std::pair<SHORT, BYTE>(pCharacter->Key, byRandom));
		}
		else
		{
			byRandom = iter->second;
		}

		int Index = gSummonEffect.IsSummonEffect((DWORD)pCharacter);

		if (Index > -1)
		{
			for (std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = gSummonEffect.m_SummonEffectInfo.begin(); it != gSummonEffect.m_SummonEffectInfo.end(); it++)
			{
				auto ItemInfo = &(*it).second;

				if (ItemInfo->ItemIndex == ItemIndex)
				{
					if (!g_isCharacterBuff(pObject, eBuff_Cloaking)) //HIDE
					{
						if (ItemInfo->SafeZone)
						{
							if (!SearchEffect(ItemInfo->Model, pObject, ItemInfo->Type) && sinf(WorldTime * 0.0004f + byRandom * 0.024f) > 0.3f)
							{
								CreateEffect(ItemInfo->Model, pObject->Position, pObject->Angle, vLight, ItemInfo->Type, pObject, -1, byRandom);
							}
						}
						else
						{
							if (!SearchEffect(ItemInfo->Model, pObject, ItemInfo->Type) && !pCharacter->SafeZone && sinf(WorldTime * 0.0004f + byRandom * 0.024f) > 0.3f)
							{
								CreateEffect(ItemInfo->Model, pObject->Position, pObject->Angle, vLight, ItemInfo->Type, pObject, -1, byRandom);
							}
						}
					}
				}
			}
		}
	}
}

void CSummonEffect::RemoveSummonEffect(CHARACTER* pCharacter)
{
	OBJECT* pObject = &pCharacter->Object;

	int Index = gSummonEffect.IsSummonEffect((DWORD)pCharacter);

	if (Index > -1)
	{
		for (std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = gSummonEffect.m_SummonEffectInfo.begin(); it != gSummonEffect.m_SummonEffectInfo.end(); it++)
		{
			auto ItemInfo = &(*it).second;

			DeleteEffect(ItemInfo->Model, pObject, ItemInfo->Type);
		}
	}

}

void CSummonEffect::MoveSummonEffect_Wrist(CHARACTER* pCharacter, int ItemIndex)
{
	OBJECT* pObject = &pCharacter->Object;

	if (!g_isCharacterBuff(pObject, eBuff_Cloaking)) //HIDE
	{
		int IndexWrist = gSummonEffect.IsSummonEffect_Wrist((DWORD)pCharacter);

		if (ItemIndex == IndexWrist)
		{
			CreateSummonEffect_Wrist(pCharacter, ItemIndex);
		}
		else
		{
			RemoveSummonEffect_Wrist(pCharacter);
		}
	}
}

void CSummonEffect::CreateSummonEffect_Wrist(CHARACTER* pCharacter, int ItemIndex)
{
	OBJECT* pObject = &pCharacter->Object;
	BMD* pModel = &Models[pObject->Type];

	if (!g_isCharacterBuff(pObject, eBuff_Cloaking)) //HIDE
	{
		vec3_t vPos, vRelative, vLight;
		Vector(0.0f, 0.0f, 0.0f, vRelative);

		int Index = gSummonEffect.IsSummonEffect_Wrist((DWORD)pCharacter);

		if (Index > -1)
		{
			for (std::map<int, CUSTOM_SUMMON_EFFECT_WRIST>::iterator it = gSummonEffect.m_SummonEffectWristInfo.begin(); it != gSummonEffect.m_SummonEffectWristInfo.end(); it++)
			{
				auto ItemInfo = &(*it).second;

				if (ItemInfo->ItemIndex == ItemIndex)
				{
					pModel->TransformPosition(pObject->BoneTransform[ItemInfo->Bone], vRelative, vPos, true);

					gSummonEffect.SummonEffectColor_Wrist(ItemIndex, vLight);

					if (!SearchJoint(MODEL_SPEARSKILL, pObject, 14))
					{
						for (int i = 0; i < 4; ++i)
						{
							CreateJoint(MODEL_SPEARSKILL, vPos, vPos, pObject->Angle, 14, pObject, 18.0f, -1, 0, 0, pCharacter->TargetCharacter, vLight);
						}

					}
					switch (ItemInfo->Type)
					{
					case 0:
						CreateEffect(MODEL_SUMMONER_WRISTRING_EFFECT, vPos, pObject->Angle, vLight, 0, pObject);
						break;
					case 1:
						CreateSprite(BITMAP_FLARE, vPos, 0.7f, vLight, pObject);
						break;
					case 2:
						CreateSprite(BITMAP_SHINY + 1, vPos, 0.8f, vLight, pObject);
						break;
					case 3:
						CreateSprite(BITMAP_SHINY + 1, vPos, 1.0f, vLight, pObject, (float)((int)(WorldTime / 20) % 360));
						break;
					case 4:
						if (rand() % 4)
						{
							CreateParticle(BITMAP_SPARK + 1, vPos, pObject->Angle, vLight, 23, 0.5f, pObject);
						}
						break;
					}
				}
			}
		}
	}
}

void CSummonEffect::RemoveSummonEffect_Wrist(CHARACTER* pCharacter)
{
	int Index = gSummonEffect.IsSummonEffect_Wrist((DWORD)pCharacter);

	if (Index > -1)
	{
		for (std::map<int, CUSTOM_SUMMON_EFFECT_WRIST>::iterator it = gSummonEffect.m_SummonEffectWristInfo.begin(); it != gSummonEffect.m_SummonEffectWristInfo.end(); it++)
		{
			OBJECT* pObject = &pCharacter->Object;
			DeleteJoint(MODEL_SPEARSKILL, pObject, 14);
			DeleteEffect(MODEL_SUMMONER_WRISTRING_EFFECT, pObject);
		}
	}
}

void CSummonEffect::RemoveSummonEffectsAll(CHARACTER* pCharacter)
{
	RemoveSummonEffect(pCharacter);
	RemoveSummonEffect_Wrist(pCharacter);
}

void CSummonEffect::RestartLua()
{
    m_SummonEffectRandom.clear();

    this->m_Lua.CloseLua();

    this->m_Lua.RegisterLua();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    RegisterClassObject(this->m_Lua.GetState());

    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetSummonEffect", &SetSummonEffect);

	LuaModule(this->m_Lua.GetState()).fun("SetSummonEffect_Wrist", &SetSummonEffect_Wrist);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\SkillColorEffect\\ColorSummonEffect.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartSummonEffect", "");
}

void CSummonEffect::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    RegisterClassObject(this->m_Lua.GetState());

    this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	LuaModule(this->m_Lua.GetState()).fun("SetSummonEffect", &SetSummonEffect);

	LuaModule(this->m_Lua.GetState()).fun("SetSummonEffect_Wrist", &SetSummonEffect_Wrist);

    char name[MAX_PATH] = { 0 };

    sprintf_s(name, "%s\\SkillColorEffect\\ColorSummonEffect.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartSummonEffect", "");
}

bool CSummonEffect::SummonEffectSafezone(int ItemIndex)
{
	std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = this->m_SummonEffectInfo.find(ItemIndex);

	if (it == this->m_SummonEffectInfo.end())
	{
		return false;
	}

	return true;
}

float CSummonEffect::SummonEffectSpeed(int ItemIndex)
{
	std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = this->m_SummonEffectInfo.find(ItemIndex);

	if (it == this->m_SummonEffectInfo.end())
	{
		return 1.0f;
	}

	return it->second.Speed;
}

int CSummonEffect::SummonEffectType(int ItemIndex)
{
	std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = this->m_SummonEffectInfo.find(ItemIndex);

	if (it == this->m_SummonEffectInfo.end())
	{
		return 0;
	}

	return it->second.Type;
}

int CSummonEffect::SummonEffectModel(int ItemIndex)
{
	std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = this->m_SummonEffectInfo.find(ItemIndex);

	if (it == this->m_SummonEffectInfo.end())
	{
		return 0;
	}

	return it->second.Model;
}

void CSummonEffect::SummonEffectColor(int ItemIndex, vec3_t light)
{
	std::map<int, CUSTOM_SUMMON_EFFECT>::iterator it = this->m_SummonEffectInfo.find(ItemIndex);

	if (it == this->m_SummonEffectInfo.end())
	{
		Vector(1.f, 1.f, 1.f, light);
		return;
	}

	Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
	return;
}

int CSummonEffect::SummonEffectType_Wrist(int ItemIndex)
{
	std::map<int, CUSTOM_SUMMON_EFFECT_WRIST>::iterator it = this->m_SummonEffectWristInfo.find(ItemIndex);

	if (it == this->m_SummonEffectWristInfo.end())
	{
		return 0;
	}

	return it->second.Type;
}

int CSummonEffect::SummonEffectBone_Wrist(int ItemIndex)
{
	std::map<int, CUSTOM_SUMMON_EFFECT_WRIST>::iterator it = this->m_SummonEffectWristInfo.find(ItemIndex);

	if (it == this->m_SummonEffectWristInfo.end())
	{
		return 0;
	}

	return it->second.Bone;
}

void CSummonEffect::SummonEffectColor_Wrist(int ItemIndex, vec3_t light)
{
	std::map<int, CUSTOM_SUMMON_EFFECT_WRIST>::iterator it = this->m_SummonEffectWristInfo.find(ItemIndex);

	if (it == this->m_SummonEffectWristInfo.end())
	{
		Vector(1.f, 1.f, 1.f, light);
		return;
	}

	Vector(it->second.ColorR, it->second.ColorG, it->second.ColorB, light);
	return;
}

int CSummonEffect::IsSummonEffect(DWORD characterObj)
{
	int result = 0;
	this->m_Lua.Generic_Call("CheckSummonEffect", "j>i", characterObj, &result);

	return result;
}

int CSummonEffect::IsSummonEffect_Wrist(DWORD characterObj)
{
	int result = 0;
	this->m_Lua.Generic_Call("CheckSummonEffectWrist", "j>i", characterObj, &result);

	return result;
}