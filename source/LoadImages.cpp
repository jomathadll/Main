#include "stdafx.h"
#include "LoadImages.h"
#include "LuaStack.hpp"
#include "NewUISystem.h"
#include "RenderModel.h"
#include "CustomCape.h"
#include "DeathStabColor.h"
#include "Patente.h"

CLoadImages gLoadImages;

CLoadImages::CLoadImages() : m_Lua()
{
	m_fisrtCall = true;
}

CLoadImages::~CLoadImages()
{
}

void LuaLoadImage(char* path, int id) {
	char imagePath[MAX_PATH] = { 0 };
	sprintf_s(imagePath, "%s", path);

	LoadBitmap(imagePath, id, GL_LINEAR, GL_REPEAT);
}


void LuaLoadImageCustom(char* path, int id) {
	char imagePath[MAX_PATH] = { 0 };
	sprintf_s(imagePath, "%s", path);

	LoadBitmapCustom(imagePath, id, GL_LINEAR, GL_REPEAT);
}

void LuaLoadImageRune(char* path, int id) {
	char imagePath[MAX_PATH] = { 0 };
	sprintf_s(imagePath, "%s", path);

	LoadBitmap(imagePath, id, GL_LINEAR, GL_CLAMP_TO_EDGE);
}

int LoadImageByDir(char* string)
{
	LoadBitmap(string, gLoadImages.luaStartImage);

	return gLoadImages.luaStartImage++;
}

void LuaUnloadImage(int id) {
	DeleteBitmap(id);
}

void CLoadImages::SetFunctions(lua_State* lua)
{
	luaaa::LuaModule(lua).fun("LoadImage", &LuaLoadImage);

	luaaa::LuaModule(lua).fun("LoadImageCustom", &LuaLoadImageCustom);

	luaaa::LuaModule(lua).fun("LoadImageRune", &LuaLoadImageRune);

	luaaa::LuaModule(lua).fun("LoadImageByDir", &LoadImageByDir);
}

