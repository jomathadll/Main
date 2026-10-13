// NewUIPetInformation.cpp: imp0lementation of the CNewUIPetInfoWindow class.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIPetInfoWindow.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "GIPetManager.h"
#include "CharacterManager.h"
#include "SkillManager.h"

using namespace SEASON3B;

CNewUIPetInfoWindow::CNewUIPetInfoWindow() 
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_iNumCurOpenTab = TAB_TYPE_DARKHORSE;
	CalcDamage(m_iNumCurOpenTab);
	m_EventState = EVENT_NONE;

}

CNewUIPetInfoWindow::~CNewUIPetInfoWindow() 
{ 
	Release(); 
}

bool CNewUIPetInfoWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_PET, this );
	
	SetPos(x, y);
	
	LoadImages();
	
	InitButtons( );
	
	Show(false);
	
	return true;
}

void CNewUIPetInfoWindow::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CNewUIPetInfoWindow::InitButtons( )
{
	
	// Exit Button
	m_BtnExit.ChangeButtonImgState(true, IMAGE_PETINFO_BTN_EXIT, false);
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	std::list<unicode::t_string> ltext;
	
	ltext.push_back( GlobalText[1187] );
	ltext.push_back( GlobalText[1214] );

	// Tab Button	
	m_BtnTab.CreateRadioGroup( 2, IMAGE_PETINFO_TAB_BUTTON );
	m_BtnTab.ChangeRadioText( ltext );
	m_BtnTab.ChangeRadioButtonInfo( true, m_Pos.x+12.f, m_Pos.y+48.f, 56, 22 );
	m_BtnTab.ChangeFrame( m_iNumCurOpenTab );
	m_BtnExit.ChangeButtonInfo( m_Pos.x+13, m_Pos.y+392, 36, 29 );		

#elif CLIENT_VERSION >= 1001
	m_BtnTab.CreateRadioGroup(2, BITMAP_UNKNOWN);
	m_BtnTab.ChangeRadioButtonInfo(true, m_Pos.x + 10.f, m_Pos.y + 27, 83, 22);
	m_BtnTab.ChangeFrame(m_iNumCurOpenTab);
	m_BtnExit.ChangeButtonInfo( m_Pos.x+23, m_Pos.y+392, 25, 24 );		

#endif
	m_BtnExit.ChangeToolTipText(GlobalText[1002], true);	// 1002 "´Ý±â"

}

void CNewUIPetInfoWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_BtnTab.ChangeRadioButtonInfo( true, m_Pos.x+12.f, m_Pos.y+48.f, 56, 22 );
	m_BtnExit.ChangeButtonInfo(m_Pos.x + 13, m_Pos.y + 392, 36, 29);

#elif CLIENT_VERSION >= 1001
	m_BtnTab.ChangeRadioButtonInfo(true, m_Pos.x + 10.f, m_Pos.y + 27, 83, 22);
	m_BtnExit.ChangeButtonInfo(m_Pos.x + 23, m_Pos.y + 392, 25, 24);

#endif
}

bool CNewUIPetInfoWindow::UpdateMouseEvent()
{
	if( true == BtnProcess() )
		return false;
	
	if(CheckMouseIn(m_Pos.x, m_Pos.y, PETINFOWINDOW_WIDTH, PETINFOWINDOW_HEIGHT))
		return false;
	
	return true;
}

bool CNewUIPetInfoWindow::UpdateKeyEvent()
{
	return true;
}

bool CNewUIPetInfoWindow::Update()
{		
	if( IsVisible() == false )
		return true;

	int iNumCurOpenTab = m_BtnTab.UpdateMouseEvent();

	if( (iNumCurOpenTab != RADIOGROUPEVENT_NONE) && (m_iNumCurOpenTab != iNumCurOpenTab) )
	{
		CalcDamage(iNumCurOpenTab);	
		m_iNumCurOpenTab = iNumCurOpenTab;
	}
	return true;
}

