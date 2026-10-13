#include "stdafx.h"
#include "LuaUser.h"
#include "LuaStack.hpp"
#include "ZzzOpenglUtil.h"
#include "MapManager.h"
#include "CharacterManager.h"
#include "ZzzCharacter.h"

using namespace luaaa;

int LuaUserMap() {
	return gMapManager.WorldActive;
}

int LuaUserGetLevel() {
	return CharacterAttribute->Level;
}

int LuaUserGetKey()
{
	auto lpUser = Hero;
	return  lpUser->Key;
}

int LuaUserGetClass() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetSword() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetShield() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetHelper() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}


int LuaUserGetWing() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserUserGetHelm() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetArmor() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetPants() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetGloves() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

int LuaUserGetBoots() {
	return gCharacterManager.GetCharacterClass(CharacterAttribute->Class);
}

std::string LuaUserGetName() {
	return CharacterAttribute->Name;
}

int LuaUserGetCtlCode() {
	if (Hero == nullptr) {
		return 0;
	}

	return Hero->CtlCode;
}

void CLuaUser::RegisterLuaUser(lua_State * Lua)
{
	//LuaModule(Lua).fun("UserGetStrength", LuaUserGetStrength);
	//LuaModule(Lua).fun("UserGetDexterity", LuaUserGetDexterity);
	//LuaModule(Lua).fun("UserGetVitality", LuaUserGetVitality);
	//LuaModule(Lua).fun("UserGetEnergy", LuaUserGetEnergy);
	LuaModule(Lua).fun("UserGetMap", LuaUserMap);
	LuaModule(Lua).fun("UserGetName", LuaUserGetName);
	LuaModule(Lua).fun("UserGetIndex", LuaUserGetKey);
	//LuaModule(Lua).fun("UserPositionX", LuaUserPositionX);
	//LuaModule(Lua).fun("UserPositionY", LuaUserPositionY);
	LuaModule(Lua).fun("UserGetHelper", LuaUserGetHelper);
	LuaModule(Lua).fun("UserGetWing", LuaUserGetWing);
	LuaModule(Lua).fun("UserGetSword", LuaUserGetSword);
	LuaModule(Lua).fun("UserGetShield", LuaUserGetShield);
	LuaModule(Lua).fun("UserGetHelm", LuaUserUserGetHelm);
	LuaModule(Lua).fun("UserGetArmor", LuaUserGetArmor);
	LuaModule(Lua).fun("UserGetPants", LuaUserGetPants);
	LuaModule(Lua).fun("UserGetGloves", LuaUserGetGloves);
	LuaModule(Lua).fun("UserGetBoots", LuaUserGetBoots);
	LuaModule(Lua).fun("UserGetLevel", LuaUserGetLevel);
	LuaModule(Lua).fun("UserGetClass", LuaUserGetClass);
	LuaModule(Lua).fun("UserGetCtlCode", LuaUserGetCtlCode);
	//LuaModule(Lua).fun("UserGetGuild", LuaUserGetGuild);
	//LuaModule(Lua).fun("GuildGetName", LuaGuildGetName);
	//LuaModule(Lua).fun("UserGetLanguage", LuaUserGetLanguage);
	

}