void CLoadImages::Init()
{
	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImage", &LuaLoadImage);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImageCustom", &LuaLoadImageCustom);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImageRune", &LuaLoadImageRune);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("UnloadImage", &LuaUnloadImage);

	char luaLoadImagePath[MAX_PATH] = { 0 };
	sprintf_s(luaLoadImagePath, "%s\\Controller\\LoadImages.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(luaLoadImagePath);

	this->m_Lua.Generic_Call("LoadImages", ">");

	if (this->m_fisrtCall) {
		this->m_fisrtCall = false;
		gRenderModel.LoadImageRenderModel();
		gCustomCape.LoadImageCape();
		gDeathStabColor.LoadImageDeathStab();
		gPatente.LoadImages();
	}

	LoadBitmap("Interface\\HealthBar_Back.tga", BITMAP_NEW_HEALTHBAR_BACK, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\HealthBar_Life.jpg", BITMAP_NEW_HEALTHBAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\HealthBar_Boss_Back.tga", BITMAP_NEW_HEALTHBAR_BOSS_BACK, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\HealthBar_Boss_Life.tga", BITMAP_NEW_HEALTHBAR_BOSS_LIFE, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\HealthBar_Monster_Back.tga", BITMAP_NEW_HEALTHBAR_MONSTER_BACK, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\HealthBar_Monster_Life.tga", BITMAP_NEW_HEALTHBAR_MONSTER_LIFE, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\SkillShopBuff_01.tga", BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 0, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\SkillShopBuff_02.tga", BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 1, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\SkillShopBuff_03.tga", BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 2, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\SkillShopBuff_04.tga", BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 3, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\SkillShopBuff_05.tga", BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 4, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\SkillShopBuff_06.tga", BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 5, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\DragonSkillFireBurst.tga", BITMAP_DRAGON_SKILL_ICON, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Custom\\Interface\\BuffStackButton.tga", BITMAP_BUFF_STACK_BUTTON, GL_LINEAR, GL_CLAMP_TO_EDGE);
	LoadBitmap("Interface\\tooltip_bg01.tga", BITMAP_NEW_TOOLTIP_START, GL_LINEAR, GL_REPEAT);     //Topo esquerda
	LoadBitmap("Interface\\tooltip_bg02.tga", BITMAP_NEW_TOOLTIP_START + 1, GL_LINEAR, GL_REPEAT); //Topo meio
	LoadBitmap("Interface\\tooltip_bg03.tga", BITMAP_NEW_TOOLTIP_START + 2, GL_LINEAR, GL_REPEAT); //Topo direita
	LoadBitmap("Interface\\tooltip_bg04.tga", BITMAP_NEW_TOOLTIP_START + 3, GL_LINEAR, GL_REPEAT); //Lateral esquerda
	LoadBitmap("Interface\\tooltip_bg06.tga", BITMAP_NEW_TOOLTIP_START + 4, GL_LINEAR, GL_REPEAT); //Lateral direita
	LoadBitmap("Interface\\tooltip_bg07.tga", BITMAP_NEW_TOOLTIP_START + 5, GL_LINEAR, GL_REPEAT); //Baixo esquerda
	LoadBitmap("Interface\\tooltip_bg08.tga", BITMAP_NEW_TOOLTIP_START + 6, GL_LINEAR, GL_REPEAT); //Baixo meio
	LoadBitmap("Interface\\tooltip_bg09.tga", BITMAP_NEW_TOOLTIP_START + 7, GL_LINEAR, GL_REPEAT); //Baixo direita
	LoadBitmap("Interface\\tooltip_bg05.tga", BITMAP_NEW_TOOLTIP_START + 8, GL_LINEAR, GL_REPEAT); //Fundo
	LoadBitmap("Custom\\Interface\\btn_tradex.jpg", BITMAP_TRADEX_ICON, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\btn_hiden_tradex.jpg", BITMAP_TRADEX_HIDE, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\btn_check_on.jpg", BITMAP_NEW_CHECKBOX_ON, GL_LINEAR, GL_REPEAT); 
	LoadBitmap("Custom\\Interface\\btn_check_off.jpg", BITMAP_NEW_CHECKBOX_OFF, GL_LINEAR, GL_REPEAT);

	LoadBitmap("Custom\\Interface\\interface_scroll_up.jpg", BITMAP_NEW_SCROLL_UP, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\interface_scroll_m.jpg", BITMAP_NEW_SCROLL_MIDDLE2, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\interface_scroll_down.jpg", BITMAP_NEW_SCROLL_DOWN, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\interface_scroll_on.tga", BITMAP_NEW_SCROLL_ON, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\NewUi_ImageFront.tga", 12506, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Custom\\Interface\\NewUi_ImageFrontSecond.tga", 12507, GL_LINEAR, GL_REPEAT);
}


void CLoadImages::Unload()
{
	this->m_Lua.Generic_Call("UnloadImages", ">");

	DeleteBitmap(BITMAP_NEW_HEALTHBAR_BACK);
	DeleteBitmap(BITMAP_NEW_HEALTHBAR);
	DeleteBitmap(BITMAP_NEW_HEALTHBAR_BOSS_BACK);
	DeleteBitmap(BITMAP_NEW_HEALTHBAR_BOSS_LIFE);
	DeleteBitmap(BITMAP_NEW_HEALTHBAR_MONSTER_BACK);
	DeleteBitmap(BITMAP_NEW_HEALTHBAR_MONSTER_LIFE);
	DeleteBitmap(BITMAP_DRAGON_SKILL_ICON);
	for(int n = 0; n < 6; ++n)
	{
		DeleteBitmap(BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + n);
	}
	DeleteBitmap(BITMAP_BUFF_STACK_BUTTON);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 1);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 2);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 3);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 4);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 5);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 6);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 7);
	DeleteBitmap(BITMAP_NEW_TOOLTIP_START + 8);
	DeleteBitmap(BITMAP_TRADEX_ICON);
	DeleteBitmap(BITMAP_TRADEX_HIDE);
	DeleteBitmap(BITMAP_NEW_CHECKBOX_ON);
	DeleteBitmap(BITMAP_NEW_CHECKBOX_OFF);
	DeleteBitmap(BITMAP_NEW_SCROLL_UP);
	DeleteBitmap(BITMAP_NEW_SCROLL_MIDDLE2);
	DeleteBitmap(BITMAP_NEW_SCROLL_DOWN);
	DeleteBitmap(BITMAP_NEW_SCROLL_ON);
}

void CLoadImages::RestartLua()
{
	this->m_Lua.Generic_Call("UnloadImages", ">");

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImage", &LuaLoadImage);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImageCustom", &LuaLoadImageCustom);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("LoadImageRune", &LuaLoadImageRune);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("UnloadImage", &LuaUnloadImage);

	char luaLoadImagePath[MAX_PATH] = { 0 };
	sprintf_s(luaLoadImagePath, "%s\\Controller\\LoadImages.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(luaLoadImagePath);

	this->m_Lua.Generic_Call("LoadImages", ">");

}