bool CNewUIPetInfoWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	g_pRenderText->SetFont( g_hFont );
	g_pRenderText->SetTextColor(0xFFFFFFFF);
	
	RenderImage(IMAGE_PETINFO_BACK, m_Pos.x, m_Pos.y, float(PETINFOWINDOW_WIDTH), float(PETINFOWINDOW_HEIGHT));
	RenderImage(IMAGE_PETINFO_TOP, m_Pos.x, m_Pos.y, float(PETINFOWINDOW_WIDTH), 64.f);
	RenderImage(IMAGE_PETINFO_LEFT, m_Pos.x, m_Pos.y+64.f, 21.f, float(PETINFOWINDOW_HEIGHT)-64.f-45.f);
	RenderImage(IMAGE_PETINFO_RIGHT, m_Pos.x+float(PETINFOWINDOW_WIDTH)-21.f, m_Pos.y+64.f, 21.f, float(PETINFOWINDOW_HEIGHT)-64.f-45.f);
	RenderImage(IMAGE_PETINFO_BOTTOM, m_Pos.x, m_Pos.y+float(PETINFOWINDOW_HEIGHT)-45.f, float(PETINFOWINDOW_WIDTH), 45.f);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 13, GlobalText[1217], 70, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFont);
	m_BtnTab.Render();

#elif CLIENT_VERSION >= 1001
	//RenderBitmap(IMAGE_PETINFO_BACK_VOID, m_Pos.x, m_Pos.y, float(PETINFOWINDOW_WIDTH), float(PETINFOWINDOW_HEIGHT), 0.0, 0.0, 0.83, 0.935, 1, 1, 0.0);
	RenderImageScaleAuto(IMAGE_PETINFO_BACK_VOID, m_Pos.x, m_Pos.y, float(PETINFOWINDOW_WIDTH), float(PETINFOWINDOW_HEIGHT));

	//RenderImageScale(IMAGE_PETINFO_TAB_BACK, m_Pos.x + 10, m_Pos.y + 27, 166.f, 22.f, 0.f, 0.f, 332.f, 44.f);
	RenderImageScaleAuto(IMAGE_PETINFO_TAB_BACK, m_Pos.x + 10, m_Pos.y + 27, 166.f, 22.f);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255,255,255,255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 14, GlobalText[1217], 190, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFont);

	if (m_iNumCurOpenTab == 0) {
		RenderImageScale(IMAGE_PETINFO_TAB_ON, m_Pos.x + 11.5f, m_Pos.y + 32.f, 80.f, 16.f, 0.f, 0.f, 160.f, 30.f);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	}
	else {
		g_pRenderText->SetTextColor(181, 181, 181, 181);
	}
	g_pRenderText->RenderText(m_Pos.x + 12, m_Pos.y + 34, GlobalText[1187], 80, 0, RT3_SORT_CENTER);
	
	if (m_iNumCurOpenTab == 1) {
		RenderImageScale(IMAGE_PETINFO_TAB_ON, m_Pos.x + 11.5f+83.f, m_Pos.y + 32.f, 80.f, 16.f, 0.f, 0.f, 160.f, 30.f);
		g_pRenderText->SetTextColor(255, 255, 255, 255);
	}
	else {
		g_pRenderText->SetTextColor(181, 181, 181, 181);
	}
	g_pRenderText->RenderText(m_Pos.x + 95, m_Pos.y + 34, GlobalText[1214], 80, 0, RT3_SORT_CENTER);

#endif
	PET_INFO* pPetInfo = NULL;

	switch (m_iNumCurOpenTab)
	{
	case TAB_TYPE_DARKHORSE:
	{
		pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_HORSE);

		if (pPetInfo->m_dwPetType == PET_TYPE_NONE)
		{
			g_pRenderText->SetTextColor(0xFF0000FF);
			g_pRenderText->SetFont(g_hFontBold);
			unicode::t_char szText[256] = { 0, };
			unicode::_sprintf(szText, GlobalText[1233], GlobalText[1187]);
			g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 100, szText, 160, 30, RT3_SORT_CENTER);
		}
		else
		{
			RenderDarkHorseInfo(pPetInfo);
		}
	}
	break;
	case TAB_TYPE_DARKSPIRIT:
	{
		pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_SPIRIT);

		if (pPetInfo->m_dwPetType == PET_TYPE_NONE)
		{
			g_pRenderText->SetTextColor(0xFF0000FF);
			g_pRenderText->SetFont(g_hFontBold);
			g_pRenderText->RenderText(m_Pos.x + 15, m_Pos.y + 100, GlobalText[1169], 160, 30, RT3_SORT_CENTER);
		}
		else
		{
			RenderDarkSpiritInfo(pPetInfo);
		}
	}
	break;
	}


	// Exit Button
	m_BtnExit.Render();

	DisableAlphaBlend();
	
	return true;
}

