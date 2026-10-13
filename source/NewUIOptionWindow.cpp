// NewUIOptionWindow.cpp: implementation of the CNewUIOptionWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIOptionWindow.h"
#include "NewUISystem.h"
#include "ZzzTexture.h"
#include "DSPlaySound.h"
#include "ZzzOpenData.h"
#include "MoveWindow.h"
#include "ZzzInfomation.h"
#include "MoveCommandData.h"
#include "CSQuest.h"
#include "SocketSystem.h"
#include "CSItemOption.h"
#include "wsclientinline.h"
#include "./ExternalObject/leaf/regkey.h"

using namespace SEASON3B;

namespace
{
	bool IsSupportedClientLanguage(const char* lang)
	{
		return lang != NULL
			&& (_stricmp(lang, "Eng") == 0
				|| _stricmp(lang, "Por") == 0
				|| _stricmp(lang, "Spn") == 0);
	}

	void SaveClientLanguageSelection(const char* lang)
	{
		if (lang == NULL)
		{
			return;
		}

		char szRegisterPath[256] = { 0 };
		wsprintf(szRegisterPath, "SOFTWARE\\%s\\Config", gProtect->m_MainInfo.m_registerPath);

		leaf::CRegKey regkey;
		regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, szRegisterPath);
		regkey.WriteString("LangSelection", lang);
	}
	void ReloadLocalizedClientData(const char* lang)
	{
		char Text[256] = { 0 };

		sprintf(Text, "Data\\Local\\%s\\Dialog.bmd", lang);
		OpenDialogFile(Text);

		sprintf(Text, "Data\\Local\\%s\\Item.bmd", lang);
		OpenItemScript(Text);

		sprintf(Text, "Data\\Local\\%s\\movereq.bmd", lang);
		SEASON3B::CMoveCommandData::OpenMoveReqScript(Text);

		sprintf(Text, "Data\\Local\\%s\\NpcName.txt", lang);
		OpenMonsterScript(Text);

		sprintf(Text, "Data\\Local\\%s\\Quest.bmd", lang);
		g_csQuest.OpenQuestScript(Text);

		sprintf(Text, "Data\\Local\\%s\\Skill.bmd", lang);
		OpenSkillScript(Text);

		sprintf(Text, "Data\\Local\\%s\\SocketItem.bmd", lang);
		g_SocketItemMgr.OpenSocketItemScript(Text);

		GlobalText.RemoveAll();
		OpenTextData();

		g_csItemOption.OpenItemSetScript(false);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SEASON3B::CNewUIOptionWindow::CNewUIOptionWindow()
{
	m_pNewUIMng = NULL;
	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bAutoAttack = true;
	m_bWhisperSound = false;
	m_bSlideHelp = true;
	m_iVolumeLevel = 0;
	m_iRenderLevel = 4;

	m_EffectNormal = false;
	m_EffectSprite = false;
	m_EffectParticle = false;
	m_Terrain = false;
	m_Wings = false;
	m_Swords = false;
	m_Equipments = false;
	m_Window = false;
}

SEASON3B::CNewUIOptionWindow::~CNewUIOptionWindow()
{
	Release();
}

bool SEASON3B::CNewUIOptionWindow::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if( NULL == pNewUIMng )
		return false;
	
	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_OPTION, this);
	m_MoveWindow.Create(x, y, OPTION_WIDTH, OPTION_HEIGHT);
	SetPos(x, y);
	LoadImages();
	SetButtonInfo();
	Show(false);
	return true;
}

void SEASON3B::CNewUIOptionWindow::SetButtonInfo()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_BtnClose.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnClose.ChangeButtonImgState(true, IMAGE_OPTION_BTN_CLOSE, true);
	m_BtnClose.ChangeButtonInfo(m_Pos.x + 68, m_Pos.y + 209, 54, 30);
	m_BtnClose.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnClose.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
