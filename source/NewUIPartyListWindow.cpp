// NewUIPartyListWindow.cpp: implementation of the CNewUIPartyInfo class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIPartyListWindow.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "ZzzInventory.h"
#include "CharacterManager.h"
#include "SkillManager.h"
#if CLIENT_VERSION >= 1001
#include "PartyBuff.h"
#endif

using namespace SEASON3B;

int biVal_list[6] = {0,0,0,0,0,0};

CNewUIPartyListWindow::CNewUIPartyListWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	m_bActive = false;
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_iVal = 24;
#elif CLIENT_VERSION >= 1001
	m_iVal = 34;
#endif
	m_iLimitUserIDHeight[0] = 48;
	m_iLimitUserIDHeight[1] = 58;
	m_iSelectedCharacter = -1;
#if CLIENT_VERSION >= 1001
	m_minimizeState = false;
#endif

	for( int i=0 ; i<MAX_PARTYS ; i++ )
	{
		m_iPartyListBGColor[ i ]  = PARTY_LIST_BGCOLOR_DEFAULT;
		m_bPartyMemberoutofSight[ i ] = false;
	}
}

CNewUIPartyListWindow::~CNewUIPartyListWindow()
{
	Release();
}

bool CNewUIPartyListWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_PARTY_INFO_WINDOW, this );
#if CLIENT_VERSION >= 1001
	m_minimizeState = false;
#endif
	SetPos(x, y);
	
	LoadImages();
	
	// Exit Party Button Initialize
	for( int i=0 ; i<MAX_PARTYS ; i++ )
	{	
		int iVal = i*m_iVal;
		m_BtnPartyExit[i].ChangeButtonImgState( true, IMAGE_PARTY_LIST_EXIT );
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		m_BtnPartyExit[i].ChangeButtonInfo( m_Pos.x+63, m_Pos.y+3+iVal, 11, 11 );
#elif CLIENT_VERSION >= 1001
		m_BtnPartyExit[i].ChangeButtonInfo( m_Pos.x + 2 + PARTY_LIST_WINDOW_WIDTH - 3 - 9, m_Pos.y+20+iVal+3, 8, 8);
#endif
	}	

	Show( true );
	
	return true;
}

void CNewUIPartyListWindow::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void CNewUIPartyListWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
	
	for( int i=0 ; i<MAX_PARTYS ; i++ )
	{	
		int iVal = i*m_iVal;
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		m_BtnPartyExit[i].ChangeButtonInfo(m_Pos.x + 63, m_Pos.y + 3 + iVal, 11, 11);
#elif CLIENT_VERSION >= 1001
		m_BtnPartyExit[i].ChangeButtonInfo(m_Pos.x + 2 + PARTY_LIST_WINDOW_WIDTH - 3 - 9, m_Pos.y + 20 + iVal + 3, 8, 8);
#endif
	}
}

void CNewUIPartyListWindow::SetPos(int x)
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	SetPos(x - (PARTY_LIST_WINDOW_WIDTH + 2), m_Pos.y);
#elif CLIENT_VERSION >= 1001
	m_Pos.y = g_pItemEnduranceInfo->GetUIEndPos();
	SetPos((GetScreenWidth() - PARTY_LIST_WINDOW_WIDTH - 5), m_Pos.y);
#endif
}

int CNewUIPartyListWindow::GetSelectedCharacter()
{
	if( m_iSelectedCharacter == -1 )
		return -1;
	
	return Party[m_iSelectedCharacter].index;
}

void CNewUIPartyListWindow::SetListBGColor( )
{
	for( int i=0 ; i<PartyNumber ; i++)
	{	
		m_iPartyListBGColor[ i ] = PARTY_LIST_BGCOLOR_DEFAULT;
		
		if( Party[i].index == -1 )
		{
			m_iPartyListBGColor[ i ] = PARTY_LIST_BGCOLOR_RED;
		}
		
		if( Party[i].index>-1 )
		{
			m_iPartyListBGColor[ i ]  = PARTY_LIST_BGCOLOR_GREEN;
		}
	}
}

