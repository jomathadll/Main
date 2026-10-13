#include "stdafx.h"
#include "CustomButtons.h"
#include "UIControls.h"
#include "GlobalBitmap.h"
#include "ZzzTexture.h"
#include "WideScreen.h"
#include "ZzzInterface.h"

namespace
{
	void PointSet(POINT& p, int x, int y)
	{
		p.x = x; p.y = y;
	}

	void RenderText(const char* text, int x, int y, int sx, int sy, HFONT hFont, DWORD color, DWORD backcolor, int sort)
	{
		g_pRenderText->SetFont(hFont);

		DWORD backuptextcolor = g_pRenderText->GetTextColor();
		DWORD backuptextbackcolor = g_pRenderText->GetBgColor();

		g_pRenderText->SetTextColor(color);
		g_pRenderText->SetBgColor(backcolor);
		g_pRenderText->RenderText(x, y, text, sx, sy, sort);

		g_pRenderText->SetTextColor(backuptextcolor);
		g_pRenderText->SetBgColor(backuptextbackcolor);
	}
};

using namespace SEASON3B;

//////////////////////////////////////////////////////////////////////
// CCustomButtons
//////////////////////////////////////////////////////////////////////
CCustomButtons::CCustomButtons() : CNewUIBaseButton(), m_CurImgIndex(0),
m_CurImgState(0), m_ImgWidth(0), m_ImgHeight(0),
m_NameColor(RGBA(245, 232, 202, 255)), m_NameBackColor(0x00000000),
m_CurImgColor(0xFFFFFFFF), m_TooltipTextColor(0xFFFFFFFF), m_IsTopPos(false),
#ifndef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE			// #ifndef
m_IsImgWidth(false),
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
m_fAlpha(1.0f)
{
	Initialize();
}

CCustomButtons::~CCustomButtons()
{
	Destroy();
}

void SEASON3B::CCustomButtons::Initialize()
{
	m_hTextFont = g_hFont;
	m_hToolTipFont = g_hFont;
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	m_iMoveTextPosX = 0;
	m_iMoveTextPosY = 0;
	m_bClickEffect = false;
	m_BtnSize = 0;
	m_BtnForceDisabled = false;
	m_BtnForceDown = false;
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
}

void SEASON3B::CCustomButtons::Destroy()
{
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	UnRegisterButtonState();
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
}

#ifdef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void SEASON3B::CCustomButtons::ChangeButtonImgState(bool imgregister, int imgindex, bool overflg /* = false */,
	bool bLockImage /* = false */, bool bClickEffect /* = false  */)
{
	m_bClickEffect = bClickEffect;

	if (imgregister)
	{
		RegisterButtonState(BUTTON_STATE_UP, imgindex, 0);

		if (overflg)
		{
			RegisterButtonState(BUTTON_STATE_OVER, imgindex, 1);
			RegisterButtonState(BUTTON_STATE_DOWN, imgindex, 2);
		}
		else
		{
			RegisterButtonState(BUTTON_STATE_OVER, imgindex, 0);
			RegisterButtonState(BUTTON_STATE_DOWN, imgindex, 1);
		}

		if (bLockImage)
		{
			RegisterButtonState(BUTTON_STATE_LOCK, imgindex, 3);
		}
		else
		{
			RegisterButtonState(BUTTON_STATE_LOCK, imgindex, 0);
		}

		ChangeImgIndex(imgindex, 0);
	}
}
#else // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
void SEASON3B::CCustomButtons::ChangeButtonImgState(bool imgregister, int imgindex, bool overflg, bool isimgwidth, bool bClickEffect)
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
void SEASON3B::CCustomButtons::ChangeButtonImgState(bool imgregister, int imgindex, bool overflg, bool isimgwidth)
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
{
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	m_bClickEffect = bClickEffect;
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

	if (imgregister)
	{
		if (overflg)
		{
			RegisterButtonState(BUTTON_STATE_UP, imgindex, 0);
			RegisterButtonState(BUTTON_STATE_OVER, imgindex, 1);
			RegisterButtonState(BUTTON_STATE_DOWN, imgindex, 2);
		}
		else
		{
			RegisterButtonState(BUTTON_STATE_UP, imgindex, 0);
			RegisterButtonState(BUTTON_STATE_DOWN, imgindex, 1);
		}

		ChangeImgIndex(imgindex, 0);
		ChangeImgWidth(isimgwidth);
	}
}
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE


