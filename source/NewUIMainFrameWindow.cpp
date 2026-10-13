//////////////////////////////////////////////////////////////////////
// NewUIMainFrameWindow.cpp: implementation of the CNewUIMainFrameWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIMainFrameWindow.h"	// self
#if CLIENT_VERSION == 1002
#include "NewUIOptionWindowFairPlay.h"
#else
#include "NewUIOptionWindow.h"
#endif
#include "NewUISystem.h"
#include "NewUIMyInventory.h"
#include "UIBaseDef.h"
#include "DSPlaySound.h"
#include "ZzzInfomation.h"
#include "ZzzBMD.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzOpenglUtil.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "CSItemOption.h"
#include "CSChaosCastle.h"
#include "MapManager.h"
#include "CharacterManager.h"
#include "SkillManager.h"
#include "GIPetManager.h"
#include "HelperSystem.h"
#include "AutoPotion.h"
#include "GMDoppelGanger1.h"
#include "GMDoppelGanger2.h"
#include "GMDoppelGanger3.h"
#include "GMDoppelGanger4.h"
#include "./Time/CTimCheck.h"
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
#include "MonkSystem.h"
#endif //PBG_ADD_NEWCHAR_MONK_SKILL

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
#include "GameShop/InGameShopSystem.h"
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME

#include "WideScreen.h"
#include <vector>

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int  MouseUpdateTime;
extern int  MouseUpdateTimeMax;
extern int SelectedCharacter;
extern int Attacking;

namespace
{
	const BYTE SKILL_SHOP_CLIENT_SUBCODE = 0xD6;
	const BYTE SKILL_SHOP_LIST_SUBCODE = 0xD7;
	const BYTE SKILL_SHOP_ACTION_REQUEST_LIST = 0x00;
	const BYTE SKILL_SHOP_ACTION_BUY_BUFF = 0x01;
	const float SKILL_SHOP_WINDOW_WIDTH = 172.f;
	const float SKILL_SHOP_WINDOW_HEIGHT = 335.f;
	const int SKILL_SHOP_SKILL_COLUMNS = 5;
	const int SKILL_SHOP_BUFF_COLUMNS = 5;
	const int SKILL_SHOP_SKILLS_PER_PAGE = 15;
	const int SKILL_SHOP_BUFFS_PER_PAGE = 10;
	const float SKILL_SHOP_ICON_SOURCE_WIDTH = 20.f;
	const float SKILL_SHOP_ICON_SOURCE_HEIGHT = 28.f;
	const float SKILL_SHOP_CELL_WIDTH = 32.f;
	const float SKILL_SHOP_CELL_HEIGHT = 38.f;
	const float SKILL_SHOP_CELL_STEP_X = 32.f;
	const float SKILL_SHOP_CELL_STEP_Y = 38.f;
	const float SKILL_SHOP_ICON_WIDTH = 20.f;
	const float SKILL_SHOP_ICON_HEIGHT = 28.f;
	const float SKILL_SHOP_SKILL_START_X = 6.f;
	const float SKILL_SHOP_SKILL_START_Y = 58.f;
	const float SKILL_SHOP_SKILL_PAGE_Y = 176.f;
	const float SKILL_SHOP_BUFF_START_X = 6.f;
	const float SKILL_SHOP_BUFF_START_Y = 224.f;
	const float SKILL_SHOP_BUFF_PAGE_Y = 304.f;
	const int SKILL_SHOP_DISABLED_ENTRY_BASE = 10000;

	bool IsFenrirItemForPlasmaStorm(const ITEM& item)
	{
		if(item.Type == -1)
		{
			return false;
		}

		if(item.Type == ITEM_HELPER + 37)
		{
			return item.Durability > 0;
		}

		return gHelperSystem.CheckHelperType(item.Type + MODEL_ITEM, 8);
	}

	bool IsFenrirEquippedForPlasmaStorm()
	{
		if(Hero == NULL || CharacterMachine == NULL)
		{
			return false;
		}

		const int helperSlots[] = { EQUIPMENT_HELPER, NEW_EQUIPMENT_HELPER_FLY, NEW_EQUIPMENT_HELPER_AUX, NEW_EQUIPMENT_HELPER };
		for(int i = 0; i < (int)(sizeof(helperSlots) / sizeof(helperSlots[0])); ++i)
		{
			if(IsFenrirItemForPlasmaStorm(CharacterMachine->Equipment[helperSlots[i]]) == true)
			{
				return true;
			}
		}

		return false;
	}

	void SyncFenrirPlasmaStormSkill()
	{
		if(CharacterAttribute == NULL)
		{
			return;
		}

		if(IsFenrirEquippedForPlasmaStorm())
		{
			CharacterAttribute->Skill[AT_SKILL_PLASMA_STORM_FENRIR] = AT_SKILL_PLASMA_STORM_FENRIR;
		}
		else if(CharacterAttribute->Skill[AT_SKILL_PLASMA_STORM_FENRIR] == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			CharacterAttribute->Skill[AT_SKILL_PLASMA_STORM_FENRIR] = 0;
			CharacterAttribute->SkillDelay[AT_SKILL_PLASMA_STORM_FENRIR] = 0;
		}
	}

	bool IsFenrirPlasmaStormSkillEntry(int skillIndex)
	{
		if(skillIndex != AT_SKILL_PLASMA_STORM_FENRIR)
		{
			return false;
		}

		SyncFenrirPlasmaStormSkill();
		return CharacterAttribute != NULL
			&& CharacterAttribute->Skill[AT_SKILL_PLASMA_STORM_FENRIR] == AT_SKILL_PLASMA_STORM_FENRIR
			&& IsFenrirEquippedForPlasmaStorm();
	}

	bool IsDarkHorseItemForEarthquake(const ITEM& item)
	{
		if(item.Type == -1 || item.Durability <= 0)
		{
			return false;
		}

		return item.Type == ITEM_HELPER + 4 || gHelperSystem.CheckHelperType(item.Type + MODEL_ITEM, 16);
	}

	bool IsDarkHorseEquippedForEarthquake()
	{
		if(Hero == NULL || CharacterMachine == NULL)
		{
			return false;
		}

		const int helperSlots[] = { EQUIPMENT_HELPER, NEW_EQUIPMENT_HELPER_FLY, NEW_EQUIPMENT_HELPER_AUX, NEW_EQUIPMENT_HELPER };
		for(int i = 0; i < (int)(sizeof(helperSlots) / sizeof(helperSlots[0])); ++i)
		{
			if(IsDarkHorseItemForEarthquake(CharacterMachine->Equipment[helperSlots[i]]) == true)
			{
				return true;
			}
		}

		return false;
	}

	bool IsShieldEquippedForBlocking()
	{
		if(Hero == NULL || CharacterMachine == NULL)
		{
			return false;
		}

		const ITEM& item = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
		return item.Type >= ITEM_SHIELD && item.Type < ITEM_SHIELD + MAX_ITEM_INDEX && item.Durability > 0;
	}

	void SyncShieldBlockingSkill()
	{
		if(CharacterAttribute == NULL)
		{
			return;
		}

		if(IsShieldEquippedForBlocking())
		{
			CharacterAttribute->Skill[AT_SKILL_BLOCKING] = AT_SKILL_BLOCKING;
		}
		else if(CharacterAttribute->Skill[AT_SKILL_BLOCKING] == AT_SKILL_BLOCKING)
		{
			CharacterAttribute->Skill[AT_SKILL_BLOCKING] = 0;
			CharacterAttribute->SkillDelay[AT_SKILL_BLOCKING] = 0;
		}
	}

	bool IsShieldBlockingSkillEntry(int skillIndex)
	{
		if(skillIndex != AT_SKILL_BLOCKING)
		{
			return false;
		}

		SyncShieldBlockingSkill();
		return CharacterAttribute != NULL
			&& CharacterAttribute->Skill[AT_SKILL_BLOCKING] == AT_SKILL_BLOCKING
			&& IsShieldEquippedForBlocking();
	}
	struct PMSG_CLIENT_SKILL_SHOP_REQUEST_SEND
	{
		NEW_PSBMSG_HEAD header;
		BYTE action;
		BYTE index;
	};

	struct PMSG_CLIENT_SKILL_SHOP_LIST_RECV
	{
		NEW_PSWMSG_HEAD header;
		BYTE count;
	};

	void FormatSkillShopPrice(DWORD price, char* buffer, size_t bufferSize)
	{
		if(buffer == NULL || bufferSize == 0)
		{
			return;
		}

		if(price >= 1000000)
		{
			_snprintf(buffer, bufferSize, "%ukk", (unsigned int)(price / 1000000));
		}
		else if(price >= 1000)
		{
			_snprintf(buffer, bufferSize, "%uk", (unsigned int)(price / 1000));
		}
		else
		{
			_snprintf(buffer, bufferSize, "%u", (unsigned int)price);
		}

		buffer[bufferSize - 1] = '\0';
	}

	void FormatSkillShopDuration(DWORD seconds, char* buffer, size_t bufferSize)
	{
		if(buffer == NULL || bufferSize == 0)
		{
			return;
		}

		if(seconds >= 3600)
		{
			DWORD hours = seconds / 3600;
			DWORD minutes = (seconds % 3600) / 60;
			_snprintf(buffer, bufferSize, "%dh %02dm", hours, minutes);
		}
		else if(seconds >= 60)
		{
			_snprintf(buffer, bufferSize, "%dm", seconds / 60);
		}
		else
		{
			_snprintf(buffer, bufferSize, "%ds", seconds);
		}

		buffer[bufferSize - 1] = '\0';
	}

	void GetSkillShopBuffDescription(WORD effect, DWORD value1, DWORD value2, DWORD value3, DWORD value4, char* buffer, size_t bufferSize)
	{
		if(buffer == NULL || bufferSize == 0)
		{
			return;
		}

		switch(effect)
		{
		case 44:
			_snprintf(buffer, bufferSize, "Velocidade de ataque +%u", value1);
			break;
		case 45:
			_snprintf(buffer, bufferSize, "Defesa +%u", value1);
			break;
		case 46:
			_snprintf(buffer, bufferSize, "Dano fisico +%u", value1);
			break;
		case 47:
			_snprintf(buffer, bufferSize, "Dano magico +%u", value1);
			break;
		case 48:
			_snprintf(buffer, bufferSize, "Vida maxima +%u", value1);
			break;
		case 49:
			_snprintf(buffer, bufferSize, "Mana maxima +%u", value1);
			break;
		case 89:
			_snprintf(buffer, bufferSize, "Chance de dano critico +%u%%", value1);
			break;
		case 90:
			_snprintf(buffer, bufferSize, "Chance de dano excelente +%u%%", value1);
			break;
		case 121:
			_snprintf(buffer, bufferSize, "Recupera HP +%u%% e vida +%u", value1, value2);
			break;
		default:
			if(value1 != 0 || value2 != 0 || value3 != 0 || value4 != 0)
			{
				_snprintf(buffer, bufferSize, "Bonus: %u / %u / %u / %u", value1, value2, value3, value4);
			}
			else
			{
				_snprintf(buffer, bufferSize, "Efeito de buff %u", effect);
			}
			break;
		}

		buffer[bufferSize - 1] = '\0';
	}

	int GetSkillShopCustomBuffBitmap(int effect)
	{
		switch(effect)
		{
		case 44:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 0;
		case 45:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 1;
		case 46:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 2;
		case 47:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 3;
		case 48:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 4;
		case 49:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 5;
		case 89:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 2;
		case 90:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 3;
		default:
			return 0;
		}
	}
	void PositionSkillShopTooltipOutsideWindow(float tooltipW, float tooltipH, float shopX, float shopY, float shopW, float shopH, float& tooltipX, float& tooltipY)
	{
		tooltipX = shopX + shopW + 8.f;
		tooltipY = (float)MouseY - (tooltipH * 0.5f);

		if(tooltipX + tooltipW > gWideScreen.g_WideWindowWidth - 4.f)
		{
			tooltipX = shopX - tooltipW - 8.f;
		}

		if(tooltipX < 4.f)
		{
			tooltipX = (float)MouseX + 14.f;
			if(tooltipX + tooltipW > gWideScreen.g_WideWindowWidth - 4.f)
			{
				tooltipX = (float)MouseX - tooltipW - 12.f;
			}
		}

		if(tooltipY + tooltipH > gWideScreen.g_WideWindowHeight - 4.f)
		{
			tooltipY = gWideScreen.g_WideWindowHeight - tooltipH - 4.f;
		}

		if(tooltipY < 4.f)
		{
			tooltipY = 4.f;
		}
	}

