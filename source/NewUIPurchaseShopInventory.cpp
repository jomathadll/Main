// NewUIPurchaseShopInventory.cpp: implementation of the CNewUIPurchaseShopInventory class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewUIPurchaseShopInventory.h"
#include "NewUISystem.h"
#include "NewUICustomMessageBox.h"
#include "wsclientinline.h"
#include "PersonalShopTitleImp.h"



namespace
{
	void RenderText( const char* text, int x, int y, int sx, int sy, DWORD color, DWORD backcolor, int sort, HFONT hFont = g_hFont )
	{
		g_pRenderText->SetFont(hFont);

		DWORD backuptextcolor = g_pRenderText->GetTextColor();
		DWORD backuptextbackcolor = g_pRenderText->GetBgColor();

		g_pRenderText->SetTextColor(color);
		g_pRenderText->SetBgColor(backcolor);
		g_pRenderText->RenderText(x, y, text, sx, sy, sort);

		g_pRenderText->SetTextColor(backuptextcolor);
		g_pRenderText->SetBgColor(backuptextbackcolor);
	}
};

using namespace SEASON3B;

SEASON3B::CNewUIPurchaseShopInventory::CNewUIPurchaseShopInventory() : m_pNewUIMng( NULL ), m_pNewInventoryCtrl( NULL )
{
	m_Pos.x = m_Pos.y = 0;
	m_ShopCharacterIndex = -1;
}

SEASON3B::CNewUIPurchaseShopInventory::~CNewUIPurchaseShopInventory()
{
	Release();
}

bool SEASON3B::CNewUIPurchaseShopInventory::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng || NULL == g_pNewUI3DRenderMng || NULL == g_pNewItemMng)
		return false;

	LoadImages();

	SetPos(x, y);

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_PURCHASESHOP_INVENTORY, this);

	m_pNewInventoryCtrl = new CNewUIInventoryCtrl;
	if(false == m_pNewInventoryCtrl->Create(g_pNewUI3DRenderMng, g_pNewItemMng, this, m_Pos.x+16, m_Pos.y+90, 8, 4))
	{
		SAFE_DELETE(m_pNewInventoryCtrl);
		return false;
	}

	m_pNewInventoryCtrl->SetToolTipType( TOOLTIP_TYPE_PURCHASE_SHOP );
	m_pNewInventoryCtrl->LockInventory();
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_Button = new CNewUIButton;
	m_Button->ChangeButtonInfo( m_Pos.x+13, m_Pos.y+391, 36, 29 );
#elif CLIENT_VERSION >= 1001
	m_Button = new CCustomButtons;
	m_Button->ChangeButtonInfo(m_Pos.x + 23, m_Pos.y + 392, 25, 24);
	m_Button->ChangeToolTipText(GlobalText[1108], true);
#endif
	m_Button->ChangeButtonImgState( true, IMAGE_INVENTORY_EXIT_BTN, false );
	Show(false);

	return true;
}

const POINT& SEASON3B::CNewUIPurchaseShopInventory::GetPos() const
{
	return m_Pos;
}

void SEASON3B::CNewUIPurchaseShopInventory::Release()
{
	SAFE_DELETE(m_Button);

	SAFE_DELETE(m_pNewInventoryCtrl);

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}

	UnloadImages();
}

bool SEASON3B::CNewUIPurchaseShopInventory::InsertItem(int iIndex, BYTE* pbyItemPacket)
{
	if(m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->AddItem(iIndex, pbyItemPacket);
	}

	return false;
}

void SEASON3B::CNewUIPurchaseShopInventory::DeleteItem(int iIndex)
{
	if(m_pNewInventoryCtrl)
	{
		ITEM* pItem = m_pNewInventoryCtrl->FindItem(iIndex);

		if(pItem != NULL)
		{
			m_pNewInventoryCtrl->RemoveItem(pItem);
		}
	}
}

ITEM* SEASON3B::CNewUIPurchaseShopInventory::FindItem(int iLinealPos)
{
	if(m_pNewInventoryCtrl)
	{
		return m_pNewInventoryCtrl->FindItem(iLinealPos);
	}

	return NULL;
}