bool CNewUIPartyListWindow::BtnProcess()
{
	m_iSelectedCharacter = -1;

	for( int i=0 ; i<PartyNumber ; i++ )
	{	
		int iVal = i*m_iVal;	

		iVal += biVal_list[i];
		
		if( !strcmp( Party[0].Name, Hero->ID ) || !strcmp( Party[i].Name, Hero->ID ) )
		{
			if( m_BtnPartyExit[i].UpdateMouseEvent() )
			{
				g_pPartyInfoWindow->LeaveParty( i );
				return true;
			}
		}

		if( CheckMouseIn(m_Pos.x + 2, m_Pos.y+20 + 2 + iVal, PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT+biVal_list[i+1]) )
		{
			m_iSelectedCharacter = i;

			if( SelectedCharacter==-1) {
				CHARACTER *c = &CharactersClient[Party[i].index];
				if( c && c != Hero ) {
					CreateChat ( c->ID, "", c );
				}
			}

			if( SelectCharacterInPartyList(	&Party[i] ) )	
			{
				return true;
			}
		}
	}
	
	return false;
}

bool CNewUIPartyListWindow::UpdateMouseEvent()
{
	if( !m_bActive )
		return true;
	
	if( true == BtnProcess() )
		return false;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502

	if (PartyNumber > 0)
	{
		int iHeight = (PARTY_LIST_WINDOW_HEIGHT * PartyNumber) + (4 * (PartyNumber - 1));
		if (CheckMouseIn(m_Pos.x, m_Pos.y, PARTY_LIST_WINDOW_WIDTH, iHeight + 20))
		{
			return false;
		}
	}

#elif CLIENT_VERSION >= 1001
	int iNextPosY = m_Pos.y;
	if (CheckMouseIn(m_Pos.x, iNextPosY, 100, 20) && SEASON3B::IsPress(VK_LBUTTON)) {
		if (CheckMouseIn(m_Pos.x + 80, iNextPosY + 2, 13, 13) && SEASON3B::IsPress(VK_LBUTTON)) {
			m_minimizeState = !m_minimizeState;

			PlayBuffer(SOUND_CLICK01);
		}
		return false;
	}

	if( PartyNumber > 0)
	{
		int iHeight = (PARTY_LIST_WINDOW_HEIGHT * PartyNumber) + (4 * (PartyNumber - 1));
		if (CheckMouseIn(m_Pos.x - 5, iNextPosY, 100.f, iHeight + 30))
		{
			return false;
		}
	}
#endif
	
	return true;
}

bool CNewUIPartyListWindow::UpdateKeyEvent()
{
	return true;
}

bool CNewUIPartyListWindow::Update()
{
	if( PartyNumber <= 0 )
	{
		m_bActive = false;
		return true;
	}
	
	m_bActive = true;
	
	for( int i=0 ; i<PartyNumber ; i++)
	{	
		Party[ i ].index = -2;
	}
	
	return true;
}

bool CNewUIPartyListWindow::Render()
{
	if( !m_bActive )
		return true;

	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);
	
	int iNextPosY = m_Pos.y;

#if CLIENT_VERSION >= 1001
	RenderImageScale(IMAGE_PARTY_LIST_MINIMIZE_BAR, m_Pos.x - 5, iNextPosY, 100.f, 18.f, 0.f, 0.f, 120.f, 20.f);
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(m_Pos.x - 5 + (95 / 2), iNextPosY + 4.5f, GlobalText[190], 0, 0, RT3_WRITE_CENTER);
	if (m_minimizeState) {
		RenderBitmap(IMAGE_PARTY_LIST_MINIMIZE_BTN, m_Pos.x + 80, iNextPosY + 2, 13, 13, 0.008, 0.263782, 0.51488634, 0.263782, 1, 1, 1.0);
		return true;
	}
	RenderBitmap(IMAGE_PARTY_LIST_MINIMIZE_BTN, m_Pos.x + 80, iNextPosY + 2, 13, 13, 0.008, 0.52488634, 0.51488634, 0.263782, 1, 1, 1.0);

	

	iNextPosY += 20;