	void RenderSkillShopSkillTypeTooltip(int skillType, float shopX, float shopY, float shopW, float shopH)
	{
		if(skillType <= 0 || skillType >= MAX_SKILLS)
		{
			return;
		}

		char name[64] = { 0 };
		int mana = 0;
		int distance = 0;
		int skillMana = 0;
		gSkillManager.GetSkillInformation(skillType, 1, name, &mana, &distance, &skillMana);

		if(name[0] == '\0')
		{
			_snprintf(name, sizeof(name), "%s", SkillAttribute[skillType].Name);
			name[sizeof(name) - 1] = '\0';
		}

		const float tooltipW = 154.f;
		const float tooltipH = 76.f;
		float tooltipX = 0.f;
		float tooltipY = 0.f;
		PositionSkillShopTooltipOutsideWindow(tooltipW, tooltipH, shopX, shopY, shopW, shopH, tooltipX, tooltipY);

		char manaText[48] = { 0 };
		char distanceText[48] = { 0 };
		char skillManaText[48] = { 0 };

		_snprintf(manaText, sizeof(manaText), "Mana: %d", mana);
		_snprintf(distanceText, sizeof(distanceText), "Alcance: %d", distance);
		_snprintf(skillManaText, sizeof(skillManaText), "AG: %d", skillMana);
		manaText[sizeof(manaText) - 1] = '\0';
		distanceText[sizeof(distanceText) - 1] = '\0';
		skillManaText[sizeof(skillManaText) - 1] = '\0';

		EnableAlphaTest();
		glColor4f(0.f, 0.f, 0.f, 0.84f);
		RenderColor(tooltipX, tooltipY, tooltipW, tooltipH);
		glColor4f(0.21f, 0.10f, 0.04f, 0.92f);
		RenderColor(tooltipX, tooltipY, tooltipW, 15.f);
		EndRenderColor();

		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetTextColor(255, 185, 35, 255);
		g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 3, name, (int)tooltipW - 12, 0, 0);

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetTextColor(190, 190, 190, 255);
		g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 22, "Skill ainda nao aprendida.", (int)tooltipW - 12, 0, 0);
		g_pRenderText->SetTextColor(230, 230, 230, 255);
		g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 36, manaText, (int)tooltipW - 12, 0, 0);
		g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 50, distanceText, (int)tooltipW - 12, 0, 0);
		if(skillMana > 0)
		{
			g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 64, skillManaText, (int)tooltipW - 12, 0, 0);
		}
	}

	float GetSkillShopViewportWidth()
	{
		float width = gWideScreen.g_WideWindowWidth;
		if(width < (SKILL_SHOP_WINDOW_WIDTH + 8.f))
		{
			width = 640.f;
		}

		return width;
	}

	float GetSkillShopViewportHeight()
	{
		float height = gWideScreen.g_WideWindowHeight;
		if(height < (SKILL_SHOP_WINDOW_HEIGHT + 8.f))
		{
			height = 480.f;
		}

		return height;
	}

	bool IsSkillShopLeftButtonDown()
	{
		return (MouseLButton || HIBYTE(GetAsyncKeyState(VK_LBUTTON)) == 128);
	}

	POINT GetSkillShopClampedMouse()
	{
		POINT mouse;
		mouse.x = (long)MouseX;
		mouse.y = (long)MouseY;

		const int maxX = (int)GetSkillShopViewportWidth();
		const int maxY = (int)GetSkillShopViewportHeight();

		if(mouse.x < 0) { mouse.x = 0; }
		if(mouse.y < 0) { mouse.y = 0; }
		if(mouse.x > maxX) { mouse.x = maxX; }
		if(mouse.y > maxY) { mouse.y = maxY; }

		return mouse;
	}

	void CenterSkillShopWindow(POINT& pos)
	{
		pos.x = (long)((GetSkillShopViewportWidth() - SKILL_SHOP_WINDOW_WIDTH) / 2.f);
		pos.y = (long)((GetSkillShopViewportHeight() - SKILL_SHOP_WINDOW_HEIGHT) / 2.f);

		if(pos.x < 4) { pos.x = 4; }
		if(pos.y < 4) { pos.y = 4; }
	}

	void ClampSkillShopWindow(POINT& pos)
	{
		int maxX = (int)(GetSkillShopViewportWidth() - SKILL_SHOP_WINDOW_WIDTH - 4.f);
		int maxY = (int)(GetSkillShopViewportHeight() - SKILL_SHOP_WINDOW_HEIGHT - 4.f);

		if(maxX < 4)
		{
			maxX = 4;
		}

		if(maxY < 4)
		{
			maxY = 4;
		}

		if(pos.x < 4) { pos.x = 4; }
		if(pos.x > maxX) { pos.x = maxX; }
		if(pos.y < 4) { pos.y = 4; }
		if(pos.y > maxY) { pos.y = maxY; }
	}

	int GetSkillShopHotKeySlot(const int* hotKeySkillType, int skillIndex)
	{
		for(int i = 0; i < 10; ++i)
		{
			if(hotKeySkillType[i] == skillIndex)
			{
				return i;
			}
		}

		return -1;
	}

	int MakeSkillShopDisabledEntry(int skillType)
	{
		return SKILL_SHOP_DISABLED_ENTRY_BASE + skillType;
	}

	bool IsSkillShopDisabledEntry(int entry)
	{
		return (entry >= SKILL_SHOP_DISABLED_ENTRY_BASE);
	}

	bool IsWeaponSkillForSkillBar(int skillType)
	{
		switch(skillType)
		{
		case AT_SKILL_BLOCKING:
		case AT_SKILL_SWORD1:
		case AT_SKILL_SWORD2:
		case AT_SKILL_SWORD3:
		case AT_SKILL_SWORD4:
		case AT_SKILL_SWORD5:
		case AT_SKILL_CROSSBOW:
		case AT_SKILL_BOW:
		case AT_SKILL_RUSH:
		case AT_SKILL_SPEAR:
		case AT_SKILL_RIDER:
		case AT_SKILL_REDUCEDEFENSE:
		case AT_SKILL_ICE_BLADE:
		case AT_SKILL_ONEFLASH:
		case AT_SKILL_LONG_SPEAR:
		case AT_SKILL_EXPLODE:
			return true;
		default:
			return false;
		}
	}

	bool IsClassicSkillBarSkillType(int skillType)
	{
		if(skillType == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			return IsFenrirPlasmaStormSkillEntry(AT_SKILL_PLASMA_STORM_FENRIR);
		}

		if(skillType == AT_SKILL_BLOCKING)
		{
			return IsShieldBlockingSkillEntry(AT_SKILL_BLOCKING);
		}

		if(skillType >= AT_PET_COMMAND_DEFAULT && skillType < AT_PET_COMMAND_END)
		{
			return true;
		}

		if(skillType <= 0 || skillType >= MAX_SKILLS)
		{
			return false;
		}

		if(IsWeaponSkillForSkillBar(skillType) == true)
		{
			return false;
		}

		if(skillType >= AT_SKILL_STUN && skillType <= AT_SKILL_REMOVAL_BUFF)
		{
			return false;
		}

		if(skillType >= AT_SKILL_CURSED_TEMPLE_PRODECTION && skillType <= AT_SKILL_CURSED_TEMPLE_SUBLIMATION)
		{
			return false;
		}

		BYTE bySkillUseType = SkillAttribute[skillType].SkillUseType;
		if(bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
		{
			return false;
		}

		return true;
	}

	bool IsClassicSkillBarEntry(int skillIndex)
	{
		if(IsFenrirPlasmaStormSkillEntry(skillIndex) == true)
		{
			return true;
		}

		if(IsShieldBlockingSkillEntry(skillIndex) == true)
		{
			return true;
		}

		if(skillIndex >= AT_PET_COMMAND_DEFAULT && skillIndex < AT_PET_COMMAND_END)
		{
			return true;
		}

		if(skillIndex < 0 || skillIndex >= MAX_MAGIC)
		{
			return false;
		}

		return IsClassicSkillBarSkillType(CharacterAttribute->Skill[skillIndex]);
	}

	int FindFirstClassicSkillBarEntry()
	{
		for(int i = 0; i < MAX_MAGIC; ++i)
		{
			if(IsClassicSkillBarEntry(i) == true)
			{
				return i;
			}
		}

		return 0;
	}
	int GetSkillShopEntrySkillIndex(int entry)
	{
		if(IsSkillShopDisabledEntry(entry) == true)
		{
			return -1;
		}

		if(entry == AT_SKILL_PLASMA_STORM_FENRIR || entry == AT_SKILL_BLOCKING)
		{
			return entry;
		}

		if(entry >= AT_PET_COMMAND_DEFAULT && entry < AT_PET_COMMAND_END)
		{
			return entry;
		}

		return entry;
	}

	int GetSkillShopEntrySkillType(int entry)
	{
		if(IsSkillShopDisabledEntry(entry) == true)
		{
			return entry - SKILL_SHOP_DISABLED_ENTRY_BASE;
		}

		if(entry == AT_SKILL_PLASMA_STORM_FENRIR || entry == AT_SKILL_BLOCKING)
		{
			return entry;
		}

		if(entry >= AT_PET_COMMAND_DEFAULT && entry < AT_PET_COMMAND_END)
		{
			return entry;
		}

		if(entry < 0 || entry >= MAX_MAGIC)
		{
			return 0;
		}

		return CharacterAttribute->Skill[entry];
	}

	bool IsSkillShopLearnedSkillType(int skillType)
	{
		for(int i = 0; i < MAX_MAGIC; ++i)
		{
			if(IsClassicSkillBarEntry(i) == true && CharacterAttribute->Skill[i] == skillType)
			{
				return true;
			}
		}

		return false;
	}

	int GetSkillShopLearnedSkillIndex(int skillType)
	{
		for(int i = 0; i < MAX_MAGIC; ++i)
		{
			if(IsClassicSkillBarEntry(i) == true && CharacterAttribute->Skill[i] == skillType)
			{
				return i;
			}
		}

		return -1;
	}

	bool IsSkillShopSkillTypeInList(const std::vector<int>& skillList, int skillType)
	{
		for(std::vector<int>::const_iterator it = skillList.begin(); it != skillList.end(); ++it)
		{
			if(GetSkillShopEntrySkillType(*it) == skillType)
			{
				return true;
			}
		}

		return false;
	}

	bool IsSkillShop97DSkillType(int skillType)
	{
		switch(skillType)
		{
		case AT_SKILL_POISON:
		case AT_SKILL_METEO:
		case AT_SKILL_THUNDER:
		case AT_SKILL_FIREBALL:
		case AT_SKILL_FLAME:
		case AT_SKILL_TELEPORT:
		case AT_SKILL_SLOW:
		case AT_SKILL_STORM:
		case AT_SKILL_EVIL:
		case AT_SKILL_HELL:
		case AT_SKILL_POWERWAVE:
		case AT_SKILL_FLASH:
		case AT_SKILL_BLAST:
		case AT_SKILL_INFERNO:
		case AT_SKILL_TELEPORT_B:
		case AT_SKILL_WIZARDDEFENSE:
		case AT_SKILL_ENERGYBALL:
		case AT_SKILL_CROSSBOW:
		case AT_SKILL_BOW:
		case AT_SKILL_HEALING:
		case AT_SKILL_DEFENSE:
		case AT_SKILL_ATTACK:
		case AT_SKILL_SUMMON + 1:
		case AT_SKILL_SUMMON + 2:
		case AT_SKILL_SUMMON + 3:
		case AT_SKILL_SUMMON + 4:
		case AT_SKILL_BLAST_POISON:
		case AT_SKILL_BLAST_FREEZE:
		case AT_SKILL_WHEEL:
		case AT_SKILL_FURY_STRIKE:
		case AT_SKILL_ONETOONE:
		case AT_SKILL_DEEPIMPACT:
		case AT_SKILL_SPEAR:
		case AT_SKILL_VITALITY:
		case AT_SKILL_RIDER:
		case AT_SKILL_PARALYZE:
		case AT_SKILL_PIERCING:
		case AT_SKILL_IMPROVE_AG:
		case AT_SKILL_BLAST_CROSSBOW4:
		case AT_SKILL_REDUCEDEFENSE:
		case AT_SKILL_ICE_BLADE:
		case AT_SKILL_ONEFLASH:
		case AT_SKILL_STRONG_PIER:
		case AT_SKILL_LONGPIER_ATTACK:
		case AT_SKILL_DARK_HORSE:
		case AT_SKILL_PARTY_TELEPORT:
		case AT_SKILL_ADD_CRITICAL:
		case AT_SKILL_THUNDER_STRIKE:
		case AT_SKILL_DARK_SCREAM:
			return true;
		default:
			return false;
		}
	}

	bool IsSkillShopRemovedSkillType(int skillType)
	{
		if(skillType == AT_SKILL_BLOCKING)
		{
			return false;
		}

		if(IsWeaponSkillForSkillBar(skillType) == true)
		{
			return true;
		}

		switch(skillType)
		{
		case AT_SKILL_SPEAR:          // Impale
		case AT_SKILL_REDUCEDEFENSE:  // Fire Slash
		case AT_SKILL_ICE_BLADE:      // Power Slash
		case AT_SKILL_ONEFLASH:       // Spiral Slash
			return true;
		default:
			return false;
		}
	}

	bool IsSkillShopBlockedFor97DClass(int skillType, int baseClass)
	{
		switch(baseClass)
		{
		case CLASS_KNIGHT:
			return (skillType == AT_SKILL_RIDER);
		case CLASS_DARK:
			return (skillType == AT_SKILL_FLAME || skillType == AT_SKILL_EVIL || skillType == AT_SKILL_HELL ||
				skillType == AT_SKILL_INFERNO || skillType == AT_SKILL_BLAST_FREEZE);
		case CLASS_DARK_LORD:
			return (skillType == AT_SKILL_STRONG_PIER || skillType == AT_SKILL_DARK_HORSE || skillType == AT_SKILL_PARTY_TELEPORT ||
				skillType == AT_SKILL_ADD_CRITICAL || skillType == AT_SKILL_THUNDER_STRIKE);
		default:
			return false;
		}
	}

	bool IsSkillShopFenrirEquipped()
	{
		return IsFenrirEquippedForPlasmaStorm();
	}

	bool IsSkillShopShieldEquipped()
	{
		return IsShieldEquippedForBlocking();
	}

	bool IsSkillShopDisplayableSkillType(int skillType)
	{
		if(skillType <= 0 || skillType >= MAX_SKILLS)
		{
			return false;
		}

		if(skillType == 74)
		{
			return false;
		}

		if(IsSkillShopRemovedSkillType(skillType) == true)
		{
			return false;
		}

		if(SkillAttribute[skillType].Name[0] == '\0')
		{
			return false;
		}

		if(skillType >= AT_SKILL_STUN && skillType <= AT_SKILL_REMOVAL_BUFF)
		{
			return false;
		}

		if(skillType >= AT_SKILL_CURSED_TEMPLE_PRODECTION && skillType <= AT_SKILL_CURSED_TEMPLE_SUBLIMATION)
		{
			return false;
		}

		if(skillType >= AT_SKILL_ATTACK_RATEUP)
		{
			return false;
		}

		if(skillType >= AT_SKILL_MONSTER_SUMMON && skillType <= AT_SKILL_MONSTER_PHY_DEF)
		{
			return false;
		}

		if(skillType >= AT_SKILL_SWORD1 && skillType <= AT_SKILL_SWORD5)
		{
			return false;
		}

		if(skillType == AT_SKILL_DOPPELGANGER_SELFDESTRUCTION)
		{
			return false;
		}

		if(skillType == AT_SKILL_LONG_SPEAR || skillType == AT_SKILL_EXPLODE)
		{
			return false;
		}

		BYTE bySkillUseType = SkillAttribute[skillType].SkillUseType;
		if(bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
		{
			return false;
		}

		if(SkillAttribute[skillType].KillCount != 0)
		{
			return false;
		}

		for(int n = 0; n < MAX_DUTY_CLASS; ++n)
		{
			if(SkillAttribute[skillType].RequireDutyClass[n] != 0)
			{
				return false;
			}
		}

		if(skillType != AT_SKILL_PLASMA_STORM_FENRIR && skillType != AT_SKILL_BLOCKING && IsSkillShop97DSkillType(skillType) == false)
		{
			return false;
		}

		return true;
	}

	bool IsSkillShopClassSkillType(int skillType)
	{
		if(IsSkillShopDisplayableSkillType(skillType) == false)
		{
			return false;
		}

		if(IsSkillShop97DSkillType(skillType) == false)
		{
			return false;
		}

		int baseClass = gCharacterManager.GetBaseClass(Hero->Class);
		if(baseClass < 0 || baseClass >= MAX_CLASS)
		{
			return false;
		}

		if(IsSkillShopBlockedFor97DClass(skillType, baseClass) == true)
		{
			return false;
		}

		return (SkillAttribute[skillType].RequireClass[baseClass] > 0 && SkillAttribute[skillType].RequireClass[baseClass] <= 2);
	}

	void AddSkillShopSkillType(std::vector<int>& skillList, int skillType, bool enabled)
	{
		if(IsSkillShopDisplayableSkillType(skillType) == false)
		{
			return;
		}

		if(IsSkillShopSkillTypeInList(skillList, skillType) == true)
		{
			return;
		}

		if(skillType == AT_SKILL_PLASMA_STORM_FENRIR && enabled == true)
		{
			SyncFenrirPlasmaStormSkill();
			skillList.push_back(AT_SKILL_PLASMA_STORM_FENRIR);
			return;
		}

		if(skillType == AT_SKILL_BLOCKING && enabled == true)
		{
			SyncShieldBlockingSkill();
			skillList.push_back(AT_SKILL_BLOCKING);
			return;
		}

		int skillIndex = GetSkillShopLearnedSkillIndex(skillType);
		if(enabled == true && skillIndex != -1)
		{
			skillList.push_back(skillIndex);
		}
		else
		{
			skillList.push_back(MakeSkillShopDisabledEntry(skillType));
		}
	}

	void RenderSkillShopFrame(float x,float y,float width,float height,float red,float green,float blue,float alpha)
	{
		EnableAlphaTest();
		glColor4f(red,green,blue,alpha);
		RenderColor(x,y,width,1.f);
		RenderColor(x,y+height-1.f,width,1.f);
		RenderColor(x,y,1.f,height);
		RenderColor(x+width-1.f,y,1.f,height);
		EndRenderColor();
	}

	bool g_DIVInterfaceReady = false;
	bool g_DIVPotionBarVisible = true;
	bool g_DIVPotionToggleMouseDown = false;
	bool g_DIVNotificationMouseDown = false;
	bool g_VoidMainButtonMouseDown = false;
	bool g_DIVSkillBarRendering = false;
	bool g_VoidMainInterfaceReady = false;

	const int VOID_MAIN_IMAGE_FRAME_LEFT = 120300;
	const int VOID_MAIN_IMAGE_FRAME_RIGHT = 120301;
	const int VOID_MAIN_IMAGE_GAUGE_BLUE = 120303;
	const int VOID_MAIN_IMAGE_GAUGE_GREEN = 120304;
	const int VOID_MAIN_IMAGE_GAUGE_RED = 120305;
	const int VOID_MAIN_IMAGE_GAUGE_AG = 120306;
	const int VOID_MAIN_IMAGE_GAUGE_SD = 120307;
	const int VOID_MAIN_IMAGE_BTN_AUTOPOTION = 120308;
	const int VOID_MAIN_IMAGE_BTN_MENU = 120309;
	const int VOID_MAIN_IMAGE_BTN_PARTY = 120310;
	const int VOID_MAIN_IMAGE_BTN_INVENTORY = 120311;
	const int VOID_MAIN_IMAGE_BTN_CHARACTER = 120312;
	const int VOID_MAIN_IMAGE_BTN_LOCK = 120313;
	const int VOID_MAIN_IMAGE_BTN_GUILD = 120314;

	bool DIVTextureExists(const char* logicalPath);

	bool AreVoidMainAssetsAvailable()
	{
		const char* files[] =
		{
			"Custom\\Interface\\main_frame_left.tga",
			"Custom\\Interface\\main_frame_right.tga",
			"Custom\\Interface\\Menu_Blue.tga",
			"Custom\\Interface\\Menu_Green.tga",
			"Custom\\Interface\\Menu_Red.tga",
			"Custom\\Interface\\Menu_AG.jpg",
			"Custom\\Interface\\Menu_SD.jpg",
			"Custom\\Interface\\btn_autopotion.jpg",
			"Custom\\Interface\\btn_menu.jpg",
			"Custom\\Interface\\btn_party_v2.jpg",
			"Custom\\Interface\\btn_inventory_v2.jpg",
			"Custom\\Interface\\btn_character_v2.jpg",
			"Custom\\Interface\\btn_lock_v2.jpg",
			"Custom\\Interface\\btn_guild_v2.jpg",
			"Custom\\Interface\\newui_skillbox.jpg",
			"Custom\\Interface\\newui_skillbox2.jpg"
		};

		for (int i = 0; i < (int)(sizeof(files) / sizeof(files[0])); ++i)
		{
			if (DIVTextureExists(files[i]) == false)
			{
				return false;
			}
		}

		return true;
	}

	float GetVoidMainX()
	{
		return (float)gWideScreen.GetCenterPosition(0);
	}

	float GetVoidMainY()
	{
		return gWideScreen.GetBottomPosition(419.f);
	}

	float GetVoidOrbY()
	{
		return gWideScreen.GetBottomPosition(426.f);
	}

	float GetVoidVerticalGaugeY()
	{
		return gWideScreen.GetBottomPosition(438.f);
	}

	float GetVoidNumberY()
	{
		return gWideScreen.GetBottomPosition(468.f);
	}

	float GetVoidCurrentSkillX()
	{
		return GetVoidMainX() + 310.f;
	}

	float GetVoidCurrentSkillY()
	{
		return gWideScreen.GetBottomPosition(447.f);
	}

	float GetVoidPotionItemX(int index)
	{
		const float positions[3] = { 210.f, 242.f, 273.f };
		return GetVoidMainX() + positions[index];
	}

	float GetVoidPotionCountX(int index)
	{
		const float positions[3] = { 220.f, 252.f, 285.f };
		return GetVoidMainX() + positions[index];
	}

	float GetVoidPotionItemY()
	{
		return gWideScreen.GetBottomPosition(453.f);
	}

	float GetVoidPotionCountY()
	{
		return gWideScreen.GetBottomPosition(444.f);
	}

	void RenderVoidAtlasButton(int image, float x, float y, float width, float height, bool pressed)
	{
		float v = 0.002f;
		if (pressed == true)
		{
			v = 0.429f;
		}
		else if (SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			v = 0.213f;
		}

		RenderBitmap(image, x, y, width, height, 0.008f, v, 0.83f, 0.20f, true, true, 0.f);
	}

	void RenderVoidSkillAtlasBox(float x, float y, bool selected)
	{
		const int image = selected ? SEASON3B::CNewUISkillList::IMAGE_SKILLBOX_USE : SEASON3B::CNewUISkillList::IMAGE_SKILLBOX;
		RenderBitmap(image, x, y, 32.f, 38.f, 0.f, 0.f, 0.53f, 0.61926f, true, true, 0.f);
	}

	enum EVoidMainButton
	{
		VOID_MAIN_BUTTON_NONE = 0,
		VOID_MAIN_BUTTON_AUTOPOTION,
		VOID_MAIN_BUTTON_GUILD,
		VOID_MAIN_BUTTON_PARTY,
		VOID_MAIN_BUTTON_CHARACTER,
		VOID_MAIN_BUTTON_INVENTORY,
		VOID_MAIN_BUTTON_LOCK
	};

	EVoidMainButton GetVoidMainButtonUnderMouse()
	{
		if (g_VoidMainInterfaceReady == false)
		{
			return VOID_MAIN_BUTTON_NONE;
		}

		const float x = GetVoidMainX();
		const float y = GetVoidMainY();

		if (SEASON3B::CheckMouseIn(x + 92.f, y + 18.f, 16.f, 16.f) == true)
		{
			return VOID_MAIN_BUTTON_AUTOPOTION;
		}
		if (SEASON3B::CheckMouseIn(x + 92.f, y + 39.f, 16.f, 16.f) == true)
		{
			return VOID_MAIN_BUTTON_GUILD;
		}
		if (SEASON3B::CheckMouseIn(x + 345.f, y + 31.f, 25.f, 24.f) == true)
		{
			return VOID_MAIN_BUTTON_PARTY;
		}
		if (SEASON3B::CheckMouseIn(x + 376.5f, y + 31.f, 25.f, 24.f) == true)
		{
			return VOID_MAIN_BUTTON_CHARACTER;
		}
		if (SEASON3B::CheckMouseIn(x + 408.f, y + 31.f, 25.f, 24.f) == true)
		{
			return VOID_MAIN_BUTTON_INVENTORY;
		}
		if (SEASON3B::CheckMouseIn(x + 526.f, y + 31.f, 25.f, 24.f) == true)
		{
			return VOID_MAIN_BUTTON_LOCK;
		}

		return VOID_MAIN_BUTTON_NONE;
	}

	void ToggleVoidPartyWindow()
	{
		g_pNewUISystem->Toggle(SEASON3B::INTERFACE_PARTY);

		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);
		}
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY))
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
		}
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO))
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_GUILDINFO);
		}
	}

	bool ProcessVoidMainButtonClick()
	{
		if (g_VoidMainInterfaceReady == false)
		{
			g_VoidMainButtonMouseDown = false;
			return false;
		}

		const EVoidMainButton button = GetVoidMainButtonUnderMouse();

		if (button == VOID_MAIN_BUTTON_NONE)
		{
			if (SEASON3B::IsNone(VK_LBUTTON) == true || SEASON3B::IsRelease(VK_LBUTTON) == true || MouseLButtonPop == true)
			{
				g_VoidMainButtonMouseDown = false;
			}
			return false;
		}

		const bool mousePressed = (SEASON3B::IsPress(VK_LBUTTON) == true || MouseLButtonPush == true);
		const bool mouseReleased = (SEASON3B::IsRelease(VK_LBUTTON) == true || MouseLButtonPop == true);
		const bool mouseBusy = (mousePressed == true || SEASON3B::IsRepeat(VK_LBUTTON) == true || mouseReleased == true);
		if (mouseBusy == false)
		{
			if (SEASON3B::IsNone(VK_LBUTTON) == true)
			{
				g_VoidMainButtonMouseDown = false;
			}
			return false;
		}

		MouseLButtonPush = false;
		MouseLButtonPop = false;

		if (mousePressed == false)
		{
			if (mouseReleased == true)
			{
				g_VoidMainButtonMouseDown = false;
			}
			return true;
		}

		if (g_VoidMainButtonMouseDown == true)
		{
			return true;
		}

		const bool canOpenInventory = (g_pNewUIHotKey->CanUpdateKeyEventRelatedMyInventory() == true || g_pNewUIHotKey->CanUpdateKeyEvent() == true);
		const bool canOpenGeneral = (g_pNewUIHotKey->CanUpdateKeyEvent() == true);
		const bool canProcess = (button == VOID_MAIN_BUTTON_INVENTORY) ? canOpenInventory : canOpenGeneral;

		g_VoidMainButtonMouseDown = true;

		if (canProcess == false)
		{
			return true;
		}

		switch (button)
		{
		case VOID_MAIN_BUTTON_AUTOPOTION:
			gAutoPotion.Toggle();
			break;
		case VOID_MAIN_BUTTON_GUILD:
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_GUILDINFO);
			break;
		case VOID_MAIN_BUTTON_PARTY:
			ToggleVoidPartyWindow();
			break;
		case VOID_MAIN_BUTTON_CHARACTER:
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);
			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
			{
				g_QuestMng.SendQuestIndexByEtcSelection();
			}
			break;
		case VOID_MAIN_BUTTON_INVENTORY:
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			break;
		case VOID_MAIN_BUTTON_LOCK:
			g_pNewUISystem->m_Lua.Generic_Call("OpenDIVCentralShortcut", ">");
			break;
		default:
			return true;
		}

		PlayBuffer(SOUND_CLICK01);
		return true;
	}

	void RenderVoidMainInfoText()
	{
		if (g_VoidMainInterfaceReady == false || Hero == NULL)
		{
			return;
		}

		char coordText[32];
		_snprintf(coordText, sizeof(coordText), "%d %d", Hero->PositionX, Hero->PositionY);
		coordText[sizeof(coordText) - 1] = '\0';

		char levelText[32];
		const int displayLevel = (CharacterAttribute == NULL) ? 0 : CharacterAttribute->Level;
		_snprintf(levelText, sizeof(levelText), "LV:%d", displayLevel);
		levelText[sizeof(levelText) - 1] = '\0';
		SYSTEMTIME serverTime;
		GetLocalTime(&serverTime);

		const char* mapName = gMapManager.GetMapName(gMapManager.WorldActive);
		if (mapName == NULL)
		{
			mapName = "";
		}

		char serverInfoText[128];
	_snprintf(serverInfoText, sizeof(serverInfoText), "SERVER TIME: %02d:%02d:%02d  |  %s", serverTime.wHour, serverTime.wMinute, serverTime.wSecond, mapName);
		serverInfoText[sizeof(serverInfoText) - 1] = '\0';

		const float x = GetVoidMainX();
		const float y = GetVoidMainY();

		EnableAlphaTest();
		g_pRenderText->SetBgColor(0, 0, 0, 0);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetTextColor(255, 220, 0, 255);
		g_pRenderText->RenderText((int)(x + 49.f), (int)(y + 48.f), coordText, 30, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFontSmall);
	g_pRenderText->RenderText((int)(x + 321.f), (int)(y + 16.f), serverInfoText, 200, 0, RT3_WRITE_CENTER);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->RenderText((int)(x + 561.f), (int)(y + 47.f), levelText, 36, 0, RT3_SORT_CENTER);
	}
	void RenderVoidMainButtons()
	{
		if (g_VoidMainInterfaceReady == false)
		{
			return;
		}

		const float x = GetVoidMainX();
		const float y = GetVoidMainY();
		RenderVoidAtlasButton(VOID_MAIN_IMAGE_BTN_AUTOPOTION, x + 92.f, y + 18.f, 16.f, 16.f, gAutoPotion.IsEnabled());
		RenderVoidAtlasButton(VOID_MAIN_IMAGE_BTN_GUILD, x + 92.f, y + 39.f, 16.f, 16.f, false);
		RenderVoidAtlasButton(VOID_MAIN_IMAGE_BTN_PARTY, x + 345.f, y + 31.f, 25.f, 24.f, g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY));
		RenderVoidAtlasButton(VOID_MAIN_IMAGE_BTN_CHARACTER, x + 376.5f, y + 31.f, 25.f, 24.f, g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER));
		RenderVoidAtlasButton(VOID_MAIN_IMAGE_BTN_INVENTORY, x + 408.f, y + 31.f, 25.f, 24.f, g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY));
		RenderVoidAtlasButton(VOID_MAIN_IMAGE_BTN_LOCK, x + 526.f, y + 31.f, 25.f, 24.f, false);
	}

	enum EDIVShortcutTarget
	{
		DIV_SHORTCUT_NONE = 0,
		DIV_SHORTCUT_MENU,
		DIV_SHORTCUT_SECURITY
	};

	void BuildDIVPhysicalPath(const char* logicalPath, char* physicalPath, size_t physicalPathSize)
	{
		_snprintf(physicalPath, physicalPathSize, "Data\\%s", logicalPath);
		physicalPath[physicalPathSize - 1] = '\0';

		char* extension = strrchr(physicalPath, '.');
		if (extension == NULL)
		{
			return;
		}

		if (_stricmp(extension, ".tga") == 0)
		{
			strcpy(extension, ".ozt");
		}
		else if (_stricmp(extension, ".jpg") == 0)
		{
			strcpy(extension, ".ozj");
		}
	}

	bool DIVTextureExists(const char* logicalPath)
	{
		char physicalPath[MAX_PATH];
		BuildDIVPhysicalPath(logicalPath, physicalPath, sizeof(physicalPath));
		return GetFileAttributesA(physicalPath) != INVALID_FILE_ATTRIBUTES;
	}

	bool AreDIVAssetsAvailable()
	{
		const char* files[] =
		{
			"Interface\\DIV\\DIV_MainUI.tga",
			"Interface\\DIV\\DIV_MainOrbLife.tga",
			"Interface\\DIV\\DIV_MainOrbMana.tga",
			"Interface\\DIV\\DIV_MainOrbPoison.tga",
			"Interface\\DIV\\DIV_MainOrbLine.jpg",
			"Interface\\DIV\\DIV_MainBarAG.tga",
			"Interface\\DIV\\DIV_MainBarSD.tga",
			"Interface\\DIV\\DIV_RenderNumber.tga",
			"Interface\\DIV\\DIV_BackExpBar.tga",
			"Interface\\DIV\\DIV_BarPotion.tga",
			"Interface\\DIV\\DIV_Notification.tga",
			"Interface\\DIV\\DIV_HotKeySkillListUp.tga",
			"Interface\\DIV\\DIV_Skill01.jpg",
			"Interface\\DIV\\DIV_Skill02.jpg",
			"Interface\\DIV\\DIV_SkillCommand.jpg",
			"Interface\\DIV\\DIV_SkillBox.tga",
			"Interface\\DIV\\DIV_non_Skill01.jpg",
			"Interface\\DIV\\DIV_non_Skill02.jpg",
			"Interface\\DIV\\DIV_non_SkillCommand.jpg",
			"Interface\\DIV\\DIV_Skill03.jpg",
			"Interface\\DIV\\DIV_non_Skill03.jpg",
			"Interface\\DIV\\DIV_SkillMaster.jpg",
			"Interface\\DIV\\DIV_non_SkillMaster.jpg"
		};

		for (int i = 0; i < (int)(sizeof(files) / sizeof(files[0])); ++i)
		{
			if (DIVTextureExists(files[i]) == false)
			{
				return false;
			}
		}

		return true;
	}

	float GetDIVMainX()
	{
		return (float)gWideScreen.GetCenterPosition(116);
	}

	float GetDIVMainY()
	{
		return gWideScreen.GetBottomPosition(396.f);
	}

	EDIVShortcutTarget GetDIVShortcutTarget()
	{
		const float mainX = GetDIVMainX();
		const float mainY = GetDIVMainY();
		const float menuCenterX = mainX + 317.f + 8.5f;
		const float menuCenterY = mainY + 11.f + 8.5f;
		const float securityCenterX = mainX + 332.f + 8.5f;
		const float securityCenterY = mainY - 5.f + 8.5f;

		const bool menuHover = SEASON3B::CheckMouseIn((int)(menuCenterX - 17.f), (int)(menuCenterY - 15.f), 34, 30);
		const bool securityHover = SEASON3B::CheckMouseIn((int)(securityCenterX - 17.f), (int)(securityCenterY - 17.f), 34, 30);
		if (menuHover == false && securityHover == false)
		{
			return DIV_SHORTCUT_NONE;
		}

		if (menuHover == true && securityHover == false)
		{
			return DIV_SHORTCUT_MENU;
		}

		if (securityHover == true && menuHover == false)
		{
			return DIV_SHORTCUT_SECURITY;
		}

		const float menuDistance = (MouseX - menuCenterX) * (MouseX - menuCenterX) + (MouseY - menuCenterY) * (MouseY - menuCenterY);
		const float securityDistance = (MouseX - securityCenterX) * (MouseX - securityCenterX) + (MouseY - securityCenterY) * (MouseY - securityCenterY);
		return (securityDistance <= menuDistance) ? DIV_SHORTCUT_SECURITY : DIV_SHORTCUT_MENU;
	}

	float GetDIVPotionSlotX(int index)
	{
		return GetDIVMainX() - 70.f + (index * 20.f);
	}

	float GetDIVPotionItemY()
	{
		return GetDIVMainY() + 45.f;
	}

	float GetDIVPotionCountX(int index)
	{
		return GetDIVPotionSlotX(index) + 9.f;
	}

	float GetDIVPotionCountY()
	{
		return GetDIVMainY() + 65.f;
	}

	float ConvertDIVY(float value)
	{
		return value * (float)WindowHeight / gWideScreen.g_WideWindowHeight;
	}

	float GetDIVOrbLineY(float mainY)
	{
		return gWideScreen.g_WideWindowHeight - (mainY + 72.5f) + 32.f;
	}

	void GetDIVAnimUV(float& coordU, float& coordV)
	{
		const int frame = (int)(timeGetTime() * 0.75f) % 3150 / 50;
		coordU = (float)(frame % 8) * 0.125f;
		coordV = (float)(frame / 8) * 0.125f;
	}

	float GetDIVMenuX(int index)
	{
		return GetDIVMainX() + 432.f + (index * 32.f);
	}

	float GetDIVMenuY()
	{
		return gWideScreen.GetBottomPosition(451.f);
	}

	float DIVRenderNumber(float x, float y, int number, float scale)
	{
		EnableAlphaTest(true);

		if (scale < 0.3f)
		{
			return x;
		}

		float width = 12.f * (scale - 0.25f);
		float height = 16.f * (scale - 0.3f);

		char text[32];
		itoa(number, text, 10);

		const int length = (int)strlen(text);
		x -= width * length / 3.f;

		for (int i = 0; i < length; ++i)
		{
			float u = (float)(text[i] - '0') * 12.f / 128.f;
			RenderBitmap(SEASON3B::DIV_RENDER_NUMBER, x - 0.5f, y, width, height, u, 0.f, 12.f / 128.f, 14.f / 16.f, true, true, 0.f);
			x += width * 0.7f;
		}

		return x;
	}

	void RenderDIVCircle(int texture, float x, float y, float width, float height, float u, float v, float uWidth, float vHeight)
	{
		x *= g_fScreenRate_x;
		y = ConvertDIVY(y);
		width *= g_fScreenRate_x;
		height = ConvertDIVY(height);

		BindTexture(texture);

		const int segments = 36;
		const float angleStep = 2.0f * 3.14159f / segments;

		glBegin(GL_TRIANGLE_FAN);
		for (int i = 0; i <= segments; ++i)
		{
			const float angle = i * angleStep;
			const float angleCos = (float)cos(angle);
			const float angleSin = (float)sin(angle);
			const float px = x + width * 0.5f * angleCos;
			const float py = y + height * 0.5f * angleSin;
			const float texCoordU = u + uWidth * (angleCos + 1.0f) * 0.5f;
			const float texCoordV = v + vHeight * (1.0f - (angleSin + 1.0f) * 0.5f);

			glTexCoord2f(texCoordU, texCoordV);
			glVertex2f(px, py);
		}
		glEnd();
	}
}

SEASON3B::CNewUIMainFrameWindow::CNewUIMainFrameWindow() 
{
	m_bExpEffect = false;
	m_dwExpEffectTime = 0;
	m_dwPreExp = 0;
	m_dwGetExp = 0;
	m_bButtonBlink = false;
}

SEASON3B::CNewUIMainFrameWindow::~CNewUIMainFrameWindow() 
{ 
	Release(); 
}

void SEASON3B::CNewUIMainFrameWindow::LoadImages()
{
#if CLIENT_VERSION >= 1001
	g_VoidMainInterfaceReady = AreVoidMainAssetsAvailable();
	if (g_VoidMainInterfaceReady == true)
	{
		LoadBitmap("Custom\\Interface\\main_frame_left.tga", VOID_MAIN_IMAGE_FRAME_LEFT, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\main_frame_right.tga", VOID_MAIN_IMAGE_FRAME_RIGHT, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\Menu_Blue.tga", VOID_MAIN_IMAGE_GAUGE_BLUE, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\Menu_Green.tga", VOID_MAIN_IMAGE_GAUGE_GREEN, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\Menu_Red.tga", VOID_MAIN_IMAGE_GAUGE_RED, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\Menu_AG.jpg", VOID_MAIN_IMAGE_GAUGE_AG, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\Menu_SD.jpg", VOID_MAIN_IMAGE_GAUGE_SD, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_autopotion.jpg", VOID_MAIN_IMAGE_BTN_AUTOPOTION, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_menu.jpg", VOID_MAIN_IMAGE_BTN_MENU, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_party_v2.jpg", VOID_MAIN_IMAGE_BTN_PARTY, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_inventory_v2.jpg", VOID_MAIN_IMAGE_BTN_INVENTORY, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_character_v2.jpg", VOID_MAIN_IMAGE_BTN_CHARACTER, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_lock_v2.jpg", VOID_MAIN_IMAGE_BTN_LOCK, GL_LINEAR, GL_REPEAT);
		LoadBitmap("Custom\\Interface\\btn_guild_v2.jpg", VOID_MAIN_IMAGE_BTN_GUILD, GL_LINEAR, GL_REPEAT);
		g_DIVInterfaceReady = false;
	}
	else
	{
		g_DIVInterfaceReady = AreDIVAssetsAvailable();
	}

	if (g_DIVInterfaceReady == true)
	{
		LoadBitmap("Interface\\DIV\\DIV_MainUI.tga", DIV_MAIN_UI, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_MainOrbLife.tga", DIV_MAIN_ORB_LIFE, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_MainOrbMana.tga", DIV_MAIN_ORB_MANA, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_MainOrbPoison.tga", DIV_MAIN_ORB_POISON, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_MainOrbLine.jpg", DIV_MAIN_ORB_LINE, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_MainBarAG.tga", IMAGE_GAUGE_AG, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_MainBarSD.tga", IMAGE_GAUGE_SD, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_RenderNumber.tga", DIV_RENDER_NUMBER, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_BackExpBar.tga", DIV_BACK_EXP_BAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_BarPotion.tga", DIV_BAR_POTION, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_Notification.tga", DIV_NOTIFICATION, GL_LINEAR, GL_CLAMP_TO_EDGE);
		LoadBitmap("Interface\\DIV\\DIV_HotKeySkillListUp.tga", DIV_HOTKEY_SKILL_LIST_UP, GL_LINEAR, GL_CLAMP_TO_EDGE);
	}
	else
	{
		LoadBitmap("Interface\\main_frame_life_animated.tga", IMAGE_GAUGE_RED, GL_LINEAR);
		LoadBitmap("Interface\\main_frame_mana_animated.tga", IMAGE_GAUGE_BLUE, GL_LINEAR);
		LoadBitmap("Interface\\main_frame_poison_animated.tga", IMAGE_GAUGE_GREEN, GL_LINEAR);
		LoadBitmap("Interface\\main_frame_ag.jpg", IMAGE_GAUGE_AG, GL_LINEAR);
		LoadBitmap("Interface\\main_frame_sd.jpg", IMAGE_GAUGE_SD, GL_LINEAR);
	}
	LoadBitmap("Interface\\newui_exbar.jpg", IMAGE_GAUGE_EXBAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\Exbar_Master.jpg", IMAGE_MASTER_GAUGE_BAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\main_frame_btn_character.jpg", IMAGE_MENU_BTN_CHAINFO, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\main_frame_btn_inventory.jpg", IMAGE_MENU_BTN_MYINVEN, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\main_frame_btn_party.jpg", IMAGE_MENU_BTN_WINDOW, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\newui_SW_Minimap_Frame.tga", 31602, GL_LINEAR);
#elif CLIENT_VERSION >= 401
	LoadBitmap("Interface\\newui_menu01.jpg", IMAGE_MENU_1, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu02.jpg", IMAGE_MENU_2, GL_LINEAR);
	LoadBitmap("Interface\\partCharge1\\newui_menu03.jpg", IMAGE_MENU_3, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu02-03.jpg", IMAGE_MENU_2_1, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_blue.jpg", IMAGE_GAUGE_BLUE, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_green.jpg", IMAGE_GAUGE_GREEN, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_red.jpg", IMAGE_GAUGE_RED, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_ag.jpg", IMAGE_GAUGE_AG, GL_LINEAR);
	LoadBitmap("Interface\\newui_menu_sd.jpg", IMAGE_GAUGE_SD, GL_LINEAR);
	LoadBitmap("Interface\\newui_exbar.jpg", IMAGE_GAUGE_EXBAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\Exbar_Master.jpg", IMAGE_MASTER_GAUGE_BAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt05.jpg", IMAGE_MENU_BTN_CSHOP, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt01.jpg", IMAGE_MENU_BTN_CHAINFO, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt02.jpg", IMAGE_MENU_BTN_MYINVEN, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt03.jpg", IMAGE_MENU_BTN_FRIEND, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\partCharge1\\newui_menu_Bt04.jpg", IMAGE_MENU_BTN_WINDOW, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\newui_decor.tga", IMAGE_CUSTOM_ID, GL_LINEAR);
#elif CLIENT_VERSION <= 302
	//LOAD IMAGES OLD
#endif
}

void SEASON3B::CNewUIMainFrameWindow::UnloadImages()
{
#if  CLIENT_VERSION >= 1001
	DeleteBitmap(VOID_MAIN_IMAGE_FRAME_LEFT);
	DeleteBitmap(VOID_MAIN_IMAGE_FRAME_RIGHT);
	DeleteBitmap(VOID_MAIN_IMAGE_GAUGE_BLUE);
	DeleteBitmap(VOID_MAIN_IMAGE_GAUGE_GREEN);
	DeleteBitmap(VOID_MAIN_IMAGE_GAUGE_RED);
	DeleteBitmap(VOID_MAIN_IMAGE_GAUGE_AG);
	DeleteBitmap(VOID_MAIN_IMAGE_GAUGE_SD);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_AUTOPOTION);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_MENU);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_PARTY);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_INVENTORY);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_CHARACTER);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_LOCK);
	DeleteBitmap(VOID_MAIN_IMAGE_BTN_GUILD);
	DeleteBitmap(DIV_MAIN_UI);
	DeleteBitmap(DIV_MAIN_ORB_LIFE);
	DeleteBitmap(DIV_MAIN_ORB_MANA);
	DeleteBitmap(DIV_MAIN_ORB_POISON);
	DeleteBitmap(DIV_MAIN_ORB_LINE);
	DeleteBitmap(IMAGE_GAUGE_AG);
	DeleteBitmap(IMAGE_GAUGE_SD);
	DeleteBitmap(DIV_RENDER_NUMBER);
	DeleteBitmap(DIV_BACK_EXP_BAR);
	DeleteBitmap(DIV_BAR_POTION);
	DeleteBitmap(DIV_NOTIFICATION);
	DeleteBitmap(DIV_HOTKEY_SKILL_LIST_UP);
	DeleteBitmap(IMAGE_GAUGE_EXBAR);
	DeleteBitmap(IMAGE_MASTER_GAUGE_BAR);
	DeleteBitmap(IMAGE_MENU_BTN_CHAINFO);
	DeleteBitmap(IMAGE_MENU_BTN_MYINVEN);
	DeleteBitmap(IMAGE_MENU_BTN_WINDOW);

