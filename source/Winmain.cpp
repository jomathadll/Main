///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <locale.h>
#include <zmouse.h>
#include "UIWindows.h"
#include "UIManager.h"
#include "ZzzOpenglUtil.h"
#include "ZzzTexture.h"
#include "ZzzOpenData.h"
#include "ZzzScene.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "ZzzInventory.h"
#include "zzzLodTerrain.h"
#include "DSPlaySound.h"
#include "wsclientinline.h"
#include "Resource.h"
#include <imm.h>
#include "zzzpath.h"
#include "Nprotect.h"
#include "Local.h"
#include "PersonalShopTitleImp.h"
#include "./Utilities/Log/ErrorReport.h"
#include "UIMapName.h"		// rozy
#include "./ExternalObject/leaf/ExceptionHandler.h"
#include "./Utilities/Dump/CrashReporter.h"
#include "./Utilities/Log/muConsoleDebug.h"
#include "ProtocolSend.h"
#include "ProtectSysKey.h"
#include "CBTMessageBox.h"
#include "./ExternalObject/leaf/regkey.h"

#include "CSChaosCastle.h"
#include "GMHellas.h"
#include <io.h>
#include "Input.h"
#include "./Time/Timer.h"


#include "UIMng.h"

#ifdef MOVIE_DIRECTSHOW
#include <dshow.h>
#include "MovieScene.h"
#endif // MOVIE_DIRECTSHOW
#include "GameCensorship.h"

#include "w_MapHeaders.h"

#include "w_PetProcess.h"

#include "MultiLanguage.h"
#include "WideScreen.h"
#include "HelperSystem.h"
#include "ItemManager.h"
#include "ItemPosition.h"
#include "Descriptions.h"
#include "VisualDescriptions.h"
#include "DarkSpirit.h"
#include "DisableExcellent.h"
#include "CustomBow.h"
#include "CustomWing.h"
#include "CustomItemFloor.h"
#include "CustomItemForce.h"
#include "CustomEffects.h"
#include "CustomSetEffect.h"
#include "RenderModel.h"
#include "ServerName.h"
#include "CreateFont.h"
#include "NewSounds.h"
#include "MessageColor.h"
#include "Controller.h"
#include "Camera3D.h"
#include "Monsters.h"
#include "MonsterName.h"
#include "MonsterGlow.h"
#include "MonsterEffect.h"
#include "CustomCape.h"
#include "CustomItemEdge.h"
#include "CustomRingTransform.h"
#include "CustomJewelStack.h"
#include "DrawBarItem.h"
#include "CounterItem.h"
#include "CustomMeshGlow.h"
#include "CharacterList.h"
#include "BuffColorBK.h"
#include "BuffColorSM.h"
#include "PlasmaStormColor.h"
#include "CustomEarthQuakeSkill.h"
#include "RuneColorEffect.h"
#include "CustomSetAction.h"
#include "DeathStabColor.h"
#include "CustomEye.h"
#include "MonsterItem.h"
#include "SummonEffect.h"
#include "CustomTitleItem.h"
#include "Charactername.h"
#include "CustomColorChat.h"
#include "Patente.h"
#include "CustomModelAction.h"
#include "WindowTray.h"
#include "MoveWindow.h"
#include "ItemBorder.h"
#include <glew.h>
#include "AutoPotion.h"

CUIMercenaryInputBox * g_pMercenaryInputBox = NULL;
CUITextInputBox * g_pSingleTextInputBox = NULL;
CUITextInputBox * g_pSinglePasswdInputBox = NULL;
int g_iChatInputType = 1;
extern BOOL g_bIMEBlock;

CChatRoomSocketList * g_pChatRoomSocketList = NULL;

CMultiLanguage *pMultiLanguage = NULL;

extern DWORD g_dwTopWindow;

#ifdef MOVIE_DIRECTSHOW
CMovieScene*	g_pMovieScene = NULL;
#endif // MOVIE_DIRECTSHOW

CUIManager* g_pUIManager = NULL;
CUIMapName* g_pUIMapName = NULL;		// rozy

int Time_Effect = 0;
bool ashies = false;
int weather = rand()%3;

HWND      g_hWnd  = NULL;
HINSTANCE g_hInst = NULL;
HDC       g_hDC   = NULL;
HGLRC     g_hRC   = NULL;
HFONT     g_hFont = NULL;
HFONT     g_hFontBold = NULL;
HFONT     g_hFontBig = NULL;
HFONT     g_hFixFont = NULL;
HFONT     g_hFontSmall = NULL;
HFONT     g_hFontMedium = NULL;
HFONT     g_hFontMediumBold = NULL;
HFONT     g_hFontBigBold = NULL;

CTimer*		g_pTimer = NULL;	// performance counter.
bool      Destroy = false;
bool      ActiveIME = false;

BYTE*				RendomMemoryDump;
ITEM_ATTRIBUTE*		ItemAttRibuteMemoryDump;
CHARACTER*			CharacterMemoryDump;

int       RandomTable[100];

CErrorReport g_ErrorReport;

BOOL g_bMinimizedEnabled = FALSE;
int g_iScreenSaverOldValue = 60*15;

extern float g_fScreenRate_x;	// ¡Ø
extern float g_fScreenRate_y;

#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
BOOL g_bUseWindowMode = TRUE;
#endif

char Mp3FileName[256];

#pragma comment(lib, "wzAudio.lib")
#include <wzAudio.h>

//char IP_ADDRES_CONFIG[32] = "IP"; // MU ONLINE

void StopMp3(char *Name, BOOL bEnforce)
{
    if(!m_MusicOnOff && !bEnforce) return;

	if(Mp3FileName[0] != NULL)
	{
		if(strcmp(Name, Mp3FileName) == 0) {
			wzAudioStop();
			Mp3FileName[0] = NULL;
		}
	}
}

void PlayMp3(char *Name, BOOL bEnforce )
{
	if(Destroy) return;
    if(!m_MusicOnOff && !bEnforce) return;

	if(strcmp(Name,Mp3FileName) == 0) 
	{
		return;
	}
	else 
	{
		wzAudioPlay(Name, 1);
		strcpy(Mp3FileName,Name);
	}
}


bool IsEndMp3()
{
	if (100 == wzAudioGetStreamOffsetRange())
		return true;
	return false;
}

int GetMp3PlayPosition()
{
	return wzAudioGetStreamOffsetRange();
}

extern int  LogIn;
extern char LogInID[];

void CheckHack( void)
{
	#ifdef NEW_PROTOCOL_SYSTEM
		gProtocolSend.SendCheckOnline();
	#else
		SendCheck();
	#endif
}

GLvoid KillGLWindow(GLvoid)								
{
	if (g_hRC)
	{
		if (!wglMakeCurrent(NULL,NULL))
		{
			g_ErrorReport.Write( "GL - Release Of DC And RC Failed\r\n");
			MessageBox(NULL,"Release Of DC And RC Failed.","Error",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(g_hRC))
		{
			g_ErrorReport.Write( "GL - Release Rendering Context Failed\r\n");
			MessageBox(NULL,"Release Rendering Context Failed.","Error",MB_OK | MB_ICONINFORMATION);
		}

		g_hRC=NULL;
	}

	if (g_hDC && !ReleaseDC(g_hWnd,g_hDC))
	{
		g_ErrorReport.Write( "GL - OpenGL Release Error\r\n");
		MessageBox(NULL,"OpenGL Release Error.","Error",MB_OK | MB_ICONINFORMATION);
		g_hDC=NULL;
	}

#if (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
#else
#ifdef ENABLE_FULLSCREEN
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
#endif	// USER_WINDOW_MODE
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}
#endif //ENABLE_FULLSCREEN
#endif	//WINDOWMODE(#else)
}


