
#include "stdafx.h"

#include "NewUIChatLogWindow.h"
#include "NewUIManager.h"
#include "NewUISystem.h"
#include "DSPlaySound.h"
#include "UIControls.h"
#include "ZzzInterface.h"
#include "ZzzScene.h"
#include "WideScreen.h"
#include "MessageColor.h"
#include "CustomColorChat.h"

using namespace SEASON3B;

namespace
{
	const float SOURCE_CHAT_DARK_R = 0.030f;
	const float SOURCE_CHAT_DARK_G = 0.027f;
	const float SOURCE_CHAT_DARK_B = 0.023f;
	const float SOURCE_CHAT_PANEL_R = 0.090f;
	const float SOURCE_CHAT_PANEL_G = 0.073f;
	const float SOURCE_CHAT_PANEL_B = 0.052f;
	const float SOURCE_CHAT_BORDER_R = 0.470f;
	const float SOURCE_CHAT_BORDER_G = 0.330f;
	const float SOURCE_CHAT_BORDER_B = 0.125f;
	const float SOURCE_CHAT_HOVER_R = 0.950f;
	const float SOURCE_CHAT_HOVER_G = 0.690f;
	const float SOURCE_CHAT_HOVER_B = 0.255f;

	void RenderSourceChatColor(float x, float y, float width, float height, float r, float g, float b, float a)
	{
		glColor4f(r, g, b, a);
		RenderColor(x, y, width, height);
	}

	void RenderSourceChatBorder(float x, float y, float width, float height, float r, float g, float b, float a)
	{
		RenderSourceChatColor(x, y, width, 1.0f, r, g, b, a);
		RenderSourceChatColor(x, y + height - 1.0f, width, 1.0f, r, g, b, a);
		RenderSourceChatColor(x, y, 1.0f, height, r, g, b, a);
		RenderSourceChatColor(x + width - 1.0f, y, 1.0f, height, r, g, b, a);
	}

	bool IsPostMessageText(const type_string& text)
	{
		return (text.find("[S-") == 0 || text.find("[POST]") == 0 || text.find("[Post]") == 0 || text.find("[post]") == 0);
	}

	type_string GetPostDisplayText(const type_string& text)
	{
		size_t displayStart = type_string::npos;

		if (text.find("[POST]") == 0 || text.find("[Post]") == 0 || text.find("[post]") == 0)
		{
			displayStart = 6;
		}
		else if (text.find("[S-") == 0)
		{
			size_t sellTag = text.find("S> ");
			displayStart = (sellTag != type_string::npos) ? (sellTag + 3) : text.find("]");
			if (displayStart != type_string::npos && text[displayStart] == ']')
			{
				displayStart++;
			}
		}

		if (displayStart == type_string::npos)
		{
			return text;
		}

		while (displayStart < text.length() && text[displayStart] == ' ')
		{
			displayStart++;
		}

		return text.substr(displayStart);
	}

	bool IsWelcomeMessageText(const type_string& text)
	{
		return (text.find("[WELCOME]") == 0);
	}

	bool IsPlayTimeMessageText(const type_string& text)
	{
		return (text.find("[PLAYTIME]") == 0);
	}

	bool IsQuitGameMessageText(const type_string& text)
	{
		return (text.find("quit game") != type_string::npos);
	}

	bool IsStyledSystemMessageText(const type_string& text)
	{
		return (IsPostMessageText(text) || IsWelcomeMessageText(text) || IsPlayTimeMessageText(text) || IsQuitGameMessageText(text));
	}

	type_string GetStyledDisplayText(const type_string& text)
	{
		if (IsWelcomeMessageText(text))
		{
			return text.substr(9);
		}

		if (IsPlayTimeMessageText(text))
		{
			return text.substr(10);
		}

		return GetPostDisplayText(text);
	}

#if CLIENT_VERSION >= 1001
	int GetVoidChatAddW()
	{
		return gWideScreen.GetCenterPosition(0);
	}

	int GetVoidChatClosedX()
	{
		return 0;
	}

	int GetVoidChatClosedY()
	{
		return (int)gWideScreen.GetBottomPosition(480.f - 50.f - 47.f);
	}

	int GetVoidChatExpandedX()
	{
		return 181 + GetVoidChatAddW();
	}

	int GetVoidChatExpandedY()
	{
		return (int)gWideScreen.GetBottomPosition(400.f);
	}
#endif
}

SEASON3B::CNewUIChatLogWindow::CNewUIChatLogWindow()
{
	Init();
}

SEASON3B::CNewUIChatLogWindow::~CNewUIChatLogWindow()
{
	Release();
}

void SEASON3B::CNewUIChatLogWindow::Init()
{
	m_pNewUIMng = NULL;
	m_WndPos.x = m_WndPos.y = 0;
	m_ScrollBtnPos.x = m_ScrollBtnPos.y = 0;
	m_WndSize.cx = WND_WIDTH; m_WndSize.cy = 0;
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_nShowingLines = 6;
	m_nShowingLinesBackup = 6;
#elif CLIENT_VERSION >= 1001
	m_nShowingLines = 9;
	m_nShowingLinesBackup = 6;
#endif
	m_iCurrentRenderEndLine = -1;
	m_fBackAlpha = 1.0f;

	m_EventState = EVENT_NONE;

	m_bShowFrame = false;

	m_CurrentRenderMsgType = TYPE_ALL_MESSAGE;
	m_bShowChatLog = true;

	m_bPointedMessage = false;
	m_iPointedMessageIndex = 0;

#if CLIENT_VERSION == 1002
	this->m_nShowingSystemLines = 6;
#else
	this->m_nShowingSystemLines = 4;
#endif
	this->m_iCurrentRenderEndSystemLine = -1;
}

void SEASON3B::CNewUIChatLogWindow::LoadImages()
{
}

void SEASON3B::CNewUIChatLogWindow::UnloadImages()
{
}

bool SEASON3B::CNewUIChatLogWindow::RenderBackground()
{
	if (m_bShowFrame)
	{
		float offset = 0;

		const float fRenderPosX = (float)m_WndPos.x + offset;
		const float fRenderPosY = (float)m_WndPos.y - (float)m_WndSize.cy;

		EnableAlphaTest(1);
		RenderSourceChatColor(fRenderPosX, fRenderPosY, (float)m_WndSize.cx, (float)m_WndSize.cy,
			SOURCE_CHAT_DARK_R, SOURCE_CHAT_DARK_G, SOURCE_CHAT_DARK_B, 0.64f);
		RenderSourceChatColor(fRenderPosX + 1.0f, fRenderPosY + 1.0f, (float)m_WndSize.cx - 2.0f, (float)m_WndSize.cy - 2.0f,
			SOURCE_CHAT_PANEL_R, SOURCE_CHAT_PANEL_G, SOURCE_CHAT_PANEL_B, GetBackAlpha());
		RenderSourceChatBorder(fRenderPosX, fRenderPosY, (float)m_WndSize.cx, (float)m_WndSize.cy,
			SOURCE_CHAT_BORDER_R, SOURCE_CHAT_BORDER_G, SOURCE_CHAT_BORDER_B, 0.62f);
		RenderSourceChatColor(fRenderPosX + 6.0f, fRenderPosY + 5.0f, (float)m_WndSize.cx - 22.0f, 1.0f,
			SOURCE_CHAT_BORDER_R, SOURCE_CHAT_BORDER_G, SOURCE_CHAT_BORDER_B, 0.30f);
		EndRenderColor();

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		DisableAlphaBlend();
	}
	return true;
}

