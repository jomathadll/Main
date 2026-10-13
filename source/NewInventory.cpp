#include "stdafx.h"
#include "NewInventory.h"
#include "NewUIMyInventory.h"
#include "NewUISystem.h"
#include "_struct.h"
#include "ElementPet.h"
#include "HelperManager.h"
#include "GOBoid.h"
#include "w_PetProcess.h"
#include "ZzzEffect.h"
#include "DarkSpirit.h"
#include "CharacterManager.h"
#include "GIPetManager.h"
#include "CustomWing.h"

VisualInventory gVisualInventory;

void VisualInventory::AddItem(BYTE slot, BYTE* ItemInfo)
{
	ITEM* pTargetItemSlot = &CharacterMachine->Equipment[slot];

	if (pTargetItemSlot->Type > 0)
	{
		g_pMyInventory->UnequipItem(slot);
	}

	ITEM* pTempItem = g_pNewItemMng->CreateItem(ItemInfo);

	if (NULL == pTempItem)
	{
		return;
	}

	pTempItem->lineal_pos = slot;
	pTempItem->ex_src_type = ITEM_EX_SRC_EQUIPMENT;
	memcpy(pTargetItemSlot, pTempItem, sizeof(ITEM));
	g_pNewItemMng->DeleteItem(pTempItem);

	if (slot == 247 && gCustomWing.CheckCustomWingIsCape(pTargetItemSlot->Type + MODEL_ITEM) > 0) {
		DeleteCloth(Hero, &Hero->Object);
	}

	if (slot == 248)
	{
		this->EffectHelperVisual(pTargetItemSlot);
	}
}

void VisualInventory::AddItemElemental(BYTE slot, BYTE* ItemInfo)
{
	ITEM* pTargetItemSlot = &CharacterMachine->Equipment[slot];

	if (pTargetItemSlot->Type > 0)
	{
		g_pMyInventory->UnequipItem(slot);
	}

	ITEM* pTempItem = g_pNewItemMng->CreateItem(ItemInfo);

	if (NULL == pTempItem)
	{
		return;
	}

	pTempItem->lineal_pos = slot;
	pTempItem->ex_src_type = ITEM_EX_SRC_EQUIPMENT;
	int itemType = pTempItem->Type;
	memcpy(pTargetItemSlot, pTempItem, sizeof(ITEM));
	g_pNewItemMng->DeleteItem(pTempItem);

	if (!this->AddDarkSpirit(itemType, pTargetItemSlot))
	{
		this->EffectPet(slot, pTargetItemSlot);
	}
}

void VisualInventory::EffectPet(int slot, ITEM* pItem)
{
	auto hero = Hero;

	if (slot == 236)
	{
		if (gHelperSystem.CheckIsHelper(pItem->Type + MODEL_ITEM))
		{
			gElementPetFirst.CreateHelper(pItem->Type, gHelperSystem.GetHelperModel(pItem->Type + MODEL_ITEM), hero->Object.Position, hero, 0, gHelperSystem.GetHelperSubType(pItem->Type + MODEL_ITEM));
		}
	}
	else if (slot == 237)
	{
		if (gHelperSystem.CheckIsHelper(pItem->Type + MODEL_ITEM))
		{
			gElementPetSecond.CreateHelper(pItem->Type, gHelperSystem.GetHelperModel(pItem->Type + MODEL_ITEM), hero->Object.Position, hero, 0, gHelperSystem.GetHelperSubType(pItem->Type + MODEL_ITEM));
		}
	}
}

void VisualInventory::DeleteEffectPets(int slot)
{
	if (slot == 236)
	{
		gElementPetFirst.DeleteHelper(Hero);
	}
	else if (slot == 237)
	{
		gElementPetSecond.DeleteHelper(Hero);
	}
}

