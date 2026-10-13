

#include "stdafx.h"
#include "NewUIChatInputBox.h"
#include "NewUIChatLogWindow.h"
#include "UIControls.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "CSChaosCastle.h"
#include "ZzzOpenData.h"
#include "ZzzTexture.h"
#include "ZzzScene.h"
#include "ZzzInterface.h"
#include "MapManager.h"
#include "CSPetSystem.h"
using namespace SEASON3B;

namespace
{
	const int SOURCE_CHAT_IMAGE_BACK = 51530;
	const int SOURCE_CHAT_IMAGE_WHISPER = 51532;

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
	const float SOURCE_CHAT_BAR_Y = 25.5f;
	const float SOURCE_CHAT_BAR_HEIGHT = 19.0f;
	const float SOURCE_CHAT_MAIN_TEXT_X = 5.0f;
	const float SOURCE_CHAT_MAIN_TEXT_Y = 29.0f;
	const float SOURCE_CHAT_MAIN_TEXT_WIDTH = 150.0f;
	const float SOURCE_CHAT_WHISPER_X = 170.0f;
	const float SOURCE_CHAT_WHISPER_WIDTH = 45.0f;
	const float SOURCE_CHAT_BUTTON_Y = 31.0f;
	const float SOURCE_CHAT_BUTTON_WHISPER_Y = 28.0f;
	const float SOURCE_CHAT_BUTTON_SIZE = 16.0f;
	const float SOURCE_CHAT_BUTTON_WHISPER_SIZE = 14.0f;
	const float SOURCE_CHAT_BUTTON_CHAT_X = 96.0f;
	const float SOURCE_CHAT_BUTTON_SIZE_X = 113.0f;
	const float SOURCE_CHAT_BUTTON_ALPHA_X = 130.0f;
	const float SOURCE_CHAT_BUTTON_WHISPER_X = 223.0f;

	enum ESlashChatAliasType
	{
		SLASH_CHAT_ALIAS_NONE = 0,
		SLASH_CHAT_ALIAS_CHANNEL,
		SLASH_CHAT_ALIAS_REPLY_WHISPER,
	};

	bool IsSlashAliasSpace(wchar_t ch)
	{
		return ch == L' ' || ch == L'\t';
	}

	wchar_t ToLowerSlashAliasChar(wchar_t ch)
	{
		if (ch >= L'A' && ch <= L'Z')
		{
			return ch + (L'a' - L'A');
		}

		return ch;
	}

	bool MatchSlashAliasCommand(const std::wstring& text, const wchar_t* command, size_t& messageOffset)
	{
		const size_t commandLength = wcslen(command);
		if (text.length() <= commandLength)
		{
			return false;
		}

		for (size_t i = 0; i < commandLength; ++i)
		{
			if (ToLowerSlashAliasChar(text[i]) != command[i])
			{
				return false;
			}
		}

		if (!IsSlashAliasSpace(text[commandLength]))
		{
			return false;
		}

		messageOffset = commandLength + 1;
		while (messageOffset < text.length() && IsSlashAliasSpace(text[messageOffset]))
		{
			++messageOffset;
		}

		return messageOffset < text.length();
	}

	ESlashChatAliasType ApplySlashChatAlias(std::wstring& text)
	{
		struct SSlashChatAlias
		{
			const wchar_t* Command;
			const wchar_t* Prefix;
		};

		static const SSlashChatAlias ChatAliases[] =
		{
			{ L"/g", L"@" },
			{ L"/ally", L"@@" },
			{ L"/pt", L"~" },
		};

		for (int i = 0; i < (int)(sizeof(ChatAliases) / sizeof(ChatAliases[0])); ++i)
		{
			size_t messageOffset = 0;
			if (MatchSlashAliasCommand(text, ChatAliases[i].Command, messageOffset))
			{
				text = ChatAliases[i].Prefix + text.substr(messageOffset);
				return SLASH_CHAT_ALIAS_CHANNEL;
			}
		}

		size_t messageOffset = 0;
		if (MatchSlashAliasCommand(text, L"/r", messageOffset))
		{
			text = text.substr(messageOffset);
			return SLASH_CHAT_ALIAS_REPLY_WHISPER;
		}

		return SLASH_CHAT_ALIAS_NONE;
	}

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

	void RenderSourceChatField(float x, float y, float width, float height, bool focused)
	{
		RenderSourceChatColor(x, y, width, height, 0.010f, 0.010f, 0.010f, focused ? 0.82f : 0.64f);
		RenderSourceChatBorder(x, y, width, height,
			focused ? SOURCE_CHAT_HOVER_R : SOURCE_CHAT_BORDER_R,
			focused ? SOURCE_CHAT_HOVER_G : SOURCE_CHAT_BORDER_G,
			focused ? SOURCE_CHAT_HOVER_B : SOURCE_CHAT_BORDER_B,
			focused ? 0.92f : 0.58f);
	}


