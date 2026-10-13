// MsgBoxIGSGiftStorageItemInfo.cpp: implementation of the CMsgBoxIGSGiftStorageItemInfo class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"


#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM

#include "MsgBoxIGSGiftStorageItemInfo.h"

#include "DSPlaySound.h"
#include "wsclientinline.h"
#include "MsgBoxIGSUseItemConfirm.h"
#include "WideScreen.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMsgBoxIGSGiftStorageItemInfo::CMsgBoxIGSGiftStorageItemInfo()
{
	m_iStorageSeq	= 0;		// 보관함 순번
	m_iStorageItemSeq = 0;		// 보관함 상품 순번
	m_wItemCode		= -1;		// 아이템 코드
	
	m_szName[0]		= '\0';		// 아이템 이름
	m_szNum[0]		= '\0';
	m_szPeriod[0]	= '\0';
	m_szItemType	= '\0';		// 상품구분 (C : 캐시, P : 상품)
	
	m_szIDInfo[0]	= '\0';		// 선물보낸 캐릭터ID
	m_szMessage[0]	= '\0';		// 선물보낸 메세지
}

CMsgBoxIGSGiftStorageItemInfo::~CMsgBoxIGSGiftStorageItemInfo()
{
	Release();
}

//--------------------------------------------
// Create
bool CMsgBoxIGSGiftStorageItemInfo::Create(float fPriority)
{
	LoadImages();
	SetAddCallbackFunc();
	
	CNewUIMessageBoxBase::Create((gWideScreen.GetAddPosition(IMAGE_IGS_WINDOW_WIDTH)/2)-(IMAGE_IGS_FRAME_WIDTH/2),
		(IMAGE_IGS_WINDOW_HEIGHT/2)-(IMAGE_IGS_FRAME_HEIGHT/2), 
		IMAGE_IGS_FRAME_WIDTH, IMAGE_IGS_FRAME_HEIGHT, fPriority);
	
	if(g_pNewUI3DRenderMng)
	{
		g_pNewUI3DRenderMng->Add3DRenderObj(this);
	}

	SetButtonInfo();

	SetMsgBackOpacity();

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	// 메세지 Input Box
	m_MessageInputBox.SetMultiline(TRUE);
	m_MessageInputBox.Init(g_hWnd, IMAGE_IGS_FRAME_WIDTH - 30, 100, 50);
	m_MessageInputBox.SetPosition(GetPos().x + 22, GetPos().y + IGS_MESSAGE_INPUT_TEXT_POS_Y + 96);
	m_MessageInputBox.SetUseScrollbar(FALSE);
	m_MessageInputBox.SetTextLimit(MAX_GIFT_MESSAGE_SIZE);
	m_MessageInputBox.SetFont(g_hFont);
	//m_MessageInputBox.SetOption(UIOPTION_NULL);
	m_MessageInputBox.SetBackColor(0, 0, 0, 0);
	m_MessageInputBox.SetState(UISTATE_NORMAL);
	m_MessageInputBox.SetTextColor(255, 0, 0, 0);
#elif CLIENT_VERSION >= 1001
	// 메세지 Input Box
	m_MessageInputBox.SetMultiline(TRUE);
	m_MessageInputBox.Init(g_hWnd, IMAGE_IGS_FRAME_WIDTH - 30, 90, 50);
	m_MessageInputBox.SetPosition(GetPos().x + 20, GetPos().y + IGS_MESSAGE_INPUT_TEXT_POS_Y + 95);
	m_MessageInputBox.SetUseScrollbar(FALSE);
	m_MessageInputBox.SetTextLimit(MAX_GIFT_MESSAGE_SIZE);
	m_MessageInputBox.SetFont(g_hFont);
	m_MessageInputBox.SetBackColor(0, 0, 0, 0);
	m_MessageInputBox.SetState(UISTATE_NORMAL);
	m_MessageInputBox.SetTextColor(255, 255, 255, 255);
#endif

	
	return true;
}

//--------------------------------------------
// IsVisible
bool CMsgBoxIGSGiftStorageItemInfo::IsVisible() const
{
	return true;
}


