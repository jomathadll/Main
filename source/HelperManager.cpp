#include "StdAfx.h"
#include "HelperManager.h"
#include "HelperSystem.h"
#include "HelperView.h"
#include "ZzzCharacter.h"
#include "ZzzInfomation.h"
#include "ZzzLodTerrain.h"
#include "ElementPet.h"
#include "GOBoid.h"
#include "w_PetProcess.h"

CHelperManager gHelperManager;

namespace
{
	int GetFenrirModelByOption(int option)
	{
		if (option == 0x01)
		{
			return MODEL_FENRIR_BLACK;
		}
		else if (option == 0x02)
		{
			return MODEL_FENRIR_BLUE;
		}
		else if (option == 0x04)
		{
			return MODEL_FENRIR_GOLD;
		}

		return MODEL_FENRIR_RED;
	}

	bool TryGetLegacyFenrirPetConfig(int& miniature, float& sizeMiniature, float& velocityMiniature, float& scale)
	{
		lua_State* state = gHelperSystem.m_Lua.GetState();
		if (state == nullptr)
		{
			return false;
		}

		lua_getglobal(state, "GetLegacyFenrirPetConfig");
		bool hasConfigFunction = lua_isfunction(state, -1) != 0;
		lua_pop(state, 1);

		if (!hasConfigFunction)
		{
			return false;
		}

		return gHelperSystem.m_Lua.Generic_Call(
			"GetLegacyFenrirPetConfig",
			">ifff",
			&miniature,
			&sizeMiniature,
			&velocityMiniature,
			&scale
		);
	}

}

CHelperManager::CHelperManager()
{
	this->m_HelperList.clear();
}

CHelperManager::~CHelperManager()
{
	//this->Relese();
}

void CHelperManager::Relese()
{
	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();) 
	{
		std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator tempIT = it;

		++it;

		BoostWeak_Ptr(HELPER_VIEW) element = *tempIT;

		if (element.expired() == false)
		{
			element.lock()->Relese();
		}

		//m_HelperList.erase(tempIT);
	}

	m_HelperList.clear();
}

void CHelperManager::Init()
{
	//SetCompleteHook(0xE8, 0x004D7348, &this->RenderPetMain);
	//SetCompleteHook(0xE8, 0x004D7A98, &this->RenderPetMain);
	//SetCompleteHook(0xE8, 0x004D9A65, &this->RenderPetMain);
	//
	//SetCompleteHook(0xE8, 0x004D6EA5, &this->UpdatePetMain);
	//SetCompleteHook(0xE8, 0x004D7787, &this->UpdatePetMain);
	//SetCompleteHook(0xE8, 0x004D9568, &this->UpdatePetMain);
}

void CHelperManager::RenderPetMain()
{
	//((void(*)())0x503950)();
	//
}

void CHelperManager::UpdatePetMain()
{
	//((void(*)())0x503720)();
	//
}

bool CHelperManager::CreateHelper(int Index, int ModelIndex, vec3_t Position, CHARACTER * m_Owner, int SubType, int Separate)
{
	if (m_Owner == nullptr)
	{
		return 0;
	}

	if (CheckExistsHelper(m_Owner))
	{
		return 1;
	}

	HELPER_VIEWPtr element = HELPER_VIEW::Make();

	if (element == nullptr)
	{
		return 0;
	}

	if (element->Create(Index, ModelIndex, Position, m_Owner, SubType, Separate))
	{
		auto elementInfo = gHelperSystem.GetHelper(ModelIndex);

		if (elementInfo == 0) 
		{
			return 0;
		}

		element->m_Type = elementInfo->Type;

		element->m_SubType = elementInfo->SubType;

		element->m_HeightFloor = elementInfo->HeightFloor;

		element->m_Movement = elementInfo->Movement;

		if (SceneFlag == 4)
		{
			element->SetScale(elementInfo->SizeCharList);
		}
		else 
		{
			element->SetScale(elementInfo->Size);
		}

		element->SetMiniature(elementInfo->Miniature, elementInfo->SizeMiniature, elementInfo->VelocityMiniature);

		this->Register(element);
		return 1;
	}

	return 0;
}

int CHelperManager::GetLegacyFenrirModelByOption(int option)
{
	return GetFenrirModelByOption(option);
}

bool CHelperManager::CreateLegacyFenrir(int ModelIndex, vec3_t Position, CHARACTER* m_Owner)
{
	if (m_Owner == nullptr)
	{
		return 0;
	}

	if (CheckExistsHelper(m_Owner))
	{
		return 1;
	}

	HELPER_VIEWPtr element = HELPER_VIEW::Make();

	if (element == nullptr)
	{
		return 0;
	}

	if (element->Create(ITEM_HELPER + 37, ModelIndex, Position, m_Owner, 0, 0))
	{
		int miniature = 1;
		float sizeMiniature = 0.42f;
		float velocityMiniature = 1.25f;
		float scale = 0.90f;

		TryGetLegacyFenrirPetConfig(miniature, sizeMiniature, velocityMiniature, scale);

		if (sizeMiniature <= 0.0f)
		{
			sizeMiniature = 0.42f;
		}

		if (velocityMiniature <= 0.0f)
		{
			velocityMiniature = 1.25f;
		}

		if (scale <= 0.0f)
		{
			scale = 0.90f;
		}

		element->m_Type = 8;
		element->m_SubType = 0;
		element->m_HeightFloor = 0.0f;
		element->m_Movement = 0;
		element->SetScale(scale);
		element->SetMiniature(miniature, sizeMiniature, velocityMiniature);

		this->Register(element);
		return 1;
	}

	return 0;
}

