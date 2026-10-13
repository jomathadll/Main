// NewUIBuffWindow.cpp: implementation of the CNewUIBuffWindow class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NewUIBuffWindow.h"
#include "ZzzBMD.h"
#include "ZzzCharacter.h"
#include "ZzzTexture.h"
#include "ZzzInventory.h"
#include "UIControls.h"
#include "NewUICommonMessageBox.h"
#include "NewUISystem.h"

#ifdef CONSOLE_DEBUG
#include "./Utilities/Log/muConsoleDebug.h"
#endif // CONSOLE_DEBUG

using namespace SEASON3B;

namespace
{
	// AQUI ALTERA O TAMANHO DOS ICONES DO BUFF
#if (CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502) || CLIENT_VERSION == 1001
	const float BUFF_IMG_WIDTH = 20.f;
	const float BUFF_IMG_HEIGHT = 28.f;
	const int BUFF_MAX_LINE_COUNT = 8;
	const int BUFF_IMG_SPACE = 5;
	const float BUFF_STACK_BUTTON_SIZE = 14.f;
	bool g_BuffWindowStacked = false;
#elif CLIENT_VERSION == 1002
	const float BUFF_IMG_WIDTH = 13.f;
	const float BUFF_IMG_HEIGHT = 18.f;
	const int BUFF_MAX_LINE_COUNT = 8;
	const int BUFF_IMG_SPACE = 5;
	const float BUFF_STACK_BUTTON_SIZE = 14.f;
	bool g_BuffWindowStacked = false;
#endif

	void RenderBuffStackButton(float x, float y, bool stacked)
	{
		(void)stacked;
		RenderBitmap(BITMAP_BUFF_STACK_BUTTON, x, y, BUFF_STACK_BUTTON_SIZE, BUFF_STACK_BUTTON_SIZE, 0.f, 0.f, 1.f, 1.f);
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	int GetSkillShopActiveBuffBitmap(eBuffState effect)
	{
		switch((int)effect)
		{
		case 44:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 0;
		case 45:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 1;
		case 46:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 2;
		case 47:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 3;
		case 48:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 4;
		case 49:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 5;
		case 89:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 2;
		case 90:
			return BITMAP_SKILL_SHOP_BUFF_ICON_BEGIN + 3;
		default:
			return 0;
		}
	}
	
};

SEASON3B::CNewUIBuffWindow::CNewUIBuffWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
#if CLIENT_VERSION == 1002
	this->AccountLevel = 0;
#endif
}

SEASON3B::CNewUIBuffWindow::~CNewUIBuffWindow()
{
	Release();
}

bool SEASON3B::CNewUIBuffWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj( SEASON3B::INTERFACE_BUFF_WINDOW, this );
	
	SetPos(x, y);

	LoadImages();
	
	Show( true );

	return true;
}

void SEASON3B::CNewUIBuffWindow::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}

void SEASON3B::CNewUIBuffWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void SEASON3B::CNewUIBuffWindow::SetPos(int iScreenWidth)
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (iScreenWidth == 640) {
		SetPos(220, 15);
}
	else if (iScreenWidth == 450) {
		SetPos(125, 15);
	}
	else if (iScreenWidth == 373) {
		SetPos(86, 15);
	}
	else if (iScreenWidth == 260) {
		SetPos(30, 15);
	}
	else {
		SetPos(220, 15);
	}
#elif CLIENT_VERSION >= 1001
	SetPos(260, 15);
#endif
	
}

void SEASON3B::CNewUIBuffWindow::BuffSort(std::list<eBuffState>& buffstate )
{
	OBJECT* pHeroObject = &Hero->Object;

	int iBuffSize = g_CharacterBuffSize( pHeroObject );
	
	for(int i=0; i<iBuffSize; ++i) 
	{
		eBuffState eBuffType = g_CharacterBuff(pHeroObject, i);

		if(SetDisableRenderBuff(eBuffType))	continue;

		if(eBuffType != eBuffNone) {
			eBuffClass eBuffClassType = g_IsBuffClass(eBuffType);
			
			if(eBuffClassType == eBuffClass_Buff) {
				buffstate.push_front( eBuffType );
			}
			else if(eBuffClassType == eBuffClass_DeBuff) {
				buffstate.push_back( eBuffType );
			}
			else {
				assert(!"SetDisableRenderBuff");
			}
		}
	}	
}