bool CNewUIPetInfoWindow::RenderDarkHorseInfo(PET_INFO* pPetInfo)
{
	unicode::t_char szText[256] = { 0, };	

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 60, m_Pos.y + 25, GlobalText[1187], 70, 0, RT3_SORT_CENTER);

	int iPosX = m_Pos.x + 12;
	int iPosY = m_Pos.y + 75;

	RenderGroupBox(iPosX, iPosY, 168, 105);

	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->SetFont(g_hFontBold);

	unicode::_sprintf(szText, GlobalText[200], pPetInfo->m_wLevel);
	g_pRenderText->RenderText(iPosX + 2, iPosY + 8, szText, 70 - 14, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	unicode::_sprintf(szText, GlobalText[358], pPetInfo->m_wLife, 255);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 28, szText, 0, 0, RT3_SORT_CENTER);
	RenderImage(IMAGE_PETINFO_LIFEBAR, iPosX + 7, iPosY + 40, 151, 12);
	int iHP = (min(pPetInfo->m_wLife, 255) * 147) / 255;
	RenderImage(IMAGE_PETINFO_LIFE, iPosX + 9, iPosY + 42, iHP, 8);
	unicode::_sprintf(szText, GlobalText[357], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 59, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[203], m_aiDamage[0], m_aiDamage[1], pPetInfo->m_wAttackSuccess);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 72, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[64], pPetInfo->m_wAttackSpeed);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 85, szText, 0, 0, RT3_SORT_CENTER);

#elif CLIENT_VERSION >= 1001

	int iPosX = m_Pos.x + 11;
	int iPosY = m_Pos.y + 60; 

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	RenderColor(iPosX, iPosY, 166, 15);
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	RenderColor(iPosX, iPosY + 15, 166, 85);
	EndRenderColor();

	g_pRenderText->SetFont(g_hFontMediumBold);
	g_pRenderText->SetTextColor(247, 128, 0, 255);

	unicode::_sprintf(szText, GlobalText[200], pPetInfo->m_wLevel);
	g_pRenderText->RenderText(iPosX, iPosY + 2, szText, 166, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);


	unicode::_sprintf(szText, GlobalText[357], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 25, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[203], m_aiDamage[0], m_aiDamage[1], pPetInfo->m_wAttackSuccess);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 40, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[64], pPetInfo->m_wAttackSpeed);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 55, szText, 0, 0, RT3_SORT_CENTER);


	int iHP = (min(pPetInfo->m_wLife, 255) * 152) / 255;
	RenderImageScale(IMAGE_PETINFO_LIFEBAR, iPosX + 7, iPosY + 73, 152, 6, 0.f, 0.f, 333.f, 14.f);
	RenderImageScale(IMAGE_PETINFO_LIFE, iPosX + 7, iPosY + 73, iHP, 6, 0.f, 0.f, 333.f, 14.f);

	g_pRenderText->SetFont(g_hFontSmall);
	g_pRenderText->SetTextColor(RGBA(237, 214, 161, 255));
	unicode::_sprintf(szText, GlobalText[358], pPetInfo->m_wLife, 255);
	g_pRenderText->RenderText(iPosX+7+152, iPosY + 83, szText, 152, 0, RT3_WRITE_RIGHT_TO_LEFT);
	

#endif
	return true;

}

