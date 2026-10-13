#include "stdafx.h"
#include "LuaGlobal.h"
#include "LuaStack.hpp"
#include "CSItemOption.h"
#include "CSQuest.h"
#include "MoveCommandData.h"
#include "SocketSystem.h"
#include "ZzzInfomation.h"
#include "ZzzOpenData.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "NewUISystem.h"
#include "Hunting.h"
#include "VisualDescriptions.h"
#include "wsclientinline.h"
#include "WideScreen.h"

extern HWND g_hWnd;

using namespace luaaa;

LuaGlobal::LuaGlobal() {

}

LuaGlobal::~LuaGlobal() {

}

namespace
{
	bool IsSupportedClientLanguage(const char* lang)
	{
		return lang != NULL
			&& (_stricmp(lang, "Eng") == 0
				|| _stricmp(lang, "Por") == 0
				|| _stricmp(lang, "Spn") == 0);
	}

	BYTE GetClientLanguageValue(const char* lang)
	{
		if (_stricmp(lang, "Por") == 0)
		{
			return 1;
		}

		if (_stricmp(lang, "Spn") == 0)
		{
			return 2;
		}

		return 0;
	}

	void ReloadLocalizedClientData(const char* lang)
	{
		char Text[256] = { 0 };

		sprintf(Text, "Data\\Local\\%s\\Dialog.bmd", lang);
		OpenDialogFile(Text);

		sprintf(Text, "Data\\Local\\%s\\Item.bmd", lang);
		OpenItemScript(Text);

		sprintf(Text, "Data\\Local\\%s\\movereq.bmd", lang);
		SEASON3B::CMoveCommandData::OpenMoveReqScript(Text);

		sprintf(Text, "Data\\Local\\%s\\NpcName.txt", lang);
		OpenMonsterScript(Text);

		sprintf(Text, "Data\\Local\\%s\\Quest.bmd", lang);
		g_csQuest.OpenQuestScript(Text);

		sprintf(Text, "Data\\Local\\%s\\Skill.bmd", lang);
		OpenSkillScript(Text);

		sprintf(Text, "Data\\Local\\%s\\SocketItem.bmd", lang);
		g_SocketItemMgr.OpenSocketItemScript(Text);

		GlobalText.RemoveAll();
		OpenTextData();

		g_csItemOption.OpenItemSetScript(false);
	}
}

void SetClipboard(char* clipdata) {
	if (!OpenClipboard(0)) return;
	EmptyClipboard();
	size_t ln = strlen(clipdata) + 1;
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, ln);
	memcpy(GlobalLock(hg), clipdata, ln);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
}

std::string GetClipboard() {
	int tp = 0;
	if (IsClipboardFormatAvailable(CF_TEXT))
	{
		tp = CF_TEXT;
	}
	else if (IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		tp = CF_UNICODETEXT;
	}

	if (tp == CF_TEXT || tp == CF_UNICODETEXT) {
		if (!OpenClipboard(0)) return "";
		HANDLE clipdata = GetClipboardData(tp);
		std::string cliptext = (char*)clipdata;
		CloseClipboard();
		return cliptext;
	}
	return "";

}

std::string GetLanguage() {
	if (_strcmpi(g_aszMLSelection, "Por") == 0) 
	{
		return "Por";
	}
	else if (_strcmpi(g_aszMLSelection, "Eng") == 0) 
	{
		return "Eng";
	}
	else if (_strcmpi(g_aszMLSelection, "Spn") == 0) 
	{
		return "Spn";
	}
	else 
	{
		return "Eng";
	}
}

void setLang(char * lang) {
	static DWORD m_CallFunc = 0x00409B10;
	static DWORD m_CallFunc2 = 0x00500E80;

	_asm {
		MOV EAX, lang;
		PUSH EAX;
		MOV ECX, 0x00E8C6EC;
		CALL[m_CallFunc];
		CALL[m_CallFunc2];
	}

}

void LuaEnableAlphaBlendMinus()
{
	EnableAlphaBlendMinus();
}