BOOL GetFileNameOfFilePath( char *lpszFile, char *lpszPath)
{
	int iFind = ( int)'\\';
	char *lpFound = lpszPath;
	char *lpOld = lpFound;
	while ( lpFound)
	{
		lpOld = lpFound;
		lpFound = strchr( lpFound + 1, iFind);
	}

	if ( strchr( lpszPath, iFind))
	{
		strcpy( lpszFile, lpOld + 1);
	}
	else
	{
		strcpy( lpszFile, lpOld);
	}

	BOOL bCheck = TRUE;
	for ( char *lpTemp = lpszFile; bCheck; ++lpTemp)
	{
		switch ( *lpTemp)
		{
		case '\"':
		case '\\':
		case '/':
		case ' ':
			*lpTemp = '\0';
		case '\0':
			bCheck = FALSE;
			break;
		}
	}

	return ( TRUE);
}

HANDLE g_hMainExe = INVALID_HANDLE_VALUE;

BOOL OpenMainExe( void)
{
#ifdef _DEBUG
	return ( TRUE);
#endif
	char lpszFile[MAX_PATH];
char *lpszCommandLine = GetCommandLine();
	GetFileNameOfFilePath( lpszFile, lpszCommandLine);

	g_hMainExe = CreateFile( ( char*)lpszFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, 0);
	
	return ( INVALID_HANDLE_VALUE != g_hMainExe);
}

void CloseMainExe( void)
{
	CloseHandle( g_hMainExe);
}

WORD DecryptCheckSumKey( WORD wSource)
{
	WORD wAcc = wSource ^ 0xB479;
	return ( ( wAcc >> 10) << 4) | ( wAcc & 0xF);
}

DWORD GenerateCheckSum( BYTE *pbyBuffer, DWORD dwSize, WORD wKey)
{
	DWORD dwKey = ( DWORD)wKey;
	DWORD dwResult = dwKey << 9;
	for ( DWORD dwChecked = 0; dwChecked <= dwSize - 4; dwChecked += 4)
	{
		DWORD dwTemp;
		memcpy( &dwTemp, pbyBuffer + dwChecked, sizeof ( DWORD));

		switch ( ( dwChecked / 4 + wKey) % 3)
		{
		case 0:
			dwResult ^= dwTemp;
			break;
		case 1:
			dwResult += dwTemp;
			break;
		case 2:
			dwResult <<= ( dwTemp % 11);
			dwResult ^= dwTemp;
			break;
		}

		if ( 0 == ( dwChecked % 4))
		{
			dwResult ^= ( ( dwKey + dwResult) >> ( ( dwChecked / 4) % 16 + 3));
		}
	}

	return ( dwResult);
}

DWORD GetCheckSum( WORD wKey)
{
	wKey = DecryptCheckSumKey( wKey);

	char lpszFile[MAX_PATH];

	strcpy( lpszFile, "data\\local\\Gameguard.csr");

	HANDLE hFile = CreateFile( ( char*)lpszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if ( INVALID_HANDLE_VALUE == hFile)
	{
		return ( 0);
	}
	
	DWORD dwSize = GetFileSize( hFile, NULL);
	BYTE *pbyBuffer = new BYTE [dwSize];
	DWORD dwNumber;
	ReadFile( hFile, pbyBuffer, dwSize, &dwNumber, 0);
	CloseHandle( hFile);
	
	DWORD dwCheckSum = GenerateCheckSum(pbyBuffer, dwSize, wKey);
	delete [] pbyBuffer;
	
	return (dwCheckSum);
}


BOOL GetFileVersion( char *lpszFileName, WORD *pwVersion)
{
	DWORD dwHandle;
	DWORD dwLen = GetFileVersionInfoSize( lpszFileName, &dwHandle);
	if ( dwLen <= 0)
	{
		return ( FALSE);
	}

	BYTE *pbyData = new BYTE [dwLen];
	if ( !GetFileVersionInfo( lpszFileName, dwHandle, dwLen, pbyData))
	{
		delete [] pbyData;
		return ( FALSE);
	}

	VS_FIXEDFILEINFO *pffi;
	UINT uLen;
	if ( !VerQueryValue( pbyData, "\\", ( LPVOID*)&pffi, &uLen))
	{
		delete [] pbyData;
		return ( FALSE);
	}

	pwVersion[0] = HIWORD( pffi->dwFileVersionMS);
	pwVersion[1] = LOWORD( pffi->dwFileVersionMS);
	pwVersion[2] = HIWORD( pffi->dwFileVersionLS);
	pwVersion[3] = LOWORD( pffi->dwFileVersionLS);

	delete [] pbyData;
	return ( TRUE);
}

extern PATH     *path;

void SaveLoginInfo(char* id, char* pass)
{
	char szRegisterPath[256];
	wsprintf(szRegisterPath, "SOFTWARE\\%s\\Config", gProtect->m_MainInfo.m_registerPath);
	leaf::CRegKey regkey;
	regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, szRegisterPath);
	regkey.WriteString("ID", id);
	regkey.WriteString("PASS", pass);
	strcpy(m_ID, id);
	strcpy(m_PASS, pass);

}

void DestroyWindow()
{
	char szRegisterPath[256];
	wsprintf(szRegisterPath, "SOFTWARE\\%s\\Config", gProtect->m_MainInfo.m_registerPath);
	//. save volume level
	leaf::CRegKey regkey;
	regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, szRegisterPath);
	regkey.WriteDword("VolumeLevel", g_pOption->GetVolumeLevel());

	CUIMng::Instance().Release();

#ifdef MOVIE_DIRECTSHOW
	if(g_pMovieScene)
	{
		g_pMovieScene->Destroy();
	}
#endif // MOVIE_DIRECTSHOW

	//. release font handle
	if(g_hFont)
		DeleteObject((HGDIOBJ)g_hFont);

	if(g_hFontBold)
		DeleteObject((HGDIOBJ)g_hFontBold);

	if(g_hFontBig)
		DeleteObject((HGDIOBJ)g_hFontBig);

	if(g_hFixFont)
		::DeleteObject((HGDIOBJ)g_hFixFont);
	
	ReleaseCharacters();

    if ( path!=NULL )
    {
	    delete path;
    }
	SAFE_DELETE(GateAttribute);

	for ( int i = 0; i < MAX_SKILLS; ++i)
	{
	}
	SAFE_DELETE(SkillAttribute);

	SAFE_DELETE(CharacterMachine);

    DeleteWaterTerrain ();

#ifdef MOVIE_DIRECTSHOW
	if(SceneFlag != MOVIE_SCENE)
#endif // MOVIE_DIRECTSHOW
	{
		gMapManager.DeleteObjects();

		// Object.
		for(int i=MODEL_LOGO;i<MAX_MODELS;i++)
		{
			Models[i].Release();
		}

		// Bitmap
		Bitmaps.UnloadAllImages();
	}

	SAFE_DELETE_ARRAY( CharacterMemoryDump );
	SAFE_DELETE_ARRAY( ItemAttRibuteMemoryDump );
	SAFE_DELETE_ARRAY( RendomMemoryDump );
	SAFE_DELETE_ARRAY( ModelsDump );
	//SAFE_DELETE_ARRAY( Models );
	
