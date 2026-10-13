// NewUIMainFrameWindow.h: interface for the CNewUIMainFrameWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUIMAINFRAMEWINDOW_H__46A029CA_44A5_4050_9216_FA8A25EC4629__INCLUDED_)
#define AFX_NEWUIMAINFRAMEWINDOW_H__46A029CA_44A5_4050_9216_FA8A25EC4629__INCLUDED_

#pragma once

#include "NewUIBase.h"
#include "ZzzTexture.h"
#include "NewUI3DRenderMng.h"
#include "NewUIButton.h"
#include "CustomButtons.h"
#include <vector>

namespace SEASON3B
{
	enum
	{
		HOTKEY_Q = 0,
		HOTKEY_W,
		HOTKEY_E,
#if (CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502) || CLIENT_VERSION >= 1001
		HOTKEY_R,
#endif
		HOTKEY_COUNT
	};
	
	enum
	{
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
//#if CLIENT_VERSION >= 302 && CLIENT_VERSION <= 502
		MAINFRAME_BTN_PARTCHARGE = 0,
//#endif
#endif //defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
		MAINFRAME_BTN_CHAINFO,
		MAINFRAME_BTN_MYINVEN,
#if CLIENT_VERSION >= 402 && CLIENT_VERSION <= 502
		MAINFRAME_BTN_WINDOW,
		MAINFRAME_BTN_FRIEND,
#endif
#if CLIENT_VERSION <= 302 || CLIENT_VERSION >= 1001
		MAINFRAME_BTN_FRIEND,
		MAINFRAME_BTN_PARTY,
#endif
		IMAGE_CUSTOM_ID = 120000,
		DIV_MAIN_UI = 120100,
		DIV_MAIN_ORB_LIFE,
		DIV_MAIN_ORB_MANA,
		DIV_MAIN_ORB_POISON,
		DIV_MAIN_ORB_LINE,
		DIV_RENDER_NUMBER,
		DIV_BACK_EXP_BAR,
		DIV_BAR_POTION,
		DIV_NOTIFICATION,
		DIV_HOTKEY_SKILL_LIST_UP,
		DIV_SKILL_BOX,
		DIV_SKILL1,
		DIV_SKILL2,
		DIV_COMMAND_SKILL,
		DIV_SKILL3,
		DIV_NON_SKILL1,
		DIV_NON_SKILL2,
		DIV_NON_COMMAND_SKILL,
		DIV_NON_SKILL3,
		DIV_SKILL_MASTER,
		DIV_NON_SKILL_MASTER,
	};

	enum KINDOFSKILL
	{
		KOS_COMMAND = 1,
		KOS_SKILL1,
		KOS_SKILL2,
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
		KOS_SKILL3,
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
	};

	class CNewUIItemHotKey
	{
		
	public:
		CNewUIItemHotKey();
		virtual ~CNewUIItemHotKey();

		bool UpdateKeyEvent();

		void SetHotKey(int iHotKey, int iItemType, int iItemLevel);
		int GetHotKey(int iHotKey);
		int GetHotKeyLevel(int iHotKey);
		void UseItemRButton();
		void RenderItems();
		void RenderItemCount();
		int GetHotKeyItemIndex(int iType, bool bItemCount = false);

	private:
		bool GetHotKeyCommonItem(IN int iHotKey, OUT int& iStart, OUT int& iEnd);
		int GetHotKeyItemCount(int iType);

		int m_iHotKeyItemType[HOTKEY_COUNT];
		int m_iHotKeyItemLevel[HOTKEY_COUNT];
	};

	class CNewUISkillList : public CNewUIObj
	{
		enum
		{
			SKILLHOTKEY_COUNT = 10,
			SKILL_SHOP_MAX_BUFF = 16
		};		
		enum EVENT_STATE
		{
			EVENT_NONE = 0,

			// currentskill
			EVENT_BTN_HOVER_CURRENTSKILL,
			EVENT_BTN_DOWN_CURRENTSKILL,

			// skillhotkey 
			EVENT_BTN_HOVER_SKILLHOTKEY,
			EVENT_BTN_DOWN_SKILLHOTKEY,

			// skilllist
			EVENT_BTN_HOVER_SKILLLIST,
			EVENT_BTN_DOWN_SKILLLIST,
		};

