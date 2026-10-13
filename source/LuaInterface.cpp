#include "stdafx.h"
#include "LuaInterface.h"
#include "LuaStack.hpp"
#include "ZzzOpenglUtil.h"
#include "UIControls.h"
#include "NewUISystem.h"
#include "WideScreen.h"
#include "ZzzInterface.h"
#include "DSPlaySound.h"
#include "UIMng.h"

using namespace luaaa;

int MousePosX() {
	return MouseX;
}

int MousePosY() {
	return MouseY;
}

void LuaglColor3f(float r, float g, float b) {
	glColor3f(r, g, b);
}

void LuaglColor4f(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void LuaEnableAlphaTest() {
	EnableAlphaTest();
}

void LuaDisableAlphaBlend() {
	DisableAlphaBlend();
}

void LuaSetCustomWIndow(int value)
{
	g_pNewUISystem->CustomLuaWindow = value;

	int iScreenWidth = GetScreenWidth();
	g_pItemEnduranceInfo->SetPos(iScreenWidth);
	g_pBuffWindow->SetPos(iScreenWidth);
	g_pPartyListWindow->SetPos(iScreenWidth);
}

void LuaDrawBar(float x, float y, float w, float h) {
	RenderColor(x, y, w, h);
}

void LuaDrawBarRadius(float x, float y, float w, float h, float radius) {
	RenderColorRadius(x, y, w, h, 0, 0, radius);
}

void LuaDrawBarRadiusBorder(float x, float y, float w, float h, float radius, float strokeWidth, float strokeR, float strokeG, float strokeB, float strokeA) {
	RenderShapeWithStroke(x, y, w, h, 0, 0, radius, strokeWidth, strokeR, strokeG, strokeB, strokeA);
}

void LuaEndDrawBar() {
	EndRenderColor();
}

void LuaSetFontType(int font) {
	switch (font) {
	case 0:
		g_pRenderText->SetFont(g_hFont);
		break;
	case 1:
		g_pRenderText->SetFont(g_hFontBold);
		break;
	case 2:
		g_pRenderText->SetFont(g_hFontBig);
		break;
	case 3:
		g_pRenderText->SetFont(g_hFixFont);
		break;
	case 4:
		g_pRenderText->SetFont(g_hFontSmall);
		break;
	case 5:
		g_pRenderText->SetFont(g_hFontMedium);
		break;
	case 6:
		g_pRenderText->SetFont(g_hFontMediumBold);
		break;
	case 7:
		g_pRenderText->SetFont(g_hFontBigBold);
		break;
	default:
		if (gCreateFont.m_newFont.count(font) != 0) {
			g_pRenderText->SetFont(gCreateFont.m_newFont[font]);
		}
		else {
			g_pRenderText->SetFont(g_hFont);
		}
		break;
	}
}

void LuaSetFontBg(int r, int g, int b, int a) {
	g_pRenderText->SetBgColor(RGBA(r,g,b,a));
}

void LuaSetTextColor(int r, int g, int b, int a) {
	g_pRenderText->SetTextColor(RGBA(r, g, b, a));
}

void LuaRenderText(int x, int y, char* text, int width, int sort) {
	g_pRenderText->RenderText(x, y, text, width, 0, sort);
}

void LuaRenderText2(int x, int y, char* text, int sort) {
	g_pRenderText->RenderText(x, y, text, 0, 0, sort);
}

int LuaGetTextPosY(int font, char* text, int y, int height) {
	SIZE Fontsize;
	switch (font) {
	case 0:
		g_pRenderText->SetFont(g_hFont);
		break;
	case 1:
		g_pRenderText->SetFont(g_hFontBold);
		break;
	case 2:
		g_pRenderText->SetFont(g_hFontBig);
		break;
	case 3:
		g_pRenderText->SetFont(g_hFixFont);
		break;
	case 4:
		g_pRenderText->SetFont(g_hFontSmall);
		break;
	case 5:
		g_pRenderText->SetFont(g_hFontMedium);
		break;
	case 6:
		g_pRenderText->SetFont(g_hFontMediumBold);
		break;
	case 7:
		g_pRenderText->SetFont(g_hFontBigBold);
		break;
	default:
		g_pRenderText->SetFont(g_hFont);
		break;
	}
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), text, sizeof(text), &Fontsize);

	Fontsize.cy = Fontsize.cy / ((float)WindowHeight / 480);

	int posy = y + ((height / 2) - (Fontsize.cy / 2));

	return posy;
}