void VisualInventory::EffectHelperVisual(ITEM* pItem)
{
	auto c = Hero;
	
	auto getHelper = &CharacterMachine->Equipment[8];

	if (getHelper != nullptr)
	{
		DeleteBug(&c->Object);

		ThePetProcess().DeletePet(c);

		gHelperManager.DeleteHelper(c);

		if (pItem->Type != -1 && gHelperSystem.CheckIsHelper(pItem->Type + MODEL_ITEM))
		{
			c->Helper.Type = pItem->Type + MODEL_ITEM;
			c->Helper.Level = (pItem->Level >> 3) & 15;

			gHelperManager.CreateHelper(pItem->Type, gHelperSystem.GetHelperModel(c->Helper.Type), c->Object.Position, c, 0, gHelperSystem.GetHelperSubType(c->Helper.Type));
		}
	}
}

void VisualInventory::RecoveryOldHelper()
{
	auto getHelper = &CharacterMachine->Equipment[8];
	auto visualHelper = &CharacterMachine->Equipment[NEW_EQUIPMENT_HELPER];

	OBJECT* pHeroObject = &Hero->Object;

	if (visualHelper != nullptr && visualHelper->Type != -1 && gHelperSystem.CheckIsHelper(visualHelper->Type + MODEL_ITEM))
	{
		auto c = Hero;

		DeleteBug(&c->Object);
		ThePetProcess().DeletePet(c);
		gHelperManager.DeleteHelper(c);

		c->Helper.Type = visualHelper->Type + MODEL_ITEM;
		c->Helper.Level = (visualHelper->Level >> 3) & 15;

		gHelperManager.CreateHelper(visualHelper->Type, gHelperSystem.GetHelperModel(c->Helper.Type), c->Object.Position, c, 0, gHelperSystem.GetHelperSubType(c->Helper.Type));
		return;
	}

	if (getHelper != nullptr) 
	{
		auto c = Hero;

		if (gHelperSystem.CheckIsHelper(getHelper->Type + MODEL_ITEM)) 
		{
			DeleteBug(&c->Object);

			ThePetProcess().DeletePet(c);

			c->Helper.Type = getHelper->Type + MODEL_ITEM;
			c->Helper.Level = (getHelper->Level >> 3) & 15;

			gHelperManager.CreateHelper(getHelper->Type, gHelperSystem.GetHelperModel(c->Helper.Type), c->Object.Position, c, 0, gHelperSystem.GetHelperSubType(c->Helper.Type));
		}
		else
		{
			gHelperManager.DeleteHelper(c);

			if (getHelper->Type == -1)
			{
				c->Helper.Type = -1;
				c->Helper.Level = 0;
				c->Helper.Option1 = 0;
				c->Helper.ExtOption = 0;
				return;
			}

			c->Helper.Type = getHelper->Type + MODEL_ITEM;

			c->Helper.Level = 0;

			switch (getHelper->Type)
			{
			case ITEM_HELPER:
				CreateBug(MODEL_HELPER, pHeroObject->Position, pHeroObject);
				break;
			case ITEM_HELPER + 2:
				CreateBug(MODEL_UNICON, pHeroObject->Position, pHeroObject);
				if (!Hero->SafeZone)
					CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
				break;
			case ITEM_HELPER + 3:
				CreateBug(MODEL_PEGASUS, pHeroObject->Position, pHeroObject);
				if (!Hero->SafeZone)
					CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
				break;
			case ITEM_HELPER + 4:
				CreateBug(MODEL_DARK_HORSE, pHeroObject->Position, pHeroObject);
				if (!Hero->SafeZone)
					CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
				break;
			case ITEM_HELPER + 37:
				Hero->Helper.Type = MODEL_HELPER + 37;
				Hero->Helper.Option1 = getHelper->Option1;
				DeleteBug(pHeroObject);
				ThePetProcess().DeletePet(Hero);
				gHelperManager.DeleteHelper(Hero, MODEL_HELPER + 37, true);
				gHelperManager.CreateLegacyFenrir(gHelperManager.GetLegacyFenrirModelByOption(getHelper->Option1), pHeroObject->Position, Hero);

				if (!Hero->SafeZone)
				{
					CreateEffect(BITMAP_MAGIC + 1, pHeroObject->Position, pHeroObject->Angle, pHeroObject->Light, 1, pHeroObject);
				}
				break;
			case ITEM_HELPER + 64:
				ThePetProcess().CreatePet(getHelper->Type, MODEL_HELPER + 64, pHeroObject->Position, Hero);
				break;
			case ITEM_HELPER + 65:
				ThePetProcess().CreatePet(getHelper->Type, MODEL_HELPER + 65, pHeroObject->Position, Hero);
				break;
			case ITEM_HELPER + 67:
				ThePetProcess().CreatePet(getHelper->Type, MODEL_HELPER + 67, pHeroObject->Position, Hero);
				break;
			case ITEM_HELPER + 80:
				ThePetProcess().CreatePet(getHelper->Type, MODEL_HELPER + 80, pHeroObject->Position, Hero);
				break;
			case ITEM_HELPER + 106:
				ThePetProcess().CreatePet(getHelper->Type, MODEL_HELPER + 106, pHeroObject->Position, Hero);
				break;
			case ITEM_HELPER + 123:
				ThePetProcess().CreatePet(getHelper->Type, MODEL_HELPER + 123, pHeroObject->Position, Hero);
				break;
			}

			//EFEITO DOS PETS EQUIPADOS
			if (gHelperSystem.CheckIsHelper(getHelper->Type + MODEL_ITEM))
			{
				gHelperSystem.InvCreateEquippingEffect(getHelper->Type);
			}

		}
	}
}