bool SEASON3B::CNewUIChatLogWindow::RenderSystemMessages()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	float fRenderPosX = m_WndPos.x, fRenderPosY = m_WndPos.y - 120 + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#elif CLIENT_VERSION == 1001
	float fRenderPosX = m_WndPos.x, fRenderPosY = m_WndPos.y - gWideScreen.GetBottomPosition(360) + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#elif CLIENT_VERSION == 1002
	float fRenderPosX = m_WndPos.x, fRenderPosY = m_WndPos.y - gWideScreen.GetBottomPosition(380) + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#endif

	type_vector_msgs* pvecMsgs = GetMsgs(TYPE_SYSTEM_MESSAGE);

	if (pvecMsgs == NULL)
	{
		assert(!"empty chat!");
		return false;
	}

	int iRenderStartLine = 0;

	if (GetCurrentRenderEndSystemLine() >= m_nShowingSystemLines)
	{
		iRenderStartLine = GetCurrentRenderEndSystemLine() - m_nShowingSystemLines + 1;
	}
	else
	{
		fRenderPosY = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * (m_nShowingSystemLines - GetCurrentRenderEndSystemLine() - 1));
	}

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	fRenderPosY -= 200;
#endif

	BYTE byAlpha = 150;

	if (m_bShowFrame) byAlpha = 125;

	EnableAlphaTest(1);

	for (int i = iRenderStartLine, s = 0; i <= GetCurrentRenderEndSystemLine(); i++, s++)
	{
		if (i < 0 && i >= (int)pvecMsgs->size()) break;

		bool bRenderMessage = true;

		g_pRenderText->SetFont(g_hFont);

		CMessageText* pMsgText = (*pvecMsgs)[i];

		if (IsPostMessageText(pMsgText->GetText()))
		{
			continue;
		}

		if (pMsgText->GetType() == TYPE_SYSTEM_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 0, 0, 150);
			g_pRenderText->SetTextColor(100, 150, 255, 255);
			if (pMsgText->GetColor() >= 25 && pMsgText->GetColor() <= 100 && gMessageColor.CheckIsColor(pMsgText->GetColor(), 0))
			{
				gMessageColor.SetMessageColor(pMsgText->GetColor());
			}
		}

		

		if (bRenderMessage && !pMsgText->GetText().empty())
		{
			bool bWelcomeMessage = IsWelcomeMessageText(pMsgText->GetText());
			bool bPlayTimeMessage = IsPlayTimeMessageText(pMsgText->GetText());
			bool bQuitGameMessage = IsQuitGameMessageText(pMsgText->GetText());
			bool bStyledMessage = (bWelcomeMessage || bPlayTimeMessage || bQuitGameMessage);
			type_string strDisplayText = bStyledMessage ? GetStyledDisplayText(pMsgText->GetText()) : pMsgText->GetText();

			if (bWelcomeMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				g_pRenderText->SetBgColor(0, 0, 0, 180);
				g_pRenderText->SetTextColor(0, 255, 70, 255);
			}
			else if (bPlayTimeMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				g_pRenderText->SetBgColor(255, 255, 0, 220);
				g_pRenderText->SetTextColor(0, 0, 0, 255);
			}
			else if (bQuitGameMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				g_pRenderText->SetBgColor(0, 0, 0, 180);
				g_pRenderText->SetTextColor(0, 255, 70, 255);
			}

			POINT ptRenderPos = { (long)fRenderPosX + (long)WND_LEFT_RIGHT_EDGE, (long)fRenderPosY + (long)FONT_LEADING + ((long)SCROLL_MIDDLE_PART_HEIGHT * (long)s) };


			if (!pMsgText->GetID().empty())
			{
				if (m_bPointedMessage == true && m_iPointedMessageIndex == i)
				{
					//g_pRenderText->SetBgColor(30, 30, 30, 180);
					//g_pRenderText->SetTextColor(255, 128, 255, 255);
				}

				std::string strIDUTF8 = "";

				g_pMultiLanguage->ConvertANSIToUTF8OrViceVersa(strIDUTF8, (pMsgText->GetID()).c_str());

				type_string strLine = strIDUTF8 + ": " + strDisplayText;

				g_pRenderText->RenderText(ptRenderPos.x, ptRenderPos.y, strLine.c_str());
			}
			else
			{
				g_pRenderText->RenderText(ptRenderPos.x, ptRenderPos.y, strDisplayText.c_str());
			}
		}
	}

	DisableAlphaBlend();

	return true;
}

bool SEASON3B::CNewUIChatLogWindow::RenderMessages()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	float fRenderPosX = m_WndPos.x, fRenderPosY = m_WndPos.y - m_WndSize.cy + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#elif CLIENT_VERSION == 1001
	float fRenderPosX = m_WndPos.x, fRenderPosY = m_WndPos.y - m_WndSize.cy + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#elif CLIENT_VERSION == 1002
	float fRenderPosX = m_WndPos.x, fRenderPosY = m_WndPos.y - m_WndSize.cy + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#endif

	type_vector_msgs* pvecMsgs = GetMsgs(GetCurrentMsgType());

	if (pvecMsgs == NULL)
	{
		assert(!"empty chat!");
		return false;
	}

	int iRenderStartLine = 0;
	if (GetCurrentRenderEndLine() >= m_nShowingLines)
	{
		iRenderStartLine = GetCurrentRenderEndLine() - m_nShowingLines + 1;
	}
	else
	{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		fRenderPosY = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * (m_nShowingLines - GetCurrentRenderEndLine() - 1));
#elif CLIENT_VERSION >= 1001
		if (m_bShowFrame)
			fRenderPosY = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * (m_nShowingLines - GetCurrentRenderEndLine() - 1));
		else
			fRenderPosY = fRenderPosY + FONT_LEADING + (11 * (m_nShowingLines - GetCurrentRenderEndLine() - 1));

#endif

	}

	BYTE byAlpha = 150;
	if (m_bShowFrame) byAlpha = 125;

	float offset = 0;

	EnableAlphaTest(1);
	for (int i = iRenderStartLine, s = 0; i <= GetCurrentRenderEndLine(); i++, s++)
	{
		if (i < 0 && i >= (int)pvecMsgs->size()) break;

		bool bRenderMessage = true;
		g_pRenderText->SetFont(g_hFont);

		CMessageText* pMsgText = (*pvecMsgs)[i];

		if (pMsgText->GetType() == TYPE_SYSTEM_MESSAGE)
		{
			continue;
		}

		if (pMsgText->GetType() == TYPE_WHISPER_MESSAGE)
		{
			g_pRenderText->SetBgColor(255, 128, 0, 200);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_SYSTEM_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 0, 0, 150);
			g_pRenderText->SetTextColor(100, 150, 255, 255);
		}
		else if (pMsgText->GetType() == TYPE_ERROR_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 0, 0, 150);
			g_pRenderText->SetTextColor(255, 30, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_CHAT_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 0, 0, byAlpha);
			g_pRenderText->SetTextColor(205, 220, 239, 255);
		}
		else if (pMsgText->GetType() == TYPE_PARTY_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 200, 255, 150);
			g_pRenderText->SetTextColor(255, 255, 255, 255);
		}
		else if (pMsgText->GetType() == TYPE_GUILD_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 255, 150, 200);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_UNION_MESSAGE)
		{
			g_pRenderText->SetBgColor(200, 200, 0, 200);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GM_MESSAGE)
		{
			g_pRenderText->SetBgColor(30, 30, 30, 200);
			g_pRenderText->SetTextColor(250, 200, 50, 255);
			g_pRenderText->SetFont(g_hFontBold);
		

			//CUSTOM COLOR CHAT LEFT
			if (g_bCustomColor)
			{
				for (std::map<std::string, CUSTOM_COLOR_CHAT_LEFT>::iterator it = gCustomColorChat.m_CustomColorChatLeft.begin(); it != gCustomColorChat.m_CustomColorChatLeft.end(); it++)
				{
					auto chatInfo = &(*it).second;
			
					for (int i = 0; i < MAX_CHARACTERS_CLIENT; i++)
					{
						CHARACTER* c = &CharactersClient[i];
						if (strcmp(c->ID, chatInfo->Player) == 0)
						{
							if (chatInfo->Bold == 1)
								g_pRenderText->SetFont(g_hFontBold);
							else
								g_pRenderText->SetFont(g_hFont);
			
							if (chatInfo->ColorR > -1 && chatInfo->ColorG > -1 && chatInfo->ColorB > -1)
							{
								g_pRenderText->SetTextColor(chatInfo->ColorR, chatInfo->ColorG, chatInfo->ColorB, chatInfo->ColorA);
							}
			
							if (chatInfo->BackgroundR > -1 && chatInfo->BackgroundG > -1 && chatInfo->BackgroundB > -1)
							{
								if (chatInfo->BackgroundA > -1)
									g_pRenderText->SetBgColor(chatInfo->BackgroundR, chatInfo->BackgroundG, chatInfo->BackgroundB, chatInfo->BackgroundA);
								else
									g_pRenderText->SetBgColor(chatInfo->BackgroundR, chatInfo->BackgroundG, chatInfo->BackgroundB, byAlpha);
							}

							break;
						}
					}
				}
			}
		}
		else if (pMsgText->GetType() == TYPE_PARTY_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 200, 255, 150);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GUILD_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 255, 150, 200);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GUILD_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 255, 150, 200);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_UNION_MESSAGE)
		{
			g_pRenderText->SetBgColor(200, 200, 0, 200);
			g_pRenderText->SetTextColor(0, 0, 0, 255);
		}
		else if (pMsgText->GetType() == TYPE_GM_MESSAGE)
		{
			g_pRenderText->SetBgColor(30, 30, 30, 200);
			g_pRenderText->SetTextColor(250, 200, 50, 255);
			g_pRenderText->SetFont(g_hFontBold);
		}
