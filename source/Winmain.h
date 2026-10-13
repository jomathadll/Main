#ifndef __WINMAIN_H__
#define __WINMAIN_H__

//#define FOR_WORK
/*--------------------------------------------------------------------------------------*/

#define LANGUAGE_KOREAN			( 0 )
#define LANGUAGE_ENGLISH		( 1 )
#define LANGUAGE_TAIWANESE		( 2 )
#define LANGUAGE_CHINESE		( 3 )
#define LANGUAGE_JAPANESE		( 4 )
#define LANGUAGE_THAILAND		( 5 )
#define LANGUAGE_PHILIPPINES	( 6 )
#define LANGUAGE_VIETNAMESE		( 7 )
#define NUM_LANGUAGE			( 8 )

#define SELECTED_LANGUAGE	( LANGUAGE_ENGLISH )

#define _DEBUG
#define FOR_WORK

//#define DYNAMIC_FRUSTRUM


// NoxyDEV debug console.
// 1 = enabled in source, 0 = disabled without touching the rest of the project.
#define NOXY_DEBUG_CONSOLE_ENABLED 1

#if NOXY_DEBUG_CONSOLE_ENABLED
	#define CSK_LH_DEBUG_CONSOLE
	#define CONSOLE_DEBUG
#endif

#ifdef _DEBUG
	#define ENABLE_EDIT
	// ENABLE_EDIT2: movia Hero->Object.Position direto pelas setas (FN+WASD = setas em alguns notebooks)
	// com Velocity = TERRAIN_SCALE * 1.25, sem validacao no servidor. Desativado para evitar speedhack.
	//#define ENABLE_EDIT2
	//#define DEBUG_BITMAP_CACHE
#endif // _DEBUG

#ifdef FOR_WORK
#ifdef _DEBUG

	#if NOXY_DEBUG_CONSOLE_ENABLED
	#define CSK_LH_DEBUG_CONSOLE
	#endif
	#ifdef CSK_LH_DEBUG_CONSOLE
		#define CONSOLE_DEBUG
	#endif // CSK_LH_DEBUG_CONSOLE

	#define CSK_DEBUG_MAP_ATTRIBUTE

	#define CSK_DEBUG_RENDER_BOUNDINGBOX

	#define CSK_DEBUG_MAP_PATHFINDING

#endif // _DEBUG
#endif // FOR_WORK

#define WINDOWMODE						// (2006-07-18)
//#define ENABLE_FULLSCREEN

#if defined USER_WINDOW_MODE || (defined WINDOWMODE)
#define ACTIVE_FOCUS_OUT
#endif	// USER_WINDOW_MODE

//#define DEVIAS_XMAS_EVENT  //more snow in devias
//#define GUILD_WAR_EVENT
#define DUEL_SYSTEM
#define PROTECT_SYSTEMKEY   //alow alt+tab on fullscreen mode
//#define CAMERA_TEST

#define WM_USER_MEMORYHACK	( WM_USER + 1)
#define WM_NPROTECT_EXIT_TWO  (WM_USER + 10001)

extern bool ashies;
extern int weather;
extern int Time_Effect;
extern HWND      g_hWnd;
extern HINSTANCE g_hInst;
extern HDC       g_hDC;
extern HGLRC     g_hRC;
extern HFONT     g_hFont;
extern HFONT     g_hFontBold;
extern HFONT     g_hFontBig;
extern HFONT     g_hFixFont;
extern HFONT     g_hFontSmall;
extern HFONT     g_hFontMedium;
extern HFONT     g_hFontMediumBold;
extern HFONT     g_hFontBigBold;
extern bool      Destroy;
extern int       RandomTable[];
extern double CPU_AVG;
extern int g_MaxMessagePerCycle;

extern char m_ID[];
extern char m_PASS[];
extern char m_Version[];
extern int  m_SoundOnOff;
extern int  m_MusicOnOff;
extern int  m_Resolution;
extern int m_nColorDepth;	
extern char g_aszMLSelection[4];

// ---- 3D camera ----
extern int	m_CameraOnOff;
// -------------------


extern void CheckHack( void);
extern DWORD GetCheckSum( WORD wKey);
extern void StopMp3(char *Name, BOOL bEnforce=false);
extern void PlayMp3(char *Name, BOOL bEnforce=false);
extern bool IsEndMp3();
extern int GetMp3PlayPosition();
extern void SaveLoginInfo(char* id, char* pass);
extern void SetMaxMessagePerCycle(int messages);

extern void CloseMainExe( void);
extern GLvoid KillGLWindow(GLvoid);
extern void DestroyWindow();
extern void DestroySound();

#endif