void SEASON3B::CCustomButtons::ChangeButtonInfo(int x, int y, int sx, int sy)
{
	SetPos(x, y);
	SetSize(sx, sy);
}

int SEASON3B::CCustomButtons::GetButtonSize()
{
	return m_BtnSize;
}

void SEASON3B::CCustomButtons::SetButtonSize(int size)
{
	m_BtnSize = size;
}

void SEASON3B::CCustomButtons::ForceButtonDown(bool state)
{
	m_BtnForceDown = state;
}

void SEASON3B::CCustomButtons::ForceButtonDisabled(bool state)
{
	m_BtnForceDisabled = state;
}

void SEASON3B::CCustomButtons::RegisterButtonState(BUTTON_STATE eventstate, int imgindex, int btstate)
{
	ButtonInfo btinfo;
	btinfo.s_ImgIndex = imgindex;
	btinfo.s_BTstate = btstate;

	m_ButtonInfo.insert(std::make_pair(eventstate, btinfo));
}

void SEASON3B::CCustomButtons::UnRegisterButtonState()
{
	m_ButtonInfo.clear();
}

void SEASON3B::CCustomButtons::ChangeImgIndex(int imgindex, int curimgstate)
{
	m_CurImgIndex = imgindex;
	m_CurImgState = curimgstate;

	if (m_CurImgIndex != -1)
	{
		BITMAP_t* b = &Bitmaps[m_CurImgIndex];

		m_ImgWidth = b->Width;
		m_ImgHeight = b->Height;
	}
}

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
void SEASON3B::CCustomButtons::ChangeButtonState(BUTTON_STATE eventstate, int iButtonState)
{
	if (m_ButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_ButtonInfo.find(static_cast<int>(eventstate));

		if (iter != m_ButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;
			info.s_BTstate = iButtonState;
		}
	}
}

void SEASON3B::CCustomButtons::MoveTextPos(int iX, int iY)
{
	m_iMoveTextPosX = iX;
	m_iMoveTextPosY = iY;
}
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

void CCustomButtons::ChangeAlpha(unsigned char fAlpha, bool isfontalph)
{
	m_CurImgColor &= ~(0xff << 24);
	m_CurImgColor |= (fAlpha << 24);

	if (isfontalph)
	{
		m_NameColor &= ~(0xff << 24);
		m_NameColor |= (fAlpha << 24);
	}
}

void CCustomButtons::ChangeAlpha(float fAlpha, bool isfontalph)
{
	m_CurImgColor &= ~(0xff << 24);
	m_CurImgColor |= (static_cast<unsigned char>((float)(0xff) * fAlpha) << 24);

	if (isfontalph)
	{
		m_NameColor &= ~(0xff << 24);
		m_NameColor |= (static_cast<unsigned char>((float)(0xff) * fAlpha) << 24);
	}
}

void SEASON3B::CCustomButtons::ChangeImgColor(BUTTON_STATE eventstate, unsigned int color)
{
	if (m_ButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_ButtonInfo.find(static_cast<int>(eventstate));

		if (iter != m_ButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;
			info.s_imgColor = color;
			if (GetBTState() == eventstate)
			{
				m_CurImgColor = color;
			}
		}
	}
}

void SEASON3B::CCustomButtons::ChangeFrame()
{
	if (m_ButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_ButtonInfo.find(static_cast<int>(GetBTState()));

		if (iter != m_ButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;

			ChangeImgIndex(info.s_ImgIndex, info.s_BTstate);

			m_CurImgColor = info.s_imgColor;
		}
	}
}

bool SEASON3B::CCustomButtons::UpdateMouseEvent()
{
	if (IsLock())
	{
		return false;
	}

	BUTTON_STATE backevent = GetBTState();

	bool result = Process();

	if (backevent != GetBTState())
	{
		ChangeFrame();
	}

	return result;
}

#ifdef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void SEASON3B::CCustomButtons::Lock()
{
	CNewUIBaseButton::Lock();
	ChangeFrame();
}