#if CLIENT_VERSION == 1001
		else if (pMsgText->GetType() == TYPE_SYSTEM_MESSAGE)
		{
			g_pRenderText->SetBgColor(0, 0, 0, 150);
			g_pRenderText->SetTextColor(100, 150, 255, 255);
			if (pMsgText->GetColor() >= 25 && pMsgText->GetColor() <= 100 && gMessageColor.CheckIsColor(pMsgText->GetColor(), 0))
			{
				gMessageColor.SetMessageColor(pMsgText->GetColor());
			}
		}
#endif
		else
		{
			bRenderMessage = false;
		}

		if (bRenderMessage && !pMsgText->GetText().empty())
		{
			bool bPostMessage = IsPostMessageText(pMsgText->GetText());
			bool bWelcomeMessage = IsWelcomeMessageText(pMsgText->GetText());
			bool bPlayTimeMessage = IsPlayTimeMessageText(pMsgText->GetText());
			bool bQuitGameMessage = IsQuitGameMessageText(pMsgText->GetText());
			bool bStyledMessage = (bPostMessage || bWelcomeMessage || bPlayTimeMessage || bQuitGameMessage);
			type_string strDisplayText = bStyledMessage ? GetStyledDisplayText(pMsgText->GetText()) : pMsgText->GetText();

			if (bPostMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				if (m_bShowFrame)
				{
					g_pRenderText->SetBgColor(0, 118, 245, 220);
					g_pRenderText->SetTextColor(10, 18, 28, 255);
				}
				else
				{
					g_pRenderText->SetBgColor(235, 235, 235, 190);
					g_pRenderText->SetTextColor(35, 35, 35, 255);
				}
			}
			else if (bWelcomeMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				g_pRenderText->SetBgColor(0, 0, 0, 180);
				g_pRenderText->SetTextColor(0, 255, 70, 255);
			}
			else if (bPlayTimeMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				g_pRenderText->SetBgColor(255, 255, 0, 220);
				g_pRenderText->SetTextColor(0, 0, 0, 255);
			}
			else if (bQuitGameMessage)
			{
				g_pRenderText->SetFont(g_hFont);
				g_pRenderText->SetBgColor(0, 0, 0, 180);
				g_pRenderText->SetTextColor(0, 255, 70, 255);
			}
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
			POINT ptRenderPos = { (long)fRenderPosX + (long)WND_LEFT_RIGHT_EDGE + offset, (long)fRenderPosY + (long)FONT_LEADING + ((long)SCROLL_MIDDLE_PART_HEIGHT * (long)s) };
#elif CLIENT_VERSION >= 1001
			POINT ptRenderPos = { (long)fRenderPosX + (long)WND_LEFT_RIGHT_EDGE + offset, (long)fRenderPosY + (long)FONT_LEADING + ((long)SCROLL_MIDDLE_PART_HEIGHT * (long)s) };
			if (!m_bShowFrame)
				ptRenderPos.y = (long)fRenderPosY + (long)FONT_LEADING + ((long)11 * (long)s);

#endif
			if (!pMsgText->GetID().empty())
			{
				if (m_bPointedMessage == true && m_iPointedMessageIndex == i)
				{
					//g_pRenderText->SetBgColor(30, 30, 30, 180);
					//g_pRenderText->SetTextColor(255, 128, 255, 255);
				}
				
				//CUSTOM COLOR CHAT LEFT NAME
				std::string strIDUTF8 = "";
				g_pMultiLanguage->ConvertANSIToUTF8OrViceVersa(strIDUTF8, (pMsgText->GetID()).c_str());
				type_string strLine = bPostMessage ? (strIDUTF8 + " : " + strDisplayText) : (strIDUTF8 + ": " + strDisplayText);
				g_pRenderText->RenderText(ptRenderPos.x, ptRenderPos.y, strLine.c_str());
			}
			else
			{
				g_pRenderText->RenderText(ptRenderPos.x, ptRenderPos.y, strDisplayText.c_str());
			}
		}
	}
	DisableAlphaBlend();

	return true;
}

bool SEASON3B::CNewUIChatLogWindow::RenderFrame()
{
	if (m_bShowFrame)
	{
		float offset = 0;

		const float fRenderPosX = (float)m_WndPos.x + offset;
		const float fRenderPosY = (float)m_WndPos.y - (float)m_WndSize.cy;

		EnableAlphaTest(1);

		const bool bResizeDown = (m_EventState == EVENT_RESIZING_BTN_DOWN);
		const bool bResizeHover = (m_EventState == EVENT_RESIZING_BTN_HOVER || bResizeDown);
		const float gripX = fRenderPosX;
		const float gripY = fRenderPosY - (float)RESIZING_BTN_HEIGHT;
		RenderSourceChatColor(gripX, gripY, (float)RESIZING_BTN_WIDTH, (float)RESIZING_BTN_HEIGHT,
			bResizeDown ? 0.180f : SOURCE_CHAT_DARK_R,
			bResizeDown ? 0.130f : SOURCE_CHAT_DARK_G,
			bResizeDown ? 0.060f : SOURCE_CHAT_DARK_B,
			0.86f);
		RenderSourceChatBorder(gripX, gripY, (float)RESIZING_BTN_WIDTH, (float)RESIZING_BTN_HEIGHT,
			bResizeHover ? SOURCE_CHAT_HOVER_R : SOURCE_CHAT_BORDER_R,
			bResizeHover ? SOURCE_CHAT_HOVER_G : SOURCE_CHAT_BORDER_G,
			bResizeHover ? SOURCE_CHAT_HOVER_B : SOURCE_CHAT_BORDER_B,
			bResizeHover ? 0.88f : 0.58f);
		RenderSourceChatColor(gripX + 78.0f, gripY + 4.0f, (float)RESIZING_BTN_WIDTH - 156.0f, 1.0f,
			SOURCE_CHAT_BORDER_R, SOURCE_CHAT_BORDER_G, SOURCE_CHAT_BORDER_B, 0.52f);
		RenderSourceChatColor(gripX + 78.0f, gripY + 6.0f, (float)RESIZING_BTN_WIDTH - 156.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 0.42f);

		const float scrollX = fRenderPosX + (float)m_WndSize.cx - 14.0f;
		const float scrollY = fRenderPosY + 2.0f;
		const float scrollW = 9.0f;
		const float scrollH = (float)m_WndSize.cy - 4.0f;
		RenderSourceChatColor(scrollX, scrollY, scrollW, scrollH, 0.010f, 0.010f, 0.010f, 0.62f);
		RenderSourceChatBorder(scrollX, scrollY, scrollW, scrollH, SOURCE_CHAT_BORDER_R, SOURCE_CHAT_BORDER_G, SOURCE_CHAT_BORDER_B, 0.48f);

		const bool bScrollDown = (m_EventState == EVENT_SCROLL_BTN_DOWN);
		RenderSourceChatColor((float)m_ScrollBtnPos.x + offset, (float)m_ScrollBtnPos.y, 9.0f, (float)SCROLL_BTN_HEIGHT,
			bScrollDown ? 0.220f : 0.145f,
			bScrollDown ? 0.160f : 0.100f,
			bScrollDown ? 0.072f : 0.045f,
			0.95f);
		RenderSourceChatBorder((float)m_ScrollBtnPos.x + offset, (float)m_ScrollBtnPos.y, 9.0f, (float)SCROLL_BTN_HEIGHT,
			bScrollDown ? SOURCE_CHAT_HOVER_R : SOURCE_CHAT_BORDER_R,
			bScrollDown ? SOURCE_CHAT_HOVER_G : SOURCE_CHAT_BORDER_G,
			bScrollDown ? SOURCE_CHAT_HOVER_B : SOURCE_CHAT_BORDER_B,
			bScrollDown ? 0.90f : 0.66f);
		RenderSourceChatColor((float)m_ScrollBtnPos.x + offset + 3.0f, (float)m_ScrollBtnPos.y + 8.0f, 3.0f, 1.0f, SOURCE_CHAT_HOVER_R, SOURCE_CHAT_HOVER_G, SOURCE_CHAT_HOVER_B, 0.42f);
		RenderSourceChatColor((float)m_ScrollBtnPos.x + offset + 3.0f, (float)m_ScrollBtnPos.y + 15.0f, 3.0f, 1.0f, SOURCE_CHAT_HOVER_R, SOURCE_CHAT_HOVER_G, SOURCE_CHAT_HOVER_B, 0.42f);
		RenderSourceChatColor((float)m_ScrollBtnPos.x + offset + 3.0f, (float)m_ScrollBtnPos.y + 22.0f, 3.0f, 1.0f, SOURCE_CHAT_HOVER_R, SOURCE_CHAT_HOVER_G, SOURCE_CHAT_HOVER_B, 0.42f);
		EndRenderColor();

		DisableAlphaBlend();
	}

	return true;
}