bool CNewUIPetInfoWindow::RenderDarkSpiritInfo(PET_INFO* pPetInfo)
{
	unicode::t_char szText[256] = { 0, };

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	
	g_pRenderText->SetFont(g_hFont);	
	g_pRenderText->SetTextColor( 255, 255, 255, 255 );
	g_pRenderText->RenderText(m_Pos.x+60, m_Pos.y+25, GlobalText[1214], 70, 0, RT3_SORT_CENTER);

	int iGBox1PosX = m_Pos.x + 12;
	int iGBox1PosY = m_Pos.y + 75;
	int iGBox2PosX = m_Pos.x + 12;

	int iGBox2PosY = m_Pos.y + 196;	
	RenderGroupBox( iGBox1PosX, iGBox1PosY, 168, 121 );
	RenderGroupBox( iGBox2PosX, iGBox2PosY, 168, 195 );
														
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor( 255, 255, 0, 255 );
		
	unicode::_sprintf(szText, GlobalText[200], pPetInfo->m_wLevel);
	g_pRenderText->RenderText(iGBox1PosX+2, iGBox1PosY+8, szText, 70-14, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX+2, iGBox2PosY+8, GlobalText[1218], 70-14, 0, RT3_SORT_CENTER);
	
	g_pRenderText->SetFont(g_hFont);	
	g_pRenderText->SetTextColor( 255, 255, 255, 255 );

	unicode::_sprintf(szText, GlobalText[358], pPetInfo->m_wLife, 255);
	g_pRenderText->RenderText(iGBox1PosX+10, iGBox1PosY+28, szText, 0, 0, RT3_SORT_CENTER);
	RenderImage(IMAGE_PETINFO_LIFEBAR, iGBox1PosX+7, iGBox1PosY+40, 151, 12);
	int iHP = (min(pPetInfo->m_wLife,255)*147)/255;		
	RenderImage(IMAGE_PETINFO_LIFE, iGBox1PosX+9, iGBox1PosY+42, iHP, 8);	
	unicode::_sprintf(szText, GlobalText[357], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	g_pRenderText->RenderText(iGBox1PosX+10, iGBox1PosY+59, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[203], m_aiDamage[0], m_aiDamage[1], 
															pPetInfo->m_wAttackSuccess);
	g_pRenderText->RenderText(iGBox1PosX+10, iGBox1PosY+72, szText, 0, 0, RT3_SORT_CENTER);	
	unicode::_sprintf(szText, GlobalText[64], pPetInfo->m_wAttackSpeed);
	g_pRenderText->RenderText(iGBox1PosX+10, iGBox1PosY+85, szText, 0, 0, RT3_SORT_CENTER);	
	unicode::_sprintf(szText, GlobalText[698], (185+(pPetInfo->m_wLevel*15)));
	g_pRenderText->RenderText(iGBox1PosX+10, iGBox1PosY+98, szText, 0, 0, RT3_SORT_CENTER);

	// SkillBox
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX+10, iGBox2PosY+28, 32.f, 38.f);
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX+10, iGBox2PosY+68, 32.f, 38.f);
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX+10, iGBox2PosY+108, 32.f, 38.f);
	RenderImage(IMAGE_PETINFO_SKILLBOX, iGBox2PosX+10, iGBox2PosY+148, 32.f, 38.f);
	// Skill	(x = SKillbox.x + 7, y = SkillBox.y + 6)
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX+17, iGBox2PosY+34, 19.f, 27.f, 1.f, 0.f);		
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX+17, iGBox2PosY+74, 19.f, 27.f, 21.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX+17, iGBox2PosY+114, 19.f, 27.f, 41.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iGBox2PosX+17, iGBox2PosY+154, 19.f, 27.f, 61.f, 0.f);
	
	g_pRenderText->RenderText(iGBox2PosX+52, iGBox2PosY+45, GlobalText[1219], 0, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX+52, iGBox2PosY+85, GlobalText[1220], 0, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX+52, iGBox2PosY+125, GlobalText[1221], 0, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iGBox2PosX+52, iGBox2PosY+165, GlobalText[1222], 0, 0, RT3_SORT_CENTER);