void CHelperManager::DeleteHelper(CHARACTER* Owner, int itemType, bool allDelete)
{
	if (Owner == nullptr)
	{
		return;
	}

	UnRegister(Owner, itemType, allDelete);
}

void CHelperManager::Register(BoostSmart_Ptr(HELPER_VIEW) info)
{
	this->m_HelperList.push_back(info);
}

void CHelperManager::UnRegister(CHARACTER* Owner, int itemType, bool isUnregistAll)
{
	if (Owner == nullptr)
	{
		return;
	}

	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();)
	{
		BoostWeak_Ptr(HELPER_VIEW) element = *it;

		if (element.expired() == true)
		{
			it = m_HelperList.erase(it);
			continue;
		}

		HELPER_VIEWPtr helper = element.lock();
		if (helper == nullptr || helper->m_Obj == nullptr)
		{
			it = m_HelperList.erase(it);
			continue;
		}

		if ((-1 == itemType && helper->IsSameOwner(&Owner->Object)) || helper->IsSameObject(&Owner->Object, itemType))
		{
			helper->Relese();
			it = m_HelperList.erase(it);

			if (-1 == itemType || !isUnregistAll)
			{
				return;
			}

			continue;
		}

		++it;
	}
}

bool CHelperManager::CheckExistsHelper(CHARACTER* Owner)
{
	if (Owner == nullptr) 
	{
		return 0;
	}

	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();)
	{
		BoostWeak_Ptr(HELPER_VIEW) element = *it;

		if (element.expired() == true)
		{
			it = m_HelperList.erase(it);
			continue;
		}

		HELPER_VIEWPtr helper = element.lock();
		if (helper == nullptr || helper->m_Obj == nullptr || helper->IsLive() == false)
		{
			if (helper != nullptr)
			{
				helper->Relese();
			}

			it = m_HelperList.erase(it);
			continue;
		}

		if (helper->IsSameOwner(&Owner->Object) && helper->IsSameObject(&Owner->Object, Owner->Helper.Type))
		{
			return 1;
		}

		++it;
	}

	return 0;
}

void CHelperManager::RenderHelpers()
{
	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();)
	{
		BoostWeak_Ptr(HELPER_VIEW) element = *it;

		if (element.expired() == true)
		{
			it = m_HelperList.erase(it);
			continue;
		}

		HELPER_VIEWPtr helper = element.lock();
		if (helper != nullptr && helper->m_Obj != nullptr && helper->IsLive() == true)
		{
			helper->Render();
			++it;
		}
		else
		{
			if (helper != nullptr)
			{
				helper->Relese();
			}

			it = m_HelperList.erase(it);
		}
	}

	gElementPetFirst.RenderElement();
	gElementPetSecond.RenderElement();
}

void CHelperManager::UpdateHelpers()
{
	for (std::vector<BoostSmart_Ptr(HELPER_VIEW)>::iterator it = m_HelperList.begin(); it != m_HelperList.end();)
	{
		BoostWeak_Ptr(HELPER_VIEW) element = *it;

		if (element.expired() == true)
		{
			it = m_HelperList.erase(it);
			continue;
		}

		HELPER_VIEWPtr helper = element.lock();
		if (helper != nullptr && helper->m_Obj != nullptr && helper->IsLive() == true)
		{
			helper->Update();
			++it;
		}
		else
		{
			if (helper != nullptr)
			{
				helper->Relese();
			}

			it = m_HelperList.erase(it);
		}
	}

	if (Hero != nullptr && CharacterMachine != nullptr && Hero->Object.Live && Hero->Object.Kind == KIND_PLAYER)
	{
		ITEM* helperItem = &CharacterMachine->Equipment[EQUIPMENT_HELPER];

		if (helperItem->Type != ITEM_HELPER + 37)
		{
			DeleteHelper(Hero, MODEL_HELPER + 37, true);

			if (Hero->Helper.Type == MODEL_HELPER + 37)
			{
				Hero->Helper.Type = -1;
				Hero->Helper.Option1 = 0;
				Hero->Helper.ExtOption = 0;
				Hero->Helper.Level = 0;
			}
		}
		else if (Hero->Helper.Type == MODEL_HELPER + 37)
		{
			Hero->Helper.Option1 = helperItem->Option1;

			if (!CheckExistsHelper(Hero))
			{
				DeleteBug(&Hero->Object);
				ThePetProcess().DeletePet(Hero);
				CreateLegacyFenrir(GetFenrirModelByOption(helperItem->Option1), Hero->Object.Position, Hero);
			}
		}
	}

	gElementPetFirst.UpdateElement();
	gElementPetSecond.UpdateElement();
}
