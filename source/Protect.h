#pragma once

#include "HelperSystem.h"
#include "CustomJewel.h"

struct MAIN_FILE_INFO
{
	char CustomerName[32];
	char IpAddress[32];
	WORD IpAddressPort;
	char ClientVersion[8];
	char ClientSerial[17];
	char WindowName[32];
	char ScreenShotPath[50];
	BYTE EnableVisualIventory;
	BYTE LuaReload;
	BYTE LuaCrypt;
	char m_registerPath[125];
	char m_PrivateCode[125];
	BYTE m_PersonalShop_Coins;
	int m_AttackSpeedDW;
	int m_AttackSpeedDK;
	int m_AttackSpeedELF;
	int m_AttackSpeedMG;
	int m_AttackSpeedDL;
	int m_AttackSpeedSUM;
	int m_MagicSpeedDW;
	int m_MagicSpeedDK;
	int m_MagicSpeedELF;
	int m_MagicSpeedMG;
	int m_MagicSpeedDL;
	int m_MagicSpeedSUM;
};

struct LOAD_CUSTOM_JEWEL_INFO
{
	CUSTOM_JEWEL_INFO CustomJewelInfo[MAX_CUSTOM_JEWEL];
};

class CProtect
{
public:
	CProtect() = default;
	~CProtect() = default;
	bool ReadMainFile(char* Path);
	bool ReadCustomJewelConfig(char* name);

public:
	LOAD_CUSTOM_JEWEL_INFO m_CustomJewel;
	MAIN_FILE_INFO m_MainInfo;
	DWORD m_ClientFileCRC;
};

extern CProtect* gProtect;