void LuaRenderImage(int imageIndex, float x, float y, float w, float h) {
	SEASON3B::RenderImage(imageIndex, x, y, w, h);
}

void LuaRenderBitmap(int Texture, float x, float y, float w, float h, float u, float v, float uWidth, float vHeight, int Scale, int StartScale, float Alpha) {
	RenderBitmap(Texture, x, y, w, h, u, v, uWidth, vHeight, Scale, StartScale, Alpha);
}

void LuaRenderImageScale(int Texture, float x, float y, float w, float h, float u, float v, float uWidth, float vHeight, float Alpha) {
	SEASON3B::RenderImageScale(Texture, x, y, w, h, u, v, uWidth, vHeight, Alpha);
}

void LuaRenderImageScaleAuto(int Texture, float x, float y, float w, float h) {
	SEASON3B::RenderImageScaleAuto(Texture, x, y, w, h);
}

int LuaCheckIsRepeatKey(int Key) {
	if (g_hWnd != GetActiveWindow()) {
		return 0;
	}
	return SEASON3B::IsRepeat(Key);
}

int LuaCheckPressedKey(int Key) {
	if (g_hWnd != GetActiveWindow()) {
		return 0;
	}
	return SEASON3B::IsPress(Key);
}

int LuaCheckReleasedKey(int Key) {
	if (g_hWnd != GetActiveWindow()) {
		return 0;
	}
	return SEASON3B::IsRelease(Key);
}

int LuaCheckWindowOpen(int Key) {
	return g_pNewUISystem->IsVisible(Key);
}

void LuaCloseWindow(int Key) {
	g_pNewUISystem->Hide(Key);
}

void LuaOpenWindow(int Key) {
	g_pNewUISystem->Show(Key);
}

void LuaCloseLegacySysMenu() {
	CUIMng& rUIMng = CUIMng::Instance();
	rUIMng.SetSysMenuWinShow(false);
	rUIMng.HideWin(&rUIMng.m_SysMenuWin);
	rUIMng.HideWin(&rUIMng.m_OptionWin);
}

void LuaResetMouseL() {
	MouseLButton = false;
	MouseLButtonPop = false;
	MouseLButtonPush = false;
}

void LuaResetMouseR() {
	MouseRButton = false;
	MouseRButtonPop = false;
	MouseRButtonPush = false;
}

void LuaResetMouseM() {
	MouseMButton = false;
	MouseMButtonPop = false;
	MouseMButtonPush = false;
}

void LuaDisableCLick() {
	BlockClick = true;
	LuaResetMouseL();
	LuaResetMouseR();
	LuaResetMouseM();
}

int LuaGetWindowWidth() {
	return gWideScreen.g_WideWindowWidth;
}

int LuaGetWindowWidthAdd() {
	return gWideScreen.g_WideWindowWidthAdd;
}

int LuaGetWindowHeight() {
	return gWideScreen.g_WideWindowHeight;
}

int LuaGetWindowHeightAdd() {
	return gWideScreen.g_WideWindowHeightAdd;
}

float LuaGetImageWidth(int texture) {
	BITMAP_t* pImage = &Bitmaps[texture];
	return pImage->Width;
}

float LuaGetImageHeight(int texture) {
	BITMAP_t* pImage = &Bitmaps[texture];
	return pImage->Height;
}

void ItemByteConvert(BYTE* lpMsg, int ItemIndex, int Level, int dur, int Op1, int Op2, int Op3, int Exc, int Ancient, int JoH, int Socket, BYTE Sock1, BYTE Sock2, BYTE Sock3, BYTE Sock4, BYTE Sock5)
{
	lpMsg[0] = ItemIndex & 0xFF;

	lpMsg[1] = 0;
	lpMsg[1] |= Level * 8;
	lpMsg[1] |= Op1 * 128;
	lpMsg[1] |= Op2 * 4;
	lpMsg[1] |= Op3 & 3;

	lpMsg[2] = dur;

	lpMsg[3] = 0;
	lpMsg[3] |= (ItemIndex & 256) >> 1;
	lpMsg[3] |= ((Op3 > 3) ? 64 : 0);
	lpMsg[3] |= Exc;

	lpMsg[4] = Ancient;

	lpMsg[5] = 0;
	lpMsg[5] |= (ItemIndex & gItemManager.GET_ITEM(15, 0)) >> 5;
	lpMsg[5] |= ((0 & 128) >> 4);
	lpMsg[5] |= ((0 & 1) << 1);
	lpMsg[5] |= ((0 & 1) << 2);

	//if (JoH) {
		lpMsg[6] = JoH;
	//}
	//else {
	//	lpMsg[6] = Socket;
	//}

	lpMsg[7] = Sock1;
	lpMsg[8] = Sock2;
	lpMsg[9] = Sock3;
	lpMsg[10] = Sock4;
	lpMsg[11] = Sock5;
}