#elif CLIENT_VERSION >= 401
	DeleteBitmap(IMAGE_MENU_1);
	DeleteBitmap(IMAGE_MENU_2);
	DeleteBitmap(IMAGE_MENU_3);
	DeleteBitmap(IMAGE_MENU_2_1);
	DeleteBitmap(IMAGE_GAUGE_BLUE);
	DeleteBitmap(IMAGE_GAUGE_GREEN);
	DeleteBitmap(IMAGE_GAUGE_RED);
	DeleteBitmap(IMAGE_GAUGE_AG);
	DeleteBitmap(IMAGE_GAUGE_SD);
	DeleteBitmap(IMAGE_GAUGE_EXBAR);
	DeleteBitmap(IMAGE_MENU_BTN_CHAINFO);
	DeleteBitmap(IMAGE_MENU_BTN_MYINVEN);
	DeleteBitmap(IMAGE_MENU_BTN_FRIEND);
	DeleteBitmap(IMAGE_MENU_BTN_WINDOW);
#endif
}

bool SEASON3B::CNewUIMainFrameWindow::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if(NULL == pNewUIMng || NULL == pNewUI3DRenderMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_MAINFRAME, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;
	m_pNewUI3DRenderMng->Add3DRenderObj(this, ITEMHOTKEYNUMBER_CAMERA_Z_ORDER);

	LoadImages();

	SetButtonInfo();

	Show(true);
	
	return true;
}

void SEASON3B::CNewUIMainFrameWindow::SetButtonInfo()
{	
#if  CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	int x_Next = gWideScreen.GetCenterPosition(489);
	int y_Next = 480-51;
	int x_Add = 30;
	int y_Add = 41;
	m_BtnCShop.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnCShop.ChangeButtonImgState( true, IMAGE_MENU_BTN_CSHOP, true );
	m_BtnCShop.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Add;
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeToolTipText(GlobalText[2277], true);

	m_BtnChaInfo.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnChaInfo.ChangeButtonImgState( true, IMAGE_MENU_BTN_CHAINFO, true );
	m_BtnChaInfo.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	x_Next += x_Add;
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeToolTipText(GlobalText[362], true);

	m_BtnMyInven.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnMyInven.ChangeButtonImgState( true, IMAGE_MENU_BTN_MYINVEN, true );
	m_BtnMyInven.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	x_Next += x_Add;
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeToolTipText(GlobalText[363], true);

	m_BtnFriend.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnFriend.ChangeButtonImgState( true, IMAGE_MENU_BTN_FRIEND, true );
	m_BtnFriend.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	x_Next += x_Add;
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeToolTipText(GlobalText[1043], true);

	m_BtnWindow.ChangeTextBackColor(RGBA(255,255,255,0));
	m_BtnWindow.ChangeButtonImgState( true, IMAGE_MENU_BTN_WINDOW, true );
	m_BtnWindow.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);	
	m_BtnWindow.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnWindow.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnWindow.ChangeToolTipText(GlobalText[1744], true);
#elif CLIENT_VERSION <= 302
	int x_Next = gWideScreen.GetCenterPosition(489);
	int y_Next = 480 - 51;
	int x_Add = 30;
	int y_Add = 41;

	m_BtnCShop.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnCShop.ChangeButtonImgState(true, IMAGE_MENU_BTN_CSHOP, true);
	m_BtnCShop.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Add;
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnCShop.ChangeToolTipText(GlobalText[2277], true);

	m_BtnChaInfo.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnChaInfo.ChangeButtonImgState(true, IMAGE_MENU_BTN_CHAINFO, true);
	m_BtnChaInfo.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Add;
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnChaInfo.ChangeToolTipText(GlobalText[362], true);

	m_BtnMyInven.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnMyInven.ChangeButtonImgState(true, IMAGE_MENU_BTN_MYINVEN, true);
	m_BtnMyInven.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Add;
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnMyInven.ChangeToolTipText(GlobalText[363], true);

	m_BtnFriend.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnFriend.ChangeButtonImgState(true, IMAGE_MENU_BTN_FRIEND, true);
	m_BtnFriend.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	x_Next += x_Add;
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
	m_BtnFriend.ChangeToolTipText(GlobalText[1043], true);
#elif CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		float x_Next = GetVoidMainX() + 345.f;
		int y_Next = (int)(GetVoidMainY() + 31.f);
		float x_Add = 31.5f;
		int y_Add = 24;

		m_BtnParty.ChangeTextBackColor(RGBA(255, 255, 255, 0));
		m_BtnParty.ChangeButtonImgState(true, IMAGE_MENU_BTN_WINDOW, true);
		m_BtnParty.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
		m_BtnParty.SetSize(25, 24);
		m_BtnParty.ChangeToolTipText(GlobalText[361], true);
		x_Next += x_Add;

		m_BtnChaInfo.ChangeTextBackColor(RGBA(255, 255, 255, 0));
		m_BtnChaInfo.ChangeButtonImgState(true, IMAGE_MENU_BTN_CHAINFO, true, false, true);
		m_BtnChaInfo.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
		m_BtnChaInfo.SetSize(25, 24);
		m_BtnChaInfo.ChangeToolTipText(GlobalText[362], true);
		x_Next += x_Add;

		m_BtnMyInven.ChangeTextBackColor(RGBA(255, 255, 255, 0));
		m_BtnMyInven.ChangeButtonImgState(true, IMAGE_MENU_BTN_MYINVEN, true);
		m_BtnMyInven.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
		m_BtnMyInven.SetSize(25, 24);
		m_BtnMyInven.ChangeToolTipText(GlobalText[363], true);
		return;
	}
	float x_Next = GetDIVMenuX(0);
	int y_Next = (int)GetDIVMenuY();
	float x_Add = 32;
	int y_Add = 24;

	m_BtnParty.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnParty.ChangeButtonImgState(true, IMAGE_MENU_BTN_WINDOW, true);
	m_BtnParty.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	m_BtnParty.SetSize(25, 24);
	m_BtnParty.ChangeToolTipText(GlobalText[361], true);
	x_Next += x_Add;

	m_BtnChaInfo.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnChaInfo.ChangeButtonImgState(true, IMAGE_MENU_BTN_CHAINFO, true, false, true);
	m_BtnChaInfo.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	m_BtnChaInfo.SetSize(25, 24);
	m_BtnChaInfo.ChangeToolTipText(GlobalText[362], true);
	x_Next += x_Add;

	m_BtnMyInven.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnMyInven.ChangeButtonImgState(true, IMAGE_MENU_BTN_MYINVEN, true);
	m_BtnMyInven.ChangeButtonInfo(x_Next, y_Next, x_Add, y_Add);
	m_BtnMyInven.SetSize(25, 24);
	m_BtnMyInven.ChangeToolTipText(GlobalText[363], true);
	x_Next += x_Add;
#endif
}

void SEASON3B::CNewUIMainFrameWindow::Release()
{
	UnloadImages();

	if(m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->Remove3DRenderObj(this);
		m_pNewUI3DRenderMng = NULL;
	}

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

bool SEASON3B::CNewUIMainFrameWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();

	EnableAlphaTest();
	RenderLifeMana();
	RenderGuageSD();
	RenderGuageAG();
	RenderButtons();
#if CLIENT_VERSION >= 401 && CLIENT_VERSION < 502
	RenderExperience();
#elif CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == true)
	{
		RenderExperience();
	}
#endif
	g_pSkillList->RenderCurrentSkillAndHotSkillList();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	g_pNewUISystem->m_Lua.Generic_Call("MainProc", ">");
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);
	/*
	if (!g_pCharacterInfoWindow->IsVisible() && !g_pMyInventory->IsVisible()
		&& !g_pGuildInfoWindow->IsVisible() && !g_pWindowMgr->IsVisible()
		&& !g_pPartyInfoWindow->IsVisible() && !g_pMyQuestInfoWindow->IsVisible()
		&& !g_pCommandWindow->IsVisible())
	{
		DeleteBitmap(31604);
		unicode::t_char szText[256] = { 0, };
		unicode::_sprintf(szText, "Custom\\MiniMap\\Map%d.jpg", gMapManager.WorldActive + 1);
		LoadBitmap(szText, 31604, GL_LINEAR);


		int m_MiniMapPosx = gWideScreen.g_WideWindowWidth - 154;
		int m_MiniMapPosy = 280;

		int m_HeroPosInWorldx = (Hero->PositionX) / 1;
		int m_HeroPosInWorldy = (256 - (Hero->PositionY)) / 1;

		int m_MiniMapScaleOffsetx = max((m_HeroPosInWorldx - (64 * 1)), 0);
		int m_MiniMapScaleOffsety = min(max((m_HeroPosInWorldy - (64 * 1)), 0), 128);

		int m_HeroPosInMiniMapx = m_HeroPosInWorldx - m_MiniMapScaleOffsetx + m_MiniMapPosx + 20;
		int m_HeroPosInMiniMapy = m_HeroPosInWorldy - m_MiniMapScaleOffsety + m_MiniMapPosy;

		float m_fMiniMapTexU = (float)(m_MiniMapScaleOffsetx) / (256.f / (float)1);
		float m_fMiniMapTexV = (float)(m_MiniMapScaleOffsety) / (256.f / (float)1);



		EnableAlphaTest();
		glColor4f(1.f, 1.f, 1.f, 1.0f);
		g_pRenderText->SetFont(g_hFontBold);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
		g_pRenderText->SetBgColor(0, 0, 0, 0);

		RenderBitmap(31604, (float)(m_MiniMapPosx + 20), (float)(m_MiniMapPosy), 128.f, 128.f, m_fMiniMapTexU, m_fMiniMapTexV, 0.5f * 1, 0.5f * 1);

		RenderImage(31602, m_MiniMapPosx, m_MiniMapPosy - 21, 154, 162);
		//RenderImage(31603, m_TimeUIPos.x, m_TimeUIPos.y, 134, 37);

		glColor4f(1.f, 1.f, 1.f, 1.0f);


		glColor4f(1.f, 1.f, 0.f, 1.0f);
		RenderColor((float)(m_HeroPosInMiniMapx), (float)(m_HeroPosInMiniMapy), 3, 3);
	}
	*/
	



	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIMainFrameWindow::Render3D()
{
	m_ItemHotKey.RenderItems();
}

void SEASON3B::CNewUIMainFrameWindow::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	g_pMainFrame->RenderHotKeyItemCount();

}

bool SEASON3B::CNewUIMainFrameWindow::IsVisible() const
{
	return CNewUIObj::IsVisible();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFrame()
{

#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	float width, height;
	float x, y;
	width = 256.f; height = 51.f;
	x = gWideScreen.GetCenterPosition(0); y = 480.f - height;
	SEASON3B::RenderImage(IMAGE_MENU_1, x, y, width, height);
	RenderBitmap(IMAGE_CUSTOM_ID, x- 70.28048780487805, y, 70.28048780487805, height, 0.0, 0.0, 0.8785000443, 0.6409999728, 1, 1, 0.0);
	width = 128.f;
	x += 256;
	SEASON3B::RenderImage(IMAGE_MENU_2, x, y, width, height);
	width = 256.f;
	x += 128;
	SEASON3B::RenderImage(IMAGE_MENU_3, x, y, width, height);
	RenderBitmap(IMAGE_CUSTOM_ID, x + width, y, 70.28048780487805, height, 0.8785000443, 0.0, -0.8785000443, 0.6409999728, 1, 1, 0.0);

	if(g_pSkillList->IsSkillListUp() == true)
	{
		width = 160.f; height = 40.f;
		x = gWideScreen.GetCenterPosition(222);
		SEASON3B::RenderImage(IMAGE_MENU_2_1, x, y, width, height);
	}
#endif

#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		const float x = GetVoidMainX();
		const float y = GetVoidMainY();
		const float frameTopTrim = 2.f / 96.f;
		const float frameVHeight = 0.9899f - frameTopTrim;
		RenderBitmap(VOID_MAIN_IMAGE_FRAME_LEFT, x - 2.f, y, 325.f, 81.f, 0.f, frameTopTrim, 0.9999f, frameVHeight, true, true, 0.f);
		RenderBitmap(VOID_MAIN_IMAGE_FRAME_RIGHT, x + 318.f, y, 322.f, 81.f, 0.002f, frameTopTrim, 0.9975f, frameVHeight, true, true, 0.f);
		RenderVoidMainButtons();
		RenderVoidMainInfoText();
		return;
	}
	if (g_DIVInterfaceReady == true)
	{
	const float x = GetDIVMainX();
	const float y = GetDIVMainY();

	glAlphaFunc(GL_GREATER, 0.0f);
	RenderBitmap(DIV_MAIN_UI, x, y, 204.f, 82.f, 0.f, 0.f, 0.986f, 0.5f, true, true, 0.f);
	RenderBitmap(DIV_MAIN_UI, x + 204.f, y, 204.f, 82.f, 0.014f, 0.5f, 0.986f, 0.5f, true, true, 0.f);
	if (g_DIVPotionBarVisible == true)
	{
		RenderBitmap(DIV_BAR_POTION, x - 78.f, y + 44.f, 104.f, 30.f, 0.f, 0.f, 1.f, 0.5f, true, true, 0.f);
	}


	const bool potionToggleHover = SEASON3B::CheckMouseIn(x - 2.f, y + 44.f, 28.f, 30.f);
	if (potionToggleHover == true)
	{
		if (MouseLButtonPush == true && g_DIVPotionToggleMouseDown == false)
		{
			g_DIVPotionBarVisible = !g_DIVPotionBarVisible;
			g_DIVPotionToggleMouseDown = true;
			PlayBuffer(SOUND_CLICK01);
		}
		RenderBitmap(DIV_BAR_POTION, x - 78.f, y + 44.f, 104.f, 30.f, 0.f, 0.5f, 1.f, 0.5f, true, true, 0.f);
	}
	if (MouseLButtonPush == false)
	{
		g_DIVPotionToggleMouseDown = false;
	}

	if (g_Time.GetTimeCheck(5, 500))
	{
		m_bButtonBlink = !m_bButtonBlink;
	}

	const float newMenuShortcutX = x + 317.f;
	const float newMenuShortcutY = y + 11.f;
	const float centralShortcutX = x + 332.f;
	const float centralShortcutY = y - 5.f;

	RenderBitmap(DIV_NOTIFICATION, newMenuShortcutX, newMenuShortcutY, 17.f, 17.f, 0.5f, 0.f, 0.5f, 1.f, true, true, 0.f);
	RenderBitmap(DIV_NOTIFICATION, centralShortcutX, centralShortcutY, 17.f, 17.f, 0.f, 0.f, 0.5f, 1.f, true, true, 0.f);

	const EDIVShortcutTarget shortcutTarget = GetDIVShortcutTarget();
	if (shortcutTarget == DIV_SHORTCUT_MENU)
	{
		glColor4f(1.f, 0.9f, 0.55f, 1.f);
		RenderBitmap(DIV_NOTIFICATION, newMenuShortcutX - 3.f, newMenuShortcutY - 3.f, 23.f, 23.f, 0.5f, 0.f, 0.5f, 1.f, true, true, 0.f);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		RenderTipText((int)newMenuShortcutX - 4, (int)newMenuShortcutY - 18, "Menu");
	}
	else if (shortcutTarget == DIV_SHORTCUT_SECURITY)
	{
		glColor4f(1.f, 0.9f, 0.55f, 1.f);
		RenderBitmap(DIV_NOTIFICATION, centralShortcutX - 3.f, centralShortcutY - 3.f, 23.f, 23.f, 0.f, 0.f, 0.5f, 1.f, true, true, 0.f);
		glColor4f(1.f, 1.f, 1.f, 1.f);
		RenderTipText((int)centralShortcutX - 14, (int)centralShortcutY - 18, "Security");
	}
	}
#endif

	//RenderBitmap(600016, gWideScreen.GetAddPosition(345), 450, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
	//RenderBitmap(600018, gWideScreen.GetAddPosition(345) + 31.5, 450, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
	//RenderBitmap(600017, gWideScreen.GetAddPosition(345) + 31.5 + 31.5, 450, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
	//RenderBitmap(600020, 92 + addw, 458, 16, 16, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
}

void SEASON3B::CNewUIMainFrameWindow::RenderLifeMana()
{
	DWORD wLifeMax, wLife, wManaMax, wMana;


	if(gCharacterManager.IsMasterLevel( Hero->Class ) == true )
	{
		wLifeMax = Master_Level_Data.wMaxLife;
		wLife = min(max(0, CharacterAttribute->Life), wLifeMax);
		wManaMax = Master_Level_Data.wMaxMana;
		wMana = min(max(0, CharacterAttribute->Mana), wManaMax);
	}
	else
	{
		wLifeMax = CharacterAttribute->LifeMax;
		wLife = min(max(0, CharacterAttribute->Life), wLifeMax);
		wManaMax = CharacterAttribute->ManaMax;
		wMana = min(max(0, CharacterAttribute->Mana), wManaMax);
	}

	if(wLifeMax > 0)
	{
		if(wLife > 0 && (wLife / (float)wLifeMax) < 0.2f)
		{
			PlayBuffer(SOUND_HEART);
		}
	}

	float fLife = 0.f;
	float fMana = 0.f;

	if(wLifeMax > 0)
	{
		fLife = (wLifeMax - wLife) / (float)wLifeMax;
	}
	if(wManaMax > 0)	
	{
		fMana = (wManaMax - wMana) / (float)wManaMax;
	}

#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		float width = 52.f;
		float height = 51.f;
		float x = GetVoidMainX() + 140.f;
		float y = GetVoidOrbY();
		float fY = y + (fLife * height);
		float fH = height - (fLife * height);
		float fV = fLife;
		const int lifeTexture = g_isCharacterBuff((&Hero->Object), eDeBuff_Poison) ? VOID_MAIN_IMAGE_GAUGE_GREEN : VOID_MAIN_IMAGE_GAUGE_RED;
		RenderBitmap(lifeTexture, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.f - fV) * height / 64.f, true, true, 0.f);
		glColor3f(1.0f, 0.81f, 0.55f);
		SEASON3B::RenderOldNumber(x + 35.f, GetVoidNumberY(), (int)wLife, 0.8f);
		glColor3f(1.0f, 1.0f, 1.0f);

		char strTipText[256];
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			sprintf(strTipText, GlobalText[358], wLife, wLifeMax);
			RenderTipText((int)x, (int)gWideScreen.GetBottomPosition(418), strTipText);
		}

		x = GetVoidMainX() + 446.f;
		fY = y + (fMana * height);
		fH = height - (fMana * height);
		fV = fMana;
		RenderBitmap(VOID_MAIN_IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.f - fV) * height / 64.f, true, true, 0.f);
		glColor3f(1.0f, 0.81f, 0.55f);
		SEASON3B::RenderOldNumber(x + 30.f, GetVoidNumberY(), (int)wMana, 0.8f);
		glColor3f(1.0f, 1.0f, 1.0f);

		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			sprintf(strTipText, GlobalText[359], wMana, wManaMax);
			RenderTipText((int)x, (int)gWideScreen.GetBottomPosition(418), strTipText);
		}
		return;
	}

	if (g_DIVInterfaceReady == true)
	{
	const float mainX = GetDIVMainX();
	const float mainY = GetDIVMainY();
	float coordU = 0.f;
	float coordV = 0.f;
	GetDIVAnimUV(coordU, coordV);

	float filledHeight = (1.f - fLife) * 64.f;
	if (filledHeight > 0.f)
	{
		const int lifeTexture = g_isCharacterBuff((&Hero->Object), eDeBuff_Poison) ? DIV_MAIN_ORB_POISON : DIV_MAIN_ORB_LIFE;
		RenderBitmap(lifeTexture, mainX + 8.25f, mainY + 72.5f, 64.f, -filledHeight, coordU, coordV + 0.125f, 0.125f, -filledHeight / 512.f, true, true, 0.f);

		const float lineV = filledHeight / 62.5f;
		if (lineV >= 0.03f && lineV <= 0.98f)
		{
			EnableAlphaBlend();
			RenderDIVCircle(DIV_MAIN_ORB_LINE, mainX + 40.f, GetDIVOrbLineY(mainY), 64.f, 64.f, g_isCharacterBuff((&Hero->Object), eDeBuff_Poison) ? 0.5f : 0.f, lineV, 0.25f, 1.f);
			EnableAlphaTest(true);
		}
	}

	filledHeight = (1.f - fMana) * 64.f;
	if (filledHeight > 0.f)
	{
		RenderBitmap(DIV_MAIN_ORB_MANA, mainX + 336.f, mainY + 72.5f, 64.f, -filledHeight, coordU, coordV + 0.125f, 0.125f, -filledHeight / 512.f, true, true, 0.f);

		const float lineV = filledHeight / 62.5f;
		if (lineV >= 0.03f && lineV <= 0.98f)
		{
			EnableAlphaBlend();
			RenderDIVCircle(DIV_MAIN_ORB_LINE, mainX + 368.f, GetDIVOrbLineY(mainY), 64.f, 64.f, 0.25f, lineV, 0.25f, 1.f);
			EnableAlphaTest(true);
		}
	}

	DIVRenderNumber(mainX + 40.5f, mainY + 38.f, (int)wLife, 0.75f);
	DIVRenderNumber(mainX + 368.5f, mainY + 38.f, (int)wMana, 0.75f);

	char divTipText[256];
	const bool divSDHover = (SEASON3B::CheckMouseIn(mainX + 57.f, mainY + 62.f, 58.f, 11.f) == true);
	const bool divAGHover = (SEASON3B::CheckMouseIn(mainX + 294.f, mainY + 62.f, 58.f, 11.f) == true);
	if (divSDHover == false && SEASON3B::CheckMouseIn(mainX + 8.f, mainY + 9.f, 64.f, 64.f) == true)
	{
		sprintf(divTipText, GlobalText[358], wLife, wLifeMax);
		RenderTipText((int)mainX + 8, (int)gWideScreen.GetBottomPosition(418), divTipText);
	}
	const bool divShortcutHover = (GetDIVShortcutTarget() != DIV_SHORTCUT_NONE);
	if (divAGHover == false && divShortcutHover == false && SEASON3B::CheckMouseIn(mainX + 336.f, mainY + 9.f, 64.f, 64.f) == true)
	{
		sprintf(divTipText, GlobalText[359], wMana, wManaMax);
		RenderTipText((int)mainX + 318, (int)gWideScreen.GetBottomPosition(418), divTipText);
	}

	glAlphaFunc(GL_GREATER, 0.25f);
	return;
	}
#endif

	float width, height;
	float x, y;
	float fY, fH, fV;
	
	// life
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	width = 45.f; 
	x = gWideScreen.GetCenterPosition(158);
	height = 39.f; 
	y = 480.f - 48.f;
#elif CLIENT_VERSION >= 1001
	width = 59;
	x = gWideScreen.GetCenterPosition(142);
	height = 55;
	y = gWideScreen.GetBottomPosition(480.f - 49.5f);
#endif

	// 45, 45, 0, 0, 0.7906, 0.760600, 1, 1, 1.0
	
	fY = y + (fLife * height);
	fH = height - (fLife * height);
	fV = fLife;
	bool animated = true;

	if( g_isCharacterBuff((&Hero->Object), eDeBuff_Poison) )
	{
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
		RenderBitmap(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
#elif CLIENT_VERSION >= 1001
		if (animated)
		{
			int Cal = (int)(timeGetTime() * 0.75f) % 3150 / 50;
			float CoordU = (float)(Cal % 8) * 0.125f;
			float CoordV = (float)(Cal / 8) * 0.125f;
			float uWidth = 0.125f;
			float vHeight = 0.125f;

			RenderBitmap(IMAGE_GAUGE_GREEN_ANIMATED, x - 45.f, fY - 13.0f, width + 12.15f, fH + 15.8f, CoordU, CoordV + fV * 0.125f, uWidth, vHeight * (1.0f - fV));
}
		else
		{
			RenderBitmap(IMAGE_GAUGE_GREEN, x, fY, width, fH, 0.f, fV * height / 57.f, width / 57.f, (1.0f - fV) * height / 57.f);
		}
#endif
	}
	else
	{
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
		RenderBitmap(IMAGE_GAUGE_RED, x, fY, width, fH, 0.f, fV * height / 64.f, width / 64.f, (1.0f - fV) * height / 64.f);
#elif CLIENT_VERSION >= 1001
		if (animated)
		{
			//int Cal = (int)(timeGetTime() * 0.75f) % 3150 / 50;
			int Cal = (int)(timeGetTime() * 0.75f) % 600 / 40;
			float CoordU = (float)(Cal % 8) * 0.125f;
			float CoordV = (float)(Cal / 8) * 0.125f;
			float uWidth = 0.125f;
			float vHeight = 0.125f;

			RenderBitmap(IMAGE_GAUGE_RED_ANIMATED, x - 42.f, fY - 13.0f, width+12.15f, fH + 15.8f, CoordU, CoordV + fV * 0.125f, uWidth, vHeight * (1.0f - fV));
		}
		else
		{
			//RenderBitmap(IMAGE_GAUGE_RED, x, fY, width, fH, 0.f, fV * height / 57.f, width / 57.f, (1.0f - fV) * height / 57.f);
		}
#endif
	}
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	SEASON3B::RenderNumber(x+25, 480-18, wLife);
#endif

#if CLIENT_VERSION >= 1001
	glColor3f(1.0f, 0.81f, 0.55f);
	SEASON3B::RenderOldNumber(x + +5, gWideScreen.GetBottomPosition(480 - 12), wLife, 0.9f);
	glColor3f(1.0f, 1.0f, 1.0f);
#endif

	char strTipText[256];
	if(SEASON3B::CheckMouseIn(x-50, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[358], wLife, wLifeMax);
		RenderTipText((int)x-50, (int)gWideScreen.GetBottomPosition(418), strTipText);
	}

	// mana
	

#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	width = 45.f;
	x = gWideScreen.GetCenterPosition(256.f + 128.f + 53.f);
	height = 39.f;
	y = 480.f - 48.f;
#endif

#if CLIENT_VERSION >= 1001
	width = 59;
	x = gWideScreen.GetCenterPosition(453)+0.5f;
	height = 55;
	y = gWideScreen.GetBottomPosition(480.f - 49.5f);
#endif

	fY = y + (fMana * height);
	fH = height - (fMana * height);
	fV = fMana;
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	RenderBitmap(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV* height / 64.f, width / 64.f, (1.0f - fV)* height / 64.f);
#elif CLIENT_VERSION >= 1001
	if (animated)
	{
		int Cal = (int)(timeGetTime() * 0.75f) % 3150 / 50;
		float CoordU = (float)(Cal % 8) * 0.125f;
		float CoordV = (float)(Cal / 8) * 0.125f;
		float uWidth = 0.125f;
		float vHeight = 0.125f;

		//RenderBitmap(IMAGE_GAUGE_BLUE_ANIMATED, x - 5.35f, fY - 7.0f, width-0.1f, fH + 2.8f, CoordU, CoordV + fV * 0.125f, uWidth, vHeight * (1.0f - fV));
		RenderBitmap(IMAGE_GAUGE_BLUE_ANIMATED, x + 43.f, fY - 13.0f, width + 12.15f, fH + 15.8f, CoordU, CoordV + fV * 0.125f, uWidth, vHeight * (1.0f - fV));
		//RenderBitmap(IMAGE_GAUGE_RED_ANIMATED, x - 10.2f, fY - 7.0f, width - 0.15f, fH + 5.0f, CoordU, CoordV + fV * 0.125f, uWidth, vHeight * (1.0f - fV));
	}
	else
	{
		RenderBitmap(IMAGE_GAUGE_BLUE, x, fY, width, fH, 0.f, fV* height / 57.f, width / 57.f, (1.0f - fV)* height / 57.f);
	}
#endif
	

#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	SEASON3B::RenderNumber(x + 30, 480 - 18, wMana);
#endif

#if CLIENT_VERSION >= 1001
	glColor3f(1.0f, 0.81f, 0.55f);
	SEASON3B::RenderOldNumber(x + 75, gWideScreen.GetBottomPosition(480 - 12), wMana, 0.9f);
	glColor3f(1.0f, 1.0f, 1.0f);
#endif

	// mana
	if(SEASON3B::CheckMouseIn(x+50, y, width, height) == true)
	{
		sprintf(strTipText, GlobalText[359], wMana, wManaMax);
		RenderTipText((int)x+50, (int)gWideScreen.GetBottomPosition(415), strTipText);
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageAG()
{
	float x, y, width, height;
	float fY, fH, fV;

	DWORD dwMaxSkillMana,dwSkillMana;

	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
	{
		
		dwMaxSkillMana = max(1, Master_Level_Data.wMaxBP);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);
	}
	else
	{
		dwMaxSkillMana = max(1, CharacterAttribute->SkillManaMax);
		dwSkillMana = min(dwMaxSkillMana, CharacterAttribute->SkillMana);
	}

	float fSkillMana = 0.0f;

	if(dwMaxSkillMana > 0)
	{
		fSkillMana = (dwMaxSkillMana - dwSkillMana) / (float)dwMaxSkillMana;
	}

#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		float width = 16.f;
		float height = 39.f;
		float x = GetVoidMainX() + 505.f;
		float y = GetVoidVerticalGaugeY();
		float fY = y + (fSkillMana * height);
		float fH = height - (fSkillMana * height);
		float fV = fSkillMana;
		RenderBitmap(VOID_MAIN_IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV * height / 64.f, 0.501351f, (1.f - fV) * height / 64.f, true, true, 0.f);
		glColor3f(1.0f, 0.81f, 0.55f);
		SEASON3B::RenderOldNumber(x + 12.f, GetVoidNumberY(), (int)dwSkillMana, 0.8f);
		glColor3f(1.0f, 1.0f, 1.0f);

		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			char strTipText[256];
			sprintf(strTipText, GlobalText[214], dwSkillMana, dwMaxSkillMana);
			RenderTipText((int)x - 20, (int)gWideScreen.GetBottomPosition(418), strTipText);
		}
		return;
	}