#endif

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	g_pRenderText->SetFont( g_hFont );
	g_pRenderText->SetTextColor( 255, 255, 255, 255 );
	g_pRenderText->SetBgColor( 0, 0, 0, 0 );
	
	for( int i=0 ; i<PartyNumber ; i++)
	{	
		int iVal = i*m_iVal;
		
		glColor4f ( 0.f, 0.f, 0.f, 0.9f );
		RenderColor( float(m_Pos.x+2), float(iNextPosY +2+iVal), PARTY_LIST_WINDOW_WIDTH-3, PARTY_LIST_WINDOW_HEIGHT-6 );
		EnableAlphaTest();
		
		if( Party[i].index == -1 )
		{
			glColor4f ( 0.3f, 0.f, 0.f, 0.5f );
			RenderColor( m_Pos.x+2, iNextPosY +2+iVal, PARTY_LIST_WINDOW_WIDTH-3, PARTY_LIST_WINDOW_HEIGHT-6 );
			EnableAlphaTest();
		}
		else
		{	
			if( Party[i].index >= 0 && Party[i].index < MAX_CHARACTERS_CLIENT )
			{
				CHARACTER* pChar = &CharactersClient[Party[i].index];
				OBJECT* pObj = &pChar->Object;
				
				if(g_isCharacterBuff(pObj, eBuff_Defense) == true)
				{
					glColor4f ( 0.2f, 1.f, 0.2f, 0.2f );
					RenderColor( m_Pos.x+2, iNextPosY+2+iVal, PARTY_LIST_WINDOW_WIDTH-3, PARTY_LIST_WINDOW_HEIGHT-6 );
					EnableAlphaTest();
				}
			}
			if( m_iSelectedCharacter != -1 && m_iSelectedCharacter == i )
			{
				glColor4f ( 0.4f, 0.4f, 0.4f, 0.7f );
				RenderColor( m_Pos.x+2, iNextPosY+2+iVal, PARTY_LIST_WINDOW_WIDTH-3, PARTY_LIST_WINDOW_HEIGHT-6 );
				EnableAlphaTest();
			}
		}
		
		EndRenderColor();
		RenderImage( IMAGE_PARTY_LIST_BACK, m_Pos.x, iNextPosY+iVal, PARTY_LIST_WINDOW_WIDTH, PARTY_LIST_WINDOW_HEIGHT );
			
		if( i==0 )
		{	
			if( Party[i].index == -1 )
			{
				g_pRenderText->SetTextColor( RGBA( 128, 75, 11, 255 ) );		
			}
			else
			{
				g_pRenderText->SetTextColor( RGBA( 255, 148, 22, 255 ) );
			}
			
			RenderImage( IMAGE_PARTY_LIST_FLAG, m_Pos.x+53, iNextPosY+3, 9, 10 );
			g_pRenderText->RenderText( m_Pos.x+4, iNextPosY+4+iVal, Party[i].Name, m_iLimitUserIDHeight[0], 0, RT3_SORT_LEFT );
		}
		else
		{
			if( Party[i].index == -1 )
			{
				g_pRenderText->SetTextColor( RGBA( 128, 128, 128, 255 ) );
			}
			else
			{
				g_pRenderText->SetTextColor( RGBA( 255, 255, 255, 255 ) );
			}
			g_pRenderText->RenderText( m_Pos.x+4, iNextPosY+4+iVal, Party[i].Name, m_iLimitUserIDHeight[1], 0, RT3_SORT_LEFT );
		}
		
		int iStepHP = min( 10, Party[i].stepHP );
		float fLife = ((float)iStepHP/(float)10)*(float)PARTY_LIST_HP_BAR_WIDTH;
		RenderImage(IMAGE_PARTY_LIST_HPBAR, m_Pos.x+4, iNextPosY+16+iVal, fLife, 3);
		
		if( !strcmp( Party[0].Name, Hero->ID ) || !strcmp( Party[i].Name, Hero->ID ) )
		{
			m_BtnPartyExit[i].Render();
		}
	}