#elif CLIENT_VERSION >= 1001

	int iPosX = m_Pos.x + 11;
	int iPosY = m_Pos.y + 60;

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	RenderColor(iPosX, iPosY, 166, 15);
	RenderColor(iPosX, iPosY+130, 166, 15);
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	RenderColor(iPosX, iPosY + 15, 166, 100);
	RenderColor(iPosX, iPosY + 130+15, 166, 160);
	EndRenderColor();

	g_pRenderText->SetFont(g_hFontMediumBold);
	g_pRenderText->SetTextColor(247, 128, 0, 255);

	unicode::_sprintf(szText, GlobalText[200], pPetInfo->m_wLevel);
	g_pRenderText->RenderText(iPosX, iPosY + 2, szText, 166, 0, RT3_SORT_CENTER);
	g_pRenderText->RenderText(iPosX, iPosY + 130 + 2, GlobalText[1218], 166, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);


	unicode::_sprintf(szText, GlobalText[357], pPetInfo->m_dwExp1, pPetInfo->m_dwExp2);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 25, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[203], m_aiDamage[0], m_aiDamage[1], pPetInfo->m_wAttackSuccess);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 40, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[64], pPetInfo->m_wAttackSpeed);
	g_pRenderText->RenderText(iPosX + 10, iPosY + 55, szText, 0, 0, RT3_SORT_CENTER);
	unicode::_sprintf(szText, GlobalText[698], (185 + (pPetInfo->m_wLevel * 15)));
	g_pRenderText->RenderText(iPosX + 10, iPosY + 70, szText, 0, 0, RT3_SORT_CENTER);

	int iHP = (min(pPetInfo->m_wLife, 255) * 152) / 255;
	RenderImageScale(IMAGE_PETINFO_LIFEBAR, iPosX + 7, iPosY + 88, 152, 6, 0.f, 0.f, 333.f, 14.f);
	RenderImageScale(IMAGE_PETINFO_LIFE, iPosX + 7, iPosY + 88, iHP, 6, 0.f, 0.f, 333.f, 14.f);

	g_pRenderText->SetFont(g_hFontSmall);
	g_pRenderText->SetTextColor(RGBA(237, 214, 161, 255));
	unicode::_sprintf(szText, GlobalText[358], pPetInfo->m_wLife, 255);
	g_pRenderText->RenderText(iPosX + 7 + 152, iPosY + 98, szText, 152, 0, RT3_WRITE_RIGHT_TO_LEFT);

	iPosY += 130;

	RenderImage(IMAGE_PETINFO_SKILL, iPosX + 5, iPosY + 25, 19.f, 27.f, 1.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iPosX + 5, iPosY + 62, 19.f, 27.f, 21.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iPosX + 5, iPosY + 99, 19.f, 27.f, 41.f, 0.f);
	RenderImage(IMAGE_PETINFO_SKILL, iPosX + 5, iPosY + 136, 19.f, 27.f, 61.f, 0.f);


	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(128, 179, 255, 255);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 25 + 4, GlobalText[1219], 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 62 + 4, GlobalText[1220], 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 99 + 4, GlobalText[1221], 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 136 + 4, GlobalText[1222], 0, 0, RT3_SORT_LEFT);


	g_pRenderText->SetFont(g_hFontSmall);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	g_pRenderText->RenderText(iPosX + 29, iPosY + 35 + 4, GlobalText[1223], 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 72 + 4, GlobalText[1224], 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 109 + 4, GlobalText[1225], 0, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(iPosX + 29, iPosY + 146 + 4, GlobalText[1226], 0, 0, RT3_SORT_LEFT);


#endif
	return true;
}

void CNewUIPetInfoWindow::RenderGroupBox( int iPosX, int iPosY, int iWidth, int iHeight, int iTitleWidth/* =60 */, int iTitleHeight/* =20  */ )
{
	EnableAlphaTest();

	glColor4f ( 0.f, 0.f, 0.f, 0.9f );
	RenderColor( float(iPosX+3), float(iPosY+2), float(iTitleWidth-8), float(iTitleHeight) );
 	glColor4f ( 0.f, 0.f, 0.f, 0.6f );							
 	RenderColor( float(iPosX+3), float(iPosY+2+iTitleHeight), float(iWidth-7), float(iHeight-iTitleHeight-7) );

	EndRenderColor();
	
	RenderImage(IMAGE_PETINFO_TABLE_TOP_LEFT, iPosX, iPosY, 14, 14);
	RenderImage(IMAGE_PETINFO_TABLE_TOP_RIGHT, iPosX+iTitleWidth-14, iPosY, 14, 14);
	RenderImage(IMAGE_PETINFO_TABLE_TOP_RIGHT, iPosX+iWidth-14, iPosY+iTitleHeight, 14, 14);
	RenderImage(IMAGE_PETINFO_TABLE_BOTTOM_LEFT, iPosX, iPosY+iHeight-14, 14, 14);
	RenderImage(IMAGE_PETINFO_TABLE_BOTTOM_RIGHT, iPosX+iWidth-14, iPosY+iHeight-14, 14, 14);

	RenderImage(IMAGE_PETINFO_TABLE_TOP_PIXEL, iPosX+6, iPosY, iTitleWidth-12, 14);
	RenderImage(IMAGE_PETINFO_TABLE_RIGHT_PIXEL, iPosX+iTitleWidth-14, iPosY+6, 14, iTitleHeight-6);
	RenderImage(IMAGE_PETINFO_TABLE_TOP_PIXEL, iPosX+iTitleWidth-5, iPosY+iTitleHeight, iWidth-iTitleWidth-6, 14);
	RenderImage(IMAGE_PETINFO_TABLE_RIGHT_PIXEL, iPosX+iWidth-14, iPosY+iTitleHeight+6, 14, iHeight-iTitleHeight-14);
	RenderImage(IMAGE_PETINFO_TABLE_BOTTOM_PIXEL, iPosX+6, iPosY+iHeight-14, iWidth-12, 14);
	RenderImage(IMAGE_PETINFO_TABLE_LEFT_PIXEL, iPosX, iPosY+6, 14, iHeight-14);
}