#endif
#if CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == true)
	{
	const float mainX = GetDIVMainX();
	const float mainY = GetDIVMainY();
	float coordU = 0.f;
	float coordV = 0.f;
	GetDIVAnimUV(coordU, coordV);

	const float filledWidth = (1.f - fSkillMana) * 55.f;
	if (filledWidth > 0.f)
	{
		RenderBitmap(IMAGE_GAUGE_AG, mainX + 349.f, mainY + 65.f, -filledWidth, 7.f, coordU + 0.125f, coordV, -filledWidth / 440.f, 0.125f, true, true, 0.f);
	}

	DIVRenderNumber(mainX + 323.f, mainY + 65.f, (int)dwSkillMana, 0.75f);

	if(SEASON3B::CheckMouseIn(mainX + 294.f, mainY + 62.f, 58.f, 11.f) == true)
	{
		char strTipText[256];
		sprintf(strTipText, GlobalText[214], dwSkillMana, dwMaxSkillMana);
		RenderTipText((int)mainX + 270, (int)gWideScreen.GetBottomPosition(404), strTipText);
	}
	return;
	}
#endif

#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	width = 16.f, height = 39.f;
	x = gWideScreen.GetCenterPosition(256 + 128 + 36); y = 480.f - 49.f;
	fY = y + (fSkillMana * height);
	fH = height - (fSkillMana * height);
	fV = fSkillMana;

	RenderBitmap(IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV*height/64.f, width/16.f, (1.0f - fV)*height/64.f);
	SEASON3B::RenderNumber(x+10, 480-18, (int)dwSkillMana);
#elif CLIENT_VERSION >= 1001

	width = 16.f, height = 42.f;
	x = 570.8f; y = gWideScreen.GetBottomPosition(476.f - 42.f);
	x += gWideScreen.GetCenterPosition(0);
	fY = y + (fSkillMana * height);
	fH = height - (fSkillMana * height);
	fV = fSkillMana;

	RenderBitmap(IMAGE_GAUGE_AG, x, fY, width, fH, 0.f, fV * height / 64.f, 0.501351, (1.0f - fV) * height / 64.f, true, true, 0.0f);

	glColor3f(1.0f, 0.81f, 0.55f);
	SEASON3B::RenderOldNumber(x + 12, gWideScreen.GetBottomPosition(480 - 12), (int)dwSkillMana, 0.9f);
	glColor3f(1.0f, 1.0f, 1.0f);
#endif

	if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{	
		char strTipText[256];

        sprintf(strTipText, GlobalText[214], dwSkillMana, dwMaxSkillMana);
		RenderTipText((int)x-20, (int)gWideScreen.GetBottomPosition(418), strTipText);
    }
}

void SEASON3B::CNewUIMainFrameWindow::RenderGuageSD()
{
	float x, y, width, height;
	float fY, fH, fV;
	DWORD wMaxShield,wShield;

	//Master_Level_Data.wMaxShield
	if(gCharacterManager.IsMasterLevel(Hero->Class) == true)
	{
		wMaxShield = max (1, Master_Level_Data.wMaxShield);
		wShield = min (wMaxShield, CharacterAttribute->Shield);
	}
	else
	{
		wMaxShield = max (1, CharacterAttribute->ShieldMax);
		wShield = min (wMaxShield, CharacterAttribute->Shield);
	}
	
	float fShield = 0.0f;

	if(wMaxShield > 0)
	{
		fShield = (wMaxShield - wShield) / (float)wMaxShield;
	}
#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		float width = 16.f;
		float height = 39.f;
		float x = GetVoidMainX() + 119.f;
		float y = GetVoidVerticalGaugeY();
		float fY = y + (fShield * height);
		float fH = height - (fShield * height);
		float fV = fShield;
		RenderBitmap(VOID_MAIN_IMAGE_GAUGE_SD, x, fY, width, fH, 0.f, fV * height / 64.f, 0.501351f, (1.f - fV) * height / 64.f, true, true, 0.f);
		glColor3f(1.0f, 0.81f, 0.55f);
		SEASON3B::RenderOldNumber(x + 15.f, GetVoidNumberY(), (int)wShield, 0.8f);
		glColor3f(1.0f, 1.0f, 1.0f);

		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			char strTipText[256];
			sprintf(strTipText, GlobalText[2037], wShield, wMaxShield);
			RenderTipText((int)x - 20, (int)gWideScreen.GetBottomPosition(418), strTipText);
		}
		return;
	}
#endif
#if CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == true)
	{
	const float mainX = GetDIVMainX();
	const float mainY = GetDIVMainY();
	float coordU = 0.f;
	float coordV = 0.f;
	GetDIVAnimUV(coordU, coordV);

	const float filledWidth = (1.f - fShield) * 55.f;
	if (filledWidth > 0.f)
	{
		RenderBitmap(IMAGE_GAUGE_SD, mainX + 60.f, mainY + 65.f, filledWidth, 7.f, coordU, coordV, filledWidth / 440.f, 0.125f, true, true, 0.f);
	}

	DIVRenderNumber(mainX + 89.f, mainY + 65.f, (int)wShield, 0.75f);

	if(SEASON3B::CheckMouseIn(mainX + 57.f, mainY + 62.f, 58.f, 11.f) == true)
	{
		char strTipText[256];
		sprintf(strTipText, GlobalText[2037], wShield, wMaxShield);
		RenderTipText((int)mainX + 82, (int)gWideScreen.GetBottomPosition(404), strTipText);
	}
	return;
	}
#endif
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	width = 16.f, height = 39.f;
	x = gWideScreen.GetCenterPosition(204); y = 480.f - 49.f;
	fY = y + (fShield * height);
	fH = height - (fShield * height);
	fV = fShield;

	RenderBitmap(IMAGE_GAUGE_SD, x, fY, width, fH, 0.f, fV*height/64.f, width/16.f, (1.0f - fV)*height/64.f);
	SEASON3B::RenderNumber(x+15, 480-18, (int)wShield);
#elif CLIENT_VERSION >= 1001
	width = 16.f, height = 39.f;
	x = 116.5f; y = gWideScreen.GetBottomPosition(480.f - 42.f);
	
	x += gWideScreen.GetCenterPosition(0);
	
	fY = y + (fShield * height);
	fH = height - (fShield * height);
	fV = fShield;

	// ATIVAR OU DESATIVAR O SD

	if (wMaxShield > 0) {
		RenderBitmap(IMAGE_GAUGE_SD, x-31, fY-3, width, fH, 0.f, fV * height / 6.f, 0.501351, (1.0f - fV) * height / 70.f, 1, 1, 0.0f);
	
	}
	glColor3f(1.0f, 0.81f, 0.55f);
	SEASON3B::RenderOldNumber(x - 25, gWideScreen.GetBottomPosition(480 - 12), (int)wShield, 0.9f);
	glColor3f(1.0f, 1.0f, 1.0f);
#endif


	height = 39.f;
	y = gWideScreen.GetBottomPosition(480.f - 10.f - 39.f);
	if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{	
		char strTipText[256];

        sprintf(strTipText, GlobalText[2037], wShield, wMaxShield);
		RenderTipText((int)x-20, (int)gWideScreen.GetBottomPosition(418), strTipText);
    }
}

void SEASON3B::CNewUIMainFrameWindow::RenderExperience()
{
	
	__int64 wLevel;	
	__int64 dwNexExperience;
	__int64 dwExperience;
	float x, y, width, height;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION < 502
	if (gCharacterManager.IsMasterLevelExpCheck(CharacterAttribute->Class) == true)
	{
		wLevel = (__int64)Master_Level_Data.nMLevel;
		dwNexExperience = (__int64)Master_Level_Data.lNext_MasterLevel_Experince;
		dwExperience = (__int64)Master_Level_Data.lMasterLevel_Experince;
	}
	else
#endif
	{
		wLevel = CharacterAttribute->Level;
		dwNexExperience = CharacterAttribute->NextExperince;
		dwExperience = CharacterAttribute->Experience;
	}

#if CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == true)
	{
	const float mainX = GetDIVMainX();
	const float mainY = GetDIVMainY();
	__int64 priorExperience = 0;

	if (gCharacterManager.IsMasterLevelExpCheck(CharacterAttribute->Class) == true)
	{
		const __int64 totalLevel = wLevel + 400;
		const __int64 overLevel = totalLevel - 255;
		const __int64 masterBase = (((__int64)9 + totalLevel) * totalLevel * totalLevel * (__int64)10)
			+ (((__int64)9 + overLevel) * overLevel * overLevel * (__int64)1000);
		priorExperience = (masterBase - (__int64)3892250000) / (__int64)2;
	}
	else
	{
		const WORD priorLevel = (WORD)(wLevel - 1);
		if (priorLevel > 0)
		{
			priorExperience = (9 + priorLevel) * priorLevel * priorLevel * 10;
			if (priorLevel > 255)
			{
				const int overLevel = priorLevel - 255;
				priorExperience += (9 + overLevel) * overLevel * overLevel * 1000;
			}
		}
	}

	double needExperience = (double)dwNexExperience - (double)priorExperience;
	double currentExperience = (double)dwExperience - (double)priorExperience;
	if (currentExperience < 0.0)
	{
		currentExperience = 0.0;
	}

	double expBar = 0.0;
	if (currentExperience > 0.0 && needExperience > 0.0)
	{
		expBar = (currentExperience / needExperience) * 10.0;
	}

	if (expBar < 0.0)
	{
		expBar = 0.0;
	}
	if (expBar > 10.0)
	{
		expBar = 10.0;
	}

	const int section = (int)expBar;
	const int expTexture = gCharacterManager.IsMasterLevelExpCheck(CharacterAttribute->Class) == true ? IMAGE_MASTER_GAUGE_BAR : IMAGE_GAUGE_EXBAR;
	const float expBaseX = mainX + 117.5f;
	const float expBaseY = mainY + 26.f;
	const float expFillX = mainX + 119.f;
	const float expFillY = mainY + 27.f;
	const float expFillMaxWidth = 175.f;
	const float expFillHeight = 4.f;

	RenderBitmap(DIV_BACK_EXP_BAR, expBaseX, expBaseY, 175.f, 6.f, 0.f, 0.f, 1.f, 1.f, true, true, 0.f);
	const float expFillWidth = expFillMaxWidth * ((float)expBar / 10.f);
	if (expFillWidth > 0.f)
	{
		RenderBitmap(expTexture, expFillX, expFillY, expFillWidth, expFillHeight, 0.f, 0.f, expFillWidth / 228.f, 1.f, true, true, 0.f);
	}
	RenderBitmap(DIV_BACK_EXP_BAR, expBaseX, expBaseY, 175.f, 6.f, 0.f, 0.f, 1.f, 1.f, true, true, 0.f);
	DIVRenderNumber(mainX + 113.5f, mainY + 22.f, section, 0.75f);

	if(SEASON3B::CheckMouseIn(mainX + 137.f, mainY + 24.f, 156.f, 10.f) == true)
	{
		char strTipText[256];
		sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
		RenderTipText((int)mainX + 148, (int)gWideScreen.GetBottomPosition(404), strTipText);
	}
	return;
	}
#endif

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (gCharacterManager.IsMasterLevelExpCheck(CharacterAttribute->Class) == true)
	{
		x = gWideScreen.GetCenterPosition(0); y = 470; width = 6; height = 4;

		__int64 iTotalLevel = wLevel + 400;
		__int64 iTOverLevel = iTotalLevel - 255;
		__int64 iBaseExperience = 0;

		__int64 iData_Master =	// A
			(
				(
					(__int64)9 + (__int64)iTotalLevel
				)
				* (__int64)iTotalLevel
				* (__int64)iTotalLevel
				* (__int64)10
			)
			+
			(
				(
					(__int64)9 + (__int64)iTOverLevel
				)
				* (__int64)iTOverLevel
				* (__int64)iTOverLevel
				* (__int64)1000
			);
		iBaseExperience = (iData_Master - (__int64)3892250000) / (__int64)2;	// B

		double fNeedExp = (double)dwNexExperience - (double)iBaseExperience;
		double fExp = (double)dwExperience - (double)iBaseExperience;

		if(dwExperience < iBaseExperience)
		{
			fExp = 0.f;
		}

		double fExpBarNum = 0.f;
		if(fExp > 0.f && fNeedExp > 0)
		{
			fExpBarNum = ((double)fExp / (double)fNeedExp) * (double)10.f;
		}

		double fProgress = fExpBarNum - __int64(fExpBarNum);

		if(m_bExpEffect == true)
		{
			double fPreProgress = 0.f;
			double fPreExp = (double)m_loPreExp - (double)iBaseExperience;
			if(m_loPreExp < iBaseExperience)
			{
				x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fProgress * 629.f; height = 4.f;
				RenderBitmap(IMAGE_MASTER_GAUGE_BAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
				glColor4f(1.f, 1.f, 1.f, 0.6f);
				RenderColor(x, y, width, height);
				EndRenderColor();
			}
			else
			{
				int iPreExpBarNum = 0;
				int iExpBarNum = 0;
				if(fPreExp > 0.f && fNeedExp > 0.f)
				{
					fPreProgress = ((double)fPreExp / (double)fNeedExp) * (double)10.f;
					iPreExpBarNum = (int)fPreProgress;
					fPreProgress = (double)fPreProgress - __int64(fPreProgress);
				}
				iExpBarNum = (int)fExpBarNum;

				if(iExpBarNum > iPreExpBarNum)
				{
					x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fProgress * 629.f; height = 4.f;
					RenderBitmap(IMAGE_MASTER_GAUGE_BAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
					glColor4f(1.f, 1.f, 1.f, 0.6f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
				else
				{
					double fGapProgress = 0.f;
					fGapProgress = (double)fProgress - (double)fPreProgress;
					x = gWideScreen.GetCenterPosition(2); y = 473.f; width = (double)fPreProgress * (double)629.f; height = 4.f;
					RenderBitmap(IMAGE_MASTER_GAUGE_BAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);

					x += width; width = (double)fGapProgress * (double)629.f;
					RenderBitmap(IMAGE_MASTER_GAUGE_BAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
					glColor4f(1.f, 1.f, 1.f, 0.6f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
			}
		}
		else
		{
			x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fProgress * 629.f; height = 4.f;
			RenderBitmap(IMAGE_MASTER_GAUGE_BAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
		}

		int iExp = (int)fExpBarNum;
		x = gWideScreen.GetCenterPosition(635); y = 469.f;
		SEASON3B::RenderNumber(x, y, iExp);

		x = gWideScreen.GetCenterPosition(2); y = 473.f; width = 629.f; height = 4.f;
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{	
			char strTipText[256];
	
			sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
			RenderTipText(280, 418, strTipText);
		}
	}
	else
#endif
	{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION < 502
		x = gWideScreen.GetCenterPosition(0); y = 470; width = 6; height = 4;

		WORD wPriorLevel = wLevel - 1;
		DWORD dwPriorExperience = 0;

		if(wPriorLevel > 0)
		{
			dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;

			if(wPriorLevel > 255)
			{
				int iLevelOverN = wPriorLevel - 255;
				dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
			}
		}

		float fNeedExp = dwNexExperience - dwPriorExperience;
		float fExp = dwExperience - dwPriorExperience;

		if(dwExperience < dwPriorExperience)
		{
			fExp = 0.f;
		}

		float fExpBarNum = 0.f;
		if(fExp > 0.f && fNeedExp > 0)
		{
			fExpBarNum = (fExp / fNeedExp) * 10.f;
		}

		float fProgress = fExpBarNum;
		fProgress = fProgress - (int)fProgress;

		if(m_bExpEffect == true)
		{
			float fPreProgress = 0.f;
			fExp = m_dwPreExp - dwPriorExperience;
			if(m_dwPreExp < dwPriorExperience)
			{
				x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fProgress * 629.f; height = 4.f;
				RenderBitmap(IMAGE_GAUGE_EXBAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
				glColor4f(1.f, 1.f, 1.f, 0.4f);
				RenderColor(x, y, width, height);
				EndRenderColor();
			}
			else
			{
				int iPreExpBarNum = 0;
				int iExpBarNum = 0;
				if(fExp > 0.f && fNeedExp > 0.f)
				{
					fPreProgress = (fExp / fNeedExp) * 10.f;
					iPreExpBarNum = (int)fPreProgress;
					fPreProgress = fPreProgress - (int)fPreProgress;
				}

				iExpBarNum = (int)fExpBarNum;

				if(iExpBarNum > iPreExpBarNum)
				{
					x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fProgress * 629.f; height = 4.f;
					RenderBitmap(IMAGE_GAUGE_EXBAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
					glColor4f(1.f, 1.f, 1.f, 0.4f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
				else
				{
					float fGapProgress = 0.f;
					fGapProgress = fProgress - fPreProgress;
					x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fPreProgress * 629.f; height = 4.f;
					RenderBitmap(IMAGE_GAUGE_EXBAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
					x += width; width = fGapProgress * 629.f;
					RenderBitmap(IMAGE_GAUGE_EXBAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
					glColor4f(1.f, 1.f, 1.f, 0.4f);
					RenderColor(x, y, width, height);
					EndRenderColor();
				}
			}
		}
		else
		{
			x = gWideScreen.GetCenterPosition(2); y = 473.f; width = fProgress * 629.f; height = 4.f;
			RenderBitmap(IMAGE_GAUGE_EXBAR, x, y, width, height, 0.f, 0.f, 6.f/8.f, 4.f/4.f);
		}

		int iExp = (int)fExpBarNum;
		x = gWideScreen.GetCenterPosition(635); y = 469.f;
		SEASON3B::RenderNumber(x, y, iExp);

		x = gWideScreen.GetCenterPosition(2); y = 473.f; width = 629.f; height = 4.f;
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{	
			char strTipText[256];
	
			sprintf(strTipText, GlobalText[1748], dwExperience, dwNexExperience);
			RenderTipText(280, 418, strTipText);
		}
#elif CLIENT_VERSION == 1002

	WORD wPriorLevel = wLevel - 1;
	DWORD dwPriorExperience = 0;
	
	if (wPriorLevel > 0)
	{
		dwPriorExperience = (9 + wPriorLevel) * wPriorLevel * wPriorLevel * 10;
	
		if (wPriorLevel > 255)
		{
			int iLevelOverN = wPriorLevel - 255;
			dwPriorExperience += (9 + iLevelOverN) * iLevelOverN * iLevelOverN * 1000;
		}
	}
	
	float fNeedExp = floor(dwNexExperience - dwPriorExperience);
	int fExp = dwExperience - dwPriorExperience;
	
	if (dwExperience < dwPriorExperience)
	{
		fExp = 0.f;
	}

	float stepValue = fNeedExp / 10;

	int currentSection = floor(fExp / stepValue);

	double sectionExp = fExp % (int)stepValue;

	float sectionProgress = sectionExp / stepValue;

	width = sectionProgress * 200;


	x = gWideScreen.GetCenterPosition(220); y = gWideScreen.GetBottomPosition(437.f); height = 4.f;

	glColor4f(1.0f, 0.81f, 0.55f, 0.4f);
	RenderColor(x, y, width, height);
	EndRenderColor();

	glColor3f(1.0f, 0.81f, 0.55f);
	SEASON3B::RenderOldNumber(x + 206, y-2.5f, currentSection, 0.8f);
	glColor3f(1.0f, 1.0f, 1.0f);

#endif
	}
}

void SEASON3B::CNewUIMainFrameWindow::RenderHotKeyItemCount()
{
	m_ItemHotKey.RenderItemCount();
}

void SEASON3B::CNewUIMainFrameWindow::RenderButtons()
{


#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	m_BtnCShop.Render();
#endif //defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
	RenderCharInfoButton();
	m_BtnMyInven.Render();

	RenderFriendButton();
	m_BtnWindow.Render();
#endif
#if CLIENT_VERSION >= 1001
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY)) {
		m_BtnMyInven.ForceButtonDown(true);
	}
	else {
		m_BtnMyInven.ForceButtonDown(false);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)) {
		m_BtnChaInfo.ForceButtonDown(true);
	}
	else {
		m_BtnChaInfo.ForceButtonDown(false);
	}

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_PARTY)) {
		m_BtnParty.ForceButtonDown(true);
	}
	else {
		m_BtnParty.ForceButtonDown(false);
	}
	if (g_VoidMainInterfaceReady == true)
	{
		return;
	}

	if (g_DIVInterfaceReady == false)
	{
		 m_BtnMyInven.Render();
		m_BtnParty.Render();
		m_BtnChaInfo.Render();
	}
#endif


	
}

void SEASON3B::CNewUIMainFrameWindow::RenderCharInfoButton()
{
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	m_BtnChaInfo.Render();



	if (g_QuestMng.IsQuestIndexByEtcListEmpty())
		return;

	if (g_Time.GetTimeCheck(5, 500))
		m_bButtonBlink = !m_bButtonBlink;

	if (m_bButtonBlink)
	{
		if (!(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_QUEST_PROGRESS_ETC)
			|| g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)))
			RenderImage(IMAGE_MENU_BTN_CHAINFO, gWideScreen.GetCenterPosition(489)+30, 480-51, 30, 41, 0.0f, 41.f);
	}
#endif

#if CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == false)
	{
	RenderBitmap(IMAGE_MENU_BTN_CHAINFO, gWideScreen.GetCenterPosition(347) + 32, 450, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
	}

	//if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER)) {
	//
	//RenderBitmap(IMAGE_MENU_BTN_CHAINFO, gWideScreen.GetCenterPosition(345) + 31.5, 450, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
	//}
#endif
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButton()
{
	m_BtnFriend.Render();

	int iBlinkTemp = g_pFriendMenu->GetBlinkTemp();
	BOOL bIsAlertTime = (iBlinkTemp % 24 < 12);

	if (g_pFriendMenu->IsNewChatAlert() && bIsAlertTime)
	{
		RenderFriendButtonState();	
	}
	if (g_pFriendMenu->IsNewMailAlert())
	{
		if (bIsAlertTime)
		{
			RenderFriendButtonState();

			if (iBlinkTemp % 24 == 11) 
			{
				g_pFriendMenu->IncreaseLetterBlink();
			}
		}
	}
	else if (g_pLetterList->CheckNoReadLetter())
	{
		RenderFriendButtonState();
	}

	g_pFriendMenu->IncreaseBlinkTemp();
}

void SEASON3B::CNewUIMainFrameWindow::RenderFriendButtonState()
{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, gWideScreen.GetCenterPosition(489) +(30*3), 480-51, 30, 41, 0.0f, 123.f);
	}
	else
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, gWideScreen.GetCenterPosition(489) +(30*3), 480-51, 30, 41, 0.0f, 41.f);
	}
#else //defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_FRIEND) == true)
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, gWideScreen.GetCenterPosition(488) +76, 480-51, 38, 42, 0.0f, 126.f);
	}
	else
	{
		RenderImage(IMAGE_MENU_BTN_FRIEND, gWideScreen.GetCenterPosition(488) +76, 480-51, 38, 42, 0.0f, 42.f);
	}
#endif//defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateMouseEvent()
{
	if( g_pNewUIHotKey->IsStateGameOver() == true )
	{
		return true;
	}

	if(BtnProcess() == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::BtnProcess()
{
#if CLIENT_VERSION >= 1001
	if (ProcessVoidMainButtonClick() == true)
	{
		return true;
	}

	if (g_DIVInterfaceReady == true)
	{
		const EDIVShortcutTarget shortcutTarget = GetDIVShortcutTarget();
		if (shortcutTarget != DIV_SHORTCUT_NONE)
		{
			if ((SEASON3B::IsPress(VK_LBUTTON) == true || MouseLButtonPush == true) && g_DIVNotificationMouseDown == false)
			{
				if (shortcutTarget == DIV_SHORTCUT_MENU)
				{
					g_pNewUISystem->m_Lua.Generic_Call("OpenDIVNewMenuShortcut", ">");
				}
				else if (shortcutTarget == DIV_SHORTCUT_SECURITY)
				{
					g_pNewUISystem->m_Lua.Generic_Call("OpenDIVCentralShortcut", ">");
				}

				g_DIVNotificationMouseDown = true;
				MouseLButtonPush = false;
				PlayBuffer(SOUND_CLICK01);
				return true;
			}

			if (SEASON3B::IsRepeat(VK_LBUTTON) == true)
			{
				return true;
			}
		}

		if (SEASON3B::IsNone(VK_LBUTTON) == true || SEASON3B::IsRelease(VK_LBUTTON) == true)
		{
			g_DIVNotificationMouseDown = false;
		}
	}
#endif

	if(g_pNewUIHotKey->CanUpdateKeyEventRelatedMyInventory() == true)
	{
		if(m_BtnMyInven.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
	}
	else if(g_pNewUIHotKey->CanUpdateKeyEvent() == true)
	{
		if(m_BtnMyInven.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
		else if(m_BtnChaInfo.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);
			PlayBuffer(SOUND_CLICK01);

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
				g_QuestMng.SendQuestIndexByEtcSelection();

			return true;
		}
		else if(m_BtnFriend.UpdateMouseEvent() == true)
		{
			if( gMapManager.InChaosCastle() == true )
			{
				PlayBuffer(SOUND_CLICK01);
				return true;
			}

			int iLevel = CharacterAttribute->Level;
			
			if(iLevel < 6)
			{
				if(g_pChatListBox->CheckChatRedundancy(GlobalText[1067]) == FALSE)
				{
					g_pChatListBox->AddText("",GlobalText[1067],SEASON3B::TYPE_SYSTEM_MESSAGE);
				}
			}
			else
			{
				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_FRIEND);
			}
			PlayBuffer(SOUND_CLICK01);
			return true;
		}
#if CLIENT_VERSION >= 1001
		else if (m_BtnParty.UpdateMouseEvent() == true)
		{
			g_pNewUISystem->Toggle(SEASON3B::INTERFACE_PARTY);
			PlayBuffer(SOUND_CLICK01);

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHARACTER))
				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_CHARACTER);

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INVENTORY))
				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_INVENTORY);

			if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_GUILDINFO))
				g_pNewUISystem->Toggle(SEASON3B::INTERFACE_GUILDINFO);

			return true;
		}
#endif
		//else if(m_BtnWindow.UpdateMouseEvent() == true)
		//{
		//	g_pNewUISystem->Toggle(SEASON3B::INTERFACE_WINDOW_MENU);
		//	PlayBuffer(SOUND_CLICK01);
		//	return true;
		//}
		/*
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
		else if(m_BtnCShop.UpdateMouseEvent() == true)
		{
			if(g_pInGameShop->IsInGameShopOpen() == false)
				return false;

#ifdef KJH_MOD_SHOP_SCRIPT_DOWNLOAD
			if( g_InGameShopSystem->IsScriptDownload() == true )
			{
				if( g_InGameShopSystem->ScriptDownload() == false )
					return false;
			}
			
			if( g_InGameShopSystem->IsBannerDownload() == true )
			{
				g_InGameShopSystem->BannerDownload();
			}
#endif // KJH_MOD_SHOP_SCRIPT_DOWNLOAD
			
			if( g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_INGAMESHOP) == false)
			{
				if( g_InGameShopSystem->GetIsRequestShopOpenning() == false )		
				{
					SendRequestIGS_CashShopOpen(0);
					g_InGameShopSystem->SetIsRequestShopOpenning(true);

#ifdef KJH_MOD_SHOP_SCRIPT_DOWNLOAD
					g_pMainFrame->SetBtnState(MAINFRAME_BTN_PARTCHARGE, true);
#endif // KJH_MOD_SHOP_SCRIPT_DOWNLOAD

				}
			}
			else
			{
				SendRequestIGS_CashShopOpen(1);
				g_pNewUISystem->Hide(SEASON3B::INTERFACE_INGAMESHOP);
			}		

			return true;
		}
#endif //PBG_ADD_INGAMESHOP_UI_MAINFRAME
		*/
	}

	return false;
}

bool SEASON3B::CNewUIMainFrameWindow::UpdateKeyEvent()
{
	if(m_ItemHotKey.UpdateKeyEvent() == false)
	{
		return false;
	}
	return true;
}

bool SEASON3B::CNewUIMainFrameWindow::Update()
{
	if(m_bExpEffect == true)
	{
		if(timeGetTime() - m_dwExpEffectTime > 2000)
		{
			m_bExpEffect = false;
			m_dwExpEffectTime = 0;
			m_dwGetExp = 0;
		}
	}
	
	return true;
}

float SEASON3B::CNewUIMainFrameWindow::GetLayerDepth()
{
	return 10.6f;
}

float SEASON3B::CNewUIMainFrameWindow::GetKeyEventOrder()
{
	return 2.9f;
}