	void RenderSourceChatIconButton(float x, float y, int iconType, bool active, bool hover)
	{
		const float size = SOURCE_CHAT_BUTTON_SIZE;
		RenderSourceChatColor(x, y, size, size, active ? 0.170f : 0.025f, active ? 0.120f : 0.022f, active ? 0.055f : 0.019f, 0.88f);
		RenderSourceChatBorder(x, y, size, size,
			(active || hover) ? SOURCE_CHAT_HOVER_R : SOURCE_CHAT_BORDER_R,
			(active || hover) ? SOURCE_CHAT_HOVER_G : SOURCE_CHAT_BORDER_G,
			(active || hover) ? SOURCE_CHAT_HOVER_B : SOURCE_CHAT_BORDER_B,
			(active || hover) ? 0.88f : 0.58f);
		RenderSourceChatColor(x + 2.0f, y + 2.0f, size - 4.0f, 1.0f, 0.54f, 0.50f, 0.40f, 0.20f);

		const float ir = active ? SOURCE_CHAT_HOVER_R : 0.56f;
		const float ig = active ? SOURCE_CHAT_HOVER_G : 0.52f;
		const float ib = active ? SOURCE_CHAT_HOVER_B : 0.42f;
		switch (iconType)
		{
		case 0:
			RenderSourceChatBorder(x + 4.0f, y + 5.0f, 7.0f, 5.0f, ir, ig, ib, 0.88f);
			RenderSourceChatColor(x + 5.0f, y + 10.0f, 3.0f, 1.0f, ir, ig, ib, 0.70f);
			break;
		case 1:
			RenderSourceChatColor(x + 7.0f, y + 3.0f, 1.0f, 9.0f, ir, ig, ib, 0.78f);
			RenderSourceChatColor(x + 5.0f, y + 4.0f, 5.0f, 1.0f, ir, ig, ib, 0.78f);
			RenderSourceChatColor(x + 5.0f, y + 11.0f, 5.0f, 1.0f, ir, ig, ib, 0.78f);
			RenderSourceChatColor(x + 6.0f, y + 5.0f, 3.0f, 1.0f, ir, ig, ib, 0.54f);
			RenderSourceChatColor(x + 6.0f, y + 10.0f, 3.0f, 1.0f, ir, ig, ib, 0.54f);
			break;
		case 2:
			RenderSourceChatBorder(x + 4.0f, y + 4.0f, 7.0f, 6.0f, ir, ig, ib, 0.82f);
			RenderSourceChatBorder(x + 6.0f, y + 6.0f, 7.0f, 6.0f, ir, ig, ib, 0.58f);
			break;
		case 3:
			RenderSourceChatColor(x + 4.0f, y + 4.0f, 7.0f, 7.0f, ir, ig, ib, 0.30f);
			RenderSourceChatBorder(x + 4.0f, y + 4.0f, 7.0f, 7.0f, ir, ig, ib, 0.78f);
			break;
		}
	}
	void RenderVoidChatButton(int texture, float x, float y, float size, bool active, bool hover)
	{
		const float u = 0.008f;
		const float uWidth = 0.51488634f;
		const float stateHeight = 0.263782f;
		float v = 0.0f;
		if (active || (hover && (GetKeyState(VK_LBUTTON) & 0x8000) != 0))
		{
			v = 0.527564f;
		}
		else if (hover)
		{
			v = stateHeight;
		}

		RenderBitmap(texture, x, y, size, size,
			u, v, uWidth, stateHeight, true, true, 1.0f);
	}

	void RenderSourceChatButton(float x, float y, float width, float height, bool active, bool hover, const char* label)
	{
		RenderSourceChatColor(x, y, width, height,
			active ? 0.205f : SOURCE_CHAT_DARK_R,
			active ? 0.145f : SOURCE_CHAT_DARK_G,
			active ? 0.062f : SOURCE_CHAT_DARK_B,
			0.88f);
		RenderSourceChatBorder(x, y, width, height,
			(active || hover) ? SOURCE_CHAT_HOVER_R : SOURCE_CHAT_BORDER_R,
			(active || hover) ? SOURCE_CHAT_HOVER_G : SOURCE_CHAT_BORDER_G,
			(active || hover) ? SOURCE_CHAT_HOVER_B : SOURCE_CHAT_BORDER_B,
			(active || hover) ? 0.92f : 0.54f);
		EndRenderColor();

		g_pRenderText->SetFont(g_hFont);
		g_pRenderText->SetBgColor(0);
		g_pRenderText->SetTextColor(active ? 255 : 222, active ? 235 : 213, active ? 190 : 196, 255);
		g_pRenderText->RenderText(x, y + 4.0f, label, width, 0, RT3_SORT_CENTER);
	}
}

SEASON3B::CNewUIChatInputBox::CNewUIChatInputBox() : MAX_CHAT_SIZE_UTF16((int)(MAX_CHAT_SIZE/(g_pMultiLanguage->GetNumByteForOneCharUTF8())))

{ 
	Init(); 
}

SEASON3B::CNewUIChatInputBox::~CNewUIChatInputBox() 
{ 
	Release(); 
}

