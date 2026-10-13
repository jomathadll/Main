#include "stdafx.h"
#include "Lua.h"
#include "LuaStack.hpp"
#include "CustomColorChat.h"
#include "LuaCharacterObject.h"

using namespace luaaa;

CCustomColorChat gCustomColorChat;

CCustomColorChat::CCustomColorChat() : m_Lua()
{
    this->m_CustomColorPlayer.clear();
    this->m_CustomColorGuild.clear();
    this->m_CustomColorUnion.clear();
    this->m_CustomColorChat.clear();
    this->m_CustomColorChatLeft.clear();
    this->m_CustomColorTitle.clear();
    this->m_CustomColorCommand.clear();
}

CCustomColorChat::~CCustomColorChat()
{
    this->m_CustomColorPlayer.clear();
    this->m_CustomColorGuild.clear();
    this->m_CustomColorUnion.clear();
    this->m_CustomColorChat.clear();
    this->m_CustomColorChatLeft.clear();
    this->m_CustomColorTitle.clear();
    this->m_CustomColorCommand.clear();
}

void SetCustomColorPlayer(char* Player, char* Name, int Bold, int Align, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA)
{
    CUSTOM_COLOR_PLAYER info;

    strcpy_s(info.Player, Player);

    strcpy_s(info.Name, Name);

    info.Bold = Bold;

    info.Align = Align;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    gCustomColorChat.m_CustomColorPlayer[Player] = info;
}

void SetCustomColorTitle(char* Player, char* Name, int Bold, int Align, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA)
{
    CUSTOM_COLOR_TITLE info;

    strcpy_s(info.Player, Player);

    strcpy_s(info.Name, Name);

    info.Bold = Bold;

    info.Align = Align;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    gCustomColorChat.m_CustomColorTitle[Player] = info;
}

void SetCustomColorChat(char* Player, int Bold, int Align, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA)
{
    CUSTOM_COLOR_CHAT info;

    strcpy_s(info.Player, Player);

    info.Align = Align;

    info.Bold = Bold;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    gCustomColorChat.m_CustomColorChat[Player] = info;
}

void SetCustomColorChatLeft(char* Player, char* Name, int Bold, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA)
{
    CUSTOM_COLOR_CHAT_LEFT info;

    strcpy_s(info.Player, Player);

    strcpy_s(info.Name, Name);

    info.Bold = Bold;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    gCustomColorChat.m_CustomColorChatLeft[Player] = info;
}

void SetCustomColorGuild(char* Player, int Bold, int Align, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA)
{
    CUSTOM_COLOR_GUILD info;

    strcpy_s(info.Player, Player);

    info.Bold = Bold;

    info.Align = Align;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    gCustomColorChat.m_CustomColorGuild[Player] = info;
}

void SetCustomColorUnion(char* Player, int Bold, int Align, int ColorR, int ColorG, int ColorB, int ColorA, int BackgroundR, int BackgroundG, int BackgroundB, int BackgroundA)
{
    CUSTOM_COLOR_UNION info;

    strcpy_s(info.Player, Player);

    info.Bold = Bold;

    info.Align = Align;

    info.ColorR = ColorR;

    info.ColorG = ColorG;

    info.ColorB = ColorB;

    info.ColorA = ColorA;

    info.BackgroundR = BackgroundR;

    info.BackgroundG = BackgroundG;

    info.BackgroundB = BackgroundB;

    info.BackgroundA = BackgroundA;

    gCustomColorChat.m_CustomColorUnion[Player] = info;
}

void SetCustomColorCommand(char* Player, char* Command)
{
    CUSTOM_COLOR_COMMAND info;

    strcpy_s(info.Player, Player);

    strcpy_s(info.Command, Command);

    gCustomColorChat.m_CustomColorCommand[Player] = info;
}

void CCustomColorChat::RestartLua()
{
    this->m_CustomColorPlayer.clear();

    this->m_CustomColorGuild.clear();

    this->m_CustomColorUnion.clear();

    this->m_CustomColorChat.clear();

    this->m_CustomColorChatLeft.clear();

    this->m_CustomColorTitle.clear();

    this->m_CustomColorCommand.clear();

    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorPlayer", &SetCustomColorPlayer);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorGuild", &SetCustomColorGuild);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorUnion", &SetCustomColorUnion);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorChat", &SetCustomColorChat);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorChatLeft", &SetCustomColorChatLeft);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorTitle", &SetCustomColorTitle);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorCommand", &SetCustomColorCommand);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Player\\CustomColorChat.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCustomColorName", "");
}

void CCustomColorChat::Init()
{
    RegisterLuaCharacterObject(this->m_Lua.GetState());

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorPlayer", &SetCustomColorPlayer);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorGuild", &SetCustomColorGuild);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorUnion", &SetCustomColorUnion);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorChat", &SetCustomColorChat);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorChatLeft", &SetCustomColorChatLeft);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorTitle", &SetCustomColorTitle);

    LuaModule(this->m_Lua.GetState()).fun("SetCustomColorCommand", &SetCustomColorCommand);

    char name[MAX_PATH] = { 0 };
    sprintf_s(name, "%s\\Player\\CustomColorChat.lua", LUA_LOAD_PATH);

    this->m_Lua.DoFile(name);

    this->m_Lua.Generic_Call("StartCustomColorName", "");
}