bool SEASON3B::CNewUIBuffWindow::SetDisableRenderBuff(const eBuffState& _BuffState)
{
	switch(_BuffState)
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

bool SEASON3B::CNewUIBuffWindow::UpdateMouseEvent()
{
	std::list<eBuffState> buffstate;
	BuffSort(buffstate);

	if (buffstate.empty() == false)
	{
		const float toggleX = max(2.f, m_Pos.x - (BUFF_STACK_BUTTON_SIZE + 2.f));
		const float toggleY = (float)m_Pos.y;

		if (SEASON3B::CheckMouseIn(toggleX, toggleY, BUFF_STACK_BUTTON_SIZE, BUFF_STACK_BUTTON_SIZE))
		{
			if (SEASON3B::IsRelease(VK_LBUTTON))
			{
				g_BuffWindowStacked = !g_BuffWindowStacked;
			}

			return false;
		}
	}

	if (g_pTradeX->isHidden())
	{
		if (CheckMouseIn(g_pTradeX->m_BtnShow.GetPos().x, g_pTradeX->m_BtnShow.GetPos().y, g_pTradeX->m_BtnShow.GetSize().x, g_pTradeX->m_BtnShow.GetSize().y)) {
			g_pTradeX->UpdateMouseEventTradeExtended();
			
			return false;
		}
		
	}

	/*

	float x = 0.0f, y = 0.0f;
	int buffwidthcount = 0, buffheightcount = 0;

	std::list<eBuffState> buffstate;
	BuffSort( buffstate );

	if (buffstate.empty() == false && renderstate == BUFF_RENDER_ICON)
	{
		const float toggleX = max(2.f, m_Pos.x - (BUFF_STACK_BUTTON_SIZE + 2.f));
		RenderBuffStackButton(toggleX, (float)m_Pos.y, g_BuffWindowStacked);
	}

#if CLIENT_VERSION == 1002
	if (this->AccountLevel > 0) buffwidthcount = 1;
#endif

	std::list<eBuffState>::iterator iter;
	int stackindex = 0;
	for( iter = buffstate.begin(); iter != buffstate.end(); )
	{
		std::list<eBuffState>::iterator tempiter = iter;
		++iter;
		eBuffState buff = (*tempiter);

		if (g_BuffWindowStacked)
		{
			const int renderindex = min(stackindex, 3);
			x = m_Pos.x + (renderindex * 2.f);
			y = m_Pos.y + (renderindex * 2.f);
		}
		else
		{
			x = m_Pos.x + (buffwidthcount * (BUFF_IMG_WIDTH+BUFF_IMG_SPACE));
			y = m_Pos.y + (buffheightcount * (BUFF_IMG_HEIGHT+BUFF_IMG_SPACE)) ;
		}
#if CLIENT_VERSION >= 1001
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DUEL_WINDOW)) {
			y += 35;
		}
#endif

		if( SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT) )
		{
			//BUFF ELF INFINITY ARROW
			if(buff == eBuff_InfinityArrow)
			{
				if(SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CInfinityArrowCancelMsgBoxLayout));
				}
			}
			//BUFF ELF ATTACK
			if(buff == eBuff_Attack)
			{
				if(SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffElfAttackCancelMsgBoxLayout));
				}
			}
			//BUFF ELF DEFENSE
			if(buff == eBuff_Defense)
			{
				if(SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffElfDefenseCancelMsgBoxLayout));
				}
			}
			//BUFF SM MAGIC CIRCLE
			else if(buff == eBuff_SwellOfMagicPower)
			{
				if(SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffSwellOfMPCancelMsgBoxLayOut));
				}
			}
			//BUFF SM SOUL BARRIER / MANA SHIELD
			else if(buff == eBuff_PhysDefense)
			{
				if(SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffSMCancelMsgBoxLayOut));
				}
			}
			//BUFF BK VITALITY
			else if(buff == eBuff_HpRecovery)
			{
				if(SEASON3B::IsRelease(VK_RBUTTON))
				{
					SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CBuffBKCancelMsgBoxLayOut));
				}
			}
			
			return false;
		}

		if( ++buffwidthcount >= BUFF_MAX_LINE_COUNT ) {
			buffwidthcount = 0;
			++buffheightcount;
		}
	}
	*/

	return true;
}

bool SEASON3B::CNewUIBuffWindow::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIBuffWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIBuffWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	if (g_pTradeX->isHidden())
	{
		g_pTradeX->RenderIconTradeExtended();
	}

	// ATIVA OU DESETIVA OS ICONES DOS BUFFS

	RenderBuffStatus( BUFF_RENDER_ICON );

	RenderBuffStatus( BUFF_RENDER_TOOLTIP );
		
	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIBuffWindow::RenderStackButton()
{
	std::list<eBuffState> buffstate;
	BuffSort(buffstate);

	if (buffstate.empty())
	{
		return;
	}

	const float toggleX = max(2.f, m_Pos.x - (BUFF_STACK_BUTTON_SIZE + 2.f));
	RenderBuffStackButton(toggleX, (float)m_Pos.y, g_BuffWindowStacked);
}

void SEASON3B::CNewUIBuffWindow::RenderBuffStatus( BUFF_RENDER renderstate )
{
	float x = 0.0f, y = 0.0f;
	int buffwidthcount = 0, buffheightcount = 0;
	
	std::list<eBuffState> buffstate;
	BuffSort( buffstate );

#if CLIENT_VERSION == 1002
	if (this->AccountLevel > 0) {
		buffwidthcount = 1;
		x = m_Pos.x;
		y = m_Pos.y;
#if CLIENT_VERSION >= 1001
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DUEL_WINDOW)) {
			y += 35;
		}
#endif

		if (renderstate == BUFF_RENDER_ICON)
		{
			float Height = (float)((28.0f / 100) * 64.3f);
			float Width = (float)((20.0f / 100) * 64.3f);
			RenderImageScale(IMAGE_BUFF_VIP, x, y, Width, Height, 0.f, 0.f, 20.f, 28.f);
		}
		else if (renderstate == BUFF_RENDER_TOOLTIP)
		{
			// ���� ���� ������
			if (SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT)) {
				float fTooltip_x = x + (BUFF_IMG_WIDTH / 2);
				float fTooltip_y = y + BUFF_IMG_WIDTH;
				int TextNum = 0;
				::memset(TextList[0], 0, sizeof(char) * 30 * 100);
				::memset(TextListColor, 0, sizeof(int) * 30);
				::memset(TextBold, 0, sizeof(int) * 30);

			
					unicode::_sprintf(TextList[TextNum], "VIP");
					TextListColor[TextNum] = TEXT_COLOR_GREEN;
					TextBold[TextNum] = true;
					TextNum++;
					unicode::_sprintf(TextList[TextNum], "\n");
					TextListColor[TextNum] = TEXT_COLOR_BLUE;
					TextBold[TextNum] = true;
					TextNum++;
					for (int line = 0; line < 3; line++) {
						unicode::_sprintf(TextList[TextNum], GlobalText[3156+line]);
						TextListColor[TextNum] = TEXT_COLOR_BLUE;
						TextBold[TextNum] = false;
						TextNum++;
					}

					SIZE TextSize = { 0, 0 };
					g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize);
					RenderTipTextList(fTooltip_x, fTooltip_y, TextNum, 0);
					return;
			}
		}
	}