void SEASON3B::CNewUIMainFrameWindow::SetItemHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	m_ItemHotKey.SetHotKey(iHotKey, iItemType, iItemLevel);	
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKey(int iHotKey)
{
	return m_ItemHotKey.GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetItemHotKeyLevel(int iHotKey)
{
	return m_ItemHotKey.GetHotKeyLevel(iHotKey);
}

void SEASON3B::CNewUIMainFrameWindow::UseHotKeyItemRButton()
{
	m_ItemHotKey.UseItemRButton();
}

void SEASON3B::CNewUIMainFrameWindow::UpdateItemHotKey()
{
	m_ItemHotKey.UpdateKeyEvent();
}

void SEASON3B::CNewUIMainFrameWindow::ResetSkillHotKey()
{
	g_pSkillList->Reset();
}

void SEASON3B::CNewUIMainFrameWindow::SetSkillHotKey(int iHotKey, int iSkillType)
{
	g_pSkillList->SetHotKey(iHotKey, iSkillType);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKey(int iHotKey)
{
	return g_pSkillList->GetHotKey(iHotKey);
}

int SEASON3B::CNewUIMainFrameWindow::GetSkillHotKeyIndex(int iSkillType)
{
	return g_pSkillList->GetSkillIndex(iSkillType);
}

SEASON3B::CNewUIItemHotKey::CNewUIItemHotKey()
{
	for(int i=0; i<HOTKEY_COUNT; ++i)
	{
		m_iHotKeyItemType[i] = -1;
		m_iHotKeyItemLevel[i] = 0;
	}
}

SEASON3B::CNewUIItemHotKey::~CNewUIItemHotKey()
{

}

bool SEASON3B::CNewUIItemHotKey::UpdateKeyEvent()
{
	int iIndex = -1;
	static bool bAutoHoldQ = false;
	static bool bAutoHoldW = false;
	static bool bAutoHoldE = false;
	
	// Verifica se houve um novo pressionamento (sem repeti��o)
	//if (SEASON3B::IsPress('Q')) 
	//{
	//    bAutoHoldQ = !bAutoHoldQ; // Alterna entre ligado/desligado
	//}
	//if (SEASON3B::IsPress('W')) 
	//{
	//    bAutoHoldW = !bAutoHoldW;
	//}
	//if (SEASON3B::IsPress('E')) 
	//{
	//    bAutoHoldE = !bAutoHoldE;
	//}
	//
	// Define iIndex com base no estado atual
	if (SEASON3B::IsPress('Q'))
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_Q);
	}
	else if (SEASON3B::IsPress('W'))
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_W);
	}
	else if (SEASON3B::IsPress('E'))
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_E);
	}
#if (CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502) || CLIENT_VERSION >= 1001
	else if(SEASON3B::IsPress('R') == true)
	{
		iIndex = GetHotKeyItemIndex(HOTKEY_R);
	}
#endif
	else
	{
		iIndex = -1;
	}
	
	if(iIndex != -1)
	{
		ITEM* pItem = NULL;
		pItem = g_pMyInventory->FindItem(iIndex);
		if( ( pItem->Type>=ITEM_POTION+78 && pItem->Type<=ITEM_POTION+82 ) )
		{
			std::list<eBuffState> secretPotionbufflist;
			secretPotionbufflist.push_back( eBuff_SecretPotion1 );
			secretPotionbufflist.push_back( eBuff_SecretPotion2 );
			secretPotionbufflist.push_back( eBuff_SecretPotion3 );
			secretPotionbufflist.push_back( eBuff_SecretPotion4 );
			secretPotionbufflist.push_back( eBuff_SecretPotion5 );
			
			if( g_isCharacterBufflist( (&Hero->Object), secretPotionbufflist ) != eBuffNone ) {
				SEASON3B::CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255) );	
			}
			else {
				SendRequestUse(iIndex, 0);
			}
		}
		else
		
		{
			SendRequestUse(iIndex, 0);
		}
		return false;
	}

	return true;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemIndex(int iType, bool bItemCount)
{
	int iStartItemType = 0, iEndItemType = 0;
	int i, j;

	switch(iType)
	{
	case HOTKEY_Q:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if(m_iHotKeyItemType[iType] >= ITEM_POTION+4 && m_iHotKeyItemType[iType] <= ITEM_POTION+6)
            {
                iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
            }
			else
			{
				iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
			}
		}
		break;
	case HOTKEY_W:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if(m_iHotKeyItemType[iType] >= ITEM_POTION+0 && m_iHotKeyItemType[iType] <= ITEM_POTION+3)
            {
                iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
            }
			else
			{
				iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
			}	
		}
		break;
	case HOTKEY_E:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if(m_iHotKeyItemType[iType] >= ITEM_POTION+0 && m_iHotKeyItemType[iType] <= ITEM_POTION+3)
            {
                iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
            }
			else if(m_iHotKeyItemType[iType] >= ITEM_POTION+4 && m_iHotKeyItemType[iType] <= ITEM_POTION+6)
            {
                iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
            }
			else
			{
				iStartItemType = ITEM_POTION+8; iEndItemType = ITEM_POTION+8;
			}
		}
		break;
#if (CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502) || CLIENT_VERSION >= 1001
	case HOTKEY_R:
		if(GetHotKeyCommonItem(iType, iStartItemType, iEndItemType) == false)
		{
			if(m_iHotKeyItemType[iType] >= ITEM_POTION+0 && m_iHotKeyItemType[iType] <= ITEM_POTION+3)
            {
                iStartItemType = ITEM_POTION+3; iEndItemType = ITEM_POTION+0;
            }
			else if(m_iHotKeyItemType[iType] >= ITEM_POTION+4 && m_iHotKeyItemType[iType] <= ITEM_POTION+6)
            {
                iStartItemType = ITEM_POTION+6; iEndItemType = ITEM_POTION+4;
            }
			else
			{
				iStartItemType = ITEM_POTION+37; iEndItemType = ITEM_POTION+35;	
			}
		}
		break;	
#endif
	}

	int iItemCount = 0;
	ITEM* pItem = NULL;

	int iNumberofItems = g_pMyInventory->GetInventoryCtrl()->GetNumberOfItems();
	for(i=iStartItemType; i>=iEndItemType; --i)
	{
		if(bItemCount)
		{
			for(j=0; j<iNumberofItems; ++j)
			{
				pItem = g_pMyInventory->GetInventoryCtrl()->GetItem(j);
				if(pItem == NULL)
				{
					continue;
				}

				if( 
					(pItem->Type == i && ((pItem->Level>>3)&15) == m_iHotKeyItemLevel[iType])
				|| (pItem->Type == i && (pItem->Type >= ITEM_POTION+0 && pItem->Type <= ITEM_POTION+3)) 
				)
				{
					if(pItem->Type == ITEM_POTION+9
						|| pItem->Type == ITEM_POTION+10
						|| pItem->Type == ITEM_POTION+20
						)
					{
						iItemCount++;
					}
					else
					{
						iItemCount += pItem->Durability;
					}
				}
			}
		}
		else
		{
			int iIndex = -1;
			if(i >= ITEM_POTION+0 && i <= ITEM_POTION+3)	
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i);
			}
			else
			{
				iIndex = g_pMyInventory->FindItemReverseIndex(i, m_iHotKeyItemLevel[iType]);
			}
			
			if (-1 != iIndex)
			{
				pItem = g_pMyInventory->FindItem(iIndex);
				if((pItem->Type != ITEM_POTION+7
					&& pItem->Type != ITEM_POTION+10
					&& pItem->Type != ITEM_POTION+20)
					|| ((pItem->Level>>3)&15) == m_iHotKeyItemLevel[iType]
					)
				{
					return iIndex;
				}
			}
		}
	}

	if(bItemCount == true)
	{
		return iItemCount;
	}

	return -1;
}

bool SEASON3B::CNewUIItemHotKey::GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd)
{
	switch(m_iHotKeyItemType[iHotKey])
	{
	case ITEM_POTION+7:
	case ITEM_POTION+8:
	case ITEM_POTION+9:
	case ITEM_POTION+10:
	case ITEM_POTION+20:
	case ITEM_POTION+46:
	case ITEM_POTION+47:
	case ITEM_POTION+48:
	case ITEM_POTION+49:
	case ITEM_POTION+50:
	case ITEM_POTION+70:
	case ITEM_POTION+71:
	case ITEM_POTION+78:
	case ITEM_POTION+79:
	case ITEM_POTION+80:
	case ITEM_POTION+81:
	case ITEM_POTION+82:
	case ITEM_POTION+94:
	case ITEM_POTION+85:
	case ITEM_POTION+86:
	case ITEM_POTION+87:
	case ITEM_POTION+133:
		if(m_iHotKeyItemType[iHotKey] != ITEM_POTION+20 || m_iHotKeyItemLevel[iHotKey] == 0)
		{
			iStart = iEnd = m_iHotKeyItemType[iHotKey];
			return true;
		}
		break;
	default:
		if(m_iHotKeyItemType[iHotKey] >= ITEM_POTION+35 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION+37)
		{
			iStart = ITEM_POTION+37; iEnd = ITEM_POTION+35;
			return true;
		}
		else if(m_iHotKeyItemType[iHotKey] >= ITEM_POTION+38 && m_iHotKeyItemType[iHotKey] <= ITEM_POTION+40)
		{
			iStart = ITEM_POTION+40; iEnd = ITEM_POTION+38;
			return true;
		}
		break;
	}
	return false;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyItemCount(int iType)
{
	return 0;
}

void SEASON3B::CNewUIItemHotKey::SetHotKey(int iHotKey, int iItemType, int iItemLevel)
{
	if(iHotKey != -1 && CNewUIMyInventory::CanRegisterItemHotKey(iItemType) == true
		)
	{
		m_iHotKeyItemType[iHotKey] = iItemType;
		m_iHotKeyItemLevel[iHotKey] = iItemLevel;
	}
	else
	{
		m_iHotKeyItemType[iHotKey] = -1;
		m_iHotKeyItemLevel[iHotKey] = 0;
	}
}

int SEASON3B::CNewUIItemHotKey::GetHotKey(int iHotKey)
{
	if(iHotKey != -1)
	{
		return m_iHotKeyItemType[iHotKey];
	}

	return -1;
}

int SEASON3B::CNewUIItemHotKey::GetHotKeyLevel(int iHotKey)
{
	if(iHotKey != -1)
	{
		return m_iHotKeyItemLevel[iHotKey];
	}
	
	return 0;
}

void SEASON3B::CNewUIItemHotKey::RenderItems()
{
#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		for (int i = 0; i < HOTKEY_COUNT && i < 3; ++i)
		{
			int iIndex = GetHotKeyItemIndex(i);
			if (iIndex == -1)
			{
				continue;
			}

			ITEM* pItem = g_pMyInventory->FindItem(iIndex);
			if (pItem == NULL)
			{
				continue;
			}

			float x = GetVoidPotionItemX(i);
			float y = GetVoidPotionItemY();
			RenderItem3D(x, y, 20.f, 20.f, pItem->Type, pItem->Level, 0, 0);
		}
		return;
	}
	if (g_DIVInterfaceReady == true && g_DIVPotionBarVisible == false)
	{
		return;
	}
#endif
	float x, y, width, height;


	for (int i = 0; i < HOTKEY_COUNT; ++i)
	{
		int iIndex = GetHotKeyItemIndex(i);
		if (iIndex != -1)
		{
			ITEM* pItem = g_pMyInventory->FindItem(iIndex);
			if (pItem)
			{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
				x = gWideScreen.GetCenterPosition(10) + (i * 38); y = gWideScreen.GetBottomPosition(443); width = 20; height = 20;
#elif CLIENT_VERSION >= 1001	
				if (g_DIVInterfaceReady == true)
				{
					x = GetDIVPotionSlotX(i); y = GetDIVPotionItemY(); width = 20; height = 20;
				}
				else
				{
					x = gWideScreen.GetCenterPosition(10) + (i * 38); y = gWideScreen.GetBottomPosition(443); width = 20; height = 20;
				}
#endif
				RenderItem3D(x, y, width, height, pItem->Type, pItem->Level, 0, 0);
			}
		}
	}
}

void SEASON3B::CNewUIItemHotKey::RenderItemCount()
{
#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		for (int i = 0; i < HOTKEY_COUNT && i < 3; ++i)
		{
			int iCount = GetHotKeyItemIndex(i, true);
			if (iCount <= 0)
			{
				continue;
			}

			glColor4f(1.0f, 0.81f, 0.55f, 1.f);
			SEASON3B::RenderOldNumber(GetVoidPotionCountX(i), GetVoidPotionCountY(), iCount, 0.8f);
		}
		return;
	}
	if (g_DIVInterfaceReady == true && g_DIVPotionBarVisible == false)
	{
		return;
	}
#endif
	float x, y, width, height;


	for(int i=0; i<HOTKEY_COUNT; ++i)
	{
		int iCount = GetHotKeyItemIndex(i, true);
		if(iCount > 0)
		{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
			glColor4f(1.f, 1.f, 1.f, 1.f);
			x = gWideScreen.GetCenterPosition(30) + (i*38); y = 457; width = 8; height = 9;
			SEASON3B::RenderNumber(x, y, iCount);
#elif CLIENT_VERSION >= 1001
			glColor4f(1.0f, 0.81f, 0.55f, 1.f);
			if (g_DIVInterfaceReady == true)
			{
				x = GetDIVPotionCountX(i); y = GetDIVPotionCountY(); width = 8; height = 9;
				DIVRenderNumber(x, y, iCount, 0.75f);
			}
			else
			{
				x = gWideScreen.GetCenterPosition(30) + (i * 38); y = gWideScreen.GetBottomPosition(23.f); width = 8; height = 9;
				SEASON3B::RenderOldNumber(x, y, iCount, 0.8f);
			}
#endif
		}
	}
}

void SEASON3B::CNewUIItemHotKey::UseItemRButton()
{
#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		for (int i = 0; i < HOTKEY_COUNT && i < 3; ++i)
		{
			int x = (int)GetVoidPotionItemX(i);
			int y = (int)GetVoidPotionItemY();
			int width = 20;
			int height = 20;
			if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
			{
				if(MouseRButtonPush)
				{
					MouseRButtonPush = false;
					int iIndex = GetHotKeyItemIndex(i);
					if(iIndex != -1)
					{
						SendRequestUse(iIndex, 0);
						break;
					}
				}
			}
		}
		return;
	}

	if (g_DIVInterfaceReady == true && g_DIVPotionBarVisible == false)
	{
		return;
	}
#endif
	int x, y, width, height;

	for(int i=0; i<HOTKEY_COUNT; ++i)
	{
		#if CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == true)
		{
			x = (int)GetDIVPotionSlotX(i); y = (int)(GetDIVMainY() + 44.f); width = 20; height = 30;
		}
		else
		{
			x = gWideScreen.GetCenterPosition(10) + (i * 38); y = gWideScreen.GetBottomPosition(445); width = 20; height = 20;
		}
#else
		x = gWideScreen.GetCenterPosition(10) + (i * 38); y = gWideScreen.GetBottomPosition(445); width = 20; height = 20;
#endif
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if(MouseRButtonPush)
			{
				MouseRButtonPush = false;
				int iIndex = GetHotKeyItemIndex(i);
				if(iIndex != -1)
				{
					SendRequestUse(iIndex, 0);
					break;
				}
			}
		}
	}	
}

SEASON3B::CNewUISkillList::CNewUISkillList()
{
	m_pNewUIMng = NULL;
	Reset();
}

SEASON3B::CNewUISkillList::~CNewUISkillList()
{
	Release();
}

bool SEASON3B::CNewUISkillList::Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng)
{
	if(NULL == pNewUIMng)
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_SKILL_LIST, this);

	m_pNewUI3DRenderMng = pNewUI3DRenderMng;

	LoadImages();

#if CLIENT_VERSION >= 1001
	SetButtonInfo();
#endif

	Show(true);

	return true;
}

#if CLIENT_VERSION >= 1001
void SEASON3B::CNewUISkillList::SetButtonInfo()
{
	m_SkillListButtonUp.ChangeButtonImgState(true, IMAGE_SKILL_ARROW_UP, true);
	m_SkillListButtonUp.ChangeButtonInfo(gWideScreen.GetCenterPosition(320) + 182.f, gWideScreen.GetBottomPosition(363.5f), 14, 14);
	m_SkillListButtonUp.ForceButtonDown(true);
	m_SkillListButtonUp.Lock();

	m_SkillListButtonDown.ChangeButtonImgState(true, IMAGE_SKILL_ARROW_DOWN, true);
	m_SkillListButtonDown.ChangeButtonInfo(gWideScreen.GetCenterPosition(320) + 182.f, gWideScreen.GetBottomPosition(381.5f), 14, 14);
	m_SkillListButtonDown.ForceButtonDown(true);
	m_SkillListButtonDown.Lock();

}
#endif

void SEASON3B::CNewUISkillList::Release()
{
	if(m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);
	}
	
	UnloadImages();

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUISkillList::Reset()
{
	m_bSkillList = false;
	m_bHotKeySkillListUp = false;

#if CLIENT_VERSION >= 1001
	m_SkillPage = 0;
	m_SkillShopBuffPage = 0;
	CenterSkillShopWindow(m_SkillShopPos);
	m_SkillShopDragOffset.x = 0;
	m_SkillShopDragOffset.y = 0;
	m_SkillShopDragging = false;
	m_SkillShopConfigLoaded = false;
	m_SkillShopLastConfigRequestTick = 0;
	m_SkillShopLastBuyRequestTick = 0;
	m_SkillShopBuffCount = 0;
	memset(m_SkillShopBuffs, 0, sizeof(m_SkillShopBuffs));
	m_SkillListButtonUp.ForceButtonDown(true);
	m_SkillListButtonUp.Lock();

	m_SkillListButtonDown.ForceButtonDown(true);
	m_SkillListButtonDown.Lock();
#endif
	m_bRenderSkillInfo = false;
	m_iRenderSkillInfoType = 0;
	m_iRenderSkillInfoPosX = 0;
	m_iRenderSkillInfoPosY = 0;

	for(int i=0; i<SKILLHOTKEY_COUNT; ++i)
	{
		m_iHotKeySkillType[i] = -1;
	}

	m_EventState = EVENT_NONE;
}