bool SEASON3B::CNewUIChatLogWindow::Create(CNewUIManager* pNewUIMng, int x, int y, int nShowingLines /* = 6 */)
{
	Release();

	if (NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHATLOGWINDOW, this);
#if CLIENT_VERSION >= 1001
	x = GetVoidChatClosedX();
	y = GetVoidChatClosedY();
#endif
	m_WndPos.x = x; m_WndPos.y = y;
	SetNumberOfShowingLines(nShowingLines);
	LoadImages();
	return true;
}

void SEASON3B::CNewUIChatLogWindow::Release()
{
	UnloadImages();
	ResetFilter();
	ClearAll();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
	Init();
}

void SEASON3B::CNewUIChatLogWindow::SetPosition(int x, int y)
{
	m_WndPos.x = x;
	m_WndPos.y = y;
}

void SEASON3B::CNewUIChatLogWindow::AddText(const type_string& strID, const type_string& strText, MESSAGE_TYPE MsgType, DWORD MsgColor, MESSAGE_TYPE ErrMsgType /*= TYPE_ALL_MESSAGE*/)
{
	if (strID.empty() && strText.empty())
	{
		return;
	}

	if (MsgType == TYPE_SYSTEM_MESSAGE && IsPostMessageText(strText))
	{
		MsgType = TYPE_CHAT_MESSAGE;
	}

	if (GetNumberOfLines(MsgType) >= MAX_NUMBER_OF_LINES)
	{
		RemoveFrontLine(MsgType);
	}

	if (GetNumberOfLines(TYPE_ALL_MESSAGE) >= MAX_NUMBER_OF_LINES)
	{
		RemoveFrontLine(TYPE_ALL_MESSAGE);
	}

	if (m_vecFilters.empty() == true)
	{
		ProcessAddText(strID, strText, MsgType, MsgColor, ErrMsgType);
	}
	else
	{
		if (MsgType != TYPE_CHAT_MESSAGE || MsgType != TYPE_CHAT_MESSAGE_CUSTOM)
		{
			ProcessAddText(strID, strText, MsgType, MsgColor, ErrMsgType);
		}
		else if (CheckFilterText(strID) || CheckFilterText(strText))
		{
			ProcessAddText(strID, strText, MsgType, MsgColor, ErrMsgType);

			if (g_pOption->IsWhisperSound())
			{
				PlayBuffer(SOUND_WHISPER);
			}
		}
	}
}

void SEASON3B::CNewUIChatLogWindow::ProcessAddText(const type_string& strID, const type_string& strText, MESSAGE_TYPE MsgType, DWORD MsgColor, MESSAGE_TYPE ErrMsgType)
{
	type_vector_msgs* pvecMsgs = GetMsgs(MsgType);

	if (pvecMsgs == NULL)
	{
		assert(!"Empty Message");
		return;
	}

	int nScrollLines = 0;
	int nScrollSystemLines = 0;

	if (IsWelcomeMessageText(strText))
	{
		CMessageText* pMsgText = new CMessageText;
		if (!pMsgText->Create(strID, strText, MsgType, MsgColor))
			delete pMsgText;
		else
		{
			pvecMsgs->push_back(pMsgText);
		}

		CMessageText* pAllMsgText = new CMessageText;
		if (!pAllMsgText->Create(strID, strText, MsgType, MsgColor))
			delete pAllMsgText;
		else
		{
			m_vecAllMsgs.push_back(pAllMsgText);
		}

		if (MsgType == TYPE_SYSTEM_MESSAGE)
		{
			nScrollSystemLines++;
		}
		else if ((GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType) && MsgType != TYPE_SYSTEM_MESSAGE)
		{
			nScrollLines++;
		}

		if (MsgType == TYPE_SYSTEM_MESSAGE)
		{
			pvecMsgs = GetMsgs(MsgType);
			if (pvecMsgs != NULL)
			{
				if (nScrollSystemLines > 0 && ((pvecMsgs->size() - (m_iCurrentRenderEndSystemLine + 1) - nScrollSystemLines) < 3))
					m_iCurrentRenderEndSystemLine = pvecMsgs->size() - 1;
				else if (!m_bShowFrame)
					m_iCurrentRenderEndSystemLine = pvecMsgs->size() - 1;
			}
		}
		else
		{
			pvecMsgs = GetMsgs(GetCurrentMsgType());
			if (pvecMsgs != NULL)
			{
				if (nScrollLines > 0 && ((pvecMsgs->size() - (m_iCurrentRenderEndLine + 1) - nScrollLines) < 3))
					m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
				else if (!m_bShowFrame)
					m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
			}
		}

		return;
	}

	if (strText.size() >= 20)
	{
		type_string	strText1, strText2;
		SeparateText(strID, strText, strText1, strText2);
		if (!strText1.empty())
		{
			CMessageText* pMsgText = new CMessageText;
			if (!pMsgText->Create(strID, strText1, MsgType, MsgColor))
				delete pMsgText;
			else
			{
				pvecMsgs->push_back(pMsgText);
			}

#if CLIENT_VERSION != 1001
			if (MsgType != TYPE_SYSTEM_MESSAGE)
#endif
			{
				CMessageText* pAllMsgText = new CMessageText;
				if (!pAllMsgText->Create(strID, strText1, MsgType, MsgColor))
					delete pAllMsgText;
				else
				{
					m_vecAllMsgs.push_back(pAllMsgText);
				}
			}

			if ((MsgType == TYPE_ERROR_MESSAGE) && (ErrMsgType != TYPE_ERROR_MESSAGE && ErrMsgType != TYPE_ALL_MESSAGE))
			{
				type_vector_msgs* pErrvecMsgs = GetMsgs(ErrMsgType);
				if (pErrvecMsgs == NULL)
				{
					assert(!"Error Chat");
					return;
				}

				CMessageText* pErrMsgText = new CMessageText;
				if (!pErrMsgText->Create(strID, strText1, MsgType, MsgColor))
					delete pErrMsgText;
				else
				{
					pErrvecMsgs->push_back(pErrMsgText);
				}
			}

			if (MsgType == TYPE_SYSTEM_MESSAGE)
			{
				nScrollSystemLines++;
#if CLIENT_VERSION == 1001
				if ((GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType))
				{
					nScrollLines++;
				}
#endif
			}
			else if ((GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType) && MsgType != TYPE_SYSTEM_MESSAGE)
			{
				nScrollLines++;
			}
		}

		if (!strText2.empty())
		{
			CMessageText* pMsgText = new CMessageText;
			if (!pMsgText->Create("", strText2, MsgType, MsgColor))
				delete pMsgText;
			else
			{
				pvecMsgs->push_back(pMsgText);
			}

			CMessageText* pAllMsgText = new CMessageText;
			if (!pAllMsgText->Create("", strText2, MsgType, MsgColor))
				delete pAllMsgText;
			else
			{
				m_vecAllMsgs.push_back(pAllMsgText);
			}

			if ((MsgType == TYPE_ERROR_MESSAGE) && (ErrMsgType != TYPE_ERROR_MESSAGE && ErrMsgType != TYPE_ALL_MESSAGE))
			{
				type_vector_msgs* pErrvecMsgs = GetMsgs(ErrMsgType);
				if (pErrvecMsgs == NULL)
				{
					assert(!"Error chat 2");
					return;
				}

				CMessageText* pErrMsgText = new CMessageText;
				if (!pErrMsgText->Create("", strText2, MsgType, MsgColor))
					delete pErrMsgText;
				else
				{
					pErrvecMsgs->push_back(pErrMsgText);
				}
			}

			if (MsgType == TYPE_SYSTEM_MESSAGE)
			{
				nScrollSystemLines++;
#if CLIENT_VERSION == 1001
				if ((GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType))
				{
					nScrollLines++;
				}
#endif
			}
			else if (MsgType != TYPE_SYSTEM_MESSAGE && (GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType))
			{
				nScrollLines++;
			}
		}
	}
	else
	{
		CMessageText* pMsgText = new CMessageText;
		if (!pMsgText->Create(strID, strText, MsgType, MsgColor))
			delete pMsgText;
		else
		{
			pvecMsgs->push_back(pMsgText);
		}

#if CLIENT_VERSION != 1001
		if (MsgType != TYPE_SYSTEM_MESSAGE)
#endif
		{
			CMessageText* pAllMsgText = new CMessageText;
			if (!pAllMsgText->Create(strID, strText, MsgType, MsgColor))
				delete pAllMsgText;
			else
			{
				m_vecAllMsgs.push_back(pAllMsgText);
			}
		}

		if ((MsgType == TYPE_ERROR_MESSAGE) && (ErrMsgType != TYPE_ERROR_MESSAGE && ErrMsgType != TYPE_ALL_MESSAGE))
		{
			type_vector_msgs* pErrvecMsgs = GetMsgs(ErrMsgType);
			if (pErrvecMsgs == NULL)
			{
				assert(!"Error chat 3");
				return;
			}

			CMessageText* pErrMsgText = new CMessageText;
			if (!pErrMsgText->Create(strID, strText, MsgType, MsgColor))
				delete pErrMsgText;
			else
			{
				pErrvecMsgs->push_back(pErrMsgText);
			}
		}

		if (MsgType == TYPE_SYSTEM_MESSAGE)
		{
			nScrollSystemLines++;
#if CLIENT_VERSION == 1001
			if ((GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType) && MsgType != TYPE_SYSTEM_MESSAGE)
			{
				nScrollLines++;
			}
#endif
		}
		else if (MsgType != TYPE_SYSTEM_MESSAGE && (GetCurrentMsgType() == TYPE_ALL_MESSAGE || GetCurrentMsgType() == MsgType))
		{
			nScrollLines++;
		}
	}

	if (MsgType == TYPE_SYSTEM_MESSAGE)
	{
		pvecMsgs = GetMsgs(MsgType);

		if (pvecMsgs == NULL)
		{
			assert(!"Error chat 4");
			return;
		}

		if (nScrollSystemLines > 0 && ((pvecMsgs->size() - (m_iCurrentRenderEndSystemLine + 1) - nScrollSystemLines) < 3))
		{
			m_iCurrentRenderEndSystemLine = pvecMsgs->size() - 1;
		}
		else if (!m_bShowFrame)
		{
			m_iCurrentRenderEndSystemLine = pvecMsgs->size() - 1;
		}

#if CLIENT_VERSION == 1001
		pvecMsgs = GetMsgs(GetCurrentMsgType());

		if (pvecMsgs == NULL)
		{
			assert(!"Error chat 4");
			return;
		}

		//. Auto Scrolling
		if (nScrollLines > 0 && ((pvecMsgs->size() - (m_iCurrentRenderEndLine + 1) - nScrollLines) < 3))
			m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
		else if (!m_bShowFrame)
			m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
#endif
	}
	else
	{
		pvecMsgs = GetMsgs(GetCurrentMsgType());

		if (pvecMsgs == NULL)
		{
			assert(!"Error chat 4");
			return;
		}

		//. Auto Scrolling
		if (nScrollLines > 0 && ((pvecMsgs->size() - (m_iCurrentRenderEndLine + 1) - nScrollLines) < 3))
			m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
		else if (!m_bShowFrame)
			m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
	}
}