//--------------------------------------------
// Initialize
void CMsgBoxIGSGiftStorageItemInfo::Initialize(int iStorageSeq, int iStorageItemSeq, WORD wItemCode, unicode::t_char szItemType, 
											   unicode::t_char* pszID, unicode::t_char* pszMessage, unicode::t_char* pszName, 
											   unicode::t_char* pszNum, unicode::t_char* pszPeriod)
{
	m_iStorageSeq		= iStorageSeq;
	m_iStorageItemSeq	= iStorageItemSeq;
	m_wItemCode			= wItemCode;
	m_szItemType		= szItemType;
	
	// Name
	strcpy(m_szName, pszName);

	// Num
	sprintf(m_szNum, GlobalText[3040], pszNum);		// "수량 : %s"

	// Period
	sprintf(m_szPeriod, GlobalText[3039], pszPeriod);	// "기간 : %s"

	// ID Info
	// "\'%s\' 님이 보낸 선물입니다."
	sprintf(m_szIDInfo, GlobalText[3041], pszID);

	m_MessageInputBox.SetText(pszMessage);
}

//--------------------------------------------
// Release
void CMsgBoxIGSGiftStorageItemInfo::Release()
{
	CNewUIMessageBoxBase::Release();
	
	if(g_pNewUI3DRenderMng)
		g_pNewUI3DRenderMng->Remove3DRenderObj(this);

	UnloadImages();
}

//--------------------------------------------
// Update
bool CMsgBoxIGSGiftStorageItemInfo::Update()
{
	m_BtnUse.Update();
	m_BtnCancel.Update();
	
	return true;
}

//--------------------------------------------
// Render
bool CMsgBoxIGSGiftStorageItemInfo::Render()
{
	EnableAlphaTest();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	RenderMsgBackColor(true);

	RenderFrame();
	RenderTexts();
	RenderButtons();

	m_MessageInputBox.Render();

	DisableAlphaBlend();
	return true;
}

//--------------------------------------------
// Render3D
void CMsgBoxIGSGiftStorageItemInfo::Render3D()
{
	if(m_wItemCode == 65535)
		return;

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	RenderItem3D(GetPos().x + IGS_3DITEM_POS_X, GetPos().y + IGS_3DITEM_POS_Y, IGS_3DITEM_WIDTH, IGS_3DITEM_HEIGHT, m_wItemCode, 0, 0, 0, true);
#elif CLIENT_VERSION >= 1001
	RenderItem3D(GetPos().x + IGS_3DITEM_POS_X, GetPos().y + IGS_3DITEM_POS_Y-15, IGS_3DITEM_WIDTH, IGS_3DITEM_HEIGHT, m_wItemCode, 0, 0, 0, true);
#endif
}

//--------------------------------------------
// SetAddCallbackFunc
void CMsgBoxIGSGiftStorageItemInfo::SetAddCallbackFunc()
{
	AddCallbackFunc(CMsgBoxIGSGiftStorageItemInfo::LButtonUp, MSGBOX_EVENT_MOUSE_LBUTTON_UP);
	AddCallbackFunc(CMsgBoxIGSGiftStorageItemInfo::OKButtonDown, MSGBOX_EVENT_USER_COMMON_OK);
	AddCallbackFunc(CMsgBoxIGSGiftStorageItemInfo::CancelButtonDown, MSGBOX_EVENT_USER_COMMON_CANCEL);
}

//--------------------------------------------
// LButtonUp
CALLBACK_RESULT CMsgBoxIGSGiftStorageItemInfo::LButtonUp(class CNewUIMessageBoxBase* pOwner, const leaf::xstreambuf& xParam)
{
	CMsgBoxIGSGiftStorageItemInfo* pOwnMsgBox = dynamic_cast<CMsgBoxIGSGiftStorageItemInfo*>(pOwner);

	if(pOwnMsgBox)
	{
		if(pOwnMsgBox->m_BtnUse.IsMouseIn() == true)
		{
			g_MessageBox->SendEvent(pOwner, MSGBOX_EVENT_USER_COMMON_OK);
			return CALLBACK_BREAK;
		}

		if(pOwnMsgBox->m_BtnCancel.IsMouseIn() == true)
		{
			g_MessageBox->SendEvent(pOwner, MSGBOX_EVENT_USER_COMMON_CANCEL);
			return CALLBACK_BREAK;
		}
	}

	return CALLBACK_CONTINUE;
}