void SEASON3B::CNewUISkillList::LoadImages()
{
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_skill.jpg", IMAGE_SKILL1, GL_LINEAR);
	LoadBitmap("Interface\\newui_skill2.jpg", IMAGE_SKILL2, GL_LINEAR);
	LoadBitmap("Interface\\newui_command.jpg", IMAGE_COMMAND, GL_LINEAR);
	LoadBitmap((g_VoidMainInterfaceReady || AreVoidMainAssetsAvailable()) ? "Custom\\Interface\\newui_skillbox.jpg" : "Interface\\newui_skillbox.jpg", IMAGE_SKILLBOX, GL_LINEAR);
	LoadBitmap((g_VoidMainInterfaceReady || AreVoidMainAssetsAvailable()) ? "Custom\\Interface\\newui_skillbox2.jpg" : "Interface\\newui_skillbox2.jpg", IMAGE_SKILLBOX_USE, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill.jpg", IMAGE_NON_SKILL1, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill2.jpg", IMAGE_NON_SKILL2, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_command.jpg", IMAGE_NON_COMMAND, GL_LINEAR);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	LoadBitmap("Interface\\newui_skill3.jpg", IMAGE_SKILL3, GL_LINEAR);
	LoadBitmap("Interface\\newui_non_skill3.jpg", IMAGE_NON_SKILL3, GL_LINEAR);
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
#elif CLIENT_VERSION >= 1001
	if (g_DIVInterfaceReady == true)
	{
		LoadBitmap("Interface\\newui_skill.jpg", IMAGE_SKILL1, GL_LINEAR);
		LoadBitmap("Interface\\newui_skill2.jpg", IMAGE_SKILL2, GL_LINEAR);
		LoadBitmap("Interface\\newui_command.jpg", IMAGE_COMMAND, GL_LINEAR);
		LoadBitmap("Interface\\newui_non_command.jpg", IMAGE_NON_COMMAND, GL_LINEAR);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		LoadBitmap("Interface\\newui_skill3.jpg", IMAGE_SKILL3, GL_LINEAR);
		LoadBitmap("Interface\\newui_non_skill3.jpg", IMAGE_NON_SKILL3, GL_LINEAR);
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
		LoadBitmap("Interface\\DIV\\DIV_Skill01.jpg", DIV_SKILL1, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_Skill02.jpg", DIV_SKILL2, GL_NEAREST, GL_REPEAT);
		LoadBitmap((g_VoidMainInterfaceReady || AreVoidMainAssetsAvailable()) ? "Custom\\Interface\\newui_skillbox.jpg" : "Interface\\newui_skillbox.jpg", IMAGE_SKILLBOX, GL_LINEAR);
		LoadBitmap((g_VoidMainInterfaceReady || AreVoidMainAssetsAvailable()) ? "Custom\\Interface\\newui_skillbox2.jpg" : "Interface\\newui_skillbox2.jpg", IMAGE_SKILLBOX_USE, GL_LINEAR);
		LoadBitmap("Interface\\DIV\\DIV_SkillCommand.jpg", DIV_COMMAND_SKILL, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_SkillBox.tga", DIV_SKILL_BOX, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_non_Skill01.jpg", DIV_NON_SKILL1, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_non_Skill02.jpg", DIV_NON_SKILL2, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_non_SkillCommand.jpg", DIV_NON_COMMAND_SKILL, GL_NEAREST, GL_REPEAT);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		LoadBitmap("Interface\\DIV\\DIV_Skill03.jpg", DIV_SKILL3, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_non_Skill03.jpg", DIV_NON_SKILL3, GL_NEAREST, GL_REPEAT);
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
		LoadBitmap("Interface\\DIV\\DIV_SkillMaster.jpg", DIV_SKILL_MASTER, GL_NEAREST, GL_REPEAT);
		LoadBitmap("Interface\\DIV\\DIV_non_SkillMaster.jpg", DIV_NON_SKILL_MASTER, GL_NEAREST, GL_REPEAT);
	}
	else
	{
		LoadBitmap("Interface\\newui_skill.jpg", IMAGE_SKILL1, GL_LINEAR);
		LoadBitmap("Interface\\newui_skill2.jpg", IMAGE_SKILL2, GL_LINEAR);
		LoadBitmap("Interface\\newui_command.jpg", IMAGE_COMMAND, GL_LINEAR);
		LoadBitmap((g_VoidMainInterfaceReady || AreVoidMainAssetsAvailable()) ? "Custom\\Interface\\newui_skillbox.jpg" : "Interface\\newui_skillbox.jpg", IMAGE_SKILLBOX, GL_LINEAR);
		LoadBitmap((g_VoidMainInterfaceReady || AreVoidMainAssetsAvailable()) ? "Custom\\Interface\\newui_skillbox2.jpg" : "Interface\\newui_skillbox2.jpg", IMAGE_SKILLBOX_USE, GL_LINEAR);
		LoadBitmap("Interface\\newui_non_command.jpg", IMAGE_NON_COMMAND, GL_LINEAR);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		LoadBitmap("Interface\\newui_skill3.jpg", IMAGE_SKILL3, GL_LINEAR);
		LoadBitmap("Interface\\newui_non_skill3.jpg", IMAGE_NON_SKILL3, GL_LINEAR);
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
	}
	LoadBitmap("Interface\\skill_render.jpg", IMAGE_SKILLBOX_LARGE, GL_LINEAR);
	LoadBitmap("Interface\\skill_render_on.tga", IMAGE_SKILL_USE, GL_LINEAR);
	LoadBitmap("Interface\\skill_render_page.jpg", IMAGE_SKILL_PAGE, GL_LINEAR);
	LoadBitmap("Interface\\btn_arrowup.jpg", IMAGE_SKILL_ARROW_UP, GL_LINEAR);
	LoadBitmap("Interface\\btn_arrowdown.jpg", IMAGE_SKILL_ARROW_DOWN, GL_LINEAR);

#endif
}

void SEASON3B::CNewUISkillList::UnloadImages()
{
	DeleteBitmap(IMAGE_SKILL1);
	DeleteBitmap(IMAGE_SKILL2);
	DeleteBitmap(IMAGE_COMMAND);
	DeleteBitmap(IMAGE_SKILLBOX);
	DeleteBitmap(IMAGE_SKILLBOX_USE);
	//DeleteBitmap(IMAGE_NON_SKILL1);
	//DeleteBitmap(IMAGE_NON_SKILL2);
	DeleteBitmap(IMAGE_NON_COMMAND);
	DeleteBitmap(DIV_SKILL_BOX);
	DeleteBitmap(DIV_SKILL1);
	DeleteBitmap(DIV_SKILL2);
	DeleteBitmap(DIV_COMMAND_SKILL);
	DeleteBitmap(DIV_SKILL3);
	DeleteBitmap(DIV_NON_SKILL1);
	DeleteBitmap(DIV_NON_SKILL2);
	DeleteBitmap(DIV_NON_COMMAND_SKILL);
	DeleteBitmap(DIV_NON_SKILL3);
	DeleteBitmap(DIV_SKILL_MASTER);
	DeleteBitmap(DIV_NON_SKILL_MASTER);
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	DeleteBitmap(IMAGE_SKILL3);
	DeleteBitmap(IMAGE_NON_SKILL3);
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
#if CLIENT_VERSION >= 1001
	DeleteBitmap(IMAGE_SKILLBOX_LARGE);
	DeleteBitmap(IMAGE_SKILL_USE);
	DeleteBitmap(IMAGE_SKILL_PAGE);
	DeleteBitmap(IMAGE_SKILL_ARROW_UP);
	DeleteBitmap(IMAGE_SKILL_ARROW_DOWN);
#endif
}

// TOOLTIP E CLICK
bool SEASON3B::CNewUISkillList::UpdateMouseEvent()
{
	if(g_isCharacterBuff((&Hero->Object), eBuff_DuelWatch))
	{
		m_bSkillList = false;
		return true;
	}

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;
	BYTE bySkillMasterNumber = CharacterAttribute->SkillMasterNumber;

	float x, y, width, height;

	m_bRenderSkillInfo = false;

	if(bySkillNumber <= 0)
	{
		return true;
	}
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	x = gWideScreen.GetCenterPosition(385); y = 431.f; width = 32.f; height = 38.f;
#elif CLIENT_VERSION >= 1001
	x = GetDIVMainX() + 251.5f; y = GetDIVMainY() + 39.5f; width = 26; height = 28;
#endif
	float currentSkillHitX = x + 15.f;
	float currentSkillHitY = y;
	float currentSkillHitW = width;
	float currentSkillHitH = height;
#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		x = GetVoidCurrentSkillX();
		y = GetVoidCurrentSkillY();
		currentSkillHitX = GetVoidMainX() + 302.f;
		currentSkillHitY = gWideScreen.GetBottomPosition(431.5f);
		currentSkillHitW = 40.f;
		currentSkillHitH = 44.f;
	}
	else if (g_DIVInterfaceReady == true)
	{
		currentSkillHitX = GetDIVMainX() + 258.f;
		currentSkillHitY = GetDIVMainY() + 35.f;
		currentSkillHitW = 44.f;
		currentSkillHitH = 40.f;
	}
#endif
	if(m_EventState == EVENT_NONE && MouseLButtonPush == false 
		&& SEASON3B::CheckMouseIn(currentSkillHitX, currentSkillHitY, currentSkillHitW, currentSkillHitH) == true)
	{
		m_EventState = EVENT_BTN_HOVER_CURRENTSKILL;
		return true;
	}
	if(m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(currentSkillHitX, currentSkillHitY, currentSkillHitW, currentSkillHitH) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if(m_EventState == EVENT_BTN_HOVER_CURRENTSKILL && (MouseLButtonPush == true || MouseLButtonDBClick == true)
		&& SEASON3B::CheckMouseIn(currentSkillHitX, currentSkillHitY, currentSkillHitW, currentSkillHitH) == true)
	{
#if CLIENT_VERSION >= 1001
		if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
#endif
			m_EventState = EVENT_BTN_DOWN_CURRENTSKILL;
		return false;
	}
	if(m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		if(MouseLButtonPush == false && MouseLButtonDBClick == false)
		{
			if(SEASON3B::CheckMouseIn(currentSkillHitX, currentSkillHitY, currentSkillHitW, currentSkillHitH) == true)
			{		
				m_bSkillList = !m_bSkillList;

				PlayBuffer(SOUND_CLICK01);

#if CLIENT_VERSION >= 1001
				m_SkillPage = 0;
				m_SkillShopBuffPage = 0;
				m_SkillListButtonUp.ForceButtonDown(true);
				m_SkillListButtonUp.Lock();

				m_SkillListButtonDown.ForceButtonDown(true);
				m_SkillListButtonDown.Lock();
				if(m_bSkillList == true)
				{
					CenterSkillShopWindow(m_SkillShopPos);
					m_SkillShopDragging = false;
					RequestSkillShopList();
				}
#endif
				m_EventState = EVENT_NONE;
				return false;
			}
			m_EventState = EVENT_NONE;
			return true;
		}
		
	}
	
	if(m_EventState == EVENT_BTN_HOVER_CURRENTSKILL)
	{
		if (m_bSkillList) {
			m_EventState = EVENT_NONE;
			return false;
		}
		m_bRenderSkillInfo = true;
		m_iRenderSkillInfoType = Hero->CurrentSkill;
		m_iRenderSkillInfoPosX = x + 5+5;
		m_iRenderSkillInfoPosY = y-10;
		
		return false;
	}
	else if(m_EventState == EVENT_BTN_DOWN_CURRENTSKILL)
	{
		return false;
	}
#if CLIENT_VERSION >= 1001
	if (g_VoidMainInterfaceReady == true)
	{
		if (m_bSkillList == false)
		{
			return true;
		}

		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
		{
			m_bSkillList = false;
			m_SkillPage = 0;
			m_SkillListButtonUp.ForceButtonDown(true);
			m_SkillListButtonUp.Lock();

			m_SkillListButtonDown.ForceButtonDown(true);
			m_SkillListButtonDown.Lock();
			return true;
		}

		return UpdateSkillShopMouseEvent();
	}
#endif
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	x = gWideScreen.GetCenterPosition(222); y =431.f; width = 32.f*5.f; height = 38.f;
	if(m_EventState == EVENT_NONE && MouseLButtonPush == false 
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_HOVER_SKILLHOTKEY;
		return true;
	}
	if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(x, y, width, height) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == true
		&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
	{
		m_EventState = EVENT_BTN_DOWN_SKILLHOTKEY;
		return false;
	}
	
	x = gWideScreen.GetCenterPosition(190); y =431.f; width = 32.f; height = 38.f;
	int iStartIndex = (m_bHotKeySkillListUp == true) ? 6 : 1;
	for(int i=0, iIndex = iStartIndex; i<5; ++i, iIndex++)
	{
		x += width;

		if(iIndex == 10)
		{
			iIndex = 0;
		}
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			if(m_iHotKeySkillType[iIndex] == -1)
			{
				if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
				{
					m_bRenderSkillInfo = false;
					m_iRenderSkillInfoType = -1;
				}
				if(m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY && MouseLButtonPush == false)
				{
					m_EventState = EVENT_NONE;
				}
				continue;
			}

			if(IsClassicSkillBarEntry(m_iHotKeySkillType[iIndex]) == false)

			{

				m_iHotKeySkillType[iIndex] = -1;

				continue;

			}

			WORD bySkillType = CharacterAttribute->Skill[m_iHotKeySkillType[iIndex]];

			if(bySkillType == 0 || ( bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF ))
				continue;

			BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

			if(bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
			{
				continue;
			}

			if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
			{
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = m_iHotKeySkillType[iIndex];
				m_iRenderSkillInfoPosX = x - 5;
				m_iRenderSkillInfoPosY = y;
				return true;
			}
			if(m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
			{
				if(MouseLButtonPush == false)
				{
					if(m_iRenderSkillInfoType == m_iHotKeySkillType[iIndex])
					{
						m_EventState = EVENT_NONE;
						m_wHeroPriorSkill = (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END) ? Hero->CurrentSkill : CharacterAttribute->Skill[Hero->CurrentSkill];
						Hero->CurrentSkill = m_iHotKeySkillType[iIndex];
						PlayBuffer(SOUND_CLICK01);
						return false;
					}
					else
					{
						m_EventState = EVENT_NONE;
					}
				}
			}
		}
	}
	
	x = gWideScreen.GetCenterPosition(222); y =431.f; width = 32.f*5.f; height = 38.f;
	if(m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
	{
		if(MouseLButtonPush == false && SEASON3B::CheckMouseIn(x, y, width, height) == false)
		{
			m_EventState = EVENT_NONE;
			return true;	
		}
		return false;
	}

	if(m_bSkillList == false)
		return true;
	
	WORD bySkillType = 0;

	int iSkillCount = 0;
	bool bMouseOnSkillList = false;
	
	x = gWideScreen.GetCenterPosition(385); y = 390; width = 32; height = 38;
	float fOrigX = x;// 385.f;
	
	EVENT_STATE PrevEventState = m_EventState;

	for(int i=0; i<MAX_MAGIC; ++i)
	{
		bySkillType = CharacterAttribute->Skill[i];

		if(bySkillType == 0 || ( bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF ))
			continue;

		BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;

		if(bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
		{
			continue;
		}


		if(iSkillCount == 18)
		{
			y -= height;
		}
		
		if(iSkillCount < 14)
		{
			int iRemainder = iSkillCount % 2;
			int iQuotient = iSkillCount / 2;
			
			if(iRemainder == 0)
			{
				x = fOrigX + iQuotient * width;
			}
			else
			{
				x = fOrigX - (iQuotient + 1) * width;
			}
		}
		else if(iSkillCount >= 14 && iSkillCount < 18)
		{
			x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
		}
		else
		{
			x = fOrigX - (12 * width) + ((iSkillCount - 17) * width);
		}

		iSkillCount++;
		
		if(SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			bMouseOnSkillList = true;
			if(m_EventState == EVENT_NONE && MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = i;
				m_iRenderSkillInfoPosX = x;
				m_iRenderSkillInfoPosY = y;
				break;
			}
		}
		
		if(m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true
			&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
#if CLIENT_VERSION >= 1001
			if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
#endif
				m_EventState = EVENT_BTN_DOWN_SKILLLIST;
			break;
		}
		
		if(m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false
			&& SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_bRenderSkillInfo = true;
			m_iRenderSkillInfoType = i;
			m_iRenderSkillInfoPosX = x;
			m_iRenderSkillInfoPosY = y;
		}
		
		if(m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
			&& m_iRenderSkillInfoType == i && SEASON3B::CheckMouseIn(x, y, width, height) == true)
		{
			m_EventState = EVENT_NONE;

			m_wHeroPriorSkill = (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END) ? Hero->CurrentSkill : CharacterAttribute->Skill[Hero->CurrentSkill];

			Hero->CurrentSkill = i;
			m_bSkillList = false;
	
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}
#elif CLIENT_VERSION >= 1001

	const float divHotKeyAreaX = GetDIVMainX() + 117.f;
	const float divHotKeyAreaY = GetDIVMainY() + 36.f;
	const float divHotKeyAreaW = 152.f;
	const float divHotKeyAreaH = 36.f;
	if(m_EventState == EVENT_NONE && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(divHotKeyAreaX, divHotKeyAreaY, divHotKeyAreaW, divHotKeyAreaH) == true)
	{
		m_EventState = EVENT_BTN_HOVER_SKILLHOTKEY;
		return true;
	}
	if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == false
		&& SEASON3B::CheckMouseIn(divHotKeyAreaX, divHotKeyAreaY, divHotKeyAreaW, divHotKeyAreaH) == false)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY && MouseLButtonPush == true
		&& SEASON3B::CheckMouseIn(divHotKeyAreaX, divHotKeyAreaY, divHotKeyAreaW, divHotKeyAreaH) == true)
	{
		m_EventState = EVENT_BTN_DOWN_SKILLHOTKEY;
		return false;
	}
	const int divStartSkillIndex = (m_bHotKeySkillListUp == true) ? 6 : 1;
	for(int i = 0; i < 5; ++i)
	{
		int iIndex = divStartSkillIndex + i;
		if(iIndex == 10)
		{
			iIndex = 0;
		}
		const float hotKeyX = (29.5f * i) + GetDIVMainX() + 119.f;
		const float hotKeyY = GetDIVMainY() + 40.f;
		if(SEASON3B::CheckMouseIn(hotKeyX - 3.f, hotKeyY - 4.f, 32.f, 36.f) == true)
		{
			if(m_iHotKeySkillType[iIndex] == -1)
			{
				if(m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY && MouseLButtonPush == false)
				{
					m_EventState = EVENT_NONE;
				}
				continue;
			}
			if(IsClassicSkillBarEntry(m_iHotKeySkillType[iIndex]) == false)
			{
				m_iHotKeySkillType[iIndex] = -1;
				continue;
			}
			WORD bySkillType = CharacterAttribute->Skill[m_iHotKeySkillType[iIndex]];
			if(bySkillType == 0 || (bySkillType >= AT_SKILL_STUN && bySkillType <= AT_SKILL_REMOVAL_BUFF))
			{
				continue;
			}
			if(m_EventState == EVENT_BTN_HOVER_SKILLHOTKEY)
			{
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = m_iHotKeySkillType[iIndex];
				m_iRenderSkillInfoPosX = hotKeyX - 5.f;
				m_iRenderSkillInfoPosY = hotKeyY;
				return true;
			}
			if(m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY && MouseLButtonPush == false)
			{
				if(m_iRenderSkillInfoType == m_iHotKeySkillType[iIndex])
				{
					m_EventState = EVENT_NONE;
					m_wHeroPriorSkill = (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END) ? Hero->CurrentSkill : CharacterAttribute->Skill[Hero->CurrentSkill];
					Hero->CurrentSkill = m_iHotKeySkillType[iIndex];
					PlayBuffer(SOUND_CLICK01);
					return false;
				}
				m_EventState = EVENT_NONE;
			}
		}
	}
	if(m_EventState == EVENT_BTN_DOWN_SKILLHOTKEY)
	{
		if(MouseLButtonPush == false && SEASON3B::CheckMouseIn(divHotKeyAreaX, divHotKeyAreaY, divHotKeyAreaW, divHotKeyAreaH) == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}

	if (m_bSkillList == false)
		return true;

	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
	{
		m_bSkillList = !m_bSkillList;
		m_SkillPage = 0;
		m_SkillListButtonUp.ForceButtonDown(true);
		m_SkillListButtonUp.Lock();

		m_SkillListButtonDown.ForceButtonDown(true);
		m_SkillListButtonDown.Lock();
		return true;
	}

	return UpdateSkillShopMouseEvent();

	std::vector<int> m_skillList;

	m_skillList.clear();

	int skillOffset = 0;

	int skillCount = 0;

	bool bMouseOnSkillList = false;

	x = gWideScreen.GetCenterPosition(320) - 175.f; y = gWideScreen.GetBottomPosition(339); width = 35; height = 40;

	float originalX = x;
	goto RENDERSKILL;

RENDERSKILL:
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		int iSkillType = CharacterAttribute->Skill[i];

		if(IsClassicSkillBarEntry(i) == false)
		{
			continue;
		}
		if (iSkillType == 0 || (iSkillType >= AT_SKILL_STUN && iSkillType <= AT_SKILL_REMOVAL_BUFF)) continue;

		BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;

		if (bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL) continue;

		m_skillList.push_back(i);
	}
	if (giPetManager::IsDarkSpiritEquipped(Hero))
	{
		for (int cmd = 0; cmd < PET_CMD_END; ++cmd)
		{
			m_skillList.push_back(AT_PET_COMMAND_DEFAULT + cmd);
		}
	}

	skillOffset += m_SkillPage * 10;

	for (std::vector<int>::iterator it = m_skillList.begin() + skillOffset; it != m_skillList.end(); ++it) 
	{
		int tempX = originalX;

		if (skillCount < 10) {
			tempX += (skillCount * 35);
		}
		else {
			tempX += ((skillCount - 10) * 35);
		}

		int tempY = y + (skillCount > 9 ? 40 : 0);

		if (SEASON3B::CheckMouseIn(tempX, tempY, width, height) == true)
		{
			bMouseOnSkillList = true;
			if (m_EventState == EVENT_NONE && MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
				m_bRenderSkillInfo = true;
				m_iRenderSkillInfoType = *it;
				m_iRenderSkillInfoPosX = tempX;
				m_iRenderSkillInfoPosY = tempY;
				break;
			}
		}

		if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == true
			&& SEASON3B::CheckMouseIn(tempX, tempY, width, height) == true)
		{
			if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
				m_EventState = EVENT_BTN_DOWN_SKILLLIST;
			break;
		}

		if (m_EventState == EVENT_BTN_HOVER_SKILLLIST && MouseLButtonPush == false
			&& SEASON3B::CheckMouseIn(tempX, tempY, width, height) == true)
		{
			m_bRenderSkillInfo = true;
			m_iRenderSkillInfoType = *it;
			m_iRenderSkillInfoPosX = tempX;
			m_iRenderSkillInfoPosY = tempY;
		}

		if (m_EventState == EVENT_BTN_DOWN_SKILLLIST && MouseLButtonPush == false
			&& m_iRenderSkillInfoType == *it && SEASON3B::CheckMouseIn(tempX, tempY, width, height) == true)
		{
			m_EventState = EVENT_NONE;

			m_wHeroPriorSkill = (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END) ? Hero->CurrentSkill : CharacterAttribute->Skill[Hero->CurrentSkill];

			Hero->CurrentSkill = *it;
			m_bSkillList = false;

			PlayBuffer(SOUND_CLICK01);
			return false;
		}

		skillCount++;
		if (skillCount == 20) break;
	}

#endif

	if(bMouseOnSkillList == false && m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
		return true;
	}
	if(bMouseOnSkillList == false && MouseLButtonPush == false
		&& m_EventState == EVENT_BTN_DOWN_SKILLLIST )
	{
		m_EventState = EVENT_NONE;
		return false;
	}
	if(m_EventState == EVENT_BTN_DOWN_SKILLLIST)
	{
		if(MouseLButtonPush == false)
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		return false;
	}
#if CLIENT_VERSION >= 1001

	if (m_SkillPage == 0) {
		m_SkillListButtonUp.Lock();
		m_SkillListButtonUp.ForceButtonDown(true);
	}
	else{
		m_SkillListButtonUp.UnLock();
		m_SkillListButtonUp.ForceButtonDown(false);
	}

	if (m_skillList.size() < 20 || m_SkillPage == 1) {
		m_SkillListButtonDown.Lock();
		m_SkillListButtonDown.ForceButtonDown(true);
	}
	else {
		m_SkillListButtonDown.UnLock();
		m_SkillListButtonDown.ForceButtonDown(false);
	}

	if (m_SkillListButtonUp.UpdateMouseEvent() == true && m_SkillPage > 0)
	{
		
			m_SkillPage--;
			PlayBuffer(SOUND_CLICK01);
			m_EventState = EVENT_NONE;
			return false;
		
	}

	if (m_SkillListButtonDown.UpdateMouseEvent() == true && m_skillList.size() > 20)
	{
		
			if (m_SkillPage < 1) {
				m_SkillPage++;
				PlayBuffer(SOUND_CLICK01);
				m_EventState = EVENT_NONE;
				return false;
			}
	}


	if (m_bSkillList && SEASON3B::CheckMouseIn(gWideScreen.GetCenterPosition(320) - 175.f, gWideScreen.GetBottomPosition(339), 350, 81) )
	{
		return false;
	}

#endif

	if (SEASON3B::CheckMouseIn(x + 355.f, y + 22.5f, 18, 36) == true)
	{
		m_EventState = EVENT_NONE;
		return false;
	}

	return true;
}

bool SEASON3B::CNewUISkillList::UpdateKeyEvent()
{
#if CLIENT_VERSION >= 1001
	if(m_bSkillList == true && SEASON3B::IsPress(VK_ESCAPE) == true)
	{
		CloseSkillShop();
		PlayBuffer(SOUND_CLICK01);
		return false;
	}

	if(m_bSkillList == true && SEASON3B::IsRepeat(VK_CONTROL) == true)
	{
		std::vector<int> skillList;
		BuildSkillShopSkillList(skillList);

		const int skillStart = m_SkillPage * SKILL_SHOP_SKILLS_PER_PAGE;
		for(int n = 0; n < SKILL_SHOP_SKILLS_PER_PAGE; ++n)
		{
			int listIndex = skillStart + n;
			if(listIndex >= (int)skillList.size())
			{
				break;
			}

			float cellX = (float)m_SkillShopPos.x + SKILL_SHOP_SKILL_START_X + ((n % SKILL_SHOP_SKILL_COLUMNS) * SKILL_SHOP_CELL_STEP_X);
			float cellY = (float)m_SkillShopPos.y + SKILL_SHOP_SKILL_START_Y + ((n / SKILL_SHOP_SKILL_COLUMNS) * SKILL_SHOP_CELL_STEP_Y);

			if(SEASON3B::CheckMouseIn(cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT) == true)
			{
				if(IsSkillShopDisabledEntry(skillList[listIndex]) == true)
				{
					for(int slot = 1; slot <= 6; ++slot)
					{
						if(SEASON3B::IsPress('0' + slot) == true)
						{
							return false;
						}
					}

					break;
				}

				for(int slot = 1; slot <= 6; ++slot)
				{
					if(SEASON3B::IsPress('0' + slot) == true)
					{
						SetHotKey(slot, skillList[listIndex]);
						PlayBuffer(SOUND_CLICK01);
						return false;
					}
				}

				break;
			}
		}
	}

#endif

	for(int i=0; i<9; ++i)
	{
		if(SEASON3B::IsPress('1' + i))
		{
			UseHotKey(i+1);
		}
	}

	if(SEASON3B::IsPress('0'))
	{
		UseHotKey(0);
	}

	if(m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		if(SEASON3B::IsRepeat(VK_CONTROL))
		{
			for(int i=0; i<9; ++i)
			{
				if(SEASON3B::IsPress('1' + i))
				{
					SetHotKey(i+1, m_iRenderSkillInfoType);

					return false;
				}
			}

			if(SEASON3B::IsPress('0'))
			{
				SetHotKey(0, m_iRenderSkillInfoType);

				return false;
			}
		}
	}

	if(SEASON3B::IsRepeat(VK_SHIFT))
	{
		return true;
	}
		
	return true;
}

bool SEASON3B::CNewUISkillList::IsArrayUp(BYTE bySkill)
{
	for(int i=0; i<SKILLHOTKEY_COUNT; ++i)
	{
		if(m_iHotKeySkillType[i] == bySkill)
		{
			if(i == 0 || i > 5)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool SEASON3B::CNewUISkillList::IsArrayIn(BYTE bySkill)
{
	for(int i=0; i<SKILLHOTKEY_COUNT; ++i)
	{
		if(m_iHotKeySkillType[i] == bySkill)
		{
			return true;
		}
	}

	return false;
}

void SEASON3B::CNewUISkillList::SetHotKey(int iHotKey, int iSkillType)
{
	if(iHotKey < 0 || iHotKey >= SKILLHOTKEY_COUNT)
	{
		return;
	}

	if(IsClassicSkillBarEntry(iSkillType) == false)
	{
		m_iHotKeySkillType[iHotKey] = -1;
		return;
	}

	for(int i=0; i<SKILLHOTKEY_COUNT; ++i)
	{
		if(m_iHotKeySkillType[i] == iSkillType)
		{
			m_iHotKeySkillType[i] = -1;
			break;
		}
	}	

	m_iHotKeySkillType[iHotKey] = iSkillType;
}

int SEASON3B::CNewUISkillList::GetHotKey(int iHotKey)
{
	return m_iHotKeySkillType[iHotKey];
}

int SEASON3B::CNewUISkillList::GetSkillIndex(int iSkillType)
{
	int iReturn = -1;
	for(int i=0; i<MAX_MAGIC; ++i)
	{
		if(IsClassicSkillBarEntry(i) == true && CharacterAttribute->Skill[i] == iSkillType)
		{
			iReturn = i;
			break;
		}
	}

	return iReturn;
}

void SEASON3B::CNewUISkillList::UseHotKey(int iHotKey)
{
	if(m_iHotKeySkillType[iHotKey] != -1)
	{
		int iHotKeySkillType = m_iHotKeySkillType[iHotKey];

		if(IsClassicSkillBarEntry(iHotKeySkillType) == false)
		{
			m_iHotKeySkillType[iHotKey] = -1;
			return;
		}
		WORD wHotKeySkill = 0;

		if(iHotKeySkillType >= AT_PET_COMMAND_DEFAULT && iHotKeySkillType < AT_PET_COMMAND_END)
		{
			wHotKeySkill = iHotKeySkillType;
		}
		else
		{
			wHotKeySkill = CharacterAttribute->Skill[iHotKeySkillType];
		}

		if(wHotKeySkill == 0)
		{
			return;
		}

		m_wHeroPriorSkill = (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END) ? Hero->CurrentSkill : CharacterAttribute->Skill[Hero->CurrentSkill];

		Hero->CurrentSkill = iHotKeySkillType;
		WORD bySkill = wHotKeySkill;


        if ( 
			g_pOption->IsAutoAttack() == true
			&& gMapManager.WorldActive != WD_6STADIUM 
			&& gMapManager.InChaosCastle()==false 
			&& ( bySkill == AT_SKILL_TELEPORT || bySkill == AT_SKILL_TELEPORT_B ) )
        {
            SelectedCharacter = -1;
            Attacking = -1;
        }
	}
}

bool SEASON3B::CNewUISkillList::Update()
{
	SyncFenrirPlasmaStormSkill();
	SyncShieldBlockingSkill();

	for(int i=0; i<SKILLHOTKEY_COUNT; ++i)
	{
		if(m_iHotKeySkillType[i] != -1 && IsClassicSkillBarEntry(m_iHotKeySkillType[i]) == false)
		{
			m_iHotKeySkillType[i] = -1;
		}
	}

	if(IsClassicSkillBarEntry(Hero->CurrentSkill) == false)
	{
		Hero->CurrentSkill = FindFirstClassicSkillBarEntry();
	}
	if(IsArrayIn(Hero->CurrentSkill) == true)
	{
		if(IsArrayUp(Hero->CurrentSkill) == true)
		{
			m_bHotKeySkillListUp = true;
		}
		else
		{
			m_bHotKeySkillListUp = false;
		}
	}
	return true;
}

// SKILL ATUAL
void SEASON3B::CNewUISkillList::RenderCurrentSkillAndHotSkillList()
{
	if(IsClassicSkillBarEntry(Hero->CurrentSkill) == false)
	{
		Hero->CurrentSkill = FindFirstClassicSkillBarEntry();
	}

	float x, y, width, height;
	
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;
	
	if(bySkillNumber > 0)
	{		
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
		int i;
		int iStartSkillIndex = 1;
		if (m_bHotKeySkillListUp)
		{
			iStartSkillIndex = 6;
		}

		x = gWideScreen.GetCenterPosition(190)-0.5f; y = 431.5f; width = 32; height = 38;
		for(i=0; i<5; ++i)
		{
			x += width;
			
			int iIndex = iStartSkillIndex + i;
			if(iIndex == 10)
			{
				iIndex = 0;
			}
			
			if(m_iHotKeySkillType[iIndex] == -1)
			{
				continue;
			}

			if(IsClassicSkillBarEntry(m_iHotKeySkillType[iIndex]) == false)
			{
				m_iHotKeySkillType[iIndex] = -1;
				continue;
			}
			if(Hero->CurrentSkill == m_iHotKeySkillType[iIndex])
			{
				SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
			}
			RenderSkillIcon(m_iHotKeySkillType[iIndex], x+6, y+6, 20, 28);
		}
		x = gWideScreen.GetCenterPosition(392); y = 437; width = 20; height = 28;
#endif
#if CLIENT_VERSION >= 1001
		if (g_VoidMainInterfaceReady == true)
		{
			RenderVoidSkillAtlasBox(GetVoidCurrentSkillX() - 6.f, GetVoidCurrentSkillY() - 6.f, true);
			RenderSkillIcon(Hero->CurrentSkill, GetVoidCurrentSkillX(), GetVoidCurrentSkillY(), 20.f, 28.f);
			RenderVoidMainButtons();
			return;
		}

	if (g_DIVInterfaceReady == true)
		{
		const float mainX = GetDIVMainX();
		const float mainY = GetDIVMainY();
		const int startSkillIndex = (m_bHotKeySkillListUp == true) ? 6 : 1;
			g_DIVSkillBarRendering = true;

		RenderBitmap(SEASON3B::DIV_HOTKEY_SKILL_LIST_UP, mainX + 122.f, mainY + 68.f, 134.f, 14.f, 0.f, 0.f, 1.f, 0.33f, true, true, 0.f);
		RenderBitmap(SEASON3B::DIV_HOTKEY_SKILL_LIST_UP, mainX + 122.f, mainY + 68.f, 134.f, 14.f, 0.f, (m_bHotKeySkillListUp ? 0.66f : 0.33f), 1.f, 0.33f, true, true, 0.f);

		for (int i = 0; i < 5; ++i)
		{
			int iIndex = startSkillIndex + i;
			if (iIndex == 10)
			{
				iIndex = 0;
			}
			if (m_iHotKeySkillType[iIndex] == -1)
			{
				continue;
			}

			if(IsClassicSkillBarEntry(m_iHotKeySkillType[iIndex]) == false)
			{
				m_iHotKeySkillType[iIndex] = -1;
				continue;
			}

			const float hotKeyX = 29.5f * i + (mainX + 119.f);
			const float hotKeyY = mainY + 40.f;
			RenderSkillIcon(m_iHotKeySkillType[iIndex], hotKeyX, hotKeyY, 20.f, 28.f);
			const float skillBoxU = (Hero->CurrentSkill == m_iHotKeySkillType[iIndex]) ? 0.5f : 0.f;
			RenderBitmap(DIV_SKILL_BOX, hotKeyX - 1.5f, hotKeyY - 1.5f, 26.f, 26.f, skillBoxU, 0.f, 0.25f, 1.f, true, true, 0.f);
		}

		x = mainX + 266.5f; y = mainY + 39.5f; width = 20; height = 28;
		RenderSkillIcon(Hero->CurrentSkill, x, y, width, height);
		RenderBitmap(DIV_SKILL_BOX, x - 1.5f, y - 1.5f, 26.f, 26.f, 0.25f, 0.f, 0.25f, 1.f, true, true, 0.f);
			g_DIVSkillBarRendering = false;
		return;
		}
		x = gWideScreen.GetCenterPosition(392); y = 437; width = 20; height = 28;
#endif
		RenderSkillIcon(Hero->CurrentSkill, x, y, width, height);
	}
}

// RENDERIZA LISTA SKILLS
bool SEASON3B::CNewUISkillList::Render()
{
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	int i;
	float x, y, width, height;
	
	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	if(bySkillNumber > 0)
	{
		if(m_bSkillList == true)
		{
			x = gWideScreen.GetCenterPosition(385); y = 390; width = 32; height = 38;
			float fOrigX = gWideScreen.GetCenterPosition(385);
            int iSkillType  = 0;
			int iSkillCount = 0;

			for(i=0; i<MAX_MAGIC; ++i)
			{
                iSkillType = CharacterAttribute->Skill[i];
				
                if(IsClassicSkillBarEntry(i) == false)
                {
                	continue;
                }
				if(iSkillType != 0 && (iSkillType < AT_SKILL_STUN || iSkillType > AT_SKILL_REMOVAL_BUFF))
				{
                    BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;
                  
                    if(bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL)
					{
                        continue;
					}

					if(iSkillCount == 18)
					{
						y -= height;
					}

					if(iSkillCount < 14)
					{
						int iRemainder = iSkillCount % 2;
						int iQuotient = iSkillCount / 2;

						if(iRemainder == 0)
						{
							x = fOrigX + iQuotient * width;
						}
						else
						{
							x = fOrigX - (iQuotient + 1) * width;
						}
					}
					else if(iSkillCount >= 14 && iSkillCount < 18)
					{
						x = fOrigX - (8 * width) - ((iSkillCount - 14) * width);
					}
					else
					{
						x = fOrigX - (12 * width) + ((iSkillCount - 17) * width);
					}

					iSkillCount++;

					if(i == Hero->CurrentSkill)
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX_USE, x, y, width, height);
					}
					else
					{
						SEASON3B::RenderImage(IMAGE_SKILLBOX, x, y, width, height);	
					}
					
					RenderSkillIcon(i, x+6, y+6, 20, 28);
				}
			}
			RenderPetSkill();
		}
	}

#elif CLIENT_VERSION >- 1001
	float x, y, width, height, boxWidth, boxHeight, originalX;

	BYTE bySkillNumber = CharacterAttribute->SkillNumber;

	std::vector<int> m_skillList;

	m_skillList.clear();

	int skillOffset = 0;

	int skillCount = 0;

	if (bySkillNumber < 1 || m_bSkillList == false) goto ENDSKILLRENDER;

	RenderSkillShopWindow();
	goto ENDSKILLRENDER;
	
	x = gWideScreen.GetCenterPosition(320)-175.f; y = gWideScreen.GetBottomPosition(339); width = 350; height = 81, boxWidth = 35, boxHeight = 40;

	originalX = x + 7;

	SEASON3B::RenderImageScale(IMAGE_SKILLBOX_LARGE, x, y, width, height, 0.f, 0.f, 701.f, 163.f);
	SEASON3B::RenderImageScale(IMAGE_SKILL_PAGE, x+355.f, y+22.5f, 18, 36, 0.f, 0.f, 72.f, 144.f);

	m_SkillListButtonUp.Render();
	m_SkillListButtonDown.Render();
	goto RENDERSKILL;

RENDERSKILL:
	for (int i = 0; i < MAX_MAGIC; ++i)
	{
		int iSkillType = CharacterAttribute->Skill[i];
	
		if(IsClassicSkillBarEntry(i) == false)
		{
			continue;
		}
		if (iSkillType == 0 || (iSkillType >= AT_SKILL_STUN && iSkillType <= AT_SKILL_REMOVAL_BUFF)) continue;
		
		BYTE bySkillUseType = SkillAttribute[iSkillType].SkillUseType;
		
		if (bySkillUseType == SKILL_USE_TYPE_MASTER || bySkillUseType == SKILL_USE_TYPE_MASTERLEVEL) continue;

		m_skillList.push_back(i);
	}
	if (giPetManager::IsDarkSpiritEquipped(Hero))
	{
		for (int cmd = 0; cmd < PET_CMD_END; ++cmd)
		{
			m_skillList.push_back(AT_PET_COMMAND_DEFAULT + cmd);
		}
	}

	skillOffset += m_SkillPage * 10;

	for (std::vector<int>::iterator it = m_skillList.begin()+skillOffset; it != m_skillList.end(); ++it) 
	{
		int tempX = originalX;

		if (skillCount < 10) {
			tempX += (skillCount * 35);
		}
		else {
			tempX += ((skillCount-10) * 35);
		}

		int tempY = y + 6 + (skillCount > 9 ? 40 : 0);

		if (*it == Hero->CurrentSkill)
			SEASON3B::RenderImageScale(IMAGE_SKILL_USE, tempX -7, tempY-6, boxWidth, boxHeight, 0.f, 0.f, 69.f, 80.f);

		RenderSkillIcon(*it, tempX, tempY, 20, 28);

		skillCount++;
		if (skillCount == 20) break;
	}

ENDSKILLRENDER:
#endif
	
	if(m_bRenderSkillInfo == true && m_pNewUI3DRenderMng)
	{
		m_pNewUI3DRenderMng->RenderUI2DEffect(ITEMHOTKEYNUMBER_CAMERA_Z_ORDER, UI2DEffectCallback, this, 0, 0);

		m_bRenderSkillInfo = false;
	}

	return true;
}

void SEASON3B::CNewUISkillList::BuildSkillShopSkillList(std::vector<int>& skillList)
{
	skillList.clear();

	for(int skillType = 1; skillType < MAX_SKILLS; ++skillType)
	{
		if(IsSkillShopClassSkillType(skillType) == false)
		{
			continue;
		}

		int skillIndex = GetSkillShopLearnedSkillIndex(skillType);
		if(skillIndex != -1)
		{
			skillList.push_back(skillIndex);
		}
		else
		{
			skillList.push_back(MakeSkillShopDisabledEntry(skillType));
		}
	}

	for(int i = 0; i < MAX_MAGIC; ++i)
	{
		int iSkillType = CharacterAttribute->Skill[i];
		if(IsClassicSkillBarEntry(i) == false)
		{
			continue;
		}
		int baseClass = gCharacterManager.GetBaseClass(Hero->Class);

		if(IsSkillShopDisplayableSkillType(iSkillType) == false)
		{
			continue;
		}

		if(IsSkillShop97DSkillType(iSkillType) == false)
		{
			continue;
		}

		if(IsSkillShopBlockedFor97DClass(iSkillType, baseClass) == true)
		{
			continue;
		}

		if(IsSkillShopSkillTypeInList(skillList,iSkillType) == true)
		{
			continue;
		}

		skillList.push_back(i);
	}

	AddSkillShopSkillType(skillList, AT_SKILL_PLASMA_STORM_FENRIR, IsSkillShopFenrirEquipped());
	AddSkillShopSkillType(skillList, AT_SKILL_DARK_HORSE, IsDarkHorseEquippedForEarthquake());
	AddSkillShopSkillType(skillList, AT_SKILL_BLOCKING, IsSkillShopShieldEquipped());

	if(giPetManager::IsDarkSpiritEquipped(Hero))
	{
		for(int cmd = 0; cmd < PET_CMD_END; ++cmd)
		{
			skillList.push_back(AT_PET_COMMAND_DEFAULT + cmd);
		}
	}
}

void SEASON3B::CNewUISkillList::RequestSkillShopList()
{
#if CLIENT_VERSION >= 1001
	DWORD tick = GetTickCount();

	if((tick - m_SkillShopLastConfigRequestTick) < 1200)
	{
		return;
	}

	m_SkillShopLastConfigRequestTick = tick;

	PMSG_CLIENT_SKILL_SHOP_REQUEST_SEND pMsg;
	pMsg.header.set(0xF3, SKILL_SHOP_CLIENT_SUBCODE, sizeof(pMsg));
	pMsg.action = SKILL_SHOP_ACTION_REQUEST_LIST;
	pMsg.index = 0;

	DataSend((BYTE*)&pMsg, pMsg.header.size);
#endif
}

void SEASON3B::CNewUISkillList::SendSkillShopBuyBuff(BYTE index)
{
#if CLIENT_VERSION >= 1001
	DWORD tick = GetTickCount();

	if((tick - m_SkillShopLastBuyRequestTick) < 600)
	{
		return;
	}

	m_SkillShopLastBuyRequestTick = tick;

	PMSG_CLIENT_SKILL_SHOP_REQUEST_SEND pMsg;
	pMsg.header.set(0xF3, SKILL_SHOP_CLIENT_SUBCODE, sizeof(pMsg));
	pMsg.action = SKILL_SHOP_ACTION_BUY_BUFF;
	pMsg.index = index;

	DataSend((BYTE*)&pMsg, pMsg.header.size);
#endif
}

const char* SEASON3B::CNewUISkillList::GetSkillShopCurrencyName(BYTE currency)
{
	switch(currency)
	{
	case 1:
		return "WCoinC";
	case 2:
		return "WCoinP";
	case 3:
		return "Goblin";
	case 4:
		return "UpPoints";
	case 5:
		return "JcPoints";
	case 6:
		return "VCoin";
	case 7:
		return "VGold";
	default:
		return "Zen";
	}
}

	bool SEASON3B::CNewUISkillList::UpdateSkillShopMouseEvent()
	{
#if CLIENT_VERSION >= 1001
	const float shopW = SKILL_SHOP_WINDOW_WIDTH;
	const float shopH = SKILL_SHOP_WINDOW_HEIGHT;
	ClampSkillShopWindow(m_SkillShopPos);
	float shopX = (float)m_SkillShopPos.x;
	float shopY = (float)m_SkillShopPos.y;

	if(m_SkillShopConfigLoaded == false)
	{
		RequestSkillShopList();
	}

	if(m_SkillShopDragging == true)
	{
		if(IsSkillShopLeftButtonDown() == true)
		{
			POINT mouse = GetSkillShopClampedMouse();
			m_SkillShopPos.x = mouse.x - m_SkillShopDragOffset.x;
			m_SkillShopPos.y = mouse.y - m_SkillShopDragOffset.y;
			ClampSkillShopWindow(m_SkillShopPos);
			m_EventState = EVENT_NONE;
			MouseLButtonPush = false;
			MouseLButtonPop = false;
			return false;
		}

		m_SkillShopDragging = false;
		ClampSkillShopWindow(m_SkillShopPos);
		ResetMouseLButton();
		return false;
	}

	if(MouseLButtonPush == true && SEASON3B::CheckMouseIn(shopX + shopW - 29.f, shopY + 11.f, 22.f, 22.f))
	{
		CloseSkillShop();
		ResetMouseLButton();
		PlayBuffer(SOUND_CLICK01);
		return false;
	}

	if((MouseLButtonPush || SEASON3B::IsPress(VK_LBUTTON)) && SEASON3B::CheckMouseIn(shopX + 10.f, shopY + 8.f, shopW - 43.f, 26.f))
	{
		POINT mouse = GetSkillShopClampedMouse();
		m_SkillShopDragging = true;
		m_SkillShopDragOffset.x = mouse.x - m_SkillShopPos.x;
		m_SkillShopDragOffset.y = mouse.y - m_SkillShopPos.y;
		m_EventState = EVENT_NONE;
		MouseLButtonPush = false;
		MouseLButtonPop = false;
		return false;
	}

	std::vector<int> skillList;
	BuildSkillShopSkillList(skillList);

	int maxSkillPage = 0;
	if(skillList.size() > 0)
	{
		maxSkillPage = (int)((skillList.size() - 1) / SKILL_SHOP_SKILLS_PER_PAGE);
	}

	if(MouseLButtonPush == true && SEASON3B::CheckMouseIn(shopX + 56.f, shopY + SKILL_SHOP_SKILL_PAGE_Y, 18.f, 16.f))
	{
		if(m_SkillPage > 0)
		{
			m_SkillPage--;
			PlayBuffer(SOUND_CLICK01);
		}
		ResetMouseLButton();
		return false;
	}

	if(MouseLButtonPush == true && SEASON3B::CheckMouseIn(shopX + 98.f, shopY + SKILL_SHOP_SKILL_PAGE_Y, 18.f, 16.f))
	{
		if(m_SkillPage < maxSkillPage)
		{
			m_SkillPage++;
			PlayBuffer(SOUND_CLICK01);
		}
		ResetMouseLButton();
		return false;
	}

	int maxBuffPage = 0;
	if(m_SkillShopBuffCount > 0)
	{
		maxBuffPage = (int)((m_SkillShopBuffCount - 1) / SKILL_SHOP_BUFFS_PER_PAGE);
	}

	if(MouseLButtonPush == true && SEASON3B::CheckMouseIn(shopX + 56.f, shopY + SKILL_SHOP_BUFF_PAGE_Y, 18.f, 16.f))
	{
		if(m_SkillShopBuffPage > 0)
		{
			m_SkillShopBuffPage--;
			PlayBuffer(SOUND_CLICK01);
		}
		ResetMouseLButton();
		return false;
	}

	if(MouseLButtonPush == true && SEASON3B::CheckMouseIn(shopX + 98.f, shopY + SKILL_SHOP_BUFF_PAGE_Y, 18.f, 16.f))
	{
		if(m_SkillShopBuffPage < maxBuffPage)
		{
			m_SkillShopBuffPage++;
			PlayBuffer(SOUND_CLICK01);
		}
		ResetMouseLButton();
		return false;
	}

	const int skillStart = m_SkillPage * SKILL_SHOP_SKILLS_PER_PAGE;
	bool skillHovered = false;
	for(int n = 0; n < SKILL_SHOP_SKILLS_PER_PAGE; ++n)
	{
		int listIndex = skillStart + n;
		if(listIndex >= (int)skillList.size())
		{
			break;
		}

		float cellX = shopX + SKILL_SHOP_SKILL_START_X + ((n % SKILL_SHOP_SKILL_COLUMNS) * SKILL_SHOP_CELL_STEP_X);
		float cellY = shopY + SKILL_SHOP_SKILL_START_Y + ((n / SKILL_SHOP_SKILL_COLUMNS) * SKILL_SHOP_CELL_STEP_Y);

		if(SEASON3B::CheckMouseIn(cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT))
		{
			if(IsSkillShopDisabledEntry(skillList[listIndex]) == true)
			{
				break;
			}

			skillHovered = true;
			m_bRenderSkillInfo = true;
			m_iRenderSkillInfoType = skillList[listIndex];
			m_iRenderSkillInfoPosX = (int)cellX;
			m_iRenderSkillInfoPosY = (int)cellY;
			if(MouseLButtonPush == false)
			{
				m_EventState = EVENT_BTN_HOVER_SKILLLIST;
			}

			if(MouseLButtonPush == true)
			{
				m_wHeroPriorSkill = (Hero->CurrentSkill >= AT_PET_COMMAND_DEFAULT && Hero->CurrentSkill < AT_PET_COMMAND_END) ? Hero->CurrentSkill : CharacterAttribute->Skill[Hero->CurrentSkill];
				Hero->CurrentSkill = skillList[listIndex];
				ResetMouseLButton();
				PlayBuffer(SOUND_CLICK01);
				return false;
			}

			break;
		}
	}

	if(skillHovered == false && m_EventState == EVENT_BTN_HOVER_SKILLLIST)
	{
		m_EventState = EVENT_NONE;
	}

	const int buffStart = m_SkillShopBuffPage * SKILL_SHOP_BUFFS_PER_PAGE;
	for(int n = 0; n < SKILL_SHOP_BUFFS_PER_PAGE; ++n)
	{
		int listIndex = buffStart + n;
		if(listIndex >= m_SkillShopBuffCount)
		{
			break;
		}

		SKILL_SHOP_BUFF_INFO* lpInfo = &m_SkillShopBuffs[listIndex];
		float cellX = shopX + SKILL_SHOP_BUFF_START_X + ((n % SKILL_SHOP_BUFF_COLUMNS) * SKILL_SHOP_CELL_STEP_X);
		float cellY = shopY + SKILL_SHOP_BUFF_START_Y + ((n / SKILL_SHOP_BUFF_COLUMNS) * SKILL_SHOP_CELL_STEP_Y);

		if(lpInfo->Enabled != 0 && SEASON3B::CheckMouseIn(cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT))
		{
			if(MouseLButtonPush == true)
			{
				SendSkillShopBuyBuff(lpInfo->Index);
				ResetMouseLButton();
				PlayBuffer(SOUND_CLICK01);
				return false;
			}

			break;
		}
	}

	if(SEASON3B::CheckMouseIn(shopX, shopY, shopW, shopH))
	{
		return false;
	}
#endif

	return true;
}

void SEASON3B::CNewUISkillList::RenderSkillShopWindow()
{
#if CLIENT_VERSION >= 1001
	const float shopW = SKILL_SHOP_WINDOW_WIDTH;
	const float shopH = SKILL_SHOP_WINDOW_HEIGHT;
	ClampSkillShopWindow(m_SkillShopPos);
	const float shopX = (float)m_SkillShopPos.x;
	const float shopY = (float)m_SkillShopPos.y;

	if(m_SkillShopConfigLoaded == false)
	{
		RequestSkillShopList();
	}

	EnableAlphaTest();
	glColor4f(0.025f, 0.024f, 0.022f, 0.96f);
	RenderColor(shopX, shopY, shopW, shopH);
	glColor4f(0.105f, 0.095f, 0.080f, 0.90f);
	RenderColor(shopX + 3.f, shopY + 3.f, shopW - 6.f, shopH - 6.f);
	glColor4f(0.038f, 0.037f, 0.035f, 0.94f);
	RenderColor(shopX + 7.f, shopY + 7.f, shopW - 14.f, shopH - 14.f);
	glColor4f(0.12f, 0.045f, 0.020f, 0.94f);
	RenderColor(shopX + 11.f, shopY + 12.f, shopW - 22.f, 22.f);
	glColor4f(0.055f, 0.055f, 0.052f, 0.82f);
	RenderColor(shopX + 6.f, shopY + 50.f, shopW - 12.f, 142.f);
	RenderColor(shopX + 6.f, shopY + 205.f, shopW - 12.f, 118.f);
	glColor4f(0.18f, 0.16f, 0.125f, 0.45f);
	RenderColor(shopX + 6.f, shopY + 193.f, shopW - 12.f, 1.f);
	RenderColor(shopX + 6.f, shopY + 202.f, shopW - 12.f, 1.f);
	EndRenderColor();

	RenderSkillShopFrame(shopX, shopY, shopW, shopH, 0.23f, 0.20f, 0.14f, 0.80f);
	RenderSkillShopFrame(shopX + 6.f, shopY + 6.f, shopW - 12.f, shopH - 12.f, 0.05f, 0.05f, 0.05f, 0.85f);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 172, 25, 255);
	g_pRenderText->RenderText((int)shopX + 14, (int)shopY + 17, "Skill Shop System", (int)shopW - 28, 0, RT3_SORT_CENTER);

	bool closeHover = SEASON3B::CheckMouseIn(shopX + shopW - 29.f, shopY + 11.f, 22.f, 22.f);
	float closeV = closeHover ? 0.2123746f : 0.0f;
	RenderBitmap(CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN, shopX + shopW - 29.f, shopY + 11.f, 22.f, 22.f, 0.018f, closeV, 0.812731823f, 0.2023746f, 1, 1, 1.0f);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(221, 221, 221, 255);
	g_pRenderText->RenderText((int)shopX + 20, (int)shopY + 40, "Skills", (int)shopW - 40, 0, RT3_SORT_CENTER);

	std::vector<int> skillList;
	BuildSkillShopSkillList(skillList);
	int maxSkillPage = 0;
	if(skillList.size() > 0)
	{
		maxSkillPage = (int)((skillList.size() - 1) / SKILL_SHOP_SKILLS_PER_PAGE);
	}

	if(m_SkillPage > maxSkillPage)
	{
		m_SkillPage = maxSkillPage;
	}

	const int skillStart = m_SkillPage * SKILL_SHOP_SKILLS_PER_PAGE;
	int hoverDisabledSkillType = -1;
	for(int n = 0; n < SKILL_SHOP_SKILLS_PER_PAGE; ++n)
	{
		int listIndex = skillStart + n;
		if(listIndex >= (int)skillList.size())
		{
			break;
		}

		int skillEntry = skillList[listIndex];
		bool skillDisabled = IsSkillShopDisabledEntry(skillEntry);
		int skillIndex = GetSkillShopEntrySkillIndex(skillEntry);
		int skillIconType = GetSkillShopEntrySkillType(skillEntry);
		float cellX = shopX + SKILL_SHOP_SKILL_START_X + ((n % SKILL_SHOP_SKILL_COLUMNS) * SKILL_SHOP_CELL_STEP_X);
		float cellY = shopY + SKILL_SHOP_SKILL_START_Y + ((n / SKILL_SHOP_SKILL_COLUMNS) * SKILL_SHOP_CELL_STEP_Y);

		RenderImage(IMAGE_SKILLBOX, cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT);

		if(skillDisabled == false && skillIndex == Hero->CurrentSkill)
		{
			RenderSkillShopFrame(cellX + 2.f, cellY + 2.f, SKILL_SHOP_CELL_WIDTH - 4.f, SKILL_SHOP_CELL_HEIGHT - 4.f, 1.f, 0.66f, 0.12f, 0.75f);
		}

		if(SEASON3B::CheckMouseIn(cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT) == true)
		{
			RenderSkillShopFrame(cellX + 2.f, cellY + 2.f, SKILL_SHOP_CELL_WIDTH - 4.f, SKILL_SHOP_CELL_HEIGHT - 4.f, 1.f, 0.58f, 0.16f, 0.70f);
			if(skillDisabled == true)
			{
				hoverDisabledSkillType = skillIconType;
			}
		}

		glColor4f(skillDisabled ? 0.38f : 1.f, skillDisabled ? 0.38f : 1.f, skillDisabled ? 0.38f : 1.f, 1.f);
		RenderSkillShopIconBySkillType(skillIconType, cellX + 6.f, cellY + 5.f, SKILL_SHOP_ICON_WIDTH, SKILL_SHOP_ICON_HEIGHT);
		glColor4f(1.f, 1.f, 1.f, 1.f);

		if(skillDisabled == true)
		{
			EnableAlphaTest();
			glColor4f(0.f, 0.f, 0.f, 0.42f);
			RenderColor(cellX + 5.f, cellY + 4.f, SKILL_SHOP_ICON_WIDTH + 2.f, SKILL_SHOP_ICON_HEIGHT + 2.f);
			EndRenderColor();
		}
		else
		{
			const int hotKeySlot = GetSkillShopHotKeySlot(m_iHotKeySkillType, skillIndex);
			if(hotKeySlot != -1)
			{
				glColor3f(1.f, 0.9f, 0.8f);
				SEASON3B::RenderNumber(cellX + 8.f, cellY + 5.f, hotKeySlot, 1.f);
				glColor3f(1.f, 1.f, 1.f);
			}
		}
	}

	if(hoverDisabledSkillType > 0)
	{
		RenderSkillShopSkillTypeTooltip(hoverDisabledSkillType, shopX, shopY, shopW, shopH);
	}

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 124, 18, 255);
	g_pRenderText->RenderText((int)shopX + 57, (int)shopY + (int)SKILL_SHOP_SKILL_PAGE_Y, "<", 18, 0, RT3_SORT_CENTER);
	char pageText[16] = { 0 };
	_snprintf(pageText, sizeof(pageText), "%d / %d", m_SkillPage + 1, maxSkillPage + 1);
	g_pRenderText->RenderText((int)shopX + 74, (int)shopY + (int)SKILL_SHOP_SKILL_PAGE_Y, pageText, 28, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText((int)shopX + 99, (int)shopY + (int)SKILL_SHOP_SKILL_PAGE_Y, ">", 18, 0, RT3_SORT_CENTER);

	g_pRenderText->SetTextColor(255, 185, 35, 255);
	g_pRenderText->RenderText((int)shopX + 18, (int)shopY + 206, "Here you can buy buffs", (int)shopW - 36, 0, RT3_SORT_CENTER);

	if(m_SkillShopConfigLoaded == false)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetTextColor(180, 180, 180, 255);
		g_pRenderText->RenderText((int)shopX + 20, (int)shopY + 258, "Loading...", (int)shopW - 40, 0, RT3_SORT_CENTER);
		return;
	}

	if(m_SkillShopBuffCount <= 0)
	{
		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetTextColor(180, 180, 180, 255);
		g_pRenderText->RenderText((int)shopX + 20, (int)shopY + 258, "No buffs configured", (int)shopW - 40, 0, RT3_SORT_CENTER);
		return;
	}

	int maxBuffPage = (int)((m_SkillShopBuffCount - 1) / SKILL_SHOP_BUFFS_PER_PAGE);
	if(m_SkillShopBuffPage > maxBuffPage)
	{
		m_SkillShopBuffPage = maxBuffPage;
	}

	const int buffStart = m_SkillShopBuffPage * SKILL_SHOP_BUFFS_PER_PAGE;
	int hoverBuff = -1;
	for(int n = 0; n < SKILL_SHOP_BUFFS_PER_PAGE; ++n)
	{
		int listIndex = buffStart + n;
		if(listIndex >= m_SkillShopBuffCount)
		{
			break;
		}

		SKILL_SHOP_BUFF_INFO* lpInfo = &m_SkillShopBuffs[listIndex];
		float cellX = shopX + SKILL_SHOP_BUFF_START_X + ((n % SKILL_SHOP_BUFF_COLUMNS) * SKILL_SHOP_CELL_STEP_X);
		float cellY = shopY + SKILL_SHOP_BUFF_START_Y + ((n / SKILL_SHOP_BUFF_COLUMNS) * SKILL_SHOP_CELL_STEP_Y);
		bool hover = (lpInfo->Enabled != 0 && SEASON3B::CheckMouseIn(cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT));

		RenderImage(IMAGE_SKILLBOX, cellX, cellY, SKILL_SHOP_CELL_WIDTH, SKILL_SHOP_CELL_HEIGHT);

		if(hover)
		{
			RenderSkillShopFrame(cellX + 2.f, cellY + 2.f, SKILL_SHOP_CELL_WIDTH - 4.f, SKILL_SHOP_CELL_HEIGHT - 4.f, 1.f, 0.58f, 0.16f, 0.70f);
			hoverBuff = listIndex;
		}

		if(lpInfo->Enabled == 0)
		{
			glColor4f(0.35f, 0.35f, 0.35f, 1.0f);
		}
		else
		{
			glColor4f(1.f, 1.f, 1.f, 1.f);
		}

		RenderSkillShopBuffIconByEffect(lpInfo->Effect, cellX + 6.f, cellY + 5.f, SKILL_SHOP_ICON_WIDTH, SKILL_SHOP_ICON_HEIGHT);
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 124, 18, 255);
	g_pRenderText->RenderText((int)shopX + 57, (int)shopY + (int)SKILL_SHOP_BUFF_PAGE_Y, "<", 18, 0, RT3_SORT_CENTER);
	_snprintf(pageText, sizeof(pageText), "%d / %d", m_SkillShopBuffPage + 1, maxBuffPage + 1);
	g_pRenderText->RenderText((int)shopX + 74, (int)shopY + (int)SKILL_SHOP_BUFF_PAGE_Y, pageText, 28, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText((int)shopX + 99, (int)shopY + (int)SKILL_SHOP_BUFF_PAGE_Y, ">", 18, 0, RT3_SORT_CENTER);

	RenderSkillShopBuffTooltip(hoverBuff);
#endif
}

void SEASON3B::CNewUISkillList::RenderSkillInfo()
{
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
	::RenderSkillInfo(m_iRenderSkillInfoPosX+15, m_iRenderSkillInfoPosY-10, m_iRenderSkillInfoType);
#elif CLIENT_VERSION >= 1001
	::RenderSkillInfo(m_iRenderSkillInfoPosX+15, m_iRenderSkillInfoPosY-25, m_iRenderSkillInfoType);
#endif
}

void SEASON3B::CNewUISkillList::RenderSkillShopBuffTooltip(int buffIndex)
{
#if CLIENT_VERSION >= 1001
	if(buffIndex < 0 || buffIndex >= m_SkillShopBuffCount)
	{
		return;
	}

	SKILL_SHOP_BUFF_INFO* lpInfo = &m_SkillShopBuffs[buffIndex];
	const float tooltipW = 154.f;
	const float tooltipH = 64.f;
	float tooltipX = (float)MouseX + 14.f;
	float tooltipY = (float)MouseY + 16.f;

	if(tooltipX + tooltipW > gWideScreen.g_WideWindowWidth - 4.f)
	{
		tooltipX = (float)MouseX - tooltipW - 12.f;
	}

	if(tooltipY + tooltipH > gWideScreen.g_WideWindowHeight - 4.f)
	{
		tooltipY = (float)MouseY - tooltipH - 12.f;
	}

	if(tooltipX < 4.f) { tooltipX = 4.f; }
	if(tooltipY < 4.f) { tooltipY = 4.f; }

	char desc[96] = { 0 };
	char timeText[32] = { 0 };
	char duration[48] = { 0 };
	char price[32] = { 0 };
	char priceText[56] = { 0 };

	GetSkillShopBuffDescription(lpInfo->Effect, lpInfo->Value1, lpInfo->Value2, lpInfo->Value3, lpInfo->Value4, desc, sizeof(desc));
	FormatSkillShopDuration(lpInfo->Time, timeText, sizeof(timeText));
	FormatSkillShopPrice(lpInfo->Price, price, sizeof(price));
	_snprintf(duration, sizeof(duration), "Duracao: %s", timeText);
	if(lpInfo->Currency == 6 && lpInfo->Price == 1)
	{
		_snprintf(priceText, sizeof(priceText), "Preco: 1 VCoin / 2500 VGold");
	}
	else
	{
		_snprintf(priceText, sizeof(priceText), "Preco: %s %s", price, GetSkillShopCurrencyName(lpInfo->Currency));
	}
	duration[sizeof(duration) - 1] = '\0';
	priceText[sizeof(priceText) - 1] = '\0';

	EnableAlphaTest();
	glColor4f(0.f, 0.f, 0.f, 0.82f);
	RenderColor(tooltipX, tooltipY, tooltipW, tooltipH);
	glColor4f(0.21f, 0.10f, 0.04f, 0.92f);
	RenderColor(tooltipX, tooltipY, tooltipW, 15.f);
	EndRenderColor();

	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 185, 35, 255);
	g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 3, lpInfo->Name, (int)tooltipW - 12, 0, 0);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(230, 230, 230, 255);
	g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 22, desc, (int)tooltipW - 12, 0, 0);
	g_pRenderText->SetTextColor(190, 210, 255, 255);
	g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 36, duration, (int)tooltipW - 12, 0, 0);
	g_pRenderText->SetTextColor(230, 205, 135, 255);
	g_pRenderText->RenderText((int)tooltipX + 6, (int)tooltipY + 50, priceText, (int)tooltipW - 12, 0, 0);
#endif
}

