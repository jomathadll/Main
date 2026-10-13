//*****************************************************************************
// File: LoginWin.cpp
//*****************************************************************************

#include "stdafx.h"
#include "LoginWin.h"
#include "Input.h"
#include "UIMng.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "UIControls.h"
#include "ZzzScene.h"
#include "wsclientinline.h"
#include "DSPlaySound.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include "ProtocolSend.h"
#include "ServerListManager.h"
#include "Reconnect.h"
#include "WideScreen.h"

#define	LIW_ACCOUNT		0
#define	LIW_PASSWORD	1

#define LIW_OK			0
#define LIW_CANCEL		1

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
extern int g_iChatInputType;

CLoginWin::CLoginWin()
{
	m_pIDInputBox = NULL;
	m_pPassInputBox = NULL;
}

CLoginWin::~CLoginWin()
{
	SAFE_DELETE(m_pIDInputBox);
	SAFE_DELETE(m_pPassInputBox);
}

void CLoginWin::Create()
{
	CWin::Create(gWideScreen.g_WideWindowWidth, gWideScreen.g_WideWindowHeight, -2);

	m_asprInputBox[LIW_ACCOUNT].Create(130, 18, BITMAP_LOG_IN + 8);
	m_asprInputBox[LIW_PASSWORD].Create(130, 18, BITMAP_LOG_IN + 8);

	for (int i = 0; i < 2; ++i)
	{
		//m_aBtn[i].Create(60, 18, -1);
		m_aBtn[i].Create(60, 18, -1);
		CWin::RegisterButton(&m_aBtn[i]);
	}

	SAFE_DELETE(m_pIDInputBox);

	m_pIDInputBox = new CUITextInputBox;
	m_pIDInputBox->Init(g_hWnd, 130, 18, MAX_ID_SIZE);
	m_pIDInputBox->SetBackColor(0, 0, 0, 255);
	m_pIDInputBox->SetTextColor(255, 255, 230, 210);
	m_pIDInputBox->SetFont(g_hFixFont);
	m_pIDInputBox->SetState(UISTATE_NORMAL);
	m_pIDInputBox->SetText(m_ID);

	SAFE_DELETE(m_pPassInputBox);

	m_pPassInputBox = new CUITextInputBox;
	m_pPassInputBox->Init(g_hWnd, 130, 18, MAX_PASSWORD_SIZE, TRUE);
	m_pPassInputBox->SetBackColor(0, 0, 0, 25);
	m_pPassInputBox->SetTextColor(255, 255, 230, 210);
	m_pPassInputBox->SetFont(g_hFixFont);
	m_pPassInputBox->SetState(UISTATE_NORMAL);
	m_pPassInputBox->SetText(m_PASS);//AUTOLOGIN

	m_pIDInputBox->SetTabTarget(m_pPassInputBox);
	m_pPassInputBox->SetTabTarget(m_pIDInputBox);
	
	this->FirstLoad = 1;
}

void CLoginWin::PreRelease()
{
	for (int i = 0; i < 2; ++i)
		m_asprInputBox[i].Release();
}