//--------------------------------------------
// OKButtonDown
CALLBACK_RESULT CMsgBoxIGSGiftStorageItemInfo::OKButtonDown(class CNewUIMessageBoxBase* pOwner, const leaf::xstreambuf& xParam)
{
	CMsgBoxIGSGiftStorageItemInfo* pOwnMsgBox = dynamic_cast<CMsgBoxIGSGiftStorageItemInfo*>(pOwner);

	// 사용하기 확인 창
	CMsgBoxIGSUseItemConfirm* pMsgBox = NULL;
	CreateMessageBox(MSGBOX_LAYOUT_CLASS(CMsgBoxIGSUseItemConfirmLayout), &pMsgBox);
	pMsgBox->Initialize(pOwnMsgBox->m_iStorageSeq, pOwnMsgBox->m_iStorageItemSeq, 
							pOwnMsgBox->m_wItemCode, pOwnMsgBox->m_szItemType, pOwnMsgBox->m_szName);

	PlayBuffer(SOUND_CLICK01);
	g_MessageBox->SendEvent(pOwner, MSGBOX_EVENT_DESTROY);

	return CALLBACK_BREAK;
}

//--------------------------------------------
// CancelButtonDown
CALLBACK_RESULT CMsgBoxIGSGiftStorageItemInfo::CancelButtonDown(class CNewUIMessageBoxBase* pOwner, const leaf::xstreambuf& xParam)
{
	PlayBuffer(SOUND_CLICK01);
	g_MessageBox->SendEvent(pOwner, MSGBOX_EVENT_DESTROY);
	
	return CALLBACK_BREAK;
}

//--------------------------------------------
// SetButtonInfo
void CMsgBoxIGSGiftStorageItemInfo::SetButtonInfo()
{
		


#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	m_BtnUse.SetInfo(IMAGE_IGS_BUTTON, GetPos().x + IGS_BTN_OK_POS_X, GetPos().y + IGS_BTN_POS_Y + 102, IMAGE_IGS_BTN_WIDTH, IMAGE_IGS_BTN_HEIGHT, CNewUIMessageBoxButton::MSGBOX_BTN_CUSTOM, true);
	m_BtnUse.MoveTextPos(0, -1);
	m_BtnUse.SetText(GlobalText[228]);

	m_BtnCancel.SetInfo(IMAGE_IGS_BUTTON, GetPos().x + IGS_BTN_CANCEL_POS_X, GetPos().y + IGS_BTN_POS_Y + 102, IMAGE_IGS_BTN_WIDTH, IMAGE_IGS_BTN_HEIGHT, CNewUIMessageBoxButton::MSGBOX_BTN_CUSTOM, true);
	m_BtnCancel.MoveTextPos(0, -1);
	m_BtnCancel.SetText(GlobalText[229]);
#elif CLIENT_VERSION >= 1001
	m_BtnUse.SetInfo(IMAGE_IGS_BUTTON, GetPos().x + IGS_BTN_OK_POS_X, GetPos().y + IGS_BTN_POS_Y + 122, IMAGE_IGS_BTN_WIDTH, IMAGE_IGS_BTN_HEIGHT);
	m_BtnUse.SetButtonSize(1);
	m_BtnUse.SetText(GlobalText[228]);

	m_BtnCancel.SetInfo(IMAGE_IGS_BUTTON, GetPos().x + IGS_BTN_CANCEL_POS_X, GetPos().y + IGS_BTN_POS_Y + 122, IMAGE_IGS_BTN_WIDTH, IMAGE_IGS_BTN_HEIGHT);
	m_BtnCancel.SetButtonSize(1);
	m_BtnCancel.SetText(GlobalText[229]);
#endif
}

