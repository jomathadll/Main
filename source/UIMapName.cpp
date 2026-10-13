//*****************************************************************************
// File: UIMapName.cpp
//*****************************************************************************

#include "stdafx.h"
#include "UIMapName.h"
#include "MapManager.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "WideScreen.h"

#include "UIWindows.h"
#ifdef ASG_ADD_GENS_SYSTEM
#include "ZzzInventory.h"
#endif	// ASG_ADD_GENS_SYSTEM

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

#define	UIMN_SHOW_TIME			5000
#define	UIMN_ALPHA_VARIATION	0.015f

#define	UIMN_IMG_WIDTH			166.0f
#define	UIMN_IMG_HEIGHT			90.0f

#ifdef ASG_ADD_GENS_SYSTEM
#define UIMN_STRIFE_HEIGHT		28.0f
#endif	// ASG_ADD_GENS_SYSTEM

#define	UIMN_IMG_POS_X		((::GetScreenWidth() * g_fScreenRate_x - UIMN_IMG_WIDTH) / 2.0f)
#define	UIMN_IMG_POS_Y		(220.0f * g_fScreenRate_y)

#ifdef ASG_ADD_GENS_SYSTEM
#define UIMN_STRIFE_POS_Y	(UIMN_IMG_POS_Y - UIMN_STRIFE_HEIGHT)
#endif	// ASG_ADD_GENS_SYSTEM

CUIMapName::CUIMapName()
{
	InitImgPathMap();
}

CUIMapName::~CUIMapName()
{

}