void LuaEnableAlphaBlend()
{
	EnableAlphaBlend();
}

void SetLanguage(char* value) 
{
	if (!IsSupportedClientLanguage(value))
	{
		return;
	}

	if (_stricmp(g_aszMLSelection, value) == 0)
	{
		return;
	}

	setLang(value);

	g_pMultiLanguage->SetLanguage(GetClientLanguageValue(value));
	strcpy_s(g_aszMLSelection, 4, value);
	g_strSelectedML = g_aszMLSelection;
	ReloadLocalizedClientData(value);

	//gMenuWindow.ChangeLanguage(g_aszMLSelection);

	if (g_bGameServerConnected == TRUE)
	{
		ClientSendMac();
	}
}

int GetWindowWidth() {
	return WindowWidth;
}

int GetWindowHeight() {
	return WindowHeight;
}

int GetFontValue() {
	return FontHeight;
}

void SetFontValue(int value) {
	//gMenuWindow.ChangeFontSize(value);
}

int GetResolution() {
	return m_Resolution;
}

bool GetResolutionSize(int value, unsigned int& width, unsigned int& height) {
	switch (value)
	{
	case 0: width = 800;  height = 480;  return true;
	case 1: width = 1024; height = 600;  return true;
	case 2: width = 1200; height = 768;  return true;
	case 3: width = 1280; height = 720;  return true;
	case 4: width = 1364; height = 768;  return true;
	case 5: width = 1400; height = 900;  return true;
	case 6: width = 1600; height = 900;  return true;
	case 7: width = 1920; height = 900;  return true;
	case 8: width = 1920; height = 1080; return true;
	default: return false;
	}
}

bool GetExecutableDirectory(wchar_t* directory, DWORD directorySize) {
	if (GetModuleFileNameW(NULL, directory, directorySize) == 0) {
		return false;
	}

	wchar_t* slash = wcsrchr(directory, L'\\');
	if (slash == NULL) {
		return false;
	}

	*slash = L'\0';
	return true;
}

bool ImportResolutionRegFile(const wchar_t* regFilePath) {
	wchar_t parameters[MAX_PATH * 2];
	swprintf_s(parameters, L"/s \"%s\"", regFilePath);

	SHELLEXECUTEINFOW shellExecuteInfo = { 0 };
	shellExecuteInfo.cbSize = sizeof(shellExecuteInfo);
	shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellExecuteInfo.lpVerb = L"open";
	shellExecuteInfo.lpFile = L"regedit.exe";
	shellExecuteInfo.lpParameters = parameters;
	shellExecuteInfo.nShow = SW_HIDE;

	if (!ShellExecuteExW(&shellExecuteInfo)) {
		return false;
	}

	if (shellExecuteInfo.hProcess != NULL) {
		WaitForSingleObject(shellExecuteInfo.hProcess, 5000);
		CloseHandle(shellExecuteInfo.hProcess);
	}

	return true;
}

bool FindResolutionRegFileInFolder(const wchar_t* baseDirectory, const wchar_t* folderName, unsigned int width, unsigned int height, wchar_t* regFilePath, DWORD regFilePathSize) {
	wchar_t searchPath[MAX_PATH];
	swprintf_s(searchPath, L"%s\\%s\\*.reg", baseDirectory, folderName);

	WIN32_FIND_DATAW findData;
	HANDLE findHandle = FindFirstFileW(searchPath, &findData);

	if (findHandle == INVALID_HANDLE_VALUE) {
		return false;
	}

	wchar_t resolutionText[32];
	swprintf_s(resolutionText, L"%ux%u", width, height);

	do {
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			continue;
		}

		if (wcsstr(findData.cFileName, resolutionText) == NULL) {
			continue;
		}

		swprintf_s(regFilePath, regFilePathSize, L"%s\\%s\\%s", baseDirectory, folderName, findData.cFileName);
		FindClose(findHandle);
		return true;
	} while (FindNextFileW(findHandle, &findData));

	FindClose(findHandle);
	return false;
}