#elif CLIENT_VERSION >= 1001
	m_BtnClose.SetButtonSize(1);
	m_BtnClose.ChangeButtonImgState(true, IMAGE_OPTION_BTN_CLOSE, true);
	m_BtnClose.SetFont(g_hFontBold);
	m_BtnClose.ChangeTextBackColor(RGBA(255, 255, 255, 0));
	m_BtnClose.ChangeTextColor(RGBA(245, 232, 202, 255));
	m_BtnClose.ChangeButtonInfo(m_Pos.x + OPTION_WIDTH - 21, m_Pos.y + 3, 18, 18);
	m_BtnClose.ChangeImgColor(BUTTON_STATE_UP, RGBA(255, 255, 255, 255));
	m_BtnClose.ChangeImgColor(BUTTON_STATE_DOWN, RGBA(255, 255, 255, 255));
#endif
}

void SEASON3B::CNewUIOptionWindow::Release()
{
	UnloadImages();
	
	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj( this );
		m_pNewUIMng = NULL;
	}
}


void SEASON3B::CNewUIOptionWindow::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	if (m_pNewUIMng != NULL)
	{
		SetButtonInfo();
	}
}

bool SEASON3B::CNewUIOptionWindow::UpdateMouseEvent()
{
	if (m_MoveWindow.UpdatePosition(m_Pos.x, m_Pos.y))
	{
		SetPos(m_MoveWindow.m_PosTemp.x, m_MoveWindow.m_PosTemp.y);


		return false;
	}

#if CLIENT_VERSION >= 1001
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + OPTION_WIDTH - 21, m_Pos.y + 3, 18, 18))
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
		return false;
	}
#else
	if(m_BtnClose.UpdateMouseEvent() == true)
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
		return false;
	}
#endif

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 220, m_Pos.y + 182, 15, 15))
	{

		m_Window = !m_Window;

		LONG windowStyle = GetWindowLong(g_hWnd, GWL_STYLE);

		RECT rcWindow;
		GetWindowRect(g_hWnd, &rcWindow);

		RECT rcClient;
		GetClientRect(g_hWnd, &rcClient);

		if (m_Window)
		{
			windowStyle &= ~WS_CAPTION;
			windowStyle &= ~WS_THICKFRAME;

			AdjustWindowRect(&rcClient, WS_POPUP | WS_CLIPCHILDREN, FALSE);

			SetWindowPos(g_hWnd, NULL, rcWindow.left, rcWindow.top,
				rcClient.right - rcClient.left,
				rcClient.bottom - rcClient.top,
				SWP_NOZORDER | SWP_FRAMECHANGED);
		}
		else
		{
			windowStyle |= WS_CAPTION;
			windowStyle &= ~WS_THICKFRAME;

			AdjustWindowRect(&rcClient, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, FALSE);

			SetWindowPos(g_hWnd, NULL, rcWindow.left, rcWindow.top,
				rcClient.right - rcClient.left,
				rcClient.bottom - rcClient.top,
				SWP_NOZORDER | SWP_FRAMECHANGED);
		}

		SetWindowLong(g_hWnd, GWL_STYLE, windowStyle);

		SetWindowPos(g_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
	}


#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 43, 15, 15))
	{
		m_bAutoAttack = !m_bAutoAttack;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 65, 15, 15))
	{
		m_bWhisperSound = !m_bWhisperSound;
	}
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(m_Pos.x + 150, m_Pos.y + 127, 15, 15))
	{
		m_bSlideHelp = !m_bSlideHelp;
	}

	if (CheckMouseIn(m_Pos.x + 33 - 8, m_Pos.y + 104, 124 + 8, 16))
	{
		int iOldValue = m_iVolumeLevel;
		if (MouseWheel > 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel++;
			if (m_iVolumeLevel > 10)
			{
				m_iVolumeLevel = 10;
			}
		}
		else if (MouseWheel < 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel--;
			if (m_iVolumeLevel < 0)
			{
				m_iVolumeLevel = 0;
			}
		}
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 33);
			if (x < 0)
			{
				m_iVolumeLevel = 0;
}
			else
			{
				float fValue = (10.f * x) / 124.f;
				m_iVolumeLevel = (int)fValue + 1;
			}
		}

		if (iOldValue != m_iVolumeLevel)
		{
			SetEffectVolumeLevel(m_iVolumeLevel);
		}
	}
	if (CheckMouseIn(m_Pos.x + 25, m_Pos.y + 168, 141, 29))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 25);
			float fValue = (5.f * x) / 141.f;
			m_iRenderLevel = (int)fValue;
		}
	}

	if (CheckMouseIn(m_Pos.x, m_Pos.y, 190, 249) == true)
	{
		return false;
	}