void SEASON3B::CCustomButtons::UnLock()
{
	CNewUIBaseButton::UnLock();
	ChangeFrame();
}
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE


bool SEASON3B::CCustomButtons::Render(bool RendOption)
{
	if (m_ButtonInfo.size() != 0)
	{
		switch (m_BtnSize) {
		case 0:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.429, 0.83, 0.20, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.213, 0.83, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.002, 0.83, 0.20, 1, 1, 0.5);
			}
			break;
		case 1:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.95, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.429, 0.95, 0.20, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.213, 0.95, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.95, 0.20, 1, 1, 0.5);
			}
			break;
		case 2:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.775, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.429, 0.775, 0.2, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.213, 0.775, 0.2, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.775, 0.2, 1, 1, 0.5);
			}
			break;
		case 3:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.0, 0.812731823, 0.2023746, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.4247492, 0.812731823, 0.2023746, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.2123746, 0.812731823, 0.2023746, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.0, 0.812731823, 0.2023746, 1, 1, 0.5);
			}
			break;
		case 4:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.001, 0.575, 0.21, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.419, 0.575, 0.21, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.21, 0.575, 0.21, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.001, 0.575, 0.21, 1, 1, 0.3);
			}
			break;
		}
	}

	if (m_Name.size() != 0)
	{
		SIZE Fontsize;
		g_pRenderText->SetFont(m_hTextFont);
		g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), m_Name.c_str(), m_Name.size(), &Fontsize);

		Fontsize.cx = Fontsize.cx / ((float)WindowWidth / 640);
		Fontsize.cy = Fontsize.cy / ((float)WindowHeight / 480);

		int x = m_Pos.x + ((m_Size.x / 2));// -(Fontsize.cx / 2));
		int y = m_Pos.y + ((m_Size.y / 2) - (Fontsize.cy / 2));

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		if ((m_bClickEffect == true) && (GetBTState() == BUTTON_STATE_DOWN))
		{
			RenderText(m_Name.c_str(), x + m_iMoveTextPosX + 1, y + m_iMoveTextPosY + 1, m_Size.x, 0, m_hTextFont, m_NameColor, m_NameBackColor, RT3_WRITE_CENTER);
		}
		else
		{
			RenderText(m_Name.c_str(), x + m_iMoveTextPosX, y + m_iMoveTextPosY, m_Size.x, 0, m_hTextFont, m_NameColor, m_NameBackColor, RT3_WRITE_CENTER);
		}
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
		RenderText(m_Name.c_str(), x, y, m_Size.x, 0, m_hTextFont, m_NameColor, m_NameBackColor, RT3_SORT_LEFT);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
	}

	if (m_TooltipText.size() != 0)
	{
		if (CheckMouseIn(m_Pos.x, m_Pos.y, m_Size.x, m_Size.y))
		{
			//SIZE Fontsize;
			//g_pRenderText->SetFont(m_hToolTipFont);
			//g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), m_TooltipText.c_str(), m_TooltipText.size(), &Fontsize);
			//
			//Fontsize.cx = Fontsize.cx;// / ((float)WindowWidth / gWideScreen.g_WideWindowWidth);
			//Fontsize.cy = Fontsize.cy / ((float)WindowHeight / 480);
			//
			//int x = m_Pos.x + ((m_Size.x / 2) - (Fontsize.cx / 2));
			//int y = m_Pos.y + m_Size.y + 2;
			//
			//int _iTempWidth = x + Fontsize.cx + 6;
			//x = (_iTempWidth > gWideScreen.g_WideWindowWidth) ? (x - (_iTempWidth - gWideScreen.g_WideWindowWidth)) : x;
			//
			//if (m_IsTopPos) y = m_Pos.y - (Fontsize.cy + 2);
			//
			//RenderText(m_TooltipText.c_str(), x, y, Fontsize.cx + 6, 0, m_hToolTipFont, m_TooltipTextColor, RGBA(0, 0, 0, 180), RT3_SORT_CENTER);

			RenderTipText(m_Pos.x, m_Pos.y-12, m_TooltipText.c_str());
		}
	}

	return true;
}

