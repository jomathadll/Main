#pragma once
#pragma once
#include "stdafx.h"

struct CHARACTER_NAME
{
    char Name[32];
    char Charactername[32];
    char Uncharactername[32];

};

class CCharactername
{
public:
    CCharactername();
    ~CCharactername();

    void Init();
    void RestartLua();

private:
    Lua m_Lua;

public:
    std::map<std::string, CHARACTER_NAME> m_Charactername;
};

extern CCharactername gCharactername;