void SEASON3B::CNewUIChatInputBox::Init()
{
	m_pNewUIMng = NULL;
	m_pNewUIChatLogWnd = NULL;
	m_WndPos.x = m_WndPos.y = 0;
	m_WndSize.cx = m_WndSize.cy = 0;
	m_pChatInputBox = m_pWhsprIDInputBox = NULL;
	m_iCurChatHistory = m_iCurWhisperIDHistory = 0;

	m_iTooltipType = INPUT_TOOLTIP_NOTHING;
	m_iInputMsgType = INPUT_CHAT_MESSAGE;
	m_bBlockWhisper = false;
	m_bOnlySystemMessage = false;
	m_bShowChatLog = true;
	m_bWhisperSend = true;
#if CLIENT_VERSION >= 1001
	m_ShowSettingsPanel = false;
#endif

	m_bShowMessageElseNormal = false;
}

void SEASON3B::CNewUIChatInputBox::LoadImages()
{
	LoadBitmap("Custom\\Interface\\newui_chat_back.tga", SOURCE_CHAT_IMAGE_BACK, GL_LINEAR, GL_REPEAT, false);
	LoadBitmap("Custom\\Interface\\btnWhisper.jpg", SOURCE_CHAT_IMAGE_WHISPER, GL_LINEAR, GL_REPEAT, false);
}

void SEASON3B::CNewUIChatInputBox::UnloadImages()
{
	DeleteBitmap(SOURCE_CHAT_IMAGE_BACK);
	DeleteBitmap(SOURCE_CHAT_IMAGE_WHISPER);
}

bool SEASON3B::CNewUIChatInputBox::Create(CNewUIManager* pNewUIMng, CNewUIChatLogWindow* pNewUIChatLogWnd, int x, int y)
{
	Release();
	
	if(NULL == pNewUIChatLogWnd || NULL == pNewUIMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHATINPUTBOX, this);
	
	m_pNewUIChatLogWnd = pNewUIChatLogWnd;
	SetWndPos(x, y);

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_pChatInputBox = new CUITextInputBox;
	m_pChatInputBox->SetPosition(m_WndPos.x + 72, m_WndPos.y + 32);
	m_pChatInputBox->SetTextColor(255, 255, 230, 210);
	m_pChatInputBox->SetBackColor(0, 0, 0, 0);
	m_pChatInputBox->SetFont(g_hFont);
	m_pChatInputBox->SetState(UISTATE_HIDE);

	m_pWhsprIDInputBox = new CUITextInputBox;
	m_pWhsprIDInputBox->SetPosition(m_WndPos.x + 5, m_WndPos.y + 32);
	m_pWhsprIDInputBox->SetTextColor(255, 200, 200, 200);
	m_pWhsprIDInputBox->SetBackColor(0, 0, 0, 0);
	m_pWhsprIDInputBox->SetFont(g_hFont);
	m_pWhsprIDInputBox->SetState(UISTATE_HIDE);
#elif CLIENT_VERSION >= 1001
	m_pChatInputBox = new CUITextInputBox;
	m_pChatInputBox->Init(g_hWnd, (int)SOURCE_CHAT_MAIN_TEXT_WIDTH, 14, MAX_CHAT_SIZE_UTF16 - 1);
	m_pChatInputBox->SetPosition(m_WndPos.x + (int)SOURCE_CHAT_MAIN_TEXT_X, m_WndPos.y + (int)SOURCE_CHAT_MAIN_TEXT_Y);
	m_pChatInputBox->SetSize((int)SOURCE_CHAT_MAIN_TEXT_WIDTH, 14);
	m_pChatInputBox->SetTextColor(255, 255, 230, 210);
	m_pChatInputBox->SetBackColor(0, 0, 0, 0);
	m_pChatInputBox->SetFont(g_hFont);
	m_pChatInputBox->SetState(UISTATE_HIDE);

	m_pWhsprIDInputBox = new CUITextInputBox;
	m_pWhsprIDInputBox->Init(g_hWnd, (int)SOURCE_CHAT_WHISPER_WIDTH, 14, 10);
	m_pWhsprIDInputBox->SetPosition(m_WndPos.x + (int)SOURCE_CHAT_WHISPER_X, m_WndPos.y + (int)SOURCE_CHAT_MAIN_TEXT_Y);
	m_pWhsprIDInputBox->SetSize((int)SOURCE_CHAT_WHISPER_WIDTH, 14);
	m_pWhsprIDInputBox->SetTextColor(255, 200, 200, 200);
	m_pWhsprIDInputBox->SetBackColor(0, 0, 0, 0);
	m_pWhsprIDInputBox->SetFont(g_hFont);
	m_pWhsprIDInputBox->SetState(UISTATE_HIDE);
#endif
	

	m_pChatInputBox->SetTabTarget(m_pWhsprIDInputBox);
	m_pWhsprIDInputBox->SetTabTarget(m_pChatInputBox);

	LoadImages();

	SetInputMsgType(INPUT_CHAT_MESSAGE);

	SetButtonInfo();

	Show(false);
	
	return true;
}

void SEASON3B::CNewUIChatInputBox::Release()
{
	UnloadImages();
	
	RemoveAllChatHIstory();
	RemoveAllWhsprIDHIstory();

	SAFE_DELETE(m_pChatInputBox);
	SAFE_DELETE(m_pWhsprIDInputBox);

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
	
	Init();
}