//--------------------------------------------
// RenderFrame
void CMsgBoxIGSGiftStorageItemInfo::RenderFrame()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	RenderImage(IMAGE_IGS_FRAME, GetPos().x, GetPos().y, IMAGE_IGS_FRAME_WIDTH, IMAGE_IGS_FRAME_HEIGHT);
#elif CLIENT_VERSION >= 1001
	float x, y, width;
	int height;

	x = GetPos().x;
	y = GetPos().y - 35;
	width = IMAGE_IGS_FRAME_WIDTH;
	height = 8;

	if (height <= 0) {
		height = 1;
	}

	float newH = (80.f / 460.f) * width;

	RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_TOP_TITLEBAR, x, y, width, newH, 0.001, 0.001, 0.89692983, 0.62088972, 1, 1, 1.0);
	float endY = y;
	if (height > 0) {
		for (int i = 1; i <= height; i++) {
			endY = y + newH * i;
			RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_MIDDLE, x, endY, width, newH + 0.5, 0.001, 0.005, 0.89692983, 0.61988972, 1, 1, 1.0);
		}
	}
	RenderBitmap(CNewUIMessageBoxMng::IMAGE_MSGBOX_BOTTOM, x, endY + newH, width, newH, 0.001, 0.005, 0.89692983, 0.62088972, 1, 1, 1.0);

	RenderImageScale(IMAGE_IGS_FRAME, x + ((IMAGE_IGS_FRAME_WIDTH / 2) - (95 / 2)), y + 42, 95, 70, 0.f, 0.f, 330.f, 212.f, 0.7f);
	RenderImageScale(IMAGE_IGS_FRAME, x + 15, y + 128, IMAGE_IGS_FRAME_WIDTH - 30, 60, 0.f, 0.f, 330.f, 212.f, 0.7f);
	RenderImageScale(IMAGE_IGS_FRAME, x + 15, y + 205, IMAGE_IGS_FRAME_WIDTH - 30, 110, 0.f, 0.f, 330.f, 212.f, 0.7f);

	//RenderImageScale(IMAGE_IGS_DECO, x + 15, y + 40, IMAGE_IGS_FRAME_WIDTH - 30, 50, 0.f, 0.f, 330.f, 106.f, 0.7f);
	
	//
	//RenderImageScale(IMAGE_IGS_INPUTTEXT, x + 115, y + 95, 80, 20, 0.f, 0.f, 330.f, 53.f, 0.7f);
#endif
}