#elif CLIENT_VERSION >= 1001
	float x, y;

	x = m_Pos.x + 30.f + 190.f;
	y = m_Pos.y + 41.f;

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_EffectNormal = !m_EffectNormal;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_EffectParticle = !m_EffectParticle;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_EffectSprite = !m_EffectSprite;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_Wings = !m_Wings;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_Swords = !m_Swords;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_Equipments = !m_Equipments;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_Terrain = !m_Terrain;
	}

	x = m_Pos.x + 30.f;
	y = m_Pos.y + 41.f;

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_bAutoAttack = !m_bAutoAttack;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_bWhisperSound = !m_bWhisperSound;
	}
	y += 20;
	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 15, 15))
	{
		m_bSlideHelp = !m_bSlideHelp;
	}
	y += 37;

	if (CheckMouseIn(m_Pos.x + 33 - 8, y, 124 + 8, 16))
	{
		int iOldValue = m_iVolumeLevel;
		if (MouseWheel > 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel++;
			if (m_iVolumeLevel > 10)
			{
				m_iVolumeLevel = 10;
			}
		}
		else if (MouseWheel < 0)
		{
			MouseWheel = 0;
			m_iVolumeLevel--;
			if (m_iVolumeLevel < 0)
			{
				m_iVolumeLevel = 0;
			}
		}
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 33);
			if (x < 0)
			{
				m_iVolumeLevel = 0;
			}
			else
			{
				float fValue = (10.f * x) / 124.f;
				m_iVolumeLevel = (int)fValue + 1;
			}
		}

		if (iOldValue != m_iVolumeLevel)
		{
			SetEffectVolumeLevel(m_iVolumeLevel);
		}
	}

	y += 35;

	if (CheckMouseIn(m_Pos.x + 25, y, 141, 29))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int x = MouseX - (m_Pos.x + 25);
			float fValue = (5.f * x) / 141.f;
			m_iRenderLevel = (int)fValue;
		}
	}


	y = m_Pos.y + 218;
	
	x = m_Pos.x + 27;

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 42, 27))
	{
		this->SetLanguage("Eng", 0);
	}

	x += 47;

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 42, 27))
	{
		this->SetLanguage("Por", 1);
	}

	x += 47;

	if (SEASON3B::IsPress(VK_LBUTTON) && CheckMouseIn(x, y, 42, 27))
	{
		this->SetLanguage("Spn", 2);
	}


	if (CheckMouseIn(m_Pos.x, m_Pos.y, OPTION_WIDTH, OPTION_HEIGHT+10) == true)
	{
		return false;
	}
#endif

	return true;
}

bool SEASON3B::CNewUIOptionWindow::UpdateKeyEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_OPTION) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_OPTION);
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIOptionWindow::Update()
{
	return true;
}

bool SEASON3B::CNewUIOptionWindow::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderFrame();
	RenderContents();
	RenderButtons();
	DisableAlphaBlend();
	return true;
}

float SEASON3B::CNewUIOptionWindow::GetLayerDepth()	//. 10.5f
{
	return 10.5f;
}

float SEASON3B::CNewUIOptionWindow::GetKeyEventOrder()	// 10.f;
{
	return 10.0f;
}