void SEASON3B::CNewUIChatLogWindow::RemoveFrontLine(MESSAGE_TYPE MsgType)
{
	type_vector_msgs* pvecMsgs = GetMsgs(MsgType);

	if (pvecMsgs == NULL)
	{
		assert(!"Empty Message RemoveFrontLine");
		return;
	}

	type_vector_msgs::iterator vi = pvecMsgs->begin();
	if (vi != pvecMsgs->end())
	{
		delete (*vi);
		vi = pvecMsgs->erase(vi);
	}

	if (MsgType == GetCurrentMsgType())
	{
		Scrolling(GetCurrentRenderEndLine());
	}
}

void SEASON3B::CNewUIChatLogWindow::Clear(MESSAGE_TYPE MsgType)
{
	type_vector_msgs* pvecMsgs = GetMsgs(MsgType);
	if (pvecMsgs == NULL)
	{
		assert(!"Empty Message CNewUIChatLogWindow");
		return;
	}

	type_vector_msgs::iterator vi_msg = pvecMsgs->begin();
	for (; vi_msg != pvecMsgs->end(); vi_msg++)
		delete (*vi_msg);
	pvecMsgs->clear();

	if (MsgType == GetCurrentMsgType())
	{
		m_iCurrentRenderEndLine = -1;
	}
}

size_t SEASON3B::CNewUIChatLogWindow::GetNumberOfLines(MESSAGE_TYPE MsgType)
{
	type_vector_msgs* pvecMsgs = GetMsgs(MsgType);
	if (pvecMsgs == NULL)
	{
		return 0;
	}

	return pvecMsgs->size();
}

int SEASON3B::CNewUIChatLogWindow::GetCurrentRenderEndSystemLine() const
{
	return m_iCurrentRenderEndSystemLine;
}

int SEASON3B::CNewUIChatLogWindow::GetCurrentRenderEndLine() const
{
	return m_iCurrentRenderEndLine;
}

void SEASON3B::CNewUIChatLogWindow::Scrolling(int nRenderEndLine)
{
	type_vector_msgs* pvecMsgs = GetMsgs(m_CurrentRenderMsgType);
	if (pvecMsgs == NULL)
	{
		assert(!"Empty message Scrolling");
		return;
	}

	if ((int)pvecMsgs->size() <= m_nShowingLines)
	{
		m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
	}
	else
	{
		if (nRenderEndLine < m_nShowingLines)
			m_iCurrentRenderEndLine = m_nShowingLines - 1;

		else if (nRenderEndLine >= (int)pvecMsgs->size())
			m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
		else
			m_iCurrentRenderEndLine = nRenderEndLine;
	}
}

void SEASON3B::CNewUIChatLogWindow::SetFilterText(const type_string& strFilterText)
{
	bool bPrevFilter = false;

	if (!m_vecFilters.empty())
	{
		bPrevFilter = true;
		ResetFilter();
	}

	unicode::t_char szTemp[MAX_CHAT_BUFFER_SIZE + 1] = { 0, };
	unicode::_strncpy(szTemp, strFilterText.c_str(), MAX_CHAT_BUFFER_SIZE);
	szTemp[MAX_CHAT_BUFFER_SIZE] = '\0';
	unicode::t_char* token = unicode::_strtok(szTemp, " ");
	token = unicode::_strtok(NULL, " ");

	if (token == NULL)
	{
		ResetFilter();
		AddText("", GlobalText[756], TYPE_SYSTEM_MESSAGE);
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			if (NULL == token)
			{
				break;
			}
			AddFilterWord(token);
			token = unicode::_strtok(NULL, " ");
		}

		AddText("", GlobalText[755], TYPE_SYSTEM_MESSAGE);
	}
}

void SEASON3B::CNewUIChatLogWindow::ResetFilter()
{
	m_vecFilters.clear();
}

void SEASON3B::CNewUIChatLogWindow::SetSizeAuto()
{
	SetNumberOfShowingLines(GetNumberOfShowingLines() + 3);
}

void SEASON3B::CNewUIChatLogWindow::SetNumberOfShowingLines(int nShowingLines, OUT LPSIZE lpBoxSize/* = NULL*/)
{
	m_nShowingLines = (int)(nShowingLines / 3) * 3;
	if (m_nShowingLines < 3)
		m_nShowingLines = 3;
#if CLIENT_VERSION == 1002
	if (m_nShowingLines > 24)
#else
	if (m_nShowingLines > 15)
#endif
		m_nShowingLines = 3;

	if (m_nShowingLines > GetCurrentRenderEndLine())
		Scrolling(m_nShowingLines - 1);

	UpdateWndSize();
	UpdateScrollPos();

	if (lpBoxSize)
	{
		lpBoxSize->cx = WND_WIDTH;
		lpBoxSize->cy = (SCROLL_MIDDLE_PART_HEIGHT * GetNumberOfShowingLines()) + (SCROLL_TOP_BOTTOM_PART_HEIGHT * 2) + (WND_TOP_BOTTOM_EDGE * 2);
	}
}

size_t SEASON3B::CNewUIChatLogWindow::GetNumberOfShowingLines() const
{
	return m_nShowingLines;
}

