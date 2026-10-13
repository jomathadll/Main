// NewUIDuelWindow.h: interface for the CNewUIDuelWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIDUELWINDOW_H__446BA52D_E675_4B70_8A9B_65A672B9FBEB__INCLUDED_)
#define AFX_NEWUIDUELWINDOW_H__446BA52D_E675_4B70_8A9B_65A672B9FBEB__INCLUDED_

#pragma once

#include "NewUIManager.h"

namespace SEASON3B
{
	
	class CNewUIDuelWindow  : public CNewUIObj  
	{
	private:
		enum IMAGE_LIST
		{
			IMAGE_DUEL_BACK = BITMAP_INTERFACE_NEW_BATTLE_SOCCER_SCORE_BEGIN,
#if CLIENT_VERSION >= 1001
			IMAGE_DUEL_HPBAR_BACK = BITMAP_INTERFACE_NEW_PETINFO_WINDOW_BEGIN,
			IMAGE_DUEL_HPBAR = BITMAP_NEW_HPBAR,
			IMAGE_DUEL_SDBAR,
#endif
		};
		enum
		{
			DUEL_WND_WIDTH = 131,
			DUEL_WND_HEIGHT = 70,
		};
	public:
		CNewUIDuelWindow();
		virtual ~CNewUIDuelWindow();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		float GetLayerDepth();	//. 1.1f

	private:
		void LoadImages();
		void UnloadImages();

		void RenderFrame();
		void RenderContents();
		
		CNewUIManager* m_pNewUIMng;		// UI 매니저.
		POINT m_Pos;					// 창의 위치.

#if CLIENT_VERSION >= 1001
		BOOL m_bHasHPReceived;	// HP 초기상태인가
		float m_fPrevHPRate1;
		float m_fPrevHPRate2;
		float m_fPrevSDRate1;
		float m_fPrevSDRate2;
		float m_fLastHPRate1;
		float m_fLastHPRate2;
		float m_fLastSDRate1;
		float m_fLastSDRate2;
		float m_fReceivedHPRate1;
		float m_fReceivedHPRate2;
		float m_fReceivedSDRate1;
		float m_fReceivedSDRate2;
#endif
		
	};
	
}

#endif // !defined(AFX_NEWUIDUELWINDOW_H__446BA52D_E675_4B70_8A9B_65A672B9FBEB__INCLUDED_)