void SEASON3B::CNewUIOptionWindow::OpenningProcess()
{

}

void SEASON3B::CNewUIOptionWindow::ClosingProcess()
{

}

void SEASON3B::CNewUIOptionWindow::LoadImages()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_button_close.tga", IMAGE_OPTION_BTN_CLOSE, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\btn_medium.jpg", IMAGE_OPTION_BTN_CLOSE, GL_LINEAR);
#endif
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_OPTION_FRAME_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_OPTION_FRAME_DOWN, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_top.tga", IMAGE_OPTION_FRAME_UP, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_back06(L).tga", IMAGE_OPTION_FRAME_LEFT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_back06(R).tga", IMAGE_OPTION_FRAME_RIGHT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_line.jpg", IMAGE_OPTION_LINE, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_point.tga", IMAGE_OPTION_POINT, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_check.tga", IMAGE_OPTION_BTN_CHECK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_effect03.tga", IMAGE_OPTION_EFFECT_BACK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_effect04.tga", IMAGE_OPTION_EFFECT_COLOR, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_volume01.tga", IMAGE_OPTION_VOLUME_BACK, GL_LINEAR);	
	LoadBitmap("Interface\\newui_option_volume02.tga", IMAGE_OPTION_VOLUME_COLOR, GL_LINEAR);
	LoadBitmap("Custom\\Interface\\eng.tga", IMAGE_OPTION_LANG_ENG, GL_LINEAR);
	LoadBitmap("Custom\\Interface\\por.tga", IMAGE_OPTION_LANG_POR, GL_LINEAR);
	LoadBitmap("Custom\\Interface\\spn.tga", IMAGE_OPTION_LANG_SPN, GL_LINEAR);
}

void SEASON3B::CNewUIOptionWindow::UnloadImages()
{
	DeleteBitmap(IMAGE_OPTION_BTN_CLOSE);
	DeleteBitmap(IMAGE_OPTION_FRAME_BACK);
	DeleteBitmap(IMAGE_OPTION_FRAME_DOWN);
	DeleteBitmap(IMAGE_OPTION_FRAME_UP);
	DeleteBitmap(IMAGE_OPTION_FRAME_LEFT);
	DeleteBitmap(IMAGE_OPTION_FRAME_RIGHT);
	DeleteBitmap(IMAGE_OPTION_LINE);
	DeleteBitmap(IMAGE_OPTION_POINT);
	DeleteBitmap(IMAGE_OPTION_BTN_CHECK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_BACK);
	DeleteBitmap(IMAGE_OPTION_EFFECT_COLOR);
	DeleteBitmap(IMAGE_OPTION_VOLUME_BACK);
	DeleteBitmap(IMAGE_OPTION_VOLUME_COLOR);
#if CLIENT_VERSION >= 1001
	DeleteBitmap(IMAGE_OPTION_LANG_ENG);
	DeleteBitmap(IMAGE_OPTION_LANG_POR);
	DeleteBitmap(IMAGE_OPTION_LANG_SPN);
#endif
}