#elif CLIENT_VERSION >= 1001
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 0);

	int bIval = 0;
		int addY = 0;

	for (int i = 0; i < PartyNumber; i++)
	{
		int iVal = i * m_iVal;

		iVal += biVal_list[i];

		glColor4f(0.f, 0.f, 0.f, 0.9f);
		RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal), PARTY_LIST_WINDOW_WIDTH - 3, 10);
		EnableAlphaTest();


		if (Party[i].index == -1)
		{
			auto buffs = gPartyBuffs.GetInfoByName(Party[i].Name);
			if (buffs != nullptr) {
				if (buffs->count > 0) {
					glColor4f(0.f, 0.f, 0.f, 0.6f);
					RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
					glColor4f(0.3f, 0.f, 0.f, 0.5f);
					RenderColor(m_Pos.x + 2, iNextPosY + 2 + iVal + 10, PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
					addY += 10;
					EnableAlphaTest();
				}
				else {
					glColor4f(0.f, 0.f, 0.f, 0.6f);
					RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT - 10);
					glColor4f(0.3f, 0.f, 0.f, 0.5f);
					RenderColor(m_Pos.x + 2, iNextPosY + 2 + iVal + 10, PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT - 10);
					EnableAlphaTest();
				}
			}
		}
		else
		{
			if (Party[i].index >= 0 && Party[i].index < MAX_CHARACTERS_CLIENT)
			{
				CHARACTER* pChar = &CharactersClient[Party[i].index];
				OBJECT* pObj = &pChar->Object;

				if (g_isCharacterBuff(pObj, eBuff_Defense) == true)
				{
					glColor4f(0.f, 0.f, 0.f, 0.6f);
					RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
					glColor4f(0.2f, 1.f, 0.2f, 0.2f);
					RenderColor(m_Pos.x + 2, iNextPosY + 2 + iVal + 10, PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
					addY += 10;
					EnableAlphaTest();
				}
				else {
					auto buffs = gPartyBuffs.GetInfoByName(Party[i].Name);
					if (buffs != nullptr) {
						if (buffs->count > 0) {
							glColor4f(0.f, 0.f, 0.f, 0.6f);
							RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
							addY += 10;
						}
						else {
							glColor4f(0.f, 0.f, 0.f, 0.6f);
							RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT - 10);
						}
					}

					EnableAlphaTest();
				}
			}

			if (m_iSelectedCharacter != -1 && m_iSelectedCharacter == i)
			{
				auto buffs = gPartyBuffs.GetInfoByName(Party[i].Name);
				if (buffs != nullptr) {
					if (buffs->count > 0) {
						glColor4f(0.f, 0.f, 0.f, 0.6f);
						RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
						glColor4f(0.4f, 0.4f, 0.4f, 0.7f);
						RenderColor(m_Pos.x + 2, iNextPosY + 2 + iVal + 10, PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT);
					}
					else {
						glColor4f(0.f, 0.f, 0.f, 0.6f);
						RenderColor(float(m_Pos.x + 2), float(iNextPosY + 2 + iVal + 10), PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT - 10);
						glColor4f(0.4f, 0.4f, 0.4f, 0.7f);
						RenderColor(m_Pos.x + 2, iNextPosY + 2 + iVal + 10, PARTY_LIST_WINDOW_WIDTH - 3, PARTY_LIST_WINDOW_HEIGHT - 10);
					}
				}

				EnableAlphaTest();
			}
		}

		EndRenderColor();

		g_pRenderText->SetFont(g_hFontSmall);
		if (i == 0)
		{
			if (Party[i].index == -1)
			{
				g_pRenderText->SetTextColor(RGBA(128, 75, 11, 255));
			}
			else
			{
				g_pRenderText->SetTextColor(RGBA(255, 148, 22, 255));
			}

			RenderImage(IMAGE_PARTY_LIST_FLAG, m_Pos.x + 4, iNextPosY + 2, 9, 10);
			g_pRenderText->RenderText(m_Pos.x + 2 + ((PARTY_LIST_WINDOW_WIDTH - 3) / 2), iNextPosY + 3.5f + iVal, Party[i].Name, PARTY_LIST_WINDOW_WIDTH - 3, 0, RT3_WRITE_CENTER);

		}
		else
		{
			if (Party[i].index == -1)
			{
				g_pRenderText->SetTextColor(RGBA(128, 128, 128, 255));
			}
			else
			{
				g_pRenderText->SetTextColor(RGBA(255, 255, 255, 255));
			}
			g_pRenderText->RenderText(m_Pos.x + 2 + ((PARTY_LIST_WINDOW_WIDTH - 3) / 2), iNextPosY + 3.5f + iVal, Party[i].Name, PARTY_LIST_WINDOW_WIDTH - 3, 0, RT3_WRITE_CENTER);
		}

		float fLife = ((float)Party[i].currHP / (float)Party[i].maxHP) * (float)PARTY_LIST_HP_BAR_WIDTH;

		RenderImageScale(IMAGE_PARTY_LIST_BACK, m_Pos.x + 5, iNextPosY + iVal + 15, PARTY_LIST_HP_BAR_WIDTH, PARTY_LIST_HP_BAR_HEIGHT, 0.f, 0.f, 333.f, 14.f);
		RenderImageScale(IMAGE_PARTY_LIST_HPBAR, m_Pos.x + 5, iNextPosY + iVal + 15, fLife, PARTY_LIST_HP_BAR_HEIGHT, 0.f, 0.f, 333.f, 14.f);

		unicode::t_char szText[256] = { 0, };
		g_pRenderText->SetTextColor(RGBA(237, 214, 161, 255));
		unicode::_sprintf(szText, GlobalText[358], Party[i].currHP, Party[i].maxHP);
		g_pRenderText->RenderText(m_Pos.x + PARTY_LIST_WINDOW_WIDTH - 5, iNextPosY + iVal + 21, szText, 90, 0, RT3_WRITE_RIGHT_TO_LEFT);

		float x = 0.0f, y = 0.0f;
		x = m_Pos.x + 5;
		y = iNextPosY + iVal + 30;
		auto buffs = gPartyBuffs.GetInfoByName(Party[i].Name);
		if (buffs != nullptr){
			if (buffs->count > 0) {
				gPartyBuffs.DrawPartyBuffs(x, y, Party[i].Name);
			}
		}
		/*
		if (Party[i].index >= 0)
		{

			float x = 0.0f, y = 0.0f;
			int buffwidthcount = 0, buffheightcount = 0;

			CHARACTER* pChar = &CharactersClient[Party[i].index];
			OBJECT* pObj = &pChar->Object;

			std::list<eBuffState> buffstate;
			BuffSort(pObj, buffstate);

			std::list<eBuffState>::iterator iter;
			for (iter = buffstate.begin(); iter != buffstate.end(); )
			{
				std::list<eBuffState>::iterator tempiter = iter;
				++iter;
				eBuffState buff = (*tempiter);

				x = m_Pos.x + 5 + (buffwidthcount * (7 + 1));
				y = iNextPosY + iVal + 30;

				RenderBuffIcon(buff, x, y, 9, 9);
				buffwidthcount++;
			}

			buffwidthcount = 0, buffheightcount = 0;
			for (iter = buffstate.begin(); iter != buffstate.end(); )
			{
				std::list<eBuffState>::iterator tempiter = iter;
				++iter;
				eBuffState buff = (*tempiter);

				x = m_Pos.x + 5 + (buffwidthcount * (7 + 1));
				y = iNextPosY + iVal + 30;

				if (SEASON3B::CheckMouseIn(x, y, 8, 10)) {
					eBuffClass buffclass = g_IsBuffClass(buff);
					EnableAlphaTest();
					RenderBuffTooltip(buffclass, buff, x, y - 20);
					EndRenderColor();
				}
				buffwidthcount++;
			}
		}
		*/

		biVal_list[i+1] = addY;

		if (!strcmp(Party[0].Name, Hero->ID) || !strcmp(Party[i].Name, Hero->ID))
		{
			m_BtnPartyExit[i].ChangeButtonInfo(m_Pos.x + 2 + PARTY_LIST_WINDOW_WIDTH - 3 - 9, m_Pos.y + 20 + iVal + 3, 8, 8);

			m_BtnPartyExit[i].Render();
		}
	}