#endif

	std::list<eBuffState>::iterator iter;
	int stackindex = 0;
	for( iter = buffstate.begin(); iter != buffstate.end(); )
	{
		std::list<eBuffState>::iterator tempiter = iter;
		++iter;
		eBuffState buff = (*tempiter);
		
		if (g_BuffWindowStacked)
		{
			const int renderindex = min(stackindex, 3);
			x = m_Pos.x + (renderindex * 2.f);
			y = m_Pos.y + (renderindex * 2.f);
		}
		else
		{
			x = m_Pos.x + (buffwidthcount * (BUFF_IMG_WIDTH+BUFF_IMG_SPACE));
			y = m_Pos.y + (buffheightcount * (BUFF_IMG_HEIGHT+BUFF_IMG_SPACE)) ;
		}
#if CLIENT_VERSION >= 1001
		if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_DUEL_WINDOW)) {
			y += 35;
		}
#endif
		
		if( renderstate == BUFF_RENDER_ICON ) 
		{
			if (g_BuffWindowStacked == false || stackindex < 4)
			{
				RenderBuffIcon(buff, x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT);
			}
		}
		else if( renderstate == BUFF_RENDER_TOOLTIP )
		{
			// ���� ���� ������
			if(SEASON3B::CheckMouseIn(x, y, BUFF_IMG_WIDTH, BUFF_IMG_HEIGHT)) {
				float fTooltip_x = x + (BUFF_IMG_WIDTH / 2);
				float fTooltip_y = y + BUFF_IMG_WIDTH;
				eBuffClass buffclass = g_IsBuffClass(buff);
				RenderBuffTooltip( buffclass, buff, fTooltip_x, fTooltip_y );
			}
		}
		
		stackindex++;

		if (g_BuffWindowStacked == false)
		{
			if( ++buffwidthcount >= BUFF_MAX_LINE_COUNT ) {
				buffwidthcount = 0;
				++buffheightcount;
			}
		}
	}
}	