void SEASON3B::CNewUIPurchaseShopInventory::LoadImages()
{



#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_MSGBOX_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back01.tga", IMAGE_INVENTORY_BACK_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_INVENTORY_BACK_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_INVENTORY_BACK_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_INVENTORY_BACK_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\newui_exit_00.tga", IMAGE_INVENTORY_EXIT_BTN, GL_LINEAR);
	LoadBitmap("Interface\\newui_Box_openTitle.tga", IMAGE_MYSHOPINVENTORY_EDIT, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\btn_exit.jpg", IMAGE_INVENTORY_EXIT_BTN, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Interface\\newui_msgbox_back.jpg", IMAGE_MSGBOX_BACK, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back01.tga", IMAGE_INVENTORY_BACK_TOP, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-L.tga", IMAGE_INVENTORY_BACK_LEFT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back02-R.tga", IMAGE_INVENTORY_BACK_RIGHT, GL_LINEAR);
	LoadBitmap("Interface\\newui_item_back03.tga", IMAGE_INVENTORY_BACK_BOTTOM, GL_LINEAR);
	LoadBitmap("Interface\\store_name.jpg", IMAGE_MYSHOPINVENTORY_EDIT, GL_LINEAR);
	LoadBitmap("Interface\\default_window_back.jpg", IMAGE_MYSHOPINVENTORY_BACK_VOID, GL_LINEAR, GL_REPEAT);
#endif
}

void SEASON3B::CNewUIPurchaseShopInventory::UnloadImages()
{
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_EDIT);	
	DeleteBitmap(IMAGE_INVENTORY_EXIT_BTN);
	DeleteBitmap(IMAGE_INVENTORY_BACK_BOTTOM);
	DeleteBitmap(IMAGE_INVENTORY_BACK_RIGHT);
	DeleteBitmap(IMAGE_INVENTORY_BACK_LEFT);
	DeleteBitmap(IMAGE_INVENTORY_BACK_TOP);
	DeleteBitmap(IMAGE_MSGBOX_BACK);
#if CLIENT_VERSION >= 1001
	DeleteBitmap(IMAGE_MYSHOPINVENTORY_BACK_VOID);
#endif
}

bool SEASON3B::CNewUIPurchaseShopInventory::UpdateMouseEvent()
{
	POINT ptExitBtn1 = { m_Pos.x+169, m_Pos.y+7 };
	if(SEASON3B::IsRelease(VK_LBUTTON) && CheckMouseIn(ptExitBtn1.x, ptExitBtn1.y, 13, 12)) 
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PURCHASESHOP_INVENTORY);
		return false;
	}
	if(m_Button->UpdateMouseEvent())
	{
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_PURCHASESHOP_INVENTORY);
		return false;
	}

	if(m_pNewInventoryCtrl)
	{
		if(false == m_pNewInventoryCtrl->UpdateMouseEvent())
		{
			return false;
		}

		if( PurchaseShopInventoryProcess() )
		{
			return false;
		}
	}

	if(CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT))
	{
		if(SEASON3B::IsPress(VK_RBUTTON))
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
			return false;
		}

		if(SEASON3B::IsNone(VK_LBUTTON) == false)
		{
			return false;
		}
	}

	return true;
}

bool SEASON3B::CNewUIPurchaseShopInventory::UpdateKeyEvent()
{
	return true;
}

bool SEASON3B::CNewUIPurchaseShopInventory::PurchaseShopInventoryProcess()
{
	if(m_pNewInventoryCtrl && IsPress(VK_LBUTTON))
	{
		int iCurSquareIndex = m_pNewInventoryCtrl->FindItemptIndex(MouseX, MouseY);
		if( iCurSquareIndex != -1 )
		{
			ChangeSourceIndex(iCurSquareIndex);
			CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CPersonalShopItemBuyMsgBoxLayout));
		}

		return true;
	}

	return false;
}

bool SEASON3B::CNewUIPurchaseShopInventory::Update()
{
	if(m_pNewInventoryCtrl && false == m_pNewInventoryCtrl->Update())
	{
		return false;
	}
	return true;
}

