// NewUIPartyListWindow.h: interface for the CNewUIPartyInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_NEWUIPARTYMINIWINDOW_H_)
#define _NEWUIPARTYMINIWINDOW_H_

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIButton.h"
#include "NewUIPartyInfoWindow.h"

namespace SEASON3B
{
	class CNewUIPartyListWindow : public CNewUIObj  
	{
	public:
		enum IMAGE_LIST
		{	
			// 기타 이미지
			IMAGE_PARTY_LIST_FLAG = CNewUIPartyInfoWindow::IMAGE_PARTY_FLAG,		// newui_party_flag.tga			(9, 10)	
			IMAGE_PARTY_LIST_EXIT = CNewUIPartyInfoWindow::IMAGE_PARTY_EXIT,		// newui_party_x.tga			(11, 11)
				
			// 파티미니창
			IMAGE_PARTY_LIST_BACK = BITMAP_PARTY_MINILIST_BEGIN,					// newui_party_back.tga			(77, 23)
			IMAGE_PARTY_LIST_HPBAR,													// newui_party_hpbar.jpg		(69, 3)				
#if CLIENT_VERSION >= 1001
			IMAGE_PARTY_LIST_MINIMIZE_BAR = BITMAP_NEW_MINIMIZE_BAR,
			IMAGE_PARTY_LIST_MINIMIZE_BTN = 31286,
			IMAGE_BUFF_STATUS = BITMAP_BUFFWINDOW_BEGIN,
			IMAGE_BUFF_STATUS2,
#endif
		};
	private:
		enum PARTY_WINDOW_SIZE
		{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
			PARTY_LIST_WINDOW_WIDTH = 77,
			PARTY_LIST_WINDOW_HEIGHT = 23,
			PARTY_LIST_HP_BAR_WIDTH = 69,
			PARTY_LIST_HP_BAR_HEIGHT = 3,
#elif CLIENT_VERSION >= 1001
			PARTY_LIST_WINDOW_WIDTH = 90,
			PARTY_LIST_WINDOW_HEIGHT = 30,
			PARTY_LIST_HP_BAR_WIDTH = 80,
			PARTY_LIST_HP_BAR_HEIGHT = 4,
#endif
		};

		enum PARTY_BG_COLOR
		{
			PARTY_LIST_BGCOLOR_DEFAULT = 0,
			PARTY_LIST_BGCOLOR_RED,
			PARTY_LIST_BGCOLOR_GREEN
		};
		
	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;

		
		int							m_iPartyListBGColor[MAX_PARTYS];		// 파티리스트 배경칼라
		bool						m_bPartyMemberoutofSight[MAX_PARTYS];	// 파티원이 내 캐릭터의 시야 밖에 있는가
		
		bool						m_bActive;
		int							m_iVal;		// 인덱스에 따른 편차(y의 위치)

		int							m_iSelectedCharacter;
		int							m_iLimitUserIDHeight[2];
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		CNewUIButton				m_BtnPartyExit[MAX_PARTYS];				// 파티탈퇴 버튼
#elif CLIENT_VERSION >= 1001
		CCustomButtons				m_BtnPartyExit[MAX_PARTYS];				// 파티탈퇴 버튼
		bool						m_minimizeState;
#endif
		
	public:
		CNewUIPartyListWindow();
		virtual ~CNewUIPartyListWindow();
		
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		
		void SetPos(int x, int y);
		void SetPos(int x);
		
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		bool BtnProcess();
		
		float GetLayerDepth();	//. 5.4f

		void OpenningProcess();
		void ClosingProcess();	
#if CLIENT_VERSION >= 1001
		void BuffSort(OBJECT* pHeroObject, std::list<eBuffState>& buffstate);
		void RenderBuffIcon(eBuffState& eBuffType, float x, float y, float width, float height);
		bool SetDisableRenderBuff(const eBuffState& _BuffState);
		void RenderBuffTooltip(eBuffClass& eBuffClassType, eBuffState& eBuffType, float x, float y);
#endif
		int GetSelectedCharacter();
		void SetListBGColor( );

#if CLIENT_VERSION >= 1001
		bool	SetMinimizeState(bool state) { m_minimizeState = state; };
#endif
		
	private:
		void LoadImages();
		void UnloadImages();	
		
		bool SelectCharacterInPartyList( PARTY_t* pMember );
		void RenderPartyHPOnHead( );
	};
}

#endif // !defined(_NEWUIPARTYMINIWINDOW_H_)
