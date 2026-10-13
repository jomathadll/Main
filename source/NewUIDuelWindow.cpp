// NewUIDuelWindow.cpp: implementation of the CNewUIDuelWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIDuelWindow.h"
#include "ZzzTexture.h"
#include "ZzzInventory.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "UIControls.h"
#include "DuelMgr.h"
#include "GuildCache.h"

using namespace SEASON3B;

SEASON3B::CNewUIDuelWindow::CNewUIDuelWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
}

SEASON3B::CNewUIDuelWindow::~CNewUIDuelWindow()
{
	Release();
}

bool SEASON3B::CNewUIDuelWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_DUEL_WINDOW, this);

	SetPos(x, y);

	LoadImages();

	Show(false);

	return true;
}

void SEASON3B::CNewUIDuelWindow::Release()
{
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIDuelWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

bool SEASON3B::CNewUIDuelWindow::UpdateMouseEvent()
{
	return true;
}

bool SEASON3B::CNewUIDuelWindow::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIDuelWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIDuelWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();
	RenderContents();

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIDuelWindow::RenderFrame()
{

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	RenderImage(IMAGE_DUEL_BACK, m_Pos.x, m_Pos.y, 131, 70);
#elif CLIENT_VERSION >= 1001
	RenderImageScale(IMAGE_DUEL_BACK, m_Pos.x, m_Pos.y, 300, 25, 0.f, 0.f, 742.f, 64.f);

	if (m_bHasHPReceived == FALSE || g_DuelMgr.GetFighterRegenerated2())
	{
		m_bHasHPReceived = TRUE;
		g_DuelMgr.SetFighterRegenerated2(FALSE);

		m_fPrevHPRate1 = m_fLastHPRate1 = m_fReceivedHPRate1 = g_DuelMgr.GetHP(DUEL_HERO);
		m_fPrevHPRate2 = m_fLastHPRate2 = m_fReceivedHPRate2 = g_DuelMgr.GetHP(DUEL_ENEMY);
		m_fPrevSDRate1 = m_fLastSDRate1 = m_fReceivedSDRate1 = g_DuelMgr.GetSD(DUEL_HERO);
		m_fPrevSDRate2 = m_fLastSDRate2 = m_fReceivedSDRate2 = g_DuelMgr.GetSD(DUEL_ENEMY);
		
	}

	if (m_fLastHPRate1 != g_DuelMgr.GetHP(DUEL_HERO) || m_fLastHPRate2 != g_DuelMgr.GetHP(DUEL_ENEMY) || m_fLastSDRate1 != g_DuelMgr.GetSD(DUEL_HERO) || m_fLastSDRate2 != g_DuelMgr.GetSD(DUEL_ENEMY))
	{
		m_fLastHPRate1 = g_DuelMgr.GetHP(DUEL_HERO);
		m_fLastHPRate2 = g_DuelMgr.GetHP(DUEL_ENEMY);
		m_fLastSDRate1 = g_DuelMgr.GetSD(DUEL_HERO);
		m_fLastSDRate2 = g_DuelMgr.GetSD(DUEL_ENEMY);
		m_fReceivedHPRate1 = m_fPrevHPRate1;
		m_fReceivedHPRate2 = m_fPrevHPRate2;
		m_fReceivedSDRate1 = m_fPrevSDRate1;
		m_fReceivedSDRate2 = m_fPrevSDRate2;
	}

	// HP HERO
	int iDamageGap = int(absf(m_fReceivedHPRate1 - g_DuelMgr.GetHP(DUEL_HERO)) * 5.0f) + 2;
	float fHPRatePerPixel = 1.0f / 130.f * iDamageGap;

	float fHPRate = g_DuelMgr.GetHP(DUEL_HERO);
	RenderImage(IMAGE_DUEL_HPBAR_BACK, m_Pos.x, m_Pos.y + 26, 130.f, 6.f);
	if (m_fPrevHPRate1 > fHPRate + fHPRatePerPixel)
	{
		m_fPrevHPRate1 -= fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_HPBAR, m_Pos.x + 130.f * (1.f - fHPRate), m_Pos.y + 26, 130.f * fHPRate, 6.f, 130.f / 256.f * fHPRate, 0, -130.f / 256.f * fHPRate, 6.f / 8.f);
	}
	else if (m_fPrevHPRate1 < fHPRate - fHPRatePerPixel)
	{
		m_fPrevHPRate1 += fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_HPBAR, m_Pos.x + 130.f * (1.f - m_fPrevHPRate1), m_Pos.y + 26, 130.f * m_fPrevHPRate1, 6.f, 130.f / 256.f * m_fPrevHPRate1, 0, -130.f / 256.f * m_fPrevHPRate1, 6.f / 8.f);
	}
	else
	{
		m_fPrevHPRate1 = fHPRate;
		RenderImage(IMAGE_DUEL_HPBAR, m_Pos.x + 130.f * (1.f - fHPRate), m_Pos.y + 26, 130.f * fHPRate, 6.f, 130.f / 256.f * fHPRate, 0, -130.f / 256.f * fHPRate, 6.f / 8.f);
	}

	// SD HERO
	iDamageGap = int(absf(m_fReceivedSDRate1 - g_DuelMgr.GetSD(DUEL_HERO)) * 5.0f) + 2;
	fHPRatePerPixel = 1.0f / 130.f * iDamageGap;

	fHPRate = g_DuelMgr.GetSD(DUEL_HERO);
	RenderImage(IMAGE_DUEL_HPBAR_BACK, m_Pos.x, m_Pos.y + 34, 130.f, 6.f);
	if (m_fPrevSDRate1 > fHPRate + fHPRatePerPixel)
	{
		m_fPrevSDRate1 -= fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_SDBAR, m_Pos.x + 130.f * (1.f - fHPRate), m_Pos.y + 34, 130.f * fHPRate, 6.f, 130.f / 256.f * fHPRate, 0, -130.f / 256.f * fHPRate, 6.f / 8.f);
	}
	else if (m_fPrevSDRate1 < fHPRate - fHPRatePerPixel)
	{
		m_fPrevSDRate1 += fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_SDBAR, m_Pos.x + 130.f * (1.f - m_fPrevSDRate1), m_Pos.y + 34, 130.f * m_fPrevSDRate1, 6.f, 130.f / 256.f * m_fPrevSDRate1, 0, -130.f / 256.f * m_fPrevSDRate1, 6.f / 8.f);
	}
	else
	{
		m_fPrevSDRate1 = fHPRate;
		RenderImage(IMAGE_DUEL_SDBAR, m_Pos.x + 130.f * (1.f - fHPRate), m_Pos.y + 34, 130.f * fHPRate, 6.f, 130.f / 256.f * fHPRate, 0, -130.f / 256.f * fHPRate, 6.f / 8.f);
	}



	
	//HP ENEMY
	iDamageGap = int(absf(m_fReceivedHPRate2 - g_DuelMgr.GetHP(DUEL_ENEMY)) * 5.0f) + 2;
	fHPRatePerPixel = 1.0f / 130.f * iDamageGap;

	fHPRate = g_DuelMgr.GetHP(DUEL_ENEMY);
	RenderImage(IMAGE_DUEL_HPBAR_BACK, m_Pos.x + 130.f+40, m_Pos.y + 26, 130.f, 6.f);
	if (m_fPrevHPRate2 > fHPRate + fHPRatePerPixel)
	{
		m_fPrevHPRate2 -= fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_HPBAR, m_Pos.x + 130.f + 40, m_Pos.y + 26, 130.f * fHPRate, 6.f);
	}
	else if (m_fPrevHPRate2 < fHPRate - fHPRatePerPixel)
	{
		m_fPrevHPRate2 += fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_HPBAR, m_Pos.x + 130.f + 40, m_Pos.y + 26, 130.f * m_fPrevHPRate2, 6.f);
	}
	else
	{
		m_fPrevHPRate2 = fHPRate;
		RenderImage(IMAGE_DUEL_HPBAR, m_Pos.x + 130.f + 40, m_Pos.y + 26, 130.f * fHPRate, 6.f);
	}

	//SD ENEMY
	iDamageGap = int(absf(m_fReceivedSDRate2 - g_DuelMgr.GetSD(DUEL_ENEMY)) * 5.0f) + 2;
	fHPRatePerPixel = 1.0f / 130.f * iDamageGap;

	fHPRate = g_DuelMgr.GetSD(DUEL_ENEMY);
	RenderImage(IMAGE_DUEL_HPBAR_BACK, m_Pos.x + 130.f + 40, m_Pos.y + 34, 130.f, 6.f);
	if (m_fPrevSDRate2 > fHPRate + fHPRatePerPixel)
	{
		m_fPrevSDRate2 -= fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_SDBAR, m_Pos.x + 130.f + 40, m_Pos.y + 34, 130.f * fHPRate, 6.f);
	}
	else if (m_fPrevSDRate2 < fHPRate - fHPRatePerPixel)
	{
		m_fPrevSDRate2 += fHPRatePerPixel;
		RenderImage(IMAGE_DUEL_SDBAR, m_Pos.x + 130.f + 40, m_Pos.y + 34, 130.f * m_fPrevSDRate2, 6.f);
	}
	else
	{
		m_fPrevSDRate2 = fHPRate;
		RenderImage(IMAGE_DUEL_SDBAR, m_Pos.x + 130.f + 40, m_Pos.y + 34, 130.f * fHPRate, 6.f);
	}



	int heroMark = 0;
	int enemyMark = 0;

	for (int i = 0; i < MARK_EDIT; ++i)
	{
		MARK_t* p = &GuildMark[i];
		if (strcmp(p->GuildName, g_DuelMgr.GetGuild(DUEL_HERO)) == NULL)
		{
			heroMark = i;
		}

		if (strcmp(p->GuildName, g_DuelMgr.GetGuild(DUEL_ENEMY)) == NULL)
		{
			enemyMark = i;
		}
	}

	::CreateGuildMark(heroMark);
	::RenderBitmap(BITMAP_GUILD, m_Pos.x + 105, m_Pos.y+3, 11, 11);// 길드 마크

	::CreateGuildMark(enemyMark);
	::RenderBitmap(BITMAP_GUILD, m_Pos.x + 185, m_Pos.y + 3, 11, 11);// 길드 마크