float SEASON3B::CNewUISkillList::GetLayerDepth()
{
	return 2.2f;
}

WORD SEASON3B::CNewUISkillList::GetHeroPriorSkill()
{
	return m_wHeroPriorSkill;
}

void SEASON3B::CNewUISkillList::SetHeroPriorSkill(BYTE bySkill)
{
	m_wHeroPriorSkill = bySkill;
}

// RENDERIZA SKILL DE PET
void SEASON3B::CNewUISkillList::RenderPetSkill()
{
	return;
}

void SEASON3B::CNewUISkillList::RenderSkillShopBuffIconByEffect(int iEffect, float x, float y, float width, float height)
{
	if(iEffect <= 0)
	{
		return;
	}

	int customBuffBitmap = GetSkillShopCustomBuffBitmap(iEffect);
	if(customBuffBitmap != 0)
	{
		RenderBitmap(customBuffBitmap, x, y, width, height, 0.f, 0.f, 1.f, 1.f);
		return;
	}

#if CLIENT_VERSION == 1002
	float sourceHeight = (float)((28.0f / 100) * 64.3f);
	float sourceWidth = (float)((20.0f / 100) * 64.3f);
	float renderWidth = (float)((256.0f / 100) * 64.3f);

	if(iEffect < 81)
	{
		int effectValue = (iEffect - 1) % 80;
		float sourceX = (float)(effectValue % 10) * sourceWidth / renderWidth;
		float sourceY = (float)(effectValue / 10) * sourceHeight / renderWidth;
		RenderBitmap((iEffect - 1) / 80 + 31719, x, y, width, height, sourceX, sourceY, sourceWidth / renderWidth, sourceHeight / renderWidth, 1, 1, 0);
	}
	else
	{
		int effectValue = (iEffect - 81) % 81;
		float sourceX = (float)(effectValue % 10) * sourceWidth / renderWidth;
		float sourceY = (float)(effectValue / 10) * sourceHeight / renderWidth;
		RenderBitmap((iEffect - 81) / 81 + 31720, x, y, width, height, sourceX, sourceY, sourceWidth / renderWidth, sourceHeight / renderWidth, 1, 1, 0);
	}
#else
	if(iEffect < 81)
	{
		int widthIndex = (iEffect - 1) % 10;
		int heightIndex = (iEffect - 1) / 10;
		float u = widthIndex * width / 256.f;
		float v = heightIndex * height / 256.f;
		RenderBitmap(BITMAP_BUFFWINDOW_BEGIN, x, y, width, height, u, v, width / 256.f, height / 256.f);
	}
	else
	{
		int widthIndex = (iEffect - 81) % 10;
		int heightIndex = (iEffect - 81) / 10;
		float u = widthIndex * width / 256.f;
		float v = heightIndex * height / 256.f;
		RenderBitmap(BITMAP_BUFFWINDOW_BEGIN + 1, x, y, width, height, u, v, width / 256.f, height / 256.f);
	}
#endif
}