//--------------------------------------------
// RenderTexts
void CMsgBoxIGSGiftStorageItemInfo::RenderTexts()
{
	
	
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->SetFont(g_hFontBold);

	// Title "선물 정보창" 
	g_pRenderText->RenderText(GetPos().x, GetPos().y + IGS_TEXT_TITLE_POS_Y, GlobalText[3048], IMAGE_IGS_FRAME_WIDTH, 0, RT3_SORT_CENTER);

	// Item Name
	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->RenderText(GetPos().x, GetPos().y + IGS_TEXT_ITEM_NAME_POS_Y, m_szName, IMAGE_IGS_FRAME_WIDTH, 0, RT3_SORT_CENTER);

	// ID Info
	g_pRenderText->SetTextColor(0, 0, 0, 255);
	g_pRenderText->RenderText(GetPos().x, GetPos().y + IGS_TEXT_ID_INFO_POS_Y, m_szIDInfo, IMAGE_IGS_FRAME_WIDTH, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	// Item Info
	g_pRenderText->RenderText(GetPos().x + IGS_TEXT_ITEM_INFO_POS_X, GetPos().y + IGS_TEXT_ITEM_INFO_NUM_POS_Y, m_szNum, IGS_TEXT_ITEM_INFO_WIDTH, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(GetPos().x + IGS_TEXT_ITEM_INFO_POS_X, GetPos().y + IGS_TEXT_ITEM_INFO_PERIOD_POS_Y, m_szPeriod, IGS_TEXT_ITEM_INFO_WIDTH, 0, RT3_SORT_LEFT);
#elif CLIENT_VERSION >= 1001
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(255, 128, 0, 255);
	g_pRenderText->SetFont(g_hFontBold);

	// Title "선물 정보창" 
	g_pRenderText->RenderText(GetPos().x, GetPos().y + IGS_TEXT_TITLE_POS_Y-30, GlobalText[3048], IMAGE_IGS_FRAME_WIDTH, 0, RT3_SORT_CENTER);

	// Item Name
	g_pRenderText->SetTextColor(255, 255, 0, 255);
	g_pRenderText->RenderText(GetPos().x, GetPos().y + IGS_TEXT_ITEM_NAME_POS_Y, m_szName, IMAGE_IGS_FRAME_WIDTH, 0, RT3_SORT_CENTER);

	// ID Info
	g_pRenderText->RenderText(GetPos().x, GetPos().y + IGS_TEXT_ID_INFO_POS_Y, m_szIDInfo, IMAGE_IGS_FRAME_WIDTH, 0, RT3_SORT_CENTER);

	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetTextColor(255, 255, 255, 255);

	// Item Info
	g_pRenderText->RenderText(GetPos().x + IGS_TEXT_ITEM_INFO_POS_X, GetPos().y + IGS_TEXT_ITEM_INFO_NUM_POS_Y-5, m_szNum, IGS_TEXT_ITEM_INFO_WIDTH, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(GetPos().x + IGS_TEXT_ITEM_INFO_POS_X, GetPos().y + IGS_TEXT_ITEM_INFO_PERIOD_POS_Y-5, m_szPeriod, IGS_TEXT_ITEM_INFO_WIDTH, 0, RT3_SORT_LEFT);
#endif


#ifdef FOR_WORK
	// debug
	unicode::t_char szText[256] = {0,};
	g_pRenderText->SetTextColor(255, 0, 0, 255);
	if( m_wItemCode == 65535 )
	{
		sprintf(szText, "아이템코드가 없습니다.");
	}
	else
	{
		sprintf(szText, "ItemCode : %d (%d, %d)", m_wItemCode, m_wItemCode/MAX_ITEM_INDEX, m_wItemCode%MAX_ITEM_INDEX);
	}
	g_pRenderText->RenderText(GetPos().x+IMAGE_IGS_FRAME_WIDTH, GetPos().y+10, szText, 150, 0, RT3_SORT_LEFT);
	sprintf(szText, "Storage Seq : %d", m_iStorageSeq);
	g_pRenderText->RenderText(GetPos().x+IMAGE_IGS_FRAME_WIDTH, GetPos().y+20, szText, 150, 0, RT3_SORT_LEFT);
	sprintf(szText, "Storage ItemSeq : %d", m_iStorageItemSeq);
	g_pRenderText->RenderText(GetPos().x+IMAGE_IGS_FRAME_WIDTH, GetPos().y+30, szText, 150, 0, RT3_SORT_LEFT);
#endif // FOR_WORK
}

//--------------------------------------------
// RenderButtons
void CMsgBoxIGSGiftStorageItemInfo::RenderButtons()
{
	m_BtnUse.Render();
	m_BtnCancel.Render();
}

//--------------------------------------------
// LoadImages
void CMsgBoxIGSGiftStorageItemInfo::LoadImages()
{
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	LoadBitmap("Interface\\InGameShop\\Ingame_Bt03.tga", IMAGE_IGS_BUTTON, GL_LINEAR);
	LoadBitmap("Interface\\InGameShop\\ingame_Box_List_B.tga", IMAGE_IGS_FRAME, GL_LINEAR);
#elif CLIENT_VERSION >= 1001
	LoadBitmap("Interface\\btn_medium.jpg", IMAGE_IGS_BUTTON, GL_LINEAR);
	LoadBitmap("Interface\\store_name_big.jpg", IMAGE_IGS_FRAME, GL_LINEAR);
#endif
}

//--------------------------------------------
// UnloadImages
void CMsgBoxIGSGiftStorageItemInfo::UnloadImages()
{
	DeleteBitmap(IMAGE_IGS_BUTTON);
	DeleteBitmap(IMAGE_IGS_FRAME);
}

////////////////////////////////////////////////////////////////////
// LayOut
////////////////////////////////////////////////////////////////////
bool CMsgBoxIGSGiftStorageItemInfoLayout::SetLayout()
{
	CMsgBoxIGSGiftStorageItemInfo* pMsgBox = GetMsgBox();
	if(false == pMsgBox)
		return false;
	
	if(false == pMsgBox->Create())
		return false;
	
	return true;
}

#endif KJH_ADD_INGAMESHOP_UI_SYSTEM