#endif
	
	DisableAlphaBlend();
	
	return true;
}

void SEASON3B::CNewUIPartyListWindow::RenderPartyHPOnHead( )
{
    if ( PartyNumber<=0 ) 
		return;

    float   Width = 38.f;
    char    Text[100];

    for ( int j=0; j<PartyNumber; ++j )
    {
		PARTY_t *p = &Party[j];

        if (p->index<=-1) continue;

        CHARACTER*  c = &CharactersClient[p->index];
	    OBJECT*     o = &c->Object;
	    vec3_t      Position;
	    int         ScreenX, ScreenY;

	    Vector ( o->Position[0], o->Position[1], o->Position[2]+o->BoundingBoxMax[2]+100.f, Position );
		
		BeginOpengl();
		Projection ( Position, &ScreenX, &ScreenY );
		EndOpengl();

        ScreenX -= (int)(Width/2);

	    if((MouseX>=ScreenX && MouseX<ScreenX+Width && MouseY>=ScreenY-2 && MouseY<ScreenY+6))
	    {
		    sprintf ( Text,"HP : %d0%%", p->stepHP );
			g_pRenderText->SetTextColor(255, 230, 210, 255);
		    g_pRenderText->RenderText(ScreenX, ScreenY-6, Text);
	    }

	    EnableAlphaTest ();
   	    glColor4f ( 0.f, 0.f, 0.f, 0.5f );
	    RenderColor ( (float)(ScreenX+1), (float)(ScreenY+1), Width+4.f, 5.f );

	    EnableAlphaBlend ();
	    glColor3f ( 0.2f, 0.0f, 0.0f );
	    RenderColor ( (float)ScreenX, (float)ScreenY, Width+4.f, 5.f );

        glColor3f ( 50.f/255.f, 10/255.f, 0.f );
	    RenderColor ( (float)(ScreenX+2), (float)(ScreenY+2), Width, 1.f );

        int stepHP = min ( 10, p->stepHP );

        glColor3f ( 250.f/255.f, 10/255.f, 0.f );
        for ( int k=0; k<stepHP; ++k )
        {
	        RenderColor ( (float)(ScreenX+2+(k*4)), (float)(ScreenY+2), 3.f, 2.f );
        }
	    DisableAlphaBlend();
    }
	DisableAlphaBlend();
	glColor3f(1.f,1.f,1.f);
}

