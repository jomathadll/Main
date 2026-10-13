// NewUICharacterInfoWindow.h: interface for the CNewUICharacterInfoWindow class.

#pragma once

#include "NewUIBase.h"
#include "NewUIManager.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIButton.h"
#if CLIENT_VERSION >= 1001
#include "CustomButtons.h"
#endif

namespace SEASON3B
{

	class CNewUICharacterInfoWindow : public CNewUIObj  
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_CHAINFO_BACK = CNewUIMessageBoxMng::IMAGE_MSGBOX_BACK,			// newui_msgbox_back.jpg
			IMAGE_CHAINFO_TOP = CNewUIMyInventory::IMAGE_INVENTORY_BACK_TOP2,		// newui_item_back04.tga	(190,64)
			IMAGE_CHAINFO_LEFT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_LEFT,		// newui_item_back02-l.tga	(21,320)
			IMAGE_CHAINFO_RIGHT = CNewUIMyInventory::IMAGE_INVENTORY_BACK_RIGHT,	// newui_item_back02-r.tga	(21,320)
			IMAGE_CHAINFO_BOTTOM = CNewUIMyInventory::IMAGE_INVENTORY_BACK_BOTTOM,	// newui_item_back03.tga	(190,45)

			IMAGE_CHAINFO_TABLE_TOP_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_LEFT,	//. newui_item_table01(L).tga (14,14)
			IMAGE_CHAINFO_TABLE_TOP_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_RIGHT,	//. newui_item_table01(R).tga (14,14)
			IMAGE_CHAINFO_TABLE_BOTTOM_LEFT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_LEFT,	//. newui_item_table02(L).tga (14,14)
			IMAGE_CHAINFO_TABLE_BOTTOM_RIGHT = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_RIGHT,	//. newui_item_table02(R).tga (14,14)
			IMAGE_CHAINFO_TABLE_TOP_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_TOP_PIXEL,			//. newui_item_table03(up).tga (1, 14)
			IMAGE_CHAINFO_TABLE_BOTTOM_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_BOTTOM_PIXEL,	//. newui_item_table03(dw).tga (1,14)
			IMAGE_CHAINFO_TABLE_LEFT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_LEFT_PIXEL,		//. newui_item_table03(L).tga (14,1)
			IMAGE_CHAINFO_TABLE_RIGHT_PIXEL = CNewUIInventoryCtrl::IMAGE_ITEM_TABLE_RIGHT_PIXEL,		//. newui_item_table03(R).tga (14,1)
#if CLIENT_VERSION >= 1001
			IMAGE_CHAINFO_BACK_VOID = BITMAP_NEW_DEFAULT_WINDOW_BACK,
			IMAGE_CHAINFO_FRAME_BOX,
			IMAGE_CHAINFO_FRAME_BOX_LARGE,
			IMAGE_CHAINFO_POINTS_BAR,
#endif
			IMAGE_CHAINFO_BTN_EXIT = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,

			IMAGE_CHAINFO_TEXTBOX = BITMAP_INTERFACE_NEW_CHAINFO_WINDOW_BEGIN,
			IMAGE_CHAINFO_BTN_STAT,
			IMAGE_CHAINFO_BTN_QUEST,
			IMAGE_CHAINFO_BTN_PET,
			IMAGE_CHAINFO_BTN_MASTERLEVEL,
		};

		enum
		{
			CHAINFO_WINDOW_WIDTH = 190,
			CHAINFO_WINDOW_HEIGHT = 429,
			HEIGHT_STRENGTH = 120,
			HEIGHT_DEXTERITY = 175,
			HEIGHT_VITALITY = 240,
			HEIGHT_ENERGY = 295,
			HEIGHT_CHARISMA = 350,
			BTN_STAT_COUNT = 5,
			STAT_STRENGTH = 0,
			STAT_DEXTERITY,
			STAT_VITALITY,
			STAT_ENERGY,
			STAT_CHARISMA,
		};
		
	private:
		CNewUIManager*				m_pNewUIMng;
		POINT						m_Pos;
		CNewUI3DRenderMng*			m_pNewUI3DRenderMng;
		

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
		CNewUIButton m_BtnStat[BTN_STAT_COUNT];
		CNewUIButton m_BtnExit;
		CNewUIButton m_BtnQuest;
		CNewUIButton m_BtnPet;
		CNewUIButton m_BtnMasterLevel;
#elif CLIENT_VERSION >= 1001
		CCustomButtons m_BtnStat[BTN_STAT_COUNT];
		CCustomButtons m_BtnExit;
		CCustomButtons m_BtnQuest;
		CCustomButtons m_BtnPet;
		CCustomButtons m_BtnMasterLevel;
		cMoveWindow	   m_MoveWindow;
#endif

	public:
		CNewUICharacterInfoWindow();
		virtual ~CNewUICharacterInfoWindow();
		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();
		void SetPos(int x, int y);
		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();
		float GetLayerDepth();	//. 5.1f
		void OpenningProcess();
		void ResetEquipmentLevel();
		
	private:
		void LoadImages();
		void UnloadImages();
		void SetButtonInfo();
		bool BtnProcess();
		void RenderFrame();
		void RenderTexts();
		void RenderSubjectTexts();
		void RenderTableTexts();
		void RenderAttribute();
		void RenderButtons();
		void ResetMouseRButton();
		float m_fOriginalDepth;  // Para armazenar a profundidade original
		float m_fBroughtForwardDepth; // Profundidade quando trazida para a frente
		bool m_bIsBroughtForward; // Sinalizar para rastrear se antecipado
	};


}