void LuaRenderTooltipComplete(int x, int y, int ItemIndex, int Level, int dur, int Op1, int Op2, int Op3, int Exc, int Ancient, int JoH, int Socket, int Sock1, int Sock2, int Sock3, int Sock4, int Sock5) {
	EnableAlphaTest(1);

	static BYTE ItemBuffer[12];
	std::memset(&ItemBuffer[0], 0, sizeof(ItemBuffer));

	ItemByteConvert(ItemBuffer, ItemIndex, Level, dur, Op1, Op2, Op3, Exc, Ancient, JoH, Socket, Sock1, Sock2, Sock3, Sock4, Sock5);

	auto m_Item = g_pNewItemMng->CreateItem(ItemBuffer);

	RenderItemInfo(x, y, m_Item, false);

	DisableAlphaBlend();

	if (m_Item) {
		g_pNewItemMng->DeleteItem(m_Item);
		m_Item = nullptr;
	}
}

void LuaRenderTooltip(int x, int y, int ItemIndex, int Level, int Skill, int Luck, int JoL, int Dur, int Exc, int Anc) {
	EnableAlphaTest(1);

	static BYTE ItemBuffer[12];
	std::memset(&ItemBuffer[0], 0, sizeof(ItemBuffer));

	ItemByteConvert(ItemBuffer, ItemIndex, Level, Dur, Skill, Luck, JoL, Exc, Anc, 0, 255, 255, 255, 255, 255, 255);

	auto m_Item = g_pNewItemMng->CreateItem(ItemBuffer);

	RenderItemInfo(x, y, m_Item, false);

	DisableAlphaBlend();

	if (m_Item) {
		g_pNewItemMng->DeleteItem(m_Item);
		m_Item = nullptr;
	}
}