#ifdef DYNAMIC_FRUSTRUM
	DeleteAllFrustrum();
#endif //DYNAMIC_FRUSTRUM

	SAFE_DELETE(g_pMercenaryInputBox);
	SAFE_DELETE(g_pSingleTextInputBox);
	SAFE_DELETE(g_pSinglePasswdInputBox);

	SAFE_DELETE(g_pChatRoomSocketList);
	SAFE_DELETE(g_pUIMapName);	// rozy
	SAFE_DELETE( g_pTimer );
	SAFE_DELETE(g_pUIManager);
	 
#ifdef MOVIE_DIRECTSHOW
	SAFE_DELETE(g_pMovieScene);
#endif // MOVIE_DIRECTSHOW

	SAFE_DELETE(pMultiLanguage);
	BoostRest( g_BuffSystem );
	BoostRest( g_MapProcess );
	BoostRest( g_petProcess );

	g_ErrorReport.Write( "Destroy" );
	 
	HWND shWnd = FindWindow(NULL, "MuPlayer");
	if(shWnd)
		SendMessage(shWnd, WM_DESTROY, 0, 0);
}
void DestroySound()
{
	for(int i=0;i<MAX_BUFFER;i++)
		ReleaseBuffer(i);

	FreeDirectSound();
	wzAudioDestroy();
}

int g_iInactiveTime = 0;
int g_iNoMouseTime = 0;
int g_iInactiveWarning = 0;
bool g_bWndActive = false;
bool HangulDelete = false;
int Hangul = 0;
bool g_bEnterPressed = false;

int g_iMousePopPosition_x = 0;
int g_iMousePopPosition_y = 0;

extern int TimeRemain;
extern bool EnableFastInput;
void MainScene(HDC hDC);

LONG FAR PASCAL WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch (msg)
    {
	case TRAYMODE_ICON_MESSAGE:
	{
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			gTrayMode.SwitchState();
			break;
		}
		break;
	}
	/* mensagem ao fechar o jogo
	case WM_CLOSE:
		ShowCursor(TRUE);
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		if (MessageBox(hwnd, "Tem certeza de que deseja sair do jogo?", "Você está prestes a sair", MB_OKCANCEL) == IDOK);
			PostMessage(hwnd, WM_DESTROY, 0, 0);
		break;
	*/
	case WM_SYSKEYDOWN:
		{
			return 0;
		}
		break;
#if defined PROTECT_SYSTEMKEY && defined NDEBUG
#ifndef FOR_WORK
	case WM_SYSCOMMAND:
		{
			if(wParam == SC_KEYMENU || wParam == SC_SCREENSAVE)
			{
				return 0;
			}
		}
		break;
#endif // !FOR_WORK
#endif // PROTECT_SYSTEMKEY && NDEBUG
    case WM_ACTIVATE:
		if(LOWORD(wParam) == WA_INACTIVE)
		{
#ifdef ACTIVE_FOCUS_OUT
			if (g_bUseWindowMode == FALSE)
#endif	// ACTIVE_FOCUS_OUT
				g_bWndActive = false;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
			if (g_bUseWindowMode == TRUE)
			{
				MouseLButton = false;
				MouseLButtonPop = false;
				//MouseLButtonPush = false;
				MouseRButton = false;
				MouseRButtonPop = false;
				MouseRButtonPush = false;
				MouseLButtonDBClick = false;
				MouseMButton = false;
				MouseMButtonPop = false;
				MouseMButtonPush = false;
				MouseWheel = 0;
			}
#endif
		}
		else
		{
			g_bWndActive = true;
		}
		break;
	case WM_TIMER :
		//MessageBox(NULL,GlobalText[16],"Error",MB_OK);
		switch( wParam )
		{
		case HACK_TIMER:
			// PKD_ADD_BINARY_PROTECTION
#ifndef CLIENT_DEBUG
			//VMProtectBegin("HackCheck");
#endif
			CheckHack();
#ifndef CLIENT_DEBUG
			//VMProtectEnd();
#endif
			break;
		case WINDOWMINIMIZED_TIMER:
			PostMessage(g_hWnd, WM_CLOSE, 0, 0);
			break;
		case CHATCONNECT_TIMER:
			g_pFriendMenu->SendChatRoomConnectCheck();
			break;
		case SLIDEHELP_TIMER:
			if(g_bWndActive)
			{
				if(g_pSlideHelpMgr)
					g_pSlideHelpMgr->CreateSlideText();
			}
			break;
		}
		break;
	case WM_USER_MEMORYHACK:
		//SetTimer( g_hWnd, WINDOWMINIMIZED_TIMER, 1*1000, NULL);
		KillGLWindow();
		break;
	case WM_NPROTECT_EXIT_TWO:
		SendHackingChecked( 0x04, 0);
		SetTimer( g_hWnd, WINDOWMINIMIZED_TIMER, 1*1000, NULL);
		MessageBox(NULL,GlobalText[16],"Error",MB_OK);
		break;
	case WM_ASYNCSELECTMSG :
		switch( WSAGETSELECTEVENT( lParam ) )
		{
		case FD_CONNECT :
			break;
		case FD_READ :
			SocketClient.nRecv();
			break;
		case FD_WRITE :
			SocketClient.FDWriteSend();
			break;
		case FD_CLOSE :
			g_pChatListBox->AddText("", GlobalText[3], SEASON3B::TYPE_SYSTEM_MESSAGE);
#ifdef CONSOLE_DEBUG
			switch(WSAGETSELECTERROR(lParam))
			{
			case WSAECONNRESET:
				g_ConsoleDebug->Write(MCD_ERROR, "The connection was reset by the remote side.");
				g_ErrorReport.Write("The connection was reset by the remote side.\r\n");
g_ErrorReport.WriteCurrentTime();
				break;
			case WSAECONNABORTED:
				g_ConsoleDebug->Write(MCD_ERROR, "The connection was terminated due to a time-out or other failure.");
				g_ErrorReport.Write("The connection was terminated due to a time-out or other failure.\r\n");
g_ErrorReport.WriteCurrentTime();
				break;
			}
#endif // CONSOLE_DEBUG
			SocketClient.Close();

			#ifdef NEW_PROTOCOL_SYSTEM
				gProtocolSend.DisconnectServer();
			#endif	

			CUIMng::Instance().PopUpMsgWin(MESSAGE_SERVER_LOST);
			break;
		}
		break;
	case WM_CTLCOLOREDIT:
		SetBkColor((HDC)wParam, RGB(0, 0, 0));
		SetTextColor((HDC)wParam, RGB(255, 255, 255));
		return (LRESULT)GetStockObject(BLACK_BRUSH);
		break;
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps ;
			HDC hDC = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps) ;
		}
		return 0;
		break;
	case WM_DESTROY:
		{
			Destroy = true;
			SocketClient.Close();

			#ifdef NEW_PROTOCOL_SYSTEM
				gProtocolSend.DisconnectServer();
			#endif	

			DestroySound();
			//DestroyWindow();
			KillGLWindow();	
			CloseMainExe();
			PostQuitMessage(0);
		}
		break;
    case WM_SETCURSOR:
        ShowCursor(false);
		break;