bool TryImportResolutionRegFile(int value, unsigned int width, unsigned int height) {
	wchar_t executableDirectory[MAX_PATH];
	wchar_t currentDirectory[MAX_PATH];
	wchar_t regFilePath[MAX_PATH];

	const wchar_t* folders[] = {
		L"Resolu" L"\x00E7" L"\x00F5" L"es",
		L"Resolucoes",
		L"Resolutions"
	};

	if (GetExecutableDirectory(executableDirectory, MAX_PATH)) {
		for (int i = 0; i < (int)(sizeof(folders) / sizeof(folders[0])); i++) {
			if (FindResolutionRegFileInFolder(executableDirectory, folders[i], width, height, regFilePath, MAX_PATH)) {
				return ImportResolutionRegFile(regFilePath);
			}
		}
	}

	if (GetCurrentDirectoryW(MAX_PATH, currentDirectory) != 0) {
		for (int i = 0; i < (int)(sizeof(folders) / sizeof(folders[0])); i++) {
			if (FindResolutionRegFileInFolder(currentDirectory, folders[i], width, height, regFilePath, MAX_PATH)) {
				return ImportResolutionRegFile(regFilePath);
			}
		}
	}

	return false;
}

void SaveResolutionRegistry(int value) {
	char szRegisterPath[256];
	wsprintf(szRegisterPath, "SOFTWARE\\%s\\Config", gProtect->m_MainInfo.m_registerPath);

	HKEY hKey = NULL;
	DWORD dwDisp = 0;

	if (RegCreateKeyEx(HKEY_CURRENT_USER, szRegisterPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisp) == ERROR_SUCCESS)
	{
		DWORD resolution = (DWORD)value;
		RegSetValueEx(hKey, "Resolution", 0, REG_DWORD, (const BYTE*)&resolution, sizeof(resolution));
		RegCloseKey(hKey);
	}
}

void SetResolution(int value) {
	unsigned int width = 0;
	unsigned int height = 0;

	if (!GetResolutionSize(value, width, height)) {
		return;
	}

	SaveResolutionRegistry(value);
	TryImportResolutionRegFile(value, width, height);
}

int GetVolume() {
	return 0;//return ((int(__thiscall*)(LPVOID))0x849E80)(((LPVOID(__thiscall*)(LPVOID))0x861620)(pWindowThis()));
}

void LuaSetVolume(int value) {
	//((void(__thiscall*)(LPVOID, int))0x849E60)(((LPVOID(__thiscall*)(LPVOID))0x861620)(pWindowThis()), value);
}

void SetHealthBar(int value) {
	//gMenuWindow.m_HealthBarDisabled = value;
}

void SetGlowSwitch(int value) {
	//((void(__thiscall*)(LPVOID, int))0x849EA0)(((LPVOID(__thiscall*)(LPVOID))0x861620)(pWindowThis()), value);
}

void SetWingSwitch(int value) {
	//gMenuWindow.m_WingDisabled = value;
}

char* GetNameByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) 
	{
		return itemAtt->Name;
	}

	return "not find";
}

void StartGames() {
	//((void(*)(int))0x4D6A60)(0);

	//lpViewObj lpObj = &*(ObjectPreview*)((DWORD(__thiscall*)(void*, DWORD))0x0096A4C0)(((void* (*)())0x00402BC0)(), ((int(*)(int))0x57D9A0)(*(DWORD*)0xE60B4C));
	//
	//*(WORD *)(*(DWORD*)0x8128AC8 + 14) = lpObj->Level;
	//*(BYTE *)(*(DWORD*)0x8128AC8 + 11) = lpObj->Class;
	//*(BYTE *)(*(DWORD*)0x8128AC8 + 12) = lpObj->Skin;
	//strcpy((char*)(*(DWORD*)0x8128AC8), lpObj->Name);
	//((void(*)())0x631610)();
	//*(BYTE*)0xE8CB4D = 0;
	//*(DWORD*)0xE609E8 = 3;
}