size_t SEASON3B::CNewUIChatLogWindow::GetNumberOfShowingLinesSystem() const
{
	return m_nShowingSystemLines;
}

void SEASON3B::CNewUIChatLogWindow::SetBackAlphaAuto()
{
	if(m_fBackAlpha > 0.95f)
	{
		m_fBackAlpha = 0.7f;
	}
	else if(m_fBackAlpha > 0.55f)
	{
		m_fBackAlpha = 0.5f;
	}
	else if(m_fBackAlpha > 0.35f)
	{
		m_fBackAlpha = 0.3f;
	}
	else if(m_fBackAlpha > 0.05f)
	{
		m_fBackAlpha = 0.0f;
	}
	else
	{
		m_fBackAlpha = 1.0f;
	}
}

void SEASON3B::CNewUIChatLogWindow::SetBackAlpha(float fAlpha)
{
	if (fAlpha < 0.f)
		m_fBackAlpha = 0.f;
	else if (fAlpha > 1.f)
		m_fBackAlpha = 1.f;
	else
		m_fBackAlpha = fAlpha;
}

float SEASON3B::CNewUIChatLogWindow::GetBackAlpha() const
{
	return m_fBackAlpha;
}

void SEASON3B::CNewUIChatLogWindow::ShowFrame()
{
#if CLIENT_VERSION >= 1001
	m_bShowFrame = true;
	SetPosition(GetVoidChatExpandedX(), GetVoidChatExpandedY());
	SetNumberOfShowingLines(m_nShowingLinesBackup);
#else
	m_bShowFrame = true;
#endif

}

void SEASON3B::CNewUIChatLogWindow::HideFrame()
{
#if CLIENT_VERSION >= 1001
	m_bShowFrame = false;
	SetPosition(GetVoidChatClosedX(), GetVoidChatClosedY());
	m_nShowingLinesBackup = m_nShowingLines;
	SetNumberOfShowingLines(9);
#else
	m_bShowFrame = false;
#endif
}

bool SEASON3B::CNewUIChatLogWindow::IsShowFrame()
{
	return m_bShowFrame;
}

bool SEASON3B::CNewUIChatLogWindow::UpdateMouseEvent()
{
	extern float g_fScreenRate_x;

	float offset = 0;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (m_EventState == EVENT_NONE && false == MouseLButtonPush &&
		SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy))
#elif CLIENT_VERSION == 1001
	if ((m_EventState == EVENT_NONE && false == MouseLButtonPush && m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy)) || (m_EventState == EVENT_NONE && false == MouseLButtonPush && !m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, 25, m_WndSize.cx, 110)))
#elif CLIENT_VERSION == 1002
	if ((m_EventState == EVENT_NONE && false == MouseLButtonPush && m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy)) || (m_EventState == EVENT_NONE && false == MouseLButtonPush && !m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, 5, m_WndSize.cx, 120)))
#endif
	{
		m_EventState = EVENT_CLIENT_WND_HOVER;
		return false;
	}

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (false == MouseLButtonPush && m_EventState == EVENT_CLIENT_WND_HOVER &&
		false == SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy))
#elif CLIENT_VERSION == 1001
	if ((false == MouseLButtonPush && m_EventState == EVENT_CLIENT_WND_HOVER && m_bShowFrame &&
		false == SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy)) || (false == MouseLButtonPush && m_EventState == EVENT_CLIENT_WND_HOVER && !m_bShowFrame &&
			false == SEASON3B::CheckMouseIn(m_WndPos.x + offset, 25, m_WndSize.cx, 110)))
#elif CLIENT_VERSION == 1002
	if ((false == MouseLButtonPush && m_EventState == EVENT_CLIENT_WND_HOVER && m_bShowFrame &&
		false == SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy)) || (false == MouseLButtonPush && m_EventState == EVENT_CLIENT_WND_HOVER && !m_bShowFrame &&
			false == SEASON3B::CheckMouseIn(m_WndPos.x + offset, 5, m_WndSize.cx, 120)))
#endif
	{
		m_EventState = EVENT_NONE;
		return true;
	}

	if (m_EventState == EVENT_CLIENT_WND_HOVER)
	{
		if (MouseWheel > 0)
			Scrolling(GetCurrentRenderEndLine() - 1);
		if (MouseWheel < 0)
			Scrolling(GetCurrentRenderEndLine() + 1);
		if (MouseWheel != 0)
			MouseWheel = 0;
	}

	m_bPointedMessage = false;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	if (SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy))
#elif CLIENT_VERSION == 1001
	if ((m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy)) || (!m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, 25, m_WndSize.cx, 110)))
#elif CLIENT_VERSION == 1002
	if ((m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy, m_WndSize.cx, m_WndSize.cy)) || (!m_bShowFrame && SEASON3B::CheckMouseIn(m_WndPos.x + offset, 5, m_WndSize.cx, 120)))
#endif
	{
		int iRenderStartLine = 0;
		if (GetCurrentRenderEndLine() >= m_nShowingLines)
			iRenderStartLine = GetCurrentRenderEndLine() - m_nShowingLines + 1;

		for (int i = iRenderStartLine, s = 0; i <= GetCurrentRenderEndLine(); i++, s++)
		{
			type_vector_msgs* pvecMsgs = GetMsgs(GetCurrentMsgType());
			if (pvecMsgs == NULL)
			{
				return false;
			}

			CMessageText* pMsgText = (*pvecMsgs)[i];

			if (pMsgText->GetType() == TYPE_WHISPER_MESSAGE
				|| pMsgText->GetType() == TYPE_CHAT_MESSAGE
				|| pMsgText->GetType() == TYPE_CHAT_MESSAGE_CUSTOM
				|| pMsgText->GetType() == TYPE_PARTY_MESSAGE
				|| pMsgText->GetType() == TYPE_GUILD_MESSAGE
				|| pMsgText->GetType() == TYPE_UNION_MESSAGE
				|| pMsgText->GetType() == TYPE_GM_MESSAGE
				)
			{




				float fRenderPosX = m_WndPos.x + offset;
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
				float fRenderPosY = m_WndPos.y - m_WndSize.cy + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#elif CLIENT_VERSION == 1001
				float fRenderPosY = m_WndPos.y - gWideScreen.GetBottomPosition(360) + SCROLL_TOP_BOTTOM_PART_HEIGHT;
				if (m_bShowFrame)
					fRenderPosY = m_WndPos.y - m_WndSize.cy + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#elif CLIENT_VERSION == 1002
				float fRenderPosY = m_WndPos.y - gWideScreen.GetBottomPosition(380) + SCROLL_TOP_BOTTOM_PART_HEIGHT;
				if (m_bShowFrame)
					fRenderPosY = m_WndPos.y - m_WndSize.cy + SCROLL_TOP_BOTTOM_PART_HEIGHT;
#endif
				if (GetCurrentRenderEndLine() < m_nShowingLines)
				{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
					fRenderPosY = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * (m_nShowingLines - GetCurrentRenderEndLine() - 1));
#elif CLIENT_VERSION >= 1001
					if (m_bShowFrame) {
						fRenderPosY = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * (m_nShowingLines - GetCurrentRenderEndLine() - 1));
					}
					else {
						fRenderPosY = fRenderPosY + FONT_LEADING + (11 * (m_nShowingLines - GetCurrentRenderEndLine() - 1));
					}
#endif

				}

				POINT ptRenderPos;
				ptRenderPos.x = fRenderPosX + WND_LEFT_RIGHT_EDGE;



#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
				ptRenderPos.y = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * s);
#elif CLIENT_VERSION >= 1001
				if (m_bShowFrame) {
					ptRenderPos.y = fRenderPosY + FONT_LEADING + (SCROLL_MIDDLE_PART_HEIGHT * s);
				}
				else {
					ptRenderPos.y = fRenderPosY + FONT_LEADING + (11 * s);
				}
#endif



#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
				if (SEASON3B::CheckMouseIn(ptRenderPos.x, ptRenderPos.y, WND_WIDTH, SCROLL_MIDDLE_PART_HEIGHT))
#elif CLIENT_VERSION >= 1001
				int height = 11;
				if (m_bShowFrame) {
					height = SCROLL_MIDDLE_PART_HEIGHT;
				}
				if (SEASON3B::CheckMouseIn(ptRenderPos.x, ptRenderPos.y, WND_WIDTH, height))
