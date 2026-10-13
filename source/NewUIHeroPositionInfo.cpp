// NewUIHeroPositionInfo.cpp: implementation of the CNewUIHeroPositionInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIHeroPositionInfo.h"
#include "NewUISystem.h"
#include "MapManager.h"
#include "WideScreen.h"

using namespace SEASON3B;

namespace
{
	void RenderHudShadowText(int x, int y, const unicode::t_char* text, int width, int sort, bool bold)
	{
		g_pRenderText->SetFont(bold ? g_hFontBold : g_hFont);
		g_pRenderText->SetBgColor(0, 0, 0, 0);

		g_pRenderText->SetTextColor(0, 0, 0, 230);
		g_pRenderText->RenderText(x + 1, y + 1, text, width, 0, sort);
		g_pRenderText->SetTextColor(92, 52, 0, 210);
		g_pRenderText->RenderText(x, y + 1, text, width, 0, sort);
		g_pRenderText->RenderText(x + 1, y, text, width, 0, sort);

		g_pRenderText->SetTextColor(255, 230, 0, 255);
		g_pRenderText->RenderText(x, y, text, width, 0, sort);
	}
}

CNewUIHeroPositionInfo::CNewUIHeroPositionInfo()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_CurHeroPosition.x = m_CurHeroPosition.y = 0;
}

CNewUIHeroPositionInfo::~CNewUIHeroPositionInfo()
{
	Release();
}

//---------------------------------------------------------------------------------------------
// Create
bool CNewUIHeroPositionInfo::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_HERO_POSITION_INFO, this );

	WidenX = HERO_POSITION_INFO_BASEB_WINDOW_WIDTH;
	if(WindowWidth == 800)
	{
		WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.4f));
	}
	else
	if(WindowWidth == 1024)
	{
		WidenX = (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH + (HERO_POSITION_INFO_BASEB_WINDOW_WIDTH * 0.2f));
	}

	SetPos(x, y);
	LoadImages();
	Show( true );
	return true;
}

void CNewUIHeroPositionInfo::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void CNewUIHeroPositionInfo::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool CNewUIHeroPositionInfo::BtnProcess()
{
	
	return false;
}

bool CNewUIHeroPositionInfo::UpdateMouseEvent()
{
	if( true == BtnProcess() )
		return false;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	int Width = HERO_POSITION_INFO_BASEA_WINDOW_WIDTH + WidenX + HERO_POSITION_INFO_BASEC_WINDOW_WIDTH;
	if( CheckMouseIn(m_Pos.x, m_Pos.y, Width, HERO_POSITION_INFO_BASE_WINDOW_HEIGHT) )
		return false;
#endif
	
	return true;
}

bool CNewUIHeroPositionInfo::UpdateKeyEvent()
{
	return true;
}

bool CNewUIHeroPositionInfo::Update()
{
	if( (IsVisible() == true) && (Hero != NULL) )
	{
		m_CurHeroPosition.x = ( Hero->PositionX );
		m_CurHeroPosition.y = ( Hero->PositionY );
	}

	return true;
}

bool CNewUIHeroPositionInfo::Render()
{
    if (Hero != NULL)
    {
        SYSTEMTIME st;
        GetLocalTime(&st);

        unicode::t_char szMapText[256] = { 0 };
        unicode::t_char szTimeText[64] = { 0 };
        unicode::_sprintf(szMapText, "%s (%d, %d)", gMapManager.GetMapName(gMapManager.WorldActive), m_CurHeroPosition.x, m_CurHeroPosition.y);
        unicode::_sprintf(szTimeText, "Server Time: %02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

        const int screenWidth = (int)gWideScreen.g_WideWindowWidth;
        const int margin = 10;
        const int top = 18;
        int infoWidth = 280;

        if (infoWidth > (screenWidth - (margin * 2)))
        {
            infoWidth = screenWidth - (margin * 2);
        }

        int x = screenWidth - infoWidth - margin;
        if (x < 0) x = 0;

        EnableAlphaTest();
        RenderHudShadowText(x, top, szMapText, infoWidth, RT3_SORT_RIGHT, true);
        RenderHudShadowText(x, top + 13, szTimeText, infoWidth, RT3_SORT_RIGHT, true);
        DisableAlphaBlend();
    }

    return true;
}

float CNewUIHeroPositionInfo::GetLayerDepth()
{
	return 0.1f;
}

void CNewUIHeroPositionInfo::OpenningProcess()
{
	
}

void CNewUIHeroPositionInfo::ClosingProcess()
{
	
}

void CNewUIHeroPositionInfo::SetCurHeroPosition( int x, int y )
{
	m_CurHeroPosition.x = x;
	m_CurHeroPosition.y = y;
}

void CNewUIHeroPositionInfo::LoadImages()
{
	LoadBitmap("Interface\\Minimap_positionA.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW, GL_LINEAR);
	LoadBitmap("Interface\\Minimap_positionB.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW+1, GL_LINEAR);
	LoadBitmap("Interface\\Minimap_positionC.tga", IMAGE_HERO_POSITION_INFO_BASE_WINDOW+2, GL_LINEAR);
}

void CNewUIHeroPositionInfo::UnloadImages()
{
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW+1);
	DeleteBitmap(IMAGE_HERO_POSITION_INFO_BASE_WINDOW+2);
}
