//*****************************************************************************
// File: CharSelMainWin.cpp
//*****************************************************************************

#include "stdafx.h"
#include "CharSelMainWin.h"
#include "Input.h"
#include "UIMng.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "UIGuildInfo.h"
#include "ZzzOpenData.h"
#include "ZzzOpenglUtil.h"
#include "ServerListManager.h"
#include "CharacterList.h"
#include "NewUICommon.h"

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern bool LogOut;

CCharSelMainWin::CCharSelMainWin()
{

}

CCharSelMainWin::~CCharSelMainWin()
{

}

void CCharSelMainWin::Create()
{


	int offset = 0;
	offset = 190;

	m_asprBack[CSMW_SPR_DECO].Create(189, 103, BITMAP_LOG_IN+2);
	m_asprBack[CSMW_SPR_INFO].Create(
	CInput::Instance().GetScreenWidth() - 266, 21);
	m_asprBack[CSMW_SPR_INFO].SetColor(0, 0, 0);
	m_asprBack[CSMW_SPR_INFO].SetAlpha(143);



	m_aBtn[CSMW_BTN_CREATE].Create(90, 40, BITMAP_LOG_IN+3, 4, 2, 1, 3);
	//m_aBtn[CSMW_BTN_MENU].Create(54, 30, BITMAP_LOG_IN+4, 3, 2, 1);
	//m_aBtn[CSMW_BTN_CONNECT].Create(54, 30, BITMAP_LOG_IN+5, 4, 2, 1, 3);
	m_aBtn[CSMW_BTN_DELETE].Create(90, 40, BITMAP_LOG_IN+6, 4, 2, 1, 3);

	int offsetY = 0;

	if (WindowWidth == 800 && WindowHeight == 480) {
		offsetY = 200;
	}

	if (WindowWidth == 1024 && WindowHeight == 600) {
		offsetY = 260;
	}

	if (WindowWidth == 1200 && WindowHeight == 768) {
		offsetY = 330;
	}

	if (WindowWidth == 1280 && WindowHeight == 720) {
		offsetY = 308;
	}

	if (WindowWidth == 1364 && WindowHeight == 768) {
		offsetY = 332;
	}

	if (WindowWidth == 1400 && WindowHeight == 900) {
		offsetY = 456;
	}

	if (WindowWidth == 1600 && WindowHeight == 900) {
		offsetY = 456;
	}

	if (WindowWidth == 1920 && WindowHeight == 900) {
		offsetY = 456;
	}

	if (WindowWidth == 1920 && WindowHeight == 1080) {
		offsetY = 626;
	}


	CWin::Create(
		m_aBtn[0].GetWidth() * CSMW_BTN_MAX + m_asprBack[CSMW_SPR_INFO].GetWidth() + 6, offsetY, -2);

	for (int i = 0; i < CSMW_BTN_MAX; ++i)
		CWin::RegisterButton(&m_aBtn[i]);

	m_bAccountBlockItem = false;

	for (int i = 0; i < gCharacterList.MaxCharacters; ++i)
	{
		if (CharactersClient[i].Object.Live)
		{
			if (CharactersClient[i].CtlCode & CTLCODE_10ACCOUNT_BLOCKITEM)
			{
				m_bAccountBlockItem = true;
				break;
			}
		}
	}
}

void CCharSelMainWin::PreRelease()
{
	for (int i = 0; i < CSMW_SPR_MAX; ++i)
		m_asprBack[i].Release();
}

void CCharSelMainWin::SetPosition(int nXCoord, int nYCoord)
{
	CWin::SetPosition(nXCoord, nYCoord);

	int nBtnWidth = m_aBtn[0].GetWidth();


	int offsetX = 0;
	int offSetXDel = 0;


	if (WindowWidth == 800 && WindowHeight == 480) {
		offsetX = 620;
		offSetXDel = 25;
	}

	if (WindowWidth == 1024 && WindowHeight == 600) {
		offsetX = 805;
		offSetXDel = 20;
	}

	if (WindowWidth == 1200 && WindowHeight == 768) {
		offsetX = 930;
		offSetXDel = 3;
	}

	if (WindowWidth == 1280 && WindowHeight == 720) {
		offsetX = 1021;
		offSetXDel = 7;
	}

	if (WindowWidth == 1364 && WindowHeight == 768) {
		offsetX = 1090;
		offSetXDel = 1;
	}

	if (WindowWidth == 1400 && WindowHeight == 900) {
		offsetX = 1127;
		offSetXDel = 2;
	}

	if (WindowWidth == 1600 && WindowHeight == 900) {
		offsetX = 1327;
		offSetXDel = 2;
	}

	if (WindowWidth == 1920 && WindowHeight == 900) {
		offsetX = 1647;
		offSetXDel = 2;
	}

	if (WindowWidth == 1920 && WindowHeight == 1080) {
		offsetX = 1647;
		offSetXDel = 2;
	}
	


	m_aBtn[CSMW_BTN_CREATE].SetPosition(offsetX, nYCoord);

	m_aBtn[CSMW_BTN_MENU].SetPosition(nXCoord + nBtnWidth + 1, nYCoord);

	//m_asprBack[CSMW_SPR_INFO].SetPosition(m_aBtn[CSMW_BTN_MENU].GetXPos() + nBtnWidth + 2, nYCoord + 5);

	int nWinRPosX = nXCoord + CWin::GetWidth();
	//m_asprBack[CSMW_SPR_DECO].SetPosition(nWinRPosX - (m_asprBack[CSMW_SPR_DECO].GetWidth() - 22), nYCoord - 59);
	m_aBtn[CSMW_BTN_DELETE].SetPosition(nWinRPosX - 155 - nBtnWidth + offSetXDel, nYCoord);
	m_aBtn[CSMW_BTN_CONNECT].SetPosition(nWinRPosX - 70 - (nBtnWidth * 2 + 1), nYCoord);
}

