#include "stdafx.h"
#include "MonsterEffect.h"
#include "LuaStack.hpp"
#include "ZzzCharacter.h"

using namespace luaaa;

CMonsterEffect gMonsterEffect;

CMonsterEffect::CMonsterEffect() : m_Lua()
{
	this->m_MonsterEffect.clear();
}

CMonsterEffect::~CMonsterEffect()
{
	this->m_MonsterEffect.clear();
}

void SetMonsterEffect(int MonsterID, int Type, int EffectID, int EffectLv, int Bone, float Size, float R, float G, float B, float Black, int Rand)
{
	CUSTOM_MONSTER_EFFECT monsterInfo;

	monsterInfo.MonsterID = MonsterID;

	CUSTOM_MONSTER_EFFECT_INFO glowInfo;

	glowInfo.Type = Type;

	glowInfo.EffectID = EffectID;

	glowInfo.EffectLv = EffectLv;

	glowInfo.Bone = Bone;

	glowInfo.Size = Size;

	glowInfo.R = R;

	glowInfo.G = G;

	glowInfo.B = B;

	glowInfo.Black = Black;

	glowInfo.Rand = Rand;

	auto it = gMonsterEffect.m_MonsterEffect.find(MonsterID);

	if (it == gMonsterEffect.m_MonsterEffect.end())
	{
		monsterInfo.m_Effects.push_back(glowInfo);

		gMonsterEffect.m_MonsterEffect.insert(std::make_pair(MonsterID, monsterInfo));
	}
	else
	{
		it->second.m_Effects.push_back(glowInfo);
	}
}

void CMonsterEffect::CreateEffectMonster(OBJECT* ObjectStruct, BMD* pModel, int Class)
{
	if (ObjectStruct == 0 || pModel == 0)
	{
		return;
	}

	auto o = (OBJECT*)ObjectStruct;

	if (!g_isCharacterBuff(o, eBuff_Cloaking)) //HIDE
	{
		auto monsterEffect = gMonsterEffect.getMonsterEffect(Class);

		if (monsterEffect != nullptr)
		{
			vec3_t p, Position, Light;

			for (const auto& effect : monsterEffect->m_Effects)
			{
				vec34_t* BoneTransform = &ObjectStruct->BoneTransform[effect.Bone];

				if (BoneTransform)
				{
					if (effect.Rand != 100)
					{
						if ((rand() % 100) > effect.Rand)
						{
							continue;
						}
					}

					Vector(0.0f, 0.0f, 0.0f, p);


					Vector(effect.R, effect.G, effect.B, Light);

					pModel->TransformPosition(BoneTransform[0], p, Position, 1);

					if (effect.Black == 1) {
						float Luminosity = 1.f;
						Vector(Luminosity * 1.f, Luminosity * 1.f, Luminosity * 1.f, Light);
					}


					if (effect.Type == 1)
					{
						CreateParticle(effect.EffectID, Position, ObjectStruct->Angle, Light, effect.EffectLv, effect.Size, ObjectStruct);
					}
					else if (effect.Type == 2)
					{
						CreateEffect(effect.EffectID, Position, ObjectStruct->Angle, Light, effect.EffectLv, ObjectStruct, -1, 0, 0, 0, 0.0, -1);
					}
					else
					{
						CreateSprite(effect.EffectID, Position, effect.Size, Light, ObjectStruct, 0, effect.Black);
					}
				}
			}
		}
	}
}


void CMonsterEffect::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetMonsterEffect", SetMonsterEffect);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterEffect.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

void CMonsterEffect::RestartLua()
{
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	gMonsterEffect.m_MonsterEffect.clear();

	LuaModule(this->m_Lua.GetState()).fun("SetMonsterEffect", SetMonsterEffect);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Monster\\CustomMonsterEffect.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

CUSTOM_MONSTER_EFFECT* CMonsterEffect::getMonsterEffect(int Type)
{
	auto it = this->m_MonsterEffect.find(Type);

	if (it == this->m_MonsterEffect.end())
	{
		return nullptr;
	}

	return &it->second;
}