void CUIMapName::InitImgPathMap()
{
	std::string strFolderName = "Local\\" + g_strSelectedML + "\\ImgsMapName\\";

	m_mapImgPath[0] = strFolderName + "lorencia.tga";
	m_mapImgPath[1] = strFolderName + "dungeon.tga";
	m_mapImgPath[2] = strFolderName + "devias.tga";
	m_mapImgPath[3] = strFolderName + "noria.tga";
	m_mapImgPath[4] = strFolderName + "losttower.tga";

	m_mapImgPath[6] = strFolderName + "stadium.tga";
	m_mapImgPath[7] = strFolderName + "atlans.tga";
	m_mapImgPath[8] = strFolderName + "tarkan.tga";
	m_mapImgPath[9] = strFolderName + "devilsquare.tga";
	m_mapImgPath[10] = strFolderName + "icarus.tga";
	m_mapImgPath[11] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[12] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[13] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[14] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[15] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[16] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[17] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[18] = strFolderName + "chaoscastle.tga";
	m_mapImgPath[19] = strFolderName + "chaoscastle.tga";
	m_mapImgPath[20] = strFolderName + "chaoscastle.tga";
	m_mapImgPath[21] = strFolderName + "chaoscastle.tga";
	m_mapImgPath[22] = strFolderName + "chaoscastle.tga";
	m_mapImgPath[23] = strFolderName + "chaoscastle.tga";
	m_mapImgPath[24] = strFolderName + "kalima.tga";
	m_mapImgPath[25] = strFolderName + "kalima.tga";
	m_mapImgPath[26] = strFolderName + "kalima.tga";
	m_mapImgPath[27] = strFolderName + "kalima.tga";
	m_mapImgPath[28] = strFolderName + "kalima.tga";
	m_mapImgPath[29] = strFolderName + "kalima.tga";
	m_mapImgPath[30] = strFolderName + "valleyofloren.tga";
	m_mapImgPath[31] = strFolderName + "landoftrial.tga";

	m_mapImgPath[33] = strFolderName + "aida.tga";
	m_mapImgPath[34] = strFolderName + "crywolffortress.tga";

	m_mapImgPath[36] = strFolderName + "kalima.tga"; //lost kalima
	m_mapImgPath[37] = strFolderName + "kanturu.tga";
	m_mapImgPath[38] = strFolderName + "kanturu.tga";
	m_mapImgPath[39] = strFolderName + "kantururemain.tga";

	m_mapImgPath[41] = strFolderName + "balgasbarrack.tga";
	m_mapImgPath[42] = strFolderName + "balgasrefuge.tga";

	m_mapImgPath[45] = strFolderName + "illusiontemple.tga";
	m_mapImgPath[46] = strFolderName + "illusiontemple.tga";
	m_mapImgPath[47] = strFolderName + "illusiontemple.tga";
	m_mapImgPath[48] = strFolderName + "illusiontemple.tga";
	m_mapImgPath[49] = strFolderName + "illusiontemple.tga";
	m_mapImgPath[50] = strFolderName + "illusiontemple.tga";

	m_mapImgPath[51] = strFolderName + "elbeland.tga";
	m_mapImgPath[52] = strFolderName + "bloodcastle.tga";
	m_mapImgPath[53] = strFolderName + "chaoscastle.tga";

	m_mapImgPath[56] = strFolderName + "swampofcalmness.tga";
	m_mapImgPath[57] = strFolderName + "raklion.tga";
	m_mapImgPath[58] = strFolderName + "raklionboss.tga";
	
	m_mapImgPath[62] = strFolderName + "santatown.tga";

	m_mapImgPath[63] = strFolderName + "vulcanus.tga";

	m_mapImgPath[64] = strFolderName + "coliseum.tga";

	m_mapImgPath[65] = strFolderName + "doppelganger.tga";

	m_mapImgPath[66] = strFolderName + "doppelganger.tga";

	m_mapImgPath[67] = strFolderName + "doppelganger.tga";

	m_mapImgPath[68] = strFolderName + "doppelganger.tga";

	m_mapImgPath[69] = strFolderName + "empirefortress.tga";

	m_mapImgPath[70] = strFolderName + "empirefortress.tga";

	m_mapImgPath[71] = strFolderName + "empirefortress.tga";

	m_mapImgPath[72] = strFolderName + "empirefortress.tga";

	m_mapImgPath[79] = strFolderName + "lorenmarket.tga";

#ifdef ASG_ADD_MAP_KARUTAN
	m_mapImgPath[80] = strFolderName + "karutan.tga";
	m_mapImgPath[81] = strFolderName + "karutan.tga";
#endif	// ASG_ADD_MAP_KARUTAN

	m_mapImgPath[82] = strFolderName + "darkvillage.tga";

	m_mapImgPath[83] = strFolderName + "map83.tga";
	m_mapImgPath[84] = strFolderName + "map84.tga";
	m_mapImgPath[85] = strFolderName + "map85.tga";
	m_mapImgPath[86] = strFolderName + "map86.tga";
	m_mapImgPath[87] = strFolderName + "map87.tga";
	m_mapImgPath[88] = strFolderName + "map88.tga";
	m_mapImgPath[89] = strFolderName + "map89.tga";
	m_mapImgPath[90] = strFolderName + "map90.tga";
	m_mapImgPath[91] = strFolderName + "map91.tga";
	m_mapImgPath[92] = strFolderName + "map92.tga";
}

void CUIMapName::Init()
{
	m_eState = HIDE;
	m_nOldWorld = -1;
	m_dwOldTime = ::timeGetTime();
	m_dwDeltaTickSum = 0;
	m_fAlpha = 1.0f;
#ifdef ASG_ADD_GENS_SYSTEM
	m_bStrife = false;
#endif	// ASG_ADD_GENS_SYSTEM
}