float CNewUIPetInfoWindow::GetLayerDepth()
{ 
	return 2.3f; 
}


bool CNewUIPetInfoWindow::BtnProcess()
{
	POINT ptExitBtn1 = { m_Pos.x+169, m_Pos.y+7 };
	
	if(SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PET);
		return true;
	}

	if(m_BtnExit.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PET);
		return true;
	}
	
	return false;
}

void CNewUIPetInfoWindow::CalcDamage( int iNumTapButton )
{
	PET_INFO* pPetInfo = NULL;

	switch( iNumTapButton )
	{
	case TAB_TYPE_DARKHORSE:
		{
			int iSkillDamage[2];
			gCharacterManager.GetSkillDamage( AT_SKILL_DARK_HORSE, &iSkillDamage[0], &iSkillDamage[1] );

			pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_HORSE);
		    m_aiDamage[0] = pPetInfo->m_wDamageMin+iSkillDamage[0];
			m_aiDamage[1] = pPetInfo->m_wDamageMax+iSkillDamage[1];
		}
		break;
	case TAB_TYPE_DARKSPIRIT: 
		{
			m_fAddDamagePercent = 0.f;
			m_aiDamage[0] = 0;
			m_aiDamage[1] = 0;

			if ( CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].Type!=-1 )
			{
				m_fAddDamagePercent = CharacterMachine->Equipment[EQUIPMENT_WEAPON_RIGHT].MagicPower;
			}
			
			pPetInfo = Hero->GetEquipedPetInfo(PET_TYPE_DARK_SPIRIT);

			m_fAddDamagePercent /= 100.f;
			m_aiDamage[0] = pPetInfo->m_wDamageMin+(int)(pPetInfo->m_wDamageMin*m_fAddDamagePercent);
			m_aiDamage[1] = pPetInfo->m_wDamageMax+(int)(pPetInfo->m_wDamageMax*m_fAddDamagePercent);
		}
		break;
	}
}