#if (defined WINDOWMODE)
	case WM_SIZE:
		if ( SIZE_MINIMIZED == wParam && g_bUseWindowMode == FALSE )
		{
			if ( !( g_bMinimizedEnabled))
			{
				DWORD dwMess[SIZE_ENCRYPTION_KEY];
				for ( int i = 0; i < SIZE_ENCRYPTION_KEY; ++i)
				{
					dwMess[i] = GetTickCount();
				}
				g_SimpleModulusCS.LoadKeyFromBuffer( ( BYTE*)dwMess, FALSE, FALSE, FALSE, TRUE);
			}
		}
		break;
#else
#ifdef NDEBUG
#ifndef FOR_WORK
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	case WM_SIZE:
		if ( SIZE_MINIMIZED == wParam
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
			&& g_bUseWindowMode == FALSE
#endif
			)
		{
			if ( !( g_bMinimizedEnabled))
			{
				SendHackingChecked( 0x05, 0);
				DWORD dwMess[SIZE_ENCRYPTION_KEY];
				for ( int i = 0; i < SIZE_ENCRYPTION_KEY; ++i)
				{
					dwMess[i] = GetTickCount();
				}
				g_SimpleModulusCS.LoadKeyFromBuffer( ( BYTE*)dwMess, FALSE, FALSE, FALSE, TRUE);
			}
		}
		break;