void SEASON3B::CNewUIOptionWindow::RenderFrame()
{
	float x, y;
	x = m_Pos.x;
	y = m_Pos.y;


#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	RenderImage(IMAGE_OPTION_FRAME_BACK, x, y, 190.f, 249.f);
	RenderImage(IMAGE_OPTION_FRAME_UP, x, y, 190.f, 64.f);
	y += 64.f;
	for (int i = 0; i < 14; ++i)
	{
		RenderImage(IMAGE_OPTION_FRAME_LEFT, x, y, 21.f, 10.f);
		RenderImage(IMAGE_OPTION_FRAME_RIGHT, x + 190 - 21, y, 21.f, 10.f);
		y += 10.f;
	}
	RenderImage(IMAGE_OPTION_FRAME_DOWN, x, y, 190.f, 45.f);

	y = m_Pos.y + 60.f;
	RenderImage(IMAGE_OPTION_LINE, x + 18, y, 154.f, 2.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_LINE, x + 18, y, 154.f, 2.f);
	y += 40.f;
	RenderImage(IMAGE_OPTION_LINE, x + 18, y, 154.f, 2.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_LINE, x + 18, y, 154.f, 2.f);
#elif CLIENT_VERSION >= 1001
	float width, height;

	width = 190;
	height = 5;

	if (height <= 0) {
		height = 1;
	}

	float newH = (80.f / 420.f) * width;

	height = OPTION_HEIGHT / newH;

	height--;

	RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_TOP_TITLEBAR, x, y+1, width, newH, 0.0009765625, 0.00390625, 0.81942085597827, 0.62109375, 1, 1, 1.0);
	RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_TOP_TITLEBAR, x+width, y+1, width, newH, 0.0780400825, 0.00390625, 0.81942085597827, 0.62109375, 1, 1, 1.0);
	y += newH;
	float addY = 0;
	if (height > 0) {
		for (int i = 1; i <= height; i++) {
			RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_MIDDLE, x, y, width, newH, 0.0009765625, 0.00390625, 0.81942085597827, 0.62109375, 1, 1, 1.0);
			RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_MIDDLE, x+width, y, width, newH, 0.0780400825, 0.00390625, 0.81942085597827, 0.62109375, 1, 1, 1.0);
			y += newH;
		}
	}
	RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_BOTTOM, x, y, width, newH, 0.0009765625, 0.00390625, 0.81942085597827, 0.62109375, 1, 1, 1.0);
	RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_BOTTOM, x+ width, y, width, newH, 0.0780400825, 0.00390625, 0.81942085597827, 0.62109375, 1, 1, 1.0);

	y = m_Pos.y + newH + 7;
	glColor4f(0.54, 0.422, 0.2322, 0.3);
	RenderColor(x + width-0.5f, y, 1.f, (height - 3) * newH + 5);
	EndRenderColor();

	y = m_Pos.y + 218;
	x = m_Pos.x + 27;

	switch (g_pMultiLanguage->GetLanguage())
	{
	case 0:
		RenderBitmap(IMAGE_OPTION_LANG_ENG, x, y, 42, 27, 0.001953125, 0.0009765625, 0.998046875, 0.32763671875, 1, 1, 1.0);
		RenderBitmap(IMAGE_OPTION_LANG_POR, x + 47, y, 42, 27, 0.001953125, 0.0009765625+0.32763671875, 0.998046875, 0.32763671875, 1, 1, 1.0);
		RenderBitmap(IMAGE_OPTION_LANG_SPN, x + 94, y, 42, 27, 0.001953125, 0.0009765625+0.32763671875, 0.998046875, 0.32763671875, 1, 1, 1.0);
		break;
	case 1:
		RenderBitmap(IMAGE_OPTION_LANG_ENG, x, y, 42, 27, 0.001953125, 0.0009765625 + 0.32763671875, 0.998046875, 0.32763671875, 1, 1, 1.0);
		RenderBitmap(IMAGE_OPTION_LANG_POR, x + 47, y, 42, 27, 0.001953125, 0.0009765625, 0.998046875, 0.32763671875, 1, 1, 1.0);
		RenderBitmap(IMAGE_OPTION_LANG_SPN, x + 94, y, 42, 27, 0.001953125, 0.0009765625 + 0.32763671875, 0.998046875, 0.32763671875, 1, 1, 1.0);
		break;

	case 2:
		RenderBitmap(IMAGE_OPTION_LANG_ENG, x, y, 42, 27, 0.001953125, 0.0009765625+0.32763671875, 0.998046875, 0.32763671875, 1, 1, 1.0);
		RenderBitmap(IMAGE_OPTION_LANG_POR, x + 47, y, 42, 27, 0.001953125, 0.0009765625+0.32763671875, 0.998046875, 0.32763671875, 1, 1, 1.0);
		RenderBitmap(IMAGE_OPTION_LANG_SPN, x + 94, y, 42, 27, 0.001953125, 0.0009765625, 0.998046875, 0.32763671875, 1, 1, 1.0);
		break;

	}
