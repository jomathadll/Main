//*****************************************************************************
// File: NewUIBattleSoccerScore.cpp
//*****************************************************************************

#include "stdafx.h"
#include "NewUIBattleSoccerScore.h"
#include "UIControls.h"
#include "WSclient.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "ZzzInventory.h"
#include "ZzzTexture.h"

using namespace SEASON3B;

SEASON3B::CNewUIBattleSoccerScore::CNewUIBattleSoccerScore()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
}

SEASON3B::CNewUIBattleSoccerScore::~CNewUIBattleSoccerScore()
{
	Release();
}

bool SEASON3B::CNewUIBattleSoccerScore::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_BATTLE_SOCCER_SCORE, this);

	SetPos(x, y);

	LoadImages();

	Show(false);	

	return true;
}

void SEASON3B::CNewUIBattleSoccerScore::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIBattleSoccerScore::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIBattleSoccerScore::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CNewUIBattleSoccerScore::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIBattleSoccerScore::Update()
{
	return true;
}

bool SEASON3B::CNewUIBattleSoccerScore::Render()
{
	::EnableAlphaTest();

	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderBackImage();
	RenderContents();

	::DisableAlphaBlend();
	
	return true;
}

void SEASON3B::CNewUIBattleSoccerScore::RenderBackImage()
{
	RenderImageScale(IMAGE_BSS_BACK, m_Pos.x, m_Pos.y, 300, 25, 0.f, 0.f, 742.f, 64.f);
}

void SEASON3B::CNewUIBattleSoccerScore::RenderContents()
{
	unicode::t_char szTemp[128];
	int nX = m_Pos.x + 30;
	int nY = m_Pos.y + 33;

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0, 0, 0, 128);

	if (EnableGuildWar && Hero->GuildMarkIndex!=-1)
	{
		if (HeroSoccerTeam == 0)
			g_pRenderText->SetTextColor(255, 60, 0, 255);
		else
			g_pRenderText->SetTextColor(0, 150, 255, 255);

		unicode::_sprintf(szTemp, "%d", GuildWarScore[0]);
		g_pRenderText->SetFont(g_hFontMediumBold);
		g_pRenderText->RenderText(m_Pos.x + 137, m_Pos.y + 8, szTemp);				// 점수

        ::CreateGuildMark(Hero->GuildMarkIndex);
        ::RenderBitmap(BITMAP_GUILD, m_Pos.x + 105, m_Pos.y + 3, 11, 11);// 길드 마크
		g_pRenderText->SetFont(g_hFontBigBold);
		g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 3, GuildMark[Hero->GuildMarkIndex].GuildName, 110, 0, RT3_SORT_CENTER);// 길드명

		if (HeroSoccerTeam == 0)
			g_pRenderText->SetTextColor(0, 150, 255, 255);
		else
			g_pRenderText->SetTextColor(255, 60, 0, 255);

		unicode::_sprintf(szTemp, "%d", GuildWarScore[1]);
		g_pRenderText->SetFont(g_hFontMediumBold);
		g_pRenderText->RenderText(m_Pos.x + 150 + 10, m_Pos.y + 8, szTemp);			// 점수
        ::CreateGuildMark(FindGuildMark(GuildWarName));
        ::RenderBitmap(BITMAP_GUILD, m_Pos.x + 185, m_Pos.y + 3, 11, 11);// 길드 마크
		g_pRenderText->SetFont(g_hFontBigBold);
		g_pRenderText->RenderText(m_Pos.x + 190, m_Pos.y + 3, GuildWarName, 110, 0, RT3_SORT_CENTER);
	}
	else if (SoccerObserver)
	{
		g_pRenderText->SetTextColor(255, 60, 0, 255);
		unicode::_sprintf(szTemp, "%d", GuildWarScore[0]);
		g_pRenderText->RenderText(nX, nY, szTemp);
        ::CreateGuildMark(FindGuildMark(SoccerTeamName[0]));
        ::RenderBitmap(BITMAP_GUILD, float(nX+21), float(nY), 8, 8);
		g_pRenderText->RenderText(nX+33, nY, SoccerTeamName[0]);

		g_pRenderText->SetTextColor(0, 150, 255, 255);
		unicode::_sprintf(szTemp,"%d",GuildWarScore[1]);
		g_pRenderText->RenderText(nX, nY+22, szTemp);
        ::CreateGuildMark(FindGuildMark(SoccerTeamName[1]));
        ::RenderBitmap(BITMAP_GUILD, float(nX+21), float(nY+22), 8, 8);
		g_pRenderText->RenderText(nX+33, nY+22, SoccerTeamName[1]);
	}
}

int SEASON3B::CNewUIBattleSoccerScore::FindGuildMark(char* pszGuildName)
{
	for (int i = 0; i < MARK_EDIT; ++i)
	{
		MARK_t *p = &GuildMark[i];
		if (strcmp(p->GuildName, pszGuildName) == NULL)
		{
			return i;
		}
	}
	return 0;
}

float SEASON3B::CNewUIBattleSoccerScore::GetLayerDepth()
{
	return 1.8f;
}

void SEASON3B::CNewUIBattleSoccerScore::LoadImages()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_Figure_ground.tga", IMAGE_BSS_BACK, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\Interface_Duel.tga", IMAGE_BSS_BACK, GL_LINEAR);
#endif
}

void SEASON3B::CNewUIBattleSoccerScore::UnloadImages()
{
	DeleteBitmap(IMAGE_BSS_BACK);
}