void CNewUIPetInfoWindow::LoadImages()
{



#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_PETINFO_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back04.tga", IMAGE_PETINFO_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_PETINFO_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_PETINFO_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_PETINFO_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_exit_00.tga", IMAGE_PETINFO_BTN_EXIT, GL_LINEAR);
	LoadBitmap("Interface\\newui_guild_tab04.tga", IMAGE_PETINFO_TAB_BUTTON, GL_LINEAR);

	// GroupBox
	LoadBitmap("Interface\\newui_item_table01(L).tga", IMAGE_PETINFO_TABLE_TOP_LEFT);
	LoadBitmap("Interface\\newui_item_table01(R).tga", IMAGE_PETINFO_TABLE_TOP_RIGHT);
	LoadBitmap("Interface\\newui_item_table02(L).tga", IMAGE_PETINFO_TABLE_BOTTOM_LEFT);
	LoadBitmap("Interface\\newui_item_table02(R).tga", IMAGE_PETINFO_TABLE_BOTTOM_RIGHT);
	LoadBitmap("Interface\\newui_item_table03(Up).tga", IMAGE_PETINFO_TABLE_TOP_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(Dw).tga", IMAGE_PETINFO_TABLE_BOTTOM_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(L).tga", IMAGE_PETINFO_TABLE_LEFT_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(R).tga", IMAGE_PETINFO_TABLE_RIGHT_PIXEL);

	// Skill
	LoadBitmap("Interface\\newui_command.jpg", IMAGE_PETINFO_SKILL, GL_LINEAR);
	LoadBitmap("Interface\\newui_skillbox.jpg", IMAGE_PETINFO_SKILLBOX, GL_LINEAR);
	// LifeBar
	LoadBitmap("Interface\\newui_pet_lifebar01.jpg", IMAGE_PETINFO_LIFEBAR, GL_LINEAR);
	LoadBitmap("Interface\\newui_pet_lifebar02.jpg", IMAGE_PETINFO_LIFE, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\btn_exit.jpg", IMAGE_PETINFO_BTN_EXIT, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_PETINFO_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back04.tga", IMAGE_PETINFO_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_PETINFO_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_PETINFO_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_PETINFO_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_guild_tab04.tga", IMAGE_PETINFO_TAB_BUTTON, GL_LINEAR);

	// GroupBox
	LoadBitmap("Interface\\newui_item_table01(L).tga", IMAGE_PETINFO_TABLE_TOP_LEFT);
	LoadBitmap("Interface\\newui_item_table01(R).tga", IMAGE_PETINFO_TABLE_TOP_RIGHT);
	LoadBitmap("Interface\\newui_item_table02(L).tga", IMAGE_PETINFO_TABLE_BOTTOM_LEFT);
	LoadBitmap("Interface\\newui_item_table02(R).tga", IMAGE_PETINFO_TABLE_BOTTOM_RIGHT);
	LoadBitmap("Interface\\newui_item_table03(Up).tga", IMAGE_PETINFO_TABLE_TOP_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(Dw).tga", IMAGE_PETINFO_TABLE_BOTTOM_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(L).tga", IMAGE_PETINFO_TABLE_LEFT_PIXEL);
	LoadBitmap("Interface\\newui_item_table03(R).tga", IMAGE_PETINFO_TABLE_RIGHT_PIXEL);

	// Skill
	LoadBitmap("Interface\\newui_command.jpg", IMAGE_PETINFO_SKILL, GL_LINEAR);
	LoadBitmap("Interface\\newui_skillbox.jpg", IMAGE_PETINFO_SKILLBOX, GL_LINEAR);
	// LifeBar
	LoadBitmap("Interface\\party_info_life_back.jpg", IMAGE_PETINFO_LIFEBAR, GL_LINEAR);
	LoadBitmap("Interface\\party_info_life_top.jpg", IMAGE_PETINFO_LIFE, GL_LINEAR);

	LoadBitmap("Interface\\default_window_back.jpg", IMAGE_PETINFO_BACK_VOID, GL_LINEAR);
	LoadBitmap("Interface\\pet_tab.tga", IMAGE_PETINFO_TAB_BACK, GL_LINEAR);
	LoadBitmap("Interface\\pet_tab_on.jpg", IMAGE_PETINFO_TAB_ON, GL_LINEAR);


#endif


}

void CNewUIPetInfoWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_PETINFO_BACK);
	DeleteBitmap(IMAGE_PETINFO_TOP);
	DeleteBitmap(IMAGE_PETINFO_LEFT);
	DeleteBitmap(IMAGE_PETINFO_RIGHT);
	DeleteBitmap(IMAGE_PETINFO_BOTTOM);
	DeleteBitmap(IMAGE_PETINFO_BTN_EXIT);
	DeleteBitmap(IMAGE_PETINFO_TAB_BUTTON);

	// GroupBox
	DeleteBitmap(IMAGE_PETINFO_TABLE_RIGHT_PIXEL);
	DeleteBitmap(IMAGE_PETINFO_TABLE_LEFT_PIXEL);
	DeleteBitmap(IMAGE_PETINFO_TABLE_BOTTOM_PIXEL);
	DeleteBitmap(IMAGE_PETINFO_TABLE_TOP_PIXEL);
	DeleteBitmap(IMAGE_PETINFO_TABLE_BOTTOM_RIGHT);
	DeleteBitmap(IMAGE_PETINFO_TABLE_BOTTOM_LEFT);
	DeleteBitmap(IMAGE_PETINFO_TABLE_TOP_RIGHT);
	DeleteBitmap(IMAGE_PETINFO_TABLE_TOP_LEFT);

	// Skill
	DeleteBitmap(IMAGE_PETINFO_SKILL);
	DeleteBitmap(IMAGE_PETINFO_SKILLBOX);

	// LifeBar
	DeleteBitmap(IMAGE_PETINFO_LIFEBAR);
	DeleteBitmap(IMAGE_PETINFO_LIFE);

#if CLIENT_VERSION >= 1001
	DeleteBitmap(IMAGE_PETINFO_BACK_VOID);
	DeleteBitmap(IMAGE_PETINFO_TAB_BACK);
	DeleteBitmap(IMAGE_PETINFO_TAB_ON);
#endif

}

void CNewUIPetInfoWindow::OpenningProcess()
{
	CalcDamage(m_iNumCurOpenTab);	
}

void CNewUIPetInfoWindow::ClosingProcess()
{

}
