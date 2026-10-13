#include "stdafx.h"
#include "LuaReload.h"
#include "NewUISystem.h"
#include "RenderModel.h"
#include "CustomEffects.h"
#include "CustomItemFloor.h"
#include "CustomItemForce.h"
#include "CustomSetEffect.h"
#include "DarkSpirit.h"
#include "Descriptions.h"
#include "HelperSystem.h"
#include "ItemPosition.h"
#include "ServerName.h"
#include "VisualDescriptions.h"
#include "DisableExcellent.h"
#include "Monsters.h"
#include "MonsterEffect.h"
#include "MonsterGlow.h"
#include "MonsterName.h"
#include "CustomCape.h"
#include "CustomItemEdge.h"
#include "DrawBarItem.h"
#include "CounterItem.h"
#include "CustomMeshGlow.h"
#include "CharacterList.h"
#include "CustomBow.h"
#include "BuffColorBK.h"
#include "BuffColorSM.h"
#include "PlasmaStormColor.h"
#include "CustomEarthQuakeSkill.h"
#include "RuneColorEffect.h"
#include "CustomRingTransform.h"
#include "CustomSetAction.h"
#include "DeathStabColor.h"
#include "CustomEye.h"
#include "MonsterItem.h"
#include "SummonEffect.h"
#include "LoadImages.h"
#include "CustomTitleItem.h"
#include "Charactername.h"
#include "CustomColorChat.h"
#include "Patente.h"
#include "CustomModelAction.h"
#include "ItemBorder.h"

CLuaReload gLuaReload;

void CLuaReload::RestartLua() {
	g_pNewUISystem->RestartLua();
	gRenderModel.RestartLua();
	gCustomEffects.RestartLua();
	gCCustomSetEffect.RestartLua();
	gCustomItemFloor.RestartLua();
	gCustomItemForce.RestartLua();
	gDarkSpirit.RestartLua();
	gDescriptions.RestartLua();
	gHelperSystem.RestartLua();
	gCustomEarthQuakeSkill.RestartLua();
	gCustomItemPosition.RestartLua();
	gCustomServerName.RestartLua();
	Visuals.RestartLua();
	gDisableExcellent.RestartLua();
	gMonsters.RestartLua();
	gMonsterEffect.RestartLua();
	gMonsterGlow.RestartLua();
	gMonsterName.RestartLua();
	gCustomCape.RestartLua();
	gItemEdge.RestartLua();
	gDrawBarItem.RestartLua();
	gCounterItem.RestartLua();
	gCustomMeshGlow.RestartLua();
	g_pChaosGenesis->RestartLua();
	gCharacterList.RestartLua();
	gCustomBow.RestartLua();
	gBuffColorBK.RestartLua();
	gBuffColorSM.RestartLua();
	gPlasmaStormColor.RestartLua();
	gRuneColorEffect.RestartLua();
	gRingTransform.RestartLua();
	gSetAction.RestartLua();
	gDeathStabColor.RestartLua();
	gCustomEye.RestartLua();
	gCustomMonsterItem.RestartLua();
	gSummonEffect.RestartLua();
	gLoadImages.RestartLua();
	gCustomTitleItem.RestartLua();
	gCharactername.RestartLua();
	gCustomColorChat.RestartLua();
	gPatente.RestartLua();
	gSetModelAction.RestartLua();
	gItemBorder.RestartLua();

	g_pChatListBox->AddText("", "Lua Reloaded", SEASON3B::TYPE_SYSTEM_MESSAGE);
}