void SEASON3B::CNewUIPurchaseShopInventory::RenderFrame()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	RenderImage(IMAGE_MSGBOX_BACK, m_Pos.x, m_Pos.y, 190.f, 429.f);
	RenderImage(IMAGE_INVENTORY_BACK_TOP, m_Pos.x, m_Pos.y, 190.f, 64.f);
	RenderImage(IMAGE_INVENTORY_BACK_LEFT, m_Pos.x, m_Pos.y+64, 21.f, 320.f);
	RenderImage(IMAGE_INVENTORY_BACK_RIGHT, m_Pos.x+190-21, m_Pos.y+64, 21.f, 320.f);
	RenderImage(IMAGE_INVENTORY_BACK_BOTTOM, m_Pos.x, m_Pos.y+429-45, 190.f, 45.f);
	RenderImage(IMAGE_MYSHOPINVENTORY_EDIT, m_Pos.x+12, m_Pos.y+49, 169.f, 26.f);
#elif CLIENT_VERSION >= 1001
	//RenderBitmap(IMAGE_MYSHOPINVENTORY_BACK_VOID, m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT, 0.0, 0.0, 0.83, 0.935, 1, 1, 0.0);
	RenderImageScaleAuto(IMAGE_MYSHOPINVENTORY_BACK_VOID, m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT);
	//RenderBitmap(IMAGE_MYSHOPINVENTORY_EDIT, m_Pos.x + 28, m_Pos.y + 48, 134, 25, 0.002, 0.0, 0.640, 0.818956, 1, 1, 1.0);
	RenderImageScaleAuto(IMAGE_MYSHOPINVENTORY_EDIT, m_Pos.x + 28, m_Pos.y + 48, 134, 25);
#endif
}

void SEASON3B::CNewUIPurchaseShopInventory::RenderTextInfo()
{
	RenderText(GlobalText[1102], m_Pos.x, m_Pos.y+15, 190, 0, 0xFF49B0FF, 0x00000000, RT3_SORT_CENTER );
	RenderText( m_TitleText.c_str(), m_Pos.x, m_Pos.y+58, 190, 0, RGBA(0, 255, 0, 255), 0x00000000, RT3_SORT_CENTER, g_hFontBold );
	unicode::t_char Text[100];

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[370]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+230, 0, 0, RGBA(255, 45, 47, 255), 0x00000000, RT3_SORT_LEFT, g_hFontBold );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1109]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+250, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1111]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+262, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1112]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+274, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1114]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+286, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1115]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+298, 0, 0, RGBA(247, 206, 77, 255), 0x00000000, RT3_SORT_LEFT );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1134]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+320, 0, 0, RGBA(255, 45, 47, 255), 0x00000000, RT3_SORT_LEFT, g_hFontBold );

	memset(&Text, 0, sizeof(unicode::t_char)*100);
	sprintf(Text, GlobalText[1135]); 
	RenderText( Text, m_Pos.x+30, m_Pos.y+332, 0, 0, RGBA(255, 45, 47, 255), 0x00000000, RT3_SORT_LEFT, g_hFontBold );
}

bool SEASON3B::CNewUIPurchaseShopInventory::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	RenderFrame();

	RenderTextInfo();

	if(m_pNewInventoryCtrl)
	{
		m_pNewInventoryCtrl->Render();
	}

	m_Button->Render();

	DisableAlphaBlend();

	return true;
}

void SEASON3B::CNewUIPurchaseShopInventory::ClosingProcess()
{
 	if( m_pNewInventoryCtrl )
 	{
 		m_pNewInventoryCtrl->RemoveAllItems();
		g_ErrorReport.Write("@ [Notice] CNewUIPurchaseShopInventory::ClosingProcess():m_pNewInventoryCtrl->RemoveAllItems(); )\n" );
 	}

	m_ShopCharacterIndex = -1;

	g_pMyInventory->ChangeMyShopButtonStateOpen();
}

int SEASON3B::CNewUIPurchaseShopInventory::GetPointedItemIndex()
{
	int iPointedItemIndex = m_pNewInventoryCtrl->GetPointedSquareIndex();
	
	if(iPointedItemIndex != -1)
	{
		iPointedItemIndex += MAX_MY_INVENTORY_INDEX;
	}
	
	return iPointedItemIndex;
}

