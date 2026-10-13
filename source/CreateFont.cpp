#include "stdafx.h"
#include "CreateFont.h"
#include "LuaStack.hpp"

using namespace luaaa;
CCreateFont gCreateFont;

CCreateFont::CCreateFont() : m_Lua()
{
}

CCreateFont::~CCreateFont()
{
}

void AddFont(int index, int bold, int addSize, int charSet, std::string name, float multiSize, int italico, int sublinhado, int riscado) {

	FONT_INFO info;
	info.Index = index;
	info.Bold = bold;
	info.AddSize = addSize;
	info.CharSet = charSet;
	info.MultiSize = multiSize;
	info.Italico = italico;
	info.Sublinhado = sublinhado;
	info.Riscado = riscado;

	gCreateFont.m_fontInfo[index] = info;
}


void CCreateFont::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("SetFont", &AddFont);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\Font.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

void CCreateFont::ReloadLua()
{
	LuaModule(this->m_Lua.GetState()).fun("SetFont", &AddFont);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\Font.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);
}

void CCreateFont::SetFont(int iFontSize, int FontHeight, int nFixFontSize, int nFixFontHeight) {

	g_hFont = CreateFont(iFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontBold = CreateFont(iFontSize, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontBig = CreateFont(iFontSize * 2, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFixFont = CreateFont(nFixFontSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[18][0] ? GlobalText[18] : NULL);

	g_hFontSmall = CreateFont(iFontSize - 2, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontMedium = CreateFont(iFontSize * 1.2, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontMediumBold = CreateFont(iFontSize * 1.2, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);
	g_hFontBigBold = CreateFont(iFontSize * 1.5, 0, 0, 0, FW_EXTRABOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GlobalText[0][0] ? GlobalText[0] : NULL);


	for (auto const& fontInfo : gCreateFont.m_fontInfo)
	{
		auto newFont = CreateFont(std::ceil(iFontSize * fontInfo.second.MultiSize) + fontInfo.second.AddSize, 0, 0, 0, fontInfo.second.Bold, fontInfo.second.Italico, fontInfo.second.Sublinhado, fontInfo.second.Riscado, fontInfo.second.CharSet, 0, 0, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontInfo.second.Name.c_str());
		this->m_newFont[fontInfo.second.Index] = newFont; 
	}
}