#endif
				{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
					m_bPointedMessage = true;
#endif
					m_iPointedMessageIndex = i;

					std::string strID = pMsgText->GetID();
					if (SEASON3B::IsPress(VK_RBUTTON) && strID.empty() == false)
					{
						g_pChatInputBox->SetWhsprID(strID.c_str());
					}
				}
			}
		}
	}

	if (m_bShowFrame)
	{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		if (m_EventState == EVENT_CLIENT_WND_HOVER && MouseLButtonPush &&
			SEASON3B::CheckMouseIn(m_ScrollBtnPos.x + offset, m_ScrollBtnPos.y, SCROLL_BTN_WIDTH, SCROLL_BTN_HEIGHT))
#elif CLIENT_VERSION >= 1001
		if (m_EventState == EVENT_CLIENT_WND_HOVER && MouseLButtonPush &&
			SEASON3B::CheckMouseIn(m_ScrollBtnPos.x + offset, m_ScrollBtnPos.y, 9, SCROLL_BTN_HEIGHT))
#endif
		{
			extern int MouseY;

			m_EventState = EVENT_SCROLL_BTN_DOWN;
			m_iGrapRelativePosY = MouseY - m_ScrollBtnPos.y;
			return false;
		}
		if (m_EventState == EVENT_SCROLL_BTN_DOWN)
		{
			if (SEASON3B::IsRepeat(VK_LBUTTON))
			{
				if (GetNumberOfLines(GetCurrentMsgType()) > GetNumberOfShowingLines())
				{
					extern int MouseY;
					if (MouseY - m_iGrapRelativePosY < m_WndPos.y - m_WndSize.cy + WND_TOP_BOTTOM_EDGE)
					{
						Scrolling(GetNumberOfShowingLines() - 1);
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
						m_ScrollBtnPos.y = m_WndPos.y - m_WndSize.cy + WND_TOP_BOTTOM_EDGE;
#elif CLIENT_VERSION >= 1001
						m_ScrollBtnPos.y = m_WndPos.y - m_WndSize.cy + WND_TOP_BOTTOM_EDGE - 5;
#endif
					}
					else if (MouseY - m_iGrapRelativePosY > m_WndPos.y - SCROLL_BTN_HEIGHT - WND_TOP_BOTTOM_EDGE)
					{
						Scrolling(GetNumberOfLines(GetCurrentMsgType()) - 1);

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
						m_ScrollBtnPos.y = m_WndPos.y - SCROLL_BTN_HEIGHT - WND_TOP_BOTTOM_EDGE;
#elif CLIENT_VERSION >= 1001
						m_ScrollBtnPos.y = m_WndPos.y - SCROLL_BTN_HEIGHT - WND_TOP_BOTTOM_EDGE - 5;
#endif
					}
					else
					{

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
						float fScrollRate = (float)((MouseY - m_iGrapRelativePosY) - (m_WndPos.y - m_WndSize.cy + WND_TOP_BOTTOM_EDGE)) / (float)(m_WndSize.cy - WND_TOP_BOTTOM_EDGE * 2 - SCROLL_BTN_HEIGHT);
#elif CLIENT_VERSION >= 1001
						float fScrollRate = (float)((MouseY - m_iGrapRelativePosY) - (m_WndPos.y - m_WndSize.cy + (WND_TOP_BOTTOM_EDGE - 5))) / (float)(m_WndSize.cy - WND_TOP_BOTTOM_EDGE * 2 - SCROLL_BTN_HEIGHT);
#endif
						Scrolling(GetNumberOfShowingLines() + (float)(GetNumberOfLines(GetCurrentMsgType()) - GetNumberOfShowingLines()) * fScrollRate);

						m_ScrollBtnPos.y = MouseY - m_iGrapRelativePosY;
					}
				}
				return false;
			}
			if (SEASON3B::IsRelease(VK_LBUTTON))
			{
				m_EventState = EVENT_NONE;
				return true;
			}
		}

		POINT ptResizingBtn = { m_WndPos.x + offset, m_WndPos.y - m_WndSize.cy - RESIZING_BTN_HEIGHT };
		if (m_EventState == EVENT_NONE && false == MouseLButtonPush &&
			SEASON3B::CheckMouseIn(ptResizingBtn.x, ptResizingBtn.y, RESIZING_BTN_WIDTH, RESIZING_BTN_HEIGHT))
		{
			m_EventState = EVENT_RESIZING_BTN_HOVER;
			return false;
		}
		if (false == MouseLButtonPush && m_EventState == EVENT_RESIZING_BTN_HOVER &&
			false == SEASON3B::CheckMouseIn(ptResizingBtn.x, ptResizingBtn.y, RESIZING_BTN_WIDTH, RESIZING_BTN_HEIGHT))
		{
			m_EventState = EVENT_NONE;
			return true;
		}
		if (m_EventState == EVENT_RESIZING_BTN_HOVER && MouseLButtonPush &&
			SEASON3B::CheckMouseIn(ptResizingBtn.x, ptResizingBtn.y, RESIZING_BTN_WIDTH, RESIZING_BTN_HEIGHT))
		{
			m_EventState = EVENT_RESIZING_BTN_DOWN;
			return false;
		}
		if (m_EventState == EVENT_RESIZING_BTN_DOWN)
		{
			if (MouseLButtonPush)
			{
#if CLIENT_VERSION == 1002
				int nTopSections = (24 - GetNumberOfShowingLines()) / 3;
#else
				int nTopSections = (15 - GetNumberOfShowingLines()) / 3;
#endif
				int nBottomSections = (GetNumberOfShowingLines() - 3) / 3;
				for (int i = 0; i < nTopSections; i++)
				{
					if (SEASON3B::CheckMouseIn(0, ptResizingBtn.y - RESIZING_BTN_HEIGHT - ((i + 1) * SCROLL_MIDDLE_PART_HEIGHT * 3 * 2),
						gWideScreen.g_WideWindowWidth, SCROLL_MIDDLE_PART_HEIGHT * 3 + RESIZING_BTN_HEIGHT))
					{
						SetNumberOfShowingLines(GetNumberOfShowingLines() + (i + 1) * 3);
						return false;
					}
				}
				for (int i = 0; i < nBottomSections; i++)
				{
					if (SEASON3B::CheckMouseIn(0, ptResizingBtn.y + RESIZING_BTN_HEIGHT + ((i + 1) * SCROLL_MIDDLE_PART_HEIGHT * 3),
						gWideScreen.g_WideWindowWidth, RESIZING_BTN_HEIGHT + SCROLL_MIDDLE_PART_HEIGHT * 3))
					{
						SetNumberOfShowingLines(GetNumberOfShowingLines() - (i + 1) * 3);
						return false;
					}
				}
#if CLIENT_VERSION == 1002
				if (SEASON3B::CheckMouseIn(0, 0, gWideScreen.g_WideWindowWidth, m_WndPos.y - (SCROLL_MIDDLE_PART_HEIGHT * 24 + RESIZING_BTN_HEIGHT + SCROLL_TOP_BOTTOM_PART_HEIGHT * 2)))
				{
					SetNumberOfShowingLines(24);
				}
#else
				if (SEASON3B::CheckMouseIn(0, 0, gWideScreen.g_WideWindowWidth, m_WndPos.y - (SCROLL_MIDDLE_PART_HEIGHT * 15 + RESIZING_BTN_HEIGHT + SCROLL_TOP_BOTTOM_PART_HEIGHT * 2)))
				{
					SetNumberOfShowingLines(15);
				}
#endif

				if (SEASON3B::CheckMouseIn(0, m_WndPos.y - (SCROLL_MIDDLE_PART_HEIGHT * 3 + SCROLL_TOP_BOTTOM_PART_HEIGHT * 2),
					gWideScreen.g_WideWindowWidth, SCROLL_MIDDLE_PART_HEIGHT * 3 + SCROLL_TOP_BOTTOM_PART_HEIGHT * 2))
				{
					SetNumberOfShowingLines(3);
				}
				return false;
			}
			if (false == MouseLButtonPush || true == MouseLButtonPop)
			{
				m_EventState = EVENT_NONE;
				return true;
			}
		}
	}
	return true;
}

bool SEASON3B::CNewUIChatLogWindow::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIChatLogWindow::Update()
{
	UpdateScrollPos();

	return true;
}

bool SEASON3B::CNewUIChatLogWindow::Render()
{
	if (SceneFlag == CHARACTER_SCENE && IsQuitGameNoticeActive())
	{
		HideFrame();
		return true;
	}

	if (RenderBackground() == false)
	{
		return false;
	}


	if (m_bShowChatLog == true)
	{
		if (RenderMessages() == false || RenderSystemMessages() == false)
		{
			return false;
		}
	}

	if (RenderFrame() == false)
	{
		return false;
	}

	return true;
}