float CNewUIPartyListWindow::GetLayerDepth()
{
	return 5.4f;
}

void CNewUIPartyListWindow::OpenningProcess()
{
	
}

void CNewUIPartyListWindow::ClosingProcess()
{
	
}

#if CLIENT_VERSION >= 1001

void CNewUIPartyListWindow::RenderBuffTooltip(eBuffClass& eBuffClassType, eBuffState& eBuffType, float x, float y)
{
	int TextNum = 0;
	::memset(TextList[0], 0, sizeof(char) * 30 * 100);
	::memset(TextListColor, 0, sizeof(int) * 30);
	::memset(TextBold, 0, sizeof(int) * 30);

	std::list<std::string> tooltipinfo;
	g_BuffToolTipString(tooltipinfo, eBuffType);

	for (std::list<std::string>::iterator iter = tooltipinfo.begin(); iter != tooltipinfo.end(); ++iter)
	{
		std::string& temp = *iter;

		unicode::_sprintf(TextList[TextNum], temp.c_str());

		if (TextNum == 0)
		{
			TextListColor[TextNum] = TEXT_COLOR_BLUE;
			TextBold[TextNum] = true;
		}
		else
		{
			TextListColor[TextNum] = TEXT_COLOR_WHITE;
			TextBold[TextNum] = false;
		}

		TextNum += 1;
	}

	std::string bufftime;
	g_BuffStringTime(eBuffType, bufftime);

	if (bufftime.size() != 0)
	{
		unicode::_sprintf(TextList[TextNum], GlobalText[2533], bufftime.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		TextNum += 1;
	}

	SIZE TextSize = { 0, 0 };
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize);
	RenderTipTextList(x, y, TextNum, 0);
}