CVoidMuRadioButton::CVoidMuRadioButton() : m_NameColor(RGB(245, 232, 202)), m_NameBackColor(0x00000000),
m_CurImgIndex(0), m_CurImgState(0), m_ImgWidth(0), m_ImgHeight(0), m_CurImgColor(0xffffffff)
{
	Initialize();
}

CVoidMuRadioButton::~CVoidMuRadioButton()
{
	Destroy();
}

void CVoidMuRadioButton::Initialize()
{
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	m_hTextFont = g_hFontBold;
	m_bClickEffect = false;
	m_BtnForceDisabled = false;
	m_BtnForceDown = false;
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
}

void CVoidMuRadioButton::Destroy()
{
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	UnRegisterButtonState();
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
}

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#ifdef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void CNewUIRadioButton::ChangeRadioButtonImgState(int imgindex, bool bMouseOnImage, bool bLockImage, bool bClickEffect)
{
	int btState = 0;

	m_bClickEffect = bClickEffect;
	m_bLockImage = bLockImage;

	RegisterButtonState(BUTTON_STATE_UP, imgindex, btState++);

	if (bMouseOnImage == true)
	{
		RegisterButtonState(BUTTON_STATE_OVER, imgindex, btState++);
	}

	RegisterButtonState(BUTTON_STATE_DOWN, imgindex, btState++);

	if (bLockImage == true)
	{
		RegisterButtonState(BUTTON_STATE_LOCK, imgindex, btState++);
	}
}
#else // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void CVoidMuRadioButton::ChangeRadioButtonImgState(int imgindex, bool isDown, bool bClickEffect)
{
	m_bClickEffect = bClickEffect;
	RegisterButtonState(BUTTON_STATE_UP, imgindex, 0);
	RegisterButtonState(BUTTON_STATE_DOWN, imgindex, 1);

	if (isDown)
	{
		ChangeFrame(BUTTON_STATE_DOWN);
	}
}
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
void CVoidMuRadioButton::ChangeRadioButtonImgState(int imgindex, bool isDown)
{
	RegisterButtonState(BUTTON_STATE_UP, imgindex, 0);
	RegisterButtonState(BUTTON_STATE_DOWN, imgindex, 1);

	if (isDown)
	{
		ChangeFrame(BUTTON_STATE_DOWN);
	}
}
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

int CVoidMuRadioButton::GetButtonSize()
{
	return m_BtnSize;
}

void CVoidMuRadioButton::SetButtonSize(int size)
{
	m_BtnSize = size;
}

void CVoidMuRadioButton::ChangeRadioButtonInfo(int x, int y, int sx, int sy)
{
	SetPos(x, y);
	SetSize(sx, sy);
}

void CVoidMuRadioButton::RegisterButtonState(BUTTON_STATE eventstate, int imgindex, int btstate)
{
	ButtonInfo btinfo;
	btinfo.s_ImgIndex = imgindex;
	btinfo.s_BTstate = btstate;

	m_RadioButtonInfo.insert(std::make_pair(eventstate, btinfo));
}

void CVoidMuRadioButton::UnRegisterButtonState()
{
	m_RadioButtonInfo.clear();
}

void CVoidMuRadioButton::ChangeImgColor(BUTTON_STATE eventstate, unsigned int color)
{
	if (m_RadioButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_RadioButtonInfo.find(static_cast<int>(eventstate));

		if (iter != m_RadioButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;
			info.s_imgColor = color;
			if (GetBTState() == eventstate)
			{
				m_CurImgColor = color;
			}
		}
	}
}

void CVoidMuRadioButton::ChangeImgIndex(int imgindex, int curimgstate)
{
	m_CurImgIndex = imgindex;
	m_CurImgState = curimgstate;

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	if (m_CurImgIndex != -1 && m_CurImgIndex != BITMAP_UNKNOWN)
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
	if (m_CurImgIndex != -1)
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
	{
		BITMAP_t* b = &Bitmaps[m_CurImgIndex];

		m_ImgWidth = b->Width;
		m_ImgHeight = b->Height;
	}
}