#endif

	
}

void SEASON3B::CNewUIDuelWindow::RenderContents()
{
	
	unicode::t_char strMyScore[12];
	unicode::t_char strDuelScore[12];
	unicode::_sprintf(strMyScore, "%d", g_DuelMgr.GetScore(DUEL_HERO));
	unicode::_sprintf(strDuelScore, "%d", g_DuelMgr.GetScore(DUEL_ENEMY));


#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(0, 0, 0, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(0, 150, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 55, m_Pos.y + 33, g_DuelMgr.GetDuelPlayerID(DUEL_HERO));
	g_pRenderText->RenderText(m_Pos.x + 31, m_Pos.y + 33, strMyScore);
	g_pRenderText->SetTextColor(255, 25, 25, 255);
	g_pRenderText->RenderText(m_Pos.x + 55, m_Pos.y + 56, g_DuelMgr.GetDuelPlayerID(DUEL_ENEMY));
	g_pRenderText->RenderText(m_Pos.x + 31, m_Pos.y + 56, strDuelScore);
#elif CLIENT_VERSION >= 1001
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(0, 178, 255, 255);
	g_pRenderText->SetFont(g_hFontBigBold);
	unicode::t_char buffName[12];
	unicode::_sprintf(buffName, "%s", g_DuelMgr.GetDuelPlayerID(DUEL_HERO));

	if (CheckMouseIn(m_Pos.x, m_Pos.y + 3, 130, 15)) {
		if (strcmp(g_DuelMgr.GetGuild(DUEL_HERO), "")) {
			unicode::_sprintf(buffName, "%s", g_DuelMgr.GetGuild(DUEL_HERO));
		}
	}
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y + 3, buffName, 110, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFontMediumBold);
	g_pRenderText->RenderText(m_Pos.x + 137, m_Pos.y + 8, strMyScore);


	unicode::_sprintf(buffName, "%s", g_DuelMgr.GetDuelPlayerID(DUEL_ENEMY));

	if (CheckMouseIn(m_Pos.x + 185, m_Pos.y + 3, 130, 15)) {
		if (strcmp(g_DuelMgr.GetGuild(DUEL_ENEMY), "")) {
			unicode::_sprintf(buffName, "%s", g_DuelMgr.GetGuild(DUEL_ENEMY));
		}
	}
	g_pRenderText->SetTextColor(255, 23, 38, 255);
	g_pRenderText->SetFont(g_hFontBigBold);
	g_pRenderText->RenderText(m_Pos.x + 190, m_Pos.y + 3, buffName, 110, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFontMediumBold);
	g_pRenderText->RenderText(m_Pos.x + 150+10, m_Pos.y + 8, strDuelScore);


	


#endif

}

float SEASON3B::CNewUIDuelWindow::GetLayerDepth()
{
	return 1.1f;
}

void SEASON3B::CNewUIDuelWindow::LoadImages()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_Figure_ground.tga", IMAGE_DUEL_BACK, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\Interface_Duel.tga", IMAGE_DUEL_BACK, GL_LINEAR);
	LoadBitmap("Interface\\Duel_Life_Bar_Back.jpg", IMAGE_DUEL_HPBAR_BACK, GL_LINEAR);
	LoadBitmap("Interface\\Duel_Life_Bar.jpg", IMAGE_DUEL_HPBAR, GL_LINEAR);
	LoadBitmap("Interface\\Duel_SD_Bar.jpg", IMAGE_DUEL_SDBAR, GL_LINEAR);

#endif
}

void SEASON3B::CNewUIDuelWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_DUEL_BACK);
#if CLIENT_VERSION >= 1001
	DeleteBitmap(IMAGE_DUEL_HPBAR_BACK);
	DeleteBitmap(IMAGE_DUEL_HPBAR);
	DeleteBitmap(IMAGE_DUEL_SDBAR);
#endif
}
