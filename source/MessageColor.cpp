#include "stdafx.h"
#include "MessageColor.h"
#include "LuaStack.hpp"
#include "UIControls.h"

using namespace luaaa;

CMessageColor gMessageColor;

CMessageColor::CMessageColor() : m_Lua()
{
	this->m_CustomMessageColorInfo.clear();
}

CMessageColor::~CMessageColor()
{
	this->m_CustomMessageColorInfo.clear();
}

bool CMessageColor::CheckIsColor(int Color, int Type)
{
	for (std::map<int, CUSTOM_MESSAGE_COLOR_INFO>::iterator it = this->m_CustomMessageColorInfo.begin(); it != this->m_CustomMessageColorInfo.end(); ++it)
	{
		if (it->second.MessageID == Color) 
		{
			return 1;
		}
	}

	return 0;
}

void CMessageColor::SetMessageColor(int m_Message)
{
	auto msgInfo = gMessageColor.getInfo(m_Message);

	if (msgInfo) 
	{
		g_pRenderText->SetBgColor(msgInfo->BgR, msgInfo->BgG, msgInfo->BgB, msgInfo->BgA);
		g_pRenderText->SetTextColor(msgInfo->TextR, msgInfo->TextG, msgInfo->TextB, msgInfo->TextA);
	}
}

void SetCustomMessageColor(int index, int type, int textR, int textG, int textB, int textA, int bgR, int bgG, int bgB, int bgA) 
{
	CUSTOM_MESSAGE_COLOR_INFO info;

	info.MessageID = index;

	info.Type = type;

	info.TextR = textR;

	info.TextG = textG;

	info.TextB = textB;

	info.TextA = textA;

	info.BgR = bgR;

	info.BgG = bgG;

	info.BgB = bgB;

	info.BgA = bgA;

	gMessageColor.m_CustomMessageColorInfo.insert(std::make_pair(info.MessageID, info));
}

void CMessageColor::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetCustomMessageColor", &SetCustomMessageColor);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\MessageColors.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadCustomMessageColors", "");
}

CUSTOM_MESSAGE_COLOR_INFO * CMessageColor::getInfo(int messageID)
{
	std::map<int, CUSTOM_MESSAGE_COLOR_INFO>::iterator it = this->m_CustomMessageColorInfo.find(messageID);

	if (it == this->m_CustomMessageColorInfo.end()) 
	{
		return 0;
	}

	return &it->second;
}