void CVoidMuRadioButton::ChangeFrame(BUTTON_STATE eventstate)
{
	m_EventState = eventstate;

	if (m_RadioButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_RadioButtonInfo.find(static_cast<int>(GetBTState()));

		if (iter != m_RadioButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;

			ChangeImgIndex(info.s_ImgIndex, info.s_BTstate);

			m_CurImgColor = info.s_imgColor;
		}
	}

	if (m_Name.size() != 0)
	{
		if (GetBTState() == BUTTON_STATE_UP)
			ChangeTextColor(RGBA(245, 232, 202, 155));
		else
			ChangeTextColor(RGBA(245, 232, 202, 255));
	}
}

void CVoidMuRadioButton::ChangeFrame()
{
	if (m_RadioButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_RadioButtonInfo.find(static_cast<int>(GetBTState()));

		if (iter != m_RadioButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;

			ChangeImgIndex(info.s_ImgIndex, info.s_BTstate);

			m_CurImgColor = info.s_imgColor;
		}
	}

	if (m_Name.size() != 0)
	{
		if (GetBTState() == BUTTON_STATE_UP)
			ChangeTextColor(0xffB5B5B5);
		else
			ChangeTextColor(0xffFFFFFF);
	}
}

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
void CVoidMuRadioButton::ChangeButtonState(int iImgIndex, BUTTON_STATE eventstate, int iButtonState)
{
	if (m_RadioButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_RadioButtonInfo.find(static_cast<int>(eventstate));

		if (iter != m_RadioButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;
			info.s_ImgIndex = iImgIndex;
			info.s_BTstate = iButtonState;
		}
	}
}

void CVoidMuRadioButton::ChangeButtonState(BUTTON_STATE eventstate, int iButtonState)
{
	if (m_RadioButtonInfo.size() != 0)
	{
		ButtonStateMap::iterator iter = m_RadioButtonInfo.find(static_cast<int>(eventstate));

		if (iter != m_RadioButtonInfo.end())
		{
			ButtonInfo& info = (*iter).second;
			info.s_BTstate = iButtonState;
		}
	}
}
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

bool CVoidMuRadioButton::UpdateMouseEvent(bool isGroupevent)
{
	if (IsLock())
	{
		return false;
	}

	BUTTON_STATE backevent = GetBTState();

	bool result = false;

	if (isGroupevent)
	{
		if (GetBTState() != BUTTON_STATE_DOWN)
		{
			result = RadioProcess();
			if (backevent != GetBTState())
			{
				ChangeFrame();
			}
		}
	}
	else
	{
		result = RadioProcess();
		if (backevent != GetBTState())
		{
			ChangeFrame();
		}
	}

	return result;
}

bool CVoidMuRadioButton::Render()
{
	if (m_RadioButtonInfo.size() != 0)
	{
		switch (m_BtnSize) {
		case 0:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.429, 0.83, 0.20, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.213, 0.83, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.008, 0.002, 0.83, 0.20, 1, 1, 0.5);
			}
			break;
		case 1:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.95, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.429, 0.95, 0.20, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.213, 0.95, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.95, 0.20, 1, 1, 0.5);
			}
			break;
		case 2:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.775, 0.20, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.429, 0.775, 0.2, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.213, 0.775, 0.2, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.002, 0.775, 0.2, 1, 1, 0.5);
			}
			break;
		case 3:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.0, 0.812731823, 0.2023746, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.4247492, 0.812731823, 0.2023746, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.2123746, 0.812731823, 0.2023746, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.018, 0.0, 0.812731823, 0.2023746, 1, 1, 0.5);
			}
			break;
		case 4:
			if (GetBTState() == BUTTON_STATE_UP && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.001, 0.575, 0.21, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DOWN || m_BtnForceDown) && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.419, 0.575, 0.21, 1, 1, 1.0);
			}
			else if (GetBTState() == BUTTON_STATE_OVER && !m_BtnForceDown && !m_BtnForceDisabled) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.21, 0.575, 0.21, 1, 1, 1.0);
			}
			else if ((GetBTState() == BUTTON_STATE_DISABLED || m_BtnForceDisabled) && !m_BtnForceDown) {
				RenderBitmap(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.002, 0.001, 0.575, 0.21, 1, 1, 0.3);
			}
			break;
		case 5:
			if (m_CurImgIndex != BITMAP_UNKNOWN) {
				RenderImageScale(m_CurImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y+0.8f, 0.f, 0.f, 147.f, 32.f);
			}
			break;
		}
	}

	if (m_Name.size() != 0)
	{
		SIZE Fontsize;

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		g_pRenderText->SetFont(m_hTextFont);
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
		g_pRenderText->SetFont(g_hFont);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

		g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), m_Name.c_str(), m_Name.size(), &Fontsize);

		Fontsize.cx = Fontsize.cx / ((float)WindowWidth / 640);
		Fontsize.cy = Fontsize.cy / ((float)WindowHeight / 480);

		int x = m_Pos.x + ((m_Size.x / 2));// -(Fontsize.cx / 2));
		int y = m_Pos.y + ((m_Size.y / 2) - (Fontsize.cy / 2));

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		if ((m_bClickEffect == true) && GetBTState() == BUTTON_STATE_DOWN)
		{
			RenderText(m_Name.c_str(), x + 1, y + 1, m_Size.x, 0, m_hTextFont, m_NameColor, m_NameBackColor, 8);
		}
		else
		{
			RenderText(m_Name.c_str(), x, y, m_Size.x, 0, m_hTextFont, m_NameColor, m_NameBackColor, 8);
		}
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
		RenderText(m_Name.c_str(), x, y, m_Size.x, 0, g_hFont, m_NameColor, m_NameBackColor, 8);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// CVoidMuRadioGroupButton