void CLoginWin::SetPosition(int nXCoord, int nYCoord)
{
	CWin::SetPosition(nXCoord, nYCoord);

	float generalPosY = gWideScreen.GetBottomPosition(250);

	//SELECT CHAR LOGIN E SENHA
	m_asprInputBox[LIW_ACCOUNT].SetPosition((gWideScreen.g_WideWindowWidth / 2) - 50, generalPosY + 30);
	m_asprInputBox[LIW_PASSWORD].SetPosition((gWideScreen.g_WideWindowWidth / 2) - 50, generalPosY + 60);

	if (g_iChatInputType == 1)
	{
		m_pIDInputBox->SetPosition((gWideScreen.g_WideWindowWidth / 2) - 45, generalPosY + 34);

		m_pPassInputBox->SetPosition((gWideScreen.g_WideWindowWidth / 2) - 45, generalPosY + 65);
	}

	int buttonPosY = generalPosY + 90;

	m_aBtn[LIW_OK].SetPosition((gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY);
	m_aBtn[LIW_CANCEL].SetPosition((gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY);

}

void CLoginWin::Show(bool bShow)
{
	CWin::Show(bShow);

	for (int i = 0; i < 2; ++i)
	{
		m_asprInputBox[i].Show(bShow);
		m_aBtn[i].Show(bShow);


	}
	
}

bool CLoginWin::CursorInWin(int nArea)
{
	if (!CWin::m_bShow)
		return false;

	switch (nArea)
	{
	case WA_MOVE:
		return false;
	}

	return CWin::CursorInWin(nArea);
}

void CLoginWin::UpdateWhileActive(double dDeltaTick)
{
	CInput& rInput = CInput::Instance();
	
	int buttonPosY = gWideScreen.GetBottomPosition(250) + 90;
	
	if (::CheckMouseIn((gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY, 60, 18) && rInput.IsLBtnUp())
		RequestLogin();
	else if (::CheckMouseIn((gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY, 60, 18) && rInput.IsLBtnUp())
		CancelLogin();
	else if (CInput::Instance().IsKeyDown(VK_RETURN))
	{
		::PlayBuffer(SOUND_CLICK01);
		RequestLogin();
	}
	else if (CInput::Instance().IsKeyDown(VK_ESCAPE))
	{
		::PlayBuffer(SOUND_CLICK01);
		CancelLogin();
		CUIMng::Instance().SetSysMenuWinShow(false);
	}
}

void CLoginWin::UpdateWhileShow(double dDeltaTick)
{
	m_pIDInputBox->DoAction();
	m_pPassInputBox->DoAction();

	CInput& rInput = CInput::Instance();

	int buttonPosY = gWideScreen.GetBottomPosition(250) + 90;

	if (::CheckMouseIn((gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY, 60, 18) && rInput.IsLBtnUp())
		RequestLogin();
	else if (::CheckMouseIn((gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY, 60, 18) && rInput.IsLBtnUp())
		CancelLogin();
	else if (CInput::Instance().IsKeyDown(VK_RETURN))
	{
		::PlayBuffer(SOUND_CLICK01);
		RequestLogin();
	}
	else if (CInput::Instance().IsKeyDown(VK_ESCAPE))
	{
		::PlayBuffer(SOUND_CLICK01);
		CancelLogin();
		CUIMng::Instance().SetSysMenuWinShow(false);
	}
}

void CLoginWin::RenderControls()
{

	if (this->FirstLoad == 1)
	{
		if (strlen(m_ID) > 0)
			CUIMng::Instance().m_LoginWin.GetPassInputBox()->GiveFocus();
		else
			CUIMng::Instance().m_LoginWin.GetIDInputBox()->GiveFocus();
		this->FirstLoad = 0;
	}

	float generalPosY = gWideScreen.GetBottomPosition(250);

	SEASON3B::RenderImageScaleAuto(BITMAP_LOG_IN + 7, (gWideScreen.g_WideWindowWidth / 2) - 100, generalPosY, 200, 137);

	::RenderBitmap(BITMAP_LOG_IN + 8, (gWideScreen.g_WideWindowWidth / 2) - 50, generalPosY+30, 130, 18, 0.001, 0.001, 0.640, 0.820956, 1, 1, 1.0);
	::RenderBitmap(BITMAP_LOG_IN + 8, (gWideScreen.g_WideWindowWidth / 2) - 50, generalPosY+60, 130, 18, 0.001, 0.001, 0.640, 0.825956, 1, 1, 1.0);

	//CWin::RenderButtons();

	int buttonPosY = generalPosY+90;

	if (CheckMouseIn((gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY, 60, 18))
	{
		::RenderBitmap(BITMAP_BUTTON + 2, (gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY, 60, 18, 0.002, 0.213, 0.95, 0.20, 1, 1, 1.0);
	}
	else {
		::RenderBitmap(BITMAP_BUTTON + 2, (gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY, 60, 18, 0.002, 0.002, 0.95, 0.20, 1, 1, 1.0);
	}

	if (CheckMouseIn((gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY, 60, 18))
	{
		::RenderBitmap(BITMAP_BUTTON + 2, (gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY, 60, 18, 0.002, 0.213, 0.95, 0.20, 1, 1, 1.0);
	}
	else {
		::RenderBitmap(BITMAP_BUTTON + 2, (gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY, 60, 18, 0.002, 0.002, 0.95, 0.20, 1, 1, 1.0);
	}

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(237, 214, 161, 255);
	g_pRenderText->RenderText((gWideScreen.g_WideWindowWidth / 2) - 70, buttonPosY + ((18 / 2) - 5), GlobalText[452], 60, 0, 3);
	g_pRenderText->RenderText((gWideScreen.g_WideWindowWidth / 2) + 12, buttonPosY + ((18 / 2) - 5), GlobalText[455], 60, 0, 3);

	//for (int i = 0; i < 2; ++i)
	//	m_asprInputBox[i].Render();

	m_pIDInputBox->Render();
	m_pPassInputBox->Render();

	g_pRenderText->SetFont(g_hFixFont);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->RenderText((gWideScreen.g_WideWindowWidth / 2) - 80, generalPosY+35, GlobalText[450]);
	g_pRenderText->RenderText((gWideScreen.g_WideWindowWidth / 2) - 80, generalPosY+65, GlobalText[451]);

	unicode::t_char szServerName[MAX_TEXT_LENGTH];

	const char* apszGlobalText[4] = { GlobalText[461], GlobalText[460], GlobalText[3130], GlobalText[3131] };
	sprintf(szServerName, "%s", g_ServerListManager->GetSelectServerName());

	g_pRenderText->RenderText((gWideScreen.g_WideWindowWidth / 2) - 100, generalPosY+8, szServerName, 200, 0, 3);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
}

void CLoginWin::RequestLogin()
{
	if (CurrentProtocolState == REQUEST_JOIN_SERVER)
		return;

	CUIMng::Instance().HideWin(this);

	char szID[MAX_ID_SIZE+1] = { 0, };
	char szPass[MAX_PASSWORD_SIZE+1] = {0, };
	m_pIDInputBox->GetText(szID, MAX_ID_SIZE+1);
	m_pPassInputBox->GetText(szPass, MAX_PASSWORD_SIZE+1);
	
	if (unicode::_strlen(szID) <= 0)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_INPUT_ID);
	else if (unicode::_strlen(szPass) <= 0)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_INPUT_PASSWORD);
	else
	{
		if (CurrentProtocolState == RECEIVE_JOIN_SERVER_SUCCESS)
		{
			memcpy(g_pReconnect->s_Data.ReconnectAccount, szID, 11);  //Add
			memcpy(g_pReconnect->s_Data.ReconnectPassword, szPass, 11);  //Add

			g_ConsoleDebug->Write( MCD_NORMAL, "Login with the following account: %s", szID);

			g_ErrorReport.Write("> Login Request.\r\n");
			g_ErrorReport.Write("> Try to Login \"%s\"\r\n", szID);
			// SendRequestLogIn()

			#ifdef NEW_PROTOCOL_SYSTEM
				gProtocolSend.SendRequestLogInNew(szID, szPass);
			#else
				SendRequestLogIn(szID, szPass);
			#endif

			SaveLoginInfo(szID, szPass);
		}
	}
}

void CLoginWin::CancelLogin()
{
	ConnectConnectionServer();
	CUIMng::Instance().HideWin(this);
}

void CLoginWin::ConnectConnectionServer()
{
	#ifdef NEW_PROTOCOL_SYSTEM
		gProtocolSend.DisconnectServer();
	#endif

	LogIn = 0;
	CurrentProtocolState = REQUEST_JOIN_SERVER;
    CreateSocket(szServerIpAddress, g_ServerPort);
}