void CNewUIPartyListWindow::BuffSort(OBJECT* pHeroObject, std::list<eBuffState>& buffstate)
{
	int iBuffSize = g_CharacterBuffSize(pHeroObject);

	for (int i = 0; i < iBuffSize; ++i)
	{
		eBuffState eBuffType = g_CharacterBuff(pHeroObject, i);

		if (SetDisableRenderBuff(eBuffType))	continue;

		if (eBuffType != eBuffNone) {
			eBuffClass eBuffClassType = g_IsBuffClass(eBuffType);

			if (eBuffClassType == eBuffClass_Buff) {
				buffstate.push_front(eBuffType);
			}
			else if (eBuffClassType == eBuffClass_DeBuff) {
				buffstate.push_back(eBuffType);
			}
			else {
				assert(!"SetDisableRenderBuff");
			}
		}
	}
}

void CNewUIPartyListWindow::RenderBuffIcon(eBuffState& eBuffType, float x, float y, float width, float height)
{
	float Height = (float)((28.0f / 100) * 37.5); // ((double)(unsigned int)*(QWORD*)0x0E61E5C / 480.0); //28
	float Width = (float)((20.0f / 100) * 37.5); // ((double)(unsigned int)*(QWORD*)0xE61E58 / 640.0); //20
	float RenderW = (float)((256.0f / 100) * 37.5); // ((double)(unsigned int)*(QWORD*)0xE61E58 / 640.0);

	if (eBuffType < 81) // eBuff_Berserker
	{
		int EffectValue = (eBuffType-1) % 80;
		float SourceX = (double)(EffectValue % 10) * Width / RenderW;
		float SourceY = (double)(EffectValue / 10) * Height / RenderW;
		float SourceWidth = Width / RenderW;
		float SourceHeight = Height / RenderW;

		RenderBitmap((eBuffType-1) / 80 + IMAGE_BUFF_STATUS, x, y, Width, Height, SourceX, SourceY, SourceWidth, SourceHeight, 1, 1, 0);
	}
	else
	{
		int EffectValue = (eBuffType-81) % 81;
		float SourceX = (double)(EffectValue % 10) * Width / RenderW;
		float SourceY = (double)(EffectValue / 10) * Height / RenderW;
		float SourceWidth = Width / RenderW;
		float SourceHeight = Height / RenderW;

		RenderBitmap((eBuffType-81) / 81 + IMAGE_BUFF_STATUS2, x, y, Width, Height, SourceX, SourceY, SourceWidth, SourceHeight, 1, 1, 0);
	}
}

bool CNewUIPartyListWindow::SetDisableRenderBuff(const eBuffState& _BuffState)
{
	switch (_BuffState)
	{
#ifdef PBG_ADD_PKSYSTEM_INGAMESHOP
	case eDeBuff_MoveCommandWin:
#endif //PBG_ADD_PKSYSTEM_INGAMESHOP
	case eDeBuff_FlameStrikeDamage:
	case eDeBuff_GiganticStormDamage:
	case eDeBuff_LightningShockDamage:
	case eDeBuff_Discharge_Stamina:
		return true;
	default:
		return false;
	}
	return false;
}
#endif