void SEASON3B::CNewUIChatInputBox::SetButtonInfo()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_BtnSize.ChangeButtonImgState(true, IMAGE_INPUTBOX_BTN_SIZE, false);
	m_BtnSize.ChangeButtonInfo(m_WndPos.x + 200, m_WndPos.y, 27, 26);

	m_BtnTransparency.ChangeButtonImgState(true, IMAGE_INPUTBOX_BTN_TRANSPARENCY, false);
	m_BtnTransparency.ChangeButtonInfo(m_WndPos.x + 227, m_WndPos.y, 27, 26);
#elif CLIENT_VERSION >= 1001
	m_BtnSize.ChangeButtonImgState(true, IMAGE_INPUTBOX_BTN_SIZE, false);
	m_BtnSize.ChangeButtonInfo(m_WndPos.x + 198.5f, m_WndPos.y + 6.5f, 17, 17);

	m_BtnTransparency.ChangeButtonImgState(true, IMAGE_INPUTBOX_BTN_TRANSPARENCY, false);
	m_BtnTransparency.ChangeButtonInfo(m_WndPos.x + 217.f, m_WndPos.y + 6.5f, 17, 17);



#endif

}

void SEASON3B::CNewUIChatInputBox::SetWndPos(int x, int y)
{
	m_WndPos.x = x; m_WndPos.y = y;
	m_WndSize.cx = WND_WIDTH; m_WndSize.cy = WND_HEIGHT;

	if(m_pChatInputBox && m_pWhsprIDInputBox)
	{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		m_pChatInputBox->SetPosition(m_WndPos.x+72, m_WndPos.y+32);
		m_pWhsprIDInputBox->SetPosition(m_WndPos.x+5, m_WndPos.y+32);
#elif CLIENT_VERSION >= 1001
		m_pChatInputBox->SetPosition(m_WndPos.x + (int)SOURCE_CHAT_MAIN_TEXT_X, m_WndPos.y + (int)SOURCE_CHAT_MAIN_TEXT_Y);
		m_pWhsprIDInputBox->SetPosition(m_WndPos.x + (int)SOURCE_CHAT_WHISPER_X, m_WndPos.y + (int)SOURCE_CHAT_MAIN_TEXT_Y);
#endif
	}
}

void SEASON3B::CNewUIChatInputBox::SetInputMsgType(int iInputMsgType)
{
	m_iInputMsgType = iInputMsgType;
}

int SEASON3B::CNewUIChatInputBox::GetInputMsgType() const 
{ 
	return m_iInputMsgType; 
}

void SEASON3B::CNewUIChatInputBox::SetFont(HFONT hFont)
{
	m_pChatInputBox->SetFont(hFont);
	m_pWhsprIDInputBox->SetFont(hFont);
}

bool SEASON3B::CNewUIChatInputBox::HaveFocus() 
{ 
	return (m_pChatInputBox->HaveFocus() || m_pWhsprIDInputBox->HaveFocus()); 
}

void SEASON3B::CNewUIChatInputBox::AddChatHistory(const type_string& strText)
{
	type_vec_history::iterator vi = std::find(m_vecChatHistory.begin(), m_vecChatHistory.end(), strText);
	if(vi != m_vecChatHistory.end())
		m_vecChatHistory.erase(vi);
	else if(m_vecChatHistory.size() > 12)
		m_vecChatHistory.erase(m_vecChatHistory.begin());
	m_vecChatHistory.push_back(strText);
}

void SEASON3B::CNewUIChatInputBox::RemoveChatHistory(int index)
{
	if(index >= 0 && index < (int)m_vecChatHistory.size())
		m_vecChatHistory.erase(m_vecChatHistory.begin() + index);
}

void SEASON3B::CNewUIChatInputBox::RemoveAllChatHIstory()
{
	m_vecChatHistory.clear();
}

void SEASON3B::CNewUIChatInputBox::AddWhsprIDHistory(const type_string& strWhsprID)
{
	type_vec_history::iterator vi = std::find(m_vecWhsprIDHistory.begin(), m_vecWhsprIDHistory.end(), strWhsprID);
	if(vi != m_vecWhsprIDHistory.end())
		m_vecWhsprIDHistory.erase(vi);
	else if(m_vecWhsprIDHistory.size() > 5)
		m_vecWhsprIDHistory.erase(m_vecWhsprIDHistory.begin());
	m_vecWhsprIDHistory.push_back(strWhsprID);
}

void SEASON3B::CNewUIChatInputBox::RemoveWhsprIDHistory(int index)
{
	if(index >= 0 && index < (int)m_vecWhsprIDHistory.size())
	{
		m_vecWhsprIDHistory.erase(m_vecWhsprIDHistory.begin() + index);
	}
}

void SEASON3B::CNewUIChatInputBox::RemoveAllWhsprIDHIstory()
{
	m_vecWhsprIDHistory.clear();
}

bool SEASON3B::CNewUIChatInputBox::IsBlockWhisper()
{
	return m_bBlockWhisper;
}

void SEASON3B::CNewUIChatInputBox::SetBlockWhisper(bool bBlockWhisper)
{
	m_bBlockWhisper = bBlockWhisper;
}