//////////////////////////////////////////////////////////////////////

CVoidMuRadioGroupButton::CVoidMuRadioGroupButton()
{
	Initialize();
}

CVoidMuRadioGroupButton::~CVoidMuRadioGroupButton()
{
	Destroy();
}

void CVoidMuRadioGroupButton::Initialize()
{
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	m_iButtonDistance = 1;
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
}

void CVoidMuRadioGroupButton::Destroy()
{
	UnRegisterRadioButton();
}

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
#ifdef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void CVoidMuRadioGroupButton::CreateRadioGroup(int radiocount, int imgindex, bool bFirstIndexBtnDown /* = true */,
	bool bMouseOnImage /* = false */, bool bLockImage, bool bClickEffect /* = false  */)
{
	for (int i = 0; i < radiocount; ++i)
	{
		CVoidMuRadioButton* button = new CVoidMuRadioButton();

		button->ChangeRadioButtonImgState(imgindex, bMouseOnImage, bLockImage, bClickEffect);
		button->ChangeRadioButtonInfo(0, 0, 0, 0);
		RegisterRadioButton(button);
	}

	int iCurIndex = -1;

	if (bFirstIndexBtnDown == true)
	{
		iCurIndex = 0;
	}

	ChangeFrame(iCurIndex);
	SetCurButtonIndex(iCurIndex);
}
#else // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void CVoidMuRadioGroupButton::CreateRadioGroup(int radiocount, int imgindex, bool bClickEffect)
{
	for (int i = 0; i < radiocount; ++i)
	{
		CVoidMuRadioButton* button = new CVoidMuRadioButton();

		button->ChangeRadioButtonImgState(imgindex, ((i == 0) ? true : false), bClickEffect);
		button->ChangeRadioButtonInfo(0, 0, 0, 0);
#if CLIENT_VERSION >= 1001
		button->ChangeTextColor(RGBA(245, 232, 202, 255));
#endif
		RegisterRadioButton(button);
	}

	SetCurButtonIndex(0);
}
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
void CVoidMuRadioGroupButton::CreateRadioGroup(int radiocount, int imgindex)
{
	for (int i = 0; i < radiocount; ++i)
	{
		CVoidMuRadioButton* button = new CVoidMuRadioButton();
		button->ChangeRadioButtonImgState(imgindex, ((i == 0) ? true : false));
		button->ChangeRadioButtonInfo(0, 0, 0, 0);
		RegisterRadioButton(button);
	}

	SetCurButtonIndex(0);
}
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
void CVoidMuRadioGroupButton::ChangeRadioButtonInfo(bool iswidth, int x, int y, int sx, int sy, int iDistance/* = 1*/)
{
	int i = 0;

	m_iButtonDistance = iDistance;

	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (button)
		{
			if (iswidth) button->SetPos(x + ((sx + m_iButtonDistance) * i), y);
			else button->SetPos(x, y + ((sy + m_iButtonDistance) * i));
			button->SetSize(sx, sy);
		}

		++i;
	}
}

