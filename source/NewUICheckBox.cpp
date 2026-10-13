#pragma once
#include "stdafx.h"
#include "NewUICheckBox.h"
#include "UIControls.h"
#include "GlobalBitmap.h"
#include "ZzzTexture.h"
#include "WideScreen.h"

SEASON3B::CNewUICheckBox::CNewUICheckBox()
{
	s_ImgIndex = -1;
	m_Pos.x = 0; m_Pos.y = 0;
	m_Size.x = 15; m_Size.y = 15;
	m_Name.clear();
	m_hTextFont = g_hFont;
	m_NameColor = 0xFFFFFFFF;
	m_NameBackColor = 0x00000000;
	m_ImgWidth = 0.0;
	m_ImgHeight = 15.f;
	State = 0;
}

SEASON3B::CNewUICheckBox::~CNewUICheckBox()
{

}

void SEASON3B::CNewUICheckBox::CheckBoxImgState(int imgindex)
{
	s_ImgIndex = imgindex;
}

void SEASON3B::CNewUICheckBox::RegisterBoxState(bool eventstate)
{
	State = eventstate;
}

void SEASON3B::CNewUICheckBox::ChangeText(unicode::t_string btname)
{
	m_Name = btname;
}

void SEASON3B::CNewUICheckBox::CheckBoxInfo(int x, int y, int sx, int sy)
{
	m_Pos.x = x; m_Pos.y = y;
	m_Size.x = sx; m_Size.y = sy;
}

bool SEASON3B::CNewUICheckBox::GetBoxState()
{
	return State;
}

void SEASON3B::CNewUICheckBox::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderImage(s_ImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0.0, (State) ? 0.0 : m_Size.y);

	if (State)
	{
		RenderImage(s_ImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0, 0);
	}
	else
	{
		RenderImage(s_ImgIndex, m_Pos.x, m_Pos.y, m_Size.x, m_Size.y, 0, m_Size.y);
	}

	g_pRenderText->SetFont(m_hTextFont);
	g_pRenderText->SetTextColor(m_NameColor);
	g_pRenderText->SetBgColor(m_NameBackColor);
	g_pRenderText->RenderText(m_Pos.x + m_Size.x + 1, m_Pos.y + 4, m_Name.c_str(), 0, 0);
}

bool SEASON3B::CNewUICheckBox::UpdateMouseEvent()
{
	if (CheckMouseIn(m_Pos.x, m_Pos.y, m_Size.x, m_Size.y))
	{
		if (IsRelease(VK_LBUTTON))
		{
			State = !State;
			return TRUE;
		}
	}
	return 0;
}