bool VisualInventory::AddDarkSpirit(int ItemIndex, ITEM* pEquipmentItemSlot)
{
	if (ItemIndex == ITEM_HELPER + 5)
	{
		auto hero = Hero;

		giPetManager::DeletePet(hero);
		gDarkSpirit.DeleteDarkSpirit(hero, -1, true);
		int backupWeapon = hero->Weapon[1].Type;
		hero->Weapon[1].Type = MODEL_HELPER + 5;

		PET_INFO* pPetInfo = giPetManager::GetPetInfo(pEquipmentItemSlot != NULL ? pEquipmentItemSlot : &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT]);

		giPetManager::CreatePetDarkSpirit(hero);
		hero->Weapon[1].Type = backupWeapon;

		if (!gMapManager.InChaosCastle() && hero->m_pPet != NULL)
		{
			((CSPetSystem*)hero->m_pPet)->SetPetInfo(pPetInfo);
		}

		giPetManager::EnableDarkSpiritCommandSkills();
		return 1;
	}

	if (gDarkSpirit.checkIsDarkSpirit(ItemIndex))
	{
		auto hero = Hero;
		auto backupWeapon = hero->Weapon[1].Type;

		giPetManager::DeletePet(hero);
		gDarkSpirit.DeleteDarkSpirit(hero, -1, true);

		hero->Weapon[1].Type = ItemIndex + MODEL_ITEM;

		PET_INFO* pPetInfo = giPetManager::GetPetInfo(pEquipmentItemSlot != NULL ? pEquipmentItemSlot : &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT]);

		giPetManager::CreatePetDarkSpirit(hero);

		if (!gMapManager.InChaosCastle() && hero->m_pPet != NULL)
		{
			((CSPetSystem*)hero->m_pPet)->SetPetInfo(pPetInfo);
		}

		gDarkSpirit.CreateDarkSpirit(hero, hero->Weapon[1].Type);

		hero->RavenCustom = hero->Weapon[1].Type;

		hero->Weapon[1].Type = backupWeapon;

		giPetManager::EnableDarkSpiritCommandSkills();

		return 1;
	}

	return 0;
}


void VisualInventory::DeleteDarkSpirit(int ItemIndex)
{
	if (ItemIndex == ITEM_HELPER + 5)
	{
		giPetManager::DeletePet(Hero);
		gDarkSpirit.DeleteDarkSpirit(Hero, -1, true);
		Hero->RavenCustom = -1;
		return;
	}

	if (gDarkSpirit.checkIsDarkSpirit(ItemIndex))
	{
		giPetManager::DeletePet(Hero);
		gDarkSpirit.DeleteDarkSpirit(Hero, -1, true);
		Hero->RavenCustom = -1;

	}
}