bool SEASON3B::CNewUIChatInputBox::UpdateMouseEvent()
{
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX) == false)
	{
		return true;
	}

	if(IsVisible() && SEASON3B::IsRelease(VK_RBUTTON))
	{
		if(SelectedCharacter >= 0)
		{
			CHARACTER* pCha = &CharactersClient[SelectedCharacter];
			if(pCha->Object.Kind == KIND_PLAYER && !gMapManager.InChaosCastle() && !(::IsStrifeMap(gMapManager.WorldActive) && Hero->m_byGensInfluence != pCha->m_byGensInfluence))
			{
				SetWhsprID(pCha->ID);
			}
		}
	}

	m_iTooltipType = INPUT_TOOLTIP_NOTHING;

	const float whisperButtonY = m_WndPos.y + SOURCE_CHAT_BUTTON_WHISPER_Y;
	const bool bWhisperButton = CheckMouseIn(m_WndPos.x + SOURCE_CHAT_BUTTON_WHISPER_X, whisperButtonY, SOURCE_CHAT_BUTTON_WHISPER_SIZE, SOURCE_CHAT_BUTTON_WHISPER_SIZE) == true;

	if(bWhisperButton)
	{
		m_iTooltipType = INPUT_TOOLTIP_WHISPER;
		if(SEASON3B::IsRelease(VK_LBUTTON))
		{
			m_bWhisperSend = !m_bWhisperSend;
			if(m_bWhisperSend)
			{
				m_pWhsprIDInputBox->SetState(UISTATE_NORMAL);
				m_pWhsprIDInputBox->GiveFocus();
			}
			else
			{
				m_pWhsprIDInputBox->SetState(UISTATE_HIDE);
				m_pChatInputBox->GiveFocus();
			}
			PlayBuffer(SOUND_CLICK01);
			return false;
		}
	}

	if(CheckMouseIn(m_WndPos.x, m_WndPos.y, m_WndSize.cx, m_WndSize.cy) == true)
	{
		return false;
	}

	return true;
}

