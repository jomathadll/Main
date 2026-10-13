#pragma once
#include "stdafx.h"

struct CUSTOM_COLOR_PLAYER
{
    char Player[32];
    char Name[256];
    int Bold;
    int Align;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
};

struct CUSTOM_COLOR_TITLE
{
    char Player[32];
    char Name[256];
    int Bold;
    int Align;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
};

struct CUSTOM_COLOR_CHAT
{
    char Player[32];
    int Bold;
    int Align;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
};

struct CUSTOM_COLOR_CHAT_LEFT
{
    char Player[32];
    char Name[256];
    int Bold;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
};

struct CUSTOM_COLOR_GUILD
{
    char Player[32];
    int Bold;
    int Align;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
};

struct CUSTOM_COLOR_UNION
{
    char Player[32];
    int Bold;
    int Align;
    int ColorR;
    int ColorG;
    int ColorB;
    int ColorA;
    int BackgroundR;
    int BackgroundG;
    int BackgroundB;
    int BackgroundA;
};

struct CUSTOM_COLOR_COMMAND
{
    char Player[32];
    char Command[256];
};

class CCustomColorChat
{
public:
    CCustomColorChat();
    ~CCustomColorChat();

    void Init();
    void RestartLua();

private:
    Lua m_Lua;

public:
    std::map<std::string, CUSTOM_COLOR_PLAYER> m_CustomColorPlayer;
    std::map<std::string, CUSTOM_COLOR_TITLE> m_CustomColorTitle;
    std::map<std::string, CUSTOM_COLOR_CHAT> m_CustomColorChat;
    std::map<std::string, CUSTOM_COLOR_CHAT_LEFT> m_CustomColorChatLeft;
    std::map<std::string, CUSTOM_COLOR_GUILD> m_CustomColorGuild;
    std::map<std::string, CUSTOM_COLOR_UNION> m_CustomColorUnion;
    std::map<std::string, CUSTOM_COLOR_COMMAND> m_CustomColorCommand;
};

extern CCustomColorChat gCustomColorChat;