#endif
}

void SEASON3B::CNewUIOptionWindow::SetLanguage(const char* lang, BYTE value)
{
	if (!IsSupportedClientLanguage(lang))
	{
		return;
	}

	if (_stricmp(g_aszMLSelection, lang) == 0)
	{
		return;
	}

	g_pMultiLanguage->SetLanguage(value);
	strcpy_s(g_aszMLSelection, 4, lang);
	g_strSelectedML = g_aszMLSelection;

	SaveClientLanguageSelection(lang);
	ReloadLocalizedClientData(lang);

	if (g_bGameServerConnected == TRUE)
	{
		ClientSendMac();
	}
}

void SEASON3B::CNewUIOptionWindow::RenderContents()
{
	float x, y;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	x = m_Pos.x + 20.f;
	y = m_Pos.y + 46.f;
	RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 40.f;
	RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	y += 22.f;
	RenderImage(IMAGE_OPTION_POINT, x, y, 10.f, 10.f);
	
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+48, GlobalText[386]);
	g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+70, GlobalText[387]);
	g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+92, GlobalText[389]);
	g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+132, GlobalText[919]);
	g_pRenderText->RenderText(m_Pos.x+40, m_Pos.y+154, GlobalText[1840]);

#elif CLIENT_VERSION >= 1001
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetFont(g_hFontMediumBold);
	g_pRenderText->RenderText(m_Pos.x, m_Pos.y+14, GlobalText[3211], OPTION_WIDTH, 0, RT3_SORT_CENTER);
	g_pRenderText->SetFont(g_hFontBold);

	x = m_Pos.x + 50.f + 190.f;
	y = m_Pos.y + 45.f;
	g_pRenderText->RenderText(x, y, GlobalText[3201]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[3202]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[3203]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[3204]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[3205]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[3206]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[3207]);
	y += 22;
	g_pRenderText->RenderText(x, y, GlobalText[3210]);
	x = m_Pos.x + 50.f;
	y = m_Pos.y + 45.f;
	g_pRenderText->RenderText(x, y, GlobalText[386]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[387]);
	y += 20;
	g_pRenderText->RenderText(x, y, GlobalText[919]);
	y += 20;
	g_pRenderText->RenderText(m_Pos.x, y, GlobalText[389], 190, 0, RT3_SORT_CENTER);
	y += 35;
	g_pRenderText->RenderText(m_Pos.x, y, GlobalText[1840], 190, 0, RT3_SORT_CENTER);

	y = m_Pos.y + 204;
	g_pRenderText->RenderText(m_Pos.x + 25, y, GlobalText[3200], 141, 0, RT3_SORT_CENTER);

#endif

}

void SEASON3B::CNewUIOptionWindow::RenderButtons()
{

	if (m_Window)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 220, m_Pos.y + 182, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 220, m_Pos.y + 182, 15, 15, 0, 15.f);
	}


#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (m_bAutoAttack)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 43, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 43, 15, 15, 0, 15.f);
	}

	if (m_bWhisperSound)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 65, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 65, 15, 15, 0, 15.f);
	}

	if (m_bSlideHelp)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 127, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, m_Pos.x + 150, m_Pos.y + 127, 15, 15, 0, 15.f);
	}

	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 33, m_Pos.y + 104, 124.f, 16.f);
	if (m_iVolumeLevel > 0)
	{
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 33, m_Pos.y + 104, 124.f * 0.1f * (m_iVolumeLevel), 16.f);
	}

	RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x + 25, m_Pos.y + 168, 141.f, 29.f);
	if (m_iRenderLevel >= 0)
	{
		RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x + 25, m_Pos.y + 168, 141.f * 0.2f * (m_iRenderLevel + 1), 29.f);
	}