float SEASON3B::CNewUIChatLogWindow::GetLayerDepth()
{
	return 6.1f;
}

float SEASON3B::CNewUIChatLogWindow::GetKeyEventOrder()
{
	return 8.0f;
}

bool SEASON3B::CNewUIChatLogWindow::CheckChatRedundancy(const type_string& strText, int iSearchLine/* = 1*/)
{
	type_vector_msgs* pvecMsgs = GetMsgs(TYPE_ALL_MESSAGE);
	if (pvecMsgs == NULL)
	{
		return 0;
	}

	if (pvecMsgs->empty()) return false;
	type_vector_msgs::reverse_iterator vri_msgs = pvecMsgs->rbegin();
	for (int i = 0; (i < iSearchLine) || (vri_msgs != pvecMsgs->rend()); vri_msgs++, i++)
		if (0 == (*vri_msgs)->GetText().compare(strText)) return true;
	return false;
}

void SEASON3B::CNewUIChatLogWindow::SeparateText(IN const type_string& strID, IN const type_string& strText, OUT type_string& strText1, OUT type_string& strText2)
{
	extern float g_fScreenRate_x;

	SIZE TextSize;
	type_string strIDPart = strID + ": ";
	std::wstring wstrUTF16 = L"";

	g_pMultiLanguage->ConvertCharToWideStr(wstrUTF16, strIDPart.c_str());
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), wstrUTF16.c_str(), wstrUTF16.length(), &TextSize);
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	size_t MaxFirstLineWidth = CLIENT_WIDTH - (size_t)(TextSize.cx / g_fScreenRate_x);
#elif CLIENT_VERSION >= 1001
	size_t MaxFirstLineWidth = (CLIENT_WIDTH - 20) - (size_t)(TextSize.cx / g_fScreenRate_x);
#endif
	g_pMultiLanguage->ConvertCharToWideStr(wstrUTF16, strText.c_str());
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), wstrUTF16.c_str(), wstrUTF16.length(), &TextSize);

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	BOOL bSpaceExist = (wstrUTF16.find_last_of(L" ") != std::wstring::npos) ? TRUE : FALSE;
#elif CLIENT_VERSION >= 1001
	BOOL bSpaceExist = FALSE;
#endif
	int iLocToken = wstrUTF16.length();

	while (((size_t)(TextSize.cx / g_fScreenRate_x) > MaxFirstLineWidth) && (iLocToken > -1))
	{
		iLocToken = (bSpaceExist) ? wstrUTF16.find_last_of(L" ", iLocToken - 1) : iLocToken - 1;

		g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), (wstrUTF16.substr(0, iLocToken)).c_str(), iLocToken, &TextSize);
	}

	g_pMultiLanguage->ConvertWideCharToStr(strText1, (wstrUTF16.substr(0, iLocToken)).c_str(), CP_UTF8);
	g_pMultiLanguage->ConvertWideCharToStr(strText2, (wstrUTF16.substr(iLocToken, wstrUTF16.length() - iLocToken)).c_str(), CP_UTF8);

}

bool SEASON3B::CNewUIChatLogWindow::CheckFilterText(const type_string& strTestText)
{
	type_vector_filters::iterator vi_filters = m_vecFilters.begin();
	for (; vi_filters != m_vecFilters.end(); vi_filters++)
	{
		if (FindText(strTestText.c_str(), (*vi_filters).c_str()) == true)
		{
			return true;
		}
	}

	return false;
}

const POINT& SEASON3B::CNewUIChatLogWindow::GetPos()
{
	return m_WndPos;
}

const SIZE& SEASON3B::CNewUIChatLogWindow::GetSize()
{
	return m_WndSize;
}

void SEASON3B::CNewUIChatLogWindow::UpdateWndSize()
{
	m_WndSize.cx = WND_WIDTH;
	m_WndSize.cy = (SCROLL_MIDDLE_PART_HEIGHT * GetNumberOfShowingLines()) + (SCROLL_TOP_BOTTOM_PART_HEIGHT * 2) + (WND_TOP_BOTTOM_EDGE * 2);
}

void SEASON3B::CNewUIChatLogWindow::UpdateScrollPos()
{
	float fPosRate = 1.f;

	if (GetNumberOfLines(GetCurrentMsgType()) > GetNumberOfShowingLines())
	{
		if ((int)GetNumberOfShowingLines() > GetCurrentRenderEndLine())
		{
			fPosRate = 0.f;
		}
		else
		{
			fPosRate = (float)(GetCurrentRenderEndLine() + 1 - GetNumberOfShowingLines()) / (float)(GetNumberOfLines(GetCurrentMsgType()) - GetNumberOfShowingLines());
		}
	}
	if (m_EventState != EVENT_SCROLL_BTN_DOWN)
	{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		m_ScrollBtnPos.x = m_WndPos.x + m_WndSize.cx - SCROLL_BAR_WIDTH - WND_LEFT_RIGHT_EDGE - 4;
#elif CLIENT_VERSION >= 1001
		m_ScrollBtnPos.x = m_WndPos.x + m_WndSize.cx - SCROLL_BAR_WIDTH - WND_LEFT_RIGHT_EDGE - 3;
#endif

		m_ScrollBtnPos.y = m_WndPos.y - m_WndSize.cy + WND_TOP_BOTTOM_EDGE + ((float)(m_WndSize.cy - SCROLL_BTN_HEIGHT - WND_TOP_BOTTOM_EDGE * 2) * fPosRate);
	}
}

void SEASON3B::CNewUIChatLogWindow::AddFilterWord(const type_string& strWord)
{
	if (m_vecFilters.size() > 5)
		return;

	type_vector_filters::iterator vi_filters = m_vecFilters.begin();
	for (; vi_filters != m_vecFilters.end(); vi_filters++)
	{
		if (0 == (*vi_filters).compare(strWord))
		{
			return;
		}
	}

	m_vecFilters.push_back(strWord);
}

void SEASON3B::CNewUIChatLogWindow::ClearAll()
{
	for (int i = TYPE_ALL_MESSAGE; i < NUMBER_OF_TYPES; i++)
	{
		Clear((MESSAGE_TYPE)i);
	}

	m_iCurrentRenderEndLine = -1;
}

SEASON3B::CNewUIChatLogWindow::type_vector_msgs* SEASON3B::CNewUIChatLogWindow::GetMsgs(MESSAGE_TYPE MsgType)
{
	switch (MsgType)
	{
	case TYPE_ALL_MESSAGE:
		return &m_vecAllMsgs;
	case TYPE_CHAT_MESSAGE:
		return &m_VecChatMsgs;
	case TYPE_CHAT_MESSAGE_CUSTOM:
		return &m_VecChatMsgsCustom;
	case TYPE_WHISPER_MESSAGE:
		return &m_vecWhisperMsgs;
	case TYPE_SYSTEM_MESSAGE:
		return &m_VecSystemMsgs;
	case TYPE_ERROR_MESSAGE:
		return &m_vecErrorMsgs;
	case TYPE_PARTY_MESSAGE:
		return &m_vecPartyMsgs;
	case TYPE_GUILD_MESSAGE:
		return &m_vecGuildMsgs;
	case TYPE_UNION_MESSAGE:
		return &m_vecUnionMsgs;
	case TYPE_GM_MESSAGE:
		return &m_vecGMMsgs;
	}

	return NULL;
}

void SEASON3B::CNewUIChatLogWindow::ChangeMessage(MESSAGE_TYPE MsgType)
{
	m_CurrentRenderMsgType = MsgType;

	type_vector_msgs* pvecMsgs = GetMsgs(GetCurrentMsgType());
	if (pvecMsgs == NULL)
	{
		return;
	}

	m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
}

SEASON3B::MESSAGE_TYPE SEASON3B::CNewUIChatLogWindow::GetCurrentMsgType() const
{
	return m_CurrentRenderMsgType;
}

void SEASON3B::CNewUIChatLogWindow::ShowChatLog()
{
	m_bShowChatLog = true;

	type_vector_msgs* pvecMsgs = GetMsgs(GetCurrentMsgType());
	if (pvecMsgs == NULL)
	{
		return;
	}

	m_iCurrentRenderEndLine = pvecMsgs->size() - 1;
}

void SEASON3B::CNewUIChatLogWindow::HideChatLog()
{
	m_bShowChatLog = false;
}