bool SEASON3B::CNewUIChatInputBox::UpdateKeyEvent()
{
	if(SEASON3B::IsPress(VK_F2))
	{
		m_bShowMessageElseNormal = !m_bShowMessageElseNormal;
		
		if(m_bShowMessageElseNormal)
		{
			m_pNewUIChatLogWnd->ChangeMessage( SEASON3B::TYPE_WHISPER_MESSAGE );
		}
		else
		{
			m_pNewUIChatLogWnd->ChangeMessage( SEASON3B::TYPE_ALL_MESSAGE );
		}

		PlayBuffer(SOUND_CLICK01);
		return false;
	}
	
	if(SEASON3B::IsPress(VK_F3))
	{
		if(m_bWhisperSend == false)
		{
			m_bWhisperSend = true;
			m_pWhsprIDInputBox->SetState(UISTATE_NORMAL);
		}
		else
		{
			m_bWhisperSend = false;
			m_pWhsprIDInputBox->SetState(UISTATE_HIDE);
			if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
			{
				m_pChatInputBox->GiveFocus();
			}
		}

		return false;
	}

	if(m_pNewUIChatLogWnd->IsShowFrame())
	{
		if(SEASON3B::IsPress(VK_F4))
		{	
			m_pNewUIChatLogWnd->SetSizeAuto();
			m_pNewUIChatLogWnd->UpdateWndSize();
			m_pNewUIChatLogWnd->UpdateScrollPos();
			PlayBuffer(SOUND_CLICK01);	
			return false;
		}
	}

	if(SEASON3B::IsPress(VK_F5))
	{
		if (SceneFlag == CHARACTER_SCENE)
		{
			return false;
		}

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX))
#endif
		{
			if(m_pNewUIChatLogWnd->IsShowFrame())
			{
				m_pNewUIChatLogWnd->HideFrame();
			}
			else
			{
				m_pNewUIChatLogWnd->ShowFrame();
			}
		}
		
		PlayBuffer(SOUND_CLICK01);
		return false;
	}
	
	if(m_pNewUIChatLogWnd->IsShowFrame())
	{
		if(IsPress(VK_PRIOR))
		{
			m_pNewUIChatLogWnd->Scrolling(m_pNewUIChatLogWnd->GetCurrentRenderEndLine() - m_pNewUIChatLogWnd->GetNumberOfShowingLines());
			return false;
		}
		if(IsPress(VK_NEXT))
		{
			m_pNewUIChatLogWnd->Scrolling(m_pNewUIChatLogWnd->GetCurrentRenderEndLine() + m_pNewUIChatLogWnd->GetNumberOfShowingLines());
			return false;
		}	
	}

	if( false == IsVisible() && SEASON3B::IsPress(VK_RETURN) )
	{
		if(gMapManager.InChaosCastle() == true && g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHAOSCASTLE_TIME) == false)
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_CHATINPUTBOX);
		}
		else if(gMapManager.InChaosCastle() == false)
		{
			g_pNewUISystem->Show(SEASON3B::INTERFACE_CHATINPUTBOX);
		}

		RestoreIMEStatus();
		return false;
	}

	if(IsVisible() && HaveFocus() && SEASON3B::IsPress(VK_RETURN))
	{
		char	szChatText[MAX_CHAT_SIZE+1]	= {'\0'};
		char	szWhisperID[MAX_ID_SIZE+1]	= {'\0'};		
		wchar_t *szReceivedChat = new wchar_t[MAX_CHAT_SIZE_UTF16];
		
		m_pChatInputBox->GetText(szReceivedChat, MAX_CHAT_SIZE_UTF16);
		m_pWhsprIDInputBox->GetText(szWhisperID, MAX_ID_SIZE+1);

		for (int i = 0; i < MAX_CHAT_SIZE_UTF16; i++)
			szReceivedChat[i] = g_pMultiLanguage->ConvertFulltoHalfWidthChar(szReceivedChat[i]);

		std::wstring wstrText = L"";
		
		if (szReceivedChat[0] != 0x002F)
		{
			switch(m_iInputMsgType) {
			case INPUT_PARTY_MESSAGE:
				wstrText = L"~";
				break;
			case INPUT_GUILD_MESSAGE:
				wstrText = L"@";
				break;
			default:
				break;
			}
		}
		wstrText.append(szReceivedChat);
		ESlashChatAliasType slashChatAlias = ApplySlashChatAlias(wstrText);

		delete [] szReceivedChat;

		std::string strText;

		g_pMultiLanguage->ConvertWideCharToStr(strText, wstrText.c_str(), g_pMultiLanguage->GetCodePage());
		strncpy(szChatText, strText.c_str(), sizeof szChatText);
		
		if(unicode::_strlen(szChatText) != 0)
		{	
			char localCommand[32] = { 0 };
			int localCommandSize = 0;
			for (int n = 0; n < 31 && szChatText[n] != 0 && szChatText[n] != 0x20; ++n)
			{
				localCommand[n] = szChatText[n];
				localCommandSize = n + 1;
			}
			localCommand[localCommandSize] = 0;

			if (slashChatAlias == SLASH_CHAT_ALIAS_NONE && _stricmp(localCommand, "/noxydev") == 0)
			{
				ShowDarkRavenSpeedTunerCentered();
			}
			else if(slashChatAlias != SLASH_CHAT_ALIAS_NONE || !CheckCommand(szChatText))
			{
				{
					if(CheckAbuseFilter(szChatText))
					{
						g_pMultiLanguage->ConvertCharToWideStr(wstrText, GlobalText[570]);
					}
					
					if(slashChatAlias == SLASH_CHAT_ALIAS_REPLY_WHISPER)
					{
						char replyWhisperID[MAX_ID_SIZE+1] = {'\0'};
						const char* lastWhisperID = GetLastWhisperReplyID();
						if (lastWhisperID != NULL && lastWhisperID[0] != NULL)
						{
							strncpy(replyWhisperID, lastWhisperID, MAX_ID_SIZE);
							replyWhisperID[MAX_ID_SIZE] = NULL;
						}
						else if (strlen(szWhisperID) > 0)
						{
							strncpy(replyWhisperID, szWhisperID, MAX_ID_SIZE);
							replyWhisperID[MAX_ID_SIZE] = NULL;
						}

						if (replyWhisperID[0] == NULL)
						{
							g_pChatListBox->AddText("", "Nenhum whisper para responder.", SEASON3B::TYPE_ERROR_MESSAGE);
						}
						else
						{
							g_pMultiLanguage->ConvertWideCharToStr(strText, wstrText.c_str(), CP_UTF8);
							strncpy(szChatText, strText.c_str(), sizeof szChatText);
							szChatText[MAX_CHAT_SIZE] = NULL;
							SendChatWhisper(replyWhisperID, szChatText);
							g_pChatListBox->AddText(Hero->ID, szChatText, SEASON3B::TYPE_WHISPER_MESSAGE);
							AddWhsprIDHistory(replyWhisperID);
							SetWhsprID(replyWhisperID);
							AddChatHistory(szChatText);
						}
					}
					else if(slashChatAlias != SLASH_CHAT_ALIAS_CHANNEL && m_pWhsprIDInputBox->GetState() == UISTATE_NORMAL && unicode::_strlen(szChatText) && strlen(szWhisperID) > 0 )
					{
						g_pMultiLanguage->ConvertWideCharToStr(strText, wstrText.c_str(), CP_UTF8);
						strncpy(szChatText, strText.c_str(), sizeof szChatText);
						SendChatWhisper(szWhisperID, szChatText);
						g_pChatListBox->AddText(Hero->ID, szChatText, SEASON3B::TYPE_WHISPER_MESSAGE);
						AddWhsprIDHistory(szWhisperID);
					}
					else if (strncmp(szChatText, GlobalText[260], strlen(GlobalText[260])) == 0)
					{
						char* pszMapName = szChatText + strlen(GlobalText[260])+1;
						int iMapIndex = g_pMoveCommandWindow->GetMapIndexFromMovereq(pszMapName);

						if (g_pMoveCommandWindow->IsTheMapInDifferentServer(gMapManager.WorldActive, iMapIndex))
						{
							SaveOptions();
						}
						
						SendRequestMoveMap(g_pMoveCommandWindow->GetMoveCommandKey(), iMapIndex);
					}
					else
					{	
						if ( Hero->SafeZone || (Hero->Helper.Type != MODEL_HELPER+2 && Hero->Helper.Type != MODEL_HELPER+3 && Hero->Helper.Type != MODEL_HELPER+4 && Hero->Helper.Type != MODEL_HELPER+37) ) 
						{
							CheckChatText(szChatText);
						}
						
						g_pMultiLanguage->ConvertWideCharToStr(strText, wstrText.c_str(), CP_UTF8);
						strncpy(szChatText, strText.c_str(), sizeof szChatText);
						SendChat(szChatText);
						AddChatHistory(szChatText);
					}
				}
			}

			m_pChatInputBox->SetText("");
			m_iCurChatHistory = m_iCurWhisperIDHistory = 0;

			SaveIMEStatus();
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHATINPUTBOX);

		}
		else {
			m_pChatInputBox->SetText("");
			m_iCurChatHistory = m_iCurWhisperIDHistory = 0;

			SaveIMEStatus();
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHATINPUTBOX);
		}
		
		
		return false;
	}
	if(IsVisible() && m_pChatInputBox->HaveFocus())
	{
		if(SEASON3B::IsPress(VK_UP) && false == m_vecChatHistory.empty())
		{
			m_iCurChatHistory--;
			if(m_iCurChatHistory < 0)
				m_iCurChatHistory = m_vecChatHistory.size()-1;
			m_pChatInputBox->SetText(m_vecChatHistory[m_iCurChatHistory].c_str());

			return false;
		}
		else if(SEASON3B::IsPress(VK_DOWN) && false == m_vecChatHistory.empty())
		{
			m_iCurChatHistory++;

			if(m_iCurChatHistory >= (int)m_vecChatHistory.size())
				m_iCurChatHistory = 0;

			m_pChatInputBox->SetText(m_vecChatHistory[m_iCurChatHistory].c_str());

			return false;
		}
	}
	
	if(IsVisible() && m_pWhsprIDInputBox->HaveFocus() 
		&& m_pWhsprIDInputBox->GetState() == UISTATE_NORMAL)
	{
		if(SEASON3B::IsPress(VK_UP) && false == m_vecWhsprIDHistory.empty())
		{
			m_iCurWhisperIDHistory--;
			if(m_iCurWhisperIDHistory < 0)
				m_iCurWhisperIDHistory = m_vecWhsprIDHistory.size()-1;
			m_pWhsprIDInputBox->SetText(m_vecWhsprIDHistory[m_iCurWhisperIDHistory].c_str());

			return false;
		}
		else if(SEASON3B::IsPress(VK_DOWN) && false == m_vecWhsprIDHistory.empty())
		{
			m_iCurWhisperIDHistory++;

			if(m_iCurWhisperIDHistory >= (int)m_vecWhsprIDHistory.size())
				m_iCurWhisperIDHistory = 0;

			m_pWhsprIDInputBox->SetText(m_vecWhsprIDHistory[m_iCurWhisperIDHistory].c_str());

			return false;
		}
	}
	if(g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_CHATINPUTBOX) == true)
	{
		if(SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_CHATINPUTBOX);

			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIChatInputBox::Update()
{
	if(m_pChatInputBox->HaveFocus() && GetRelatedWnd() != m_pChatInputBox->GetHandle())
	{
		SetRelatedWnd(m_pChatInputBox->GetHandle());
	}
	if(m_pWhsprIDInputBox->HaveFocus() && GetRelatedWnd() != m_pWhsprIDInputBox->GetHandle())
	{
		SetRelatedWnd(m_pWhsprIDInputBox->GetHandle());
	}
	if(!HaveFocus() && GetRelatedWnd() != g_hWnd)
	{
		SetRelatedWnd(g_hWnd);
	}

	if(IsVisible() == false)
	{
		return true;
	}
	
	m_pChatInputBox->DoAction();
	
	if(m_pWhsprIDInputBox->GetState() == UISTATE_NORMAL)
	{
		m_pWhsprIDInputBox->DoAction();
	}

	return true;
}

bool SEASON3B::CNewUIChatInputBox::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderFrame();
	RenderButtons();
	RenderTooltip();

	m_pChatInputBox->Render();
	m_pWhsprIDInputBox->Render();

	DisableAlphaBlend();
	
	return true;
}