std::string GetCompleteNameByIndex(int index, int level, int exc) {
	static char buffer[120];
	std::memset(buffer, 0, sizeof(buffer));

	auto itemAtt = &ItemAttribute[index];

	int ItemIndex = index + 1171;

	if (ItemIndex == 8354) {
		return itemAtt->Name;
	}
	else {
		if (ItemIndex != 8356 && ItemIndex != 8357 && ItemIndex != 8358) {
			if (ItemIndex != 8350 || level != 7) {
				if (ItemIndex == 8351) {
					if (level)
					{
						if (level == 1) {
							return GlobalText[101];
						}
						else if (level == 2) {
							return GlobalText[104];
						}
					}
					else {
						return GlobalText[100];
					}
				}
				else
				{
					if (ItemIndex == 7842)
					{
						switch (level)
						{
						case 0:
							sprintf_s(buffer, "%s %s", GlobalText[168], itemAtt->Name);
							return &buffer[0];
						case 1:
							sprintf_s(buffer, "%s %s", GlobalText[169], itemAtt->Name);
							return &buffer[0];
						case 2:
							sprintf_s(buffer, "%s %s", GlobalText[167], itemAtt->Name);
							return &buffer[0];
						case 3:
							sprintf_s(buffer, "%s %s", GlobalText[166], itemAtt->Name);
							return &buffer[0];
						case 4:
							sprintf_s(buffer, "%s %s", GlobalText[1900], itemAtt->Name);
							return &buffer[0];
						default:
							return itemAtt->Name;
						}
					}
					else
					{
						switch (ItemIndex)
						{
						case 0x1EB2:
							if (level)
							{
								if (level == 1)
								{
									sprintf_s(buffer, "%s of %s", GlobalText[1214], itemAtt->Name);
									return &buffer[0];
								}
							}
							else
							{
								sprintf_s(buffer, "%s of %s", GlobalText[1187], itemAtt->Name);
								return &buffer[0];
							}
							break;
						case 0x2535:
							return GlobalText[1235];
						case 0x2529:
							return GlobalText[105];
						case 0x252A:
							if (level == 14)
							{
								return GlobalText[1650];
							}
							else
							{
								if (level == 15) {
									return GlobalText[1651];
								}

								return GlobalText[106];
							}
							break;
						case 0x252B:
							if (level == 13)
							{
								return GlobalText[117];
							}
							else
							{
								return GlobalText[107];
							}
							break;
						case 0x252C:
							return GlobalText[108];
						case 0x252D:
							return GlobalText[109];
						case 0x252E:
							return GlobalText[110];
						case 0x252F:
							sprintf_s(buffer, "%s +%d", GlobalText[115], level - 7);
							return &buffer[0];
						default:
							if (ItemIndex != 8371 || level != 1)
							{
								if (ItemIndex != 8372 || level != 1)
								{
									if (ItemIndex != 8373 || level != 1)
									{
										switch (ItemIndex)
										{
										case 0x253A:
											return GlobalText[2012];
										case 0x253B:
											return GlobalText[2013];
										case 0x253C:
											return GlobalText[2014];
										case 0x2530:
											if (level == 2) {
												return GlobalText[1098];
											}

											return GlobalText[810];
										case 0x2531:
											return GlobalText[906];
										case 0x2532:
											return GlobalText[907];
										default:
											switch (ItemIndex)
											{
											case 0x2533:
												if (level == 2)
												{
													return GlobalText[928];
												}
												else
												{
													if (level == 3) {
														return GlobalText[929];
													}
			
													return GlobalText[922];
												}
												break;
											case 0x2534:
												return GlobalText[925];
											default:
												if (ItemIndex != 8360 || level != 3)
												{
													if (ItemIndex == 8346)
													{
														if (level)
														{
															if (level == 1)
															{
																return GlobalText[1414];
															}
														}
														else
														{
															return GlobalText[1413];
														}
													}
													else if (ItemIndex == 7834)
													{
														if (level)
														{
															if (level == 1)
															{
																return GlobalText[1461];
															}
														}
														else
														{
															return GlobalText[1460];
														}
													}
													else
													{
														switch (ItemIndex)
														{
														case 0x1E9E:
															if (level)
															{
																if (level == 1)
																{
																	return GlobalText[1462];
																}
															}
															else
															{
																return GlobalText[1416];
															}
															break;
														case 0x2537:
															return GlobalText[1462];
														default:
															break;
														}
													}
												}
												else
												{
													return GlobalText[1290];
												}
												break;
											}
											break;
										}
									}
									else
									{
										return GlobalText[2014];
									}
								}
								else
								{
									return GlobalText[2013];
								}
							}
							else
							{
								return GlobalText[2012];
							}
							break;
						}
					}
				}
			}
			else
			{
				return GlobalText[111];
			}
		}
		else if ((exc >> 3) & 0xF)
		{
			sprintf_s(buffer, "%s +%d", itemAtt->Name, (exc >> 3) & 0xF);
			return &buffer[0];
		}
		else
		{
			return itemAtt->Name;
		}
	}

	return itemAtt->Name;;
}

int GetWidthByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) {
		return itemAtt->Width;
	}

	return -1;
}

int GetHeightByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) {
		return itemAtt->Height;
	}

	return -1;
}

int GetSlotByIndex(int index) 
{
	auto itemAtt = &ItemAttribute[index];

	if (itemAtt != nullptr) {
		return itemAtt->m_byItemSlot;
	}

	return -1;
}

int GetInventoryMouseSlot() {
	return g_pMyInventory->GetPointedItemIndex();
}

int GetInventoryMouseItemSlot()
{
	DWORD ItemSlot = g_pMyInventory->GetInventoryCtrl()->FindItemptIndex(MouseX, MouseY);
	
	if (ItemSlot != -1) {
		return (ItemSlot + 12);
	}

	return -1;
}

int GetInventoryMouseItemIndex() 
{
	auto item = g_pMyInventory->GetInventoryCtrl()->FindItemPointedSquareIndex();
	
	if (item) {
		return item->Type;
	}

	return -1;
}

int GetInventoryMouseItemLevel() 
{
	auto item = g_pMyInventory->GetInventoryCtrl()->FindItemPointedSquareIndex();

	if (item) {
		return ((item->Level >> 3) & 0xF);
	}

	return -1;
}

int GetInventoryMouseItemExc() {
	auto item = g_pMyInventory->GetInventoryCtrl()->FindItemPointedSquareIndex();

	if (item) {
		return item->ExtOption;
	}

	return -1;
}

void SendMessageClient(std::string msg) {
	g_pChatListBox->AddText("", msg.c_str(), SEASON3B::TYPE_SYSTEM_MESSAGE);
}

void SendMessageClientError(std::string msg) {
	g_pChatListBox->AddText("", msg.c_str(), SEASON3B::TYPE_ERROR_MESSAGE);
}

void SendChatPlayer(char* text)
{
	SendChat(text);
}

char* GetHybridNameByIndex(int ItemIndex) {
	return Visuals.GetItemHybridName(ItemIndex);
}

char* GetGlobalText(int index) {
	
	char* name = (char*)GlobalText[index];

	return name;
}

std::string GetMapName(int map) 
{
	return gMapManager.GetMapName(map);
}

int GetCountParty() 
{
	return PartyNumber;
}

std::string GetMonsterName(int Class) 
{
	for (int i = 0; i < MAX_MONSTER; ++i)
	{
		if (MonsterScript[i].Type == Class)
		{
			return MonsterScript[i].Name;
		}
	}

	return "Not Find";
}

int GetExpPerMinute() {
	return gHunting.expPerMinute;
}

int GetAvaregeExpPerMonster() {
	return gHunting.avarageExp;
}

int GetLastExpReceive() {
	return gHunting.lastExp;
}

int GetEtaLevelUp() {
	return gHunting.etaLevelUp;
}


int GetHuntingLevel() {
	return gHunting.level;
}


int GetLastZen() {
	return gHunting.lastZen;
}


int GetZenPerMinute() {
	return gHunting.zenPerMinute;
}