void CCharSelMainWin::Show(bool bShow)
{
	CWin::Show(bShow);

	int i;
	for (i = 0; i < CSMW_SPR_MAX; ++i)
		m_asprBack[i].Show(bShow);
	for (i = 0; i < CSMW_BTN_MAX; ++i)
		m_aBtn[i].Show(bShow);
}

bool CCharSelMainWin::CursorInWin(int nArea)
{
	if (!CWin::m_bShow)
		return false;

	if (CUIMng::Instance().m_CharMakeWin.IsShow())
		return false;

	switch (nArea)
	{
	case WA_MOVE:
		return false;
	}

	return CWin::CursorInWin(nArea);
}

void CCharSelMainWin::UpdateDisplay()
{
	const bool bHideActions = LogOut || IsQuitGameNoticeActive();

	m_aBtn[CSMW_BTN_CREATE].Show(!bHideActions);
	m_aBtn[CSMW_BTN_DELETE].Show(!bHideActions);

	if (bHideActions)
	{
		m_aBtn[CSMW_BTN_CREATE].SetEnable(false);
		m_aBtn[CSMW_BTN_DELETE].SetEnable(false);
	}

	m_aBtn[CSMW_BTN_CREATE].SetEnable(false);
	int i=0;

	if (!bHideActions)
	{
		for (i = 0; i < gCharacterList.MaxCharacters; ++i)
		{
			if (!CharactersClient[i].Object.Live)
			{
				m_aBtn[CSMW_BTN_CREATE].SetEnable(true);
				break;
			}
		}
	}

	if (SelectedHero > -1)
	{
		m_aBtn[CSMW_BTN_CONNECT].SetEnable(true);
		m_aBtn[CSMW_BTN_DELETE].SetEnable(!bHideActions);
	}
	else
	{
		m_aBtn[CSMW_BTN_CONNECT].SetEnable(false);
		m_aBtn[CSMW_BTN_DELETE].SetEnable(false);
	}


	bool bNobodyCharacter = true;

	for (int i = 0; i < gCharacterList.MaxCharacters; ++i)
	{
		if (CharactersClient[i].Object.Live == true)
		{
			bNobodyCharacter = false;
			break;
		}
	}
	
	if (bNobodyCharacter == true && !bHideActions)
	{
		CUIMng& rUIMng = CUIMng::Instance();
		if (!rUIMng.m_CharMakeWin.IsShow())
		{
			rUIMng.ShowWin(&rUIMng.m_CharMakeWin);
		}
	}
}

void CCharSelMainWin::UpdateWhileActive(double dDeltaTick)
{
	if (CUIMng::Instance().m_CharMakeWin.IsShow())
		return;

	if (m_aBtn[CSMW_BTN_CONNECT].IsClick())
		::StartGame();
	else if (m_aBtn[CSMW_BTN_MENU].IsClick())
	{
		CUIMng::Instance().SetSysMenuWinShow(false);
	}
	else if (!IsQuitGameNoticeActive() && !LogOut && m_aBtn[CSMW_BTN_CREATE].IsClick())
	{
		CUIMng& rUIMng = CUIMng::Instance();
		rUIMng.ShowWin(&rUIMng.m_CharMakeWin);

	}
	else if (!IsQuitGameNoticeActive() && !LogOut && m_aBtn[CSMW_BTN_DELETE].IsClick())
		DeleteCharacter();
}

void CCharSelMainWin::RenderControls()
{
	if (CUIMng::Instance().m_CharMakeWin.IsShow())
		return;

	for (int i = 0; i < CSMW_SPR_MAX; ++i)
		m_asprBack[i].Render();

	::EnableAlphaTest();
	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	g_pRenderText->SetFont(g_hFixFont);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->SetBgColor(0);

	if (m_bAccountBlockItem)
	{
		g_pRenderText->SetTextColor(0, 0, 0, 255);
		g_pRenderText->SetBgColor(255, 255, 0, 128);
		g_pRenderText->RenderText(320, 330, GlobalText[436], 0, 0, RT3_WRITE_CENTER);
		g_pRenderText->RenderText(320, 348, GlobalText[437], 0, 0, RT3_WRITE_CENTER);
	}
	CWin::RenderButtons();
}

void CCharSelMainWin::DeleteCharacter()
{
    if (CharactersClient[SelectedHero].GuildStatus != G_NONE)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_GUILDWARNING);
	else if (CharactersClient[SelectedHero].CtlCode	& (CTLCODE_02BLOCKITEM | CTLCODE_10ACCOUNT_BLOCKITEM))
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_ID_BLOCK);
	else
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_CONFIRM);
}