#elif CLIENT_VERSION >= 1001
	float x, y;
	x = m_Pos.x + 30.f + 190.f;
	y = m_Pos.y + 41.f;

	if (m_EffectNormal)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_EffectParticle)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_EffectSprite)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_Wings)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_Swords)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_Equipments)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_Terrain)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	x = m_Pos.x + 30.f;
	y = m_Pos.y + 41.f;

	if (m_bAutoAttack)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_bWhisperSound)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x,y, 15, 15, 0, 15.f);
	}

	y += 20;

	if (m_bSlideHelp)
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 0);
	}
	else
	{
		RenderImage(IMAGE_OPTION_BTN_CHECK, x, y, 15, 15, 0, 15.f);
	}

	y += 37;

	RenderImage(IMAGE_OPTION_VOLUME_BACK, m_Pos.x + 33, y, 124.f, 16.f);
	if (m_iVolumeLevel > 0)
	{
		RenderImage(IMAGE_OPTION_VOLUME_COLOR, m_Pos.x + 33, y, 124.f * 0.1f * (m_iVolumeLevel), 16.f);
	}

	y += 35;

	RenderImage(IMAGE_OPTION_EFFECT_BACK, m_Pos.x + 25, y, 141.f, 29.f);
	if (m_iRenderLevel >= 0)
	{
		RenderImage(IMAGE_OPTION_EFFECT_COLOR, m_Pos.x + 25, y, 141.f * 0.2f * (m_iRenderLevel + 1), 29.f);
	}

	float closeX = m_Pos.x + OPTION_WIDTH - 21;
	float closeY = m_Pos.y + 3;
	bool closeHover = CheckMouseIn(closeX, closeY, 18, 18);

	EnableAlphaBlend();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 10; ++i)
	{
		RenderColor(closeX, closeY, 18, 18);
	}

	glColor4f(0.88f, 0.00f, 0.00f, 1.0f);

	for (int i = 0; i < 16; ++i)
	{
		RenderColor(closeX, closeY, 18, 18);
	}

	glColor4f(1.0f, 0.22f, 0.20f, 1.0f);
	RenderColor(closeX, closeY, 18, 1);
	RenderColor(closeX, closeY + 17, 18, 1);
	RenderColor(closeX, closeY, 1, 18);
	RenderColor(closeX + 17, closeY, 1, 18);
	EndRenderColor();
	EnableAlphaTest();

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->RenderText(closeX, closeY + 4, "X", 18, 0, RT3_SORT_CENTER);
#endif

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_BtnClose.Render();
#endif

}

void SEASON3B::CNewUIOptionWindow::SetAutoAttack(bool bAuto)
{
	m_bAutoAttack = bAuto;
}

bool SEASON3B::CNewUIOptionWindow::IsAutoAttack()
{
	return m_bAutoAttack;
}

void SEASON3B::CNewUIOptionWindow::SetWhisperSound(bool bSound)
{
	m_bWhisperSound = bSound;
}

bool SEASON3B::CNewUIOptionWindow::IsWhisperSound()
{
	return m_bWhisperSound;
}

void SEASON3B::CNewUIOptionWindow::SetSlideHelp(bool bHelp)
{
	m_bSlideHelp = bHelp;
}

bool SEASON3B::CNewUIOptionWindow::IsSlideHelp()
{
	return m_bSlideHelp;
}

void SEASON3B::CNewUIOptionWindow::SetVolumeLevel(int iVolume)
{
	m_iVolumeLevel = iVolume;
}

int SEASON3B::CNewUIOptionWindow::GetVolumeLevel()
{
	return m_iVolumeLevel;
}

void SEASON3B::CNewUIOptionWindow::SetRenderLevel(int iRender)
{
	m_iRenderLevel = iRender;
}

int SEASON3B::CNewUIOptionWindow::GetRenderLevel()
{
	return m_iRenderLevel;
}