void SEASON3B::CNewUISkillList::RenderSkillShopIconBySkillType(int iSkillType, float x, float y, float width, float height)
{
	WORD bySkillType = (WORD)iSkillType;

	if(bySkillType == 0)
	{
		return;
	}

	if(bySkillType == AT_SKILL_LONGPIER_ATTACK)
	{
		RenderBitmap(BITMAP_DRAGON_SKILL_ICON, x, y, width, height, 0.f, 0.f, 1.f, 1.f);
		return;
	}

	BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;
	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;
	float fU = 0.f;
	float fV = 0.f;
	int iKindofSkill = 0;

	if(bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_END)
	{
		fU = ((bySkillType - AT_PET_COMMAND_DEFAULT) % 8) * width / 256.f;
		fV = ((bySkillType - AT_PET_COMMAND_DEFAULT) / 8) * height / 256.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if(bySkillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		fU = 4 * width / 256.f;
		fV = 0.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if((bySkillType >= AT_SKILL_ALICE_DRAINLIFE && bySkillType <= AT_SKILL_ALICE_THORNS))
	{
		fU = ((bySkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType >= AT_SKILL_ALICE_SLEEP && bySkillType <= AT_SKILL_ALICE_BLIND)
	{
		fU = ((bySkillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_ALICE_BERSERKER)
	{
		fU = 10 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType >= AT_SKILL_ALICE_WEAKNESS && bySkillType <= AT_SKILL_ALICE_ENERVATION)
	{
		fU = (bySkillType - AT_SKILL_ALICE_WEAKNESS + 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType >= AT_SKILL_SUMMON_EXPLOSION && bySkillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		fU = ((bySkillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_SUMMON_POLLUTION)
	{
		fU = 11 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		fU = 7 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_GAOTIC)
	{
		fU = 3 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_RECOVER)
	{
		fU = 9 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_MULTI_SHOT)
	{
		fU = 0 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		fU = 1 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_GIGANTIC_STORM)
	{
		fU = 2 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		fU = 2 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(AT_SKILL_LIGHTNING_SHOCK_UP <= bySkillType && bySkillType <= AT_SKILL_LIGHTNING_SHOCK_UP+4)
	{
		fU = 6 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER)
	{
		fU = 8 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillUseType == 4)
	{
		fU = (width / 256.f) * (Skill_Icon % 12);
		fV = (height / 256.f) * ((Skill_Icon / 12) + 4);
		iKindofSkill = KOS_SKILL2;
	}
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	else if(bySkillType >= AT_SKILL_THRUST)
	{
		fU = ((bySkillType - 260) % 12) * width / 256.f;
		fV = ((bySkillType - 260) / 12) * height / 256.f;
		iKindofSkill = KOS_SKILL3;
	}
#endif
	else if(bySkillType >= 57)
	{
		fU = ((bySkillType - 57) % 8) * width / 256.f;
		fV = ((bySkillType - 57) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else
	{
		fU = ((bySkillType - 1) % 8) * width / 256.f;
		fV = ((bySkillType - 1) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL1;
	}

	int iSkillIndex = 0;
	switch(iKindofSkill)
	{
	case KOS_COMMAND:
		iSkillIndex = IMAGE_COMMAND;
		break;
	case KOS_SKILL1:
		iSkillIndex = IMAGE_SKILL1;
		break;
	case KOS_SKILL2:
		iSkillIndex = IMAGE_SKILL2;
		break;
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	case KOS_SKILL3:
		iSkillIndex = IMAGE_SKILL3;
		break;
#endif
	}

	if(iSkillIndex == 0)
	{
		return;
	}

	const float inset = 0.5f / 256.f;
	RenderBitmap(iSkillIndex, x, y, width, height, fU + inset, fV + inset, (width / 256.f) - (inset * 2.f), (height / 256.f) - (inset * 2.f));
}

void SEASON3B::CNewUISkillList::RenderSkillIcon(int iIndex, float x, float y, float width, float height)
{
	WORD bySkillType = 0;

	if(iIndex >= AT_PET_COMMAND_DEFAULT && iIndex < AT_PET_COMMAND_END)
	{
		bySkillType = iIndex;
	}
	else
	{
		bySkillType = CharacterAttribute->Skill[iIndex];
	}

	if(bySkillType == 0)
	{
		return;
	}

	const bool useDragonSkillIcon = (bySkillType == AT_SKILL_LONGPIER_ATTACK);

	if(IsClassicSkillBarSkillType(bySkillType) == false)
	{
		return;
	}
	bool bCantSkill = false;

    BYTE bySkillUseType = SkillAttribute[bySkillType].SkillUseType;
	int Skill_Icon = SkillAttribute[bySkillType].Magic_Icon;

 	if( !gSkillManager.DemendConditionCheckSkill(bySkillType))
 	{
		bCantSkill = true;
	}

	if(IsCanBCSkill(bySkillType) == false)
	{
		bCantSkill = true;
	}


	if(bySkillType >= AT_SKILL_BLOCKING && bySkillType <= AT_SKILL_SWORD5 && (Hero->Helper.Type == MODEL_HELPER+2 || Hero->Helper.Type == MODEL_HELPER+3 || Hero->Helper.Type == MODEL_HELPER+37))
	{
		bCantSkill = true;
	}

	if((bySkillType == AT_SKILL_ICE_BLADE ||(AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP+4 >= bySkillType)) && (Hero->Helper.Type == MODEL_HELPER+2 || Hero->Helper.Type == MODEL_HELPER+3 || Hero->Helper.Type == MODEL_HELPER+37))
	{
		bCantSkill = true;
	}
                   
    int iEnergy = CharacterAttribute->Energy+CharacterAttribute->AddEnergy;

    if(g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
	{
		bCantSkill = true;
	}

    if(bySkillType == AT_SKILL_PARTY_TELEPORT && PartyNumber <= 0)
    {
		bCantSkill = true;
    }

	if (bySkillType == AT_SKILL_PARTY_TELEPORT && (IsDoppelGanger1() || IsDoppelGanger2() || IsDoppelGanger3() || IsDoppelGanger4()))
	{
		bCantSkill = true;
	}

	if(bySkillType == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP+4))
	{
		BYTE byDarkHorseLife = 0;
		byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
		bool isDarkHorse = (Hero->Helper.Type == MODEL_HELPER + 4 || gHelperSystem.CheckHelperType(Hero->Helper.Type, 16));
		if(!isDarkHorse || (byDarkHorseLife == 0 && Hero->Helper.Type == MODEL_HELPER + 4))
		{
			bCantSkill = true;
		}
	}
#ifdef PJH_FIX_SPRIT
/*������*/
	if( bySkillType>=AT_PET_COMMAND_DEFAULT && bySkillType<AT_PET_COMMAND_END )
	{
		int iCharisma = CharacterAttribute->Charisma+CharacterAttribute->AddCharisma;
		PET_INFO PetInfo;
		giPetManager::GetPetInfo(PetInfo, 421-PET_TYPE_DARK_SPIRIT);
		int RequireCharisma = (185+(PetInfo.m_wLevel*15));
		if( RequireCharisma > iCharisma ) 
		{
			bCantSkill = true;
		}
	}
#endif //PJH_FIX_SPRIT
	if( (bySkillType == AT_SKILL_INFINITY_ARROW) || (bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER))
	{
		if(g_csItemOption.IsDisableSkill(bySkillType, iEnergy))
		{
			bCantSkill = true;
		}
		if( ( g_isCharacterBuff((&Hero->Object), eBuff_InfinityArrow) )	|| ( g_isCharacterBuff((&Hero->Object), eBuff_SwellOfMagicPower)))
		{
			bCantSkill = true;
		}
	}

	if( bySkillType == AT_SKILL_REDUCEDEFENSE || (AT_SKILL_BLOOD_ATT_UP <= bySkillType && bySkillType <= AT_SKILL_BLOOD_ATT_UP+4))
	{
		WORD Strength;
		const WORD wRequireStrength = 596;
		Strength = CharacterAttribute->Strength + CharacterAttribute->AddStrength;
		if(Strength < wRequireStrength)
		{
			bCantSkill = true;
		}
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;
		
		if ( !( iTypeR!=-1 && ( iTypeR<ITEM_STAFF || iTypeR>=ITEM_STAFF+MAX_ITEM_INDEX ) &&	( iTypeL<ITEM_STAFF || iTypeL>=ITEM_STAFF+MAX_ITEM_INDEX ) ) )
		{
			bCantSkill = true;
		}
	}
	
	switch( bySkillType )
	{
	//case AT_SKILL_PIERCING:
	case AT_SKILL_PARALYZE:
		{
			WORD  Dexterity;
			const WORD wRequireDexterity = 646;
			Dexterity = CharacterAttribute->Dexterity + CharacterAttribute->AddDexterity;
			if(Dexterity < wRequireDexterity)
			{
				bCantSkill = true;
			}
		}break;
	}

	if( bySkillType == AT_SKILL_WHEEL || (AT_SKILL_TORNADO_SWORDA_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDA_UP+4)	|| (AT_SKILL_TORNADO_SWORDB_UP <= bySkillType && bySkillType <= AT_SKILL_TORNADO_SWORDB_UP+4)
		)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;
		
		
		if ( !( iTypeR!=-1 && ( iTypeR<ITEM_STAFF || iTypeR>=ITEM_STAFF+MAX_ITEM_INDEX ) && ( iTypeL<ITEM_STAFF || iTypeL>=ITEM_STAFF+MAX_ITEM_INDEX ) ) )
		{
			bCantSkill = true;
		}
	}

	if(gMapManager.InChaosCastle() == true)
	{
		if( bySkillType == AT_SKILL_DARK_HORSE || bySkillType == AT_SKILL_RIDER	|| (bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_TARGET) ||(AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP+4))
		{
			bCantSkill = true;
		}
	}
	else
	{
		if(bySkillType == AT_SKILL_DARK_HORSE || (AT_SKILL_ASHAKE_UP <= bySkillType && bySkillType <= AT_SKILL_ASHAKE_UP+4))
		{
			BYTE byDarkHorseLife = 0;
			byDarkHorseLife = CharacterMachine->Equipment[EQUIPMENT_HELPER].Durability;
			if(byDarkHorseLife == 0) 
			{
				bCantSkill = true;
			}
		}
	}

	int iCharisma = CharacterAttribute->Charisma+CharacterAttribute->AddCharisma;

	if(g_csItemOption.IsDisableSkill(bySkillType, iEnergy, iCharisma))
	{
		bCantSkill = true;
	}

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if(!g_CMonkSystem.IsSwordformGlovesUseSkill(bySkillType))
	{
		bCantSkill = true;
	}
	if(g_CMonkSystem.IsRideNotUseSkill(bySkillType, Hero->Helper.Type))
	{
		bCantSkill = true;
	}

	ITEM* pLeftRing = &CharacterMachine->Equipment[EQUIPMENT_RING_LEFT];
	ITEM* pRightRing = &CharacterMachine->Equipment[EQUIPMENT_RING_RIGHT];
	
	if(g_CMonkSystem.IsChangeringNotUseSkill(pLeftRing->Type, pRightRing->Type, pLeftRing->Level, pRightRing->Level)
		&& (GetBaseClass(Hero->Class) == CLASS_RAGEFIGHTER))
	{
		bCantSkill = true;
	}
#endif //PBG_ADD_NEWCHAR_MONK_SKILL

	if(bySkillType == AT_SKILL_PLASMA_STORM_FENRIR && IsFenrirEquippedForPlasmaStorm())
	{
		bCantSkill = false;
	}

	if(bySkillType == AT_SKILL_BLOCKING && IsShieldEquippedForBlocking())
	{
		bCantSkill = false;
	}
	float fU, fV;
	int iKindofSkill = 0;
	
	if(g_csItemOption.Special_Option_Check() == false && (bySkillType == AT_SKILL_ICE_BLADE||(AT_SKILL_POWER_SLASH_UP <= bySkillType && AT_SKILL_POWER_SLASH_UP+4 >= bySkillType)))
	{
		bCantSkill = true;
	}


	if(g_csItemOption.Special_Option_Check(1) == false && (bySkillType == AT_SKILL_CROSSBOW||(AT_SKILL_MANY_ARROW_UP <= bySkillType && AT_SKILL_MANY_ARROW_UP+4 >= bySkillType)))
		bCantSkill = true;

	if(bySkillType >= AT_PET_COMMAND_DEFAULT && bySkillType <= AT_PET_COMMAND_END)
    {
		fU = ((bySkillType - AT_PET_COMMAND_DEFAULT) % 8) * width / 256.f;
		fV = ((bySkillType - AT_PET_COMMAND_DEFAULT) / 8) * height / 256.f;
		iKindofSkill = KOS_COMMAND;
    }
    else if(bySkillType == AT_SKILL_PLASMA_STORM_FENRIR)
	{
		fU = 4 * width / 256.f;
		fV = 0.f;
		iKindofSkill = KOS_COMMAND;
	}
	else if((bySkillType >= AT_SKILL_ALICE_DRAINLIFE && bySkillType <= AT_SKILL_ALICE_THORNS))
	{
		fU = ((bySkillType - AT_SKILL_ALICE_DRAINLIFE) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType >= AT_SKILL_ALICE_SLEEP && bySkillType <= AT_SKILL_ALICE_BLIND)
	{
		fU = ((bySkillType - AT_SKILL_ALICE_SLEEP + 4) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_ALICE_BERSERKER)
	{
		fU = 10 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType >= AT_SKILL_ALICE_WEAKNESS && bySkillType <= AT_SKILL_ALICE_ENERVATION)
	{
		fU = (bySkillType - AT_SKILL_ALICE_WEAKNESS + 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillType >= AT_SKILL_SUMMON_EXPLOSION && bySkillType <= AT_SKILL_SUMMON_REQUIEM)
	{
		fU = ((bySkillType - AT_SKILL_SUMMON_EXPLOSION + 6) % 8) * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_SUMMON_POLLUTION)
	{
		fU = 11 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_BLOW_OF_DESTRUCTION)
	{
		fU = 7 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_GAOTIC)
	{
		fU = 3 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_RECOVER)
	{
		fU = 9 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_MULTI_SHOT)
	{
		if (gCharacterManager.GetEquipedBowType_Skill() == BOWTYPE_NONE)
		{
			bCantSkill = true;
		}

		fU = 0 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_FLAME_STRIKE)
	{
		int iTypeL = CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT].Type;
		int iTypeR = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type;
		
		if ( !( iTypeR!=-1 && ( iTypeR<ITEM_STAFF || iTypeR>=ITEM_STAFF+MAX_ITEM_INDEX ) &&	( iTypeL<ITEM_STAFF || iTypeL>=ITEM_STAFF+MAX_ITEM_INDEX ) ) )
		{
			bCantSkill = true;
		}

		fU = 1 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_GIGANTIC_STORM)
	{
		fU = 2 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if (bySkillType == AT_SKILL_LIGHTNING_SHOCK)
	{
		fU = 2 * width / 256.f;
		fV = 3 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(AT_SKILL_LIGHTNING_SHOCK_UP <= bySkillType && bySkillType <= AT_SKILL_LIGHTNING_SHOCK_UP+4)
	{
		fU = 6 * width / 256.f;
		fV = 8 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if( bySkillType == AT_SKILL_SWELL_OF_MAGICPOWER )
	{
		fU = 8 * width / 256.f;
		fV = 2 * height / 256.f;
		iKindofSkill = KOS_SKILL2;
	}
	else if(bySkillUseType == 4)
	{
		fU = (width/256.f) * (Skill_Icon%12);
		fV = (height/256.f)*((Skill_Icon/12)+4);
		iKindofSkill = KOS_SKILL2;
	}
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	else if(bySkillType >= AT_SKILL_THRUST)
	{
		fU = ((bySkillType - 260) % 12) * width / 256.f;
		fV = ((bySkillType - 260) / 12) * height / 256.f;
		iKindofSkill = KOS_SKILL3;
	}
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
	else if(bySkillType >= 57)
    {
		fU = ((bySkillType - 57) % 8) * width / 256.f;
		fV = ((bySkillType - 57) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL2;
    }
    else
    {
		fU = ((bySkillType - 1) % 8) * width / 256.f;
		fV = ((bySkillType - 1) / 8) * height / 256.f;
		iKindofSkill = KOS_SKILL1;
    }
	int iSkillIndex = 0;
	switch(iKindofSkill)
	{
	case KOS_COMMAND:
		{
			iSkillIndex = IMAGE_COMMAND;
		}break;
	case KOS_SKILL1:
		{
			iSkillIndex = IMAGE_SKILL1;
		}break;
	case KOS_SKILL2:
		{
			iSkillIndex = IMAGE_SKILL2;
		}break;
	}

	if(useDragonSkillIcon)
	{
		iSkillIndex = BITMAP_DRAGON_SKILL_ICON;
	}

	const bool bUseDIVSkillIcons = false;
	if (bUseDIVSkillIcons == true)
	{
		switch (iKindofSkill)
		{
		case KOS_COMMAND:
			iSkillIndex = DIV_COMMAND_SKILL;
			break;
		case KOS_SKILL1:
			iSkillIndex = DIV_SKILL1;
			break;
		case KOS_SKILL2:
			iSkillIndex = DIV_SKILL2;
			break;
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		case KOS_SKILL3:
			iSkillIndex = DIV_SKILL3;
			break;
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
		}
	}
	else if( bCantSkill == true && g_DIVInterfaceReady == false && useDragonSkillIcon == false )
	{
		iSkillIndex += 5;
	}
	
	if( iSkillIndex != 0)
	{
		float fSkillIconU = fU;
		float fSkillIconV = fV;
		float fSkillIconWidth = width / 256.f;
		float fSkillIconHeight = height / 256.f;
		const float fSkillIconInset = 0.5f / 256.f;

		if(useDragonSkillIcon)
		{
			fSkillIconU = 0.f;
			fSkillIconV = 0.f;
			fSkillIconWidth = 1.f;
			fSkillIconHeight = 1.f;
		}
		else if (bUseDIVSkillIcons == true)
		{
			const int iSkillCellX = (int)((fU * 256.f / width) + 0.5f);
			const int iSkillCellY = (int)((fV * 256.f / height) + 0.5f);
			const float fDIVInset = 0.5f / 512.f;

			fSkillIconU = ((float)(iSkillCellX % 16) / 16.f) + fDIVInset;
			fSkillIconV = ((float)(iSkillCellY % 16) / 16.f) + fDIVInset;
			fSkillIconWidth = (1.f / 16.f) - (fDIVInset * 2.f);
			fSkillIconHeight = (1.f / 16.f) - (fDIVInset * 2.f);
		}
		else
		{
			fSkillIconU += fSkillIconInset;
			fSkillIconV += fSkillIconInset;
			fSkillIconWidth -= fSkillIconInset * 2.f;
			fSkillIconHeight -= fSkillIconInset * 2.f;
		}
		float fRenderX = x;
		float fRenderY = y;
		float fRenderWidth = width;
		float fRenderHeight = height;

		if (g_DIVInterfaceReady == true && g_DIVSkillBarRendering == true)
		{
			fRenderWidth = 23.f;
			fRenderHeight = 23.f;
			RenderBitmap(iSkillIndex, fRenderX, fRenderY, fRenderWidth, fRenderHeight, fSkillIconU, fSkillIconV, fSkillIconWidth, fSkillIconHeight);
		}
		else
		{
			const float fSquareSize = height;
			fRenderX = x + ((width - fSquareSize) * 0.5f);
			fRenderWidth = fSquareSize;
			fRenderHeight = fSquareSize;
			RenderBitmap(iSkillIndex, fRenderX, fRenderY, fRenderWidth, fRenderHeight, fSkillIconU, fSkillIconV, fSkillIconWidth, fSkillIconHeight);
		}

		if( (bySkillType != AT_SKILL_INFINITY_ARROW) && (bySkillType != AT_SKILL_SWELL_OF_MAGICPOWER))
		{
			RenderSkillDelay(iIndex, fRenderX, fRenderY, fRenderWidth, fRenderHeight);
		}
	}

	int iHotKey = -1;
	for(int i=0; i<SKILLHOTKEY_COUNT; ++i)
	{
		if(m_iHotKeySkillType[i] == iIndex)
		{
			iHotKey = i;
			break;
		}
	}

	if(iHotKey != -1 && g_DIVSkillBarRendering == false)
	{
		glColor3f(1.f, 0.9f, 0.8f);
		SEASON3B::RenderNumber(x+20, y+20, iHotKey);
	}

#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
	if((bySkillType == AT_SKILL_GIANTSWING || bySkillType == AT_SKILL_DRAGON_KICK
		|| bySkillType == AT_SKILL_DRAGON_LOWER) && (bCantSkill))
		return;
#endif //PBG_ADD_NEWCHAR_MONK_SKILL

}

void SEASON3B::CNewUISkillList::RenderSkillDelay(int iIndex, float x, float y, float width, float height)
{
	if(iIndex >= AT_PET_COMMAND_DEFAULT && iIndex < AT_PET_COMMAND_END)
	{
		return;
	}

	if(IsClassicSkillBarEntry(iIndex) == false)
	{
		return;
	}

    int iSkillDelay = CharacterAttribute->SkillDelay[iIndex];
    if ( iSkillDelay > 0 )
    {
        int iSkillType  = CharacterAttribute->Skill[iIndex];
		
		if(iSkillType == AT_SKILL_PLASMA_STORM_FENRIR)
		{
			if(!CheckAttack())
			{
				return;
			}
		}

        int iSkillMaxDelay = SkillAttribute[iSkillType].Delay;
		if (iSkillMaxDelay <= 0)
		{
			return;
		}
		
        float fPersent = (float)(iSkillDelay/(float)iSkillMaxDelay);
        
		EnableAlphaTest();
        glColor4f(1.f, 0.5f, 0.5f, 0.5f);
        float fdeltaH = height * fPersent;
        RenderColor(x, y+height-fdeltaH, width, fdeltaH);
		EndRenderColor();
    }
}

bool SEASON3B::CNewUISkillList::IsSkillListUp()
{
	return m_bHotKeySkillListUp;
}

#if CLIENT_VERSION >= 1001
bool SEASON3B::CNewUISkillList::IsSkillShopOpen()
{
	return m_bSkillList;
}

void SEASON3B::CNewUISkillList::CloseSkillShop()
{
	m_bSkillList = false;
	m_SkillShopDragging = false;
	m_EventState = EVENT_NONE;
}

#endif

void SEASON3B::CNewUISkillList::ReceiveSkillShopList(BYTE* lpMsg)
{
#if CLIENT_VERSION >= 1001
	if(lpMsg == NULL)
	{
		return;
	}

	PMSG_CLIENT_SKILL_SHOP_LIST_RECV* lpList = (PMSG_CLIENT_SKILL_SHOP_LIST_RECV*)lpMsg;
	int packetSize = (lpMsg[0] == 0xC2) ? MAKE_NUMBERW(lpMsg[1], lpMsg[2]) : lpMsg[1];
	int offset = sizeof(PMSG_CLIENT_SKILL_SHOP_LIST_RECV);
	int count = lpList->count;

	if(count > SKILL_SHOP_MAX_BUFF)
	{
		count = SKILL_SHOP_MAX_BUFF;
	}

	m_SkillShopBuffCount = 0;
	memset(m_SkillShopBuffs, 0, sizeof(m_SkillShopBuffs));

	for(int n = 0; n < count; ++n)
	{
		if((offset + (int)sizeof(SKILL_SHOP_BUFF_INFO)) > packetSize)
		{
			break;
		}

		memcpy(&m_SkillShopBuffs[m_SkillShopBuffCount], &lpMsg[offset], sizeof(SKILL_SHOP_BUFF_INFO));
		m_SkillShopBuffs[m_SkillShopBuffCount].Name[sizeof(m_SkillShopBuffs[m_SkillShopBuffCount].Name) - 1] = '\0';

		if(m_SkillShopBuffs[m_SkillShopBuffCount].Name[0] == '\0')
		{
			_snprintf(m_SkillShopBuffs[m_SkillShopBuffCount].Name, sizeof(m_SkillShopBuffs[m_SkillShopBuffCount].Name), "Skill %d", m_SkillShopBuffs[m_SkillShopBuffCount].Skill);
			m_SkillShopBuffs[m_SkillShopBuffCount].Name[sizeof(m_SkillShopBuffs[m_SkillShopBuffCount].Name) - 1] = '\0';
		}

		m_SkillShopBuffCount++;
		offset += sizeof(SKILL_SHOP_BUFF_INFO);
	}

	m_SkillShopConfigLoaded = true;
#endif
}

void SEASON3B::CNewUISkillList::ResetMouseLButton()
{
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void SEASON3B::CNewUISkillList::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if(pClass)
	{
		CNewUISkillList* pSkillList= (CNewUISkillList*)(pClass);

#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
		pSkillList->RenderSkillInfo();
#elif CLIENT_VERSION >= 1001
		if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
			pSkillList->RenderSkillInfo();
#endif
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp_Wide(__int64 dwPreExp)
{
	m_loPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp_Wide(__int64 dwGetExp)
{
	m_loGetExp = dwGetExp;

	if(m_loGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetPreExp(DWORD dwPreExp)
{
	m_dwPreExp = dwPreExp;
}

void SEASON3B::CNewUIMainFrameWindow::SetGetExp(DWORD dwGetExp)
{
	m_dwGetExp = dwGetExp;

	if(m_dwGetExp > 0)
	{
		m_bExpEffect = true;
		m_dwExpEffectTime = timeGetTime();
	}
}

void SEASON3B::CNewUIMainFrameWindow::SetBtnState(int iBtnType, bool bStateDown)
{
	switch(iBtnType)
	{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME 
		#if CLIENT_VERSION >= 302 && CLIENT VERSION <= 502
	case MAINFRAME_BTN_PARTCHARGE:
		{
			if(bStateDown) 
			{
				m_BtnCShop.UnRegisterButtonState();
				m_BtnCShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CSHOP, 2);
				m_BtnCShop.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CSHOP, 3);
				m_BtnCShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CSHOP, 2);
				m_BtnCShop.ChangeImgIndex(IMAGE_MENU_BTN_CSHOP, 2);
			}
			else 
			{
				m_BtnCShop.UnRegisterButtonState();
				m_BtnCShop.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CSHOP, 0);
				m_BtnCShop.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CSHOP, 1);
				m_BtnCShop.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CSHOP, 2);
				m_BtnCShop.ChangeImgIndex(IMAGE_MENU_BTN_CSHOP, 0);
			}
		}
		break;
#endif
#endif //defined defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
	case MAINFRAME_BTN_CHAINFO:
		{
			if(bStateDown)
			{
				m_BtnChaInfo.UnRegisterButtonState();
				m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CHAINFO, 2);
				m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CHAINFO, 3);
				m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CHAINFO, 2);
				m_BtnChaInfo.ChangeImgIndex(IMAGE_MENU_BTN_CHAINFO, 2);
				
			}
			else
			{
				m_BtnChaInfo.UnRegisterButtonState();
				m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_CHAINFO, 0);
				m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_CHAINFO, 1);
				m_BtnChaInfo.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_CHAINFO, 2);
				m_BtnChaInfo.ChangeImgIndex(IMAGE_MENU_BTN_CHAINFO, 0);
			}
		}
		break;
	case MAINFRAME_BTN_MYINVEN:
		{
			if(bStateDown)
			{
				m_BtnMyInven.UnRegisterButtonState();
				m_BtnMyInven.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_MYINVEN, 2);
				m_BtnMyInven.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_MYINVEN, 3);
				m_BtnMyInven.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_MYINVEN, 2);
				m_BtnMyInven.ChangeImgIndex(IMAGE_MENU_BTN_MYINVEN, 2);
			}
			else
			{
				m_BtnMyInven.UnRegisterButtonState();
				m_BtnMyInven.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_MYINVEN, 0);
				m_BtnMyInven.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_MYINVEN, 1);
				m_BtnMyInven.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_MYINVEN, 2);
				m_BtnMyInven.ChangeImgIndex(IMAGE_MENU_BTN_MYINVEN, 0);
			}
		}
		break;
#if CLIENT_VERSION <= 502
	case MAINFRAME_BTN_FRIEND:
		{
			if(bStateDown)
			{
				m_BtnFriend.UnRegisterButtonState();
				m_BtnFriend.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_FRIEND, 2);
				m_BtnFriend.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_FRIEND, 3);
				m_BtnFriend.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_FRIEND, 2);
				m_BtnFriend.ChangeImgIndex(IMAGE_MENU_BTN_FRIEND, 2);
			}
			else
			{
				m_BtnFriend.UnRegisterButtonState();
				m_BtnFriend.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_FRIEND, 0);
				m_BtnFriend.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_FRIEND, 1);
				m_BtnFriend.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_FRIEND, 2);
				m_BtnFriend.ChangeImgIndex(IMAGE_MENU_BTN_FRIEND, 0);
			}
		}
		break;
#endif
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	case MAINFRAME_BTN_WINDOW:
		{
			if(bStateDown)
			{
				m_BtnWindow.UnRegisterButtonState();
				m_BtnWindow.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_WINDOW, 2);
				m_BtnWindow.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_WINDOW, 3);
				m_BtnWindow.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_WINDOW, 2);
				m_BtnWindow.ChangeImgIndex(IMAGE_MENU_BTN_WINDOW, 2);
			}
			else
			{
				m_BtnWindow.UnRegisterButtonState();
				m_BtnWindow.RegisterButtonState(BUTTON_STATE_UP, IMAGE_MENU_BTN_WINDOW, 0);
				m_BtnWindow.RegisterButtonState(BUTTON_STATE_OVER, IMAGE_MENU_BTN_WINDOW, 1);
				m_BtnWindow.RegisterButtonState(BUTTON_STATE_DOWN, IMAGE_MENU_BTN_WINDOW, 2);
				m_BtnWindow.ChangeImgIndex(IMAGE_MENU_BTN_WINDOW, 0);
			}
		}
		break;
#endif
	}
}