void CVoidMuRadioGroupButton::ChangeButtonState(BUTTON_STATE eventstate, int iButtonState)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		button->ChangeButtonState(eventstate, iButtonState);

		++i;
	}
}

void CVoidMuRadioGroupButton::ChangeButtonState(int iBtnIndex, int iImgIndex, BUTTON_STATE eventstate, int iButtonState)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (i == iBtnIndex)
		{
			button->ChangeButtonState(iImgIndex, eventstate, iButtonState);
			return;
		}

		++i;
	}
}

void CVoidMuRadioGroupButton::SetButtonSize(int size)
{
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		button->SetButtonSize(size);
	}
}

#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
void CVoidMuRadioGroupButton::ChangeRadioButtonInfo(bool iswidth, int x, int y, int sx, int sy)
{
	int i = 0;

	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (button)
		{
			if (iswidth) button->SetPos(x + ((sx + 1) * i), y);
			else button->SetPos(x, y + ((sy + 1) * i));
			button->SetSize(sx, sy);
		}

		++i;
	}
}
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

void CVoidMuRadioGroupButton::ChangeRadioText(std::list<unicode::t_string>& textlist)
{
	std::list<unicode::t_string>::iterator textiter = textlist.begin();

	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		std::list<unicode::t_string>::iterator curtextiter = textiter;
		++textiter;
		unicode::t_string text = (*curtextiter);

		button->ChangeText(text);

		if (textiter == textlist.end()) break;
	}
}

void CVoidMuRadioGroupButton::ChangeFrame(int buttonIndex)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (i != buttonIndex)
		{
			button->ChangeFrame(BUTTON_STATE_UP);
		}
		else
		{
			SetCurButtonIndex(i);
			button->ChangeFrame(BUTTON_STATE_DOWN);
		}
		++i;
	}
}

// Ãß°¡ : Pruarin(07.09.03)
void CVoidMuRadioGroupButton::LockButtonindex(int buttonIndex)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (i == buttonIndex)
		{
			button->Lock();
			break;
		}

		++i;
	}
}

#ifdef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
void CVoidMuRadioGroupButton::UnLockButtonIndex(int buttonIndex)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (i == buttonIndex)
		{
			button->UnLock();
			break;
		}

		++i;
	}
}
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE

void CVoidMuRadioGroupButton::RegisterRadioButton(CVoidMuRadioButton* button)
{
	m_RadioList.push_back(button);
}

void CVoidMuRadioGroupButton::UnRegisterRadioButton()
{
	for (RadioButtonList::reverse_iterator iter = m_RadioList.rbegin(); iter != m_RadioList.rend(); )
	{
		RadioButtonList::reverse_iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		SAFE_DELETE(button);
	}

	m_RadioList.clear();
}

int CVoidMuRadioGroupButton::UpdateMouseEvent()
{
	int i = 0;

	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (button->UpdateMouseEvent(true))
		{
			ChangeFrame(i);
			return GetCurButtonIndex();
		}

		++i;
	}

	return static_cast<int>(RADIOGROUPEVENT_NONE);
}

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
void CVoidMuRadioGroupButton::SetFont(HFONT hFont)
{
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		button->SetFont(hFont);
	}
}

void CVoidMuRadioGroupButton::SetFont(HFONT hFont, int iButtonIndex)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (i == iButtonIndex)
		{
			button->SetFont(hFont);
			break;
		}

		++i;
	}
}

POINT CVoidMuRadioGroupButton::GetPos(int iButtonIndex)
{
	int i = 0;
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		if (i == iButtonIndex)
		{
			return button->GetPos();
		}
		++i;
	}

	POINT Pos;
	Pos.x = 0;
	Pos.y = 0;

	return Pos;
}

#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

bool CVoidMuRadioGroupButton::Render()
{
	for (RadioButtonList::iterator iter = m_RadioList.begin(); iter != m_RadioList.end(); )
	{
		RadioButtonList::iterator curiter = iter;
		++iter;
		CVoidMuRadioButton* button = (*curiter);

		button->Render();
	}

	return true;
}