void LuaGlobal::RegisterGlobal(lua_State * lua)
{
	//main funcs
	LuaModule(lua).fun("GetLanguage", &GetLanguage);
	LuaModule(lua).fun("SetLanguage", &SetLanguage);
	LuaModule(lua).fun("GetWindowWidth", &GetWindowWidth);
	LuaModule(lua).fun("GetWindowHeight", &GetWindowHeight);
	LuaModule(lua).fun("GetFontValue", &GetFontValue);
	LuaModule(lua).fun("SetFontValue", &SetFontValue);
	LuaModule(lua).fun("GetResolution", &GetResolution);
	LuaModule(lua).fun("SetResolution", &SetResolution);
	LuaModule(lua).fun("GetVolume", &GetVolume);
	LuaModule(lua).fun("SetVolume", &LuaSetVolume);
	LuaModule(lua).fun("SetHealthBarSwitch", &SetHealthBar);
	LuaModule(lua).fun("SetGlowSwitch", &SetGlowSwitch);
	LuaModule(lua).fun("SetWingSwitch", &SetWingSwitch);

	LuaModule(lua).fun("SendPlayerChat", &SendChatPlayer);
	LuaModule(lua).fun("GetClipboard", &GetClipboard);
	LuaModule(lua).fun("SetClipboard", &SetClipboard);


	//Item funcs
	LuaModule(lua).fun("GetNameByIndex", &GetNameByIndex);
	LuaModule(lua).fun("GetCompleteNameByIndex", &GetCompleteNameByIndex);
	LuaModule(lua).fun("GetHybridNameByIndex", &GetHybridNameByIndex);
	LuaModule(lua).fun("GetWidthByIndex", &GetWidthByIndex);
	LuaModule(lua).fun("GetHeightByIndex", &GetHeightByIndex);
	LuaModule(lua).fun("GetSlotByIndex", &GetSlotByIndex);


	//Inventory funcs
	LuaModule(lua).fun("GetInventoryMouseSlot", &GetInventoryMouseSlot);
	LuaModule(lua).fun("GetInventoryMouseItemSlot", &GetInventoryMouseItemSlot);
	LuaModule(lua).fun("GetInventoryMouseItemIndex", &GetInventoryMouseItemIndex);
	LuaModule(lua).fun("GetInventoryMouseItemLevel", &GetInventoryMouseItemLevel);
	LuaModule(lua).fun("GetInventoryMouseItemExc", &GetInventoryMouseItemExc);

	LuaModule(lua).fun("SendMessageClient", &SendMessageClient);
	LuaModule(lua).fun("LogDebug", &SendMessageClient);
	LuaModule(lua).fun("LogAdd", &SendMessageClient);

	LuaModule(lua).fun("SendMessageClientError", &SendMessageClientError);
	LuaModule(lua).fun("LogDebugError", &SendMessageClientError);
	LuaModule(lua).fun("LogAddError", &SendMessageClientError);

	LuaModule(lua).fun("GetGlobalText", &GetGlobalText);

	LuaModule(lua).fun("GetMapName", &GetMapName);

	LuaModule(lua).fun("GetCountParty", &GetCountParty);

	LuaModule(lua).fun("GetMonsterName", &GetMonsterName);
	
	LuaModule(lua).fun("GetExpPerMinute", &GetExpPerMinute);
	LuaModule(lua).fun("GetAvaregeExpPerMonster", &GetAvaregeExpPerMonster);
	LuaModule(lua).fun("GetLastExpReceive", &GetLastExpReceive);
	LuaModule(lua).fun("GetEtaLevelUp", &GetEtaLevelUp);
	LuaModule(lua).fun("GetHuntingLevel", &GetHuntingLevel);
	LuaModule(lua).fun("GetZenPerMinute", &GetZenPerMinute);
	LuaModule(lua).fun("GetLastZen", &GetLastZen);

	LuaModule(lua).fun("EnableDark", &LuaEnableAlphaBlendMinus);
	LuaModule(lua).fun("EnableColor", &LuaEnableAlphaBlend);
}