bool CNewUIPartyListWindow::SelectCharacterInPartyList( PARTY_t* pMember )
{
    int HeroClass = gCharacterManager.GetBaseClass( Hero->Class );
	
	if ( HeroClass==CLASS_ELF 
		|| HeroClass==CLASS_WIZARD 
		|| HeroClass == CLASS_SUMMONER
		)
	{
		int Skill = CharacterAttribute->Skill[Hero->CurrentSkill];
		
		if ( Skill==AT_SKILL_HEALING 
			|| Skill==AT_SKILL_DEFENSE 
			|| (AT_SKILL_DEF_POWER_UP <= Skill && Skill <= AT_SKILL_DEF_POWER_UP+4)
			|| (AT_SKILL_ATT_POWER_UP <= Skill && Skill <= AT_SKILL_ATT_POWER_UP+4)
			|| Skill==AT_SKILL_ATTACK 
			|| Skill==AT_SKILL_TELEPORT_B 
			|| Skill==AT_SKILL_WIZARDDEFENSE 
			|| (AT_SKILL_HEAL_UP <= Skill && Skill <= AT_SKILL_HEAL_UP+4)
			|| (AT_SKILL_SOUL_UP <= Skill && Skill <= AT_SKILL_SOUL_UP+4)
			|| Skill == AT_SKILL_ALICE_THORNS
			|| Skill == AT_SKILL_RECOVER
			)
		{
			SelectedCharacter = pMember->index;
			return true;	
		}
	}

	return false;
}

void CNewUIPartyListWindow::LoadImages()
{
	
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_party_flag.tga", IMAGE_PARTY_LIST_FLAG, GL_LINEAR);
	LoadBitmap("Interface\\newui_party_x.tga", IMAGE_PARTY_LIST_EXIT, GL_LINEAR);
	LoadBitmap("Interface\\newui_party_back.tga", IMAGE_PARTY_LIST_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_party_hpbar.jpg", IMAGE_PARTY_LIST_HPBAR, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\newui_party_flag.tga", IMAGE_PARTY_LIST_FLAG, GL_LINEAR);
	LoadBitmap("Interface\\btn_exit.jpg", IMAGE_PARTY_LIST_EXIT, GL_LINEAR);
	LoadBitmap("Interface\\minimize_bar.jpg", IMAGE_PARTY_LIST_MINIMIZE_BAR, GL_LINEAR);
	LoadBitmap("Interface\\chat\\chat_buttons_size.jpg", IMAGE_PARTY_LIST_MINIMIZE_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_statusicon.jpg", IMAGE_BUFF_STATUS, GL_LINEAR);
	LoadBitmap("Interface\\newui_statusicon2.jpg", IMAGE_BUFF_STATUS2, GL_LINEAR);
	LoadBitmap("Interface\\party_info_life_back.jpg", IMAGE_PARTY_LIST_BACK, GL_LINEAR);
	LoadBitmap("Interface\\party_info_life_top.jpg", IMAGE_PARTY_LIST_HPBAR, GL_LINEAR);


#endif
}

void CNewUIPartyListWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_PARTY_LIST_FLAG);
	DeleteBitmap(IMAGE_PARTY_LIST_EXIT);
	DeleteBitmap(IMAGE_PARTY_LIST_BACK);
	DeleteBitmap( IMAGE_PARTY_LIST_HPBAR );
#if CLIENT_VERSION >= 1001
	DeleteBitmap(IMAGE_PARTY_LIST_MINIMIZE_BAR);
	DeleteBitmap(IMAGE_PARTY_LIST_MINIMIZE_BTN);
	DeleteBitmap(IMAGE_BUFF_STATUS2);
	DeleteBitmap(IMAGE_BUFF_STATUS);
#endif
}