void LuaRenderItem(float sx, float sy, float w, float h, int Type, int Level, int Option1, int ExtOption, float size) {
	EndBitmap();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(size, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	float Width = 0;
	float Height = 0;

	if (w > 0 && h > 0) {
		Width = (float)((float)ItemAttribute[Type].Width * INVENTORY_SCALE)/size;
		Height = (float)((float)ItemAttribute[Type].Height * INVENTORY_SCALE)/size;
	}

	RenderItem3D(sx, sy, Width, Height, Type, Level*8, Option1, ExtOption, false);

	UpdateMousePositionn();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	BeginBitmap();
}

void LuaSendMessageClient(char* text) {
	g_pChatListBox->AddText("", text, SEASON3B::TYPE_SYSTEM_MESSAGE);
}

void LuaSendMessageClientError(std::string msg) {
	g_pChatListBox->AddText("", msg.c_str(), SEASON3B::TYPE_ERROR_MESSAGE);
}

void LuaLockPlayerWalk() {
	LockPlayerWalk = true;
}

void LuaUnlockPlayerWalk() {
	LockPlayerWalk = false;
}

void LuaDrawTooltip(int x, int y, char* text) {
	RenderTipText(x, y, text);
}

void LuaSetLockInterfaces() {
	ErrorMessage = 1;
}

void LuaSetUnlockInterfaces() {
	ErrorMessage = 0;
}

std::string LuaGetLanguage() {
	return g_strSelectedML;
}

void LuaPlaySound(int id) {
	PlayBuffer(id);
}

void LuaStopSound(int id) {
	StopBuffer(id, true);
}

void LuaSetBlend()
{
	EnableAlphaTest();
}

void LuaInterface::RegisterInterface(lua_State* Lua)
{
	LuaModule(Lua).fun("RenderText", &LuaRenderText);
	LuaModule(Lua).fun("RenderText2", &LuaRenderText2);
	LuaModule(Lua).fun("RenderText3", &LuaRenderText);
	LuaModule(Lua).fun("EnableAlphaTest", &LuaEnableAlphaTest);
	LuaModule(Lua).fun("DisableAlphaBlend", &LuaDisableAlphaBlend);
	LuaModule(Lua).fun("glColor3f", &LuaglColor3f);
	LuaModule(Lua).fun("glColor4f", &LuaglColor4f);
	LuaModule(Lua).fun("DrawBar", &LuaDrawBar);
	LuaModule(Lua).fun("DrawBarRadius", &LuaDrawBarRadius);
	LuaModule(Lua).fun("DrawBarRadiusBorder", &LuaDrawBarRadiusBorder);
	LuaModule(Lua).fun("SetBlend", &LuaSetBlend);
	LuaModule(Lua).fun("EndDrawBar", LuaEndDrawBar);
	LuaModule(Lua).fun("SetFontType", LuaSetFontType);
	LuaModule(Lua).fun("SetTextBg", LuaSetFontBg);
	LuaModule(Lua).fun("SetTextColor", LuaSetTextColor);
	LuaModule(Lua).fun("GetTextPosY", LuaGetTextPosY);
	LuaModule(Lua).fun("PlaySound", &LuaPlaySound);
	LuaModule(Lua).fun("StopSound", &LuaStopSound);
	LuaModule(Lua).fun("MousePosX", &MousePosX);
	LuaModule(Lua).fun("MousePosY", &MousePosY);
	LuaModule(Lua).fun("RenderImage", &LuaRenderImage);
	LuaModule(Lua).fun("RenderImage2", &LuaRenderBitmap);
	LuaModule(Lua).fun("RenderImage3", &LuaRenderImageScale);
	LuaModule(Lua).fun("RenderImageScale", &LuaRenderImageScale);
	LuaModule(Lua).fun("RenderImageAuto", &LuaRenderImageScaleAuto);
	LuaModule(Lua).fun("CheckRepeatKey", &LuaCheckIsRepeatKey);
	LuaModule(Lua).fun("CheckPressedKey", &LuaCheckPressedKey);
	LuaModule(Lua).fun("CheckReleasedKey", &LuaCheckReleasedKey);
	LuaModule(Lua).fun("CheckWindowOpen", &LuaCheckWindowOpen);
	LuaModule(Lua).fun("CloseWindow", &LuaCloseWindow);
	LuaModule(Lua).fun("OpenWindow", &LuaOpenWindow);
	LuaModule(Lua).fun("CloseLegacySysMenu", &LuaCloseLegacySysMenu);
	LuaModule(Lua).fun("ResetMouseL", &LuaResetMouseL);
	LuaModule(Lua).fun("ResetMouseR", &LuaResetMouseR);
	LuaModule(Lua).fun("ResetMouseM", &LuaResetMouseM);
	LuaModule(Lua).fun("GetImageWidth", &LuaGetImageWidth);
	LuaModule(Lua).fun("GetImageHeight", &LuaGetImageHeight);
	LuaModule(Lua).fun("WindowGetWidth", &LuaGetWindowWidth);
	LuaModule(Lua).fun("WindowGetWidthAdd", &LuaGetWindowWidthAdd);
	LuaModule(Lua).fun("WindowGetHeight", &LuaGetWindowHeight);
	LuaModule(Lua).fun("WindowGetHeightAdd", &LuaGetWindowHeightAdd);
	LuaModule(Lua).fun("ShowDescriptionComplete", &LuaRenderTooltipComplete);
	LuaModule(Lua).fun("ShowItemDescription", &LuaRenderTooltip);
	LuaModule(Lua).fun("SendMessageClient", &LuaSendMessageClient);
	LuaModule(Lua).fun("LogDebug", &LuaSendMessageClient);
	LuaModule(Lua).fun("LogAdd", &LuaSendMessageClient);
	LuaModule(Lua).fun("DisableClickClient", &LuaDisableCLick);
	LuaModule(Lua).fun("LockPlayerWalk", &LuaLockPlayerWalk);
	LuaModule(Lua).fun("UnlockPlayerWalk", &LuaUnlockPlayerWalk);
	LuaModule(Lua).fun("CreateItem", &LuaRenderItem);
	LuaModule(Lua).fun("DrawTooltip", &LuaDrawTooltip);
	LuaModule(Lua).fun("SetLockInterfaces", &LuaSetLockInterfaces);
	LuaModule(Lua).fun("SetUnlockInterfaces", &LuaSetUnlockInterfaces);
	LuaModule(Lua).fun("GetLanguage", &LuaGetLanguage);
	LuaModule(Lua).fun("SetCustomWindow", &LuaSetCustomWIndow);
	LuaModule(Lua).fun("SendMessageClientError", &LuaSendMessageClientError);
	LuaModule(Lua).fun("LogDebugError", &LuaSendMessageClientError);
	LuaModule(Lua).fun("LogAddError", &LuaSendMessageClientError);
	LuaModule(Lua).fun("GLSwitchBlend", &LuaDisableAlphaBlend);
	LuaModule(Lua).fun("GLSwitch", &LuaDisableAlphaBlend);
}