void CUIMapName::ShowMapName()
{
	m_eState = FADEIN;
	m_fAlpha = 0.2f;
	m_dwDeltaTickSum = 0;

	if(gMapManager.WorldActive == WD_40AREA_FOR_GM)
	{
		m_eState = HIDE;
		return;
	}

#if CLIENT_VERSION == 1002
	if (m_nOldWorld != gMapManager.WorldActive || gMapManager.WorldActive == WD_81KARUTAN2)
#else
	if (m_nOldWorld != gMapManager.WorldActive)
#endif
	{
		char szImgPath[128];
#if CLIENT_VERSION == 1002
		if (gMapManager.WorldActive == WD_DARKVILLAGE)
		{

			if (Hero->PositionX >= 0 && Hero->PositionX <= 85 && Hero->PositionY >= 0 && Hero->PositionY <= 65) 
			{
				sprintf(szImgPath, "Local\\%s\\ImgsMapName\\darkvillage.tga", g_strSelectedML.c_str());
			}

			if (Hero->PositionX >= 160 && Hero->PositionX <= 255 && Hero->PositionY >= 0 && Hero->PositionY <= 100) 
			{
				sprintf(szImgPath, "Local\\%s\\ImgsMapName\\mysticvillage.tga", g_strSelectedML.c_str());
			}

			if (Hero->PositionX >= 160 && Hero->PositionX <= 255 && Hero->PositionY >= 160 && Hero->PositionY <= 255) 
			{
				sprintf(szImgPath, "Local\\%s\\ImgsMapName\\elvenvillage.tga", g_strSelectedML.c_str());
			}
				
		}
		else {
			::strcpy(szImgPath, m_mapImgPath[gMapManager.WorldActive].data());
		}
#else
		//::strcpy(szImgPath, m_mapImgPath[gMapManager.WorldActive].data());
#endif

		//DeleteBitmap(BITMAP_INTERFACE_EX+45);
		//LoadBitmap(szImgPath, BITMAP_INTERFACE_EX+45);
		
		m_nOldWorld = gMapManager.WorldActive;

#ifdef ASG_ADD_GENS_SYSTEM
		m_bStrife = ::IsStrifeMap(gMapManager.WorldActive);
#endif	// ASG_ADD_GENS_SYSTEM
	}
}

void CUIMapName::Update()
{
	DWORD dwNowTime = ::timeGetTime();
	DWORD dwDeltaTick = dwNowTime - m_dwOldTime;

	switch (m_eState)
	{
	case FADEIN:
		m_fAlpha += UIMN_ALPHA_VARIATION;
		if (1.0f <= m_fAlpha)
		{
			m_eState = SHOW;
			m_fAlpha = 1.0f;
		}
		break;

	case SHOW:
		m_dwDeltaTickSum += dwDeltaTick;
		if (m_dwDeltaTickSum > UIMN_SHOW_TIME)
		{
			m_eState = FADEOUT;
			m_dwDeltaTickSum = 0;
		}
		break;

	case FADEOUT:
		m_fAlpha -= UIMN_ALPHA_VARIATION;
		if (0.0f >= m_fAlpha)
		{
			m_eState = HIDE;
			m_fAlpha = 0.0f;
		}
		break;
	}

	m_dwOldTime = dwNowTime;
}

void CUIMapName::Render()
{
	Update();

	if (HIDE == m_eState)
		return;

	::EnableAlphaTest();
	::glColor4f(1.0f, 1.0f, 1.0f, m_fAlpha);

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502

#ifdef ASG_ADD_GENS_SYSTEM
	if (m_bStrife)
		::RenderBitmap(BITMAP_INTERFACE_EX+47, UIMN_IMG_POS_X, UIMN_STRIFE_POS_Y,
			UIMN_IMG_WIDTH, UIMN_STRIFE_HEIGHT, 0.0f, 0.0f,	UIMN_IMG_WIDTH / 256.0f, UIMN_STRIFE_HEIGHT / 32.0f, false, false);
#endif	// ASG_ADD_GENS_SYSTEM
	::RenderBitmap(BITMAP_INTERFACE_EX+45, UIMN_IMG_POS_X, UIMN_IMG_POS_Y,
		UIMN_IMG_WIDTH, UIMN_IMG_HEIGHT, 0.0f, 0.0f,UIMN_IMG_WIDTH / 256.0f, UIMN_IMG_HEIGHT / 128.0f, false, false);
#else
	//SEASON3B::RenderImageScale(BITMAP_INTERFACE_EX + 45, (gWideScreen.g_WideWindowWidth/2)-65, (gWideScreen.g_WideWindowHeight/2)-(139/2), 256, 139, 0, 0, 500, 271, 0);
#endif

	::glColor4f(1.0f,1.0f,1.0f,1.0f);
	::DisableAlphaBlend();
}