	public:
		enum IMAGE_LIST
		{
			IMAGE_SKILL1 = BITMAP_INTERFACE_NEW_SKILLICON_BEGIN,
			IMAGE_SKILL2,
			IMAGE_COMMAND,
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			IMAGE_SKILL3,
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
			IMAGE_SKILLBOX,
			IMAGE_SKILLBOX_USE,
		//	IMAGE_NON_SKILL1,
		//	IMAGE_NON_SKILL2,
			IMAGE_NON_COMMAND,
#ifdef PBG_ADD_NEWCHAR_MONK_SKILL
			IMAGE_NON_SKILL3,
#endif //PBG_ADD_NEWCHAR_MONK_SKILL
#if CLIENT_VERSION >= 1001
			IMAGE_SKILLBOX_LARGE = BITMAP_NEW_SKILL_BOX_LARGE,
			IMAGE_SKILL_USE,
			IMAGE_SKILL_PAGE,
			IMAGE_SKILL_ARROW_UP,
			IMAGE_SKILL_ARROW_DOWN,
#endif
		};

		CNewUISkillList();
		virtual ~CNewUISkillList();

		bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng);
#if CLIENT_VERSION >= 1001
		void SetButtonInfo();
#endif
		void Release();

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		void RenderSkillInfo();
		float GetLayerDepth();		// 10.6f
		
		WORD GetHeroPriorSkill();
		void SetHeroPriorSkill(BYTE bySkill);

		void Reset();

		void SetHotKey(int iHotKey, int iSkillType);
		int GetHotKey(int iHotKey);
		int GetSkillIndex(int iSkillType);
		void RenderCurrentSkillAndHotSkillList();

		bool IsSkillListUp();
#if CLIENT_VERSION >= 1001
		bool IsSkillShopOpen();
		void CloseSkillShop();
#endif
		void ReceiveSkillShopList(BYTE* lpMsg);

		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);
		
	private:
		void LoadImages();
		void UnloadImages();
		bool IsArrayUp(BYTE bySkill);
		bool IsArrayIn(BYTE bySkill);
		void UseHotKey(int iHotKey);
		
		void RenderSkillIcon(int iIndex, float x, float y, float width, float height);
		void RenderSkillShopIconBySkillType(int iSkillType, float x, float y, float width, float height);
		void RenderSkillShopBuffIconByEffect(int iEffect, float x, float y, float width, float height);
		void RenderSkillShopBuffTooltip(int buffIndex);
		void RenderSkillDelay(int iIndex, float x, float y, float width, float height);
		void RenderPetSkill();
		void BuildSkillShopSkillList(std::vector<int>& skillList);
		bool UpdateSkillShopMouseEvent();
		void RenderSkillShopWindow();
		void RequestSkillShopList();
		void SendSkillShopBuyBuff(BYTE index);
		const char* GetSkillShopCurrencyName(BYTE currency);

		void ResetMouseLButton();
		
	private:
		struct SKILL_SHOP_BUFF_INFO
		{
			BYTE Index;
			BYTE Enabled;
			BYTE Currency;
			BYTE AllowSafeZone;
			WORD Skill;
			WORD Effect;
			DWORD Time;
			DWORD Value1;
			DWORD Value2;
			DWORD Value3;
			DWORD Value4;
			DWORD Price;
			char Name[24];
		};

		CNewUIManager*		m_pNewUIMng;
		CNewUI3DRenderMng*	m_pNewUI3DRenderMng;

		bool m_bHotKeySkillListUp;
		int m_iHotKeySkillType[SKILLHOTKEY_COUNT];

		bool m_bSkillList;

		bool m_bRenderSkillInfo;
		int m_iRenderSkillInfoType;
		int m_iRenderSkillInfoPosX;
		int m_iRenderSkillInfoPosY;
#if CLIENT_VERSION >= 1001
		int m_SkillPage;
		int m_SkillShopBuffPage;
		POINT m_SkillShopPos;
		POINT m_SkillShopDragOffset;
		bool m_SkillShopDragging;
		bool m_SkillShopConfigLoaded;
		DWORD m_SkillShopLastConfigRequestTick;
		DWORD m_SkillShopLastBuyRequestTick;
		BYTE m_SkillShopBuffCount;
		SKILL_SHOP_BUFF_INFO m_SkillShopBuffs[SKILL_SHOP_MAX_BUFF];

		CCustomButtons m_SkillListButtonUp;
		CCustomButtons m_SkillListButtonDown;