bool SEASON3B::CNewUIChatInputBox::RenderFrame()
{
	const float x = (float)m_WndPos.x;
	const float y = (float)m_WndPos.y;
	const float barY = y + SOURCE_CHAT_BAR_Y;

	EnableAlphaTest(1);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderBitmap(SOURCE_CHAT_IMAGE_BACK, x, barY, (float)WND_WIDTH, SOURCE_CHAT_BAR_HEIGHT,
		0.002f, 0.002f, 0.5464978f, 0.52934978f, true, true, 1.0f);

	if(m_bWhisperSend == true)
	{
		RenderSourceChatColor(x + 167.5f, y + 28.5f, 54.0f, 13.0f, 0.0f, 0.0f, 0.0f, 0.58f);
		EndRenderColor();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	return true;
}

void SEASON3B::CNewUIChatInputBox::RenderButtons()
{
	const float x = (float)m_WndPos.x;
	const float whisperY = (float)m_WndPos.y + SOURCE_CHAT_BUTTON_WHISPER_Y;
	const bool bWhisperHover = CheckMouseIn(m_WndPos.x + SOURCE_CHAT_BUTTON_WHISPER_X, whisperY, SOURCE_CHAT_BUTTON_WHISPER_SIZE, SOURCE_CHAT_BUTTON_WHISPER_SIZE) == true;

	EnableAlphaTest(1);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	RenderVoidChatButton(SOURCE_CHAT_IMAGE_WHISPER, x + SOURCE_CHAT_BUTTON_WHISPER_X, whisperY, SOURCE_CHAT_BUTTON_WHISPER_SIZE, m_bWhisperSend, bWhisperHover);
}

void SEASON3B::CNewUIChatInputBox::RenderTooltip()
{
	if(m_iTooltipType == INPUT_TOOLTIP_NOTHING)
		return;

	unicode::t_char strTooltip[256];

	const int iTextIndex[9] = { 1681, 1682, 1683, 1684, 1685, 750, 1686, 751, 752};

	unicode::_sprintf(strTooltip, "%s", GlobalText[iTextIndex[m_iTooltipType]]);

	SIZE Fontsize;
	g_pRenderText->SetFont(g_hFont);
	g_pMultiLanguage->_GetTextExtentPoint32(g_pRenderText->GetFontDC(), strTooltip, unicode::_strlen(strTooltip), &Fontsize);

	Fontsize.cx = Fontsize.cx / ((float)WindowWidth / 640);
	Fontsize.cy = Fontsize.cy / ((float)WindowHeight / 480);

	float fCenterX = (float)m_WndPos.x + SOURCE_CHAT_BUTTON_CHAT_X + (SOURCE_CHAT_BUTTON_SIZE / 2.0f);
	switch(m_iTooltipType)
	{
	case INPUT_TOOLTIP_SIZE:
		fCenterX = (float)m_WndPos.x + SOURCE_CHAT_BUTTON_SIZE_X + (SOURCE_CHAT_BUTTON_SIZE / 2.0f);
		break;
	case INPUT_TOOLTIP_TRANSPARENCY:
		fCenterX = (float)m_WndPos.x + SOURCE_CHAT_BUTTON_ALPHA_X + (SOURCE_CHAT_BUTTON_SIZE / 2.0f);
		break;
	case INPUT_TOOLTIP_WHISPER:
		fCenterX = (float)m_WndPos.x + SOURCE_CHAT_BUTTON_WHISPER_X + (SOURCE_CHAT_BUTTON_WHISPER_SIZE / 2.0f);
		break;
	default:
		break;
	}

	int x = (int)(fCenterX - (Fontsize.cx / 2));
	if(x < 0) x = 0;
	int y = m_WndPos.y + (int)SOURCE_CHAT_BAR_Y - (Fontsize.cy + 1);

	RenderTipText(x, y, strTooltip);
}

float SEASON3B::CNewUIChatInputBox::GetLayerDepth() 
{
	return 6.2f; 
}

float SEASON3B::CNewUIChatInputBox::GetKeyEventOrder() 
{ 
	return 9.0f; 
}

void SEASON3B::CNewUIChatInputBox::OpenningProcess()
{
	m_pChatInputBox->GiveFocus();
	m_pChatInputBox->SetState(UISTATE_NORMAL);
	m_pChatInputBox->SetText("");

	if(m_bWhisperSend == true)
	{
		m_pWhsprIDInputBox->SetState(UISTATE_NORMAL);
	}
	else
	{
		m_pWhsprIDInputBox->SetState(UISTATE_HIDE);
	}
}

void SEASON3B::CNewUIChatInputBox::ClosingProcess()
{
#if CLIENT_VERSION>= 401 && CLIENT_VERSION <= 502
	if (m_pNewUIChatLogWnd->IsShowFrame())
	{
		m_pNewUIChatLogWnd->HideFrame();
	}
#endif

	m_pChatInputBox->SetState(UISTATE_HIDE);
	m_pWhsprIDInputBox->SetState(UISTATE_HIDE);

	SetFocus(g_hWnd);
}

void SEASON3B::CNewUIChatInputBox::GetChatText(type_string& strText)
{
	unicode::t_char szChatText[256];
	m_pChatInputBox->GetText(szChatText, 256);
	strText = szChatText;
}
void SEASON3B::CNewUIChatInputBox::GetWhsprID(type_string& strWhsprID)
{
	char szWhisperID[32];
	m_pWhsprIDInputBox->GetText(szWhisperID, 32);
	strWhsprID = szWhisperID;
}

void SEASON3B::CNewUIChatInputBox::SetWhsprID(const char* strWhsprID)
{
	m_pWhsprIDInputBox->SetText(strWhsprID);
}

void SEASON3B::CNewUIChatInputBox::SetTextPosition(int x, int y) 
{ 
	m_pChatInputBox->SetPosition(x, y); 
}

void SEASON3B::CNewUIChatInputBox::SetBuddyPosition(int x, int y) 
{ 
	m_pWhsprIDInputBox->SetPosition(x, y); 
}
