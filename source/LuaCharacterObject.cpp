#include "stdafx.h"
#include "LuaCharacterObject.h"
#include "LuaStack.hpp"

void RegisterLuaCharacterObject(lua_State* lua) {
	luaaa::LuaClass<characterObject> luaObject(lua, "Character");
	luaObject.ctor<DWORD>();
	luaObject.fun("getSword", &characterObject::getSword);
	luaObject.fun("getShield", &characterObject::getShield);
	luaObject.fun("getHelm", &characterObject::getHelm);
	luaObject.fun("getArmor", &characterObject::getArmor);
	luaObject.fun("getPants", &characterObject::getPants);
	luaObject.fun("getGloves", &characterObject::getGloves);
	luaObject.fun("getBoots", &characterObject::getBoots);
	luaObject.fun("getWing", &characterObject::getWing);
	luaObject.fun("getHelper", &characterObject::getHelper);
	luaObject.fun("getMonsterIndex", &characterObject::getMonsterIndex);
	luaObject.fun("getSafeZone", &characterObject::getSafeZone);
	luaObject.fun("getClass", &characterObject::getClass);
	luaObject.fun("getSkin", &characterObject::getSkin);
	luaObject.fun("getCtlCode", &characterObject::getCtlCode);
	luaObject.fun("getAction", &characterObject::getAction);
	luaObject.fun("getLevel", &characterObject::getLevel);
	luaObject.fun("getName", &characterObject::getName);
	luaObject.fun("getRun", &characterObject::getRun);
	luaObject.fun("getEtcPart", &characterObject::getEtcPart);
	luaObject.fun("getPatente", &characterObject::getPatente);
	luaObject.fun("getPK", &characterObject::getPK);
}