#endif
#endif
#endif
#endif	//WINDOWMODE(#else)
//-----------------------------
	default:
		if (msg >= WM_CHATROOMMSG_BEGIN && msg < WM_CHATROOMMSG_END)
			g_pChatRoomSocketList->ProcessSocketMessage(msg - WM_CHATROOMMSG_BEGIN, WSAGETSELECTEVENT(lParam));
		break;
	}

	MouseLButtonDBClick = false;
	if (MouseLButtonPop == true && (g_iMousePopPosition_x != MouseX || g_iMousePopPosition_y != MouseY)) 
		MouseLButtonPop = false;
    switch (msg)
	{
	case WM_MOUSEMOVE:
		{
			MouseX = (float)LOWORD(lParam) / g_fScreenRate_x;
			MouseY = (float)HIWORD(lParam) / g_fScreenRate_y;
			if(MouseX < 0) 
				MouseX = 0;
			if(MouseX > gWideScreen.g_WideWindowWidth)
				MouseX = gWideScreen.g_WideWindowWidth;
			if(MouseY < 0) 
				MouseY = 0;
			if(MouseY > gWideScreen.g_WideWindowHeight)
				MouseY = gWideScreen.g_WideWindowHeight;
		}
		break;
	case WM_LBUTTONDOWN:
		g_iNoMouseTime = 0;
		MouseLButtonPop = false;
		if(!MouseLButton) 
			MouseLButtonPush = true;
		MouseLButton = true;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		SetCapture(g_hWnd);
#endif
		break;
	case WM_LBUTTONUP:
		g_iNoMouseTime = 0;
		MouseLButtonPush = false;
		//if(MouseLButton) MouseLButtonPop = true;
		MouseLButtonPop = true;
		MouseLButton = false;
		g_iMousePopPosition_x = MouseX;
		g_iMousePopPosition_y = MouseY;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		ReleaseCapture();
#endif
		break;
	case WM_RBUTTONDOWN:
		g_iNoMouseTime = 0;
		MouseRButtonPop = false;
		if(!MouseRButton) MouseRButtonPush = true;
		MouseRButton = true;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		SetCapture(g_hWnd);
#endif
		break;
	case WM_RBUTTONUP:
		g_iNoMouseTime = 0;
		MouseRButtonPush = false;
		if(MouseRButton) MouseRButtonPop = true;
		MouseRButton = false;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		ReleaseCapture();
#endif
		break;
    case WM_LBUTTONDBLCLK:
		g_iNoMouseTime = 0;
		MouseLButtonDBClick = true;
		break;
	case WM_MBUTTONDOWN:
		g_iNoMouseTime = 0;
		MouseMButtonPop = false;
		if(!MouseMButton) MouseMButtonPush = true;
		MouseMButton = true;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		SetCapture(g_hWnd);
#endif
		break;
	case WM_MBUTTONUP:
		g_iNoMouseTime = 0;
		MouseMButtonPush = false;
		if(MouseMButton) MouseMButtonPop = true;
		MouseMButton = false;
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		ReleaseCapture();
#endif
		break;
	case WM_MOUSEWHEEL:
        {
            int wheelDelta = (short)HIWORD(wParam) / WHEEL_DELTA;
            MouseWheel = wheelDelta;
            gCamera.OnMouseWheel(wheelDelta);
        }
        break;
	case WM_IME_NOTIFY:
		{
			if (g_iChatInputType == 1)
			{
				switch (wParam)
				{
				case IMN_SETCONVERSIONMODE:
					if (GetFocus() == g_hWnd)
					{
						CheckTextInputBoxIME(IME_CONVERSIONMODE);
					}
					break;
				case IMN_SETSENTENCEMODE:
					if (GetFocus() == g_hWnd)
					{
						CheckTextInputBoxIME(IME_SENTENCEMODE);
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	case WM_CHAR:
		{
			switch(wParam)
			{
			case VK_RETURN:
				{
					SetEnterPressed( true );
				}
				break;
			}
		}
		break;
    }

	if( g_BuffSystem ) {
		LRESULT result;
		TheBuffStateSystem().HandleWindowMessage( msg, wParam, lParam, result );
	}

    return DefWindowProc(hwnd,msg,wParam,lParam);
}

bool CreateOpenglWindow()
{
    PIXELFORMATDESCRIPTOR pfd;

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 24;
	pfd.cDepthBits   = 24;

	if (!(g_hDC=GetDC(g_hWnd)))
	{
		g_ErrorReport.Write( "OpenGL Get DC Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL,GlobalText[4],"OpenGL Get DC Error.",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	GLuint PixelFormat;

	if (!(PixelFormat=ChoosePixelFormat(g_hDC,&pfd)))
	{
		g_ErrorReport.Write( "OpenGL Choose Pixel Format Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL,GlobalText[4],"OpenGL Choose Pixel Format Error.",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(!SetPixelFormat(g_hDC,PixelFormat,&pfd))
	{
		g_ErrorReport.Write( "OpenGL Set Pixel Format Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL,GlobalText[4],"OpenGL Set Pixel Format Error.",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(g_hRC=wglCreateContext(g_hDC)))
	{
		g_ErrorReport.Write( "OpenGL Create Context Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL,GlobalText[4],"OpenGL Create Context Error.",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	if(!wglMakeCurrent(g_hDC,g_hRC))
	{
		g_ErrorReport.Write( "OpenGL Make Current Error - ErrorCode : %d\r\n", GetLastError());
		KillGLWindow();
		MessageBox(NULL,GlobalText[4],"OpenGL Make Current Error.",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}



	ShowWindow(g_hWnd,SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);
	return true;
}

int g_MaxMessagePerCycle = -1;

void SetMaxMessagePerCycle(int messages)
{
	constexpr int custom_min = 3;
	g_MaxMessagePerCycle = (messages > 0) ? max(messages, custom_min) : messages;
}

HWND StartWindow(HINSTANCE hCurrentInst, int nCmdShow)
{
	char* windowName = gProtect->m_MainInfo.WindowName;

	WNDCLASS wndClass;
	HWND hWnd;

	wndClass.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hCurrentInst;
	wndClass.hIcon = LoadIcon(hCurrentInst, (LPCTSTR)IDI_ICON1);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = windowName;
	RegisterClass(&wndClass);

	if (g_bUseWindowMode == TRUE)
	{
		RECT rc = { 0, 0, WindowWidth, WindowHeight };
		if (g_bUseWindowMode == TRUE)
			AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN, NULL);
		else
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, NULL);
		rc.right -= rc.left;
		rc.bottom -= rc.top;

		if (g_bUseWindowMode == TRUE)
		{
			hWnd = CreateWindow(
				windowName, windowName,
				WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN,
				(GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
				(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
				rc.right,
				rc.bottom,
				NULL, NULL, hCurrentInst, NULL);
		}
		else
		{
			hWnd = CreateWindow(
				windowName, windowName,
				WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
				(GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2,
				(GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2,
				rc.right,
				rc.bottom,
				NULL, NULL, hCurrentInst, NULL);
		}
	}
	else
	{
		hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_APPWINDOW,
			windowName, windowName,
			WS_POPUP,
			0, 0,
			WindowWidth,
			WindowHeight,
			NULL, NULL, hCurrentInst, NULL);
	}

	return hWnd;
}

char m_ID[11];
char m_PASS[11];
char m_Version[11];
char m_ExeVersion[11];
int  m_SoundOnOff;
int  m_MusicOnOff;
int  m_Resolution;
int	m_nColorDepth;
int	g_iRenderTextType = 0;

// ---- 3D camera ----
int	m_CameraOnOff;
// -------------------

char g_aszMLSelection[MAX_LANGUAGE_NAME_LENGTH] = {'\0'};
std::string g_strSelectedML = "%S";

BOOL OpenInitFile()
{
	gController.Load();
	szServerIpAddress = new char[32];

	memset(szServerIpAddress, 0, sizeof(char) * 32);
	memcpy(szServerIpAddress, gProtect->m_MainInfo.IpAddress, sizeof(char) * 32);
	g_ServerPort = gProtect->m_MainInfo.IpAddressPort;

	Version[0] = gProtect->m_MainInfo.ClientVersion[0] + 1;
	Version[1] = gProtect->m_MainInfo.ClientVersion[2] + 2;
	Version[2] = gProtect->m_MainInfo.ClientVersion[3] + 3;
	Version[3] = gProtect->m_MainInfo.ClientVersion[5] + 4;
	Version[4] = gProtect->m_MainInfo.ClientVersion[6] + 5;

	memcpy(Serial, gProtect->m_MainInfo.ClientSerial, sizeof(Serial));

//#ifdef _DEBUG

	m_ID[0] = '\0';
	m_PASS[0] = '\0';
	m_SoundOnOff = 1;
	m_MusicOnOff = 1;
	m_Resolution = 0;
	m_nColorDepth = 0;

	// ---- 3D camera ----
	m_CameraOnOff = 0;
	// ----

	char szRegisterPath[256];
	wsprintf(szRegisterPath, "SOFTWARE\\%s\\Config", gProtect->m_MainInfo.m_registerPath);

	HKEY hKey;
	DWORD dwDisp;
	DWORD dwSize;

	if ( ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, szRegisterPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, & hKey, &dwDisp))
	{
		leaf::CRegKey regkey;
		regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, szRegisterPath);

		dwSize = 11;
		if ( RegQueryValueEx (hKey, "ID", 0, NULL, (LPBYTE)m_ID, & dwSize) != ERROR_SUCCESS)
		{
			regkey.WriteString("ID", "");
		}

		dwSize = 11;
		if (RegQueryValueEx(hKey, "PASS", 0, NULL, (LPBYTE)m_PASS, &dwSize) != ERROR_SUCCESS)
		{
			regkey.WriteString("PASS", "");
		}

		dwSize = sizeof ( int);
		if ( RegQueryValueEx (hKey, "SoundOnOff", 0, NULL, (LPBYTE) & m_SoundOnOff, &dwSize) != ERROR_SUCCESS)
		{
			m_SoundOnOff = true;
			regkey.WriteDword("SoundOnOff", 1);
		}
		dwSize = sizeof ( int);
		if ( RegQueryValueEx (hKey, "MusicOnOff", 0, NULL, (LPBYTE) & m_MusicOnOff, &dwSize) != ERROR_SUCCESS)
		{
			m_MusicOnOff = false;
			regkey.WriteDword("MusicOnOff", 0);
		}

		dwSize = sizeof ( int);
		if (RegQueryValueEx(hKey, "Resolution", 0, NULL, (LPBYTE)&m_Resolution, &dwSize) != ERROR_SUCCESS)
		{
			m_Resolution = 1;
			regkey.WriteDword("Resolution", 1);
		}

	    if ( RegQueryValueEx (hKey, "ColorDepth", 0, NULL, (LPBYTE) & m_nColorDepth, &dwSize) != ERROR_SUCCESS)
	    {
		    m_nColorDepth = 0;
			regkey.WriteDword("ColorDepth", 0);
	    }
		dwSize = sizeof ( int);
		if ( RegQueryValueEx (hKey, "TextOut", 0, NULL, (LPBYTE) & g_iRenderTextType, &dwSize) != ERROR_SUCCESS)
		{
			g_iRenderTextType = 0;
			regkey.WriteDword("TextOut", 0);
		}

		g_iChatInputType = 1;

#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
		dwSize = sizeof ( int);
		if ( RegQueryValueEx (hKey, "WindowMode", 0, NULL, (LPBYTE) & g_bUseWindowMode, &dwSize) != ERROR_SUCCESS)
		{
			g_bUseWindowMode = TRUE;
			regkey.WriteDword("WindowMode", 1);
		}
#endif // USER_WINDOW_MODE

		ZeroMemory(g_aszMLSelection, MAX_LANGUAGE_NAME_LENGTH);

		dwSize = MAX_LANGUAGE_NAME_LENGTH;
		if (RegQueryValueEx(hKey, "LangSelection", 0, NULL, (LPBYTE)g_aszMLSelection, &dwSize) != ERROR_SUCCESS)
		{
			strcpy(g_aszMLSelection, "Eng");
		}

		g_aszMLSelection[MAX_LANGUAGE_NAME_LENGTH - 1] = '\0';

		if (strlen(g_aszMLSelection) == 0 || !isalpha(g_aszMLSelection[0]))
		{
			strcpy(g_aszMLSelection, "Eng");
		}

		g_strSelectedML = std::string(g_aszMLSelection);
	}
	RegCloseKey( hKey);

	switch(m_Resolution)
	{
	case 0:WindowWidth = 800;   WindowHeight = 480;  break; //800
	case 1:WindowWidth = 1024;  WindowHeight = 600;  break; //819
	case 2:WindowWidth = 1200;  WindowHeight = 768;  break; //750
	case 3:WindowWidth = 1280;  WindowHeight = 720;  break; //853
	case 4:WindowWidth = 1364;  WindowHeight = 768;  break; //853
	case 5:WindowWidth = 1400;  WindowHeight = 900;  break; //746
	case 6:WindowWidth = 1600;  WindowHeight = 900;  break; //853
	case 7:WindowWidth = 1920;  WindowHeight = 900;  break; //1024
	case 8:WindowWidth = 1920;  WindowHeight = 1080; break; //853
	default:WindowWidth = 1600; WindowHeight = 900;  break;
	}


	if (m_Resolution > 3)
	{
		g_fScreenRate_x = 1.6f;
		g_fScreenRate_y = 1.6f;
	}
	else
	{
		g_fScreenRate_x = (float)WindowHeight / 480;
		g_fScreenRate_y = (float)WindowHeight / 480;
	}
	
	

	gWideScreen.Init();

	return TRUE;
}

BOOL Util_CheckOption( char *lpszCommandLine, unsigned char cOption, char *lpszString)
{
	unsigned char cComp[2];
	cComp[0] = cOption; cComp[1] = cOption;
	if ( islower( ( int)cOption))
	{
		cComp[1] = toupper( ( int)cOption);
	}
	else if ( isupper( ( int)cOption))
	{
		cComp[1] = tolower( ( int)cOption);
	}

	int nFind = ( int)'/';
	unsigned char *lpFound = ( unsigned char*)lpszCommandLine;
	while ( lpFound)
	{
		lpFound = ( unsigned char*)strchr( ( char*)( lpFound + 1), nFind);
		if ( lpFound && ( *( lpFound + 1) == cComp[0] || *( lpFound + 1) == cComp[1]))
		{	// ¹ß°ß
			if ( lpszString)
			{
				int nCount = 0;
				for ( unsigned char *lpSeek = lpFound + 2; *lpSeek != ' ' && *lpSeek != '\0'; lpSeek++)
				{
					nCount++;
				}

				memcpy( lpszString, lpFound + 2, nCount);
				lpszString[nCount] = '\0';
			}
			return ( TRUE);
		}
	}

	return ( FALSE);
}

BOOL UpdateFile( char *lpszOld, char *lpszNew)
{
	SetFileAttributes(lpszOld, FILE_ATTRIBUTE_NORMAL);
	SetFileAttributes(lpszNew, FILE_ATTRIBUTE_NORMAL);

	DWORD dwStartTickCount = ::GetTickCount();
	while(::GetTickCount() - dwStartTickCount < 5000) {
		if ( CopyFile( lpszOld, lpszNew, FALSE))
		{	// ¼º°ø
			DeleteFile( lpszOld);
			return ( TRUE);
		}
	}
	g_ErrorReport.Write("%s to %s CopyFile Error : %d\r\n", lpszNew, lpszOld, GetLastError());
	return ( FALSE);
}

char g_lpszCmdURL[50];
BOOL GetConnectServerInfo( PSTR szCmdLine, char *lpszURL, WORD *pwPort)
{
	char lpszTemp[256] = {0, };
	if( Util_CheckOption( szCmdLine, 'y', lpszTemp))
	{
		BYTE bySuffle[] = { 0x0C, 0x07, 0x03, 0x13 };

		for(int i=0; i<(int)strlen(lpszTemp); i++)
			lpszTemp[i] -= bySuffle[i%4];
		strcpy(lpszURL, lpszTemp);

		if( Util_CheckOption( szCmdLine, 'z', lpszTemp)) 
		{
			for(int j=0; j<(int)strlen(lpszTemp); j++)
				lpszTemp[j] -= bySuffle[j%4];
			*pwPort = atoi( lpszTemp);
		}
		g_ErrorReport.Write("[Virtual Connection] Connect IP : %s, Port : %d\r\n", lpszURL, *pwPort);
		return (TRUE);
	}
	if ( !Util_CheckOption( szCmdLine, 'u', lpszTemp))
	{
		return ( FALSE);
	}
	strcpy( lpszURL, lpszTemp);
	if ( !Util_CheckOption( szCmdLine, 'p', lpszTemp))
	{
		return ( FALSE);
	}
	*pwPort = atoi( lpszTemp);

	return ( TRUE);
}

extern int TimeRemain;
BOOL g_bInactiveTimeChecked = FALSE;
void MoveObject(OBJECT *o);

bool ExceptionCallback(_EXCEPTION_POINTERS* pExceptionInfo )
{
#ifdef ENABLE_FULLSCREEN
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
#endif	// USER_WINDOW_MODE
	{
		ChangeDisplaySettings(NULL,0);
	}
#endif //ENABLE_FULLSCREEN
	return true;
}

/// Verifica se o Main foi executado como Adminsitrador
bool IsRunningAsAdmin()
{
	BOOL isAdmin = FALSE;
	PSID adminGroup;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
		CheckTokenMembership(NULL, adminGroup, &isAdmin);
		FreeSid(adminGroup);
	}
	return isAdmin;
}

/// Se não, faz um processo de restart instantaneo e executa o Main como Admin!
void RestartAsAdmin()
{
	char szPath[MAX_PATH];
	if (GetModuleFileNameA(NULL, szPath, MAX_PATH)) {
		char szWorkingDir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, szWorkingDir);

		SHELLEXECUTEINFO execInfo = { 0 };
		execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		execInfo.fMask = SEE_MASK_FLAG_DDEWAIT;
		execInfo.hwnd = NULL;
		execInfo.lpVerb = "runas";
		execInfo.lpFile = szPath;
		execInfo.lpDirectory = szWorkingDir;
		execInfo.nShow = SW_SHOWNORMAL;

		if (!ShellExecuteEx(&execInfo)) {
			MessageBox(NULL, "Falha ao iniciar o programa como Administrador.", "Erro", MB_OK | MB_ICONERROR);
		}
		ExitProcess(0);
	}
}


extern "C"
{
	_declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}

// Force NVidia Optimus to use NVidia GPU on drivers 302 and later.
// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow)
{
#ifndef CLIENT_DEBUG
	if (!IsDebuggerPresent() && !IsRunningAsAdmin()) //Verifica se foi executado como Admin
	{
		RestartAsAdmin(); //Se não, restarta e executa como Admin
		return 0;
	}
#endif
gController.Load();
MSG msg;
leaf::AttachExceptionHandler(ExceptionCallback);
gProtect = new CProtect;

	char configpath[256];
	wsprintf(configpath, "Data\\Custom\\Config.%s", FILE_EXTENSION);
if (gProtect->ReadMainFile(configpath) == 0)
	{
		MessageBox(0, "Contact Support", "Configuration Error!", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}
wsprintf(configpath, "Data\\Custom\\CustomJewel.%s", FILE_EXTENSION);
if (gProtect->ReadCustomJewelConfig(configpath) == 0)
	{
		MessageBox(0, "Contact Support", "Jewel Configuration Error!", MB_OK | MB_ICONERROR);
		ExitProcess(0);
	}

	////Hook-dll Antihack//
	//HMODULE module = LoadLibrary("Data\\World105\\Antihack.dll");
	
	//if (module == 0)
	//{
	//	MessageBox(0, "Can't load dll", "Error", MB_OK | MB_ICONERROR);
	//	ExitProcess(0);
	//}
	
	//void(*EntryProc)() = (void(*)())GetProcAddress(module, "EntryProc");
	
	//if (EntryProc != 0)
	//{
	//	EntryProc();
	//}

	//if (strlen(gProtect->m_MainInfo.IpAddress) == 0 ||
	//	strcmp(gProtect->m_MainInfo.IpAddress, IP_ADDRES_CONFIG) != 0)
	//{
	//	MessageBoxA(NULL, "Conexão não permitida", "ERROR", MB_ICONSTOP);
	//	ExitProcess(0);
	//}
gTrayMode.Load();
	gItemManager.Init();
gHelperSystem.Init();
	gCustomEarthQuakeSkill.Init();
	gCustomItemPosition.Init();
	gDescriptions.Init();
	gDarkSpirit.Init();
Visuals.Init();
gCustomBow.Init();
	gCustomWing.Init();
	gCustomEffects.Init();
	gCCustomSetEffect.Init();
	gCustomItemFloor.Init();
	gCustomItemForce.Init();
	gRenderModel.Init();
	gDisableExcellent.Init();
	gCustomServerName.Init();
	gCreateFont.Init();
	gNewSounds.Init();
	gMessageColor.Init();
	gMonsters.Init();
	gMonsterName.Init();
	gMonsterGlow.Init();
	gMonsterEffect.Init();
	gCustomCape.Init();
	gItemEdge.Init();
	gRingTransform.Init();
	gJewelStack.Init();
	gDrawBarItem.Init();
	gCounterItem.Init();
	gCustomMeshGlow.Init();
	gCharacterList.Init();
	gBuffColorBK.Init();
	gBuffColorSM.Init();
	gPlasmaStormColor.Init();
	gRuneColorEffect.Init();
	gSetAction.Init();
	gDeathStabColor.Init();
	gCustomEye.Init();
	gCustomMonsterItem.Init();
	gSummonEffect.Init();
	gCustomTitleItem.Init();
	gCharactername.Init();
	gCustomColorChat.Init();
	gPatente.Init();
	gSetModelAction.Init();
	gItemBorder.Init();
	gItemBorder.UpdateFadeEffect();

	char lpszExeVersion[256] = "unknown";
char *lpszCommandLine = GetCommandLine();
	char lpszFile[MAX_PATH];
	WORD wVersion[4] = { 0,};
	if ( GetFileNameOfFilePath( lpszFile, lpszCommandLine))
	{
		if ( GetFileVersion( lpszFile, wVersion))
		{
			sprintf( lpszExeVersion, "%d.%02d", wVersion[0], wVersion[1]);
			if ( wVersion[2] > 0)
			{
				char lpszMinorVersion[2] = "a";
				lpszMinorVersion[0] += ( wVersion[2] - 1);
				strcat( lpszExeVersion, lpszMinorVersion);
			}
		}
	}
g_ErrorReport.Write( "\r\n");
	g_ErrorReport.WriteLogBegin();
	g_ErrorReport.AddSeparator();
	g_ErrorReport.Write( "Mu online %s (%s) executed. (%d.%d.%d.%d)\r\n", lpszExeVersion, "Eng", wVersion[0], wVersion[1], wVersion[2], wVersion[3]);
g_ConsoleDebug->Write(MCD_NORMAL, "Mu Online (Version: %d.%d.%d.%d)", wVersion[0], wVersion[1], wVersion[2], wVersion[3]);
	g_ErrorReport.WriteCurrentTime();
	ER_SystemInfo si;
	ZeroMemory( &si, sizeof ( ER_SystemInfo));
	GetSystemInfo( &si);
	g_ErrorReport.AddSeparator();
	g_ErrorReport.WriteSystemInfo( &si);
	g_ErrorReport.AddSeparator();

	// PKD_ADD_BINARY_PROTECTION
#ifndef CLIENT_DEBUG
	//VMProtectBegin("CSProtocol");
#endif
	WORD wPortNumber;	
	if ( GetConnectServerInfo( szCmdLine, g_lpszCmdURL, &wPortNumber))
	{
		szServerIpAddress = g_lpszCmdURL;
		g_ServerPort = wPortNumber;
	}

	if ( !OpenMainExe())
	{
		return false;
	}


	// PKD_ADD_BINARY_PROTECTION
	
	g_SimpleModulusCS.LoadEncryptionKey( "Data\\Enc1.dat");
	g_SimpleModulusSC.LoadDecryptionKey( "Data\\Dec2.dat");
	

	g_ErrorReport.Write( "> To read config.ini.\r\n");
	if( OpenInitFile() == FALSE )
	{
		g_ErrorReport.Write( "config.ini read error\r\n");
		return false;
	}
#ifndef CLIENT_DEBUG
	//VMProtectEnd();
#endif
	pMultiLanguage = new CMultiLanguage(g_strSelectedML);

	if (g_iChatInputType == 1)
		ShowCursor(FALSE);

	g_ErrorReport.Write( "> Enum display settings.\r\n");
	DEVMODE DevMode;
	DEVMODE* pDevmodes;
	int nModes = 0;
	while (EnumDisplaySettings(NULL, nModes, &DevMode)) nModes++;
	pDevmodes = new DEVMODE[nModes+1];
	nModes = 0;
	while (EnumDisplaySettings(NULL, nModes, &pDevmodes[nModes])) nModes++;

	DWORD dwBitsPerPel = 16;
	for(int n1=0; n1<nModes; n1++)
	{
		if(pDevmodes[n1].dmBitsPerPel == 16 && m_nColorDepth == 0) {
			dwBitsPerPel = 16; break;
		}
		if(pDevmodes[n1].dmBitsPerPel == 24 && m_nColorDepth == 1) {
			dwBitsPerPel = 24; break;
		}
		if(pDevmodes[n1].dmBitsPerPel == 32 && m_nColorDepth == 1) {
			dwBitsPerPel = 32; break;
		}
	}

#ifdef ENABLE_FULLSCREEN
#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
#endif	// USER_WINDOW_MODE
	{
		for(int n2=0; n2<nModes; n2++)
		{
			if(pDevmodes[n2].dmPelsWidth==WindowWidth && pDevmodes[n2].dmPelsHeight==WindowHeight && pDevmodes[n2].dmBitsPerPel == dwBitsPerPel)
			{
				g_ErrorReport.Write( "> Change display setting %dx%d.\r\n", pDevmodes[n2].dmPelsWidth, pDevmodes[n2].dmPelsHeight);
				ChangeDisplaySettings(&pDevmodes[n2],0);
				break;
			}
		}
	}
#endif //ENABLE_FULLSCREEN

	delete [] pDevmodes;

	g_ErrorReport.Write( "> Screen size = %d x %d.\r\n", WindowWidth, WindowHeight);

    g_hInst = hInstance;
    g_hWnd = StartWindow(hInstance,nCmdShow);
	g_ErrorReport.Write( "> Start window success.\r\n");

    if ( !CreateOpenglWindow())
	{
		return FALSE;
	}

	g_ErrorReport.Write( "> OpenGL init success.\r\n");
	g_ErrorReport.AddSeparator();
	//g_ErrorReport.WriteOpenGLInfo();
	g_ErrorReport.AddSeparator();
	g_ErrorReport.WriteSoundCardInfo(); 

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

	g_ErrorReport.AddSeparator();

	InitVSync();
	if (IsVSyncAvailable())
	{
EnableVSync();
	SetTargetFps(60);
	}
	
	switch(m_Resolution)
	{
		case 0:
		case 1:
			FontHeight = 13; break;
		default: FontHeight = 16; break;

	}


	
	int nFixFontHeight = 13;
	int nFixFontSize;
	int iFontSize;

	iFontSize = FontHeight - 1;
	nFixFontSize = nFixFontHeight - 1;

	gCreateFont.SetFont(iFontSize, FontHeight, nFixFontSize, nFixFontHeight);

	setlocale( LC_ALL, "pt_BR");

	CInput::Instance().Create(g_hWnd, WindowWidth, WindowHeight);

	g_pNewUISystem->Create();

	if(m_MusicOnOff)
	{
		wzAudioCreate(g_hWnd);
		wzAudioOption(WZAOPT_STOPBEFOREPLAY, 1);
	}

	if(m_SoundOnOff)
	{
		char szRegisterPath[256];
		wsprintf(szRegisterPath, "SOFTWARE\\%s\\Config", gProtect->m_MainInfo.m_registerPath);

		InitDirectSound(g_hWnd);
		leaf::CRegKey regkey;
		regkey.SetKey(leaf::CRegKey::_HKEY_CURRENT_USER, szRegisterPath);
		DWORD value;
		if(!regkey.ReadDword("VolumeLevel", value))
		{
			value = 5;	//. default setting
			regkey.WriteDword("VolumeLevel", value);
		}
		if(value<0 || value>=10)
			value = 5;
		
		g_pOption->SetVolumeLevel(int(value));
		SetEffectVolumeLevel(g_pOption->GetVolumeLevel());
	}

	SetTimer(g_hWnd, HACK_TIMER, 20*1000, NULL);

	srand((unsigned)time(NULL));
	for(int i=0;i<100;i++)
		RandomTable[i] = rand()%360;

	//memorydump[0]
	RendomMemoryDump = new BYTE [rand()%100+1];


	GateAttribute				= new GATE_ATTRIBUTE [MAX_GATES];
	SkillAttribute				= new SKILL_ATTRIBUTE[MAX_SKILLS];

	//memorydump[1]
	ItemAttRibuteMemoryDump		= new ITEM_ATTRIBUTE [MAX_ITEM+1024];
	ItemAttribute				= ((ITEM_ATTRIBUTE*)ItemAttRibuteMemoryDump)+rand()%1024;

	//memorydump[2]
	CharacterMemoryDump			= new CHARACTER      [MAX_CHARACTERS_CLIENT+1+128];
	CharactersClient			= ((CHARACTER*)CharacterMemoryDump)+rand()%128;
	CharacterMachine			= new CHARACTER_MACHINE;

	memset(GateAttribute       ,0,sizeof(GATE_ATTRIBUTE   )*(MAX_GATES              ));
	memset(ItemAttribute       ,0,sizeof(ITEM_ATTRIBUTE   )*(MAX_ITEM               ));
	memset(SkillAttribute      ,0,sizeof(SKILL_ATTRIBUTE  )*(MAX_SKILLS             ));
	memset(CharacterMachine    ,0,sizeof(CHARACTER_MACHINE));

    CharacterAttribute   = &CharacterMachine->Character;
    CharacterMachine->Init();
	Hero = &CharactersClient[0];	

	if (g_iChatInputType == 1)
	{
		 g_pMercenaryInputBox = new CUIMercenaryInputBox;
		 g_pSingleTextInputBox = new CUITextInputBox;
		 g_pSinglePasswdInputBox = new CUITextInputBox;
	}
	
	g_pChatRoomSocketList = new CChatRoomSocketList;
	g_pUIManager = new CUIManager;
	g_pUIMapName = new CUIMapName;	// rozy
	g_pTimer = new CTimer();

#ifdef MOVIE_DIRECTSHOW
	g_pMovieScene = new CMovieScene;
#endif // MOVIE_DIRECTSHOW
	
	g_BuffSystem = BuffStateSystem::Make();

	g_MapProcess = MapProcess::Make();

	g_petProcess = PetProcess::Make();

	CUIMng::Instance().Create();

	if (g_iChatInputType == 1)
	{
		g_pMercenaryInputBox->Init(g_hWnd);
		g_pSingleTextInputBox->Init(g_hWnd, 200, 20);
		g_pSinglePasswdInputBox->Init(g_hWnd, 200, 20, 9, TRUE);
		g_pSingleTextInputBox->SetState(UISTATE_HIDE);
		g_pSinglePasswdInputBox->SetState(UISTATE_HIDE);

		g_pMercenaryInputBox->SetFont(g_hFont);
		g_pSingleTextInputBox->SetFont(g_hFont);
		g_pSinglePasswdInputBox->SetFont(g_hFont);

		g_bIMEBlock = FALSE;
		HIMC  hIMC = ImmGetContext(g_hWnd);
		ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE );
		ImmReleaseContext(g_hWnd, hIMC);
		SaveIMEStatus();
		g_bIMEBlock = TRUE;
	}
#if (defined WINDOWMODE)
	if (g_bUseWindowMode == FALSE)
	{
		int nOldVal;
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &nOldVal, 0);
		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &g_iScreenSaverOldValue, 0);
		SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 300*60, NULL, 0);
	}
#else
#ifdef NDEBUG
#ifndef FOR_WORK
#ifdef ACTIVE_FOCUS_OUT
	if (g_bUseWindowMode == FALSE)
	{
#endif	// ACTIVE_FOCUS_OUT
		int nOldVal; // °ªÀÌ µé¾î°¥ ÇÊ¿ä°¡ ¾øÀ½
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, 1, &nOldVal, 0);  // ´ÜÃàÅ°¸¦ ¸ø¾²°Ô ÇÔ
		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &g_iScreenSaverOldValue, 0);  // ½ºÅ©¸°¼¼ÀÌ¹ö Â÷´Ü
		SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 300*60, NULL, 0);  // ½ºÅ©¸°¼¼ÀÌ¹ö Â÷´Ü
#ifdef ACTIVE_FOCUS_OUT
	}
#endif	// ACTIVE_FOCUS_OUT
#endif
#endif
#endif	//WINDOWMODE(#else)

#ifdef SAVE_PACKET
	DeleteFile( PACKET_SAVE_FILE);
#endif

#if defined PROTECT_SYSTEMKEY && defined NDEBUG
#ifndef FOR_WORK
	ProtectSysKey::AttachProtectSysKey(g_hInst, g_hWnd);
#endif // !FOR_WORK
#endif // PROTECT_SYSTEMKEY && NDEBUG
	
    while( 1 )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            if( !GetMessage( &msg, NULL, 0, 0 ) )
            {
				break;
            }
            else
			{
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
     	else 
		{
			//Scene
#if (defined WINDOWMODE)
			if (g_bUseWindowMode == TRUE)
			{
				Scene(g_hDC);
			}
			else if(g_bWndActive)
			{
		       	Scene(g_hDC);
			}
#ifndef FOR_WORK
			else if (g_bUseWindowMode == FALSE)
			{
				SetForegroundWindow( g_hWnd);
				SetFocus(g_hWnd);

				if ( g_iInactiveWarning > 1)
				{
					SetTimer( g_hWnd, WINDOWMINIMIZED_TIMER, 1*1000, NULL);
					PostMessage(g_hWnd, WM_CLOSE, 0, 0);
				}
				else
				{
					g_iInactiveWarning++;
					g_bMinimizedEnabled = TRUE;
					ShowWindow( g_hWnd, SW_MINIMIZE);
					g_bMinimizedEnabled = FALSE;
					ShowWindow( g_hWnd, SW_MAXIMIZE);
				}
			}
#endif//FOR_WORK
#else//WINDOWMODE
			if(g_bWndActive)
		       	Scene(g_hDC);

#endif	//WINDOWMODE(#else)
		}

	#ifdef NEW_PROTOCOL_SYSTEM
		if(SceneFlag < CHARACTER_SCENE)
			ProtocolCompiler();

		g_pChatRoomSocketList->ProtocolCompile();
		gProtocolSend.RecvMessage();
	#else
		ProtocolCompiler();
		g_pChatRoomSocketList->ProtocolCompile();
	#endif

		
    } // while( 1 )

	DestroyWindow();

    return msg.wParam;
}