void SEASON3B::CNewUIBuffWindow::RenderBuffIcon(eBuffState& eBuffType, float x, float y, float width, float height)
{
	int customBuffBitmap = GetSkillShopActiveBuffBitmap(eBuffType);
	if (customBuffBitmap != 0)
	{
		RenderBitmap(customBuffBitmap, x, y, width, height, 0.f, 0.f, 1.f, 1.f);
		return;
	}

#if CLIENT_VERSION == 1002

	float Height = (float)((28.0f / 100) * 64.3f); // ((double)(unsigned int)*(QWORD*)0x0E61E5C / 480.0); //28
	float Width = (float)((20.0f / 100) * 64.3f); // ((double)(unsigned int)*(QWORD*)0xE61E58 / 640.0); //20
	float RenderW = (float)((256.0f / 100) * 64.3f); // ((double)(unsigned int)*(QWORD*)0xE61E58 / 640.0);

	if (eBuffType < 81) // eBuff_Berserker
	{
		int EffectValue = (eBuffType - 1) % 80;
		float SourceX = (double)(EffectValue % 10) * Width / RenderW;
		float SourceY = (double)(EffectValue / 10) * Height / RenderW;
		float SourceWidth = Width / RenderW;
		float SourceHeight = Height / RenderW;
		RenderBitmap((eBuffType - 1) / 80 + 31719, x, y, Width, Height, SourceX, SourceY, SourceWidth, SourceHeight, 1, 1, 0);
	}
	else {

		int EffectValue = (eBuffType - 81) % 81;
		float SourceX = (double)(EffectValue % 10) * Width / RenderW;
		float SourceY = (double)(EffectValue / 10) * Height / RenderW;
		float SourceWidth = Width / RenderW;
		float SourceHeight = Height / RenderW;

		RenderBitmap((eBuffType - 81) / 81 + 31720, x, y, Width, Height, SourceX, SourceY, SourceWidth, SourceHeight, 1, 1, 0);
	}
#else
	int iWidthIndex, iHeightIndex;
	float u, v;

	if (eBuffType < 81) // eBuff_Berserker
	{
		iWidthIndex = (eBuffType - 1) % 10;
		iHeightIndex = (eBuffType - 1) / 10;
		
		u = iWidthIndex * width / 256.f;
		v = iHeightIndex * height / 256.f;
		
		RenderBitmap(IMAGE_BUFF_STATUS, x, y, width, height, u, v, width/256.f, height/256.f);
	}
	else
	{
		// eBuff_Santa
		iWidthIndex = (eBuffType - 81) % 10; // eBuff_Berserker
		iHeightIndex = (eBuffType - 81) / 10; // eBuff_Berserker
		
		u = iWidthIndex * width / 256.f;
		v = iHeightIndex * height / 256.f;
		
		RenderBitmap(IMAGE_BUFF_STATUS2, x, y, width, height, u, v, width/256.f, height/256.f);
	}
#endif

}

void SEASON3B::CNewUIBuffWindow::RenderBuffTooltip(eBuffClass& eBuffClassType, eBuffState& eBuffType, float x, float y)
{
	int TextNum = 0;
	::memset(TextList[0], 0, sizeof(char) * 30 * 100);
	::memset(TextListColor, 0, sizeof(int) * 30);
	::memset(TextBold, 0, sizeof(int) * 30);
	
	std::list<std::string> tooltipinfo;
	g_BuffToolTipString( tooltipinfo, eBuffType );
	
	for(std::list<std::string>::iterator iter = tooltipinfo.begin(); iter != tooltipinfo.end(); ++iter )
	{
		std::string& temp = *iter;
		
		unicode::_sprintf(TextList[TextNum], temp.c_str());
		
		if( TextNum == 0 )
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
	g_BuffStringTime( eBuffType, bufftime );
	
	if( bufftime.size() != 0 )
	{
		unicode::_sprintf(TextList[TextNum], GlobalText[2533], bufftime.c_str());
		TextListColor[TextNum] = TEXT_COLOR_PURPLE;
		TextBold[TextNum] = false;
		TextNum += 1;
	}
	
	SIZE TextSize = {0, 0};	
	g_pMultiLanguage->_GetTextExtentPoint32( g_pRenderText->GetFontDC(), TextList[0], 1, &TextSize );
	RenderTipTextList( x,y,TextNum, 0 );
}

float SEASON3B::CNewUIBuffWindow::GetLayerDepth()	//. 5.3f
{
	return 0.95f;
}

void SEASON3B::CNewUIBuffWindow::OpenningProcess()
{

}

void SEASON3B::CNewUIBuffWindow::ClosingProcess()
{

}

#if CLIENT_VERSION == 1002
void SEASON3B::CNewUIBuffWindow::ReceiveAccountLevel(int accountLevel)
{
	this->AccountLevel = accountLevel;
}
#endif


void SEASON3B::CNewUIBuffWindow::LoadImages()
{
	LoadBitmap("Interface\\newui_statusicon.jpg", IMAGE_BUFF_STATUS, GL_LINEAR);
	LoadBitmap("Interface\\newui_statusicon2.jpg", IMAGE_BUFF_STATUS2, GL_LINEAR);
#if CLIENT_VERSION == 1002
	LoadBitmap("Interface\\VipBuff.jpg", IMAGE_BUFF_VIP, GL_LINEAR);
#endif
}

void SEASON3B::CNewUIBuffWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_BUFF_STATUS2);
	DeleteBitmap(IMAGE_BUFF_STATUS);	
}