#endif

		EVENT_STATE m_EventState;
		WORD m_wHeroPriorSkill;
	};

	class CNewUIMainFrameWindow	: public CNewUIObj, public INewUI3DRenderObj
	{
	public:
		enum IMAGE_LIST
		{
			NEW_FRAME,	// newui_menu01.jpg
			IMAGE_MENU_1 = BITMAP_INTERFACE_NEW_MAINFRAME_BEGIN,	// newui_menu01.jpg
			IMAGE_MENU_2,		// newui_menu02.jpg
			IMAGE_MENU_3,		// newui_menu03.jpg
			IMAGE_MENU_2_1,
			IMAGE_GAUGE_BLUE,	// newui_menu_blue.tga
			IMAGE_GAUGE_GREEN,	// newui_menu_green.tga
			IMAGE_GAUGE_RED,	// newui_menu_red.tga
			IMAGE_GAUGE_AG,		// newui_menu_AG.tga
			IMAGE_GAUGE_SD,		// newui_menu_SD.tga
			IMAGE_GAUGE_EXBAR,	// newui_Exbar.jpg
			IMAGE_MASTER_GAUGE_BAR,	// Exbar_Master.jpg
#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
			IMAGE_MENU_BTN_CSHOP,
#endif //defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
			IMAGE_MENU_BTN_CHAINFO,
			IMAGE_MENU_BTN_MYINVEN,
			IMAGE_MENU_BTN_FRIEND,
			IMAGE_MENU_BTN_WINDOW,

			//ANIMATTED
			IMAGE_GAUGE_BLUE_ANIMATED = IMAGE_GAUGE_BLUE,
			IMAGE_GAUGE_GREEN_ANIMATED = IMAGE_GAUGE_GREEN,
			IMAGE_GAUGE_RED_ANIMATED = IMAGE_GAUGE_RED,
		};

		CNewUIMainFrameWindow();
		virtual ~CNewUIMainFrameWindow();

		bool Create(CNewUIManager* pNewUIMng, CNewUI3DRenderMng* pNewUI3DRenderMng);
		void Release();

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		void Render3D();
		
		bool IsVisible() const;

		float GetLayerDepth();		// 10.2f
		float GetKeyEventOrder();	// 7.f

		void SetItemHotKey(int iHotKey, int iItemType, int iItemLevel);
		int GetItemHotKey(int iHotKey);
		int GetItemHotKeyLevel(int iHotKey);
		void UseHotKeyItemRButton();
		//void RenderHotKeyItems();
		void UpdateItemHotKey();

		void ResetSkillHotKey();
		void SetSkillHotKey(int iHotKey, int iSkillType);
		int GetSkillHotKey(int iHotKey);
		int GetSkillHotKeyIndex(int iSkillType);

		void SetPreExp_Wide(__int64 dwPreExp);
		void SetGetExp_Wide(__int64 dwGetExp);

		void SetPreExp(DWORD dwPreExp);
		void SetGetExp(DWORD dwGetExp);

		// buttons
		void SetBtnState(int iBtnType, bool bStateDown);
		
		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);
		
	private:
		void SetButtonInfo();

		void LoadImages();
		void UnloadImages();

		bool BtnProcess();

		void RenderFrame();
		void RenderLifeMana();
		void RenderGuageAG();
		void RenderGuageSD();
		void RenderExperience();
		void RenderHotKeyItemCount();
		void RenderButtons();
		void RenderCharInfoButton();
		void RenderFriendButton();
		void RenderFriendButtonState();

	public:
		__int64	m_loPreExp;
		__int64	m_loGetExp;
		
	private:
		CNewUIManager*		m_pNewUIMng;
		CNewUI3DRenderMng*	m_pNewUI3DRenderMng;

		CNewUIItemHotKey m_ItemHotKey;
		
		bool m_bExpEffect;
		DWORD m_dwExpEffectTime;

		DWORD m_dwPreExp;
		DWORD m_dwGetExp;

#ifdef PBG_ADD_INGAMESHOP_UI_MAINFRAME
#if CLIENT_VERSION >= 302 && CLIENT_VERSION <= 502
		CNewUIButton m_BtnCShop;
#endif
#endif //defined PBG_ADD_INGAMESHOP_UI_MAINFRAME
		
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		CNewUIButton m_BtnChaInfo;
		CNewUIButton m_BtnMyInven;
		CNewUIButton m_BtnFriend;
		CNewUIButton m_BtnWindow;
#elif CLIENT_VERSION >= 1001
		CCustomButtons m_BtnChaInfo;
		CCustomButtons m_BtnMyInven;
		CCustomButtons m_BtnFriend;
#endif

#if CLIENT_VERSION <= 302 || CLIENT_VERSION >= 1001
		CCustomButtons m_BtnParty;
#endif

		bool m_bButtonBlink;
	};

}

#endif // !defined(AFX_NEWUIMAINFRAMEWINDOW_H__46A029CA_44A5_4050_9216_FA8A25EC4629__INCLUDED_)
